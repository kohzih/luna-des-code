/*====================================================================
	メッセージループと
	キー入力によるフルスクリーンとウィンドウモードの切り替えを行う
	関数のソースファイル
=====================================================================*/

#include <windows.h>
#include <ddraw.h>
#include <dinput.h>

#include "Macros.h"
#include "Draw.h"

extern HWND hWnd;		 //メインウィンドウハンドル
extern BOOL fActive;	 //アクティブフラグ(ウィンドウ表示用)
extern BOOL fFullscreen; //フルスクリーン(1)orウィンドウフラグ(0)

extern LPDIRECTDRAW7 lpDD; // DirectDrawオブジェクト
extern LPDIRECTINPUTDEVICE7 lpDIKeyboard;

extern Draw *lpDraw; //描画管理オブジェクト

//キーボード操作用
#define KEYDOWN(name, key) (name[key] & 0x80)

BOOL GameMain(void);

BOOL FullChgWindow(void);
BOOL InitDD(void);
BOOL InitDS(void);
BOOL InitDM(void);
BOOL InitDI(void);
void ReleaseDD(void);
void ReleaseDS(void);
void ReleaseDM(void);
void ReleaseDI(void);

//---------------------------------------------------------
//メッセージループ
//---------------------------------------------------------
int MessageLoop(void)
{
	MSG msg;

	HRESULT ret;
	char buffer[256];

	while (1)
	{ //メッセージを取得する
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//メッセージがある時

			// WM_QUITメッセージは処理の終了
			if (msg.message == WM_QUIT)
				break;
			//メッセージを処理してウィンドウプロシージャに転送する
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//メッセージが無い時
			//ウィンドウがアクティブかどうかチェック
			if (fActive)
			{
				ret = lpDIKeyboard->Poll();
				ret = lpDIKeyboard->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);
				if (ret == DIERR_INPUTLOST)
					lpDIKeyboard->Acquire();

				// ESCキーが押されていたら強制終了
				if (KEYDOWN(buffer, DIK_ESCAPE))
				{
					DestroyWindow(hWnd);
					break;
				}
#if DEBUG >= 1
				//左右CTRLキーが押されていたらウィンドウ切り替え
				if (KEYDOWN(buffer, DIK_RCONTROL) || KEYDOWN(buffer, DIK_LCONTROL))
				{
					//ウィンドウモードを切り替え
					if (!FullChgWindow())
					{
						DestroyWindow(hWnd);
						break;
					}
				}
#endif
				//ゲームのメイン処理
				ret = GameMain();
				if (!ret)
				{
#if DEBUG >= 1
					//	MessageBox(	hWnd,"ゲーム内で不正な処理が行われました","ERROR",MB_OK|MB_APPLMODAL);
#endif
					DestroyWindow(hWnd);
					break;
				}
			}
			else
			{
				//次のスレッドに処理を流す
				WaitMessage();
			}
		}
	}
	//メッセージループの終了
	return msg.wParam;
}

//---------------------------------------------------------
//ゲーム画面をウィンドウモードとフルスクリーンを切り替える関数
//---------------------------------------------------------
BOOL FullChgWindow()
{
	//今までフルスクリーンモードだった場合はウィンドウモードに設定
	if (fFullscreen)
	{
		//プライマリサーフェスのディスプレイデバイスハードウェアのモードを、
		// IDirectDraw7::SetDisplayModeメソッドが呼び出される前の状態にリセット
		lpDD->RestoreDisplayMode();

		//ウィンドウを再構成する

		RECT rcWindow;
		DWORD width;
		DWORD height;

		SetRect(&rcWindow, 0, 0, WinXMax, WinYMax);
		AdjustWindowRect(&rcWindow, WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU, FALSE);
		width = rcWindow.right - rcWindow.left;
		height = rcWindow.bottom - rcWindow.top;
		//ウィンドウの位置、サイズを設定し直す
		SetWindowPos(hWnd, 0, 100, 36, width, height, SWP_NOZORDER | SWP_DRAWFRAME);

		//現在使用中のオブジェクトを解放する
		lpDraw->ReleaseSuf();
		ReleaseDI();
		ReleaseDM();
		ReleaseDS();
		ReleaseDD();

		//スクリーンモードフラグをウィンドウモードに設定
		fFullscreen = 0;

		//オブジェクトを作り直す
		if (!InitDD()) //エラーが出た場合の処理
		{
			ReleaseDD();
			fFullscreen = 1; //モードを元に戻す
			if (!InitDD())	 //元のモードで作成し直す
			{
				return FALSE;
			}
			if (!InitDS())
			{
				return FALSE;
			}
			if (!InitDM())
			{
				return FALSE;
			}
			if (!InitDI())
			{
				return FALSE;
			}
			if (!lpDraw->ReloadSuf())
			{
				return FALSE;
			}
			return TRUE;
		}
		// DirectSound初期化
		if (!InitDS())
		{
			return FALSE;
		}
		// DirectMusic初期化
		if (!InitDM())
		{
			return FALSE;
		}
		// DirectInput初期化
		if (!InitDI())
		{
			return FALSE;
		}
		//オフスクリーンサーフェスをリロード
		if (!lpDraw->ReloadSuf())
		{
			return FALSE;
		}
		return TRUE;
	}

	//今までウィンドウモードだった場合フルスクリーンモードに設定
	if (!fFullscreen)
	{
		lpDraw->ReleaseSuf();
		ReleaseDI();
		ReleaseDM();
		ReleaseDS();
		ReleaseDD();

		//スクリーンモードフラグをフルスクリーンに設定
		fFullscreen = 1;

		//オブジェクトを作り直す
		if (!InitDD()) //エラーが出た場合の処理
		{
			//以下ウィンドウ作成
			lpDD->RestoreDisplayMode();

			//ウィンドウを再構成する
			RECT rcWindow;
			DWORD width;
			DWORD height;

			SetRect(&rcWindow, 0, 0, WinXMax, WinYMax);
			AdjustWindowRect(&rcWindow, WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU, FALSE);
			width = rcWindow.right - rcWindow.left;
			height = rcWindow.bottom - rcWindow.top;
			//ウィンドウの位置、サイズを設定し直す
			SetWindowPos(hWnd, 0, 100, 36, width, height, SWP_NOZORDER | SWP_DRAWFRAME);

			ReleaseDD();

			fFullscreen = 0; //モードを元に戻す
			if (!InitDD())	 //元のモードで作成し直す
			{
				return FALSE;
			}
			if (!InitDS())
			{
				return FALSE;
			}
			if (!InitDM())
			{
				return FALSE;
			}
			if (!InitDI())
			{
				return FALSE;
			}
			if (!lpDraw->ReloadSuf())
			{
				return FALSE;
			}
			return TRUE;
		}
		// DirectSound初期化
		if (!InitDS())
		{
			return FALSE;
		}
		// DirectMusic初期化
		if (!InitDM())
		{
			return FALSE;
		}
		// DirectInput初期化
		if (!InitDI())
		{
			return FALSE;
		}
		//オフスクリーンサーフェスをリロード
		if (!lpDraw->ReloadSuf())
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}