/*=============================================================================
　オプションの移動、当たり判定、描画等に関するソースファイル
==============================================================================*/
#include <windows.h>
#include <ddraw.h>

#include "Macros.h"
#include "Draw.h"
#include "Struct.h"
#include "DrawJIKI.h"
#include "DrawJIKIDataEXT.h"
#include "DrawMacros.h"

extern LPDIRECTDRAWSURFACE7 lpDDSBack; //バックサーフェス

#define _KOSIZE 5  //オプションが固定移動時に使用する過去の自機の座標を格納する構造体の数
#define _TUSIZE 60 //オプションが追尾移動時に使用する過去の自機の座標を格納する構造体の数

//////////////////////////////////////////////////////////////////////
//	OPTDATA構造体の各メンバの初期化
//////////////////////////////////////////////////////////////////////
BOOL DrawJIKI::InitOpt()
{
	short i, j;

	OPT.OptLV = 0;		 //オプションの装備されている数を初期化
	OPT.KoteCnt = 0;	 // KoteCdnの保存先のカウンタの初期化
	OPT.TuibCnt = 0;	 // TuibCdnの保存先のカウンタの初期化
	OPT.KaitKaku[0] = 0; //回転フラグの立っているオプションに使用される回転角度の初期化
	OPT.KaitKaku[1] = 90;
	OPT.KaitKaku[2] = 180;
	OPT.KaitKaku[3] = 270;
	for (i = 0; i < 4; i++)
	{ //回転方向を示すフラグの初期化
		OPT.KaitenF[i] = 0;
	}

	///////////////////////////////////////////////////////////
	//------------------------OPT.Jewelの初期化----------------

	OPTANI oa; // OPTANI構造体に前もって値をセットしてOPT.Jewelを初期化する

	oa.fOPT = 0;	//オプションの初期状態を設定
	oa.Width = 20;	//オプションの横幅
	oa.Height = 32; //オプションの縦幅
	oa.dCdn.x = 0;	//オプションの画面座標計算用
	oa.dCdn.y = 0;
	oa.Cdn.x = 0; //オプションの現在の画面座標を0で初期化
	oa.Cdn.y = 0;
	oa.MapCdn.x = 0; //オプションの現在の絶対座標も0で初期化
	oa.MapCdn.y = 0;
	oa.Anime = TamAmeJIKI[2];										   //オプションアニメーションパターン数の初期化
	memcpy(&oa.AnimePat[0], &TamPatJIKI[2][0], sizeof(BYTE) * 10);	   //オプションアニメーションパターン配列の初期化
	memcpy(&oa.AnimeFrame[0], &TamFrameJIKI[2][0], sizeof(BYTE) * 10); //オプションアニメーション切り替えタイミング配列の初期化
	oa.AnimeCnt = 0;												   //最初に表示するアニメーションパターンを初期化
	oa.FrameCnt = oa.AnimeFrame[0];									   //最初のアニメーションフレームカウントを初期化
	oa.SrcR.left = 0;												   //転送元矩形を0で初期化
	oa.SrcR.top = 0;
	oa.SrcR.right = 0;
	oa.SrcR.bottom = 0;
	oa.DesR = oa.SrcR; //転送先矩形も0で初期化

	//オプションオブジェクトを4つ分、初期化
	for (i = 0; (DWORD)i < 4; i++)
	{
		memcpy(&OPT.Jewel[i], &oa, sizeof(OPTANI)); //確保した領域の先頭からコピー
	}

	//////////////////////////////////////////////////////////////////////
	//------OPT.AtkDat[0-3].Opt_LSR1を管理するオブジェクトの初期化--------
	TAMA tama; // TAMA構造体に前もって値をセットして初期化する

	tama.fTama = _TOFF; //弾の初期状態を設定
	tama.Width = 4;		//弾の横幅
	tama.Height = 72;	//弾の縦幅
	tama.Mov.x = 0.0;	//弾の移動速度を初期化
	tama.Mov.y = 20.0;
	tama.dCdn.x = 0; //弾の画面座標計算用
	tama.dCdn.y = 0;
	tama.Cdn.x = 0; //弾の現在の画面座標を0で初期化
	tama.Cdn.y = 0;
	tama.MapCdn.x = 0; //弾の現在の絶対座標も0で初期化
	tama.MapCdn.y = 0;
	tama.Anime = TamAmeJIKI[0]; //弾アニメーションパターン数の初期化
	//弾アニメーションパターン配列の初期化
	memcpy(&tama.AnimePat[0], &TamPatJIKI[0][0], sizeof(BYTE) * 10);
	//弾アニメーション切り替えタイミング配列の初期化
	memcpy(&tama.AnimeFrame[0], &TamFrameJIKI[0][0], sizeof(BYTE) * 10);
	tama.AnimeCnt = 0;					//最初に表示するアニメーションパターンを初期化
	tama.FrameCnt = tama.AnimeFrame[0]; //最初のアニメーションフレームカウントを初期化
	tama.SrcR.left = 0;					//転送元矩形を0で初期化
	tama.SrcR.top = 0;
	tama.SrcR.right = 0;
	tama.SrcR.bottom = 0;
	tama.DesR = tama.SrcR; //転送先矩形も0で初期化

	char *pTama;

	for (i = 0; i < 4; i++)
	{
		//弾ステータス構造体に値をセットして弾数分確保した、弾オブジェクトを初期化
		OPT.AtkDat[i].Opt_LSR1.TTama = 3;								//画面上に存在できる弾の数
		OPT.AtkDat[i].Opt_LSR1.WTama = 10;								//弾を発射する間隔
		OPT.AtkDat[i].Opt_LSR1.WTamaCnt = OPT.AtkDat[i].Opt_LSR1.WTama; //弾発射までのカウンタ

		//発射できる弾数分TAMA構造体を確保
		OPT.AtkDat[i].Opt_LSR1.tm = new TAMA[OPT.AtkDat[i].Opt_LSR1.TTama];
		if (!OPT.AtkDat[i].Opt_LSR1.tm)
			return FALSE;
		//確保した領域の先頭アドレスをコピー
		pTama = (char *)OPT.AtkDat[i].Opt_LSR1.tm;
		//弾オブジェクトを確保した分、初期化
		for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_LSR1.TTama; j++)
		{
			memcpy(pTama, &tama, sizeof(TAMA)); //確保した領域の先頭からコピー
			pTama += sizeof(TAMA);				//次にコピーするところまでアドレスを進める
		}
	}

	//////////////////////////////////////////////////////////////////////
	//-------------OPT.MSR1を管理するオブジェクトの初期化---------------

	tama.fTama = _TOFF; //弾の初期状態を設定
	tama.Width = 16;	//弾の横幅
	tama.Height = 32;	//弾の縦幅
	tama.Mov.x = 0.0;	//弾の移動速度を初期化
	tama.Mov.y = 16.0;
	tama.dCdn.x = 0; //弾の画面座標計算用
	tama.dCdn.y = 0;
	tama.Cdn.x = 0; //弾の現在の画面座標を0で初期化
	tama.Cdn.y = 0;
	tama.MapCdn.x = 0; //弾の現在の絶対座標も0で初期化
	tama.MapCdn.y = 0;
	tama.Anime = TamAmeJIKI[1]; //弾アニメーションパターン数の初期化
	//弾アニメーションパターン配列の初期化
	memcpy(&tama.AnimePat[0], &TamPatJIKI[1][0], sizeof(BYTE) * 10);
	//弾アニメーション切り替えタイミング配列の初期化
	memcpy(&tama.AnimeFrame[0], &TamFrameJIKI[1][0], sizeof(BYTE) * 10);
	tama.AnimeCnt = 0;					//最初に表示するアニメーションパターンを初期化
	tama.FrameCnt = tama.AnimeFrame[0]; //最初のアニメーションフレームカウントを初期化
	tama.SrcR.left = 0;					//転送元矩形を0で初期化
	tama.SrcR.top = 0;
	tama.SrcR.right = 0;
	tama.SrcR.bottom = 0;
	tama.DesR = tama.SrcR; //転送先矩形も0で初期化

	for (i = 0; i < 4; i++)
	{
		//弾ステータス構造体に値をセットして弾数分確保した、弾オブジェクトを初期化
		OPT.AtkDat[i].Opt_MSR1.TTama = 6;								//画面上に存在できる弾の数
		OPT.AtkDat[i].Opt_MSR1.WTama = 8;								//弾を発射する間隔
		OPT.AtkDat[i].Opt_MSR1.WTamaCnt = OPT.AtkDat[i].Opt_MSR1.WTama; //弾発射までのカウンタ

		//発射できる弾数分TAMA構造体を確保
		OPT.AtkDat[i].Opt_MSR1.tm = new TAMA[OPT.AtkDat[i].Opt_MSR1.TTama];
		if (!OPT.AtkDat[i].Opt_MSR1.tm)
			return FALSE;
		//確保した領域の先頭アドレスをコピー
		pTama = (char *)OPT.AtkDat[i].Opt_MSR1.tm;
		//弾オブジェクトを確保した分、初期化
		for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_MSR1.TTama; j++)
		{
			memcpy(pTama, &tama, sizeof(TAMA)); //確保した領域の先頭からコピー
			pTama += sizeof(TAMA);				//次にコピーするところまでアドレスを進める
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	動的に確保するメモリ領域へのポインタをNULLで初期化
//////////////////////////////////////////////////////////////////////
void DrawJIKI::InitOptAtkDatTm()
{
	for (short i = 0; i < 4; i++)
	{
		OPT.AtkDat[i].Opt_MSR1.tm = NULL;
		OPT.AtkDat[i].Opt_LSR1.tm = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
//	確保していた弾オブジェクトのメモリ領域を解放
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptRelease()
{
	for (short i = 0; i < 4; i++)
	{
		RELEASE_dels(OPT.AtkDat[i].Opt_MSR1.tm);
		RELEASE_dels(OPT.AtkDat[i].Opt_LSR1.tm);
	}
}

//////////////////////////////////////////////////////////////////////
//	オプションの表示座標の初期化(パワーアップ処理画面をEXITする時に使用)
//////////////////////////////////////////////////////////////////////
void DrawJIKI::InitOptCdn()
{
	switch (Wpd.flag & _OPT_LVCHK)
	{ //オプションのレベルを調べている
	case _OPT1_ON:
		OPT.OptLV = 1;
		break;
	case _OPT2_ON:
		OPT.OptLV = 2;
		break;
	case _OPT3_ON:
		OPT.OptLV = 3;
		break;
	case _OPT4_ON:
		OPT.OptLV = 4;
		break;
	default:
		OPT.OptLV = 0;
		break;
	}

	BYTE fKO = 0, fKA = 0, fTU = 0; //固定、回転、追尾フラグの立っているオプションの表示座標初期化用フラグ
	short i, j;
	short kotsu = 0; //固定フラグの立っているオプションの数カウント用

	if (OPT.OptLV)
	{
		for (i = 0; i < OPT.OptLV; i++)
		{
			switch (OPT.Jewel[i].fOPT & _fOPT_CHK)
			{ //固定、回転、追尾のどのフラクが立っているかチェック
			case _OPTKO_ON:
				if (fKO)
					;
				else
				{
					//オプションが固定移動時に使用する過去の自機の座標を格納する構造体の初期化
					for (j = 0; j < _KOSIZE; j++)
					{
						OPT.KoteCdn[j] = Cdn;
					}
					fKO = 1; //同じ処理を２回しないようにするためにフラグを立てる
				}
				//オプションの現在の位置を初期化
				if (kotsu == 0)
				{ //固定フラグが立っている1つ目のオプションの初期座標
					OPT.Jewel[i].dCdn.x = (double)Cdn.x - 40;
					OPT.Jewel[i].dCdn.y = (double)Cdn.y;
				}
				else if (kotsu == 1)
				{ //固定フラグが立っている2つ目のオプションの初期座標
					OPT.Jewel[i].dCdn.x = (double)Cdn.x + 40;
					OPT.Jewel[i].dCdn.y = (double)Cdn.y;
				}
				else if (kotsu == 2)
				{ //固定フラグが立っている3つ目のオプションの初期座標
					OPT.Jewel[i].dCdn.x = (double)Cdn.x - 20;
					OPT.Jewel[i].dCdn.y = (double)Cdn.y - 40;
				}
				else if (kotsu == 3)
				{ //固定フラグが立っている4つ目のオプションの初期座標
					OPT.Jewel[i].dCdn.x = (double)Cdn.x + 20;
					OPT.Jewel[i].dCdn.y = (double)Cdn.y - 40;
				}
				kotsu++;
				break;
			case _OPTKA_ON:
				if (fKA)
					;
				else
				{
					//回転フラグの立っているオプションに使用される回転角度の初期化
					OPT.KaitKaku[0] = 0;
					OPT.KaitKaku[1] = 45;
					OPT.KaitKaku[2] = 90;
					OPT.KaitKaku[3] = 135;
					fKA = 1;
				}
				//回転方向を示すフラグを初期化
				OPT.KaitenF[i] = 0;

				//回転フラグが立っているオプションの初期座標
				OPT.Jewel[i].dCdn.x = (double)Cdn.x;
				OPT.Jewel[i].dCdn.y = (double)Cdn.y;

				break;
			case _OPTTU_ON:
				if (fTU)
					;
				else
				{
					//オプションが追尾移動時に使用する過去の自機の座標を格納する構造体の初期化
					for (j = 0; j < _TUSIZE; j++)
					{
						OPT.TuibCdn[j] = Cdn;
					}
					fTU = 1;
				}
				OPT.Jewel[i].dCdn.x = (double)Cdn.x;
				OPT.Jewel[i].dCdn.y = (double)Cdn.y;

				break;
			default:
				break;
			}
			//ディスプレイの表示座標に直す処理
			OPT.Jewel[i].Cdn.x = (LONG)OPT.Jewel[i].dCdn.x;
			OPT.Jewel[i].Cdn.y = (LONG)OPT.Jewel[i].dCdn.y;
			OPT.Jewel[i].MapCdn.x = MainCdn.x + OPT.Jewel[i].Cdn.x;
			OPT.Jewel[i].MapCdn.y = MainCdn.y + OPT.Jewel[i].Cdn.y;
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	自機の過去の座標を指定された数だけ保存する
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptMovCnt(BYTE joyst)
{
	//追尾オプション用
	if (joyst & 0x0F) //ジョイスティックに上下左右いずれかの入力のあった場合のみ
	{				  //自機の現在の座標を取得
		OPT.TuibCdn[OPT.TuibCnt] = Cdn;
		OPT.TuibCnt++;
		if (OPT.TuibCnt > _TUSIZE - 1)
			OPT.TuibCnt = 0;
	}

	//固定オプション用
	OPT.KoteCdn[OPT.KoteCnt] = Cdn;
	OPT.KoteCnt++;
	if (OPT.KoteCnt > _KOSIZE - 1)
		OPT.KoteCnt = 0;
}

//////////////////////////////////////////////////////////////////////
//	オプションの移動とアニメーション処理
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptMov()
{
	short kc = 0, tc = 0; //固定、追尾カウンタ作業用
	short kotsu = 0;	  //固定フラグの立っているオプションの数カウント用
	short kaisu = 0;	  //回転フラグの立っているオプションの数カウント用
	short tuisu = 1;	  //追尾フラグの立っているオプションの数カウント用

	for (short i = 0; i < OPT.OptLV; i++)
	{
		switch (OPT.Jewel[i].fOPT & _fOPT_CHK)
		{				//移動パターンのチェック
		case _OPTKO_ON: //固定
			kc = OPT.KoteCnt - (_KOSIZE - 1);
			if (kc < 0)
				kc = kc + _KOSIZE; // 0～_KOSIZEをループさせる処理
			//移動させる処理
			if (kotsu == 0)
			{
				OPT.Jewel[i].dCdn.x = (double)OPT.KoteCdn[kc].x - 40;
				OPT.Jewel[i].dCdn.y = (double)OPT.KoteCdn[kc].y;
			}
			else if (kotsu == 1)
			{
				OPT.Jewel[i].dCdn.x = (double)OPT.KoteCdn[kc].x + 40;
				OPT.Jewel[i].dCdn.y = (double)OPT.KoteCdn[kc].y;
			}
			else if (kotsu == 2)
			{
				OPT.Jewel[i].dCdn.x = (double)OPT.KoteCdn[kc].x - 20;
				OPT.Jewel[i].dCdn.y = (double)OPT.KoteCdn[kc].y - 40;
			}
			else if (kotsu == 3)
			{
				OPT.Jewel[i].dCdn.x = (double)OPT.KoteCdn[kc].x + 20;
				OPT.Jewel[i].dCdn.y = (double)OPT.KoteCdn[kc].y - 40;
			}
			kotsu++;
			break;
		case _OPTKA_ON: //回転
			OPT.Jewel[i].dCdn.y = (double)Cdn.y;
			if (!OPT.KaitenF[kaisu])
			{ //回転方向フラグをチェック
				//自機の左側で回転
				OPT.Jewel[i].dCdn.x = (double)Cdn.x - 32;
				OPT.Jewel[i].dCdn.x += 32 * Cos[OPT.KaitKaku[kaisu]];
				OPT.Jewel[i].dCdn.y += -32 * Sin[OPT.KaitKaku[kaisu]];

				OPT.KaitKaku[kaisu] += 10; //角度を10足す
				if (OPT.KaitKaku[kaisu] > 360)
				{ // 360より大きくならないようする
					OPT.KaitKaku[kaisu] = 180;
					OPT.KaitenF[kaisu] = 1; //回転方向フラグをセット
				}
			}
			else if (OPT.KaitenF[kaisu] == 1)
			{
				//自機の右側で回転(上半分)
				OPT.Jewel[i].dCdn.x = (double)Cdn.x + 32;
				OPT.Jewel[i].dCdn.x += 32 * Cos[OPT.KaitKaku[kaisu]];
				OPT.Jewel[i].dCdn.y += 32 * Sin[OPT.KaitKaku[kaisu]];

				OPT.KaitKaku[kaisu] += 10;
				if (OPT.KaitKaku[kaisu] > 360)
				{
					OPT.KaitKaku[kaisu] = 0;
					OPT.KaitenF[kaisu] = 2;
				}
			}
			else if (OPT.KaitenF[kaisu] == 2)
			{
				//自機の右側で回転(下半分)
				OPT.Jewel[i].dCdn.x = (double)Cdn.x + 32;
				OPT.Jewel[i].dCdn.x += 32 * Cos[OPT.KaitKaku[kaisu]];
				OPT.Jewel[i].dCdn.y += 32 * Sin[OPT.KaitKaku[kaisu]];

				OPT.KaitKaku[kaisu] += 10;
				if (OPT.KaitKaku[kaisu] > 180)
				{
					OPT.KaitKaku[kaisu] = 0;
					OPT.KaitenF[kaisu] = 0;
				}
			}
			kaisu++;
			break;
		case _OPTTU_ON: //追尾
			//座標を取り出す処理(意味、Juwel1==自機の12手前の座標,Juwel2==24手前.....)
			tc = OPT.TuibCnt - (tuisu * 12);
			if (tc < 0)
				tc = tc + _TUSIZE; // 0～_TUSIZEをループさせる処理
			//移動させる処理
			OPT.Jewel[i].dCdn.x = (double)OPT.TuibCdn[tc].x;
			OPT.Jewel[i].dCdn.y = (double)OPT.TuibCdn[tc].y;
			tuisu++;
			break;
		default:
			return;
			break;
		}
		//ディスプレイの表示座標に直す処理
		OPT.Jewel[i].Cdn.x = (LONG)OPT.Jewel[i].dCdn.x;
		OPT.Jewel[i].Cdn.y = (LONG)OPT.Jewel[i].dCdn.y;
		OPT.Jewel[i].MapCdn.x = MainCdn.x + OPT.Jewel[i].Cdn.x;
		OPT.Jewel[i].MapCdn.y = MainCdn.y + OPT.Jewel[i].Cdn.y;

		//サーフェスからの転送矩形範囲をセット
		OPT.Jewel[i].SrcR = TamSPJIKI[OPT.Jewel[i].AnimePat[OPT.Jewel[i].AnimeCnt]];

		OPT.Jewel[i].DesR.left = OPT.Jewel[i].Cdn.x;
		OPT.Jewel[i].DesR.top = OPT.Jewel[i].Cdn.y;
		OPT.Jewel[i].DesR.right = OPT.Jewel[i].Cdn.x + OPT.Jewel[i].Width;
		OPT.Jewel[i].DesR.bottom = OPT.Jewel[i].Cdn.y + OPT.Jewel[i].Height;

		//画面外の部分は描画しないようにする処理
		if (OPT.Jewel[i].DesR.left < 0)
		{
			OPT.Jewel[i].SrcR.left = OPT.Jewel[i].SrcR.left - OPT.Jewel[i].DesR.left;
			OPT.Jewel[i].DesR.left = 0;
		}
		else if (OPT.Jewel[i].DesR.right > WinXMax)
		{
			OPT.Jewel[i].SrcR.right = OPT.Jewel[i].SrcR.right - (OPT.Jewel[i].DesR.right - WinXMax);
			OPT.Jewel[i].DesR.right = WinXMax;
		}
		if (OPT.Jewel[i].Cdn.y < 0)
		{
			OPT.Jewel[i].SrcR.top = OPT.Jewel[i].SrcR.top - OPT.Jewel[i].DesR.top;
			OPT.Jewel[i].DesR.top = 0;
		}
		else if (OPT.Jewel[i].DesR.bottom > WinYMax)
		{
			OPT.Jewel[i].SrcR.bottom = OPT.Jewel[i].SrcR.bottom - (OPT.Jewel[i].DesR.bottom - WinYMax);
			OPT.Jewel[i].DesR.bottom = WinYMax;
		}
		//フレームを1減らして0になったらアニメパターンを進める
		if (!(OPT.Jewel[i].FrameCnt--))
		{
			OPT.Jewel[i].AnimeCnt = (OPT.Jewel[i].AnimeCnt + 1) % OPT.Jewel[i].Anime; //アニメパターンを進める
			OPT.Jewel[i].FrameCnt = OPT.Jewel[i].AnimeFrame[OPT.Jewel[i].AnimeCnt];	  //表示するアニメパターンのフレーム数セット
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	オプションの攻撃までのカウントを進める
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptAttackCnt()
{
	//現在使用している主砲をチェックしてフラグの立っている主砲の処理をする
	switch (Wpd.flag & _MAINWP_CHK)
	{
	case _LSR_ON:
		switch (Wpd.flag & _LSR_LVCHK)
		{ //レーザーのレベルを調べている
		case _LSR1_ON:
			switch (OPT.OptLV)
			{ //オプションが何個ついているか調べている
			case 4:
				OPT.AtkDat[3].Opt_LSR1.WTamaCnt--; //発射までのカウントダウン
			case 3:
				OPT.AtkDat[2].Opt_LSR1.WTamaCnt--;
			case 2:
				OPT.AtkDat[1].Opt_LSR1.WTamaCnt--;
			case 1:
				OPT.AtkDat[0].Opt_LSR1.WTamaCnt--;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case _MSR_ON:
		switch (Wpd.flag & _MSR_LVCHK)
		{
		case _MSR1_ON:
			switch (OPT.OptLV)
			{ //オプションが何個ついているか調べている
			case 4:
				OPT.AtkDat[3].Opt_MSR1.WTamaCnt--; //発射までのカウントダウン
			case 3:
				OPT.AtkDat[2].Opt_MSR1.WTamaCnt--;
			case 2:
				OPT.AtkDat[1].Opt_MSR1.WTamaCnt--;
			case 1:
				OPT.AtkDat[0].Opt_MSR1.WTamaCnt--;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
//	オプションの攻撃までのカウントをリセット
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptAttackCntReset()
{
	//レーザーの発射カウントをリセット
	switch (Wpd.flag & _LSR_LVCHK)
	{
	case _LSR1_ON:
		switch (OPT.OptLV)
		{ //オプションが何個ついているか調べている
		case 4:
			//発射までのカウントをリセット
			OPT.AtkDat[3].Opt_LSR1.WTamaCnt = OPT.AtkDat[3].Opt_LSR1.WTama;
		case 3:
			OPT.AtkDat[2].Opt_LSR1.WTamaCnt = OPT.AtkDat[2].Opt_LSR1.WTama;
		case 2:
			OPT.AtkDat[1].Opt_LSR1.WTamaCnt = OPT.AtkDat[1].Opt_LSR1.WTama;
		case 1:
			OPT.AtkDat[0].Opt_LSR1.WTamaCnt = OPT.AtkDat[0].Opt_LSR1.WTama;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	//ミサイルの発射カウントをリセット
	switch (Wpd.flag & _MSR_LVCHK)
	{
	case _MSR1_ON:
		switch (OPT.OptLV)
		{ //オプションが何個ついているか調べている
		case 4:
			//発射までのカウントをリセット
			OPT.AtkDat[3].Opt_MSR1.WTamaCnt = OPT.AtkDat[3].Opt_MSR1.WTama;
		case 3:
			OPT.AtkDat[2].Opt_MSR1.WTamaCnt = OPT.AtkDat[2].Opt_MSR1.WTama;
		case 2:
			OPT.AtkDat[1].Opt_MSR1.WTamaCnt = OPT.AtkDat[1].Opt_MSR1.WTama;
		case 1:
			OPT.AtkDat[0].Opt_MSR1.WTamaCnt = OPT.AtkDat[0].Opt_MSR1.WTama;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
//	オプションの弾を全て発射待機状態に戻す
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptAttackReset()
{
	OptAttackCntReset(); //発射までのカウンタをリセット

	short i, j;

	//全てのオプションの弾フラグを発射待機状態にセット
	switch (Wpd.flag & _LSR_LVCHK)
	{ //装備のレベルを調べている
	case _LSR1_ON:
		for (i = 0; i < OPT.OptLV; i++)
		{
			for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_LSR1.TTama; j++)
			{
				OPT.AtkDat[i].Opt_LSR1.tm[j].fTama = _TOFF;
			}
		}
		break;
	default:
		break;
	}
	switch (Wpd.flag & _MSR_LVCHK)
	{ //装備のレベルを調べている
	case _MSR1_ON:
		for (i = 0; i < OPT.OptLV; i++)
		{
			for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_MSR1.TTama; j++)
			{
				OPT.AtkDat[i].Opt_MSR1.tm[j].fTama = _TOFF;
			}
		}
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
//	オプションの攻撃処理
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptAttack()
{
	//画面に弾が存在しているかしていないか調べるフラグ
	bool TamaS = FALSE;

	//発射できる状態になっているレーザーの番号保存用
	static DWORD LsrNum[4] = {
		OPT.AtkDat[0].Opt_LSR1.TTama, OPT.AtkDat[1].Opt_LSR1.TTama,
		OPT.AtkDat[2].Opt_LSR1.TTama, OPT.AtkDat[3].Opt_LSR1.TTama};
	//発射できる状態になっているミサイルの番号保存用
	static DWORD MsrNum[4] = {
		OPT.AtkDat[0].Opt_MSR1.TTama, OPT.AtkDat[1].Opt_MSR1.TTama,
		OPT.AtkDat[2].Opt_MSR1.TTama, OPT.AtkDat[3].Opt_MSR1.TTama};

	short i, j; //ループカウンタ用

	switch (Wpd.flag & _LSR_LVCHK)
	{ //装備のレベルを調べている
	case _LSR1_ON:
		for (i = 0; i < OPT.OptLV; i++)
		{
			if (!OPT.AtkDat[i].Opt_LSR1.WTamaCnt)
			{
				//レーザーが指定された数以上に画面に存在しないようにする処理
				if (LsrNum[i] < OPT.AtkDat[i].Opt_LSR1.TTama)
				{
					//画面上に弾が発射されているフラグオン
					fJIKI |= _OPTWEP_ON;
					//発射中のフラグを立てる
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].fTama |= _TON;
					//弾アニメーションを初期化
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].AnimeCnt = 0;
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].FrameCnt = OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].AnimeFrame[0];
					//レーザーの発射された座標を初期化
					//画面座標をセット
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].Cdn.x = OPT.Jewel[i].Cdn.x + 8;
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].Cdn.y = OPT.Jewel[i].Cdn.y - (OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].Height - 32);
					//弾の移動計算用に画面座標を取得
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].dCdn.x = OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].Cdn.x;
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].dCdn.y = OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].Cdn.y;
					//絶対座標をセット
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].MapCdn.x = MainCdn.x + OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].Cdn.x;
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].MapCdn.y = MainCdn.y + OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].Cdn.y;

					//この処理はTTamaNumが上書きされない限り
					//新しく弾を発射できないようにしています
					LsrNum[i] = OPT.AtkDat[i].Opt_LSR1.TTama;
				}
				OPT.AtkDat[i].Opt_LSR1.WTamaCnt = OPT.AtkDat[i].Opt_LSR1.WTama; //発射までのカウンタを再セット
			}

			for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_LSR1.TTama; j++)
			{
				//発射中フラグの立っていない弾は処理しない
				if (OPT.AtkDat[i].Opt_LSR1.tm[j].fTama & _TON)
				{
					TamaS = TRUE; //レーザーは画面内に存在している

					//レーザーを移動させる
					OPT.AtkDat[i].Opt_LSR1.tm[j].dCdn.x += OPT.AtkDat[i].Opt_LSR1.tm[j].Mov.x;
					OPT.AtkDat[i].Opt_LSR1.tm[j].dCdn.y -= OPT.AtkDat[i].Opt_LSR1.tm[j].Mov.y;
					OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.x = (LONG)OPT.AtkDat[i].Opt_LSR1.tm[j].dCdn.x;
					OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.y = (LONG)OPT.AtkDat[i].Opt_LSR1.tm[j].dCdn.y;
					OPT.AtkDat[i].Opt_LSR1.tm[j].MapCdn.x = MainCdn.x + OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.x;
					OPT.AtkDat[i].Opt_LSR1.tm[j].MapCdn.y = MainCdn.y + OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.y;

					//レーザーが画面の外に移動していないかチェック
					if (OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.x + OPT.AtkDat[i].Opt_LSR1.tm[j].Width < 0 ||
						OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.x > WinXMax ||
						OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.y + OPT.AtkDat[i].Opt_LSR1.tm[j].Height < 0)
					{
						OPT.AtkDat[i].Opt_LSR1.tm[j].fTama &= _TOFF; //フラグをクリア
						continue;
					}

					// SPサーフェスからの転送矩形範囲をセット
					OPT.AtkDat[i].Opt_LSR1.tm[j].SrcR = TamSPJIKI[OPT.AtkDat[i].Opt_LSR1.tm[j].AnimePat[OPT.AtkDat[i].Opt_LSR1.tm[j].AnimeCnt]];

					OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.left = OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.x;
					OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.top = OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.y;
					OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.right = OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.x + LSR1.tm[i].Width;
					OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.bottom = OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.y + LSR1.tm[i].Height;

					//画面外の部分は描画しないようにする処理
					if (OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.left < 0)
					{
						OPT.AtkDat[i].Opt_LSR1.tm[j].SrcR.left = OPT.AtkDat[i].Opt_LSR1.tm[j].SrcR.left - OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.left;
						OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.left = 0;
					}
					else if (OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.right > WinXMax)
					{
						OPT.AtkDat[i].Opt_LSR1.tm[j].SrcR.right = OPT.AtkDat[i].Opt_LSR1.tm[j].SrcR.right - (OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.right - WinXMax);
						OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.right = WinXMax;
					}
					if (OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.top < 0)
					{
						OPT.AtkDat[i].Opt_LSR1.tm[j].SrcR.top = OPT.AtkDat[i].Opt_LSR1.tm[j].SrcR.top - OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.top;
						OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.top = 0;
					}

					//フレームを1減らして0になったらアニメパターンを進める
					if (!(OPT.AtkDat[i].Opt_LSR1.tm[j].FrameCnt--))
					{
						OPT.AtkDat[i].Opt_LSR1.tm[j].AnimeCnt = (OPT.AtkDat[i].Opt_LSR1.tm[j].AnimeCnt + 1) % OPT.AtkDat[i].Opt_LSR1.tm[j].Anime; //アニメパターンを進める
						OPT.AtkDat[i].Opt_LSR1.tm[j].FrameCnt = OPT.AtkDat[i].Opt_LSR1.tm[j].AnimeFrame[OPT.AtkDat[i].Opt_LSR1.tm[j].AnimeCnt];	  //表示するアニメパターンのフレーム数セット
					}
				}
				else if (OPT.AtkDat[i].Opt_LSR1.tm[j].fTama == _TOFF)
				{
					//再発射可能なレーザーの番号を保存する
					if (LsrNum[i] == OPT.AtkDat[i].Opt_LSR1.TTama)
					{
						LsrNum[i] = j;
						OPT.AtkDat[i].Opt_LSR1.tm[j].fTama |= _TSET; //レーザーを再び発射セット
					}
					else if (!(fJIKI & _OPTWEP_ON))
					{
						break; //画面に弾が存在しなければループを抜ける
					}
				}
			}
		}
		break;
	default:
		break;
	}

	switch (Wpd.flag & _MSR_LVCHK)
	{ //装備のレベルを調べている
	case _MSR1_ON:
		for (i = 0; i < OPT.OptLV; i++)
		{
			if (!OPT.AtkDat[i].Opt_MSR1.WTamaCnt)
			{
				//ミサイルが指定された数以上に画面に存在しないようにする処理
				if (MsrNum[i] < OPT.AtkDat[i].Opt_MSR1.TTama)
				{
					//画面上に弾が発射されているフラグオン
					fJIKI |= _OPTWEP_ON;
					//発射中のフラグを立てる
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].fTama |= _TON;
					//弾アニメーションを初期化
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].AnimeCnt = 0;
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].FrameCnt = OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].AnimeFrame[0];
					//ミサイルの発射された座標を初期化
					//画面座標をセット
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].Cdn.x = OPT.Jewel[i].Cdn.x + 2;
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].Cdn.y = OPT.Jewel[i].Cdn.y - 10;
					//弾の移動計算用に画面座標を取得
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].dCdn.x = OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].Cdn.x;
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].dCdn.y = OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].Cdn.y;
					//絶対座標をセット
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].MapCdn.x = MainCdn.x + OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].Cdn.x;
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].MapCdn.y = MainCdn.y + OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].Cdn.y;

					//新しく弾を発射できないようにしています
					//この処理はTTamaNumが上書きされない限り
					MsrNum[i] = OPT.AtkDat[i].Opt_MSR1.TTama;
				}
				OPT.AtkDat[i].Opt_MSR1.WTamaCnt = OPT.AtkDat[i].Opt_MSR1.WTama; //発射までのカウンタを再セット
			}

			for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_MSR1.TTama; j++)
			{
				//発射中フラグの立っていない弾は処理しない
				if (OPT.AtkDat[i].Opt_MSR1.tm[j].fTama & _TON)
				{
					TamaS = TRUE; //ミサイルは画面内に存在している

					//ミサイルを移動させる
					OPT.AtkDat[i].Opt_MSR1.tm[j].dCdn.x += OPT.AtkDat[i].Opt_MSR1.tm[j].Mov.x;
					OPT.AtkDat[i].Opt_MSR1.tm[j].dCdn.y -= OPT.AtkDat[i].Opt_MSR1.tm[j].Mov.y;
					OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.x = (LONG)OPT.AtkDat[i].Opt_MSR1.tm[j].dCdn.x;
					OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.y = (LONG)OPT.AtkDat[i].Opt_MSR1.tm[j].dCdn.y;
					OPT.AtkDat[i].Opt_MSR1.tm[j].MapCdn.x = MainCdn.x + OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.x;
					OPT.AtkDat[i].Opt_MSR1.tm[j].MapCdn.y = MainCdn.y + OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.y;

					//ミサイルが画面の外に移動していないかチェック
					if (OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.x + OPT.AtkDat[i].Opt_MSR1.tm[j].Width < 0 ||
						OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.x > WinXMax ||
						OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.y + OPT.AtkDat[i].Opt_MSR1.tm[j].Height < 0)
					{
						OPT.AtkDat[i].Opt_MSR1.tm[j].fTama &= _TOFF; //フラグをクリア
						continue;
					}

					// SPサーフェスからの転送矩形範囲をセット
					OPT.AtkDat[i].Opt_MSR1.tm[j].SrcR = TamSPJIKI[OPT.AtkDat[i].Opt_MSR1.tm[j].AnimePat[OPT.AtkDat[i].Opt_MSR1.tm[j].AnimeCnt]];

					OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.left = OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.x;
					OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.top = OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.y;
					OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.right = OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.x + OPT.AtkDat[i].Opt_MSR1.tm[j].Width;
					OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.bottom = OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.y + OPT.AtkDat[i].Opt_MSR1.tm[j].Height;

					//画面外の部分は描画しないようにする処理
					if (OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.left < 0)
					{
						OPT.AtkDat[i].Opt_MSR1.tm[j].SrcR.left = OPT.AtkDat[i].Opt_MSR1.tm[j].SrcR.left - OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.left;
						OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.left = 0;
					}
					if (OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.right > WinXMax)
					{
						OPT.AtkDat[i].Opt_MSR1.tm[j].SrcR.right = OPT.AtkDat[i].Opt_MSR1.tm[j].SrcR.right - (OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.right - WinXMax);
						OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.right = WinXMax;
					}
					if (OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.top < 0)
					{
						OPT.AtkDat[i].Opt_MSR1.tm[j].SrcR.top = OPT.AtkDat[i].Opt_MSR1.tm[j].SrcR.top - OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.top;
						OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.top = 0;
					}

					//フレームを1減らして0になったらアニメパターンを進める
					if (!(OPT.AtkDat[i].Opt_MSR1.tm[j].FrameCnt--))
					{
						OPT.AtkDat[i].Opt_MSR1.tm[j].AnimeCnt = (OPT.AtkDat[i].Opt_MSR1.tm[j].AnimeCnt + 1) % OPT.AtkDat[i].Opt_MSR1.tm[j].Anime; //アニメパターンを進める
						OPT.AtkDat[i].Opt_MSR1.tm[j].FrameCnt = OPT.AtkDat[i].Opt_MSR1.tm[j].AnimeFrame[OPT.AtkDat[i].Opt_MSR1.tm[j].AnimeCnt];	  //表示するアニメパターンのフレーム数セット
					}
				}
				else if (OPT.AtkDat[i].Opt_MSR1.tm[j].fTama == _TOFF)
				{
					//再発射可能なミサイルの番号を保存する
					if (MsrNum[i] == OPT.AtkDat[i].Opt_MSR1.TTama)
					{
						MsrNum[i] = j;
						OPT.AtkDat[i].Opt_MSR1.tm[j].fTama |= _TSET; //ミサイルを再び発射セット
					}
					else if (!(fJIKI & _OPTWEP_ON))
					{
						break; //画面に弾が存在していない場合はループを抜ける
					}
				}
			}
		}
		break;
	default:
		break;
	}

	//弾が存在していなかったら主砲発射フラグをオフ
	if (!TamaS)
		fJIKI &= _OPTWEP_OFF;
}

