/*=============================================================================
	基本クラスの処理に関するソースファイル
==============================================================================*/
#include <windows.h>
#include <math.h>
#include <ddraw.h>

#include "Macros.h"
#include "Draw.h"
#include "DrawMacros.h"

extern HINSTANCE hInst;	 //インスタンスハンドル
extern HWND hWnd;		 //メインウィンドウハンドル
extern BOOL fFullscreen; //フルスクリーン(1)orウィンドウフラグ(0)

extern LPDIRECTDRAW7 lpDD;
extern LPDIRECTDRAWSURFACE7 lpDDSBack;

LPDIRECTDRAWSURFACE7 Draw::lpDDS_JIKI; //自機関連の画像用サーフェス
LPDIRECTDRAWSURFACE7 Draw::lpDDS_SP;   //敵キャラ用画像用サーフェス
LPDIRECTDRAWSURFACE7 Draw::lpDDS_BG;   //背景画像用サーフェス
LPDIRECTDRAWSURFACE7 Draw::lpDDS_BG2;  //背景画像用サーフェス2
POINT Draw::MainCdn;				   //ゲーム内の絶対座標
DWORD Draw::fSTAGE;					   //各ステージの開始終了を示すフラグ
int Draw::StageNum;					   //現在のステージ番号
DWORD Draw::MapSizeX;				   // BGマップデータの横サイズ（実際の値はMapSizeX*ChipSizeXドット）
DWORD Draw::MapSizeY;				   // BGマップデータの縦サイズ（実際の値はMapSizeY*ChipSizeYドット）
DWORD Draw::TipSizeX;				   // BGチップ一個の横のサイズ
DWORD Draw::TipSizeY;				   // BGチップ一個の縦のサイズ
BGMAP *Draw::Parts;					   // BGマップ構造体配列の先頭アドレス
double *Draw::Sin;					   // 0-361度分のサイン値配列の先頭アドレスを格納するポインタ
double *Draw::Cos;					   // 0-361度分のコサイン値配列の先頭アドレスを格納するポインタ
double *Draw::Tan;					   // 0-89度分のタンジェント値配列の先頭アドレスを格納するポインタ
DWORD Draw::EventCnt;				   // Parts構造体配列に含まれるイベント(DrawSPオブジェクト)の数
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

Draw::Draw()
{
}

