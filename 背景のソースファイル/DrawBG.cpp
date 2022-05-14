/*=============================================================================
	背景の処理に関するソースファイル
==============================================================================*/

#include	<windows.h>
#include	<ddraw.h>
//#include	<dinput.h>

#include	"Macros.h"
#include	"Draw.h"
#include	"DrawBG.h"
#include	"DrawMacros.h"

extern	LPDIRECTDRAWSURFACE7	lpDDSBack;

//extern	BYTE	JoyNew,JoyOld;
//			BYTE	Joy_Get(void);

/////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DrawBG::DrawBG()
{
	fBG=0;
	JK_fJIKI=0;	
	ScrCnt=0;
	ScrSpd=0;
	ScrWt=0;
}

DrawBG::~DrawBG()
{

}

//////////////////////////////////////////////////////////////////////
// DrawBGオブジェクトを初期化する関数
//////////////////////////////////////////////////////////////////////
BOOL	DrawBG::InitBG()
{
	//現在のステージ番号によって処理を分岐させる
	switch(StageNum){
		case 0:
		case 1:	
			ScrCnt=0;	//背景スクロールカウンタ
			ScrSpd=0;	//背景スクロールスピード
			ScrWt=0;	//スクロールウェイトカウンタ
			break;
		case 2:
			ScrCnt=0;
			ScrSpd=1;
			ScrWt=0;
			break;
		case 1000:
		case 2000:
			ScrCnt=0;
			ScrSpd=0;
			ScrWt=0;
			break;
		default:
			break;
	}
	
	return	TRUE;
}

//////////////////////////////////////////////////////////////////////
// 絶対座標を移動させる(背景スクロール)
//////////////////////////////////////////////////////////////////////
void	DrawBG::MoveBG()
{
	//ステージフラグに何かフラグが立っている場合リターン
	if(fSTAGE&_fSTAGE_CHK)		return;	
	//ゲームを一時停止させるフラグが立っている場合リターン
	if(JK_fJIKI&_GAME_STOP_ON)		return;

/*
	//ジョイスティックorキーポードによる背景の移動
	JoyOld=JoyNew;
	JoyNew=Joy_Get();

	if(JoyNew & JOY_UP)					//上を押す			
	{
		MainCdn.y -=ScrSpd;						
	}
	if(JoyNew & JOY_DOWN)				//下を押す
	{
		MainCdn.y +=ScrSpd;								
	}
*/	
	//画面を上に向けてスクロール
	MainCdn.y -=ScrSpd;
	
	//絶対座標yが画面の上端になったら画面スクロールストップ
	if(MainCdn.y<0)	MainCdn.y=0; 
	
	//絶対座標yが画面の上端になったら画面スクロールストップ
	//MainCdn.y =MainCdn.y<0 ? 0:MainCdn.y;
	//絶対座標yが画面の下端になったら画面スクロールストップ
	//MainCdn.y =MainCdn.y>(long)(MapSizeY*TipSizeY-WinYMax) ? MapSizeY*TipSizeY-WinYMax : MainCdn.y;

/*
	MainCdn.x +=x;
	if(MainCdn.x<0)		MainCdn.x=0;
	if( MainCdn.x>(long)(MapSizeX*TipSizeX-WinXMax-1) )	MainCdn.x=MapSizeX*TipSizeX-WinXMax;
*/	
}

//////////////////////////////////////////////////////////////////////
// 受け取ったフラグによって様々な処理をする
//////////////////////////////////////////////////////////////////////
void	DrawBG::FrameBG(DWORD fJ)
{
	JK_fJIKI=fJ;		//自機の状態フラグをコピー
	
	//ステージフラグに何かフラグが立っている場合リターン
	if(fSTAGE&_fSTAGE_CHK)		return;	
	//ゲームを一時停止させるフラグが立っている場合リターン
	if(JK_fJIKI&_GAME_STOP_ON)		return;

	//**********この下に処理を記入する予定**********

	//**********************************************
}

