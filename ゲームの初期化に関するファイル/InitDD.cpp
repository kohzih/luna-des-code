/*====================================================================
	DirectDraw初期化に関するソースファイル
=====================================================================*/

#include <windows.h>
#include <ddraw.h>

#include "Macros.h"

extern HINSTANCE hInst;	   //インスタンスハンドル
extern HWND hWnd;		   //メインウィンドウハンドル
extern BOOL fFullscreen;   //フルスクリーン(1)orウィンドウフラグ(0)
extern DWORD DisplayColor; //スクリーンの現在のビットカラー値

extern LPDIRECTDRAW7 lpDD;				  // DirectDrawオブジェクト
extern LPDIRECTDRAWSURFACE7 lpDDSPrimary, //プライマリサーフェース
	lpDDSBack;							  //バックサーフェース
extern LPDIRECTDRAWPALETTE lpDDpal;		  // DirectDrawパレットオブジェクト
extern LPDIRECTDRAWCLIPPER lpDDClip;	  //クリッパー(ウィンドウ表示用)

PALETTEENTRY ape[256]; // 256色パレット

BOOL FullChgWindow(void);
BOOL LoadPalette(char *);
//列挙関数のプロトタイプ宣言
HRESULT WINAPI EnumModesCallback2(LPDDSURFACEDESC2, LPVOID);

