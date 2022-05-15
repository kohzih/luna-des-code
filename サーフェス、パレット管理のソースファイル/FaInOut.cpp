/*=============================================================================
	フェードインアウトなどのパレット管理処理のソースファイル
==============================================================================*/
#include <windows.h>
#include <ddraw.h>

#include "Macros.h"

#include "FaInOut.h"
#include "DrawMacros.h"

extern LPDIRECTDRAWPALETTE lpDDpal; // DirectDrawパレットオブジェクト

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

FaInOut::FaInOut(short StP, short PalE)
{
	//現在の設定はアウトからインへ
	WFadeF = FALSE;	 //ホワイトアウトからスタート
	BFadeF = FALSE;	 //ブラックアウトからスタート
	LBFadeF = FALSE; // LittleBlack管理用のフラグ初期化
	PalCnt = 0;		 //現在の進行状況(PalCnt>>16で0～255)

	StartPal = StP;	 //変化を開始させる先頭パレット番号
	PalEntry = PalE; //変化させるパレットの個数

	lpDDpal->GetEntries(0, 0, 256, apeH1); //現在のパレットをapeH1にコピー

	memcpy(apeH2, apeH1, sizeof(PALETTEENTRY) * 256); //現在のパレットをapeH2にコピー

	for (int i = 0; i < 256; i++) //現在のパレットをapeH3にコピー
	{
		apeH3[i].r = (int)apeH2[i].peRed << 16;
		apeH3[i].g = (int)apeH2[i].peGreen << 16;
		apeH3[i].b = (int)apeH2[i].peBlue << 16;
	}
}

FaInOut::~FaInOut()
{
}

