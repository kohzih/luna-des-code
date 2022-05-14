/*====================================================================
	敵キャラ01の処理に関するソースファイル
=====================================================================*/
#include	<windows.h>
#include	<ddraw.h>
#include	<time.h>

#include	"Macros.h"
#include	"Draw.h"	
#include	"Struct.h"
#include	"TEKI01.h"
#include	"DrawSPDataEXT.h"
#include	"DrawMacros.h"

extern	LPDIRECTDRAWSURFACE7	lpDDSBack;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TEKI01::TEKI01()
{
	TChr.AnimePat	=NULL;
	TChr.AnimeFrame	=NULL;
	TBaku.AnimePat	=NULL;
	TBaku.AnimeFrame=NULL;
	TMsr.tm			=NULL;
}

TEKI01::~TEKI01()
{	
	RELEASE_dels(TMsr.tm);
	RELEASE_dels(TBaku.AnimeFrame);
	RELEASE_dels(TBaku.AnimePat);
	RELEASE_dels(TChr.AnimeFrame);
	RELEASE_dels(TChr.AnimePat);
}

//////////////////////////////////////////////////////////////////////
//	敵キャラ01クラスの各メンバ変数の初期化
//	引数:敵キャラ01の配置されている絶対座標
//	戻り値:TRUE-初期化成功		FALSE-失敗
//////////////////////////////////////////////////////////////////////
BOOL	TEKI01::InitTeki01(POINT Tcdn)
{
	//再初期化に必要な処理
	RELEASE_dels(TMsr.tm);
	RELEASE_dels(TBaku.AnimeFrame);
	RELEASE_dels(TBaku.AnimePat);
	RELEASE_dels(TChr.AnimeFrame);
	RELEASE_dels(TChr.AnimePat);
	
	//敵キャラの状態フラグの初期化
	fSPChr=0;
	
	//乱数の初期化
	srand( (unsigned)time( NULL ) );

	//敵キャラの配置されている絶対座標を初期化
	MapCdn=Tcdn;			
	//敵キャラの配置されている画面座標を初期化
	Cdn.x=MapCdn.x-MainCdn.x;
	Cdn.y=MapCdn.y-MainCdn.y;
	//座標計算用の構造体を初期化
	dCdn.x=MapCdn.x;
	dCdn.y=MapCdn.y;
	
	//キャラの大きさの初期化
	Width=32;		
	Height=32;
	
	Shld=50;			//敵キャラ01の耐久力
	ShldCnt=Shld;		//敵キャラ01の残り耐久力のカウンタを初期化
	ATmt=20;			//攻撃態勢に入るまでのウェイト数
	ATmtCnt=ATmt;		//攻撃態勢に入るまでのウェイト数のカウンタを初期化
	ATt=40;				//攻撃を続ける時間
	ATtCnt=ATt;			//攻撃を続ける時間のカウンタを初期化
	
	//敵キャラ01の移動速度の初期化
	Mov.x=0.0;
	Mov.y=1.0;
	
	//敵キャラ01が現在表示している画面上に存在しているのかどうかチェック
	if(MainCdn.y>MapCdn.y+Height)			{fSPChr &=_SPOFF;}
	else if(MainCdn.y+WinYMax<MapCdn.y)		{fSPChr &=_SPOFF;}
	else if(MainCdn.x>MapCdn.x+Width)		{fSPChr &=_SPOFF;}
	else if(MainCdn.x+WinXMax<MapCdn.x)		{fSPChr &=_SPOFF;}
	else	fSPChr |=_SPON;		//表示フラグをオン
	
	//弾ステータス構造体に値をセットして弾数分確保した、弾オブジェクトを初期化
	TMsr.TTama=30;				//画面に存在できる弾の数
	TMsr.WTama=3;				//弾を発射する間隔
	TMsr.WTamaCnt=TMsr.WTama;	//最初の弾発射までのカウンタ
	
	TTamaNum=TMsr.TTama;		//次ぎに発射するべき弾番号を保存変数を初期化

	//TAMA構造体に前もって値をセットして弾データを初期化する
	TAMA	tama;
	char	*pTama;
	int		i;
	
	tama.fTama=_TOFF;		//弾の初期状態を設定
	tama.Width=8;			//弾の横幅
	tama.Height=8;			//弾の縦幅
	tama.Mov.x=0.0;			//弾の移動速度を0で初期化
	tama.Mov.y=0.0;
	tama.dCdn.x=0.0;		//弾の座標計算用の座標を0で初期化
	tama.dCdn.y=0.0;
	tama.Cdn.x=0;			//弾の現在の画面座標を0で初期化
	tama.Cdn.y=0;
	tama.MapCdn.x=0;		//弾の現在の絶対座標も0で初期化
	tama.MapCdn.y=0;
	tama.Anime=TamAme[0];	//弾アニメーションパターン数の初期化	
	memcpy(&tama.AnimePat[0],&TamPat[0][0],sizeof(BYTE)*10);//弾アニメーションパターン配列の初期化
	memcpy(&tama.AnimeFrame[0],&TamFrame[0][0],sizeof(BYTE)*10);//弾アニメーション切り替えタイミング配列の初期化
	tama.AnimeCnt=0;				//最初に表示するアニメーションパターンを初期化
	tama.FrameCnt=tama.AnimeFrame[0];	//最初のアニメーションフレームを初期化
	tama.SrcR.left=0;				//転送元矩形を0で初期化
	tama.SrcR.top=0;
	tama.SrcR.right=0;
	tama.SrcR.bottom=0;
	tama.DesR=tama.SrcR;			//転送先矩形も0で初期化
	
	//発射できる弾数分TAMA構造体を確保
	TMsr.tm=new TAMA[TMsr.TTama];
	if(!TMsr.tm)		return	FALSE;
			
	//確保した領域の先頭アドレスをコピー
	pTama=(char*)TMsr.tm;		
			
	//弾オブジェクトを確保した分、初期化 
	for(i=0;(DWORD)i<TMsr.TTama;i++){
		memcpy(pTama,&tama,sizeof(TAMA));	//確保した領域の先頭からコピー
		pTama +=sizeof(TAMA);				//次にコピーするところまでアドレスを進める
	}
			
	//敵キャラ01のアニメーションパターン数の初期化
	TChr.Anime=Ame[0];	
	//アニメーションパターン配列の初期化
	TChr.AnimePat=new BYTE[TChr.Anime];
	if(!TChr.AnimePat)	return	FALSE;
	memcpy(TChr.AnimePat,&Pat[0][0],sizeof(BYTE)*TChr.Anime);
	//アニメーション切り替えタイミング配列の初期化
	TChr.AnimeFrame=new BYTE[TChr.Anime];
	if(!TChr.AnimeFrame)	return	FALSE;
	memcpy(TChr.AnimeFrame,&Frame[0][0],sizeof(BYTE)*TChr.Anime);
	//最初に表示するアニメーションパターンを初期化
	TChr.AnimeCnt=0;
	//最初のアニメーションフレームを初期化
	TChr.FrameCnt=TChr.AnimeFrame[0];

	//爆発アニメーションの初期化
	TBaku.Anime=BakuAme[0];
	//アニメーションパターン配列の初期化
	TBaku.AnimePat=new BYTE[TBaku.Anime];
	if(!TBaku.AnimePat)	return	FALSE;
	memcpy(TBaku.AnimePat,&BakuPat[0][0],sizeof(BYTE)*TBaku.Anime);
	//アニメーション切り替えタイミング配列の初期化
	TBaku.AnimeFrame=new BYTE[TBaku.Anime];
	if(!TBaku.AnimeFrame)	return	FALSE;
	memcpy(TBaku.AnimeFrame,&BakuFrame[0][0],sizeof(BYTE)*TBaku.Anime);
	//最初に表示するアニメーションパターンを初期化
	TBaku.AnimeCnt=0;
	//最初のアニメーションフレームを初期化
	TBaku.FrameCnt=TBaku.AnimeFrame[0];

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////
//	敵キャラ01の移動処理
//////////////////////////////////////////////////////////////////////
void	TEKI01::MovTeki01()
{
	//破壊フラグの立っている場合はリターン
	if((fSPChr&_SPDESCHK)==(DWORD)_SPDES)	return;
	
	//敵キャラが現在の表示画面範囲に入っているかチェック
	if(MainCdn.y>MapCdn.y+Height)			fSPChr &=_SPOFF;
	else if(MainCdn.y+WinYMax<MapCdn.y)		fSPChr &=_SPOFF;
	else if(MainCdn.x>MapCdn.x+Width)		fSPChr &=_SPOFF;
	else if(MainCdn.x+WinXMax>MapCdn.x){												
		//表示フラグオン
		fSPChr |=_SPON;
		//移動量を加える(破壊された場合は移動量を加えない)	
		if(!(fSPChr&_SPDES)){
			dCdn.x +=Mov.x;
			dCdn.y +=Mov.y;
			MapCdn.x=(LONG)dCdn.x;
			MapCdn.y=(LONG)dCdn.y;
		}
		Cdn.x=MapCdn.x-MainCdn.x;
		Cdn.y=MapCdn.y-MainCdn.y;	
	}
}

//////////////////////////////////////////////////////////////////////
//	敵キャラ01の攻撃処理
//////////////////////////////////////////////////////////////////////
void	TEKI01::AttackTeki01()
{
	BOOL	ret=FALSE;

	//----------------------------------------------------------//
	//					    弾の移動処理						//
	//----------------------------------------------------------//
	
	//既に発射されている弾の座標を進める
	for(int i=0;(DWORD)i<TMsr.TTama;i++){
		//既に発射されている弾のみ処理する
		if(TMsr.tm[i].fTama&_TON)
		{ 
			//一度でも弾移動処理が行われた場合retにTRUEを代入
			ret=TRUE;
			
			//弾を移動させる
			TMsr.tm[i].dCdn.x +=TMsr.tm[i].Mov.x;
			TMsr.tm[i].dCdn.y +=TMsr.tm[i].Mov.y;				//弾の絶対座標を進める
			TMsr.tm[i].MapCdn.x=(LONG)TMsr.tm[i].dCdn.x;
			TMsr.tm[i].MapCdn.y=(LONG)TMsr.tm[i].dCdn.y;
			//弾の画面座標を求める
			TMsr.tm[i].Cdn.x=TMsr.tm[i].MapCdn.x-MainCdn.x;
			TMsr.tm[i].Cdn.y=TMsr.tm[i].MapCdn.y-MainCdn.y;
				
			//弾が画面外に移動してないかチェック
			if(	TMsr.tm[i].MapCdn.x+TMsr.tm[i].Width<MainCdn.x	||
				TMsr.tm[i].MapCdn.x>MainCdn.x+WinXMax			||
				TMsr.tm[i].MapCdn.y+TMsr.tm[i].Height<MainCdn.y	||
				TMsr.tm[i].MapCdn.y>MainCdn.y+WinYMax)
			{
				TMsr.tm[i].fTama=_TOFF;		//弾フラグオフ
				continue;
			}
		}else if(TMsr.tm[i].fTama==_TOFF){
			if(TTamaNum==TMsr.TTama){
				TTamaNum=i;
				TMsr.tm[i].fTama |=_TSET;	//再び発射セット
			}else if(!(fSPChr&_TAMA1ON)){
				break;	//画面弾存在フラグが立っていない場合は弾の移動処理をしない
			}
		}
	}
	
	//retの値がFALSEのままだった場合、画面に弾は存在していない
	if(!ret)	fSPChr &=_TAMA1OFF;		//画面弾存在フラグをオフ

	
	//----------------------------------------------------------//
	//					    弾の発射処理						//
	//----------------------------------------------------------//
	
	//敵キャラが表示されていない場合リターン
	if(!(fSPChr&_SPON))			return;
	//敵キャラの破壊フラグが立っている場合リターン
	else if(fSPChr&_SPDES)		return;
	
	//敵キャラが攻撃態勢に入っているかチェック
	if(fSPChr&_ATCKON){	
		if(!(TMsr.WTamaCnt--)){
			//現在発射可能な弾があるかチェック
			if(TTamaNum<TMsr.TTama){
				fSPChr |=_TAMA1ON;				//弾発射フラグオン
				TMsr.tm[TTamaNum].fTama |=_TON;//発射した弾の発射中フラグを立てる	
				//弾アニメーションを初期化
				TMsr.tm[TTamaNum].AnimeCnt=0;		
				TMsr.tm[TTamaNum].FrameCnt=TMsr.tm[TTamaNum].AnimeFrame[0];
				//弾の発射された絶対座標を初期化
				TMsr.tm[TTamaNum].MapCdn.x=MapCdn.x+4;
				TMsr.tm[TTamaNum].MapCdn.y=MapCdn.y+16;
				//弾の移動計算用に絶対座標を取得
				TMsr.tm[TTamaNum].dCdn.x=TMsr.tm[TTamaNum].MapCdn.x;
				TMsr.tm[TTamaNum].dCdn.y=TMsr.tm[TTamaNum].MapCdn.y;
				//弾の画面座標を初期化
				TMsr.tm[TTamaNum].Cdn.x=TMsr.tm[TTamaNum].MapCdn.x-MainCdn.x;
				TMsr.tm[TTamaNum].Cdn.y=TMsr.tm[TTamaNum].MapCdn.y-MainCdn.y;
				//弾の移動速度を初期化
				TMsr.tm[TTamaNum].Mov.x=rand()%2 ? rand()%5+1:-(rand()%5+1);
				TMsr.tm[TTamaNum].Mov.y=rand()%2 ? rand()%2+1:-(rand()%2+1);
			
				//再び発射可能な弾番号を保存するために初期化
				TTamaNum=TMsr.TTama;
			}
			TMsr.WTamaCnt=TMsr.WTama;	//発射までのカウントを再びセット
		}

		//----------------------------------------------------------//
		//	敵キャラの攻撃態勢に入るまでのカウントを行なう処理		//
		//----------------------------------------------------------//
	
		if(!(ATtCnt--)){			//攻撃時間を減らしていく
			fSPChr &=_ATCKOFF;		//攻撃態勢フラグオフ
			ATmtCnt=ATmt;			//それぞれのカウンタを再びセット
			ATtCnt=ATt;
			TMsr.WTamaCnt=TMsr.WTama;
		}
	}else{	
		//攻撃態勢に入るまでのカウンタを減らす	
		if(!(ATmtCnt--))	fSPChr |=_ATCKON;	
	}
}

//////////////////////////////////////////////////////////////////////
//	敵キャラ01の描画準備処理
//	引数:敵キャラ01の状態フラグ
//////////////////////////////////////////////////////////////////////
void	TEKI01::FrameTeki01(DWORD Tflag)
{
	fSPChr=Tflag;
	
	//----------------------------------------------------------//
	//					弾の描画準備							//
	//----------------------------------------------------------//

	//画面弾存在フラグが立っている場合のみ弾を描画
	if(fSPChr&_TAMA1ON){
		for(int i=0;(DWORD)i<TMsr.TTama;i++){
			//発射されていない弾のみ描画する
			if(TMsr.tm[i].fTama&_TON){ 
				//SPサーフェスからの転送矩形範囲をセット
				TMsr.tm[i].SrcR=TamSP[TMsr.tm[i].AnimePat[TMsr.tm[i].AnimeCnt]];
					
				TMsr.tm[i].DesR.left =TMsr.tm[i].Cdn.x;
				TMsr.tm[i].DesR.top  =TMsr.tm[i].Cdn.y;
				TMsr.tm[i].DesR.right=TMsr.tm[i].Cdn.x+TMsr.tm[i].Width;
				TMsr.tm[i].DesR.bottom=TMsr.tm[i].Cdn.y+TMsr.tm[i].Height;
				
				if(TMsr.tm[i].DesR.left<0){	
					TMsr.tm[i].SrcR.left=TMsr.tm[i].SrcR.left-TMsr.tm[i].DesR.left;
					TMsr.tm[i].DesR.left=0;
				}else if(TMsr.tm[i].DesR.right>WinXMax){
					TMsr.tm[i].SrcR.right=TMsr.tm[i].SrcR.right-(TMsr.tm[i].DesR.right-WinXMax);
					TMsr.tm[i].DesR.right=WinXMax;
				}
				if(TMsr.tm[i].DesR.top<0){
					TMsr.tm[i].SrcR.top=TMsr.tm[i].SrcR.top-TMsr.tm[i].DesR.top;
					TMsr.tm[i].DesR.top=0;
				}else if(TMsr.tm[i].DesR.bottom>WinYMax){
					TMsr.tm[i].SrcR.bottom=TMsr.tm[i].SrcR.bottom-(TMsr.tm[i].DesR.bottom-WinYMax);
					TMsr.tm[i].DesR.bottom=WinYMax;
				}
			
				//フレームを1減らして0になったらアニメパーターンを進める
				if(!(TMsr.tm[i].FrameCnt--)){
					//アニメパターンを進める
					TMsr.tm[i].AnimeCnt=(TMsr.tm[i].AnimeCnt+1)%TMsr.tm[i].Anime;
					//表示するアニメパターンのフレーム数セット
					TMsr.tm[i].FrameCnt=TMsr.tm[i].AnimeFrame[TMsr.tm[i].AnimeCnt];
				}
			}
		}	
	}		
					
	//----------------------------------------------------------//
	//					敵キャラの描画準備						//
	//----------------------------------------------------------//
	
	//敵キャラ表示フラグが立っている場合描画準備
	if(fSPChr&_SPON){
		//敵キャラ破壊のアニメーション
		if(fSPChr&_SPDES)
		{
			//SPサーフェスからの転送矩形範囲をセット
			TBaku.SrcR=BakuSP[TBaku.AnimePat[TBaku.AnimeCnt]];
		
			TBaku.DesR.left  =Cdn.x;
			TBaku.DesR.top   =Cdn.y;
			TBaku.DesR.right =Cdn.x+Width;
			TBaku.DesR.bottom=Cdn.y+Height;
				
			//画面からはみ出す部分は転送しない処理
			if(TBaku.DesR.left<0){	
				TBaku.SrcR.left=TBaku.SrcR.left-TBaku.DesR.left;
				TBaku.DesR.left=0;
			}else if(TBaku.DesR.right>WinXMax){
				TBaku.SrcR.right=TBaku.SrcR.right-(TBaku.DesR.right-WinXMax);
				TBaku.DesR.right=WinXMax;
			}
			if(TBaku.DesR.top<0){
				TBaku.SrcR.top=TBaku.SrcR.top-TBaku.DesR.top;
				TBaku.DesR.top=0;
			}else if(Cdn.y+Height>WinYMax){
				TBaku.SrcR.bottom=TBaku.SrcR.bottom-(TBaku.DesR.bottom-WinYMax);
				TBaku.DesR.bottom=WinYMax;
			}
			
			//フレームを1減らして0になったらアニメパーターンを進める
			if(!(TBaku.FrameCnt--)){
				//アニメーションシーンが1ループしたら終了
				if((TBaku.AnimeCnt+1)==TBaku.Anime){
					//爆発アニメーションデータを初期値に戻す
					TBaku.AnimeCnt=0;
					TBaku.FrameCnt=TBaku.AnimeFrame[0];
					fSPChr &=_SPOFF;	//スプライトの表示をオフ
				}else{
					TBaku.AnimeCnt=(TBaku.AnimeCnt+1)%TBaku.Anime;	//アニメパターンを進める
					TBaku.FrameCnt=TBaku.AnimeFrame[TBaku.AnimeCnt];//表示するアニメパターンのフレーム数セット
				}
			}
		}else{
		//通常時の描画
			//ダメージを受けているかどうかチェック
			if(fSPChr&_HITON){
				TChr.SrcR=SP[TChr.AnimePat[TChr.AnimeCnt]+8];	//ダメージシーン
				fSPChr &=_HITOFF;
			}else{
				TChr.SrcR=SP[TChr.AnimePat[TChr.AnimeCnt]];		//通常シーン
			}
		
			TChr.DesR.left  =Cdn.x;
			TChr.DesR.top   =Cdn.y;
			TChr.DesR.right =Cdn.x+Width;
			TChr.DesR.bottom=Cdn.y+Height;

			//画面からはみ出す部分は転送しない処理
			if(TChr.DesR.left<0){	
				TChr.SrcR.left=TChr.SrcR.left-TChr.DesR.left;
				TChr.DesR.left=0;
			}else if(TChr.DesR.right>WinXMax){
				TChr.SrcR.right=TChr.SrcR.right-(TChr.DesR.right-WinXMax);
				TChr.DesR.right=WinXMax;
			}
			if(TChr.DesR.top<0){
				TChr.SrcR.top=TChr.SrcR.top-TChr.DesR.top;
				TChr.DesR.top=0;
			}else if(TChr.DesR.bottom>WinYMax){
				TChr.SrcR.bottom=TChr.SrcR.bottom-(TChr.DesR.bottom-WinYMax);
				TChr.DesR.bottom=WinYMax;
			}
		
			//フレームを1減らして0になったらアニメパーターンを進める
			if(!(TChr.FrameCnt--)){
				TChr.AnimeCnt=(TChr.AnimeCnt+1)%TChr.Anime;	//アニメパターンを進める
				TChr.FrameCnt=TChr.AnimeFrame[TChr.AnimeCnt];//表示するアニメパターンのフレーム数セット
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	敵キャラ01の描画
//////////////////////////////////////////////////////////////////////
void	TEKI01::RendTeki01()
{
	//----------------------------------------------------------//
	//						弾の描画							//
	//----------------------------------------------------------//

	//画面弾存在フラグが立っていない場合は弾を描画しない
	if(fSPChr&_TAMA1ON){
		for(int i=0;(DWORD)i<TMsr.TTama;i++){
			//発射中でない弾は描画しない
			if(TMsr.tm[i].fTama&_TON){
				lpDDSBack->BltFast(	TMsr.tm[i].DesR.left,TMsr.tm[i].DesR.top,
									lpDDS_SP,&TMsr.tm[i].SrcR,
									DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			}
		}
	}
	
	//----------------------------------------------------------//
	//					敵キャラの描画							//
	//----------------------------------------------------------//
	
	//敵キャラ表示フラグの立っていない場合はリターン
	if(!(fSPChr&_SPON))		return;
	
	//敵キャラの描画
	//破壊フラグの立っていた場合の転送
	if(fSPChr&_SPDES){
		lpDDSBack->BltFast(	TBaku.DesR.left,TBaku.DesR.top,
							lpDDS_SP,&TBaku.SrcR,
							DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	
	//通常の転送
	}else if(fSPChr&_SPON){
		lpDDSBack->BltFast(	TChr.DesR.left,TChr.DesR.top,
							lpDDS_SP,&TChr.SrcR,
							DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	}
}

//////////////////////////////////////////////////////////////////////
//	敵キャラ状態フラグを返す
//////////////////////////////////////////////////////////////////////
DWORD	TEKI01::GetTeki01fSPChr()
{
	return	fSPChr;
}
//////////////////////////////////////////////////////////////////////
//	敵キャラの画面座標を返す
//////////////////////////////////////////////////////////////////////
POINT	TEKI01::GetTeki01Cdn()
{
	return	Cdn;
}
//////////////////////////////////////////////////////////////////////
//	敵キャラの横幅を返す
//////////////////////////////////////////////////////////////////////
short	TEKI01::GetTeki01Width()
{
	return	Width;
}
//////////////////////////////////////////////////////////////////////
//	敵キャラの縦幅を返す
//////////////////////////////////////////////////////////////////////
short	TEKI01::GetTeki01Height()
{
	return	Height;
}
//////////////////////////////////////////////////////////////////////
//	敵キャラの残り耐久力の値へのポインタを返す
//////////////////////////////////////////////////////////////////////
short*	TEKI01::GetTeki01ShldCnt_P()
{
	return	&ShldCnt;
}
//////////////////////////////////////////////////////////////////////
//	敵キャラの弾の状態を管理するTAMAS構造体へのポインタを返す
//////////////////////////////////////////////////////////////////////
TAMAS*	TEKI01::GetTeki01TMsr_P()
{
	return	&TMsr;
}