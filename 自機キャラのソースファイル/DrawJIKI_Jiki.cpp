/*=============================================================================
	自機本体に関する初期化、当たり判定、描画等のソースファイル
==============================================================================*/

#include	<windows.h>
#include	<math.h>
#include	<ddraw.h>
#include	<dsound.h>

#include	"Macros.h"
#include	"Draw.h"	
#include	"Struct.h"
#include	"DrawJIKI.h"
#include	"DrawJIKIDataEXT.h"
#include	"DrawMacros.h"

extern	LPDIRECTDRAWSURFACE7	lpDDSBack;		//バックサーフェス

//ダイレクトサウンドバッファ再生用マクロ定義
#define	_JIKIBAKU	0
#define	_TEKIBAKU	1
#define	_FUELARM	2
#define	_FIRE		3
#define	_TAMAKYU	4
#define	_BOSSBAKU	5

extern	LPDIRECTSOUNDBUFFER		lpDSBuf[6]; //ダイレクトサウンドバッファ

//ジョイスティク関連
extern	BYTE	JoyNew,JoyOld;
BYTE	Joy_Get(void);

//////////////////////////////////////////////////////////////////////
//	自機に関する様々なデータを初期化する
//////////////////////////////////////////////////////////////////////
BOOL	DrawJIKI::InitJikiData()
{	
	
	fJIKI=0;		//自機の状態フラグを初期化
	Target=0;		//破壊目標になる敵キャラ番号の初期化
	InvincCnt=0;	//無敵時間のカウンタ初期化
	ContinueCnt=0;	//コンティニューカウンタを初期化

	//ステージ番号によって初期フラグを変化させる
	switch(StageNum){
		case 0:
			break;
		case 1:
			break;
		case 2:	
			Target=0;		//破壊するとステージクリアになる敵キャラの番号初期化
			break;
		default:
			return	FALSE;
			break;
	}

	//自機の画面座標を初期化(下端の真中からスタート)
	Cdn.x=(WinXMax/2)-(_JIKI_W/2);
	Cdn.y=WinYMax-32;
	
	//自機の絶対座標を初期化
	MapCdn.x=MainCdn.x+Cdn.x;
	MapCdn.y=MainCdn.y+Cdn.y;
	
	//自機の移動速度を初期化
	Mov.x=_JIKIMOVE;
	Mov.y=_JIKIMOVE;

	//EXP値と弾吸収に使用する燃料の初期化
	EXP=0;		
	EXP_R=EXP<<16;
	FUEL=0;
	FUEL_R=FUEL<<16;

	//敵キャラの情報を初期化
	SP_Num=0;		//敵キャラの番号
	SP_fEvt=0;		//敵キャラの種類
	SP_fSPChr=0;	//敵キャラの状態
	SP_Cdn.x=0;		//敵キャラの座標
	SP_Cdn.y=0;
	SP_Width=0;		//敵キャラの横幅
	SP_Height=0;	//敵キャラの縦幅
	SP_Shld=NULL;	//敵キャラの残り耐久力へのポインタ
	SP_Tamas=NULL;	//敵キャラの弾の状態を管理するTAMAS構造体配列のポインタ
	SP_Tamas2=NULL;//敵キャラの弾の状態を管理するTAMAS2構造体配列のポインタ
	SP_Tamas3=NULL;//敵キャラの弾の状態を管理するTAMAS3構造体配列のポインタ
	
	//////////////////////////////////////////////////////////////////////
	//-------------自機の装備に関する構造体の初期化-----------------------

	Wpd.flag=0;
	Wpd.LSR=0;	
	Wpd.MSR=0;
	Wpd.HMM=0;	
	Wpd.HNB=0;
	Wpd.OPT=0;
	Wpd.LSR_temp=0;
	Wpd.MSR_temp=0;
	Wpd.HMM_temp=0;
	Wpd.HNB_temp=0;	
	Wpd.OPT_temp=0;		

	//////////////////////////////////////////////////////////////////////
	//----------アーム発射時の自機アニメーションデータの初期化------------

	//アニメパターン数の初期化
	ArmJIKI.Anime=AmeJIKI[0];					
	//アニメパターン配列の初期化
	ArmJIKI.AnimePat=new BYTE[ArmJIKI.Anime];
	if(!ArmJIKI.AnimePat)	return	FALSE;
	memcpy(ArmJIKI.AnimePat,&PatJIKI[0][0],sizeof(BYTE)*ArmJIKI.Anime);
	//アニメ切り替えタイミング配列の初期化
	ArmJIKI.AnimeFrame=new BYTE[ArmJIKI.Anime];
	if(!ArmJIKI.AnimeFrame)	return	FALSE;
	memcpy(ArmJIKI.AnimeFrame,&FramJIKI[0][0],sizeof(BYTE)*ArmJIKI.Anime);
	//現在表示するアニメパターンの初期化
	ArmJIKI.AnimeCnt=0;
	//現在のアニメーションフレームの初期化
	ArmJIKI.FrameCnt=ArmJIKI.AnimeFrame[0];

	//////////////////////////////////////////////////////////////////////
	//---------アーム発射時のアームアニメーションデータの初期化-----------
	
	Arm.Anime=AmeJIKI[1];					
	Arm.AnimePat=new BYTE[Arm.Anime];
	if(!Arm.AnimePat)		return	FALSE;
	memcpy(Arm.AnimePat,&PatJIKI[1][0],sizeof(BYTE)*Arm.Anime);
	Arm.AnimeFrame=new BYTE[Arm.Anime];
	if(!Arm.AnimeFrame)		return	FALSE;
	memcpy(Arm.AnimeFrame,&FramJIKI[1][0],sizeof(BYTE)*Arm.Anime);
	Arm.AnimeCnt=0;
	Arm.FrameCnt=Arm.AnimeFrame[0];

	//////////////////////////////////////////////////////////////////////
	//---------自機が撃墜された時のアニメーションデータの初期化	---------
	
	Baku.Anime=AmeJIKI[2];
	Baku.AnimePat=new BYTE[Baku.Anime];
	if(!Baku.AnimePat)		return	FALSE;
	memcpy(Baku.AnimePat,&PatJIKI[2][0],sizeof(BYTE)*Baku.Anime);
	Baku.AnimeFrame=new BYTE[Baku.Anime];
	if(!Baku.AnimeFrame)	return	FALSE;
	memcpy(Baku.AnimeFrame,&FramJIKI[2][0],sizeof(BYTE)*Baku.Anime);
	Baku.AnimeCnt=0;
	Baku.FrameCnt=Baku.AnimeFrame[0];
	
	//////////////////////////////////////////////////////////////////////
	//---------弾吸収時のアニメーションデータの初期化---------------------	
	
	TmKySy.Anime=AmeJIKI[3];					
	TmKySy.AnimePat=new BYTE[TmKySy.Anime];
	if(!TmKySy.AnimePat)		return	FALSE;
	memcpy(TmKySy.AnimePat,&PatJIKI[3][0],sizeof(BYTE)*TmKySy.Anime);
	TmKySy.AnimeFrame=new BYTE[TmKySy.Anime];
	if(!TmKySy.AnimeFrame)	return	FALSE;
	memcpy(TmKySy.AnimeFrame,&FramJIKI[3][0],sizeof(BYTE)*TmKySy.Anime);
	TmKySy.AnimeCnt=0;
	TmKySy.FrameCnt=TmKySy.AnimeFrame[0];

	//////////////////////////////////////////////////////////////////////
	//--------------サーフェスから転送する矩形範囲の初期化----------------
	
	//初期の自機の矩形範囲
	JIKIChr=SpJIKI[0];
	
	//FUELゲージの現在の矩形を初期化
	GAGE_F.left   =SpJIKI[22].left+(128-FUEL);
	GAGE_F.top    =SpJIKI[22].top;
	GAGE_F.right  =SpJIKI[22].right;
	GAGE_F.bottom =SpJIKI[22].bottom;
	//FUELゲージの空白部分の矩形を初期化
	GAGE_FK=SpJIKI[21];	
	//FUELゲージの枠部分の矩形を初期化
	GAGE_FW=SpJIKI[20];
	
	//EXPゲージの現在の矩形を初期化
	GAGE_E.left   =SpJIKI[19].left;
	GAGE_E.top    =SpJIKI[19].top;
	GAGE_E.right  =SpJIKI[19].right-(128-EXP);
	GAGE_E.bottom =SpJIKI[19].bottom;
	//EXPゲージの空白部分の矩形を初期化
	GAGE_EK=SpJIKI[18];	
	//EXPゲージの枠部分の矩形を初期化
	GAGE_EW=SpJIKI[17];	
	
	//スコアと表示矩形範囲を初期化
	SCORE=0;		//スコアの初期化
	ScoreCalcul();	//スコアの表示矩形範囲を初期化
	
	return	TRUE;
}