///////////////////////////////////////////////////////////
// フェードインアウトの既定値となるパレットを取得する
///////////////////////////////////////////////////////////
BOOL FaInOut::SetPaletteEntry()
{
	HRESULT ret;
	ret = lpDDpal->GetEntries(0, 0, 256, apeH1); //現在のパレットをapeH1にコピー
	if (ret != DD_OK)
		return FALSE;

	memcpy(apeH2, apeH1, sizeof(PALETTEENTRY) * 256); //現在のパレットをapeH2にコピー

	for (int i = 0; i < 256; i++) //現在のパレットをapeH3にコピー
	{
		apeH3[i].r = (int)apeH2[i].peRed << 16;
		apeH3[i].g = (int)apeH2[i].peGreen << 16;
		apeH3[i].b = (int)apeH2[i].peBlue << 16;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////
// パレットとフラグを各フェードインアウトメソッドが
// 即実行できるように設定する
// 引数:フラグ
///////////////////////////////////////////////////////////
void FaInOut::SetFade(DWORD SetF)
{
	//ホワイトインの準備を行う
	if (SetF & _SetWhiteIn)
	{
		//フラグをセット
		WFadeF = TRUE;
		// 0～9、246～255まではシステムカラーのため変更しない
		//全てのパレットを白に設定
		for (int i = StartPal; i < StartPal + PalEntry; i++)
		{
			apeH2[i].peRed = 255;
			apeH2[i].peGreen = 255;
			apeH2[i].peBlue = 255;
			apeH3[i].r = (int)apeH2[i].peRed << 16;
			apeH3[i].g = (int)apeH2[i].peGreen << 16;
			apeH3[i].b = (int)apeH2[i].peBlue << 16;
		}
		//必ず変更するパレットの先頭アドレスを指定
		lpDDpal->SetEntries(0, StartPal, PalEntry, &apeH2[StartPal]);
		//ブラックインの準備を行う
	}
	else if (SetF & _SetBlackIn)
	{
		BFadeF = TRUE;
		//全てのパレットを黒に設定
		for (int i = StartPal; i < StartPal + PalEntry; i++)
		{
			apeH2[i].peRed = 0;
			apeH2[i].peGreen = 0;
			apeH2[i].peBlue = 0;
			apeH3[i].r = (int)apeH2[i].peRed << 16;
			apeH3[i].g = (int)apeH2[i].peGreen << 16;
			apeH3[i].b = (int)apeH2[i].peBlue << 16;
		}
		//必ず変更するパレットの先頭アドレスを指定
		lpDDpal->SetEntries(0, StartPal, PalEntry, &apeH2[StartPal]);
		//ホワイトアウト、ブラックアウトの準備を行う
	}
	else if (SetF & _SetDef)
	{
		WFadeF = FALSE;
		BFadeF = FALSE;
		LBFadeF = FALSE;
		//全てのパレットを元に戻す
		for (int i = StartPal; i < StartPal + PalEntry; i++)
		{
			apeH2[i].peRed = apeH1[i].peRed;
			apeH2[i].peGreen = apeH1[i].peGreen;
			apeH2[i].peBlue = apeH1[i].peBlue;
			apeH3[i].r = (int)apeH2[i].peRed << 16;
			apeH3[i].g = (int)apeH2[i].peGreen << 16;
			apeH3[i].b = (int)apeH2[i].peBlue << 16;
		}
		//必ず変更するパレットの先頭アドレスを指定
		lpDDpal->SetEntries(0, StartPal, PalEntry, &apeH2[StartPal]);
	}
}

///////////////////////////////////////////////////////////
//	画面を少し暗くする(245番とシステムパレットは変更しない)
//	引数:1:色要素を減らす値 2:暗くする作業を繰り返すか示すフラグ(0:一回のみ 1:繰り返す)
//	戻り値:暗くする処理の途中の場合FALSEを返す。終了した場合TRUE。
///////////////////////////////////////////////////////////
BOOL FaInOut::LittleBlack(int BLACK, bool flag)
{
	//一回のみの場合
	if (!flag && !LBFadeF)
	{
		// 0～9までと245、246～255までは変更しない
		for (int i = StartPal; i < StartPal + PalEntry - 1; i++)
		{
			//赤の要素を減色
			apeH3[i].r = apeH3[i].r - BLACK;
			if (apeH3[i].r >> 16 <= 0)
			{ //減色した結果要素が0より小さくなった場合、0をセット
				apeH3[i].r = 0;
				apeH2[i].peRed = 0;
			}
			else
				apeH2[i].peRed = apeH3[i].r >> 16;
			//緑の要素を減色
			apeH3[i].g = apeH3[i].g - BLACK;
			if (apeH3[i].g >> 16 <= 0)
			{
				apeH3[i].g = 0;
				apeH2[i].peGreen = 0;
			}
			else
				apeH2[i].peGreen = apeH3[i].g >> 16;
			//青の要素を減色
			apeH3[i].b = apeH3[i].b - BLACK;
			if (apeH3[i].b >> 16 <= 0)
			{
				apeH3[i].b = 0;
				apeH2[i].peBlue = 0;
			}
			apeH2[i].peBlue = apeH3[i].b >> 16;
		}

		//必ず変更するパレットの先頭アドレスを指定
		lpDDpal->SetEntries(0, StartPal, PalEntry - 1, &apeH2[10]);

		LBFadeF = TRUE; //暗くする作業終了

		//メソッドが実行される限り繰り返す場合
	}
	else if (flag == TRUE && !LBFadeF)
	{
		// 0～9までと245、246～255までは変更しない
		for (int i = StartPal; i < StartPal + PalEntry - 1; i++)
		{
			//赤の要素を減色
			apeH3[i].r = apeH3[i].r - BLACK;
			if (apeH3[i].r >> 16 <= 0)
			{ //減色した結果要素が0より小さくなった場合、0をセット
				apeH3[i].r = 0;
				apeH2[i].peRed = 0;
			}
			else
				apeH2[i].peRed = apeH3[i].r >> 16;
			//緑の要素を減色
			apeH3[i].g = apeH3[i].g - BLACK;
			if (apeH3[i].g >> 16 <= 0)
			{
				apeH3[i].g = 0;
				apeH2[i].peGreen = 0;
			}
			else
				apeH2[i].peGreen = apeH3[i].g >> 16;
			//青の要素を減色
			apeH3[i].b = apeH3[i].b - BLACK;
			if (apeH3[i].b >> 16 <= 0)
			{
				apeH3[i].b = 0;
				apeH2[i].peBlue = 0;
			}
			apeH2[i].peBlue = apeH3[i].b >> 16;
		}

		//必ず変更するパレットの先頭アドレスを指定
		lpDDpal->SetEntries(0, StartPal, PalEntry - 1, &apeH2[10]);
	}
	else
	{
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////
// ホワイトアウト
// 引数:パレットを変化させるスピード
// 戻り値:TRUE(ホワイトアウト),FALSE(ホワイトアウトの途中)
///////////////////////////////////////////////////////////
BOOL FaInOut::WhiteOut(int PalSpeed)
{
	//フラグが立っている時は終了
	if (WFadeF)
		return TRUE;

	if (PalCnt >> 16 > 255) //パレットの各ブロックを255回増色したら終了
	{
		PalCnt = 0;
		WFadeF = 1; //ホワイトイン用フラグを立てる
		return TRUE;
	}

	PalCnt += PalSpeed; //パレットを増色した回数
						//(1回のループでPalSpeed分しか増えない)

	// 0～9、246～255まではシステムカラーのため変更しない
	for (int i = StartPal; i < StartPal + PalEntry; i++)
	{
		//赤の要素が255より小さいならば増色
		if (apeH3[i].r >> 16 < 255)
		{
			apeH3[i].r = apeH3[i].r + PalSpeed;
			apeH2[i].peRed = apeH3[i].r >> 16;
			//増色した結果要素が255より大きくなった場合、255をセット
			if (apeH3[i].r >> 16 >= 255)
			{
				apeH3[i].r = 255 << 16;
				apeH2[i].peRed = 255;
			}
		}
		if (apeH3[i].g >> 16 < 255)
		{
			apeH3[i].g = apeH3[i].g + PalSpeed;
			apeH2[i].peGreen = apeH3[i].g >> 16;
			if (apeH3[i].g >> 16 >= 255)
			{
				apeH3[i].g = 255 << 16;
				apeH2[i].peGreen = 255;
			}
		}
		if (apeH3[i].b >> 16 < 255)
		{
			apeH3[i].b = apeH3[i].b + PalSpeed;
			apeH2[i].peBlue = apeH3[i].b >> 16;
			if (apeH3[i].b >> 16 >= 255)
			{
				apeH3[i].b = 255 << 16;
				apeH2[i].peBlue = 255;
			}
		}
	}

	//必ず変更するパレットの先頭アドレスを指定
	lpDDpal->SetEntries(0, StartPal, PalEntry, &apeH2[StartPal]);

	return FALSE;
}
///////////////////////////////////////////////////////////
// ホワイトイン
// 引数:パレットを変化させるスピード
// 戻り値:TRUE(ホワイトイン),FALSE(ホワイトインの途中)
///////////////////////////////////////////////////////////
BOOL FaInOut::WhiteIn(int PalSpeed)
{
	//フラグが立っていない時は終了
	if (!(WFadeF))
		return TRUE;

	if (PalCnt >> 16 > 255) //パレットの各ブロックを255回増色したら終了
	{
		PalCnt = 0;
		WFadeF = 0; //フラグをクリア
		return TRUE;
	}

	PalCnt += PalSpeed; //各パレットブロックをPalSpd分減色
						//(1回のループでPalSpd分しか増えない)

	// 0～9、246～255まではシステムカラーのため変更しない
	for (int i = StartPal; i < StartPal + PalEntry; i++)
	{
		//赤の要素が目標の色より大きいならば減色
		if (apeH3[i].r >> 16 > apeH1[i].peRed)
		{
			apeH3[i].r = apeH3[i].r - PalSpeed;
			apeH2[i].peRed = apeH3[i].r >> 16;
			//減色した結果要素が目標の色より小さくなった場合、元の色をセット
			if (apeH3[i].r >> 16 <= apeH1[i].peRed)
			{
				apeH3[i].r = apeH1[i].peRed << 16;
				apeH2[i].peRed = apeH1[i].peRed;
			}
		}
		if (apeH3[i].g >> 16 > apeH1[i].peGreen)
		{
			apeH3[i].g = apeH3[i].g - PalSpeed;
			apeH2[i].peGreen = apeH3[i].g >> 16;
			if (apeH3[i].g >> 16 <= apeH1[i].peGreen)
			{
				apeH3[i].g = apeH1[i].peGreen << 16;
				apeH2[i].peGreen = apeH1[i].peGreen;
			}
		}
		if (apeH3[i].b >> 16 > apeH1[i].peBlue)
		{
			apeH3[i].b = apeH3[i].b - PalSpeed;
			apeH2[i].peBlue = apeH3[i].b >> 16;
			if (apeH3[i].b >> 16 <= apeH1[i].peBlue)
			{
				apeH3[i].b = apeH1[i].peBlue << 16;
				apeH2[i].peBlue = apeH1[i].peBlue;
			}
		}
	}

	//必ず変更するパレットの先頭アドレスを指定
	lpDDpal->SetEntries(0, StartPal, PalEntry, &apeH2[10]);

	return FALSE;
}

///////////////////////////////////////////////////////////
// ブラックアウト
// 引数:パレットを変化させるスピード
// 戻り値:TRUE(ブラックアウト終了),FALSE(ブラックアウトの途中)
///////////////////////////////////////////////////////////
BOOL FaInOut::BlackOut(int PalSpeed)
{
	//フラグが立っている時は終了
	if (BFadeF)
		return TRUE;

	//パレットの各ブロックを255回増色したら終了
	if (PalCnt >> 16 > 255)
	{
		PalCnt = 0;
		BFadeF = 1; //フラグを立てる
		return TRUE;
	}

	PalCnt += PalSpeed; //各パレットブロックをPalSpd分減色
						//(1回のループでPalSpd分しか増えない)

	// 0～9、246～255まではシステムカラーのため変更しない
	for (int i = StartPal; i < StartPal + PalEntry; i++)
	{
		//赤の要素が0より大きいならば減色
		if (apeH3[i].r >> 16 > 0)
		{
			apeH3[i].r = apeH3[i].r - PalSpeed;
			apeH2[i].peRed = apeH3[i].r >> 16;
			//減色した結果要素が0より小さくなった場合、0をセット
			if (apeH3[i].r >> 16 <= 0)
			{
				apeH3[i].r = 0;
				apeH2[i].peRed = 0;
			}
		}
		if (apeH3[i].g >> 16 > 0)
		{
			apeH3[i].g = apeH3[i].g - PalSpeed;
			apeH2[i].peGreen = apeH3[i].g >> 16;
			if (apeH3[i].g >> 16 <= 0)
			{
				apeH3[i].g = 0;
				apeH2[i].peGreen = 0;
			}
		}
		if (apeH3[i].b >> 16 > 0)
		{
			apeH3[i].b = apeH3[i].b - PalSpeed;
			apeH2[i].peBlue = apeH3[i].b >> 16;
			if (apeH3[i].b >> 16 <= 0)
			{
				apeH3[i].b = 0;
				apeH2[i].peBlue = 0;
			}
		}
	}

	//必ず変更するパレットの先頭アドレスを指定
	lpDDpal->SetEntries(0, StartPal, PalEntry, &apeH2[StartPal]);

	return FALSE;
}

///////////////////////////////////////////////////////////
// ブラックイン
// 引数:パレットを変化させるスピード
// 戻り値:TRUE(ブラックイン終了),FALSE(ブラックインの途中)
///////////////////////////////////////////////////////////
BOOL FaInOut::BlackIn(int PalSpeed)
{
	//フラグが立っていない場合終了
	if (!(BFadeF))
		return TRUE;

	if (PalCnt >> 16 > 255) //パレットの各ブロックを255回増色したら終了
	{
		PalCnt = 0;
		BFadeF = 0; //フラグクリア
		return TRUE;
	}

	PalCnt += PalSpeed; //各パレットブロックをPalSpd分減色
						//(1回のループでPalSpd分しか増えない)

	// 0～9、246～255まではシステムカラーのため変更しない
	for (int i = StartPal; i < StartPal + PalEntry; i++)
	{
		//赤の要素が目標の色より小さいならば増色
		if (apeH3[i].r >> 16 < apeH1[i].peRed)
		{
			apeH3[i].r = apeH3[i].r + PalSpeed;
			apeH2[i].peRed = apeH3[i].r >> 16;
			//減色した結果要素が目標の色よりより大きくなった場合、目標の色をセット
			if (apeH3[i].r >> 16 >= apeH1[i].peRed)
			{
				apeH3[i].r = apeH1[i].peRed << 16;
				apeH2[i].peRed = apeH1[i].peRed;
			}
		}
		if (apeH3[i].g >> 16 < apeH1[i].peGreen)
		{
			apeH3[i].g = apeH3[i].g + PalSpeed;
			apeH2[i].peGreen = apeH3[i].g >> 16;
			if (apeH3[i].g >> 16 >= apeH1[i].peGreen)
			{
				apeH3[i].g = apeH1[i].peGreen << 16;
				apeH2[i].peGreen = apeH1[i].peGreen;
			}
		}
		if (apeH3[i].b >> 16 < apeH1[i].peBlue)
		{
			apeH3[i].b = apeH3[i].b + PalSpeed;
			apeH2[i].peBlue = apeH3[i].b >> 16;
			if (apeH3[i].b >> 16 >= apeH1[i].peBlue)
			{
				apeH3[i].b = apeH1[i].peBlue << 16;
				apeH2[i].peBlue = apeH1[i].peBlue;
			}
		}
	}

	//必ず変更するパレットの先頭アドレスを指定
	lpDDpal->SetEntries(0, StartPal, PalEntry, &apeH2[StartPal]);

	return FALSE;
}