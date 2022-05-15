/*====================================================================
	敵キャラクラスの定義
=====================================================================*/

#include "TEKI01.h"
#include "TEKI02.h"
#include "BOSS01.h"

class DrawSP : protected Draw
{
	DWORD fEvt;		//この敵キャラの種類を表わすフラグ
	DWORD JK_fJIKI; //自機の状態フラグのコピー
	POINT JK_Cdn;	//自機の画面座標のコピー

	TEKI01 *Teki01; //敵キャラの種類が01だった場合に確保されるメモリ領域へのポインタ
	TEKI02 *Teki02; //敵キャラの種類が02だった場合に確保されるメモリ領域へのポインタ
	BOSS01 *Boss01; //敵キャラの種類が03だった場合に確保されるメモリ領域へのポインタ

public:
	//------------------以下のメソッドはDrawSP.cppに記述-----------------------//
	BOOL InitSP(POINT, WORD);		   //引数はMapCdnとイベントを初期化
	void MoveSP();					   //敵キャラを動かす
	void TamaSP();					   //敵キャラ敵の弾を管理する
	void FrameSP(DWORD, DWORD, POINT); //敵キャラの描画準備
	void RendSP();					   //敵キャラを描画する
	void GetTekiData(DWORD &fevt,	   //引数に各値を代入して自機オブジェクトに渡す
					 DWORD &fspchr,
					 POINT &cdn,
					 short &width,
					 short &height,
					 short *&shldcnt,
					 TAMAS *&tamas,
					 TAMAS2 *&tamas2,
					 TAMAS3 *&tamas3);

	DrawSP();
	~DrawSP();
	//-------------------------------------------------------------------------//
};
