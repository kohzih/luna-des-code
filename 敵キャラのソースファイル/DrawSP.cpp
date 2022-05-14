/*====================================================================
	敵キャラorスプライトオブジェクトの処理に関するソースファイル
=====================================================================*/

#include	<windows.h>
#include	<ddraw.h>

#include	"Macros.h"
#include	"Draw.h"
#include	"Struct.h"
#include	"DrawSP.h"	
#include	"DrawSPData.h"
#include	"DrawMacros.h"

extern	LPDIRECTDRAWSURFACE7	lpDDSBack;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DrawSP::DrawSP()
{
	Teki01=NULL;
	Teki02=NULL;
	Boss01=NULL;
}

DrawSP::~DrawSP()
{	
	RELEASE_del(Teki01);
	RELEASE_del(Teki02);
	RELEASE_del(Boss01);
}

//////////////////////////////////////////////////////////////////////
// 敵キャラの種類によってそれぞれメモリを確保し各メンバ変数を初期化
//////////////////////////////////////////////////////////////////////
BOOL	DrawSP::InitSP(POINT cdn,WORD et)
{
	//再初期化の場合必要な処理
	RELEASE_del(Teki01);
	RELEASE_del(Teki02);
	RELEASE_del(Boss01);

	//敵キャラの種類を初期化
	fEvt=(DWORD)et;		
	
	//自機のデータを初期化
	JK_fJIKI=0;		//自機の状態フラグのコピー
	JK_Cdn.x=0;		//自機の画面座標のコピー
	JK_Cdn.y=0;

	BOOL	ret=TRUE;
	
	//敵キャラの種類によって初期化を分けます
	switch (fEvt){
		//敵キャラ01の初期化
		case 1:
			Teki01=new TEKI01;
			if(!Teki01)	return	FALSE;
			
			ret=Teki01->InitTeki01(cdn);
			if(!ret)	return	FALSE;

			break;
		//敵キャラ02の初期化
		case 2:
			Teki02=new TEKI02;
			if(!Teki02)	return	FALSE;
			
			ret=Teki02->InitTeki02(cdn);
			if(!ret)	return	FALSE;

			break;
		//ボスキャラ01の初期化
		case 3:
			Boss01=new BOSS01;
			if(!Boss01)	return	FALSE;
			
			ret=Boss01->InitBoss01(cdn);
			if(!ret)	return	FALSE;

			break;
		default:
			break;
	}
	
	return	TRUE;
}