//---------------------------------------------------------
// DirectDraw初期化
//---------------------------------------------------------
BOOL InitDD()
{

	HRESULT ret;
	DDSURFACEDESC2 ddsd;
	DDSCAPS2 ddsCaps;

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////

	// DDオブジェクト作成
	ret = DirectDrawCreateEx(NULL, (void **)&lpDD, IID_IDirectDraw7, NULL);
	if (ret != DD_OK)
	{
		MessageBox(hWnd, "001", "エラー", MB_OK | MB_APPLMODAL);
		return FALSE;
	}

	//指定された画面モードが使用可能かチェック
	HRESULT ret2;

	ret2 = lpDD->EnumDisplayModes(DDEDM_REFRESHRATES,
								  NULL, (LPVOID)&ret,
								  EnumModesCallback2);
	if (ret2 == DDERR_INVALIDOBJECT)
		MessageBox(hWnd, "DDERR_INVALIDOBJECT", "ret2", MB_OK | MB_APPLMODAL);
	if (ret2 == DDERR_INVALIDPARAMS)
		MessageBox(hWnd, "DDERR_INVALIDPARAMS", "ret2", MB_OK | MB_APPLMODAL);

	if (ret != TRUE)
	{
		MessageBox(hWnd,
				   "残念ながらゲームの動作環境を満たしていません",
				   "ゲームができません！！",
				   MB_OK | MB_APPLMODAL);
		return FALSE;
	}

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////

	//協調レベル設定
	//フルスクリーンモード
	if (fFullscreen)
	{
		ret = lpDD->SetCooperativeLevel(hWnd,
										DDSCL_FULLSCREEN |
											DDSCL_EXCLUSIVE |
											DDSCL_ALLOWMODEX |
											DDSCL_ALLOWREBOOT);
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "002", "エラー", MB_OK | MB_APPLMODAL);
			return FALSE;
		}
		//ディスプレイモード設定
		ret = lpDD->SetDisplayMode(WinXMax, WinYMax, BPP, 0, 0);
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "003", "エラー", MB_OK | MB_APPLMODAL);
			return FALSE;
		}
	}
	//ウィンドウモード
	else
	{
		ret = lpDD->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "004", "エラー", MB_OK | MB_APPLMODAL);
			return FALSE;
		}
	}

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////

	//プライマリサーフェスの作成

	//フルスクリーンモード
	if (fFullscreen)
	{

		//プライマリサーフェスの作成
		memset(&ddsd, 0, sizeof(ddsd)); // ddsdの中を0で初期化
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;	   //バックバッファ作成
		ddsd.dwBackBufferCount = 1;							   //バックバッファ数
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |		   //プライマリサーフェース指定
							  DDSCAPS_COMPLEX |				   //複合サーフェース指定
							  DDSCAPS_FLIP;					   //フリップ指定
		ret = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL); //サーフェース指定
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "005", "エラー", MB_OK | MB_APPLMODAL);
			return FALSE;
		}

		//バックサーフェスを作成
		ZeroMemory(&ddsCaps, sizeof(ddsCaps));
		ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
		ret = lpDDSPrimary->GetAttachedSurface(&ddsCaps, &lpDDSBack);
		//複合サーフェスのバックサーフェースを取得
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "006", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_INVALIDOBJECT)
				MessageBox(hWnd, "DDERR_INVALIDOBJECT", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_INVALIDPARAMS)
				MessageBox(hWnd, "DDERR_INVALIDPARAMS", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOTFOUND)
				MessageBox(hWnd, "DDERR_NOTFOUND", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_SURFACELOST)
				MessageBox(hWnd, "DDERR_SURFACELOST", "エラー", MB_OK | MB_APPLMODAL);
			return FALSE;
		}
		// 1ピクセルあたりのビット数をセット
		DisplayColor = BPP;
	}
	//ウィンドウモード
	else
	{
		//プライマリサーフェスの作成
		memset(&ddsd, 0, sizeof(ddsd)); // ddsdの中を0で初期化
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |		   //プライマリサーフェース指定
							  DDSCAPS_SYSTEMMEMORY;			   //システムメモリに確保
		ret = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL); //サーフェース指定
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "007", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_INCOMPATIBLEPRIMARY)
				MessageBox(hWnd, "A", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_INVALIDCAPS)
				MessageBox(hWnd, "B", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_INVALIDOBJECT)
				MessageBox(hWnd, "C", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_INVALIDPARAMS)
				MessageBox(hWnd, "D", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_INVALIDPIXELFORMAT)
				MessageBox(hWnd, "E", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOALPHAHW)
				MessageBox(hWnd, "F", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOCOOPERATIVELEVELSET)
				MessageBox(hWnd, "G", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NODIRECTDRAWHW)
				MessageBox(hWnd, "H", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOEMULATION)
				MessageBox(hWnd, "I", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOEXCLUSIVEMODE)
				MessageBox(hWnd, "J", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOFLIPHW)
				MessageBox(hWnd, "K", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOMIPMAPHW)
				MessageBox(hWnd, "L", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOOVERLAYHW)
				MessageBox(hWnd, "M", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOZBUFFERHW)
				MessageBox(hWnd, "N", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_OUTOFMEMORY)
				MessageBox(hWnd, "O", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_OUTOFVIDEOMEMORY)
				MessageBox(hWnd, "P", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_PRIMARYSURFACEALREADYEXISTS)
				MessageBox(hWnd, "Q", "エラー", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_UNSUPPORTEDMODE)
				MessageBox(hWnd, "R", "エラー", MB_OK | MB_APPLMODAL);

			return FALSE;
		}

		//現在の画面のカラー設定を取得
		ddsd.dwSize = sizeof(ddsd);
		lpDDSPrimary->GetSurfaceDesc(&ddsd);
		DisplayColor = ddsd.ddpfPixelFormat.dwRGBBitCount;
		//現在のディスプレスモードが256色モード以外の場合の処理
		if (!(DisplayColor == 8))
		{
			MessageBox(hWnd,
					   "フルスクリーンモードへ移行します",
					   "画面が256色モードではありません",
					   MB_OK | MB_APPLMODAL);
			fFullscreen = 1; //フラグをフルスクリーンモードにセット
			RELEASE(lpDDSPrimary);
			RELEASE(lpDD);

			return InitDD(); //初期化をやり直す
		}

		//バックサーフェス的オフスクリーンサーフェスの作成
		memset(&ddsd, 0, sizeof(ddsd)); // ddsdの中を0で初期化
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		ddsd.dwWidth = WinXMax;
		ddsd.dwHeight = WinYMax;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		ret = lpDD->CreateSurface(&ddsd, &lpDDSBack, NULL);
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "008", "エラー", MB_OK | MB_APPLMODAL);
			return FALSE;
		}

		//クリッパーの設定
		ret = lpDD->CreateClipper(0, &lpDDClip, NULL);
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "009", "エラー", MB_OK | MB_APPLMODAL);
			return FALSE;
		}
		ret = lpDDClip->SetHWnd(0, hWnd);
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "010", "エラー", MB_OK | MB_APPLMODAL);
			return FALSE;
		}
		ret = lpDDSPrimary->SetClipper(lpDDClip);
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "011", "エラー", MB_OK | MB_APPLMODAL);
			return FALSE;
		}
	}

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////

	//プライマリサーフェスにパレットを設定
	ret = LoadPalette(".\\BitMap\\Prolog.bmp");
	if (!ret)
	{
		MessageBox(hWnd, "LoadPalette", "エラー", MB_OK | MB_APPLMODAL);
		return FALSE;
	}

	//メモリ、デバイス情報を調べる
	/*
	DDCAPS	hwCaps,swCaps;					//ハードウェア、ソフトウェア

	memset(&hwCaps,0,sizeof(DDCAPS));
	hwCaps.dwSize=sizeof(DDCAPS);
	memset(&swCaps,0,sizeof(DDCAPS));
	swCaps.dwSize=sizeof(DDCAPS);

	lpDD->GetCaps(&hwCaps,&swCaps);

	TotalMem=hwCaps.dwVidMemTotal;			//VRAMの空き容量を調べる
	FreeMem=hwCaps.dwVidMemFree;
	*/
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

