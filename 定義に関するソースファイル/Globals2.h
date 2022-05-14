/*=============================================================================
	グローバル変数2		自作オブジェクトの定義と宣言
==============================================================================*/

//自作オブジェクト定義
#include	"FaInOut.h"
#include	"Draw.h"
#include	"DrawBG.h"
#include	"Struct.h"
#include	"DrawSP.h"
#include	"DrawJIKI.h"

//自作オブジェクト宣言
FaInOut			*lpFInOut=NULL;			//フェードインアウトオブジェクト
Draw			*lpDraw=NULL;			//基本描画管理オブジェクト
DrawSP			*lpSP=NULL;				//敵キャラ管理オブジェクト
DrawJIKI		*lpJIKI=NULL;			//自機のスプライト管理オブジェクト	
DrawBG			*lpBG=NULL;				//背景管理オブジェクト