//////////////////////////////////////////////////////////////////////
//	発射した弾と敵キャラとの当たり判定を行なう
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptAttackAtari()
{
	short i, j, k;

	//発射されているレーザーと敵キャラの当たり判定
	switch (Wpd.flag & _LSR_LVCHK)
	{
	case _LSR1_ON:
		for (i = 0; i < OPT.OptLV; i++)
		{ //現在装備しているオプションの数だけループ
			for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_LSR1.TTama; j++)
			{
				//未発射の弾とは当たり判定をしない
				if (OPT.AtkDat[i].Opt_LSR1.tm[j].fTama & _TON)
				{
					if (!(Atari(OPT.AtkDat[i].Opt_LSR1.tm[j].DesR)))
					{
						JikiTekiDmg(_LSR1DMG); //敵キャラにダメージ
					}

					//破壊可能な弾との当たり判定
					if (SP_Tamas3)
					{
						for (k = 0; (DWORD)k < SP_Tamas3->TTama; k++)
						{
							if (SP_Tamas3->tm3[k].fTama & _TON)
							{
								if (!(Atari(OPT.AtkDat[i].Opt_LSR1.tm[j].DesR, SP_Tamas3->tm3[k].DesR)))
								{
									JikiTamas3Dmg(_LSR1DMG, k);
								}
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

	//発射されているミサイルと敵キャラの当たり判定
	switch (Wpd.flag & _MSR_LVCHK)
	{
	case _MSR1_ON:
		for (i = 0; i < OPT.OptLV; i++)
		{ //現在装備しているオプションの数だけループ
			for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_MSR1.TTama; j++)
			{
				//未発射の弾とは当たり判定をしない
				if (OPT.AtkDat[i].Opt_MSR1.tm[j].fTama & _TON)
				{
					if (!(Atari(OPT.AtkDat[i].Opt_MSR1.tm[j].DesR)))
					{
						//ミサイルを消滅させる
						OPT.AtkDat[i].Opt_MSR1.tm[j].fTama &= _TOFF; //フラグをクリア
						JikiTekiDmg(_MSR1DMG);						 //敵キャラにダメージ
					}

					//破壊可能な弾との当たり判定
					if (SP_Tamas3)
					{
						for (k = 0; (DWORD)k < SP_Tamas3->TTama; k++)
						{
							if (SP_Tamas3->tm3[k].fTama & _TON)
							{
								if (!(Atari(OPT.AtkDat[i].Opt_MSR1.tm[j].DesR, SP_Tamas3->tm3[k].DesR)))
								{
									//ミサイルを消滅させる
									OPT.AtkDat[i].Opt_MSR1.tm[j].fTama &= _TOFF; //フラグをクリア
									JikiTamas3Dmg(_MSR1DMG, k);
								}
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
//	オプションを描画する
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptRend()
{
	for (short i = 0; i < OPT.OptLV; i++)
	{
		//オプションの描画
		lpDDSBack->BltFast(OPT.Jewel[i].DesR.left, OPT.Jewel[i].DesR.top,
						   lpDDS_JIKI, &OPT.Jewel[i].SrcR,
						   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}
}

//////////////////////////////////////////////////////////////////////
//	オプションの発射した弾の描画
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptWepRend()
{
	short i, j;

	//レーザーの描画
	switch (Wpd.flag & _LSR_LVCHK)
	{
	case _LSR1_ON: //レーザー１
		for (i = 0; i < OPT.OptLV; i++)
		{
			for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_LSR1.TTama; j++)
			{
				//発射中フラグの立っていない弾は描画しない
				if (OPT.AtkDat[i].Opt_LSR1.tm[j].fTama & _TON)
				{
					lpDDSBack->BltFast(OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.left, OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.top,
									   lpDDS_JIKI, &OPT.AtkDat[i].Opt_LSR1.tm[j].SrcR,
									   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
				}
			}
		}
		break;
	default:
		break;
	}
	//ミサイルの描画
	switch (Wpd.flag & _MSR_LVCHK)
	{
	case _MSR1_ON: //ミサイル１
		for (i = 0; i < OPT.OptLV; i++)
		{
			for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_MSR1.TTama; j++)
			{
				//発射中フラグの立っていない弾は描画しない
				if (OPT.AtkDat[i].Opt_MSR1.tm[j].fTama & _TON)
				{
					lpDDSBack->BltFast(OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.left, OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.top,
									   lpDDS_JIKI, &OPT.AtkDat[i].Opt_MSR1.tm[j].SrcR,
									   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
				}
			}
		}
		break;
	default:
		break;
	}
}