//---------------------------------------------------------
//ビットマップからパレットを読み込む関数
//---------------------------------------------------------
BOOL LoadPalette(char *FileName)
{
	//ロードに使用する変数
	HDC hdc, memDC;
	HBITMAP hBit;
	BITMAP bm;

	//システムパレットの取得
	hdc = GetDC(0);
	GetSystemPaletteEntries(hdc, 0, 256, ape);
	ReleaseDC(0, hdc);

	//カラーパレットの読み込み
	lpDDSPrimary->GetDC(&hdc);
	memDC = CreateCompatibleDC(hdc);

	//ビットマップロード
	hBit = (HBITMAP)LoadImage(hInst,								  //インスタンスハンドル
							  FileName,								  //ファイル名
							  IMAGE_BITMAP,							  // IMAGE_BITMAP指定
							  0, 0,									  //読み込む幅と高さ(0指定で全て読み込み)
							  LR_CREATEDIBSECTION | LR_LOADFROMFILE); //フラグ
	//ファイルオープン失敗
	if (!hBit)
	{
		MessageBox(hWnd, "File Open Error", FileName, MB_OK);
		return FALSE;
	}

	GetObject(hBit, sizeof(bm), &bm);
	//パレットが２５６色以外のビットマップをロードした場合
	if (!(bm.bmBitsPixel == 8))
	{
		MessageBox(hWnd, "File Open Error(Not256ColorBitMap)", FileName, MB_OK | MB_APPLMODAL);
		return FALSE;
	}

	SelectObject(memDC, hBit);
	//画像から読み込んだ0番から236番までのパレットを、
	//既にシステムパレットを読み込んだape構造体変数に10番から上書き
	GetDIBColorTable(memDC, 0, 236, (RGBQUAD *)&ape[10]);

	DeleteDC(memDC);
	lpDDSPrimary->ReleaseDC(hdc);

	//パレットデータの入れ替え、システムパレットは変更しない
	for (int i = 10; i < 246; i++)
	{
		BYTE temp;
		//青と赤を入れ替え
		temp = ape[i].peRed;
		ape[i].peRed = ape[i].peBlue;
		ape[i].peBlue = temp;
		//フラグを設定
		ape[i].peFlags = PC_RESERVED;
	}

	//パレットオブジェクトの作成
	lpDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, ape, &lpDDpal, 0);
	lpDDSPrimary->SetPalette(lpDDpal);

	return TRUE;
}

//---------------------------------------------------------
//指定された画面モードが使用可能な時に呼び出される列挙関数
//---------------------------------------------------------
HRESULT WINAPI EnumModesCallback2(LPDDSURFACEDESC2 LPddsd, LPVOID lpContext)
{
	if (LPddsd->dwWidth == (UINT)WinXMax &&
		LPddsd->dwHeight == (UINT)WinYMax &&
		LPddsd->ddpfPixelFormat.dwRGBBitCount == (UINT)BPP)
	{
		*(DWORD *)lpContext = TRUE;
		return DDENUMRET_CANCEL;
	}
	*(DWORD *)lpContext = FALSE;
	return DDENUMRET_OK;
}

//---------------------------------------------------------
// DirectDrawオブジェクト解放(作成した順の逆順を基本に解放)
//---------------------------------------------------------
void ReleaseDD(void)
{
	RELEASE(lpDDpal);
	RELEASE(lpDDClip)
	RELEASE(lpDDSBack);
	RELEASE(lpDDSPrimary);
	RELEASE(lpDD);
}
