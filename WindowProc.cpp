/*====================================================================
	ウィンドウプロシージャのソースファイル
=====================================================================*/

#include	<windows.h>
#include	<ddraw.h>

#include	"Macros.h"
#include	"FaInOut.h"

extern	HWND		hWnd;						//メインウィンドウハンドル
extern	BOOL		fActive;					//アクティブフラグ(ウィンドウ表示用)

extern	LPDIRECTDRAWSURFACE7	lpDDSPrimary;	//プライマリサーフェース
extern	LPDIRECTDRAWPALETTE		lpDDpal;		//DirectDrawパレットオブジェクト

extern	FaInOut			*lpFInOut;		//フェードインアウトオブジェクト

void	ReleaseDD(void);
void	ReleaseDS(void);
void	ReleaseDM(void);
void	ReleaseDI(void);
void	ReleaseETC(void);

//---------------------------------------------------------
//ウィンドウプロシージャ
//---------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND	hwnd,		//ウィンドウ　ハンドル
							UINT	message,	//メッセージ
							WPARAM	wParam,		//汎用引数 W
							LPARAM	lParam		//汎用引数 L
							)
{	
	
	switch(message){
		case	WM_CREATE:
			//ウィンドウ作成
			break;
		
		case	WM_PALETTECHANGED:	//(ウィンドウ表示用)
			//パレットの変更確認メッセージ
			if(wParam == (DWORD)hWnd)	break;
			
			break;
		
		case	WM_QUERYNEWPALETTE:	//(ウィンドウ表示用)
			//パレットが新しく構築された確認メッセージ
			if(lpDDSPrimary && lpFInOut)	
			{
				lpDDSPrimary->SetPalette(lpDDpal);	//パレットセット
				lpFInOut->SetPaletteEntry();		//パレットを取得
			}
			break;
		
		case	WM_ACTIVATEAPP:	//ウィンドウ表示用
			//現在ウィンドウがアクティブか非アクティブかを受け取るメッセージ
			fActive=LOWORD(wParam);
			break;

		case	WM_DESTROY:
			//ウィンドウ破棄
			
			ReleaseETC();
			ReleaseDI();
			ReleaseDM();
			ReleaseDS();
			ReleaseDD();
			
			PostQuitMessage(0);					//プログラムの終了を告げる
			break;
		
		default:
			//その他
			return	DefWindowProc(hwnd,message,wParam,lParam);
	}
	return 0;
}
