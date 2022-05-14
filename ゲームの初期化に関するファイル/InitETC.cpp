/*====================================================================
	自作オブジェクト初期化に関するソースファイル
=====================================================================*/

#include	<windows.h>
#include	<ddraw.h>

#include	"Macros.h"
#include	"Globals2.h"

//---------------------------------------------------------
//色々な自作オブジェクトの初期化
//---------------------------------------------------------
BOOL	InitETC()
{
	HRESULT		ret;
	
	//描画管理オブジェクトの作成
	char *FileName[]={
		//自機画像ビットマップファイル名
		".\\BitMap\\Ch001.bmp",
		//プロローグ用ビットマップファイル名
		".\\BitMap\\Prolog.bmp",	
		//タイトル用ビットマップファイル名
		".\\BitMap\\TitleMain.bmp",".\\BitMap\\TitleMoji.bmp",
		//ステージ1用各ファイル名
		".\\BitMap\\Ch002.bmp",".\\BitMap\\Bg001.bmp",
		".\\BitMap\\Bg002.bmp",".\\Data\\stage1.mdt",
		//ゲームオーバー画面用ビットマップファイル名
		".\\BitMap\\GameOver.bmp",
		//ゲームクリア画面用ビットマップファイル名
		".\\BitMap\\TheEnd.bmp"
	};
	
	lpDraw=new Draw(&FileName[0],ret);
	if(!lpDraw || !ret)		return FALSE;
	
	//自機スプライト管理オブジェクトの作成
	lpJIKI=new DrawJIKI;
	if(!lpJIKI)		return	FALSE;

	//敵キャラオブジェクトはまだ作成しない
	lpSP=NULL;
	
	//背景スクロールオブジェクトの作成
	lpBG=new DrawBG;
	if(!lpBG)		return	FALSE;

	//フェードインアウトオブジェクトの作成
	lpFInOut=new FaInOut(10,236);
	if(!lpFInOut)	return	FALSE;
	
	return	TRUE;
}

//---------------------------------------------------------
//自作オブジェクト解放
//---------------------------------------------------------
void	ReleaseETC(void)
{	
	RELEASE_del(lpFInOut);	
	RELEASE_del(lpBG);
	RELEASE_del(lpJIKI);
	RELEASE_dels(lpSP);	
	lpDraw->ReleaseALL();
	RELEASE_del(lpDraw);
}