Draw::Draw(char **FileName, HRESULT &ret)
{
	lpDDS_JIKI = NULL; //各サーフェスの初期化
	lpDDS_SP = NULL;
	lpDDS_BG = NULL;
	lpDDS_BG2 = NULL;

	fSTAGE = 0;					   //フラグをクリア
	fSTAGE |= _STAGE_STARTWAIT_ON; //次のステージが始まったことを示すフラグオン
	StageNum = _StNum;			   //最初にスタートするステージ番号

	MainCdn.x = 0; //絶対座標の初期化
	MainCdn.y = 0;
	MapSizeX = 0; //背景マップデータの横サイズ（実際の値はMapSizeX*ChipSizeXドット）
	MapSizeY = 0; //背景マップデータの縦サイズ（実際の値はMapSizeY*ChipSizeYドット）
	TipSizeX = 0; //背景チップ一個の横のサイズ
	TipSizeY = 0; //背景チップ一個の縦のサイズ

	Parts = NULL;
	Sin = NULL;
	Cos = NULL;
	Tan = NULL;

	EventCnt = 0; //マップ上に配置されているイベント数初期化

	JIKIFileName = NULL; //ファイルネーム保存用のポインタを初期化
	PrologFileName = NULL;
	for (short i = 0; i < 2; i++)
	{
		TitleFileName[i] = NULL;
	}
	for (i = 0; i < 4; i++)
	{
		St1FileName[i] = NULL;
	}
	GameOverFileName = NULL;
	GameClearFileName = NULL;

	//読み込んだビットマップの名前をコピー
	int len; //文字列の長さを格納する変数

	//自機描画用ビットマップファイルネーム
	len = strlen(FileName[0]);
	JIKIFileName = new char[len + 1];
	if (JIKIFileName == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(JIKIFileName, FileName[0]);

	//プロローグ用ファイルネーム
	len = strlen(FileName[1]);
	PrologFileName = new char[len + 1];
	if (PrologFileName == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(PrologFileName, FileName[1]);

	//タイトル用ファイルネーム
	len = strlen(FileName[2]);
	TitleFileName[0] = new char[len + 1];
	if (TitleFileName[0] == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(TitleFileName[0], FileName[2]);

	len = strlen(FileName[3]);
	TitleFileName[1] = new char[len + 1];
	if (TitleFileName[1] == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(TitleFileName[1], FileName[3]);

	//ステージ1用各ファイルネーム
	len = strlen(FileName[4]);
	St1FileName[0] = new char[len + 1];
	if (St1FileName[0] == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(St1FileName[0], FileName[4]);

	len = strlen(FileName[5]);
	St1FileName[1] = new char[len + 1];
	if (St1FileName[1] == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(St1FileName[1], FileName[5]);

	len = strlen(FileName[6]);
	St1FileName[2] = new char[len + 1];
	if (St1FileName[2] == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(St1FileName[2], FileName[6]);

	len = strlen(FileName[7]);
	St1FileName[3] = new char[len + 1];
	if (St1FileName[3] == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(St1FileName[3], FileName[7]);

	//ゲームオーバー画面描画用ビットマップファイル名
	len = strlen(FileName[8]);
	GameOverFileName = new char[len + 1];
	if (GameOverFileName == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(GameOverFileName, FileName[8]);

	//ゲームクリア画面用ビットマップファイル名
	len = strlen(FileName[9]);
	GameClearFileName = new char[len + 1];
	if (GameClearFileName == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(GameClearFileName, FileName[9]);

	//サイン、コサイン、タンジェント値を格納する領域を確保
	Sin = new double[361];
	if (Sin == NULL)
	{
		ret = FALSE;
		return;
	}
	Cos = new double[361];
	if (Cos == NULL)
	{
		ret = FALSE;
		return;
	}
	Tan = new double[90];
	if (Tan == NULL)
	{
		ret = FALSE;
		return;
	}
	//サインコサイン値を求める
	for (i = 0; i < 361; i++)
	{
		Cos[i] = cos(3.1415926535 / 180 * i);
		Sin[i] = sin(3.1415926535 / 180 * i);
	}
	//タンジェント値を求める
	for (i = 0; i < 90; i++)
	{
		Tan[i] = tan(3.1415926535 / 180 * i);
	}
	//引数を利用した戻り値
	ret = TRUE;
}

Draw::~Draw()
{
}

//////////////////////////////////////////////////////////////////////
// Drawクラスの各メンバを初期化
//////////////////////////////////////////////////////////////////////
BOOL Draw::Init()
{
	//ステージを複数作った場合に必要な処理
	RELEASE(lpDDS_JIKI);
	RELEASE(lpDDS_SP);
	RELEASE(lpDDS_BG);
	RELEASE(lpDDS_BG2);
	RELEASE_dels(Parts);
	EventCnt = 0;

	BOOL ret = TRUE;

	switch (StageNum)
	{
	//プロローグ画面の初期設定
	case 0:
		ret = LoadBitMap(PrologFileName, lpDDS_BG); // lpDDS_BGサーフェスの作成
		if (!ret)
			return FALSE;
		break;
	//タイトル画面の初期設定
	case 1:
		ret = LoadBitMap(TitleFileName[0], lpDDS_BG); // lpDDS_BGサーフェスの作成
		if (!ret)
			return FALSE;
		ret = LoadBitMap(TitleFileName[1], lpDDS_BG2);
		if (!ret)
			return FALSE;
		break;
	//ステージ1の時の初期設定
	case 2:
		ret = LoadBitMap(JIKIFileName, lpDDS_JIKI); // lpDDS_JIKIサーフェスの作成
		if (!ret)
			return FALSE;
		ret = LoadBitMap(St1FileName[0], lpDDS_SP); // lpDDS_SPサーフェスの作成
		if (!ret)
			return FALSE;
		ret = LoadBitMap(St1FileName[1], lpDDS_BG); // lpDDS_BGサーフェスの作成
		if (!ret)
			return FALSE;
		ret = LoadBitMap(St1FileName[2], lpDDS_BG2); // lpDDS_BG2サーフェスの作成
		if (!ret)
			return FALSE;
		ret = LoadMapData(St1FileName[3]); //マップデータをロード
		if (!ret)
			return FALSE;

		//現在のマップデータからゲーム開始の初期座標を計算
		MainCdn.x = 0;
		MainCdn.y = (MapSizeY * TipSizeY) - WinYMax;
		break;
	//ゲームオーバー画面用の初期設定
	case 1000:
		ret = LoadBitMap(GameOverFileName, lpDDS_BG); // lpDDS_BGサーフェスの作成
		if (!ret)
			return FALSE;
		break;
	//ゲームクリア画面用の初期設定
	case 2000:
		ret = LoadBitMap(GameClearFileName, lpDDS_BG); // lpDDS_BGサーフェスの作成
		if (!ret)
			return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
// Draw::Init(int);で確保したメモリ領域を解放する
//////////////////////////////////////////////////////////////////////
BOOL Draw::ReleaseALL()
{
	RELEASE(lpDDS_JIKI);
	RELEASE(lpDDS_SP);
	RELEASE(lpDDS_BG);
	RELEASE(lpDDS_BG2);
	RELEASE_dels(JIKIFileName);
	RELEASE_dels(PrologFileName);
	for (short i = 0; i < 2; i++)
	{
		RELEASE_dels(TitleFileName[i]);
	}
	for (i = 0; i < 4; i++)
	{
		RELEASE_dels(St1FileName[i]);
	}
	RELEASE_dels(GameOverFileName);
	RELEASE_dels(GameClearFileName);
	RELEASE_dels(Parts);
	RELEASE_dels(Tan);
	RELEASE_dels(Cos);
	RELEASE_dels(Sin);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	自機描画用サーフェスのロストをチェックする関数
//////////////////////////////////////////////////////////////////////
BOOL Draw::RestoreJIKI()
{
	HRESULT ret;

	//ステージ番号によって処理を分岐
	switch (StageNum)
	{
	case 0:
	case 1:
		break;
	case 2:
		if (!lpDDS_JIKI)
			return FALSE;

		ret = lpDDS_JIKI->IsLost();
		if (ret == DD_OK)
			break; //サーフェスがロストしていないかチェック

		ret = lpDDS_JIKI->Restore(); // lpDDS_SPサーフェスのリストア
		if (ret != DD_OK)
			return FALSE;
		ret = LoadBitMap(JIKIFileName, lpDDS_JIKI); //画像をリロード
		if (ret != DD_OK)
			return FALSE;
		break;
	case 1000:
	case 2000:
		break;
	default:
		break;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// スプライトサーフェスのロストをチェックする関数
//////////////////////////////////////////////////////////////////////

BOOL Draw::RestoreSP()
{
	HRESULT ret;

	//ステージ番号によってリストア処理を分岐
	switch (StageNum)
	{
	case 0:
	case 1:
		break;
	case 2:
		if (!lpDDS_SP)
			return FALSE;

		ret = lpDDS_SP->IsLost();
		if (ret == DD_OK)
			break; //サーフェスがロストしていないかチェック

		ret = lpDDS_SP->Restore(); // lpDDS_SPサーフェスのリストア
		if (ret != DD_OK)
			return FALSE;
		ret = LoadBitMap(St1FileName[0], lpDDS_SP); //画像をリロード
		if (ret != DD_OK)
			return FALSE;

		break;
	case 1000:
	case 2000:
		break;
	default:
		break;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Draw::lpDDS_BGサーフェスをリストアする関数
//////////////////////////////////////////////////////////////////////

BOOL Draw::RestoreBG()
{
	HRESULT ret;

	//ステージ番号によってリストア処理を分岐
	switch (StageNum)
	{
	case 0:
		if (!lpDDS_BG)
			return FALSE;

		ret = lpDDS_BG->IsLost();
		if (ret != DD_OK)
		{
			ret = lpDDS_BG->Restore(); // lpDDS_BGサーフェスのリストア
			if (ret != DD_OK)
				return FALSE;
			ret = LoadBitMap(PrologFileName, lpDDS_BG); //画像をリロード
			if (ret != DD_OK)
				return FALSE;
		}
		break;
	case 1:
		if (!lpDDS_BG)
			return FALSE;
		if (!lpDDS_BG2)
			return FALSE;

		ret = lpDDS_BG->IsLost();
		if (ret != DD_OK)
		{
			ret = lpDDS_BG->Restore(); // lpDDS_BGサーフェスのリストア
			if (ret != DD_OK)
				return FALSE;
			ret = LoadBitMap(TitleFileName[0], lpDDS_BG); //画像をリロード
			if (ret != DD_OK)
				return FALSE;
		}
		ret = lpDDS_BG2->IsLost();
		if (ret != DD_OK)
		{
			ret = lpDDS_BG2->Restore(); // lpDDS_BGサーフェスのリストア
			if (ret != DD_OK)
				return FALSE;
			ret = LoadBitMap(TitleFileName[1], lpDDS_BG2); //画像をリロード
			if (ret != DD_OK)
				return FALSE;
		}
		break;
	case 2:
		if (!lpDDS_BG)
			return FALSE;
		if (!lpDDS_BG2)
			return FALSE;

		ret = lpDDS_BG->IsLost();
		if (ret != DD_OK)
		{
			ret = lpDDS_BG->Restore(); // lpDDS_BGサーフェスのリストア
			if (ret != DD_OK)
				return FALSE;
			ret = LoadBitMap(St1FileName[1], lpDDS_BG); //画像をリロード
			if (ret != DD_OK)
				return FALSE;
		}
		ret = lpDDS_BG2->IsLost();
		if (ret != DD_OK)
		{
			ret = lpDDS_BG2->Restore(); // lpDDS_BGサーフェスのリストア
			if (ret != DD_OK)
				return FALSE;
			ret = LoadBitMap(St1FileName[2], lpDDS_BG2); //画像をリロード
			if (ret != DD_OK)
				return FALSE;
		}
		break;
	case 1000:
		if (!lpDDS_BG)
			return FALSE;
		ret = lpDDS_BG->IsLost();
		if (ret != DD_OK)
		{
			ret = lpDDS_BG->Restore(); // lpDDS_BGサーフェスのリストア
			if (ret != DD_OK)
				return FALSE;
			ret = LoadBitMap(GameOverFileName, lpDDS_BG); //画像をリロード
			if (ret != DD_OK)
				return FALSE;
		}
		break;
	case 2000:
		if (!lpDDS_BG)
			return FALSE;
		ret = lpDDS_BG->IsLost();
		if (ret != DD_OK)
		{
			ret = lpDDS_BG->Restore(); // lpDDS_BGサーフェスのリストア
			if (ret != DD_OK)
				return FALSE;
			ret = LoadBitMap(GameClearFileName, lpDDS_BG); //画像をリロード
			if (ret != DD_OK)
				return FALSE;
		}
		break;
	default:
		break;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// オフスクリーンサーフェスを解放
//////////////////////////////////////////////////////////////////////
BOOL Draw::ReleaseSuf()
{
	RELEASE(lpDDS_JIKI);
	RELEASE(lpDDS_SP);
	RELEASE(lpDDS_BG);
	RELEASE(lpDDS_BG2);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// オフスクリーンサーフェスを再構築
//////////////////////////////////////////////////////////////////////
BOOL Draw::ReloadSuf()
{
	HRESULT ret;

	switch (StageNum)
	{
	//プロローグ画面用のサーフェスのリロード
	case 0:
		ret = LoadBitMap(PrologFileName, lpDDS_BG); // lpDDS_BGサーフェスの作成
		if (!ret)
			return FALSE;
		break;
	//タイトル画面時のサーフェスのリロード
	case 1:
		ret = LoadBitMap(TitleFileName[0], lpDDS_BG); // lpDDS_BGサーフェスの作成
		if (!ret)
			return FALSE;
		ret = LoadBitMap(TitleFileName[1], lpDDS_BG2); // lpDDS_BG2サーフェスの作成
		if (!ret)
			return FALSE;
		break;
	//ステージ1
	case 2:
		ret = LoadBitMap(JIKIFileName, lpDDS_JIKI); // lpDDS_JIKIサーフェスの作成
		if (!ret)
			return FALSE;
		ret = LoadBitMap(St1FileName[0], lpDDS_SP); // lpDDS_SPサーフェスの作成
		if (!ret)
			return FALSE;
		ret = LoadBitMap(St1FileName[1], lpDDS_BG); // lpDDS_BGサーフェスの作成
		if (!ret)
			return FALSE;
		ret = LoadBitMap(St1FileName[2], lpDDS_BG2); // lpDDS_BG2サーフェスの作成
		if (!ret)
			return FALSE;
		break;
	//ゲームオーバー
	case 1000:
		ret = LoadBitMap(GameOverFileName, lpDDS_BG); // lpDDS_BGサーフェスの作成
		if (!ret)
			return FALSE;
		break;
	//ゲームクリア
	case 2000:
		ret = LoadBitMap(GameClearFileName, lpDDS_BG); // lpDDS_BGサーフェスの作成
		if (!ret)
			return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ビットマップをロードする関数
//////////////////////////////////////////////////////////////////////
BOOL Draw::LoadBitMap(char *FileName, LPDIRECTDRAWSURFACE7 &lpDDsuf)
{
	//ビットマップロード
	HBITMAP hBit;
	BITMAP bm;

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
		MessageBox(hWnd, "File Open Error(Not 8bit BitMap)", FileName, MB_OK | MB_APPLMODAL);
		return FALSE;
	}

	HRESULT ret;
	LPDIRECTDRAWSURFACE7 lpDDtemp = NULL;
	DDSURFACEDESC2 ddsd;

	//オフスクリーンサーフェスが存在しなければ作成
	if (lpDDsuf == NULL)
	{
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		ddsd.dwWidth = bm.bmWidth;
		ddsd.dwHeight = (bm.bmHeight < 0 ? bm.bmHeight * -1 : bm.bmHeight);
		if (fFullscreen)
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		else
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		ret = lpDD->CreateSurface(&ddsd, &lpDDsuf, 0);
		if (ret != DD_OK)
			return FALSE;

		//透明色の設定(パレット０番)
		DDCOLORKEY ddck;

		ZeroMemory(&ddck, sizeof(ddck));
		ddck.dwColorSpaceLowValue = 0;
		ddck.dwColorSpaceHighValue = 0;
		ret = lpDDsuf->SetColorKey(DDCKEY_SRCBLT, &ddck);
		if (ret != DD_OK)
			return FALSE;
	}

	//読み込んだビットマップをオフスクリーンサーフェスに登録

	//ビデオメモリが2M以下のビデオカードのサーフェスロックエラー対策用処理
	//一度システムメモリにオフスクリーンサーフェスを作って転送し、
	//それを転送することで画像を読み込む
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ret = lpDDsuf->GetSurfaceDesc(&ddsd); //転送先のサーフェスの記述を取得
	if (ret != DD_OK)
		return FALSE;

	//メモリの確保先をシステムメモリに強制させる
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

	//一時作業用オフスクリーンサーフェス作成
	ret = lpDD->CreateSurface(&ddsd, &lpDDtemp, 0);
	if (ret != DD_OK)
		return FALSE;

	//一時作業用オフスクリーンサーフェスの透明色の設定(パレット０番)
	DDCOLORKEY ddck_t;

	ZeroMemory(&ddck_t, sizeof(ddck_t));
	ddck_t.dwColorSpaceLowValue = 0;
	ddck_t.dwColorSpaceHighValue = 0;
	ret = lpDDtemp->SetColorKey(DDCKEY_SRCBLT, &ddck_t);
	if (ret != DD_OK)
		return FALSE;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ret = lpDDtemp->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL); //メモリをロック
	if (ret != DD_OK)
		return FALSE;

	char *lpDst, //画像転送先
		*lpSrc;	 //画像転送元

	lpSrc = (char *)bm.bmBits;

	//読み込んだ画像が逆さまに登録されていたの時の処理
	if (bm.bmHeight > 0)
	{
		lpDst = (char *)ddsd.lpSurface + ddsd.lPitch * ddsd.dwHeight;
		for (long i = 0; (UINT)i < ddsd.dwHeight; i++)
		{
			lpDst -= ddsd.lPitch;
			for (long j = 0; j < ddsd.lPitch; j++)
			{
				if (*lpSrc)
					*lpDst = *lpSrc + 10; //パレットを１０進めた状態で登録
				else
					*lpDst = *lpSrc; //０の時は透明色のためそのまま登録
				lpDst++;
				lpSrc++;
			}
			lpDst -= ddsd.lPitch;
		}
	}
	else
	{
		lpDst = (char *)ddsd.lpSurface;
		for (int i = 0; (UINT)i < ddsd.lPitch * ddsd.dwHeight; i++)
		{
			if (*lpSrc)
				*lpDst = *lpSrc + 10; //パレットを１０進めた状態で登録
			else
				*lpDst = *lpSrc; //０の時は透明色のためそのまま登録
			lpDst++;
			lpSrc++;
		}
	}

	lpDDtemp->Unlock(0); //メモリロックの解除
	DeleteObject(hBit);	 //オブジェクトの削除

	ret = lpDDsuf->Blt(NULL, lpDDtemp, NULL, DDBLT_WAIT, 0); //読み込んだ画像を転送
	if (ret != DD_OK)
		return FALSE;

	RELEASE(lpDDtemp); //一時作業用オブジェクト解放
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// マップデータを読み込む関数
//////////////////////////////////////////////////////////////////////
BOOL Draw::LoadMapData(char *FileName)
{
	//ファイルオープン
	SetLastError(0);

	HANDLE hFile;

	hFile = CreateFile(FileName,
					   GENERIC_READ, 0, NULL,
					   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//オープンに失敗
	if (GetLastError())
	{
		MessageBox(hWnd, "File Open Error", "ファイルが見つかりません", MB_OK);
		CloseHandle(hFile);
		return FALSE;
	}

	DWORD ByteCount;
	//ヘッダー部分をジャンプ
	SetFilePointer(hFile, 48, NULL, FILE_BEGIN);
	//マップデータのフォーマット部分を読み込む
	ReadFile(hFile, &MapSizeX, sizeof(MapSizeX), &ByteCount, NULL);
	ReadFile(hFile, &MapSizeY, sizeof(MapSizeY), &ByteCount, NULL);
	ReadFile(hFile, &TipSizeX, sizeof(TipSizeX), &ByteCount, NULL);
	ReadFile(hFile, &TipSizeY, sizeof(TipSizeY), &ByteCount, NULL);
	//データ読み込み領域の確保
	Parts = new BGMAP[MapSizeX * MapSizeY];
	if (!Parts)
		return FALSE;
	//データを読み込む
	ReadFile(hFile, Parts, sizeof(BGMAP) * MapSizeX * MapSizeY, &ByteCount, NULL);

	//ファイルクローズ
	CloseHandle(hFile);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// 現在のマップを構成するBGMAP構造体の数を求める
//////////////////////////////////////////////////////////////////////
BOOL Draw::GetMapSizeData(DWORD &ms, DWORD &my, DWORD &cx, DWORD &cy)
{
	ms = MapSizeX;
	my = MapSizeY;
	cx = TipSizeX;
	cy = TipSizeY;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// 引数で指定された場所のBGMAP構造体のイベントデータを求める
//////////////////////////////////////////////////////////////////////
BOOL Draw::GetEventData(int i, WORD &event)
{
	event = Parts[i].Evt;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// 引数で渡されたフラグをセットする
// 引数:ステージフラグ,set=0:AND set=1:OR でセットする
//////////////////////////////////////////////////////////////////////
void Draw::SetfSTAGE(DWORD flag, int set)
{
	switch (set)
	{
	case 0:
		fSTAGE &= flag;
		break;
	case 1:
		fSTAGE |= flag;
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
// 現在のステージフラグを返す
// 戻り値:ステージフラグ
//////////////////////////////////////////////////////////////////////
DWORD Draw::GetfSTAGE()
{
	return fSTAGE;
}

//////////////////////////////////////////////////////////////////////
// 現在のステージ番号を返す
// 戻り値:ステージ番号
//////////////////////////////////////////////////////////////////////
int Draw::GetStageNum()
{
	return StageNum;
}

//////////////////////////////////////////////////////////////////////
// 引数で渡されたステージ番号をセット
// 引数:ステージ番号
//////////////////////////////////////////////////////////////////////
void Draw::SetStageNum(int StNum)
{
	StageNum = StNum;
}

//////////////////////////////////////////////////////////////////////
// タンジェントの値を角度に直す。引数はタンジェント値。戻り値は角度。
//////////////////////////////////////////////////////////////////////
short Draw::TanKaku(double tan)
{
	short i = 0;

	if (tan <= Tan[9])
	{
		for (i = 0; i < 10; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan <= Tan[19])
	{
		for (i = 10; i < 20; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan <= Tan[29])
	{
		for (i = 20; i < 30; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan <= Tan[39])
	{
		for (i = 30; i < 40; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan <= Tan[49])
	{
		for (i = 40; i < 50; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan <= Tan[59])
	{
		for (i = 50; i < 60; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan <= Tan[69])
	{
		for (i = 60; i < 70; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan <= Tan[79])
	{
		for (i = 70; i < 80; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan <= Tan[89])
	{
		for (i = 80; i < 90; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan > Tan[89])
	{
		return (89); // tan値が89度より大きい場合は89を返す
	}
	return 0;
}