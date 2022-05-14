/*====================================================================
	ボスキャラ01の処理に関するソースファイル
=====================================================================*/
#include	<windows.h>
#include	<ddraw.h>
#include	<dsound.h>
#include	<time.h>

#include	"Macros.h"
#include	"Draw.h"	
#include	"Struct.h"
#include	"BOSS01.h"
#include	"DrawSPDataEXT.h"
#include	"DrawMacros.h"

//ダイレクトサウンドバッファ再生用マクロ定義
#define	_JIKIBAKU	0
#define	_TEKIBAKU	1
#define	_FUELARM	2
#define	_FIRE		3
#define	_TAMAKYU	4
#define	_BOSSBAKU	5

extern	LPDIRECTSOUNDBUFFER		lpDSBuf[6]; //ダイレクトサウンドバッファ

extern	LPDIRECTDRAWSURFACE7	lpDDSBack;
void	DrawText(char*,long,int,int,COLORREF);	//テキスト+変数表示
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BOSS01::BOSS01()
{
	B_Msr.tm2			=NULL;
	B_Block.tm3			=NULL;
}

BOSS01::~BOSS01()
{	
	RELEASE_dels(B_Msr.tm2);
	RELEASE_dels(B_Block.tm3);
}

//////////////////////////////////////////////////////////////////////
//	ボスキャラ01の各データの初期化
//	引数:ボスキャラ01の配置されている絶対座標
//	戻り値:TRUE-初期化成功		FALSE-失敗
//////////////////////////////////////////////////////////////////////
BOOL	BOSS01::InitBoss01(POINT Tcdn)
{
	//再初期化に必要な処理
	RELEASE_dels(B_Msr.tm2);
	RELEASE_dels(B_Block.tm3);

	//乱数の初期化
	srand( (unsigned)time( NULL ) );
	
	//状態を表すフラグを初期化
	fSPChr=0;	

	Width=576;			//ボスキャラ01の横幅
	Height=320;			//ボスキャラ02の縦幅
	Shld=9000;			//ボスキャラ01破壊までの耐久力を初期化
	ShldCnt=Shld;		//ボスキャラ01残り耐久力のカウンタを初期化
	ATmt[0]=30;			//攻撃態勢に入るまでのウェイト数配列初期化
	ATmt[1]=50;
	ATmtCnt[0]=ATmt[0];	//攻撃態勢に入るまでのウェイト数のカウンタ配列初期化
	ATmtCnt[1]=ATmt[1];
	ATt[0]=100;			//攻撃を続ける時間配列初期化
	ATt[1]=20;			
	ATtCnt[0]=ATt[0];	//攻撃を続ける時間のカウンタ配列初期化
	ATtCnt[1]=ATt[1];

	ONTime=0;	//画面に現れてからの時間を初期化

	MovKakudo=0;	//ボスキャラの回転角度
	Mov.x=5;		//ボスキャラ01の移動速度の入った構造体を初期化
	Mov.y=5;
			
	MapCdn=Tcdn;				//ボスキャラ01の配置されている絶対座標を初期化		
	Cdn.x=MapCdn.x-MainCdn.x;	//ボスキャラ01の配置されている画面座標を初期化
	Cdn.y=MapCdn.y-MainCdn.y;
	dCdn.x=MapCdn.x;			//座標計算用の構造体を初期化
	dCdn.y=MapCdn.y;

	//ボスキャラ01が現在表示している画面上に存在しているのかどうかチェック
	if(MainCdn.y>MapCdn.y+Height)			{fSPChr &=_SPOFF;}
	else if(MainCdn.y+WinYMax<MapCdn.y)		{fSPChr &=_SPOFF;}
	else if(MainCdn.x>MapCdn.x+Width)		{fSPChr &=_SPOFF;}
	else if(MainCdn.x+WinXMax<MapCdn.x)		{fSPChr &=_SPOFF;}
	else	fSPChr |=_SPON;		//表示フラグをオン

	//-------------------------弾オブジェクトを初期化-------------------------------//
	
	//--------------------------攻撃パターン1用------------------------------------
	B_Msr.TTama=21;				//画面に存在できる弾の数
	B_Msr.WTama=12;				//弾を発射する間隔
	B_Msr.WTamaCnt=B_Msr.WTama;	//最初の弾発射までのカウンタ
	
	//発射待機状態にする弾番号保存用変数を初期化(攻撃パターン1)
	for(short i=0;i<7;i++){
		AttackTama1[i]=B_Msr.TTama;
	}

	//TAMA2構造体に前もって値をセットして弾データを初期化する
	TAMA2	tama2;
	char	*pTama;
	
	tama2.fTama=_TOFF;		//弾の初期状態を設定
	tama2.Width=8;			//弾の横幅
	tama2.Height=8;			//弾の縦幅
	tama2.Kakudo=0;			//弾の発射角度
	tama2.Time=0;			//弾が発射されてからの経過時間		
	tama2.Mov.x=0.0;		//弾の移動速度を0で初期化
	tama2.Mov.y=0.0;
	tama2.dCdn.x=0.0;		//弾の座標計算用の座標を0で初期化
	tama2.dCdn.y=0.0;
	tama2.Cdn.x=0;			//弾の現在の画面座標を0で初期化
	tama2.Cdn.y=0;
	tama2.MapCdn.x=0;		//弾の現在の絶対座標も0で初期化
	tama2.MapCdn.y=0;
	tama2.Anime=TamAme[0];	//弾アニメーションパターン数の初期化	
	memcpy(&tama2.AnimePat[0],&TamPat[0][0],sizeof(BYTE)*10);//弾アニメーションパターン配列の初期化
	memcpy(&tama2.AnimeFrame[0],&TamFrame[0][0],sizeof(BYTE)*10);//弾アニメーション切り替えタイミング配列の初期化
	tama2.AnimeCnt=0;					//最初に表示するアニメーションパターンを初期化
	tama2.FrameCnt=tama2.AnimeFrame[0];	//最初のアニメーションフレームを初期化
	tama2.SrcR.left=0;					//転送元矩形を0で初期化
	tama2.SrcR.top=0;
	tama2.SrcR.right=0;
	tama2.SrcR.bottom=0;
	tama2.DesR=tama2.SrcR;			//転送先矩形も0で初期化
	
	//発射できる弾数分TAMA2構造体を確保
	B_Msr.tm2=new TAMA2[B_Msr.TTama];
	if(!B_Msr.tm2)		return	FALSE;
	//確保した領域の先頭アドレスをコピー
	pTama=(char*)B_Msr.tm2;		
	//弾オブジェクトを確保した分、初期化 
	for(i=0;(DWORD)i<B_Msr.TTama;i++){
		memcpy(pTama,&tama2,sizeof(TAMA2));	//確保した領域の先頭からコピー
		pTama +=sizeof(TAMA2);				//次にコピーするところまでアドレスを進める
	}
	
	//-----------------------攻撃パターン2用------------------------------
	T_Ichi=512;						//B_Blockを発射する位置
	
	B_Block.TTama=100;				//画面に存在できる弾の数
	B_Block.WTama=1;				//弾を発射する間隔
	B_Block.WTamaCnt=B_Block.WTama;	//最初の弾発射までのカウンタ
	
	AttackTama2=B_Block.TTama;		//発射待機状態にする弾番号保存用変数を初期化(攻撃パターン1)

	//TAMA3構造体に前もって値をセットして弾データを初期化する
	TAMA3	tama3;
	
	tama3.fTama=_TOFF;		//弾の初期状態を設定
	tama3.Width=64;			//弾の横幅
	tama3.Height=32;		//弾の縦幅
	tama3.Kakudo=0;			//弾の発射角度(今回は未使用)
	tama3.Time=0;			//弾が発射されてからの経過時間		
	tama3.Shld=8;			//弾破壊までの耐久力
	tama3.ShldCnt=tama3.Shld;		//弾残り耐久力のカウンタ
	tama3.Mov.x=0.0;		//弾の移動速度を0で初期化
	tama3.Mov.y=0.0;
	tama3.dCdn.x=0.0;		//弾の座標計算用の座標を0で初期化
	tama3.dCdn.y=0.0;
	tama3.Cdn.x=0;			//弾の現在の画面座標を0で初期化
	tama3.Cdn.y=0;
	tama3.MapCdn.x=0;		//弾の現在の絶対座標も0で初期化
	tama3.MapCdn.y=0;
	tama3.Anime=TamAme[2];	//弾アニメーションパターン数の初期化	
	memcpy(&tama3.AnimePat[0],&TamPat[2][0],sizeof(BYTE)*10);//弾アニメーションパターン配列の初期化
	memcpy(&tama3.AnimeFrame[0],&TamFrame[2][0],sizeof(BYTE)*10);//弾アニメーション切り替えタイミング配列の初期化
	tama3.AnimeCnt=0;					//最初に表示するアニメーションパターンを初期化
	tama3.FrameCnt=tama3.AnimeFrame[0];	//最初のアニメーションフレームを初期化
	tama3.BakuWidth=32;			//弾の爆発シーン横幅を初期化
	tama3.BakuHeight=32;		//弾の爆発シーン縦幅を初期化
	tama3.BakuAnime=BakuAme[1];	//爆発アニメーションパターン数を初期化
	tama3.BakuAnimePat[10];		//爆発アニメーションパターン配列を初期化
	memcpy(&tama3.BakuAnimePat[0],&BakuPat[1][0],sizeof(BYTE)*10);//爆発アニメーションパターン配列初期化
	memcpy(&tama3.BakuAnimeFrame[0],&BakuFrame[1][0],sizeof(BYTE)*10);//爆発アニメーション切り替えタイミング配列の初期化
	tama3.BakuAnimeCnt=0;		//現在表示する爆発アニメーションパターンを初期化
	tama3.BakuFrameCnt=tama3.BakuAnimeFrame[0];	//現在の爆発アニメーションフレームを初期化
	tama3.SrcR.left=0;			//転送元矩形を0で初期化を初期化
	tama3.SrcR.top=0;
	tama3.SrcR.right=0;
	tama3.SrcR.bottom=0;
	tama3.DesR=tama3.SrcR;		//転送先矩形も0で初期化を初期化
	
	//発射できる弾数分TAMA3構造体を確保
	B_Block.tm3=new TAMA3[B_Block.TTama];
	if(!B_Block.tm3)		return	FALSE;
	//確保した領域の先頭アドレスをコピー
	pTama=(char*)B_Block.tm3;		
	//弾オブジェクトを確保した分、初期化 
	for(i=0;(DWORD)i<B_Block.TTama;i++){
		memcpy(pTama,&tama3,sizeof(TAMA3));	//確保した領域の先頭からコピー
		pTama +=sizeof(TAMA3);				//次にコピーするところまでアドレスを進める
	}
	
	//----------------ボスキャラ01を構成する描画用データの初期化--------------------//

	//パーツ1の初期化
	Parts1.Local.x=Cdn.x+224;				//描画する基準となる座標から距離
	Parts1.Local.y=Cdn.y;			
	Parts1.Width=96;						//パーツの横幅初期化
	Parts1.Height=64;						//パーツの縦幅初期化
	Parts1.SrcR=SP[19];						//転送元サーフェスの矩形範囲初期化
	Parts1.DesR.left=Parts1.Local.x;		//転送先サーフェスの矩形範囲初期化
	Parts1.DesR.top=Parts1.Local.y;			
	Parts1.DesR.right=Parts1.Local.x+Parts1.Width;
	Parts1.DesR.bottom=Parts1.Local.y+Parts1.Height;
	
	//パーツ2の初期化
	Parts2.Local.x=Cdn.x+96;				//描画する基準となる座標から距離
	Parts2.Local.y=Cdn.y+64;			
	Parts2.Width=384;						//パーツの横幅初期化
	Parts2.Height=32;						//パーツの縦幅初期化
	Parts2.SrcR=SP[20];						//転送元サーフェスの矩形範囲初期化
	Parts2.DesR.left=Parts2.Local.x;		//転送先サーフェスの矩形範囲初期化
	Parts2.DesR.top=Parts2.Local.y;			
	Parts2.DesR.right=Parts2.Local.x+Parts2.Width;
	Parts2.DesR.bottom=Parts2.Local.y+Parts2.Height;

	//パーツ3の初期化
	Parts3.Local.x=Cdn.x;					//描画する基準となる座標から距離
	Parts3.Local.y=Cdn.y+96;				
	Parts3.Width=576;						//パーツの横幅初期化
	Parts3.Height=64;						//パーツの縦幅初期化
	Parts3.Anime=BossAnime[0];				//アニメーションパターン数の初期化
	memcpy(&Parts3.AnimePat[0],&BossPat[0][0],sizeof(BYTE)*10);//アニメーションパターン配列の初期化
	memcpy(&Parts3.AnimeFrame[0],&BossFrame[0][0],sizeof(BYTE)*10);//アニメーション切り替えタイミング配列の初期化
	Parts3.AnimeCnt=0;						//現在表示するアニメーションパターンわ初期化
	Parts3.FrameCnt=Parts3.AnimeFrame[0];	//現在のアニメーションフレームを初期化
	Parts3.SrcR=SP[Parts3.AnimePat[Parts3.AnimeCnt]];	//転送元サーフェスの矩形範囲初期化
	Parts3.DesR.left=Parts3.Local.x;					//転送先サーフェスの矩形範囲初期化
	Parts3.DesR.top=Parts3.Local.y;			
	Parts3.DesR.right=Parts3.Local.x+Parts3.Width;
	Parts3.DesR.bottom=Parts3.Local.y+Parts3.Height;

	//パーツ4の初期化
	Parts4.Local.x=Cdn.x+128;				//描画する基準となる座標から距離
	Parts4.Local.y=Cdn.y+160;				
	Parts4.Width=288;						//パーツの横幅初期化
	Parts4.Height=32;						//パーツの縦幅初期化
	Parts4.Anime=BossAnime[1];				//アニメーションパターン数の初期化
	memcpy(&Parts4.AnimePat[0],&BossPat[1][0],sizeof(BYTE)*10);//アニメーションパターン配列の初期化
	memcpy(&Parts4.AnimeFrame[0],&BossFrame[1][0],sizeof(BYTE)*10);//アニメーション切り替えタイミング配列の初期化
	Parts4.AnimeCnt=0;						//現在表示するアニメーションパターンわ初期化
	Parts4.FrameCnt=Parts4.AnimeFrame[0];	//現在のアニメーションフレームを初期化
	Parts4.SrcR=SP[Parts4.AnimePat[Parts4.AnimeCnt]];	//転送元サーフェスの矩形範囲初期化
	Parts4.DesR.left=Parts4.Local.x;					//転送先サーフェスの矩形範囲初期化
	Parts4.DesR.top=Parts4.Local.y;			
	Parts4.DesR.right=Parts4.Local.x+Parts4.Width;
	Parts4.DesR.bottom=Parts4.Local.y+Parts4.Height;

	//パーツ5の初期化
	Parts5.Local.x=Cdn.x+192;				//描画する基準となる座標から距離
	Parts5.Local.y=Cdn.y+192;			
	Parts5.Width=192;						//パーツの横幅初期化
	Parts5.Height=32;						//パーツの縦幅初期化
	Parts5.SrcR=SP[23];						//転送元サーフェスの矩形範囲初期化
	Parts5.DesR.left=Parts5.Local.x;		//転送先サーフェスの矩形範囲初期化
	Parts5.DesR.top=Parts5.Local.y;			
	Parts5.DesR.right=Parts5.Local.x+Parts5.Width;
	Parts5.DesR.bottom=Parts5.Local.y+Parts5.Height;

	//パーツ6の初期化
	Parts6.Local.x=Cdn.x+224;				//描画する基準となる座標から距離
	Parts6.Local.y=Cdn.y+224;				
	Parts6.Width=128;						//パーツの横幅初期化
	Parts6.Height=96;						//パーツの縦幅初期化
	Parts6.Anime=BossAnime[2];				//アニメーションパターン数の初期化
	memcpy(&Parts6.AnimePat[0],&BossPat[2][0],sizeof(BYTE)*10);//アニメーションパターン配列の初期化
	memcpy(&Parts6.AnimeFrame[0],&BossFrame[2][0],sizeof(BYTE)*10);//アニメーション切り替えタイミング配列の初期化
	Parts6.AnimeCnt=0;						//現在表示するアニメーションパターンわ初期化
	Parts6.FrameCnt=Parts6.AnimeFrame[0];	//現在のアニメーションフレームを初期化
	Parts6.SrcR=SP[Parts6.AnimePat[Parts6.AnimeCnt]];	//転送元サーフェスの矩形範囲初期化
	Parts6.DesR.left=Parts6.Local.x;					//転送先サーフェスの矩形範囲初期化
	Parts6.DesR.top=Parts6.Local.y;			
	Parts6.DesR.right=Parts6.Local.x+Parts6.Width;
	Parts6.DesR.bottom=Parts6.Local.y+Parts6.Height;

	//----------------爆発シーンのアニメーションデータの初期化----------------------//
	
	//爆発1
	
	BOSSANIME2	baku1;

	baku1.Local.x=0;					//描画する基準となる座標から距離
	baku1.Local.y=0;				
	baku1.Width=32;						//パーツの横幅初期化
	baku1.Height=32;						//パーツの縦幅初期化
	baku1.Anime=BakuAme[0];					//アニメーションパターン数の初期化
	memcpy(&baku1.AnimePat[0],&BakuPat[0][0],sizeof(BYTE)*10);//アニメーションパターン配列の初期化
	memcpy(&baku1.AnimeFrame[0],&BakuFrame[0][0],sizeof(BYTE)*10);//アニメーション切り替えタイミング配列の初期化
	baku1.AnimeCnt=0;						//現在表示するアニメーションパターンわ初期化
	baku1.FrameCnt=baku1.AnimeFrame[0];		//現在のアニメーションフレームを初期化
	baku1.SrcR=SP[baku1.AnimePat[baku1.AnimeCnt]];	//転送元サーフェスの矩形範囲初期化
	baku1.DesR.left=0;				//転送先サーフェスの矩形範囲初期化
	baku1.DesR.top=0;			
	baku1.DesR.right=0;
	baku1.DesR.bottom=0;
	
	for(i=0;i<12;i++){
		baku1.Local.x=rand()%128;	//爆発する座標を乱数で求めて初期化
		baku1.Local.y=rand()%96;
		BossBaku1[i]=baku1;
	}
	
	//爆発2
	BOSSANIME2	baku2;

	baku2.Local.x=0;					//描画する基準となる座標から距離
	baku2.Local.y=0;				
	baku2.Width=64;						//パーツの横幅初期化
	baku2.Height=64;					//パーツの縦幅初期化
	baku2.Anime=BakuAme[1];				//アニメーションパターン数の初期化
	memcpy(&baku2.AnimePat[0],&BakuPat[1][0],sizeof(BYTE)*10);//アニメーションパターン配列の初期化
	memcpy(&baku2.AnimeFrame[0],&BakuFrame[1][0],sizeof(BYTE)*10);//アニメーション切り替えタイミング配列の初期化
	baku2.AnimeCnt=0;						//現在表示するアニメーションパターンわ初期化
	baku2.FrameCnt=baku2.AnimeFrame[0];		//現在のアニメーションフレームを初期化
	baku2.SrcR=SP[baku2.AnimePat[baku2.AnimeCnt]];	//転送元サーフェスの矩形範囲初期化
	baku2.DesR.left=0;				//転送先サーフェスの矩形範囲初期化
	baku2.DesR.top=0;			
	baku2.DesR.right=0;
	baku2.DesR.bottom=0;
	
	for(i=0;i<200;i++){
		baku2.Local.x=rand()%576;	//爆発する座標を乱数で求めて初期化
		baku2.Local.y=rand()%320;				
		BossBaku2[i]=baku2;
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////
//	ボスキャラ01の移動処理
//////////////////////////////////////////////////////////////////////
void	BOSS01::MovBoss01()
{
	//ボスキャラが破壊されているならリターン
	if((fSPChr&_SPDESCHK)==(DWORD)_SPDES)	return;
	
	//ボスキャラが現在の表示画面範囲に入っているかチェック
	if(MainCdn.y>MapCdn.y+Height)			fSPChr &=_SPOFF;
	else if(MainCdn.y+WinYMax<MapCdn.y)		fSPChr &=_SPOFF;
	else if(MainCdn.x>MapCdn.x+Width)		fSPChr &=_SPOFF;
	else if(MainCdn.x+WinXMax>MapCdn.x){												
		//表示フラグオン
		fSPChr |=_SPON;
		/*
		//時計回りに回転
		if(MovKakudo<=180){
			dCdn.x +=Mov.x*Cos[MovKakudo];
			dCdn.y +=Mov.y*Sin[MovKakudo];
		}else if(MovKakudo>180){
			dCdn.x +=Mov.x*Cos[MovKakudo];
			dCdn.y +=-Mov.y*Sin[MovKakudo];
		}
		*/	
		//現在の座標に変換
		MapCdn.x=(LONG)dCdn.x;
		MapCdn.y=(LONG)dCdn.y;
		Cdn.x=MapCdn.x-MainCdn.x;	//ボスキャラ01の配置されている画面座標を初期化
		Cdn.y=MapCdn.y-MainCdn.y;
	
		//回転角度を進める
		MovKakudo=MovKakudo+10>359 ? 0:MovKakudo+10;

		//各パーツの座標をセット
		Parts1.Local.x=Cdn.x+224;
		Parts1.Local.y=Cdn.y;		
		Parts2.Local.x=Cdn.x+96;
		Parts2.Local.y=Cdn.y+64;
		Parts3.Local.x=Cdn.x;
		Parts3.Local.y=Cdn.y+96;
		Parts4.Local.x=Cdn.x+128;
		Parts4.Local.y=Cdn.y+160;	
		Parts5.Local.x=Cdn.x+192;
		Parts5.Local.y=Cdn.y+192;
		Parts6.Local.x=Cdn.x+224;
		Parts6.Local.y=Cdn.y+224;
	}
}

//////////////////////////////////////////////////////////////////////
//	ボスキャラ01の攻撃処理1
//////////////////////////////////////////////////////////////////////
void	BOSS01::Attack1Boss01(POINT j_cdn)
{
	bool	ret=FALSE;
	
	//----------------------------------------------------------//
	//					   ミサイルの移動処理					//
	//----------------------------------------------------------//
	
	//既に発射されている弾の座標を進める
	for(short i=0;(DWORD)i<B_Msr.TTama;i++){
		//既に発射されている弾のみ処理する
		if(B_Msr.tm2[i].fTama&_TON)
		{ 
			//一度でも弾移動処理が行われた場合retにTRUEを代入
			ret=TRUE;
			
			//弾を移動させる
			B_Msr.tm2[i].dCdn.x +=B_Msr.tm2[i].Mov.x;
			B_Msr.tm2[i].dCdn.y +=B_Msr.tm2[i].Mov.y;				
			//弾の絶対座標を進める
			B_Msr.tm2[i].MapCdn.x=(LONG)B_Msr.tm2[i].dCdn.x;
			B_Msr.tm2[i].MapCdn.y=(LONG)B_Msr.tm2[i].dCdn.y;
			//弾の画面座標を求める
			B_Msr.tm2[i].Cdn.x=B_Msr.tm2[i].MapCdn.x-MainCdn.x;
			B_Msr.tm2[i].Cdn.y=B_Msr.tm2[i].MapCdn.y-MainCdn.y;
				
			//弾が画面外に移動してないかチェック
			if(	B_Msr.tm2[i].MapCdn.x+B_Msr.tm2[i].Width<MainCdn.x	||
				B_Msr.tm2[i].MapCdn.x>MainCdn.x+WinXMax				||
				B_Msr.tm2[i].MapCdn.y+B_Msr.tm2[i].Height<MainCdn.y	||
				B_Msr.tm2[i].MapCdn.y>MainCdn.y+WinYMax)
			{
				B_Msr.tm2[i].fTama &=_TOFF;		//弾フラグオフ
				continue;
			}
		}else if(B_Msr.tm2[i].fTama==_TOFF){
			//再発射可能なミサイルの番号を保存する
			for(short j=0;(DWORD)j<7;j++){
				if(AttackTama1[j]==B_Msr.TTama){
					AttackTama1[j]=i;
					B_Msr.tm2[i].fTama |=_TSET;	//ミサイルを再び発射セット
					break;
				}
			}
			//画面に弾が存在していない時はループを抜ける
			if(!(AttackTama1[6]==B_Msr.TTama) && !(fSPChr&_TAMA2ON)){
				break;
			}
		}
	}
	
	//retの値がFALSEのままだった場合、画面に弾は存在していない
	if(!ret)	fSPChr &=_TAMA2OFF;		//画面弾存在フラグをオフ
	
	//敵キャラが表示されていない場合リターン
	if(!(fSPChr&_SPON))			return;
	//敵キャラの破壊フラグが立っている場合リターン
	else if(fSPChr&_SPDES)		return;
	
	//----------------------------------------------------------//
	//					   ミサイルの発射処理					//
	//----------------------------------------------------------//
	
	ret=TRUE;
	
	double	X,Y,x,y;
	short	Kakudo;
	
	//ミサイルの発射処理
	if(!B_Msr.WTamaCnt--){
		
		//ミサイルが指定された数以上に画面に存在しないようにする処理
		for(short i=0;i<7;i++){	//7発分発射可能な弾が確保されているかチェック
			if(AttackTama1[i]>=B_Msr.TTama)	ret=FALSE;
		}
		
		//発射の処理
		if(ret){
			//画面上に弾が発射されているフラグオン
			fSPChr |=_TAMA2ON;
					
			//自機との角度を求める(弾はパーツ6から発射される)
			X=(j_cdn.x+10)-(Parts6.Local.x+64);//中心点で比較。64=(Parts6.Width/2)
			Y=(j_cdn.y+16)-(Parts6.Local.y+48);//48=(Parts6.Height/2)
			if(X==0){ 
				if(Y<0)			Kakudo=90;	
				else if(Y>0)	Kakudo=270;
			}else if(Y==0){
				if(X<0)			Kakudo=180;
				else if(X>0)	Kakudo=0;
			}else{
				//自機との角度を計算
				x=X<0 ? -X:X;
				y=Y<0 ? -Y:Y;
				Kakudo=TanKaku(y/x);	//タンジェント値を角度に変換
				//求まった角度をそれぞれの象限に対応した角度に変換(第1象限はそのまま)
				if(X<0){
					if(Y<0)			Kakudo=180-Kakudo;//第2象限
					else if(Y>0)	Kakudo=180+Kakudo;//第3象限
				}else if(X>0){
					if(Y>0)			Kakudo=360-Kakudo;//第4象限
				}
			}	

			Kakudo -=42;
			if(Kakudo<0)	Kakudo=360+Kakudo;
			
			//ミサイルの発射された座標を初期化
			//画面座標をセット
			for(i=0;i<7;i++){
				//発射中のフラグを立てる
				B_Msr.tm2[AttackTama1[i]].fTama |=_TON;		
				//弾アニメーションを初期化
				B_Msr.tm2[AttackTama1[i]].AnimeCnt=0;		
				B_Msr.tm2[AttackTama1[i]].FrameCnt=B_Msr.tm2[AttackTama1[i]].AnimeFrame[0];	
				//初期座標
				B_Msr.tm2[AttackTama1[i]].Cdn.x=Parts6.Local.x+64;//64=(Parts6.Width/2)
				B_Msr.tm2[AttackTama1[i]].Cdn.y=Parts6.Local.y+48;//48=(Parts6.Height/2)
				//弾の移動計算用に画面座標を取得
				B_Msr.tm2[AttackTama1[i]].dCdn.x=MainCdn.x+B_Msr.tm2[AttackTama1[i]].Cdn.x;
				B_Msr.tm2[AttackTama1[i]].dCdn.y=MainCdn.y+B_Msr.tm2[AttackTama1[i]].Cdn.y;
				//絶対座標をセット
				B_Msr.tm2[AttackTama1[i]].MapCdn.x=(LONG)B_Msr.tm2[AttackTama1[i]].dCdn.x;
				B_Msr.tm2[AttackTama1[i]].MapCdn.y=(LONG)B_Msr.tm2[AttackTama1[i]].dCdn.y;
				//弾の発射角度と移動量を初期化
				B_Msr.tm2[AttackTama1[i]].Kakudo=Kakudo;
				B_Msr.tm2[AttackTama1[i]].Mov.x=3*Cos[Kakudo];
				B_Msr.tm2[AttackTama1[i]].Mov.y=-3*Sin[Kakudo];
				
				Kakudo +=14;
				if(Kakudo>359)	Kakudo=Kakudo-360;

				//B_Msr.TTamaで初期化
				//この処理はAttackTama1が上書きされない限り新しく弾を発射できないようにしています
				AttackTama1[i]=B_Msr.TTama;
			}
		}			
		B_Msr.WTamaCnt=B_Msr.WTama;	//発射までのカウンタを再セット
	}
	
}

//////////////////////////////////////////////////////////////////////
//	ボスキャラ01の攻撃処理2
//////////////////////////////////////////////////////////////////////
void	BOSS01::Attack2Boss01()
{
	
	bool	ret=FALSE;
	
	//----------------------------------------------------------//
	//					   ブロックの移動処理					//
	//----------------------------------------------------------//
	
	//既に発射されている弾の座標を進める
	for(short i=0;(DWORD)i<B_Block.TTama;i++){
		//既に発射されている弾のみ処理する
		if(B_Block.tm3[i].fTama&_TON)
		{ 
			//一度でも弾移動処理が行われた場合retにTRUEを代入
			ret=TRUE;
			
			//弾を移動させる
			B_Block.tm3[i].dCdn.x +=B_Block.tm3[i].Mov.x;
			B_Block.tm3[i].dCdn.y +=B_Block.tm3[i].Mov.y;				
			//弾の絶対座標を進める
			B_Block.tm3[i].MapCdn.x=(LONG)B_Block.tm3[i].dCdn.x;
			B_Block.tm3[i].MapCdn.y=(LONG)B_Block.tm3[i].dCdn.y;
			//弾の画面座標を求める
			B_Block.tm3[i].Cdn.x=B_Block.tm3[i].MapCdn.x-MainCdn.x;
			B_Block.tm3[i].Cdn.y=B_Block.tm3[i].MapCdn.y-MainCdn.y;
				
			//弾が画面外に移動してないかチェック
			if(	B_Block.tm3[i].MapCdn.x+B_Block.tm3[i].Width<MainCdn.x	||
				B_Block.tm3[i].MapCdn.x>MainCdn.x+WinXMax				||
				B_Block.tm3[i].MapCdn.y+B_Block.tm3[i].Height<MainCdn.y	||
				B_Block.tm3[i].MapCdn.y>MainCdn.y+WinYMax)
			{
				B_Block.tm3[i].fTama=_TOFF;		//弾フラグオフ
				continue;
			}
		}else if(B_Block.tm3[i].fTama&_TBAKU_ON){
			ret=TRUE;	//爆発アニメーションが終了するまで弾は存在している
		}else if(B_Block.tm3[i].fTama==_TOFF){
			//再発射可能なブロックの番号を保存する
			if(AttackTama2==B_Block.TTama){
				AttackTama2=i;
				B_Block.tm3[i].fTama |=_TSET;	//ブロックを再び発射セット
			}else if(!(fSPChr&_TAMA3ON)){
				break;	//画面弾存在フラグが立っていない場合は弾の移動処理をしない
			}
		}
	}
	
	//retの値がFALSEのままだった場合、画面に弾は存在していない
	if(!ret)	fSPChr &=_TAMA3OFF;		//画面弾存在フラグをオフ
	
	//敵キャラが表示されていない場合リターン
	if(!(fSPChr&_SPON))			return;
	//敵キャラの破壊フラグが立っている場合リターン
	else if(fSPChr&_SPDES)		return;
	
	//----------------------------------------------------------//
	//					 ブロックの発射処理						//
	//----------------------------------------------------------//
	
	//ブロックの発射処理
	if(!B_Block.WTamaCnt--){
		//指定された数以上画面に弾が存在しないようにする処理
		if(AttackTama2<B_Block.TTama){
			//画面上に弾が発射されているフラグオン
			fSPChr |=_TAMA3ON;
			
			//発射中のフラグを立てる
			B_Block.tm3[AttackTama2].fTama |=_TON;		
			//弾アニメーションを初期化
			B_Block.tm3[AttackTama2].AnimeCnt=0;		
			B_Block.tm3[AttackTama2].FrameCnt=B_Block.tm3[AttackTama2].AnimeFrame[0];	
			//初期座標
			B_Block.tm3[AttackTama2].Cdn.x=Parts3.Local.x+T_Ichi;
			B_Block.tm3[AttackTama2].Cdn.y=Parts3.Local.y;
			//弾の移動計算用に画面座標を取得
			B_Block.tm3[AttackTama2].dCdn.x=MainCdn.x+B_Block.tm3[AttackTama2].Cdn.x;
			B_Block.tm3[AttackTama2].dCdn.y=MainCdn.y+B_Block.tm3[AttackTama2].Cdn.y;
			//絶対座標をセット
			B_Block.tm3[AttackTama2].MapCdn.x=(LONG)B_Block.tm3[AttackTama2].dCdn.x;
			B_Block.tm3[AttackTama2].MapCdn.y=(LONG)B_Block.tm3[AttackTama2].dCdn.y;
			//移動量の初期化
			B_Block.tm3[AttackTama2].Mov.x=0;
			B_Block.tm3[AttackTama2].Mov.y=5;
			//耐久力のセット
			B_Block.tm3[AttackTama2].ShldCnt=B_Block.tm3[AttackTama2].Shld;
			//破壊シーンの初期化
			B_Block.tm3[AttackTama2].BakuAnimeCnt=0;		
			B_Block.tm3[AttackTama2].BakuFrameCnt=B_Block.tm3[AttackTama2].BakuAnimeFrame[0];
			
			//発射位置を進める
			T_Ichi -=69;	
			if(T_Ichi<374 && T_Ichi>202)	T_Ichi=138;
			else if(T_Ichi<0)				T_Ichi=512;
		
			//B_Block.TTamaで初期化
			//この処理はAttackTama2が上書きされない限り新しく弾を発射できないようにしています
			AttackTama2=B_Block.TTama;
		}
		B_Block.WTamaCnt=B_Block.WTama;	//発射までのカウンタを再セット
	}
}

//////////////////////////////////////////////////////////////////////
//	ボスキャラ01の描画準備を行なう処理
//	引数は状態を表わすフラグ
//////////////////////////////////////////////////////////////////////
void	BOSS01::FrameBoss01(DWORD Tflag)
{
	fSPChr=Tflag;
	short i;
	
	//----------------------------------------------------------//
	//					弾の描画準備							//
	//----------------------------------------------------------//

	//画面弾存在フラグが立っている場合のみ弾を描画
	if(fSPChr&_TAMA2ON){
		//ミサイル
		for(i=0;(DWORD)i<B_Msr.TTama;i++){
			//発射されていない弾のみ描画する
			if(B_Msr.tm2[i].fTama&_TON){ 
				//SPサーフェスからの転送矩形範囲をセット
				B_Msr.tm2[i].SrcR=TamSP[B_Msr.tm2[i].AnimePat[B_Msr.tm2[i].AnimeCnt]];
					
				B_Msr.tm2[i].DesR.left =B_Msr.tm2[i].Cdn.x;
				B_Msr.tm2[i].DesR.top  =B_Msr.tm2[i].Cdn.y;
				B_Msr.tm2[i].DesR.right=B_Msr.tm2[i].Cdn.x+B_Msr.tm2[i].Width;
				B_Msr.tm2[i].DesR.bottom=B_Msr.tm2[i].Cdn.y+B_Msr.tm2[i].Height;
				
				if(B_Msr.tm2[i].DesR.left<0){	
					B_Msr.tm2[i].SrcR.left=B_Msr.tm2[i].SrcR.left-B_Msr.tm2[i].DesR.left;
					B_Msr.tm2[i].DesR.left=0;
				}else if(B_Msr.tm2[i].DesR.right>WinXMax){
					B_Msr.tm2[i].SrcR.right=B_Msr.tm2[i].SrcR.right-(B_Msr.tm2[i].DesR.right-WinXMax);
					B_Msr.tm2[i].DesR.right=WinXMax;
				}
				if(B_Msr.tm2[i].DesR.top<0){
					B_Msr.tm2[i].SrcR.top=B_Msr.tm2[i].SrcR.top-B_Msr.tm2[i].DesR.top;
					B_Msr.tm2[i].DesR.top=0;
				}else if(B_Msr.tm2[i].DesR.bottom>WinYMax){
					B_Msr.tm2[i].SrcR.bottom=B_Msr.tm2[i].SrcR.bottom-(B_Msr.tm2[i].DesR.bottom-WinYMax);
					B_Msr.tm2[i].DesR.bottom=WinYMax;
				}
				
				//フレームを1減らして0になったらアニメパーターンを進める
				if(!(B_Msr.tm2[i].FrameCnt--)){
					//アニメパターンを進める
					B_Msr.tm2[i].AnimeCnt=(B_Msr.tm2[i].AnimeCnt+1)%B_Msr.tm2[i].Anime;
					//表示するアニメパターンのフレーム数セット
					B_Msr.tm2[i].FrameCnt=B_Msr.tm2[i].AnimeFrame[B_Msr.tm2[i].AnimeCnt];
				}
			}
		}	
	}

	if(fSPChr&_TAMA3ON){
		//ブロック
		for(i=0;(DWORD)i<B_Block.TTama;i++){
			//発射されていない弾のみ描画する
			if(B_Block.tm3[i].fTama&_TON){ 
				//SPサーフェスからの転送矩形範囲をセット
				//ダメージを受けているかどうかチェック
				if(B_Block.tm3[i].fTama&_THIT_ON){
					B_Block.tm3[i].SrcR=TamSP[11];	//ダメージシーン
					B_Block.tm3[i].fTama &=_THIT_OFF;
				}else{
					B_Block.tm3[i].SrcR=TamSP[B_Block.tm3[i].AnimePat[B_Block.tm3[i].AnimeCnt]];
				}
				
				B_Block.tm3[i].DesR.left =B_Block.tm3[i].Cdn.x;
				B_Block.tm3[i].DesR.top  =B_Block.tm3[i].Cdn.y;
				B_Block.tm3[i].DesR.right=B_Block.tm3[i].Cdn.x+B_Block.tm3[i].Width;
				B_Block.tm3[i].DesR.bottom=B_Block.tm3[i].Cdn.y+B_Block.tm3[i].Height;
				
				if(B_Block.tm3[i].DesR.left<0){	
					B_Block.tm3[i].SrcR.left=B_Block.tm3[i].SrcR.left-B_Block.tm3[i].DesR.left;
					B_Block.tm3[i].DesR.left=0;
				}else if(B_Block.tm3[i].DesR.right>WinXMax){
					B_Block.tm3[i].SrcR.right=B_Block.tm3[i].SrcR.right-(B_Block.tm3[i].DesR.right-WinXMax);
					B_Block.tm3[i].DesR.right=WinXMax;
				}
				if(B_Block.tm3[i].DesR.top<0){
					B_Block.tm3[i].SrcR.top=B_Block.tm3[i].SrcR.top-B_Block.tm3[i].DesR.top;
					B_Block.tm3[i].DesR.top=0;
				}else if(B_Block.tm3[i].DesR.bottom>WinYMax){
					B_Block.tm3[i].SrcR.bottom=B_Block.tm3[i].SrcR.bottom-(B_Block.tm3[i].DesR.bottom-WinYMax);
					B_Block.tm3[i].DesR.bottom=WinYMax;
				}
			/*	
				//フレームを1減らして0になったらアニメパーターンを進める
				if(!(B_Block.tm3[i].FrameCnt--)){
					//アニメパターンを進める
					B_Block.tm3[i].AnimeCnt=(B_Block.tm3[i].AnimeCnt+1)%B_Block.tm3[i].Anime;
					//表示するアニメパターンのフレーム数セット
					B_Block.tm3[i].FrameCnt=B_Block.tm3[i].AnimeFrame[B_Block.tm3[i].AnimeCnt];
				}
			*/
			 }else if(B_Block.tm3[i].fTama&_TBAKU_ON){
				//SPサーフェスからの転送矩形範囲をセット
				B_Block.tm3[i].SrcR=BakuSP[B_Block.tm3[i].BakuAnimePat[B_Block.tm3[i].BakuAnimeCnt]];
					
				B_Block.tm3[i].DesR.left =B_Block.tm3[i].Cdn.x;
				B_Block.tm3[i].DesR.top  =B_Block.tm3[i].Cdn.y;
				B_Block.tm3[i].DesR.right=B_Block.tm3[i].Cdn.x+B_Block.tm3[i].BakuWidth;
				B_Block.tm3[i].DesR.bottom=B_Block.tm3[i].Cdn.y+B_Block.tm3[i].BakuHeight;
				
				if(B_Block.tm3[i].DesR.left<0){	
					B_Block.tm3[i].SrcR.left=B_Block.tm3[i].SrcR.left-B_Block.tm3[i].DesR.left;
					B_Block.tm3[i].DesR.left=0;
				}else if(B_Block.tm3[i].DesR.right>WinXMax){
					B_Block.tm3[i].SrcR.right=B_Block.tm3[i].SrcR.right-(B_Block.tm3[i].DesR.right-WinXMax);
					B_Block.tm3[i].DesR.right=WinXMax;
				}
				if(B_Block.tm3[i].DesR.top<0){
					B_Block.tm3[i].SrcR.top=B_Block.tm3[i].SrcR.top-B_Block.tm3[i].DesR.top;
					B_Block.tm3[i].DesR.top=0;
				}else if(B_Block.tm3[i].DesR.bottom>WinYMax){
					B_Block.tm3[i].SrcR.bottom=B_Block.tm3[i].SrcR.bottom-(B_Block.tm3[i].DesR.bottom-WinYMax);
					B_Block.tm3[i].DesR.bottom=WinYMax;
				}
				
				//フレームを1減らして0になったらアニメパーターンを進める
				if(!(B_Block.tm3[i].BakuFrameCnt--)){
					//アニメーションシーンが1ループしたら終了
					if((B_Block.tm3[i].BakuAnimeCnt+1)==B_Block.tm3[i].BakuAnime){
						B_Block.tm3[i].fTama &=_TOFF;	//弾の表示をオフ
					}else{
						//アニメパターンを進める
						B_Block.tm3[i].BakuAnimeCnt=(B_Block.tm3[i].BakuAnimeCnt+1)%B_Block.tm3[i].BakuAnime;
						//表示するアニメパターンのフレーム数セット
						B_Block.tm3[i].BakuFrameCnt=B_Block.tm3[i].BakuAnimeFrame[B_Block.tm3[i].BakuAnimeCnt];
					}
				}
			}
		}
	}

	//----------------------------------------------------------//
	//					ボスキャラの描画準備					//
	//----------------------------------------------------------//
	
	//ボスキャラ表示フラグが立っている場合描画準備
	if(fSPChr&_SPON){
		//ボスキャラ破壊のアニメーション第2段階(最終段階)
		if(fSPChr&_SPDES_a)
		{
			for(i=0;i<200;i++){
				BossBaku2[i].SrcR=BakuSP[BossBaku2[i].AnimePat[BossBaku2[i].AnimeCnt]];	//転送元サーフェスの矩形範囲初期化
				BossBaku2[i].DesR.left=Cdn.x+BossBaku2[i].Local.x;//転送先サーフェスの矩形範囲初期化
				BossBaku2[i].DesR.top=Cdn.y+BossBaku2[i].Local.y;			
				BossBaku2[i].DesR.right=BossBaku2[i].DesR.left+BossBaku2[i].Width;
				BossBaku2[i].DesR.bottom=BossBaku2[i].DesR.top+BossBaku2[i].Height;
				if(BossBaku2[i].DesR.left<0){	
					BossBaku2[i].SrcR.left=BossBaku2[i].SrcR.left-BossBaku2[i].DesR.left;
					BossBaku2[i].DesR.left=0;
				}else if(BossBaku2[i].DesR.right>WinXMax){
					BossBaku2[i].SrcR.right=BossBaku2[i].SrcR.right-(BossBaku2[i].DesR.right-WinXMax);
					BossBaku2[i].DesR.right=WinXMax;
				}
				if(BossBaku2[i].DesR.top<0){
					BossBaku2[i].SrcR.top=BossBaku2[i].SrcR.top-BossBaku2[i].DesR.top;
					BossBaku2[i].DesR.top=0;
				}else if(BossBaku2[i].DesR.bottom>WinYMax){
					BossBaku2[i].SrcR.bottom=BossBaku2[i].SrcR.bottom-(BossBaku2[i].DesR.bottom-WinYMax);
					BossBaku2[i].DesR.bottom=WinYMax;
				}
				//フレームを1減らして0になったらアニメパーターンを進める
				if(!(BossBaku2[i].FrameCnt--)){
					//アニメーションシーンが1ループしたら終了
					if((BossBaku2[i].AnimeCnt+1)==BossBaku2[i].Anime){
						//爆発アニメーションデータを初期値に戻す
						BossBaku2[i].AnimeCnt=0;
						BossBaku2[i].FrameCnt=BossBaku2[i].AnimeFrame[0];
						fSPChr &=_SPDESOFF_a;
						fSPChr &=_SPOFF;		//表示フラグオフ
					}else{
						BossBaku2[i].AnimeCnt=(BossBaku2[i].AnimeCnt+1)%BossBaku2[i].Anime;	//アニメパターンを進める
						BossBaku2[i].FrameCnt=BossBaku2[i].AnimeFrame[BossBaku2[i].AnimeCnt];//表示するアニメパターンのフレーム数セット
					}
				}
			}
		}else{
		//通常時の描画
			//パーツ1
			Parts1.SrcR=SP[19];						//転送元サーフェスの矩形範囲初期化
			Parts1.DesR.left=Parts1.Local.x;		//転送先サーフェスの矩形範囲初期化
			Parts1.DesR.top=Parts1.Local.y;			
			Parts1.DesR.right=Parts1.Local.x+Parts1.Width;
			Parts1.DesR.bottom=Parts1.Local.y+Parts1.Height;
			if(Parts1.DesR.left<0){					//画面からはみ出す部分は転送しない処理
				Parts1.SrcR.left=Parts1.SrcR.left-Parts1.DesR.left;
				Parts1.DesR.left=0;
			}else if(Parts1.DesR.right>WinXMax){
				Parts1.SrcR.right=Parts1.SrcR.right-(Parts1.DesR.right-WinXMax);
				Parts1.DesR.right=WinXMax;
			}
			if(Parts1.DesR.top<0){
				Parts1.SrcR.top=Parts1.SrcR.top-Parts1.DesR.top;
				Parts1.DesR.top=0;
			}else if(Parts1.DesR.bottom>WinYMax){
				Parts1.SrcR.bottom=Parts1.SrcR.bottom-(Parts1.DesR.bottom-WinYMax);
				Parts1.DesR.bottom=WinYMax;
			}
			//パーツ2
			Parts2.SrcR=SP[20];
			Parts2.DesR.left=Parts2.Local.x;
			Parts2.DesR.top=Parts2.Local.y;			
			Parts2.DesR.right=Parts2.Local.x+Parts2.Width;
			Parts2.DesR.bottom=Parts2.Local.y+Parts2.Height;
			if(Parts2.DesR.left<0){	
				Parts2.SrcR.left=Parts2.SrcR.left-Parts2.DesR.left;
				Parts2.DesR.left=0;
			}else if(Parts2.DesR.right>WinXMax){
				Parts2.SrcR.right=Parts2.SrcR.right-(Parts2.DesR.right-WinXMax);
				Parts2.DesR.right=WinXMax;
			}
			if(Parts2.DesR.top<0){
				Parts2.SrcR.top=Parts2.SrcR.top-Parts2.DesR.top;
				Parts2.DesR.top=0;
			}else if(Parts2.DesR.bottom>WinYMax){
				Parts2.SrcR.bottom=Parts2.SrcR.bottom-(Parts2.DesR.bottom-WinYMax);
				Parts2.DesR.bottom=WinYMax;
			}
			//パーツ3
			Parts3.SrcR=SP[Parts3.AnimePat[Parts3.AnimeCnt]];
			Parts3.DesR.left=Parts3.Local.x;
			Parts3.DesR.top=Parts3.Local.y;			
			Parts3.DesR.right=Parts3.Local.x+Parts3.Width;
			Parts3.DesR.bottom=Parts3.Local.y+Parts3.Height;
			if(Parts3.DesR.left<0){	
				Parts3.SrcR.left=Parts3.SrcR.left-Parts3.DesR.left;
				Parts3.DesR.left=0;
			}else if(Parts3.DesR.right>WinXMax){
				Parts3.SrcR.right=Parts3.SrcR.right-(Parts3.DesR.right-WinXMax);
				Parts3.DesR.right=WinXMax;
			}
			if(Parts3.DesR.top<0){
				Parts3.SrcR.top=Parts3.SrcR.top-Parts3.DesR.top;
				Parts3.DesR.top=0;
			}else if(Parts3.DesR.bottom>WinYMax){
				Parts3.SrcR.bottom=Parts3.SrcR.bottom-(Parts3.DesR.bottom-WinYMax);
				Parts3.DesR.bottom=WinYMax;
			}
			//フレームを1減らして0になったらアニメパーターンを進める
			if(!(Parts3.FrameCnt--)){
				Parts3.AnimeCnt=(Parts3.AnimeCnt+1)%Parts3.Anime;	//アニメパターンを進める
				Parts3.FrameCnt=Parts3.AnimeFrame[Parts3.AnimeCnt];//表示するアニメパターンのフレーム数セット
			}
			//パーツ4
			Parts4.SrcR=SP[Parts4.AnimePat[Parts4.AnimeCnt]];
			Parts4.DesR.left=Parts4.Local.x;
			Parts4.DesR.top=Parts4.Local.y;			
			Parts4.DesR.right=Parts4.Local.x+Parts4.Width;
			Parts4.DesR.bottom=Parts4.Local.y+Parts4.Height;
			if(Parts4.DesR.left<0){	
				Parts4.SrcR.left=Parts4.SrcR.left-Parts4.DesR.left;
				Parts4.DesR.left=0;
			}else if(Parts4.DesR.right>WinXMax){
				Parts4.SrcR.right=Parts4.SrcR.right-(Parts4.DesR.right-WinXMax);
				Parts4.DesR.right=WinXMax;
			}
			if(Parts4.DesR.top<0){
				Parts4.SrcR.top=Parts4.SrcR.top-Parts4.DesR.top;
				Parts4.DesR.top=0;
			}else if(Parts4.DesR.bottom>WinYMax){
				Parts4.SrcR.bottom=Parts4.SrcR.bottom-(Parts4.DesR.bottom-WinYMax);
				Parts4.DesR.bottom=WinYMax;
			}
			if(!(Parts4.FrameCnt--)){
				Parts4.AnimeCnt=(Parts4.AnimeCnt+1)%Parts4.Anime;
				Parts4.FrameCnt=Parts4.AnimeFrame[Parts4.AnimeCnt];
			}
			//パーツ5
			Parts5.SrcR=SP[23];	
			Parts5.DesR.left=Parts5.Local.x;
			Parts5.DesR.top=Parts5.Local.y;			
			Parts5.DesR.right=Parts5.Local.x+Parts5.Width;
			Parts5.DesR.bottom=Parts5.Local.y+Parts5.Height;
			if(Parts5.DesR.left<0){	
				Parts5.SrcR.left=Parts5.SrcR.left-Parts5.DesR.left;
				Parts5.DesR.left=0;
			}else if(Parts5.DesR.right>WinXMax){
				Parts5.SrcR.right=Parts5.SrcR.right-(Parts5.DesR.right-WinXMax);
				Parts5.DesR.right=WinXMax;
			}
			if(Parts5.DesR.top<0){
				Parts5.SrcR.top=Parts5.SrcR.top-Parts5.DesR.top;
				Parts5.DesR.top=0;
			}else if(Parts5.DesR.bottom>WinYMax){
				Parts5.SrcR.bottom=Parts5.SrcR.bottom-(Parts5.DesR.bottom-WinYMax);
				Parts5.DesR.bottom=WinYMax;
			}
			//パーツ6
			if(fSPChr&_SPDES && !(fSPChr&_SPDES_a)){
				for(i=0;i<12;i++){
					BossBaku1[i].SrcR=BakuSP[BossBaku1[i].AnimePat[BossBaku1[i].AnimeCnt]];	//転送元サーフェスの矩形範囲初期化
					BossBaku1[i].DesR.left=Parts6.Local.x+BossBaku1[i].Local.x;				//転送先サーフェスの矩形範囲初期化
					BossBaku1[i].DesR.top=Parts6.Local.y+BossBaku1[i].Local.y;			
					BossBaku1[i].DesR.right=BossBaku1[i].DesR.left+BossBaku1[i].Width;
					BossBaku1[i].DesR.bottom=BossBaku1[i].DesR.top+BossBaku1[i].Height;
					if(BossBaku1[i].DesR.left<0){	
						BossBaku1[i].SrcR.left=BossBaku1[i].SrcR.left-BossBaku1[i].DesR.left;
						BossBaku1[i].DesR.left=0;
					}else if(BossBaku1[i].DesR.right>WinXMax){
						BossBaku1[i].SrcR.right=BossBaku1[i].SrcR.right-(BossBaku1[i].DesR.right-WinXMax);
						BossBaku1[i].DesR.right=WinXMax;
					}
					if(BossBaku1[i].DesR.top<0){
						BossBaku1[i].SrcR.top=BossBaku1[i].SrcR.top-BossBaku1[i].DesR.top;
						BossBaku1[i].DesR.top=0;
					}else if(BossBaku1[i].DesR.bottom>WinYMax){
						BossBaku1[i].SrcR.bottom=BossBaku1[i].SrcR.bottom-(BossBaku1[i].DesR.bottom-WinYMax);
						BossBaku1[i].DesR.bottom=WinYMax;
					}
					//フレームを1減らして0になったらアニメパーターンを進める
					if(!(BossBaku1[i].FrameCnt--)){
						//アニメーションシーンが1ループしたら終了
						if((BossBaku1[i].AnimeCnt+1)==BossBaku1[i].Anime){
							//爆発アニメーションデータを初期値に戻す
							BossBaku1[i].AnimeCnt=0;
							BossBaku1[i].FrameCnt=BossBaku1[i].AnimeFrame[0];
							fSPChr |=_SPDES_a;	//次の爆発シーンへ進む
							//ボスキャラ爆発音
							lpDSBuf[_BOSSBAKU]->Stop();
							lpDSBuf[_BOSSBAKU]->SetCurrentPosition(0);
							lpDSBuf[_BOSSBAKU]->Play(0,0,0);

						}else{
							BossBaku1[i].AnimeCnt=(BossBaku1[i].AnimeCnt+1)%BossBaku1[i].Anime;	//アニメパターンを進める
							BossBaku1[i].FrameCnt=BossBaku1[i].AnimeFrame[BossBaku1[i].AnimeCnt];//表示するアニメパターンのフレーム数セット
						}
					}
				}	
			}else{	
				//ダメージを受けているかどうかチェック
				if(fSPChr&_HITON){
					Parts6.SrcR=SP[28];	//ダメージシーン
					fSPChr &=_HITOFF;
				}else{
					Parts6.SrcR=SP[Parts6.AnimePat[Parts6.AnimeCnt]];
				}
				Parts6.DesR.left=Parts6.Local.x;
				Parts6.DesR.top=Parts6.Local.y;			
				Parts6.DesR.right=Parts6.Local.x+Parts6.Width;
				Parts6.DesR.bottom=Parts6.Local.y+Parts6.Height;
				if(Parts6.DesR.left<0){	
					Parts6.SrcR.left=Parts6.SrcR.left-Parts6.DesR.left;
					Parts6.DesR.left=0;
				}else if(Parts6.DesR.right>WinXMax){
					Parts6.SrcR.right=Parts6.SrcR.right-(Parts6.DesR.right-WinXMax);
					Parts6.DesR.right=WinXMax;
				}
				if(Parts6.DesR.top<0){
					Parts6.SrcR.top=Parts6.SrcR.top-Parts6.DesR.top;
					Parts6.DesR.top=0;
				}else if(Parts6.DesR.bottom>WinYMax){
					Parts6.SrcR.bottom=Parts6.SrcR.bottom-(Parts6.DesR.bottom-WinYMax);
					Parts6.DesR.bottom=WinYMax;
				}
				if(!(Parts6.FrameCnt--)){
					Parts6.AnimeCnt=(Parts6.AnimeCnt+1)%Parts6.Anime;
					Parts6.FrameCnt=Parts6.AnimeFrame[Parts6.AnimeCnt];
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	ボスキャラ01の描画
//////////////////////////////////////////////////////////////////////
void	BOSS01::RendBoss01()
{
	short i;
	
	//----------------------------------------------------------//
	//						弾の描画							//
	//----------------------------------------------------------//

	//画面弾存在フラグが立っていない場合は弾を描画しない
	if(fSPChr&_TAMA2ON){
		//ミサイル
		for(i=0;(DWORD)i<B_Msr.TTama;i++){
			//発射中でない弾は描画しない
			if(B_Msr.tm2[i].fTama&_TON){
				lpDDSBack->BltFast(	B_Msr.tm2[i].DesR.left,B_Msr.tm2[i].DesR.top,
									lpDDS_SP,&B_Msr.tm2[i].SrcR,
									DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			}
		}
	}

	if(fSPChr&_TAMA3ON){
		//ブロック
		for(i=0;(DWORD)i<B_Block.TTama;i++){
			//発射中でない弾は描画しない
			if(B_Block.tm3[i].fTama&_TON || B_Block.tm3[i].fTama&_TBAKU_ON){
				lpDDSBack->BltFast(	B_Block.tm3[i].DesR.left,B_Block.tm3[i].DesR.top,
									lpDDS_SP,&B_Block.tm3[i].SrcR,
									DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			}
		}
	}
	
	//----------------------------------------------------------//
	//					ボスキャラの描画						//
	//----------------------------------------------------------//
	
	//ボスキャラ表示フラグの立っていない場合はリターン
	if(!(fSPChr&_SPON))		return;
	
	//ボスキャラの描画
	//ボスキャラ破壊のアニメーション第2段階(最終段階)
	if(fSPChr&_SPDES_a){
		for(i=0;i<200;i++){
			lpDDSBack->BltFast(	BossBaku2[i].DesR.left,BossBaku2[i].DesR.top,
								lpDDS_SP,&BossBaku2[i].SrcR,
								DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		}
	}else{
		//パーツ1
		lpDDSBack->BltFast(	Parts1.DesR.left,Parts1.DesR.top,
							lpDDS_SP,&Parts1.SrcR,
							DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		//パーツ2
		lpDDSBack->BltFast(	Parts2.DesR.left,Parts2.DesR.top,
							lpDDS_SP,&Parts2.SrcR,
							DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		//パーツ3
		lpDDSBack->BltFast(	Parts3.DesR.left,Parts3.DesR.top,
							lpDDS_SP,&Parts3.SrcR,
							DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		//パーツ4
		lpDDSBack->BltFast(	Parts4.DesR.left,Parts4.DesR.top,
							lpDDS_SP,&Parts4.SrcR,
							DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		//パーツ5
		lpDDSBack->BltFast(	Parts5.DesR.left,Parts5.DesR.top,
							lpDDS_SP,&Parts5.SrcR,
							DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		//パーツ6
		if(fSPChr&_SPDES){
			//爆発シーン第一段階
			for(i=0;i<12;i++){
				lpDDSBack->BltFast(	BossBaku1[i].DesR.left,BossBaku1[i].DesR.top,
									lpDDS_SP,&BossBaku1[i].SrcR,
									DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			}
		}else{
			lpDDSBack->BltFast(	Parts6.DesR.left,Parts6.DesR.top,
								lpDDS_SP,&Parts6.SrcR,
								DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	ボスキャラ01の状態フラグを返す
//////////////////////////////////////////////////////////////////////
DWORD	BOSS01::GetBoss01fSPChr()
{
	return	fSPChr;
}

//////////////////////////////////////////////////////////////////////
//	ボスキャラ01の画面座標を返す(当たり判定のあるのはパーツ6の部分のみ)
//////////////////////////////////////////////////////////////////////
POINT	BOSS01::GetBoss01Cdn()
{
	return	Parts6.Local;
}

//////////////////////////////////////////////////////////////////////
//	ボスキャラ01の横幅を返す(当たり判定のあるのはパーツ6の部分のみ)
//////////////////////////////////////////////////////////////////////
short	BOSS01::GetBoss01Width()
{
	return	Parts6.Width;
}

//////////////////////////////////////////////////////////////////////
//	ボスキャラ01の縦幅を返す(当たり判定のあるのはパーツ6の部分のみ)
//////////////////////////////////////////////////////////////////////
short	BOSS01::GetBoss01Height()
{
	return	Parts6.Height;
}

//////////////////////////////////////////////////////////////////////
//	ボスキャラ01の残り耐久力の値のアドレスを返す
//////////////////////////////////////////////////////////////////////
short*	BOSS01::GetBoss01ShldCnt_P()
{
	return	&ShldCnt;
}

//////////////////////////////////////////////////////////////////////
//	弾の状態を管理するTAMAS2構造体へのポインタを返す
//////////////////////////////////////////////////////////////////////
TAMAS2*	BOSS01::GetBoss01B_Msr_P()
{
	return	&B_Msr;
}

//////////////////////////////////////////////////////////////////////
//	弾の状態を管理するTAMAS3構造体へのポインタを返す
//////////////////////////////////////////////////////////////////////
TAMAS3*	BOSS01::GetBoss01B_Block_P()
{
	return	&B_Block;
}