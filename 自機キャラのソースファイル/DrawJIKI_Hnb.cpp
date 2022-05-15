/*=============================================================================
	花火の発射、当たり判定、描画等に関するソースファイル
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
//	花火オブジェクトの初期化
//////////////////////////////////////////////////////////////////////
BOOL DrawJIKI::InitHnb()
{
	//----------------花火ステータスを管理する構造体の初期化------------------

	//弾ステータス構造体に値をセットして弾数分確保した、弾オブジェクトを初期化
	HNB1.TTama[0] = 120;		//画面上に存在できる小玉
	HNB1.TTama[1] = 2;			//画面上に存在できる大玉
	HNB1.WTama = 30;			//弾を発射する間隔
	HNB1.WTamaCnt = HMM1.WTama; //弾発射までのカウンタ

	TAMA2 tama2; // TAMA構造体に前もって値をセットして初期化する

	//-----------------小玉の初期化--------------------------------------------

	tama2.fTama = _TOFF; //弾の初期状態を設定
	tama2.Width = 16;	 //弾の横幅
	tama2.Height = 16;	 //弾の縦幅
	tama2.Kakudo = 0;	 //弾の発射角度
	tama2.Time = 0;		 //弾が発射されてからの経過時間
	tama2.Mov.x = 0.0;	 //弾の移動速度を初期化
	tama2.Mov.y = 0.0;
	tama2.dCdn.x = 0; //弾の画面座標計算用
	tama2.dCdn.y = 0;
	tama2.Cdn.x = 0; //弾の現在の画面座標を0で初期化
	tama2.Cdn.y = 0;
	tama2.MapCdn.x = 0; //弾の現在の絶対座標も0で初期化
	tama2.MapCdn.y = 0;
	tama2.Anime = TamAmeJIKI[4]; //弾アニメーションパターン数の初期化
	//弾アニメーションパターン配列の初期化
	memcpy(&tama2.AnimePat[0], &TamPatJIKI[4][0], sizeof(BYTE) * 10);
	//弾アニメーション切り替えタイミング配列の初期化
	memcpy(&tama2.AnimeFrame[0], &TamFrameJIKI[4][0], sizeof(BYTE) * 10);
	tama2.AnimeCnt = 0;					  //最初に表示するアニメーションパターンを初期化
	tama2.FrameCnt = tama2.AnimeFrame[0]; //最初のアニメーションフレームカウントを初期化
	tama2.SrcR.left = 0;				  //転送元矩形を0で初期化
	tama2.SrcR.top = 0;
	tama2.SrcR.right = 0;
	tama2.SrcR.bottom = 0;
	tama2.DesR = tama2.SrcR; //転送先矩形も0で初期化

	char *pTama;
	short i;

	//発射できる弾数分TAMA構造体を確保
	HNB1.tm2[0] = new TAMA2[HNB1.TTama[0]];
	if (!HNB1.tm2[0])
		return FALSE;
	//確保した領域の先頭アドレスをコピー
	pTama = (char *)HNB1.tm2[0];
	//弾オブジェクトを確保した分、初期化
	for (i = 0; (DWORD)i < HNB1.TTama[0]; i++)
	{
		memcpy(pTama, &tama2, sizeof(TAMA2)); //確保した領域の先頭からコピー
		pTama += sizeof(TAMA2);				  //次にコピーするところまでアドレスを進める
	}

	//-----------------大玉の初期化--------------------------------------------

	tama2.Width = 32;			 //弾の横幅
	tama2.Height = 32;			 //弾の縦幅
	tama2.Anime = TamAmeJIKI[3]; //弾アニメーションパターン数の初期化
	//弾アニメーションパターン配列の初期化
	memcpy(&tama2.AnimePat[0], &TamPatJIKI[3][0], sizeof(BYTE) * 10);
	//弾アニメーション切り替えタイミング配列の初期化
	memcpy(&tama2.AnimeFrame[0], &TamFrameJIKI[3][0], sizeof(BYTE) * 10);

	//発射できる弾数分TAMA構造体を確保
	HNB1.tm2[1] = new TAMA2[HNB1.TTama[1]];
	if (!HNB1.tm2[1])
		return FALSE;
	//確保した領域の先頭アドレスをコピー
	pTama = (char *)HNB1.tm2[1];
	//弾オブジェクトを確保した分、初期化
	for (i = 0; (DWORD)i < HNB1.TTama[1]; i++)
	{
		memcpy(pTama, &tama2, sizeof(TAMA2)); //確保した領域の先頭からコピー
		pTama += sizeof(TAMA2);				  //次にコピーするところまでアドレスを進める
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	動的に確保するメモリ領域へのポインタをNULLで初期化
//////////////////////////////////////////////////////////////////////
void DrawJIKI::InitHnbTm()
{
	HNB1.tm2[0] = NULL;
	HNB1.tm2[1] = NULL;
}

//////////////////////////////////////////////////////////////////////
//	確保したメモリ領域を解放
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HnbRelease()
{
	RELEASE_dels(HNB1.tm2[0]);
	RELEASE_dels(HNB1.tm2[1]);
}

//////////////////////////////////////////////////////////////////////
//	花火発射までのカウントを進める
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HnbCnt()
{
	//花火の現在のレベルを取得する
	switch (Wpd.flag & _HNB_LVCHK)
	{
	case _HNB1_ON:
		HNB1.WTamaCnt--; //発射までのカウントダウン
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
//	花火発射までのカウントをリセットさせる
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HnbCntReset()
{
	//花火の現在のレベルを取得する
	switch (Wpd.flag & _HNB_LVCHK)
	{
	case _HNB1_ON:
		HNB1.WTamaCnt = HNB1.WTama; //発射までのカウントリセット
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
//	花火を全て発射待機状態に戻す
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HnbReset()
{
	HnbCntReset(); //発射までのカウンタをリセット

	short i, j;

	//全ての花火の弾フラグを発射待機状態にセット
	switch (Wpd.flag & _HNB_LVCHK)
	{
	case _HNB1_ON:
		for (i = 0; i < 2; i++)
		{
			for (j = 0; (DWORD)j < HNB1.TTama[i]; j++)
			{
				HNB1.tm2[i][j].fTama = _TOFF; //フラグをクリア
			}
		}
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
//	花火発射と移動の処理
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HnbAttack()
{
	//発射できる状態になっている小玉花火の番号保存用
	static DWORD KoDama[60] = {
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0]};
	//発射できる状態になっている大玉花火の番号保存用
	static DWORD ODama[2] = {
		HNB1.TTama[1], HNB1.TTama[1]};

	bool ret = FALSE;					//花火が指定された数発射待機状態になっているか調べるフラグ
	bool TamaS = FALSE;					//画面に弾が存在しているかしていないか調べるフラグ
	short TamaKaku1 = 0, TamaKaku2 = 0; //爆発時の小玉の角度(右と左)
	short i, j;

	//------弾の発射処理-------------------------------------------------------

	switch (Wpd.flag & _HNB_LVCHK)
	{ //装備のレベルを調べている
	case _HNB1_ON:
		if (!HNB1.WTamaCnt)
		{
			//花火が指定された数発射待機状態になっているか調べる
			for (i = 0; i < 60; i++)
			{
				if (KoDama[i] == HNB1.TTama[0])
					ret = TRUE;
			}
			for (i = 0; i < 2; i++)
			{
				if (ODama[i] == HNB1.TTama[1])
					ret = TRUE;
			}
			//指定された数発射待機状態になっていれば発射処理
			if (!ret)
			{
				//画面上に弾が発射されているフラグオン
				fJIKI |= _SUBHNB_ON;

				//弾の発射された座標と角度の初期化
				//小玉
				for (i = 0; i < 60; i++)
				{
					//発射中のフラグを立てる
					HNB1.tm2[0][KoDama[i]].fTama |= _TON;
					//経過時間をクリア
					HNB1.tm2[0][KoDama[i]].Time = 0;
					//弾アニメーションを初期化
					HNB1.tm2[0][KoDama[i]].AnimeCnt = 0;
					HNB1.tm2[0][KoDama[i]].FrameCnt = HNB1.tm2[0][KoDama[i]].AnimeFrame[0];
					//発射される座標の初期化
					if (i < 30)
					{
						HNB1.tm2[0][KoDama[i]].Cdn.x = Cdn.x - 24;
						HNB1.tm2[0][KoDama[i]].Cdn.y = Cdn.y + 8;
						HNB1.tm2[0][KoDama[i]].Kakudo = 100;
					}
					else if (i >= 30)
					{
						HNB1.tm2[0][KoDama[i]].Cdn.x = Cdn.x + (_JIKI_W + 8);
						HNB1.tm2[0][KoDama[i]].Cdn.y = Cdn.y + 8;
						HNB1.tm2[0][KoDama[i]].Kakudo = 80;
					}
					//弾の移動計算用に画面座標を取得
					HNB1.tm2[0][KoDama[i]].dCdn.x = HNB1.tm2[0][KoDama[i]].Cdn.x;
					HNB1.tm2[0][KoDama[i]].dCdn.y = HNB1.tm2[0][KoDama[i]].Cdn.y;
					//絶対座標をセット
					HNB1.tm2[0][KoDama[i]].MapCdn.x = MainCdn.x + HNB1.tm2[0][KoDama[i]].Cdn.x;
					HNB1.tm2[0][KoDama[i]].MapCdn.y = MainCdn.y + HNB1.tm2[0][KoDama[i]].Cdn.y;

					//この処理はKoDamaが上書きされない限り新しく弾を発射できないようにしています
					KoDama[i] = HNB1.TTama[0];
				}

				//大玉
				for (i = 0; i < 2; i++)
				{
					//発射中のフラグを立てる
					HNB1.tm2[1][ODama[i]].fTama |= _TON;
					//経過時間をクリア
					HNB1.tm2[1][ODama[i]].Time = 0;
					//弾アニメーションを初期化
					HNB1.tm2[1][ODama[i]].AnimeCnt = 0;
					HNB1.tm2[1][ODama[i]].FrameCnt = HNB1.tm2[1][ODama[i]].AnimeFrame[0];
					//発射される座標を初期化
					if (i == 0)
					{
						HNB1.tm2[1][ODama[i]].Cdn.x = Cdn.x - HNB1.tm2[1][ODama[0]].Width;
						HNB1.tm2[1][ODama[i]].Cdn.y = Cdn.y;
						HNB1.tm2[1][ODama[i]].Kakudo = 100;
					}
					else if (i == 1)
					{
						HNB1.tm2[1][ODama[i]].Cdn.x = Cdn.x + _JIKI_W;
						HNB1.tm2[1][ODama[i]].Cdn.y = Cdn.y;
						HNB1.tm2[1][ODama[i]].Kakudo = 80;
					}
					HNB1.tm2[1][ODama[i]].dCdn.x = HNB1.tm2[1][ODama[i]].Cdn.x;
					HNB1.tm2[1][ODama[i]].dCdn.y = HNB1.tm2[1][ODama[i]].Cdn.y;
					HNB1.tm2[1][ODama[i]].MapCdn.x = MainCdn.x + HNB1.tm2[1][ODama[i]].Cdn.x;
					HNB1.tm2[1][ODama[i]].MapCdn.y = MainCdn.y + HNB1.tm2[1][ODama[i]].Cdn.y;

					//この処理はODamaが上書きされない限り新しく弾を発射できないようにしています
					ODama[i] = HNB1.TTama[1];
				}
			}
			HNB1.WTamaCnt = HNB1.WTama; //発射までのカウンタを再セット
		}

		//----ここから弾の移動処理---------------------------------------------

		//大玉の移動処理
		for (i = 0; (DWORD)i < HNB1.TTama[1]; i++)
		{
			//発射中フラグの立っていない弾は処理しない
			if (HNB1.tm2[1][i].fTama & _TON)
			{
				TamaS = TRUE;		   //花火は画面内に存在している
				HNB1.tm2[1][i].Time++; //発射されてからの経過時間を進める
				//大玉が爆発する位置まで移動したかどうか調べる
				if (HNB1.tm2[1][i].Time > 81)
				{
					HNB1.tm2[1][i].fTama = _TOFF;							//フラグをクリア
					HNB1.tm2[1][i].Time = 0;								//経過時間をクリア
					HNB1.tm2[1][i].AnimeCnt = 0;							//最初に表示するアニメーションパターンを初期化
					HNB1.tm2[1][i].FrameCnt = HNB1.tm2[1][i].AnimeFrame[0]; //最初のアニメーションフレームカウントを初期化
					continue;
				}
				//大玉の移動量を決定
				HNB1.tm2[1][i].Mov.x = 5 * Cos[HNB1.tm2[1][i].Kakudo];
				HNB1.tm2[1][i].Mov.y = -((5 * Sin[HNB1.tm2[1][i].Kakudo]) - (HNB1.tm2[1][i].Time * _G));
				//大玉を移動させる
				HNB1.tm2[1][i].dCdn.x += HNB1.tm2[1][i].Mov.x;
				HNB1.tm2[1][i].dCdn.y += HNB1.tm2[1][i].Mov.y;
				HNB1.tm2[1][i].Cdn.x = (LONG)HNB1.tm2[1][i].dCdn.x;
				HNB1.tm2[1][i].Cdn.y = (LONG)HNB1.tm2[1][i].dCdn.y;
				HNB1.tm2[1][i].MapCdn.x = MainCdn.x + HNB1.tm2[1][i].Cdn.x;
				HNB1.tm2[1][i].MapCdn.y = MainCdn.y + HNB1.tm2[1][i].Cdn.y;

				//大玉が画面の外に移動していないかチェック
				if (HNB1.tm2[1][i].Cdn.x + HNB1.tm2[1][i].Width < 0 ||
					HNB1.tm2[1][i].Cdn.x > WinXMax ||
					HNB1.tm2[1][i].Cdn.y + HNB1.tm2[1][i].Height < 0 ||
					HNB1.tm2[1][i].Cdn.y > WinYMax)
				{
					HNB1.tm2[1][i].fTama = _TOFF; //フラグをクリア
					continue;
				}

				// SPサーフェスからの転送矩形範囲をセット
				HNB1.tm2[1][i].SrcR = TamSPJIKI[HNB1.tm2[1][i].AnimePat[HNB1.tm2[1][i].AnimeCnt]];

				HNB1.tm2[1][i].DesR.left = HNB1.tm2[1][i].Cdn.x;
				HNB1.tm2[1][i].DesR.top = HNB1.tm2[1][i].Cdn.y;
				HNB1.tm2[1][i].DesR.right = HNB1.tm2[1][i].Cdn.x + HNB1.tm2[1][i].Width;
				HNB1.tm2[1][i].DesR.bottom = HNB1.tm2[1][i].Cdn.y + HNB1.tm2[1][i].Height;

				//画面外の部分は描画しないようにする処理
				if (HNB1.tm2[1][i].DesR.left < 0)
				{
					HNB1.tm2[1][i].SrcR.left = HNB1.tm2[1][i].SrcR.left - HNB1.tm2[1][i].DesR.left;
					HNB1.tm2[1][i].DesR.left = 0;
				}
				else if (HNB1.tm2[1][i].DesR.right > WinXMax)
				{
					HNB1.tm2[1][i].SrcR.right = HNB1.tm2[1][i].SrcR.right - (HNB1.tm2[1][i].DesR.right - WinXMax);
					HNB1.tm2[1][i].DesR.right = WinXMax;
				}
				if (HNB1.tm2[1][i].DesR.top < 0)
				{
					HNB1.tm2[1][i].SrcR.top = HNB1.tm2[1][i].SrcR.top - HNB1.tm2[1][i].DesR.top;
					HNB1.tm2[1][i].DesR.top = 0;
				}

				//フレームを1減らして0になったらアニメパターンを進める
				if (!(HNB1.tm2[1][i].FrameCnt--))
				{
					HNB1.tm2[1][i].AnimeCnt = (HNB1.tm2[1][i].AnimeCnt + 1) % HNB1.tm2[1][i].Anime; //アニメパターンを進める
					HNB1.tm2[1][i].FrameCnt = HNB1.tm2[1][i].AnimeFrame[HNB1.tm2[1][i].AnimeCnt];	//表示するアニメパターンのフレーム数セット
				}
			}
			else if (HNB1.tm2[1][i].fTama == _TOFF)
			{
				for (j = 0; (DWORD)j < 2; j++)
				{
					if (ODama[j] == HNB1.TTama[1])
					{
						ODama[j] = i;
						HNB1.tm2[1][i].fTama |= _TSET; //再び発射セット
						break;
					}
				}
				if (!(ODama[1] == HNB1.TTama[1]) && !(fJIKI & _SUBHNB_ON))
				{
					break; //画面に弾が存在していない場合ループを抜ける
				}
			}
		}

		//小玉の移動量を決定
		for (i = 0; (DWORD)i < HNB1.TTama[0]; i++)
		{
			//発射中フラグの立っていない弾は処理しない
			if (HNB1.tm2[0][i].fTama & _TON)
			{
				TamaS = TRUE;		   //花火は画面内に存在している
				HNB1.tm2[0][i].Time++; //発射されてからの経過時間を進める

				if (HNB1.tm2[0][i].Time > 80 && !(HNB1.tm2[0][i].fTama & _TSAKU_ON))
				{
					HNB1.tm2[0][i].fTama |= _TSAKU_ON; //大玉が爆発フラグオン
					if (HNB1.tm2[0][i].Kakudo == 80)
					{									   //左
						HNB1.tm2[0][i].Kakudo = TamaKaku1; //爆発した角度をセット
						TamaKaku1 = TamaKaku1 + 12 > 348 ? 0 : TamaKaku1 + 12;
					}
					else if (HNB1.tm2[0][i].Kakudo == 100)
					{									   //右
						HNB1.tm2[0][i].Kakudo = TamaKaku2; //爆発した角度をセット
						TamaKaku2 = TamaKaku2 + 12 > 348 ? 0 : TamaKaku2 + 12;
					}
				}

				//小玉の移動量決定
				if (!(HNB1.tm2[0][i].fTama & _TSAKU_ON))
				{ //大玉の中で移動中
					HNB1.tm2[0][i].Mov.x = 5 * Cos[HNB1.tm2[0][i].Kakudo];
					HNB1.tm2[0][i].Mov.y = -((5 * Sin[HNB1.tm2[0][i].Kakudo]) - (HNB1.tm2[0][i].Time * _G));
				}
				else
				{ //大玉爆発後
					HNB1.tm2[0][i].Mov.x = 10 * Cos[HNB1.tm2[0][i].Kakudo];
					HNB1.tm2[0][i].Mov.y = -10 * Sin[HNB1.tm2[0][i].Kakudo];
				}

				//小玉を移動させる
				HNB1.tm2[0][i].dCdn.x += HNB1.tm2[0][i].Mov.x;
				HNB1.tm2[0][i].dCdn.y += HNB1.tm2[0][i].Mov.y;
				HNB1.tm2[0][i].Cdn.x = (LONG)HNB1.tm2[0][i].dCdn.x;
				HNB1.tm2[0][i].Cdn.y = (LONG)HNB1.tm2[0][i].dCdn.y;
				HNB1.tm2[0][i].MapCdn.x = MainCdn.x + HNB1.tm2[0][i].Cdn.x;
				HNB1.tm2[0][i].MapCdn.y = MainCdn.y + HNB1.tm2[0][i].Cdn.y;

				//小玉が画面の外に移動していないかチェック
				if (!(HNB1.tm2[0][i].fTama & _TSAKU_ON))
				{
					//大玉の中の時
					if (HNB1.tm2[0][i].Cdn.x + HNB1.tm2[0][i].Width < -8 ||
						HNB1.tm2[0][i].Cdn.x > WinXMax + 8 ||
						HNB1.tm2[0][i].Cdn.y + HNB1.tm2[0][i].Height < -8 ||
						HNB1.tm2[0][i].Cdn.y > WinYMax + 8)
					{
						HNB1.tm2[0][i].fTama = _TOFF; //フラグをクリア
						continue;
					}
				}
				else
				{
					//爆発した後
					if (HNB1.tm2[0][i].Cdn.x + HNB1.tm2[0][i].Width < 0 ||
						HNB1.tm2[0][i].Cdn.x > WinXMax ||
						HNB1.tm2[0][i].Cdn.y + HNB1.tm2[0][i].Height < 0 ||
						HNB1.tm2[0][i].Cdn.y > WinYMax)
					{
						HNB1.tm2[0][i].fTama = _TOFF; //フラグをクリア
						continue;
					}
				}

				// SPサーフェスからの転送矩形範囲をセット
				HNB1.tm2[0][i].SrcR = TamSPJIKI[HNB1.tm2[0][i].AnimePat[HNB1.tm2[0][i].AnimeCnt]];

				HNB1.tm2[0][i].DesR.left = HNB1.tm2[0][i].Cdn.x;
				HNB1.tm2[0][i].DesR.top = HNB1.tm2[0][i].Cdn.y;
				HNB1.tm2[0][i].DesR.right = HNB1.tm2[0][i].Cdn.x + HNB1.tm2[0][i].Width;
				HNB1.tm2[0][i].DesR.bottom = HNB1.tm2[0][i].Cdn.y + HNB1.tm2[0][i].Height;

				//画面外の部分は描画しないようにする処理
				if (HNB1.tm2[0][i].DesR.left < 0)
				{
					HNB1.tm2[0][i].SrcR.left = HNB1.tm2[0][i].SrcR.left - HNB1.tm2[0][i].DesR.left;
					HNB1.tm2[0][i].DesR.left = 0;
				}
				else if (HNB1.tm2[0][i].DesR.right > WinXMax)
				{
					HNB1.tm2[0][i].SrcR.right = HNB1.tm2[0][i].SrcR.right - (HNB1.tm2[0][i].DesR.right - WinXMax);
					HNB1.tm2[0][i].DesR.right = WinXMax;
				}
				if (HNB1.tm2[0][i].DesR.top < 0)
				{
					HNB1.tm2[0][i].SrcR.top = HNB1.tm2[0][i].SrcR.top - HNB1.tm2[0][i].DesR.top;
					HNB1.tm2[0][i].DesR.top = 0;
				}
				else if (HNB1.tm2[0][i].DesR.bottom > WinYMax)
				{
					HNB1.tm2[0][i].SrcR.bottom = HNB1.tm2[0][i].SrcR.bottom - (HNB1.tm2[0][i].DesR.bottom - WinYMax);
					HNB1.tm2[0][i].DesR.bottom = WinYMax;
				}

				//フレームを1減らして0になったらアニメパターンを進める
				if (!(HNB1.tm2[0][i].FrameCnt--))
				{
					HNB1.tm2[0][i].AnimeCnt = (HNB1.tm2[0][i].AnimeCnt + 1) % HNB1.tm2[0][i].Anime; //アニメパターンを進める
					HNB1.tm2[0][i].FrameCnt = HNB1.tm2[0][i].AnimeFrame[HNB1.tm2[0][i].AnimeCnt];	//表示するアニメパターンのフレーム数セット
				}
			}
			else if (HNB1.tm2[0][i].fTama == _TOFF)
			{
				//再発射可能な花火の番号を保存する
				for (j = 0; (DWORD)j < 60; j++)
				{
					if (KoDama[j] == HNB1.TTama[0])
					{
						KoDama[j] = i;
						HNB1.tm2[0][i].fTama |= _TSET; //再び発射セット
						break;
					}
				}
				if (!(KoDama[59] == HNB1.TTama[0]) && !(fJIKI & _SUBHNB_ON))
				{
					break; //画面に弾が存在していない場合はループを抜ける
				}
			}
		}
		break;
	default:
		break;
	}
	//弾が存在していなかったら花火発射フラグをオフ
	if (!TamaS)
		fJIKI &= _SUBHNB_OFF;
}

//////////////////////////////////////////////////////////////////////
//	敵キャラと花火との当たり判定
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HnbAtari()
{
	short i, j, k;

	//発射されている花火と敵キャラの当たり判定
	switch (Wpd.flag & _HNB_LVCHK)
	{
	case _HNB1_ON:
		//大玉の当たり判定
		for (i = 0; (DWORD)i < HNB1.TTama[1]; i++)
		{
			//未発射の弾とは当たり判定をしない
			if (HNB1.tm2[1][i].fTama & _TON)
			{
				if (!(Atari(HNB1.tm2[1][i].DesR)))
				{
					//大玉を爆発させる
					HNB1.tm2[1][i].fTama = _TOFF; //フラグをクリア

					for (j = 0; (DWORD)j < HNB1.TTama[0]; j++)
					{
						if (HNB1.tm2[0][j].fTama & _TON && !(HNB1.tm2[0][j].fTama & _TSAKU_ON))
						{
							//右の大玉が敵に当たった場合
							if (HNB1.tm2[1][i].Kakudo == 80 && HNB1.tm2[0][j].Kakudo == 80)
							{
								HNB1.tm2[0][j].Time += 80; //爆発までの時間を進める
								//左の大玉が敵に当たった場合
							}
							else if (HNB1.tm2[1][i].Kakudo == 100 && HNB1.tm2[0][j].Kakudo == 100)
							{
								HNB1.tm2[0][j].Time += 80; //爆発までの時間を進める
							}
						}
					}
				}

				//破壊可能な弾との当たり判定
				if (SP_Tamas3)
				{
					for (j = 0; (DWORD)j < SP_Tamas3->TTama; j++)
					{
						if (SP_Tamas3->tm3[j].fTama & _TON)
						{
							if (!(Atari(HNB1.tm2[1][i].DesR, SP_Tamas3->tm3[j].DesR)))
							{
								//大玉を爆発させる
								HNB1.tm2[1][i].fTama = _TOFF; //フラグをクリア
								for (k = 0; (DWORD)k < HNB1.TTama[0]; k++)
								{
									if (HNB1.tm2[0][k].fTama & _TON && !(HNB1.tm2[0][k].fTama & _TSAKU_ON))
									{
										//右の大玉が敵に当たった場合
										if (HNB1.tm2[1][i].Kakudo == 80 && HNB1.tm2[0][k].Kakudo == 80)
										{
											HNB1.tm2[0][k].Time += 80; //爆発までの時間を進める
											//左の大玉が敵に当たった場合
										}
										else if (HNB1.tm2[1][i].Kakudo == 100 && HNB1.tm2[0][k].Kakudo == 100)
										{
											HNB1.tm2[0][k].Time += 80; //爆発までの時間を進める
										}
									}
								}
							}
						}
					}
				}
			}
		}

		//小玉の当たり判定
		for (i = 0; (DWORD)i < HNB1.TTama[0]; i++)
		{
			//未発射の弾とは当たり判定をしない
			if (HNB1.tm2[0][i].fTama & _TON)
			{
				if (!(Atari(HNB1.tm2[0][i].DesR)))
				{
					//小玉を消滅させる
					HNB1.tm2[0][i].fTama = _TOFF; //フラグをクリア
					JikiTekiDmg(_HNB1DMG);		  //敵キャラにダメージ
				}

				//破壊可能な弾との当たり判定
				if (SP_Tamas3)
				{
					for (j = 0; (DWORD)j < SP_Tamas3->TTama; j++)
					{
						if (SP_Tamas3->tm3[j].fTama & _TON)
						{
							if (!(Atari(HNB1.tm2[0][i].DesR, SP_Tamas3->tm3[j].DesR)))
							{
								//小玉を消滅させる
								HNB1.tm2[0][i].fTama = _TOFF; //フラグをクリア
								JikiTamas3Dmg(_HNB1DMG, j);
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
//	花火の描画処理
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HnbRend()
{
	short i;

	switch (Wpd.flag & _HNB_LVCHK)
	{ //装備のレベルを調べている
	case _HNB1_ON:
		for (i = 0; (DWORD)i < HNB1.TTama[0]; i++)
		{
			//発射中フラグの立っていない弾は描画しない
			if (HNB1.tm2[0][i].fTama & _TON)
			{
				lpDDSBack->BltFast(HNB1.tm2[0][i].DesR.left, HNB1.tm2[0][i].DesR.top,
								   lpDDS_JIKI, &HNB1.tm2[0][i].SrcR,
								   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			}
		}
		for (i = 0; (DWORD)i < HNB1.TTama[1]; i++)
		{
			if (HNB1.tm2[1][i].fTama & _TON)
			{
				lpDDSBack->BltFast(HNB1.tm2[1][i].DesR.left, HNB1.tm2[1][i].DesR.top,
								   lpDDS_JIKI, &HNB1.tm2[1][i].SrcR,
								   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			}
		}
		break;
	default:
		break;
	}
}
