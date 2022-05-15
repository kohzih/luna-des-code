/*====================================================================
	ゲームのメイン処理に関するソースファイル
=====================================================================*/

#include <windows.h>
#include <ddraw.h>

#include "Macros.h"
#include "DrawMacros.h"
#include "FaInOut.h"
#include "Draw.h"
#include "DrawBG.h"
#include "Struct.h"
#include "DrawSP.h"
#include "DrawJIKI.h"
#include "DrawMacros.h"

extern HWND hWnd;				//メインウィンドウハンドル
extern BOOL fFullscreen;		//フルスクリーン(1)orウィンドウフラグ(0)
extern DWORD DisplayColor;		//スクリーンの現在のビットカラー値
extern DWORD TotalMem, FreeMem; // VRAMの空き容量の値を格納

extern LPDIRECTDRAWSURFACE7 lpDDSPrimary, //プライマリサーフェース
	lpDDSBack;							  //バックサーフェース

extern FaInOut *lpFInOut; //フェードインアウトオブジェクト
extern Draw *lpDraw;	  //基本描画管理オブジェクト
extern DrawSP *lpSP;	  //敵キャラ管理オブジェクト
extern DrawJIKI *lpJIKI;  //自機のスプライト管理オブジェクト
extern DrawBG *lpBG;	  //背景管理オブジェクト

//関数のプロトタイプ宣言
void FrameRateDraw(void);						 //フレームレート固定
void DrawText(char *, int, int, COLORREF);		 //テキスト表示
void DrawText(char *, long, int, int, COLORREF); //テキスト+変数表示
BOOL InitSTAGE(void);							 //ステージの初期化処理

//---------------------------------------------------------
//ゲーム内のメイン処理
//---------------------------------------------------------
BOOL GameMain(void)
{
	//プライマリサーフェスロストチェック
	if (!lpDDSPrimary == NULL && lpDDSPrimary->IsLost() == DDERR_SURFACELOST)
		lpDDSPrimary->Restore();
	//バックサーフェスクリア
	DDBLTFX ddbltfx;
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = 0; //黒
	ddbltfx.dwFillColor |= ddbltfx.dwFillColor << 8;
	ddbltfx.dwFillColor |= ddbltfx.dwFillColor << 16;
	lpDDSBack->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbltfx);

	BOOL ret = TRUE;

	//ゲーム終了フラグのチェック
	if (lpDraw->GetfSTAGE() & _GAME_EXIT)
		return FALSE;

	//各ステージの開始、終了、イベントの処理
	ret = InitSTAGE();
	if (!ret)
	{
		MessageBox(hWnd, "InitSTAGE", "ERROR", MB_OK | MB_APPLMODAL);
		return FALSE;
	}

	//以下ゲームのメイン処理
	lpBG->MoveBG();		//背景スクロール
	lpJIKI->MoveJIKI(); //自機の移動

	for (int i = 0; (DWORD)i < lpDraw->EventCnt; i++)
	{
		lpSP[i].MoveSP(); //敵キャラの移動
		lpSP[i].TamaSP(); //敵弾の処理
		//敵キャラの情報を取得
		lpJIKI->SP_Num = (DWORD)i;				//敵キャラの番号
		lpSP[i].GetTekiData(lpJIKI->SP_fEvt,	//敵キャラの種類
							lpJIKI->SP_fSPChr,	//敵キャラの状態
							lpJIKI->SP_Cdn,		//敵キャラの座標
							lpJIKI->SP_Width,	//敵キャラの横幅
							lpJIKI->SP_Height,	//敵キャラの縦幅
							lpJIKI->SP_Shld,	//敵キャラの耐久力
							lpJIKI->SP_Tamas,	//敵キャラの発射した弾情報
							lpJIKI->SP_Tamas2,	//敵キャラの発射した弾情報2
							lpJIKI->SP_Tamas3); //敵キャラの発射した弾情報3
		//自機との当たり判定を調べた後にキャラの描画準備処理
		lpSP[i].FrameSP(lpJIKI->AtariJIKI(), lpJIKI->GetfJIKI(), lpJIKI->GetJIKICdn());
	}

	lpBG->FrameBG(lpJIKI->GetfJIKI()); //背景描画準備処理
	lpJIKI->FrameJIKI();			   //自機の描画準備処理

	lpBG->RendBG(); //背景の描画
	for (i = 0; (DWORD)i < lpDraw->EventCnt; i++)
		lpSP[i].RendSP(); //敵キャラの描画
	lpJIKI->RendJIKI();	  //自機の描画

#if DEBUG >= 2
	// VRAMの空き容量を表示
	DrawText("Tmem", TotalMem, 0, 118, RGB(0, 0, 0));
	DrawText("Fmem", FreeMem, 0, 136, RGB(0, 0, 0));
#endif

#if DEBUG >= 3
	//画面の色関連
	DrawText("DisplayColor", DisplayColor, 0, 154, RGB(0, 0, 0));
#endif

	//フレームレート表示＆固定
	FrameRateDraw();

	//サーフェスのフリッピング（最終出力）
	RECT rcDst, rcSrc;

	if (fFullscreen)
		lpDDSPrimary->Flip(NULL, DDFLIP_WAIT);
	else
	{
		SetRect(&rcSrc, 0, 0, WinXMax, WinYMax);

		GetClientRect(hWnd, &rcDst);
		ClientToScreen(hWnd, (LPPOINT)&rcDst);
		ClientToScreen(hWnd, (LPPOINT)&rcDst + 1);

		ddbltfx.dwDDFX = DDBLTFX_NOTEARING;
		lpDDSPrimary->Blt(&rcDst, lpDDSBack, &rcSrc, DDBLT_WAIT, &ddbltfx);
	}

	return TRUE;
}