//////////////////////////////////////////////////////////////////////
//	動的に確保するメモリ領域へのポインタをNULLで初期化
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::InitJikiTm()
{
	ArmJIKI.AnimePat	=NULL;
	ArmJIKI.AnimeFrame	=NULL;
	Arm.AnimePat		=NULL;
	Arm.AnimeFrame		=NULL;
	Baku.AnimePat		=NULL;
	Baku.AnimeFrame		=NULL;
	TmKySy.AnimePat		=NULL;
	TmKySy.AnimeFrame	=NULL;
}

//////////////////////////////////////////////////////////////////////
//	確保したメモリ領域を解放
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::JikiTmRelease()
{
	RELEASE_dels(TmKySy.AnimeFrame);
	RELEASE_dels(TmKySy.AnimePat);
	RELEASE_dels(Baku.AnimeFrame);
	RELEASE_dels(Baku.AnimePat);
	RELEASE_dels(Arm.AnimeFrame);
	RELEASE_dels(Arm.AnimePat);
	RELEASE_dels(ArmJIKI.AnimeFrame);
	RELEASE_dels(ArmJIKI.AnimePat);
}

//////////////////////////////////////////////////////////////////////
//	引数で渡された値を元に当たり判定を行う、当たった場合FALSEを返す
//////////////////////////////////////////////////////////////////////
BOOL	DrawJIKI::Atari(RECT JIKI)
{
	if(JIKI.top>SP_Cdn.y+(SP_Height-4));
	else if(JIKI.left>SP_Cdn.x+(SP_Width-4));
	else if(JIKI.right<SP_Cdn.x+4);
	else if(JIKI.bottom>SP_Cdn.y-4)	return FALSE;
	
	return	TRUE;
}