//////////////////////////////////////////////////////////////////////
// 背景描画
// (X軸のスクロールもサポートしている)
//////////////////////////////////////////////////////////////////////
void	DrawBG::RendBG()
{
	int		i;
	RECT	rc;

	switch(StageNum){
		//プロローグ画面の描画
		case 0:
			lpDDSBack->BltFast(240,64,lpDDS_BG,NULL,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			break;
		//タイトル画面の描画
		case 1:
			lpDDSBack->BltFast(0,0,lpDDS_BG,NULL,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			if(fBG&_SetBG00_ON){
				rc.left=96;		//END選択
				rc.top=0;
				rc.right=192;
				rc.bottom=64;
			}else{
				rc.left=0;		//START選択
				rc.top=0;
				rc.right=96;
				rc.bottom=64;
			}
			lpDDSBack->BltFast(272,284,lpDDS_BG2,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			break;
		//ステージ1の描画
		case 2:
			//絶対座標から現在描画すべき背景の位置を計算
			int PartsNum;
	
			PartsNum=(MainCdn.x / TipSizeX)+(MainCdn.y / TipSizeY * MapSizeX);
	
			//絶対座標から現在描画する画面端背景スプライトの欠け具合を計算
			int	PartsLackX,PartsLackY;
	
			PartsLackX=MainCdn.x % TipSizeX;
			PartsLackY=MainCdn.y % TipSizeY;
	
			//最上段左端を描画
			SetRect(&rc,
					Parts[PartsNum].X+PartsLackX,
					Parts[PartsNum].Y+PartsLackY,
					Parts[PartsNum].X+TipSizeX,
					Parts[PartsNum].Y+TipSizeY);
	
			lpDDSBack->BltFast(	0,0,
								lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);	
	
			//最上段の描画
			for(i=1;(DWORD)i<WinXMax/MapSizeX;i++)
			{
				SetRect(&rc,
						Parts[PartsNum+i].X,
						Parts[PartsNum+i].Y+PartsLackY,
						Parts[PartsNum+i].X+TipSizeX,
						Parts[PartsNum+i].Y+TipSizeY);

				lpDDSBack->BltFast(	i*TipSizeX-PartsLackX,0,
									lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			}

			//最上段右端の描画
			SetRect(&rc,
					Parts[PartsNum+(WinXMax/TipSizeX)].X,
					Parts[PartsNum+(WinXMax/TipSizeX)].Y+PartsLackY,
					Parts[PartsNum+(WinXMax/TipSizeX)].X+PartsLackX,
					Parts[PartsNum+(WinXMax/TipSizeX)].Y+TipSizeY);
		
			lpDDSBack->BltFast(	(WinXMax/TipSizeX)*TipSizeX-PartsLackX,0,
								lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);

			//描画するBGの位置を進める
			PartsNum +=MapSizeX;
	
			//中段の描画
			for(i=1;(DWORD)i<WinYMax/TipSizeY;i++)
			{
				//左端
				SetRect(&rc,
						Parts[PartsNum].X+PartsLackX,
						Parts[PartsNum].Y,
						Parts[PartsNum].X+TipSizeX,
						Parts[PartsNum].Y+TipSizeY);
		
				lpDDSBack->BltFast(	0,i*TipSizeY-PartsLackY,
									lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		
				//中段
				for(int j=1;(DWORD)j<WinXMax/TipSizeX;j++)
				{
					SetRect(&rc,
							Parts[PartsNum+j].X,
							Parts[PartsNum+j].Y,
							Parts[PartsNum+j].X+TipSizeX,
							Parts[PartsNum+j].Y+TipSizeY);
					lpDDSBack->BltFast(	j*TipSizeX-PartsLackX,i*TipSizeY-PartsLackY,
										lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
				}
		
				//右端
				SetRect(&rc,
						Parts[PartsNum+(WinXMax/TipSizeX)].X,
						Parts[PartsNum+(WinXMax/TipSizeX)].Y,
						Parts[PartsNum+(WinXMax/TipSizeX)].X+PartsLackX,
						Parts[PartsNum+(WinXMax/TipSizeX)].Y+TipSizeY);
		
				lpDDSBack->BltFast(	(WinXMax/TipSizeX)*TipSizeX-PartsLackX,i*TipSizeY-PartsLackY,
									lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
				//描画する位置を進める
				PartsNum +=MapSizeX;
			}

			//最下段左端の描画
			SetRect(&rc,
					Parts[PartsNum].X+PartsLackX,
					Parts[PartsNum].Y,
					Parts[PartsNum].X+TipSizeX,
					Parts[PartsNum].Y+PartsLackY);
	
			lpDDSBack->BltFast(	0,WinYMax-PartsLackY,
								lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);

			//最下段中段の描画
			for(i=1;i<(int)(WinXMax/TipSizeX);i++)
			{
				SetRect(&rc,
						Parts[PartsNum+i].X,
						Parts[PartsNum+i].Y,
						Parts[PartsNum+i].X+TipSizeX,
						Parts[PartsNum+i].Y+PartsLackY);
		
				lpDDSBack->BltFast(	i*TipSizeX-PartsLackX,WinYMax-PartsLackY,
							lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			}

			//最下段右端の描画
			SetRect(&rc,
					Parts[PartsNum+(WinXMax/TipSizeX)].X,
					Parts[PartsNum+(WinXMax/TipSizeX)].Y,
					Parts[PartsNum+(WinXMax/TipSizeX)].X+PartsLackX,
					Parts[PartsNum+(WinXMax/TipSizeX)].Y+PartsLackY);
	
			lpDDSBack->BltFast(	(WinXMax/TipSizeX)*TipSizeX-PartsLackX,WinYMax-PartsLackY,
								lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			
			//ステージ1オープニング文字表示
			if(fBG&_SetBG00_ON){
				lpDDSBack->BltFast(165,145,lpDDS_BG2,NULL,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			}
			break;
		//ゲームオーバー画面の描画
		case 1000:
			lpDDSBack->BltFast(	225,224,
								lpDDS_BG,NULL,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			break;
		//ゲームクリア画面の描画
		case 2000:
			lpDDSBack->BltFast(	0,0,
								lpDDS_BG,NULL,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////
// 現在のfBGフラグを返す
// 戻り値:ステージ番号
//////////////////////////////////////////////////////////////////////
DWORD	DrawBG::GetfBG()
{
	return	fBG;
}

//////////////////////////////////////////////////////////////////////
// 引数で渡されたフラグをセットする
// 引数:BGフラグ,set=0:AND set=1:OR でセットする
//////////////////////////////////////////////////////////////////////
void	DrawBG::SetfBG(DWORD flag,int set)
{
	switch(set){
		case 0:
			fBG &=flag;
			break;
		case 1:
			fBG |=flag;
			break;
		default:
			break;
	}
}