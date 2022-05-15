/*====================================================================
	WinMain関数に関するソースファイル(ここからプログラムの起動)
=====================================================================*/

#define INITGUID
#include <windows.h>
#include <ddraw.h>
#include <dsound.h>
#include <dmusicc.h>
#include <dmusici.h>
#include <dinput.h>

#include "Macros.h"
#include ".\\BitMap\\resource.h"
#include "Globals.h"

char *szClassName = "LUNAR DESTINY beta"; //クラス名
char *szWindowCaption = "LUNAR DESTINY";  //タイトル名

//関数のプロトタイプ宣言
BOOL MakeWindowClass(HINSTANCE, WNDPROC, char *, char *);
void ShowUpdateWindow(HWND, int);
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
int MessageLoop(void);

BOOL InitDD(void);
BOOL InitDS(void);
BOOL InitDM(void);
BOOL InitDI(void);
void ReleaseDD(void);
void ReleaseDS(void);
void ReleaseDM(void);
void ReleaseDI(void);

BOOL InitETC(void);
void ReleaseETC(void);

//------------------------------------------------
//ウィンメイン関数
//------------------------------------------------
int WINAPI WinMain(HINSTANCE hThisInst, //現在のインスタンスハンドル
				   HINSTANCE hPrevInst, //以前のインスタンスハンドル
				   LPSTR lpszArgs,		//
				   int nWinMode)		//ウィンドウの表示状態
{
	//クラス定義
	if (!MakeWindowClass(hThisInst,					  //インスタンスハンドル
						 WindowProc,				  //ウィンドウプロシージャ
						 szClassName,				  //クラス名
						 MAKEINTRESOURCE(IDI_ICON1))) //アイコン
		return FALSE;

	hInst = hThisInst; //インスタンスハンドルを保存

	//ウィンドウ枠の調整(ウィンドウ表示用)
	RECT rc;
	DWORD width;
	DWORD height;

	SetRect(&rc, 0, 0, WinXMax, WinYMax);
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;

	//ウィンドウを作成する
	hWnd = CreateWindow(
		szClassName,		   //クラス名
		szWindowCaption,	   //タイトル名
		WS_POPUP | WS_BORDER | //ウィンドウスタイル
			WS_CAPTION | WS_SYSMENU,
		0, 32, width, height, //座標と幅高さ
		NULL, NULL,			  //親ウィンドウハンドルとメニューハンドル
		hThisInst,			  // WM_CREATE時に参照するウィンドウ作成情報
		NULL);

	// ShowWinsow + UpdateWindow関数
	ShowUpdateWindow(hWnd, nWinMode);

	// DirectDraw初期化
	if (!InitDD())
	{
		MessageBox(hWnd, "DirectDraw初期化", "エラー", MB_OK | MB_APPLMODAL);
		//オブジェクト解放
		ReleaseDD();
		return FALSE;
	}
	// DirectSound初期化
	if (!InitDS())
	{

		MessageBox(hWnd, "DirectSound初期化", "エラー", MB_OK | MB_APPLMODAL);
		ReleaseDS();
		ReleaseDD();
		return FALSE;
	}
	// DirectMusic初期化
	if (!InitDM())
	{
		MessageBox(hWnd, "DirectMusic初期化", "エラー", MB_OK | MB_APPLMODAL);
		ReleaseDM();
		ReleaseDS();
		ReleaseDD();
		return FALSE;
	}
	// DirectInput初期化
	if (!InitDI())
	{

		MessageBox(hWnd, "DirectInput初期化", "エラー", MB_OK | MB_APPLMODAL);
		ReleaseDI();
		ReleaseDM();
		ReleaseDS();
		ReleaseDD();
		return FALSE;
	}
	//自作オブジェクトの初期化
	if (!InitETC())
	{
		MessageBox(hWnd, "自作オブジェクトの初期化", "エラー", MB_OK | MB_APPLMODAL);
		ReleaseETC();
		ReleaseDI();
		ReleaseDM();
		ReleaseDS();
		ReleaseDD();
		return FALSE;
	}

	//メッセージループ
	return MessageLoop();
}

//---------------------------------------------------------
//ウィンドウクラス定義関数
//---------------------------------------------------------
BOOL MakeWindowClass(HINSTANCE hThisInst, //インスタンスハンドル
					 WNDPROC WindowProc,  //ウィンドウプロシージャ
					 char *szClassName,	  //クラス名
					 char *Icon)		  //アイコン
{
	WNDCLASS wc;

	wc.hInstance = hThisInst;		//インスタンスハンドル
	wc.lpszClassName = szClassName; //クラス名
	wc.lpfnWndProc = WindowProc;	//ウィンドウプロシージャ
	wc.style = CS_BYTEALIGNCLIENT | //ウィンドウスタイル
			   CS_VREDRAW | CS_HREDRAW;
	wc.hIcon = LoadIcon(hThisInst, Icon);					//アイコンのハンドル
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				//カーソルのハンドル
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //ウィンドウの背景色
	wc.lpszMenuName = NULL;									//メニュー名
	wc.cbClsExtra = 0;										//クラス拡張情報
	wc.cbWndExtra = 0;										//ウィンドウ拡張情報
	if (!RegisterClass(&wc))
		return FALSE; //クラス定義を登録

	return TRUE;
}

//---------------------------------------------------------
// ShowWindow関数　+　UpdateWindow関数
//---------------------------------------------------------
void ShowUpdateWindow(HWND hwnd, int nWinMode)
{
	ShowWindow(hwnd, nWinMode);
	UpdateWindow(hwnd);
}