//////////////////////////////////////////////////////////////////////
//	引数で渡された値を元に当たり判定を行う、当たった場合FALSEを返す
//////////////////////////////////////////////////////////////////////
BOOL	DrawJIKI::Atari(RECT JIKI,RECT TEKI)
{
	if(JIKI.top>TEKI.bottom);
	else if(JIKI.left>TEKI.right);
	else if(JIKI.right<TEKI.left);
	else if(JIKI.bottom>TEKI.top)	return FALSE;

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////
//	自機と敵の弾との当たり判定を行う
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::JikiTekitamaAtari()
{
	RECT	Jiki,JikiA,Teki;	//当たり判定に使用する
	
	//自機の現在の画面座標を初期化(+**は当たり判定の遊び)
	Jiki.top=Cdn.y+16;
	Jiki.left=Cdn.x+6;
	Jiki.right=Cdn.x+_JIKI_W-6;
	Jiki.bottom=Cdn.y+24;
	
	//自機のアーム部分の画面座標を初期化
	JikiA.top=Cdn.y-(Arm.SrcR.bottom-Arm.SrcR.top)+16;
	JikiA.left=Cdn.x-4;
	JikiA.right=Cdn.x+24;
	JikiA.bottom=Cdn.y;
	
	short i=0;
	
	//SP_Tamasとの当たり判定
	if(SP_Tamas && SP_fSPChr&_TAMA1ON){
		for(i=0;(DWORD)i<SP_Tamas->TTama;i++){	
			//発射フラグの立っていない弾との当たり判定は行わない
			if(!(SP_Tamas->tm[i].fTama&_TON))	continue;
			//自機フラグに敵キャラ消滅フラグが立っている場合全ての弾を消滅させる
			else if(fJIKI&_PWUPD_ON){
				//------------------敵弾消滅-------------------------------------//
				
				SP_Tamas->tm[i].fTama=_TOFF;	//弾を消滅
		
				//---------------------------------------------------------------//
				continue;						//次の弾を処理
			}
					
			//お互いの画面座標で当たり判定を行う
			
			//敵キャラの弾座標の初期化
			Teki.top=SP_Tamas->tm[i].Cdn.y+2;
			Teki.left=SP_Tamas->tm[i].Cdn.x+2;
			Teki.right=(SP_Tamas->tm[i].Cdn.x+SP_Tamas->tm[i].Width)-2;
			Teki.bottom=(SP_Tamas->tm[i].Cdn.y+SP_Tamas->tm[i].Height)-2;
			
			//最初にアームとの当たり判定を行う
			//アーム発射中であれば当たり処理を行う
			if(!(fJIKI&_ARM_ON));
			else if(!Atari(JikiA,Teki)){
				//アームに接触した弾を消滅させる
				//------------------敵弾消滅-------------------------------------//
			
				SP_Tamas->tm[i].fTama=_TOFF;	//弾を消滅
			
				//---------------------------------------------------------------//

				//アームの燃料吸収用部分に接触して消滅していた場合
				if((Cdn.y-(Arm.SrcR.bottom-Arm.SrcR.top)+16)+32>SP_Tamas->tm[i].Cdn.y+8)
				{
					FUEL_R=FUEL_R+_FUELUP;				//FUEL値を増やす
					if(FUEL_R>128<<16)	FUEL_R=128<<16;	//128以上にならないよう設定
					FUEL=(BYTE)(FUEL_R>>16);
					SCORE	+=10;		//スコアアップ
					//弾吸収サウンド
					lpDSBuf[_FUELARM]->Stop();
					lpDSBuf[_FUELARM]->SetCurrentPosition(0);
					lpDSBuf[_FUELARM]->Play(0,0,0);
				}
			}
			
			//ここから自機との当たり判定
			//当たっている場合、自機と弾を消滅させてリターン
			if(!Atari(Jiki,Teki)){
				//弾吸収時以外に接触した場合ダメージを受ける
				if(!(fJIKI&_FUEL_ON)){
					if(!(fJIKI&_INVINCIBLE_ON)){
						//------------------敵弾消滅-------------------------------------//
						
						SP_Tamas->tm[i].fTama=_TOFF;	//弾を消滅
				
						//---------------------------------------------------------------//
					
						EXP_R -=_EXPSH;				//EXP値からダメージ分を引く
						if(EXP_R<0)	EXP_R=0;		//EXP_Rが0以下にならないよう修正	
						EXP=(BYTE)(EXP_R>>16);
						//弾に当たった時EXPが0の場合は撃墜
						if(!EXP){					
							fJIKI |=_END_ON; //------------------------------------------------------
							//自機撃墜サウンド	
							lpDSBuf[_JIKIBAKU]->Stop();
							lpDSBuf[_JIKIBAKU]->SetCurrentPosition(0);
							lpDSBuf[_JIKIBAKU]->Play(0,0,0);
							return;
						}
					}
					continue;
				}
				//------------------敵弾消滅-------------------------------------//
				
				SP_Tamas->tm[i].fTama=_TOFF;	//弾を消滅
				
				//---------------------------------------------------------------//
				
				//弾を吸収した場合EXP値を上昇させる
				EXP_R +=_EXPUP;						//EXP値上昇
				if(EXP_R>128<<16)	EXP_R=128<<16;	//128以上にならないよう設定
				EXP=(BYTE)(EXP_R>>16);
				
				//弾吸収サウンド
				lpDSBuf[_FUELARM]->Stop();
				lpDSBuf[_FUELARM]->SetCurrentPosition(0);
				lpDSBuf[_FUELARM]->Play(0,0,0);
				
				//EXPが128貯まった場合、自機をパワーアップ
				if(EXP==128){
					fJIKI |=_GAME_STOP_ON;	//ゲームを一時停止させるフラグオン
					fJIKI |=_PWUP_ON;		//パワーアップ中フラグオン
					InitPWUP();				//パワーアップ画面の初期化
					return;
				}
				continue;
			}
			
			//弾吸収中である場合、弾の移動方向を自機に向ける処理
			if(fJIKI&_FUEL_ON){
				double	a,b,c;	//作業用
				b=(MapCdn.x+6)-SP_Tamas->tm[i].MapCdn.x;
				c=(MapCdn.y+10)-SP_Tamas->tm[i].MapCdn.y;
				a=(b*b)+(c*c);	//余弦定理
				a=sqrt(a);		//aの平方根を求める
				SP_Tamas->tm[i].Mov.x=b*_FUELMV/a;
				SP_Tamas->tm[i].Mov.y=c*_FUELMV/a;
			}		
			//弾吸収中断フラグが立っている場合吸収中の弾の移動速度を変化させる
			else if(fJIKI&_FUELD_ON){
				SP_Tamas->tm[i].Mov.x=0.0;
				SP_Tamas->tm[i].Mov.y=2.0;
			}
		}
	}

	//SP_Tamas2との当たり判定
	if(SP_Tamas2 && SP_fSPChr&_TAMA2ON){
		for(i=0;(DWORD)i<SP_Tamas2->TTama;i++){	
			//発射フラグの立っていない弾との当たり判定は行わない
			if(!(SP_Tamas2->tm2[i].fTama&_TON))	continue;
			//自機フラグに敵キャラ消滅フラグが立っている場合全ての弾を消滅させる
			else if(fJIKI&_PWUPD_ON){
				//------------------敵弾消滅-------------------------------------//
				
				SP_Tamas2->tm2[i].fTama=_TOFF;	//弾を消滅
			
				//---------------------------------------------------------------//
				continue;						//次の弾を処理
			}
					
			//お互いの画面座標で当たり判定を行う
			
			//敵キャラの弾座標の初期化
			Teki.top=SP_Tamas2->tm2[i].Cdn.y+2;
			Teki.left=SP_Tamas2->tm2[i].Cdn.x+2;
			Teki.right=(SP_Tamas2->tm2[i].Cdn.x+SP_Tamas2->tm2[i].Width)-2;
			Teki.bottom=(SP_Tamas2->tm2[i].Cdn.y+SP_Tamas2->tm2[i].Height)-2;
			
			//最初にアームとの当たり判定を行う
			//アーム発射中であれば当たり処理を行う
			if(!(fJIKI&_ARM_ON));
			else if(!Atari(JikiA,Teki)){
				//アームに接触した弾を消滅させる
				//------------------敵弾消滅-------------------------------------//
				
				SP_Tamas2->tm2[i].fTama=_TOFF;	//弾を消滅
				
				//---------------------------------------------------------------//

				//アームの燃料吸収用部分に接触して消滅していた場合
				if((Cdn.y-(Arm.SrcR.bottom-Arm.SrcR.top)+16)+32>SP_Tamas2->tm2[i].Cdn.y+8)
				{
					FUEL_R=FUEL_R+_FUELUP;				//FUEL値を増やす
					if(FUEL_R>128<<16)	FUEL_R=128<<16;	//128以上にならないよう設定
					FUEL=(BYTE)(FUEL_R>>16);
					SCORE	+=10;		//スコアアップ
					//弾吸収サウンド
					lpDSBuf[_FUELARM]->Stop();
					lpDSBuf[_FUELARM]->SetCurrentPosition(0);
					lpDSBuf[_FUELARM]->Play(0,0,0);
				}
			}
			
			//ここから自機との当たり判定
			//当たっている場合、自機と弾を消滅させてリターン
			if(!Atari(Jiki,Teki)){
				//弾吸収時以外に接触した場合ダメージを受ける
				if(!(fJIKI&_FUEL_ON)){
					if(!(fJIKI&_INVINCIBLE_ON)){
						//------------------敵弾消滅-------------------------------------//
						
						SP_Tamas2->tm2[i].fTama=_TOFF;	//弾を消滅
					
						//---------------------------------------------------------------//
					
						EXP_R -=_EXPSH;				//EXP値からダメージ分を引く
						if(EXP_R<0)	EXP_R=0;		//EXP_Rが0以下にならないよう修正	
						EXP=(BYTE)(EXP_R>>16);
						//弾に当たった時EXPが0の場合は撃墜
						if(!EXP){					
							fJIKI |=_END_ON; //------------------------------------------------------
							//自機撃墜サウンド	
							lpDSBuf[_JIKIBAKU]->Stop();
							lpDSBuf[_JIKIBAKU]->SetCurrentPosition(0);
							lpDSBuf[_JIKIBAKU]->Play(0,0,0);
							return;
						}
					}
					continue;
				}
				//------------------敵弾消滅-------------------------------------//
				
				SP_Tamas2->tm2[i].fTama=_TOFF;	//弾を消滅
	
				//---------------------------------------------------------------//
				//弾を吸収した場合EXP値を上昇させる
				EXP_R +=_EXPUP;						//EXP値上昇
				if(EXP_R>128<<16)	EXP_R=128<<16;	//128以上にならないよう設定
				EXP=(BYTE)(EXP_R>>16);
				
				//弾吸収サウンド
				lpDSBuf[_FUELARM]->Stop();
				lpDSBuf[_FUELARM]->SetCurrentPosition(0);
				lpDSBuf[_FUELARM]->Play(0,0,0);
				
				//EXPが128貯まった場合、自機をパワーアップ
				if(EXP==128){
					fJIKI |=_GAME_STOP_ON;	//ゲームを一時停止させるフラグオン
					fJIKI |=_PWUP_ON;		//パワーアップ中フラグオン
					InitPWUP();				//パワーアップ画面の初期化
					return;
				}
				continue;
			}
			
			//弾吸収中である場合、弾の移動方向を自機に向ける処理
			if(fJIKI&_FUEL_ON){
				double	a,b,c;	//作業用
				b=(MapCdn.x+6)-SP_Tamas2->tm2[i].MapCdn.x;
				c=(MapCdn.y+10)-SP_Tamas2->tm2[i].MapCdn.y;
				a=(b*b)+(c*c);	//余弦定理
				a=sqrt(a);		//aの平方根を求める
				SP_Tamas2->tm2[i].Mov.x=b*_FUELMV/a;
				SP_Tamas2->tm2[i].Mov.y=c*_FUELMV/a;
			}		
			//弾吸収中断フラグが立っている場合吸収中の弾の移動速度を変化させる
			else if(fJIKI&_FUELD_ON){
				SP_Tamas2->tm2[i].Mov.x=0.0;
				SP_Tamas2->tm2[i].Mov.y=2.0;
			}
		}
	}

	//SP_Tamas3との当たり判定
	if(SP_Tamas3 && SP_fSPChr&_TAMA3ON){
		for(i=0;(DWORD)i<SP_Tamas3->TTama;i++){	
			//発射フラグの立っていない弾との当たり判定は行わない
			if(!(SP_Tamas3->tm3[i].fTama&_TON))	continue;
			//自機フラグに敵キャラ消滅フラグが立っている場合全ての弾を消滅させる
			else if(fJIKI&_PWUPD_ON){
				//------------------敵弾にダメージ-------------------------------//
				SP_Tamas3->tm3[i].ShldCnt -=_PWUPDDMG;
				if(SP_Tamas3->tm3[i].ShldCnt<=0){
					SP_Tamas3->tm3[i].fTama &=_TOFF;
					SP_Tamas3->tm3[i].fTama |=_TBAKU_ON;

					SCORE +=50;	//スコアアップ
					//敵キャラ破壊サウンド
					lpDSBuf[_TEKIBAKU]->Stop();		
					lpDSBuf[_TEKIBAKU]->SetCurrentPosition(0);
					lpDSBuf[_TEKIBAKU]->Play(0,0,0);
				}else{
					SP_Tamas3->tm3[i].fTama |=_THIT_ON;
				}
				//---------------------------------------------------------------//
				continue;						//次の弾を処理
			}
					
			//お互いの画面座標で当たり判定を行う
			
			//敵キャラの弾座標の初期化
			Teki.top=SP_Tamas3->tm3[i].Cdn.y+2;
			Teki.left=SP_Tamas3->tm3[i].Cdn.x+2;
			Teki.right=(SP_Tamas3->tm3[i].Cdn.x+SP_Tamas3->tm3[i].Width)-2;
			Teki.bottom=(SP_Tamas3->tm3[i].Cdn.y+SP_Tamas3->tm3[i].Height)-2;
			
			//最初にアームとの当たり判定を行う
				
			//アーム発射中であれば当たり処理を行う
			if(!(fJIKI&_ARM_ON));
			else if(!Atari(JikiA,Teki)){
				//アームに接触した弾はダメージ
				//------------------敵弾消滅-------------------------------------//
				SP_Tamas3->tm3[i].ShldCnt -=_ARMDMG;
				if(SP_Tamas3->tm3[i].ShldCnt<=0){
					SP_Tamas3->tm3[i].fTama &=_TOFF;
					SP_Tamas3->tm3[i].fTama |=_TBAKU_ON;

					SCORE +=50;	//スコアアップ
					//敵弾破壊サウンド
					lpDSBuf[_TEKIBAKU]->Stop();		
					lpDSBuf[_TEKIBAKU]->SetCurrentPosition(0);
					lpDSBuf[_TEKIBAKU]->Play(0,0,0);
				}else{
					SP_Tamas3->tm3[i].fTama |=_THIT_ON;
				}
				//---------------------------------------------------------------//
			}
			
			//ここから自機との当たり判定
			//当たっている場合、自機と弾を消滅させてリターン
			if(!Atari(Jiki,Teki)){
				if(!(fJIKI&_INVINCIBLE_ON)){
					fJIKI |=_END_ON; //------------------------------------------------------
				//	//自機撃墜サウンド	
					lpDSBuf[_JIKIBAKU]->Stop();
					lpDSBuf[_JIKIBAKU]->SetCurrentPosition(0);
					lpDSBuf[_JIKIBAKU]->Play(0,0,0);
					return;
				}
			}
			continue;
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	自機と敵キャラとの当たり判定
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::JikiTekiAtari()
{
	RECT	Jiki,JikiA;
	
	//自機の現在の画面座標を初期化(+**は当たり判定の遊び)
	Jiki.top=Cdn.y+16;
	Jiki.left=Cdn.x+6;
	Jiki.right=Cdn.x+_JIKI_W-6;
	Jiki.bottom=Cdn.y+24;
	
	//自機のアーム部分の画面座標を初期化
	JikiA.top=Cdn.y-(Arm.SrcR.bottom-Arm.SrcR.top)+16;
	JikiA.left=Cdn.x-4;
	JikiA.right=Cdn.x+24;
	JikiA.bottom=Cdn.y;
	
	//敵キャラとアームとの当たり判定
	if(!(fJIKI&_ARM_ON));
	else if(!Atari(JikiA)){
		JikiTekiDmg(_ARMDMG);
		if(SP_fSPChr&_SPDES){
			//もしアームの燃料吸収用部分に接触して消滅していた場合
			if((Cdn.y-(Arm.SrcR.bottom-Arm.SrcR.top)+16)+32>SP_Cdn.y+32){
				FUEL_R=FUEL_R+(_FUELUP*2);	//燃料を増やす
				if(FUEL_R>128<<16)	FUEL_R=128<<16;	//128以上にならないようにする
				FUEL=(BYTE)(FUEL_R>>16);
			}
			return;
		}
	}	

	//敵キャラと自機との当たり判定
	if(!Atari(Jiki) && !(fJIKI&_INVINCIBLE_ON)){		
		fJIKI |=_END_ON; //----------------------------------------------------------------------
		//自機撃墜サウンド
		lpDSBuf[_JIKIBAKU]->Stop();
		lpDSBuf[_JIKIBAKU]->SetCurrentPosition(0);
		lpDSBuf[_JIKIBAKU]->Play(0,0,0);
	}
}

//////////////////////////////////////////////////////////////////////
//	引数で渡されたダメージ量を敵キャラに与える
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::JikiTekiDmg(int DMG)
{
	*SP_Shld =*SP_Shld-DMG;		//ダメージ分を引く
	//耐久力が0を切った場合
	if(*SP_Shld<=0){	
		SP_fSPChr |=_SPDES;		//破壊フラグオン
		
		//敵キャラ破壊サウンド
		lpDSBuf[_TEKIBAKU]->Stop();		
		lpDSBuf[_TEKIBAKU]->SetCurrentPosition(0);
		lpDSBuf[_TEKIBAKU]->Play(0,0,0);
		
		switch(SP_fEvt){
			case 1:
				SCORE +=100;	//スコアアップ
				break;
			case 2:
				SCORE +=200;
				break;
			case 3:
				SCORE +=10000;
				break;
			default:
				break;
		}
	}else{
		SP_fSPChr |=_HITON;		//ダメージフラグオン
	}
}

//////////////////////////////////////////////////////////////////////
//	引数で渡されたダメージ量を敵弾に与える
//	引数:ダメージ量、対象となる敵弾
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::JikiTamas3Dmg(int DMG,short i)
{
	SP_Tamas3->tm3[i].ShldCnt -=DMG;
	if(SP_Tamas3->tm3[i].ShldCnt<=0){
		SP_Tamas3->tm3[i].fTama &=_TOFF;
		SP_Tamas3->tm3[i].fTama |=_TBAKU_ON;
		SCORE +=50;	//スコアアップ
		//敵キャラ破壊サウンド
		lpDSBuf[_TEKIBAKU]->Stop();		
		lpDSBuf[_TEKIBAKU]->SetCurrentPosition(0);
		lpDSBuf[_TEKIBAKU]->Play(0,0,0);
	}else{
		SP_Tamas3->tm3[i].fTama |=_THIT_ON;
	}
}

//////////////////////////////////////////////////////////////////////
//	自機のパワーアップ画面を初期化する
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::InitPWUP()
{
	//画面に表示するカーソルの初期化
	CurCdn.x=84;
	CurCdn.y=100;
	
	//PWUゲージの部分の初期化
	GAGE_PW=SpJIKI[23];
	GAGE_P =SpJIKI[24];
	GAGE_P2=SpJIKI[26];

	//OPT枠用の初期化
	OPTWAKU=SpJIKI[27];
	
	//装備レベルの現在の値を計算用に取得
	Wpd.LSR_temp=Wpd.LSR;
	Wpd.MSR_temp=Wpd.MSR;
	Wpd.HMM_temp=Wpd.HMM;
	Wpd.HNB_temp=Wpd.HNB;
	Wpd.OPT_temp=Wpd.OPT;
}

//////////////////////////////////////////////////////////////////////
//	自機パワーアップ画面を表示しパワーアップに伴う色々な処理する
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::RendPWUP()
{
	//パワーアップに伴う敵キャラ消滅フラグの立っている場合はフラグをクリアしてリターン
	if(fJIKI&_PWUPD_ON){
		fJIKI &=_PWUPD_OFF;
		return;
	}
	
	//ジョイスティックorキーポードからの入力を取得
	JoyOld=JoyNew;
	JoyNew=Joy_Get();

	short i=0;

	//////////////////////////////////////////////////////////////////////
	//---------------------- 上が押された瞬間 ----------------------------	
	if((JoyNew^JoyOld)&(JoyNew&JOY_UP)){
		switch(CurCdn.x){
			case 84:
				if(CurCdn.y==100)		CurCdn.y=450;	//MAIN(カーソルの位置)
				else if(CurCdn.y==188)	CurCdn.y=100;	//SUB
				else if(CurCdn.y==276)	CurCdn.y=188;	//OPT
				else if(CurCdn.y==450)	CurCdn.y=276;	//EXIT
				break;
			case 184:
				if(CurCdn.y==100)		CurCdn.y=124;	//LSR
				else if(CurCdn.y==124)	CurCdn.y=100;	//MSR
				else if(CurCdn.y==188)	CurCdn.y=212;	//HMM
				else if(CurCdn.y==212)	CurCdn.y=188;	//HNB
				else if(CurCdn.y==308)	CurCdn.y=380;	//OPT1
				else if(CurCdn.y==332)	CurCdn.y=308;	//OPT2
				else if(CurCdn.y==356)	CurCdn.y=332;	//OPT3
				else if(CurCdn.y==380)	CurCdn.y=356;	//OPT4
				break;
			default:
				break;
		}
	//////////////////////////////////////////////////////////////////////	
	//--------------------- 下が押された瞬間 -----------------------------
	}else if((JoyNew^JoyOld)&(JoyNew&JOY_DOWN)){
		switch(CurCdn.x){
			case 84:
				if(CurCdn.y==100)		CurCdn.y=188;	//MAIN
				else if(CurCdn.y==188)	CurCdn.y=276;	//SUB
				else if(CurCdn.y==276)	CurCdn.y=450;	//OPT
				else if(CurCdn.y==450)	CurCdn.y=100;	//EXIT
				break;
			case 184:
				if(CurCdn.y==100)		CurCdn.y=124;	//LSR	
				else if(CurCdn.y==124)	CurCdn.y=100;	//MSR
				else if(CurCdn.y==188)	CurCdn.y=212;	//HMM
				else if(CurCdn.y==212)	CurCdn.y=188;	//HNB
				else if(CurCdn.y==308)	CurCdn.y=332;	//OPT1
				else if(CurCdn.y==332)	CurCdn.y=356;	//OPT2
				else if(CurCdn.y==356)	CurCdn.y=380;	//OPT3
				else if(CurCdn.y==380)	CurCdn.y=308;	//OPT4
				break;
			default:
				break;
		}					
	}
	//////////////////////////////////////////////////////////////////////	
	//--------------------- 右が押された瞬間 -----------------------------
	if((JoyNew^JoyOld)&(JoyNew&JOY_RIGHT)){	
		if(CurCdn.x==184 && CurCdn.y==308)			i=0;	//OPT1の位置
		else if(CurCdn.x==184 && CurCdn.y==332)		i=1;	//OPT2の位置	
		else if(CurCdn.x==184 && CurCdn.y==356)		i=2;	//OPT3の位置		
		else if(CurCdn.x==184 && CurCdn.y==380)		i=3;	//OPT4の位置	
		else i=4;

		if(i>=0 && i<=3){
			switch(OPT.Jewel[i].fOPT&_fOPT_CHK){	//固定、回転、追尾のどのフラクが立っているかチェック
				case _OPTKO_ON:
					OPT.Jewel[i].fOPT &=_OPTKO_OFF;
					OPT.Jewel[i].fOPT |=_OPTKA_ON;
					break;
				case _OPTKA_ON:
					OPT.Jewel[i].fOPT &=_OPTKA_OFF;
					OPT.Jewel[i].fOPT |=_OPTTU_ON;
					break;
				case _OPTTU_ON:
					OPT.Jewel[i].fOPT &=_OPTTU_OFF;
					OPT.Jewel[i].fOPT |=_OPTKO_ON;
					break;
				default:
					break;
			}
		}
	//////////////////////////////////////////////////////////////////////	
	//---------------------- 左が押された瞬間 ----------------------------
	}else if((JoyNew^JoyOld)&(JoyNew&JOY_LEFT)){	
		if(CurCdn.x==184 && CurCdn.y==308)			i=0;	//OPT1の位置
		else if(CurCdn.x==184 && CurCdn.y==332)		i=1;	//OPT2の位置	
		else if(CurCdn.x==184 && CurCdn.y==356)		i=2;	//OPT3の位置		
		else if(CurCdn.x==184 && CurCdn.y==380)		i=3;	//OPT4の位置
		else i=4;
		
		if(i>=0 && i<=3){
			switch(OPT.Jewel[i].fOPT&_fOPT_CHK){//固定、回転、追尾のどのフラクが立っているかチェック
				case _OPTKO_ON:
					OPT.Jewel[i].fOPT &=_OPTKO_OFF;
					OPT.Jewel[i].fOPT |=_OPTTU_ON;
					break;
				case _OPTKA_ON:
					OPT.Jewel[i].fOPT &=_OPTKA_OFF;
					OPT.Jewel[i].fOPT |=_OPTKO_ON;
					break;
				case _OPTTU_ON:
					OPT.Jewel[i].fOPT &=_OPTTU_OFF;
					OPT.Jewel[i].fOPT |=_OPTKA_ON;
					break;
				default:
					break;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////
	//---------------------- 右が押されている ----------------------------
	if(JoyNew&JOY_RIGHT){	
		if(EXP){
			if(CurCdn.x==184){
				switch(CurCdn.y){
					case 100:	//LSRゲージアップ
						if(Wpd.LSR_temp<320){
							EXP--;
							Wpd.LSR_temp++;
							if(Wpd.LSR_temp>=64)	Wpd.flag |=_LSR1_ON;//レーザー1が使用可能になる
						}
						break;
					case 124:	//MSRゲージアップ
						if(Wpd.MSR_temp<320){
							EXP--;
							Wpd.MSR_temp++;
							if(Wpd.MSR_temp>=64)	Wpd.flag |=_MSR1_ON;//レーザー1が使用可能になる
						}
						break;
					case 188:	//HMMゲージアップ
						if(Wpd.HMM_temp<320){
							EXP--;
							Wpd.HMM_temp++;
							if(Wpd.HMM_temp>=64)	Wpd.flag |=_HMM1_ON;//レーザー1が使用可能になる
						}
						break;
					case 212:	//HNBゲージアップ
						if(Wpd.HNB_temp<320){
							EXP--;
							Wpd.HNB_temp++;
							if(Wpd.HNB_temp>=64)	Wpd.flag |=_HNB1_ON;//レーザー1が使用可能になる
						}
						break;
					default:
						break;
				}
			}else if(CurCdn.x==84 && CurCdn.y==276){	//OPTゲージアップ
				if(Wpd.OPT_temp<256){
					EXP--;
					Wpd.OPT_temp++;
				}
			}
		}
	//////////////////////////////////////////////////////////////////////	
	//------------------------ 左が押されている	--------------------------
	}else if(JoyNew&JOY_LEFT){		
		if(CurCdn.x==184){
			switch(CurCdn.y){
				case 100:	//LSRゲージダウン
					if(Wpd.LSR_temp>Wpd.LSR){
						EXP++;
						Wpd.LSR_temp--;
						if(Wpd.LSR_temp<64)	Wpd.flag &=_LSR1_OFF;//レーザー1が使用不能になる
					}
					break;
				case 124:	//MSRゲージダウン
					if(Wpd.MSR_temp>Wpd.MSR){
						EXP++;
						Wpd.MSR_temp--;
						if(Wpd.MSR_temp<64)	Wpd.flag &=_MSR1_OFF;//ミサイル1が使用不能になる
					}
					break;
				case 188:	//HMMゲージダウン
					if(Wpd.HMM_temp>Wpd.HMM){
						EXP++;
						Wpd.HMM_temp--;
						if(Wpd.HMM_temp<64)	Wpd.flag &=_HMM1_OFF;//ホーミング1が使用不能になる
					}
					break;
				case 212:	//HNBゲージダウン
					if(Wpd.HNB_temp>Wpd.HNB){
						EXP++;
						Wpd.HNB_temp--;
						if(Wpd.HNB_temp<64)	Wpd.flag &=_HNB1_OFF;//花火1が使用不能になる
					}
					break;
				default:
					break;
			}
		}else if(CurCdn.x==84 && CurCdn.y==276){	//OPTゲージダウン
			if(Wpd.OPT_temp>Wpd.OPT){
				EXP++;
				Wpd.OPT_temp--;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////
	//----------------------- Aが押された瞬間 ----------------------------
	if((JoyNew^JoyOld)&(JoyNew&JOY_A)){
		//カーソルがMAINの位置にある場合
		if(CurCdn.x==84 && CurCdn.y==100)	CurCdn.x=184;	//LSRの位置
		else if(CurCdn.x==184 && CurCdn.y==100){	//LSR
			Wpd.flag |=_LSR_ON;		
			Wpd.flag &=_MSR_OFF;
		}else if(CurCdn.x==184 && CurCdn.y==124){	//MSR
			Wpd.flag |=_MSR_ON;
			Wpd.flag &=_LSR_OFF;
		}
		//カーソルがSUBの位置にある場合
		else if(CurCdn.x==84 && CurCdn.y==188)	CurCdn.x=184;	//HMMの位置
		else if(CurCdn.x==184 && CurCdn.y==188){	//HMM
			Wpd.flag |=_HMM_ON;
			Wpd.flag &=_HNB_OFF;
		}else if(CurCdn.x==184 && CurCdn.y==212){	//HNB
			Wpd.flag |=_HNB_ON;
			Wpd.flag &=_HMM_OFF;
		}
		//カーソルがOPTの位置にある場合
		else if(CurCdn.x==84 && CurCdn.y==276){	
			switch(Wpd.OPT_temp/64){	//オプションのレベル
				case 0:
					break;
				case 1:	//レベル１
					fJIKI |=_OPT_ON;		//自機フラグにオプション装備フラグを立てる
					Wpd.flag |=_OPT1_ON;	//OPTレベル１フラグオン

					//フラグが立っていない場合はオプションは固定に設定
					if(!(OPT.Jewel[0].fOPT&_fOPT_CHK))	OPT.Jewel[0].fOPT |=_OPTKO_ON;
					
					Wpd.OPT=Wpd.OPT_temp;	//Aを押すと割り振ったEXPは変更できない
					CurCdn.x=184;	
					CurCdn.y=308;
					break;
				case 2:	//レベル２
					fJIKI |=_OPT_ON;
					Wpd.flag &=_OPT1_OFF;
					Wpd.flag |=_OPT2_ON;

					for(i=0; i<2;i++){
						if(!(OPT.Jewel[i].fOPT&_fOPT_CHK))	OPT.Jewel[i].fOPT |=_OPTKO_ON;
					}

					Wpd.OPT=Wpd.OPT_temp;
					CurCdn.x=184;	
					CurCdn.y=308;
					break;
				case 3:	//レベル３
					fJIKI |=_OPT_ON;
					Wpd.flag &=_OPT1_OFF;
					Wpd.flag &=_OPT2_OFF;
					Wpd.flag |=_OPT3_ON;

					for(i=0; i<3;i++){
						if(!(OPT.Jewel[i].fOPT&_fOPT_CHK))	OPT.Jewel[i].fOPT |=_OPTKO_ON;
					}
					
					Wpd.OPT=Wpd.OPT_temp;
					CurCdn.x=184;	
					CurCdn.y=308;
					break;
				case 4:	//レベル４
					fJIKI |=_OPT_ON;
					Wpd.flag &=_OPT1_OFF;
					Wpd.flag &=_OPT2_OFF;
					Wpd.flag &=_OPT3_OFF;
					Wpd.flag |=_OPT4_ON;
					
					for(i=0; i<4;i++){
						if(!(OPT.Jewel[i].fOPT&_fOPT_CHK))	OPT.Jewel[i].fOPT |=_OPTKO_ON;
					}
					
					Wpd.OPT=Wpd.OPT_temp;
					CurCdn.x=184;	
					CurCdn.y=308;
					break;
				default:
					break;
			}
		}
		//EXIT
		else if(CurCdn.x==84 && CurCdn.y==450){
			InitOptCdn();	//オプションの表示座標の初期化
		
			Wpd.LSR=Wpd.LSR_temp;
			Wpd.MSR=Wpd.MSR_temp;
			Wpd.HMM=Wpd.HMM_temp;
			Wpd.HNB=Wpd.HNB_temp;
			Wpd.OPT=Wpd.OPT_temp;
			fJIKI &=_FUEL_OFF;		//弾吸収中フラグオフ
			fJIKI &=_PWUP_OFF;		//パワーアップ画面中フラグオフ
			fJIKI &=_GAME_STOP_OFF;	//ゲーム一時停止フラグオフ
			fJIKI |=_PWUPD_ON;		//パワーアップに伴う敵キャラ消滅フラグオン
			EXP_R=0;				//EXP_R値を0に戻す
			EXP=(BYTE)(EXP_R>>16);
		}
	}
	//////////////////////////////////////////////////////////////////////	
	//----------------------- Bが押された瞬間 ----------------------------
	if((JoyNew^JoyOld)&(JoyNew&JOY_B)){
		if(CurCdn.x==184){	//LSR,MSR,HMM,HNB,OPTの位置
			CurCdn.x=84;							//LSR,HMM
			if(CurCdn.y<=124)		CurCdn.y=100;	//MSR
			else if(CurCdn.y<=212)	CurCdn.y=188;	//HNB
			else if(CurCdn.y<=380)	CurCdn.y=276;	//OPT	
		}
		else if(CurCdn.x>=280)	CurCdn.x=184;		//LSR,MSR,HMM,HNBゲージの位置
	}
	
	//////////////////////////////////////////////////////////////////////	
	//-------------------------- 文字の表示 ------------------------------
	//EXP
	lpDDSBack->BltFast(100,50,lpDDS_JIKI,&Mj[14],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(116,50,lpDDS_JIKI,&Mj[33],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(132,50,lpDDS_JIKI,&Mj[25],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//MAIN
	lpDDSBack->BltFast(100,100,lpDDS_JIKI,&Mj[22],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(116,100,lpDDS_JIKI,&Mj[10],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(132,100,lpDDS_JIKI,&Mj[18],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(148,100,lpDDS_JIKI,&Mj[23],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//LSR
	lpDDSBack->BltFast(200,100,lpDDS_JIKI,&Mj[21],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(216,100,lpDDS_JIKI,&Mj[28],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(232,100,lpDDS_JIKI,&Mj[27],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//MSR
	lpDDSBack->BltFast(200,124,lpDDS_JIKI,&Mj[22],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(216,124,lpDDS_JIKI,&Mj[28],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(232,124,lpDDS_JIKI,&Mj[27],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//SUB
	lpDDSBack->BltFast(100,188,lpDDS_JIKI,&Mj[28],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(116,188,lpDDS_JIKI,&Mj[30],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(132,188,lpDDS_JIKI,&Mj[11],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//HMM
	lpDDSBack->BltFast(200,188,lpDDS_JIKI,&Mj[17],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(216,188,lpDDS_JIKI,&Mj[22],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(232,188,lpDDS_JIKI,&Mj[22],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//HNB
	lpDDSBack->BltFast(200,212,lpDDS_JIKI,&Mj[17],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(216,212,lpDDS_JIKI,&Mj[23],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(232,212,lpDDS_JIKI,&Mj[11],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//OPT
	lpDDSBack->BltFast(100,276,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(116,276,lpDDS_JIKI,&Mj[25],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(132,276,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//OPT1
	lpDDSBack->BltFast(200,308,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(216,308,lpDDS_JIKI,&Mj[25],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(232,308,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(248,308,lpDDS_JIKI,&Mj[1],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//KO KA TU 1
	lpDDSBack->BltFast(296,308,lpDDS_JIKI,&Mj[20],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(312,308,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(360,308,lpDDS_JIKI,&Mj[20],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(376,308,lpDDS_JIKI,&Mj[10],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(422,308,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(438,308,lpDDS_JIKI,&Mj[30],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//OPT2
	lpDDSBack->BltFast(200,332,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(216,332,lpDDS_JIKI,&Mj[25],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(232,332,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(248,332,lpDDS_JIKI,&Mj[2],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//KO KA TU 2
	lpDDSBack->BltFast(296,332,lpDDS_JIKI,&Mj[20],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(312,332,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(360,332,lpDDS_JIKI,&Mj[20],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(376,332,lpDDS_JIKI,&Mj[10],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(422,332,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(438,332,lpDDS_JIKI,&Mj[30],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//OPT3
	lpDDSBack->BltFast(200,356,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(216,356,lpDDS_JIKI,&Mj[25],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(232,356,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(248,356,lpDDS_JIKI,&Mj[3],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//KO KA TU 3
	lpDDSBack->BltFast(296,356,lpDDS_JIKI,&Mj[20],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(312,356,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(360,356,lpDDS_JIKI,&Mj[20],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(376,356,lpDDS_JIKI,&Mj[10],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(422,356,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(438,356,lpDDS_JIKI,&Mj[30],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//OPT4
	lpDDSBack->BltFast(200,380,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(216,380,lpDDS_JIKI,&Mj[25],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(232,380,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(248,380,lpDDS_JIKI,&Mj[4],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//KO KA TU 4
	lpDDSBack->BltFast(296,380,lpDDS_JIKI,&Mj[20],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(312,380,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(360,380,lpDDS_JIKI,&Mj[20],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(376,380,lpDDS_JIKI,&Mj[10],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(422,380,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(438,380,lpDDS_JIKI,&Mj[30],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//EXIT
	lpDDSBack->BltFast(100,450,lpDDS_JIKI,&Mj[14],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(116,450,lpDDS_JIKI,&Mj[33],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(132,450,lpDDS_JIKI,&Mj[18],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(148,450,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	
	//////////////////////////////////////////////////////////////////////		
	//------------------------- PWUゲージの表示 --------------------------
	//EXP
	GAGE_P.right=(SpJIKI[24].right-192)-(128-EXP);
	lpDDSBack->BltFast(248,50,lpDDS_JIKI,&GAGE_P,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_PW=SpJIKI[23];
	GAGE_PW.right -=192;
	lpDDSBack->BltFast(248,50,lpDDS_JIKI,&GAGE_PW,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//LSR
	GAGE_P.right=SpJIKI[24].right-(320-Wpd.LSR_temp);
	lpDDSBack->BltFast(248,100,lpDDS_JIKI,&GAGE_P,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_P2.right=SpJIKI[24].right-(320-(64*(Wpd.LSR_temp/64)));
	lpDDSBack->BltFast(248,100,lpDDS_JIKI,&GAGE_P2,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_PW=(Wpd.flag&_LSR_ON) ?  SpJIKI[25]:SpJIKI[23];
	lpDDSBack->BltFast(248,100,lpDDS_JIKI,&GAGE_PW,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//MSR
	GAGE_P.right=SpJIKI[24].right-(320-Wpd.MSR_temp);
	lpDDSBack->BltFast(248,124,lpDDS_JIKI,&GAGE_P,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_P2.right=SpJIKI[24].right-(320-(64*(Wpd.MSR_temp/64)));
	lpDDSBack->BltFast(248,124,lpDDS_JIKI,&GAGE_P2,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_PW=(Wpd.flag&_MSR_ON) ?  SpJIKI[25]:SpJIKI[23];
	lpDDSBack->BltFast(248,124,lpDDS_JIKI,&GAGE_PW,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//HMM
	GAGE_P.right=SpJIKI[24].right-(320-Wpd.HMM_temp);
	lpDDSBack->BltFast(248,188,lpDDS_JIKI,&GAGE_P,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_P2.right=SpJIKI[24].right-(320-(64*(Wpd.HMM_temp/64)));
	lpDDSBack->BltFast(248,188,lpDDS_JIKI,&GAGE_P2,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_PW=(Wpd.flag&_HMM_ON) ?  SpJIKI[25]:SpJIKI[23];
	lpDDSBack->BltFast(248,188,lpDDS_JIKI,&GAGE_PW,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//HNB
	GAGE_P.right=SpJIKI[24].right-(320-Wpd.HNB_temp);
	lpDDSBack->BltFast(248,212,lpDDS_JIKI,&GAGE_P,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_P2.right=SpJIKI[24].right-(320-(64*(Wpd.HNB_temp/64)));
	lpDDSBack->BltFast(248,212,lpDDS_JIKI,&GAGE_P2,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_PW=(Wpd.flag&_HNB_ON) ?  SpJIKI[25]:SpJIKI[23];
	lpDDSBack->BltFast(248,212,lpDDS_JIKI,&GAGE_PW,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//OPT
	GAGE_P.right=SpJIKI[24].right-(320-Wpd.OPT_temp);
	lpDDSBack->BltFast(248,276,lpDDS_JIKI,&GAGE_P,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_P2.right=SpJIKI[24].right-(320-(64*(Wpd.OPT_temp/64)));
	lpDDSBack->BltFast(248,276,lpDDS_JIKI,&GAGE_P2,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_PW=SpJIKI[23];
	GAGE_PW.right -=64;	//レベルが4までしか無い為矩形範囲を補正
	lpDDSBack->BltFast(248,276,lpDDS_JIKI,&GAGE_PW,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//OPT枠
	short	x=0,y=0;
	switch(Wpd.flag&0x00F00000){
		case _OPT4_ON:
			if(OPT.Jewel[3].fOPT&_OPTKO_ON)			{x=296;	y=380;}
			else if(OPT.Jewel[3].fOPT&_OPTKA_ON)	{x=360; y=380;}
			else if(OPT.Jewel[3].fOPT&_OPTTU_ON)	{x=422; y=380;}
			lpDDSBack->BltFast(x,y,lpDDS_JIKI,&OPTWAKU,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		case _OPT3_ON:
			if(OPT.Jewel[2].fOPT&_OPTKO_ON)			{x=296;	y=356;}
			else if(OPT.Jewel[2].fOPT&_OPTKA_ON)	{x=360; y=356;}
			else if(OPT.Jewel[2].fOPT&_OPTTU_ON)	{x=422; y=356;}
			lpDDSBack->BltFast(x,y,lpDDS_JIKI,&OPTWAKU,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		case _OPT2_ON:
			if(OPT.Jewel[1].fOPT&_OPTKO_ON)			{x=296;	y=332;}
			else if(OPT.Jewel[1].fOPT&_OPTKA_ON)	{x=360; y=332;}
			else if(OPT.Jewel[1].fOPT&_OPTTU_ON)	{x=422; y=332;}
			lpDDSBack->BltFast(x,y,lpDDS_JIKI,&OPTWAKU,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		case _OPT1_ON:
			if(OPT.Jewel[0].fOPT&_OPTKO_ON)			{x=296;	y=308;}
			else if(OPT.Jewel[0].fOPT&_OPTKA_ON)	{x=360; y=308;}
			else if(OPT.Jewel[0].fOPT&_OPTTU_ON)	{x=422; y=308;}
			lpDDSBack->BltFast(x,y,lpDDS_JIKI,&OPTWAKU,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			break;
		default:
			break;
	}

	//////////////////////////////////////////////////////////////////////		
	//----------------------- カーソルの表示 -----------------------------
	lpDDSBack->BltFast(	CurCdn.x,CurCdn.y,
						lpDDS_JIKI,&Mj[36],
						DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
}

//////////////////////////////////////////////////////////////////////
//	コンティニュー画面の処理と描画
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::RendCONTINUE()
{
	//ジョイスティックorキーポードからの入力を取得
	JoyOld=JoyNew;
	JoyNew=Joy_Get();
	
	//Aが押すとゲーム再開
	if((JoyNew^JoyOld)&(JoyNew&JOY_A)){
		//コンティニューに伴う自機データの初期化を行う
		InitCONTINUE();
		//ゲーム一時停止フラグオフ
		fJIKI &=_GAME_STOP_OFF;
		//コンティニューが選択されたことを示すフラグオン
		fJIKI |=_CONTINUE_ON;
		//一定時間無敵であることを示すフラグオン
		fJIKI |=_INVINCIBLE_ON;
		//無敵時間をセット
		InvincCnt=150;
		ContinueCnt=0;
		return;
	//Bを押すとカウンタに100足す
	}else if((JoyNew^JoyOld)&(JoyNew&JOY_B)){
		ContinueCnt +=120;
	}
	
	ContinueCnt++;	//コンティニューカウンタを進める
	
	//タイムオーバーの処理
	if(ContinueCnt>1199){
		ContinueCnt=0;
		fSTAGE |=_STAGE_ENDWAIT_ON;	//ステージ終了フラグオン
		fSTAGE |=_GAME_OVER_ON;		//ゲームオーバーフラグオン
		return;
	}
	
	//数字(0-9)の表示
	lpDDSBack->BltFast(	314,236,
						lpDDS_JIKI,&Mj[47-(ContinueCnt/120)],
						DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	
	//CONTINUE ?の文字表示
	lpDDSBack->BltFast(	236,204,
						lpDDS_JIKI,&Mj[37],
						DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
}

//////////////////////////////////////////////////////////////////////
//	コンティニューに伴う自機データの初期化を行う
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::InitCONTINUE()
{
	//必要の無いフラグをオフ
	fJIKI &=_ARM_OFF;	//アーム発射フラグオフ
	fJIKI &=_FUEL_OFF;	//弾吸収中フラグオフ
	
	//自機の画面座標を初期化(下端の真中からスタート)
	Cdn.x=(WinXMax/2)-(_JIKI_W/2);
	Cdn.y=WinYMax-32;
	
	//自機の絶対座標を初期化
	MapCdn.x=MainCdn.x+Cdn.x;
	MapCdn.y=MainCdn.y+Cdn.y;

	MainWepnReset();	//主砲の弾を全て発射待機状態に戻す
	HmmReset();			//ホーミングミサイルを全て発射待機状態に戻す
	HnbReset();			//花火を全て発射待機状態に戻す
	InitOptCdn();		//オプションの表示座標を初期化
	OptAttackReset();	//オプションの発射した弾を全て発射待機状態戻す
}

//////////////////////////////////////////////////////////////////////
// 現在のスコアを画面に表示するためスプライトサーフェスの矩形範囲に変換
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::ScoreCalcul()
{
	DWORD	sc=SCORE;			//スコアの値をコピー
	DWORD	rectnum=0;			//数字の番号保存用
	
	rectnum=sc / 100000000;		//1億での商を求めます
	SCORE_R[0]=Mj[rectnum];		//9桁目の数字スプライト
	sc	=SCORE % 100000000;		//次の桁の計算用

	rectnum=sc / 10000000;		//1000万での商を求めます
	SCORE_R[1]=Mj[rectnum];		//8桁目の数字スプライト
	sc	=SCORE % 10000000;

	rectnum=sc / 1000000;		//100万での商を求めます
	SCORE_R[2]=Mj[rectnum];		//7桁目の数字スプライト
	sc	=SCORE % 1000000;

	rectnum=sc / 100000;		//10万での商を求めます
	SCORE_R[3]=Mj[rectnum];		//6桁目の数字スプライト
	sc	=SCORE % 100000;

	rectnum=sc / 10000;			//1万での商を求めます
	SCORE_R[4]=Mj[rectnum];		//5桁目の数字スプライト
	sc	=SCORE % 10000;

	rectnum=sc / 1000;			//1000での商を求めます
	SCORE_R[5]=Mj[rectnum];		//4桁目の数字スプライト
	sc	=SCORE % 1000;

	rectnum=sc / 100;			//100での商を求めます
	SCORE_R[6]=Mj[rectnum];		//3桁目の数字スプライト
	sc	=SCORE % 100;

	rectnum=sc / 10;			//10での商を求めます
	SCORE_R[7]=Mj[rectnum];		//2桁目の数字スプライト

	SCORE_R[8]=Mj[0];			//1桁目は常に0
}