//////////////////////////////////////////////////////////////////////
// 敵キャラ別に座標を動かす処理
//////////////////////////////////////////////////////////////////////
void	DrawSP::MoveSP()
{
	//ステージフラグに何かフラグが立っている場合リターン
	if(fSTAGE&_fSTAGE_CHK)		return;	
	//ゲームを一時停止させるフラグが立っている場合リターン
	if(JK_fJIKI&_GAME_STOP_ON)		return;
	
	switch(fEvt){
		case 1:
			Teki01->MovTeki01();
			break;
		case 2:
			Teki02->MovTeki02();
			break;
		case 3:
			Boss01->MovBoss01();
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////
// 敵の発射する弾を管理する
//////////////////////////////////////////////////////////////////////
void	DrawSP::TamaSP()
{	
	//ステージフラグに何かフラグが立っている場合リターン
	if(fSTAGE&_fSTAGE_CHK)		return;	
	//ゲームを一時停止させるフラグが立っている場合リターン
	if(JK_fJIKI&_GAME_STOP_ON)		return;
	
	switch(fEvt){
		case 1:
			Teki01->AttackTeki01();
			break;
		case 2:
			Teki02->AttackTeki02();
			break;
		case 3:
			Boss01->Attack1Boss01(JK_Cdn);
			Boss01->Attack2Boss01();
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////
//	引数で渡された変数に
//		イベント種類フラグ、
//		敵キャラ状態フラグ、
//		敵キャラの画面座標、
//		敵キャラの横幅、
//		敵キャラの縦幅、
//		敵キャラの耐久力のアドレス
//		敵キャラの弾を管理する構造体のアドレス
//		敵キャラの弾を管理する構造体のアドレス2
//		敵キャラの弾を管理する構造体のアドレス3
//	をコピー
//////////////////////////////////////////////////////////////////////
void	DrawSP::GetTekiData(DWORD	&fevt,	//引数に各値を自機オブジェクトに渡す
							DWORD	&fspchr,
							POINT	&cdn,
							short	&width,
							short	&height,
							short	*&shldcnt,
							TAMAS	*&tamas,
							TAMAS2	*&tamas2,
							TAMAS3	*&tamas3)
{
	fevt=fEvt;		//イベントフラグを渡す
	
	switch(fEvt){
		case 1:
			fspchr	=Teki01->GetTeki01fSPChr();		//敵キャラ状態フラグ
			cdn		=Teki01->GetTeki01Cdn();		//画面座標
			width	=Teki01->GetTeki01Width();		//敵キャラの横幅
			height	=Teki01->GetTeki01Height();		//敵キャラの縦幅
			shldcnt	=Teki01->GetTeki01ShldCnt_P();	//残り耐久力の値へのポインタ
			tamas	=Teki01->GetTeki01TMsr_P();		//弾の状態を管理する構造体へのポインタ
			tamas2	=NULL;
			tamas3	=NULL;
			break;
		case 2:
			fspchr	=Teki02->GetTeki02fSPChr();		//敵キャラ状態フラグ
			cdn		=Teki02->GetTeki02Cdn();		//画面座標
			width	=Teki02->GetTeki02Width();		//敵キャラの横幅
			height	=Teki02->GetTeki02Height();		//敵キャラの縦幅
			shldcnt	=Teki02->GetTeki02ShldCnt_P();	//残り耐久力の値へのポインタ
			tamas	=Teki02->GetTeki02TMsr_P();		//弾の状態を管理する構造体へのポインタ
			tamas2	=NULL;
			tamas3	=NULL;
			break;
		case 3:
			fspchr	=Boss01->GetBoss01fSPChr();		//ボスキャラ状態フラグ
			cdn		=Boss01->GetBoss01Cdn();		//画面座標
			width	=Boss01->GetBoss01Width();		//ボスキャラの横幅
			height	=Boss01->GetBoss01Height();		//ボスキャラの縦幅
			shldcnt	=Boss01->GetBoss01ShldCnt_P();	//残り耐久力の値へのポインタ
			tamas	=NULL;							//弾の状態を管理する構造体へのポインタ
			tamas2	=Boss01->GetBoss01B_Msr_P();
			tamas3	=Boss01->GetBoss01B_Block_P();
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////
// イベント別に処理を分岐させアニメーションさせて描画の準備をする関数
//	引数:敵キャラの状態フラグ、自機の状態フラグ、自機の画面座標
//////////////////////////////////////////////////////////////////////
void	DrawSP::FrameSP(DWORD flag,DWORD fJ,POINT cdn)
{
	//自機の状態フラグをコピー
	JK_fJIKI=fJ;
	//自機の座標をコピー
	JK_Cdn=cdn;
	//ステージフラグに何かフラグが立っている場合リターン
	if(fSTAGE&_fSTAGE_CHK)		return;	
	//ゲームを一時停止させるフラグが立っている場合リターン
	if(JK_fJIKI&_GAME_STOP_ON)		return;
	
	switch(fEvt){
		case 1:
			Teki01->FrameTeki01(flag);
			break;
		case 2:
			Teki02->FrameTeki02(flag);
			break;
		case 3:
			Boss01->FrameBoss01(flag);
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////
// 敵キャラの描画
//////////////////////////////////////////////////////////////////////
void	DrawSP::RendSP()
{
	//ステージフラグに何かフラグが立っている場合リターン
	if(fSTAGE&_fSTAGE_CHK)		return;	
	
	switch(fEvt){
		case 1:
			Teki01->RendTeki01();
			break;
		case 2:
			Teki02->RendTeki02();
			break;
		case 3:
			Boss01->RendBoss01();
			break;
		default:
			break;
	}
}	

