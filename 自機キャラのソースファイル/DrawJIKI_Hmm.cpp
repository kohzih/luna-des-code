/*=============================================================================
	ホーミングミサイルの発射、当たり判定、描画等に関するソースファイル
==============================================================================*/
#include <windows.h>
#include <math.h>
#include <ddraw.h>

#include "Macros.h"
#include "Draw.h"
#include "Struct.h"
#include "DrawJIKI.h"
#include "DrawJIKIDataEXT.h"
#include "DrawMacros.h"

extern LPDIRECTDRAWSURFACE7 lpDDSBack; //バックサーフェス

//////////////////////////////////////////////////////////////////////
//	ホーミングミサイルオブジェクトの初期化
//////////////////////////////////////////////////////////////////////
BOOL DrawJIKI::InitHmm()
{
	//左右から発射されるミサイルのロックオン状況を示すフラグの初期化
	HMMDAT.fHMM[0] = 0;
	HMMDAT.fHMM[1] = 0;

	//////////////////////////////////////////////////////////////////////
	//--ホーミングミサイルのステータスを管理する構造体の初期化------------

	//弾ステータス構造体に値をセットして弾数分確保した、弾オブジェクトを初期化
	HMM1.TTama = 30;			//画面上に存在できる弾の数
	HMM1.WTama = 10;			//弾を発射する間隔
	HMM1.WTamaCnt = HMM1.WTama; //弾発射までのカウンタ

	HMTAMA tama; // HMTAMA構造体に前もって値をセットして初期化する

	tama.fTama = _TOFF;			//弾の初期状態を設定
	tama.STama = 2;				//追尾している敵キャラの座標を新しく取得する間隔
	tama.STamaCnt = tama.STama; //新しく取得する間隔のカウンタ
	tama.LockTekiNum = 0;		//弾のターゲットの敵キャラの番号の初期化
	tama.Kakudo = 0;			//現在の弾の角度の初期化
	tama.TekiKakudo = 0;		//敵キャラと弾の位置との角度の初期化
	tama.Width = 16;			//弾の横幅
	tama.Height = 16;			//弾の縦幅
	tama.Mov.x = 0.0;			//弾の移動速度を初期化
	tama.Mov.y = 0.0;
	tama.dCdn.x = 0; //弾の画面座標計算用
	tama.dCdn.y = 0;
	tama.Cdn.x = 0; //弾の現在の画面座標を0で初期化
	tama.Cdn.y = 0;
	tama.MapCdn.x = 0; //弾の現在の絶対座標も0で初期化
	tama.MapCdn.y = 0;
	//弾アニメーションパターン配列の初期化
	memcpy(&tama.AnimePat[0], &HmmPatJIKI[0], sizeof(BYTE) * 24);
	tama.SrcR.left = 0; //転送元矩形を0で初期化
	tama.SrcR.top = 0;
	tama.SrcR.right = 0;
	tama.SrcR.bottom = 0;
	tama.DesR = tama.SrcR; //転送先矩形も0で初期化

	//発射できる弾数分TAMA構造体を確保
	HMM1.tm = new HMTAMA[HMM1.TTama];
	if (!HMM1.tm)
		return FALSE;
	//確保した領域の先頭アドレスをコピー
	char *pTama;
	pTama = (char *)HMM1.tm;
	//弾オブジェクトを確保した分、初期化
	for (int i = 0; (DWORD)i < HMM1.TTama; i++)
	{
		memcpy(pTama, &tama, sizeof(HMTAMA)); //確保した領域の先頭からコピー
		pTama += sizeof(HMTAMA);			  //次にコピーするところまでアドレスを進める
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	動的に確保するメモリ領域へのポインタをNULLで初期化
//////////////////////////////////////////////////////////////////////
void DrawJIKI::InitHmmTm()
{
	HMM1.tm = NULL;
}

//////////////////////////////////////////////////////////////////////
//	確保したメモリ領域を解放
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmRelease()
{
	RELEASE_dels(HMM1.tm);
}

//////////////////////////////////////////////////////////////////////
//	ホーミングミサイル発射までのカウントを進める
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmCnt()
{
	//自機に一番近い敵キャラの番号と座標の取得開始
	HMMDAT.fHMM[0] |= _LOCKHMM_ON;
	HMMDAT.fHMM[1] |= _LOCKHMM_ON;
	HMMDAT.fHMM[0] &= _LOCKST_OFF;
	HMMDAT.fHMM[1] &= _LOCKST_OFF;

	//ホーミングミサイルの現在のレベルを取得する
	switch (Wpd.flag & _HMM_LVCHK)
	{
	case _HMM1_ON:
		HMM1.WTamaCnt--; //発射までのカウントダウン
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
//	ホーミングミサイル発射までのカウントをリセットさせる
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmCntReset()
{
	//自機に一番近い敵キャラの番号と座標の取得解除
	HMMDAT.fHMM[0] &= _LOCKHMM_OFF;
	HMMDAT.fHMM[1] &= _LOCKHMM_OFF;

	//ホーミングミサイル発射までのカウントをリセット
	switch (Wpd.flag & _HMM_LVCHK)
	{
	case _HMM1_ON:
		HMM1.WTamaCnt = HMM1.WTama; //発射までのカウントリセット
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
//	ホーミングミサイルを全て発射待機状態に戻す
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmReset()
{
	HmmCntReset(); //発射までのカウンタをリセット

	short i;

	//全てのホーミングミサイルの弾フラグを発射待機状態にセット
	switch (Wpd.flag & _HMM_LVCHK)
	{
	case _HMM1_ON:
		for (i = 0; (DWORD)i < HMM1.TTama; i++)
		{
			HMM1.tm[i].fTama = _TOFF; //フラグをクリア
		}
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
//	ロックオンするために自機に一番近い距離にいる敵キャラの情報を取得する
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmGetTekiInfo()
{
	short hsX[2] = {-20, 40}; //ホーミングミサイルを発射する自機左右の砲台の位置
	LONG x = 0, x1 = 0, y = 0, y1 = 0;

	for (short i = 0; i < 2; i++)
	{
		//ロックオンが開始されているかチェック
		if (HMMDAT.fHMM[i] & _LOCKHMM_ON)
		{
			//一番最初に比較の為に必要になる敵キャラの情報を取得
			if (!(HMMDAT.fHMM[i] & _LOCKST_ON))
			{
				HMMDAT.fHMM[i] |= _LOCKST_ON;
				HMMDAT.TekiNum[i] = SP_Num;
				HMMDAT.TekiCdn[i] = SP_Cdn;
				continue;
			}
			//現在格納されている一番自機に近い敵座標と次の敵座標の値を比較するために
			// X座標とY座標の和を求める
			x = HMMDAT.TekiCdn[i].x - (Cdn.x + hsX[i]) < 0 ? -(HMMDAT.TekiCdn[i].x - (Cdn.x + hsX[i])) : HMMDAT.TekiCdn[i].x - (Cdn.x + hsX[i]);
			y = HMMDAT.TekiCdn[i].y - Cdn.y < 0 ? -(HMMDAT.TekiCdn[i].y - Cdn.y) : HMMDAT.TekiCdn[i].y - Cdn.y;
			x1 = SP_Cdn.x - (Cdn.x + hsX[i]) < 0 ? -(SP_Cdn.x - (Cdn.x + hsX[i])) : SP_Cdn.x - (Cdn.x + hsX[i]);
			y1 = SP_Cdn.y - Cdn.y < 0 ? -(SP_Cdn.y - Cdn.y) : SP_Cdn.y - Cdn.y;
			//和の比較、次の敵座標の値のほうが自機に近ければ新しく保存する
			if ((x + y) > (x1 + y1))
			{
				HMMDAT.TekiNum[i] = SP_Num;
				HMMDAT.TekiCdn[i] = SP_Cdn;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	既に発射されたホーミングミサイルの移動量を求める
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmMovCnt()
{
	double X = 0, Y = 0;
	double x = 0, y = 0;

	for (int i = 0; (DWORD)i < HMM1.TTama; i++)
	{
		//ロックオンしている敵キャラを探す
		if (HMM1.tm[i].LockTekiNum == SP_Num && HMM1.tm[i].STamaCnt == 0)
		{
			//新たに敵キャラとの位置関係を調べ直す(キャラの中心点で比較)
			X = (SP_Cdn.x + (SP_Width >> 1)) - (HMM1.tm[i].Cdn.x + (HMM1.tm[i].Width >> 1));
			Y = (SP_Cdn.y + (SP_Height >> 1)) - (HMM1.tm[i].Cdn.y + (HMM1.tm[i].Height >> 1));
			if (X == 0)
			{
				if (Y < 0)
					HMM1.tm[i].TekiKakudo = 90;
				else if (Y > 0)
					HMM1.tm[i].TekiKakudo = 270;
			}
			else if (Y == 0)
			{
				if (X < 0)
					HMM1.tm[i].TekiKakudo = 180;
				else if (X > 0)
					HMM1.tm[i].TekiKakudo = 0;
			}
			else
			{
				//敵キャラと弾との角度を計算
				x = X < 0 ? -X : X;
				y = Y < 0 ? -Y : Y;
				HMM1.tm[i].TekiKakudo = TanKaku(y / x); //タンジェント値を角度に変換
				//求まった角度をそれぞれの象限に対応した角度に変換(第1象限はそのまま)
				if (X < 0)
				{
					if (Y < 0)
						HMM1.tm[i].TekiKakudo = 180 - HMM1.tm[i].TekiKakudo; //第2象限
					else if (Y > 0)
						HMM1.tm[i].TekiKakudo = 180 + HMM1.tm[i].TekiKakudo; //第3象限
				}
				else if (X > 0)
				{
					if (Y > 0)
						HMM1.tm[i].TekiKakudo = 360 - HMM1.tm[i].TekiKakudo; //第4象限
				}
			}
			//弾の移動量を計算(距離の近い方向で回転)
			if (HMM1.tm[i].Kakudo > HMM1.tm[i].TekiKakudo)
			{
				if (HMM1.tm[i].Kakudo - HMM1.tm[i].TekiKakudo > 180)
				{ //反時計周り
					HMM1.tm[i].Kakudo += 15;
				}
				else
				{ //時計周り
					HMM1.tm[i].Kakudo -= 15;
				}
			}
			else if (HMM1.tm[i].Kakudo < HMM1.tm[i].TekiKakudo)
			{
				if (HMM1.tm[i].TekiKakudo - HMM1.tm[i].Kakudo > 180)
				{ //時計周り
					HMM1.tm[i].Kakudo -= 15;
				}
				else
				{ //反時計周り
					HMM1.tm[i].Kakudo += 15;
				}
			}
			//角度の補正(0-345度に限定する処理)
			if (HMM1.tm[i].Kakudo == 360)
				HMM1.tm[i].Kakudo = 0;
			else if (HMM1.tm[i].Kakudo > 360)
				HMM1.tm[i].Kakudo = 15;
			else if (HMM1.tm[i].Kakudo < 0)
				HMM1.tm[i].Kakudo = 345;

			//移動量を計算
			HMM1.tm[i].Mov.x = _HMM_MOV * Cos[HMM1.tm[i].Kakudo];
			HMM1.tm[i].Mov.y = -(_HMM_MOV * Sin[HMM1.tm[i].Kakudo]);
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	ホーミングミサイルと敵キャラとの当たり判定
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmAtari()
{
	short i, j;

	//発射されているホーミングミサイルと敵キャラの当たり判定
	switch (Wpd.flag & _HMM_LVCHK)
	{
	case _HMM1_ON:
		for (i = 0; (DWORD)i < HMM1.TTama; i++)
		{
			//未発射の弾とは当たり判定をしない
			if (HMM1.tm[i].fTama & _TON)
			{
				//敵キャラ
				if (!(Atari(HMM1.tm[i].DesR)))
				{
					//ホーミングミサイルを消滅させる
					HMM1.tm[i].fTama &= _TOFF; //フラグをクリア
					JikiTekiDmg(_HMM1DMG);	   //敵キャラにダメージ
				}

				//破壊可能な弾との当たり判定
				if (SP_Tamas3)
				{
					for (j = 0; (DWORD)j < SP_Tamas3->TTama; j++)
					{
						if (SP_Tamas3->tm3[j].fTama & _TON)
						{
							if (!(Atari(HMM1.tm[i].DesR, SP_Tamas3->tm3[j].DesR)))
							{
								//ホーミングミサイルを消滅させる
								HMM1.tm[i].fTama &= _TOFF; //フラグをクリア
								JikiTamas3Dmg(_HMM1DMG, j);
							}
						}
					}
				}
			}
		}
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
//	ホーミングミサイル発射と移動の処理
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmAttack()
{
	//画面に弾が存在しているかしていないか調べるフラグ
	bool TamaS = FALSE;

	//発射できる状態になっているホーミングミサイルの番号保存用
	static DWORD TTamaNum1 = HMM1.TTama, TTamaNum2 = HMM1.TTama;

	short i;

	switch (Wpd.flag & _HMM_LVCHK)
	{ //装備のレベルを調べている
	case _HMM1_ON:
		if (!HMM1.WTamaCnt)
		{
			//ホーミングミサイルが指定された数以上に画面に存在しないようにする処理
			if (TTamaNum1 < HMM1.TTama && TTamaNum2 < HMM1.TTama)
			{
				//画面上に弾が発射されているフラグオン
				fJIKI |= _SUBHMM_ON;
				//発射中のフラグを立てる
				HMM1.tm[TTamaNum1].fTama |= _TON;
				HMM1.tm[TTamaNum2].fTama |= _TON;
				//追尾カウンタを初期化
				HMM1.tm[TTamaNum1].STamaCnt = HMM1.tm[TTamaNum1].STama;
				HMM1.tm[TTamaNum2].STamaCnt = HMM1.tm[TTamaNum2].STama;
				//ロックオンするべき敵キャラの番号の初期化
				HMM1.tm[TTamaNum1].LockTekiNum = HMMDAT.TekiNum[0];
				HMM1.tm[TTamaNum2].LockTekiNum = HMMDAT.TekiNum[1];
				//発射された角度と移動量の初期化
				HMM1.tm[TTamaNum1].Kakudo = 135;
				HMM1.tm[TTamaNum1].Mov.x = _HMM_MOV * Cos[HMM1.tm[TTamaNum1].Kakudo];
				HMM1.tm[TTamaNum1].Mov.y = -(_HMM_MOV * Sin[HMM1.tm[TTamaNum1].Kakudo]);
				HMM1.tm[TTamaNum2].Kakudo = 45;
				HMM1.tm[TTamaNum2].Mov.x = _HMM_MOV * Cos[HMM1.tm[TTamaNum2].Kakudo];
				HMM1.tm[TTamaNum2].Mov.y = -(_HMM_MOV * Sin[HMM1.tm[TTamaNum2].Kakudo]);
				//発射された座標を初期化
				//画面座標をセット
				HMM1.tm[TTamaNum1].Cdn.x = Cdn.x - (HMM1.tm[TTamaNum1].Width - 2);
				HMM1.tm[TTamaNum1].Cdn.y = Cdn.y + HMM1.tm[TTamaNum1].Height;
				HMM1.tm[TTamaNum2].Cdn.x = (Cdn.x + _JIKI_W) - 2;
				HMM1.tm[TTamaNum2].Cdn.y = Cdn.y + HMM1.tm[TTamaNum2].Height;
				//弾の移動計算用に画面座標を取得
				HMM1.tm[TTamaNum1].dCdn.x = HMM1.tm[TTamaNum1].Cdn.x;
				HMM1.tm[TTamaNum1].dCdn.y = HMM1.tm[TTamaNum1].Cdn.y;
				HMM1.tm[TTamaNum2].dCdn.x = HMM1.tm[TTamaNum2].Cdn.x;
				HMM1.tm[TTamaNum2].dCdn.y = HMM1.tm[TTamaNum2].Cdn.y;
				//絶対座標をセット
				HMM1.tm[TTamaNum1].MapCdn.x = MainCdn.x + HMM1.tm[TTamaNum1].Cdn.x;
				HMM1.tm[TTamaNum1].MapCdn.y = MainCdn.y + HMM1.tm[TTamaNum1].Cdn.y;
				HMM1.tm[TTamaNum2].MapCdn.x = MainCdn.x + HMM1.tm[TTamaNum2].Cdn.x;
				HMM1.tm[TTamaNum2].MapCdn.y = MainCdn.y + HMM1.tm[TTamaNum2].Cdn.y;

				TTamaNum1 = HMM1.TTama; // HMM1.TTamaで初期化
				TTamaNum2 = HMM1.TTama; //この処理はTTamaNumが上書きされない限り
										//新しく弾を発射できないようにしています
			}
			HMM1.WTamaCnt = HMM1.WTama; //発射までのカウンタを再セット
		}

		for (i = 0; (DWORD)i < HMM1.TTama; i++)
		{
			//発射中フラグの立っていない弾は処理しない
			if (HMM1.tm[i].fTama & _TON)
			{
				TamaS = TRUE; //ホーミングミサイルは画面内に存在している

				// STamaCnt=0の時の処理はHmmMovCnt()で使用しています。
				if (!(HMM1.tm[i].STamaCnt))
					HMM1.tm[i].STamaCnt = HMM1.tm[i].STama; //カウンタを再セット

				HMM1.tm[i].STamaCnt--; //敵キャラを追尾する処理へのカウントを進める

				//ホーミングミサイルを移動させる
				HMM1.tm[i].dCdn.x += HMM1.tm[i].Mov.x;
				HMM1.tm[i].dCdn.y += HMM1.tm[i].Mov.y;
				HMM1.tm[i].Cdn.x = (LONG)HMM1.tm[i].dCdn.x;
				HMM1.tm[i].Cdn.y = (LONG)HMM1.tm[i].dCdn.y;
				HMM1.tm[i].MapCdn.x = MainCdn.x + HMM1.tm[i].Cdn.x;
				HMM1.tm[i].MapCdn.y = MainCdn.y + HMM1.tm[i].Cdn.y;

				//ホーミングミサイルが画面の外に移動していないかチェック
				if (HMM1.tm[i].Cdn.x + HMM1.tm[i].Width < 0 ||
					HMM1.tm[i].Cdn.x > WinXMax ||
					HMM1.tm[i].Cdn.y + HMM1.tm[i].Height < 0 ||
					HMM1.tm[i].Cdn.y > WinYMax)
				{
					HMM1.tm[i].fTama = _TOFF; //フラグをクリア
					continue;
				}

				//現在の弾角度に対応したサーフェスからの転送矩形範囲をセット
				HMM1.tm[i].SrcR = TamSPJIKI[HMM1.tm[i].AnimePat[HMM1.tm[i].Kakudo / 15]];

				HMM1.tm[i].DesR.left = HMM1.tm[i].Cdn.x;
				HMM1.tm[i].DesR.top = HMM1.tm[i].Cdn.y;
				HMM1.tm[i].DesR.right = HMM1.tm[i].Cdn.x + HMM1.tm[i].Width;
				HMM1.tm[i].DesR.bottom = HMM1.tm[i].Cdn.y + HMM1.tm[i].Height;

				//画面外の部分は描画しないようにする処理
				if (HMM1.tm[i].DesR.left < 0)
				{
					HMM1.tm[i].SrcR.left = HMM1.tm[i].SrcR.left - HMM1.tm[i].DesR.left;
					HMM1.tm[i].DesR.left = 0;
				}
				else if (HMM1.tm[i].DesR.right > WinXMax)
				{
					HMM1.tm[i].SrcR.right = HMM1.tm[i].SrcR.right - (HMM1.tm[i].DesR.right - WinXMax);
					HMM1.tm[i].DesR.right = WinXMax;
				}
				if (HMM1.tm[i].DesR.top < 0)
				{
					HMM1.tm[i].SrcR.top = HMM1.tm[i].SrcR.top - HMM1.tm[i].DesR.top;
					HMM1.tm[i].DesR.top = 0;
				}
				else if (HMM1.tm[i].DesR.bottom > WinYMax)
				{
					HMM1.tm[i].SrcR.bottom = HMM1.tm[i].SrcR.bottom - (HMM1.tm[i].DesR.bottom - WinYMax);
					HMM1.tm[i].DesR.bottom = WinYMax;
				}
			}
			else if (HMM1.tm[i].fTama == _TOFF)
			{
				//再発射可能なホーミングミサイルの番号を保存する
				if (TTamaNum1 == HMM1.TTama)
				{
					TTamaNum1 = i;
					HMM1.tm[i].fTama |= _TSET; //ホーミングミサイルを再び発射セット
				}
				else if (TTamaNum2 == HMM1.TTama)
				{
					TTamaNum2 = i;
					HMM1.tm[i].fTama |= _TSET;
				}
				else if (!(fJIKI & _SUBHMM_ON))
				{
					break; //画面に弾が存在していなければループを抜ける
				}
			}
		}
		break;
	default:
		break;
	}
	//弾が存在していなかったらホーミングミサイル発射フラグをオフ
	if (!TamaS)
		fJIKI &= _SUBHMM_OFF;
}

//////////////////////////////////////////////////////////////////////
//	ホーミングミサイルを描画する
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmRend()
{
	short i;
	//ホーミングミサイルの描画
	switch (Wpd.flag & _HMM_LVCHK)
	{
	case _HMM1_ON: //ホーミングミサイル１
		for (i = 0; (DWORD)i < HMM1.TTama; i++)
		{
			//発射中フラグの立っていない弾は描画しない
			if (HMM1.tm[i].fTama & _TON)
			{
				lpDDSBack->BltFast(HMM1.tm[i].DesR.left, HMM1.tm[i].DesR.top,
								   lpDDS_JIKI, &HMM1.tm[i].SrcR,
								   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			}
		}
		break;
	default:
		break;
	}
}