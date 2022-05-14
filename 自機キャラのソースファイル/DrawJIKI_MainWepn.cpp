/*=============================================================================
	主砲の発射、当たり判定、描画等に関するソースファイル
==============================================================================*/
#include	<windows.h>
#include	<dsound.h>
#include	<ddraw.h>

#include	"Macros.h"
#include	"Draw.h"	
#include	"Struct.h"
#include	"DrawJIKI.h"
#include	"DrawJIKIDataEXT.h"
#include	"DrawMacros.h"

//ダイレクトサウンドバッファ再生用マクロ定義
#define	_JIKIBAKU	0
#define	_TEKIBAKU	1
#define	_FUELARM	2
#define	_FIRE		3
#define	_TAMAKYU	4
#define	_BOSSBAKU	5

extern	LPDIRECTSOUNDBUFFER		lpDSBuf[6]; //ダイレクトサウンドバッファ
extern	LPDIRECTDRAWSURFACE7	lpDDSBack;		//バックサーフェス

//////////////////////////////////////////////////////////////////////
//	主砲から発射される弾オブジェクトの初期化
//////////////////////////////////////////////////////////////////////
BOOL	DrawJIKI::InitMainWepn()
{	
	//////////////////////////////////////////////////////////////////////
	//------レーザー１のステータスを管理する構造体の初期化---------------
	
	//弾ステータス構造体に値をセットして弾数分確保した、弾オブジェクトを初期化
	LSR1.TTama	=6;				//画面上に存在できる弾の数
	LSR1.WTama	=10;			//弾を発射する間隔
	LSR1.WTamaCnt=LSR1.WTama;	//弾発射までのカウンタ

	TAMA	tama;				//TAMA構造体に前もって値をセットして初期化する

	tama.fTama=_TOFF;			//弾の初期状態を設定
	tama.Width=4;				//弾の横幅
	tama.Height=72;				//弾の縦幅
	tama.Mov.x=0.0;				//弾の移動速度を初期化
	tama.Mov.y=20.0;	
	tama.dCdn.x=0;				//弾の画面座標計算用
	tama.dCdn.y=0;
	tama.Cdn.x=0;				//弾の現在の画面座標を0で初期化
	tama.Cdn.y=0;
	tama.MapCdn.x=0;			//弾の現在の絶対座標も0で初期化
	tama.MapCdn.y=0;
	tama.Anime=TamAmeJIKI[0];	//弾アニメーションパターン数の初期化	
	//弾アニメーションパターン配列の初期化
	memcpy(&tama.AnimePat[0],&TamPatJIKI[0][0],sizeof(BYTE)*10);
	//弾アニメーション切り替えタイミング配列の初期化
	memcpy(&tama.AnimeFrame[0],&TamFrameJIKI[0][0],sizeof(BYTE)*10);
	tama.AnimeCnt=0;					//最初に表示するアニメーションパターンを初期化
	tama.FrameCnt=tama.AnimeFrame[0];	//最初のアニメーションフレームカウントを初期化	
	tama.SrcR.left=0;			//転送元矩形を0で初期化
	tama.SrcR.top=0;
	tama.SrcR.right=0;
	tama.SrcR.bottom=0;
	tama.DesR=tama.SrcR;		//転送先矩形も0で初期化
	
	//発射できる弾数分TAMA構造体を確保
	LSR1.tm=new TAMA[LSR1.TTama];
	if(!LSR1.tm)		return	FALSE;
	//確保した領域の先頭アドレスをコピー
	char *pTama;
	pTama=(char*)LSR1.tm;		
	//弾オブジェクトを確保した分、初期化 
	for(int i=0;(DWORD)i<LSR1.TTama;i++){
		memcpy(pTama,&tama,sizeof(TAMA));	//確保した領域の先頭からコピー
		pTama +=sizeof(TAMA);				//次にコピーするところまでアドレスを進める
	}

	//////////////////////////////////////////////////////////////////////	
	//-----ミサイル1のステータスを管理する構造体の初期化------------------

	MSR1.TTama	=60;			//画面上に存在できる弾の数
	MSR1.WTama	=8;				//弾を発射する間隔
	MSR1.WTamaCnt=MSR1.WTama;	//弾発射までのカウンタ

	tama.fTama=_TOFF;			//弾の初期状態を設定
	tama.Width=16;				//弾の横幅
	tama.Height=32;				//弾の縦幅
	tama.Mov.x=0.0;				//弾の移動速度を初期化
	tama.Mov.y=0.0;	
	tama.dCdn.x=0;				//弾の画面座標計算用
	tama.dCdn.y=0;
	tama.Cdn.x=0;				//弾の現在の画面座標を0で初期化
	tama.Cdn.y=0;
	tama.MapCdn.x=0;			//弾の現在の絶対座標も0で初期化
	tama.MapCdn.y=0;
	tama.Anime=TamAmeJIKI[1];	//弾アニメーションパターン数の初期化
	memcpy(&tama.AnimePat[0],&TamPatJIKI[1][0],sizeof(BYTE)*10);	//弾アニメーションパターン配列の初期化
	memcpy(&tama.AnimeFrame[0],&TamFrameJIKI[1][0],sizeof(BYTE)*10);//弾アニメーション切り替えタイミング配列の初期化
	tama.AnimeCnt=0;					//最初に表示するアニメーションパターンを初期化
	tama.FrameCnt=tama.AnimeFrame[0];	//最初のアニメーションフレームカウントを初期化	
	tama.SrcR.left=0;					//転送元矩形を0で初期化
	tama.SrcR.top=0;
	tama.SrcR.right=0;
	tama.SrcR.bottom=0;
	tama.DesR=tama.SrcR;				//転送先矩形も0で初期化
	
	//発射できる弾数分TAMA構造体を確保
	MSR1.tm=new TAMA[MSR1.TTama];
	if(!MSR1.tm)		return	FALSE;
	//確保した領域の先頭アドレスをコピー
	pTama=(char*)MSR1.tm;		
	//弾オブジェクトを確保した分、初期化 
	for(i=0;(DWORD)i<MSR1.TTama;i++){
		memcpy(pTama,&tama,sizeof(TAMA));	//確保した領域の先頭からコピー
		pTama +=sizeof(TAMA);				//次にコピーするところまでアドレスを進める
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////
//	動的に確保するメモリ領域へのポインタをNULLで初期化
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::InitWepnTm()
{
	LSR1.tm=NULL;
	MSR1.tm=NULL;
}

//////////////////////////////////////////////////////////////////////
//	確保したメモリ領域を解放
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::MainWepnRelease()
{	
	RELEASE_dels(MSR1.tm);
	RELEASE_dels(LSR1.tm);
}

//////////////////////////////////////////////////////////////////////
//	主砲発射までのカウントを進める
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::MainWepnCnt()	
{
	//現在使用している主砲をチェックしてフラグの立っている主砲の処理をする
	switch(Wpd.flag&_MAINWP_CHK){
		case _LSR_ON:
			switch (Wpd.flag&_LSR_LVCHK){
				case _LSR1_ON:
					LSR1.WTamaCnt--;	//発射までのカウントダウン
					break;
				default:
					break;
			}
			break;
		case _MSR_ON:
			switch (Wpd.flag&_MSR_LVCHK){
				case _MSR1_ON:
					MSR1.WTamaCnt--;	//発射までのカウントダウン
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////
//	主砲発射までのカウンタをリセットさせる
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::MainWepnCntReset()	
{
	//レーザーの発射カウントをリセット
	switch (Wpd.flag&_LSR_LVCHK){
		case _LSR1_ON:
			LSR1.WTamaCnt=LSR1.WTama;	//発射までのカウンタを再セット
			break;
		default:
			break;
	}
	
	//ミサイルの発射カウントをリセット
	switch (Wpd.flag&_MSR_LVCHK){
		case _MSR1_ON:
			MSR1.WTamaCnt=MSR1.WTama;	//発射までのカウンタを再セット
			break;
		default:
			break;		
	}
}

//////////////////////////////////////////////////////////////////////
//	主砲発射の処理と移動処理をします
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::MainWepn()	
{
	short	i,j;
	
	//画面に弾が存在しているかしていないか調べるフラグ
	bool	TamaS=FALSE;	
	
	//発射できる状態になっているレーザーの番号保存用
	static	DWORD	TTamaNum1=LSR1.TTama,TTamaNum2=LSR1.TTama;

	//レーザーの発射と移動
	switch(Wpd.flag&_LSR_LVCHK){	//装備のレベルを調べている
		case _LSR1_ON:
			//-----------------レーザーの発射処理-------------------//
			
			if(!LSR1.WTamaCnt){
				//レーザーが指定された数以上に画面に存在しないようにする処理
				if(TTamaNum1<LSR1.TTama && TTamaNum2<LSR1.TTama){
					//画面上に弾が発射されているフラグオン
					fJIKI |=_WEPN_ON;
					//発射中のフラグを立てる
					LSR1.tm[TTamaNum1].fTama |=_TON;
					LSR1.tm[TTamaNum2].fTama |=_TON;
					//弾アニメーションを初期化	
					LSR1.tm[TTamaNum1].AnimeCnt=0;
					LSR1.tm[TTamaNum1].FrameCnt=LSR1.tm[TTamaNum1].AnimeFrame[0];	
					LSR1.tm[TTamaNum2].AnimeCnt=0;
					LSR1.tm[TTamaNum2].FrameCnt=LSR1.tm[TTamaNum2].AnimeFrame[0];	
					//レーザーの発射された座標を初期化
					//画面座標をセット
					LSR1.tm[TTamaNum1].Cdn.x=Cdn.x-2;
					LSR1.tm[TTamaNum1].Cdn.y=Cdn.y-(LSR1.tm[TTamaNum1].Height-32);
					LSR1.tm[TTamaNum2].Cdn.x=Cdn.x+(_JIKI_W-2);
					LSR1.tm[TTamaNum2].Cdn.y=Cdn.y-(LSR1.tm[TTamaNum2].Height-32);
					//弾の移動計算用に画面座標を取得
					LSR1.tm[TTamaNum1].dCdn.x=LSR1.tm[TTamaNum1].Cdn.x;
					LSR1.tm[TTamaNum1].dCdn.y=LSR1.tm[TTamaNum1].Cdn.y;
					LSR1.tm[TTamaNum2].dCdn.x=LSR1.tm[TTamaNum2].Cdn.x;
					LSR1.tm[TTamaNum2].dCdn.y=LSR1.tm[TTamaNum2].Cdn.y;
					//絶対座標をセット
					LSR1.tm[TTamaNum1].MapCdn.x=MainCdn.x+LSR1.tm[TTamaNum1].Cdn.x;
					LSR1.tm[TTamaNum1].MapCdn.y=MainCdn.y+LSR1.tm[TTamaNum1].Cdn.y;
					LSR1.tm[TTamaNum2].MapCdn.x=MainCdn.x+LSR1.tm[TTamaNum2].Cdn.x;
					LSR1.tm[TTamaNum2].MapCdn.y=MainCdn.y+LSR1.tm[TTamaNum2].Cdn.y;
				
					TTamaNum1=LSR1.TTama;			//LSR1.TTamaで初期化
					TTamaNum2=LSR1.TTama;			//この処理はTTamaNumが上書きされない限り
													//新しく弾を発射できないようにしています
					//発射音
					lpDSBuf[_FIRE]->Stop();
					lpDSBuf[_FIRE]->SetCurrentPosition(0);
					lpDSBuf[_FIRE]->Play(0,0,0);
				}
				LSR1.WTamaCnt=LSR1.WTama;	//発射までのカウンタを再セット
			}
			
			//--------------------レーザーの移動処理-------------------//

			for(i=0;(DWORD)i<LSR1.TTama;i++){	
				//発射中フラグの立っていない弾は処理しない
				if(LSR1.tm[i].fTama&_TON)
				{
					TamaS=TRUE;	//レーザーは画面内に存在している
					
					//レーザーを移動させる
					LSR1.tm[i].dCdn.x +=LSR1.tm[i].Mov.x;
					LSR1.tm[i].dCdn.y -=LSR1.tm[i].Mov.y;
					LSR1.tm[i].Cdn.x=(LONG)LSR1.tm[i].dCdn.x;
					LSR1.tm[i].Cdn.y=(LONG)LSR1.tm[i].dCdn.y;
					LSR1.tm[i].MapCdn.x=MainCdn.x+LSR1.tm[i].Cdn.x;
					LSR1.tm[i].MapCdn.y=MainCdn.y+LSR1.tm[i].Cdn.y;	
					//レーザーが画面の外に移動していないかチェック
					if(LSR1.tm[i].Cdn.y+LSR1.tm[i].Height<0){
						LSR1.tm[i].fTama &=_TOFF;	//フラグをクリア
						continue;
					}
				
					//SPサーフェスからの転送矩形範囲をセット
					LSR1.tm[i].SrcR=TamSPJIKI[LSR1.tm[i].AnimePat[LSR1.tm[i].AnimeCnt]];
							
					LSR1.tm[i].DesR.left  =LSR1.tm[i].Cdn.x;
					LSR1.tm[i].DesR.top   =LSR1.tm[i].Cdn.y;
					LSR1.tm[i].DesR.right =LSR1.tm[i].Cdn.x+LSR1.tm[i].Width;
					LSR1.tm[i].DesR.bottom=LSR1.tm[i].Cdn.y+LSR1.tm[i].Height;
							
					//画面外の部分は描画しないようにする処理
					if(LSR1.tm[i].DesR.left<0){	
						LSR1.tm[i].SrcR.left=LSR1.tm[i].SrcR.left-LSR1.tm[i].DesR.left;
						LSR1.tm[i].DesR.left=0;
					}else if(LSR1.tm[i].DesR.right>WinXMax){
						LSR1.tm[i].SrcR.right=LSR1.tm[i].SrcR.right-(LSR1.tm[i].DesR.right-WinXMax);
						LSR1.tm[i].DesR.right=WinXMax;	
					}
					if(LSR1.tm[i].DesR.top<0){
						LSR1.tm[i].SrcR.top=LSR1.tm[i].SrcR.top-LSR1.tm[i].DesR.top;
						LSR1.tm[i].DesR.top=0;
					}
					//フレームを1減らして0になったらアニメパターンを進める
					if(!(LSR1.tm[i].FrameCnt--)){
						LSR1.tm[i].AnimeCnt=(LSR1.tm[i].AnimeCnt+1)%LSR1.tm[i].Anime;//アニメパターンを進める
						LSR1.tm[i].FrameCnt=LSR1.tm[i].AnimeFrame[LSR1.tm[i].AnimeCnt];//表示するアニメパターンのフレーム数セット
					}
				}else if(LSR1.tm[i].fTama==_TOFF){	
					//再発射可能なレーザーの番号を保存する
					if(TTamaNum1==LSR1.TTama){
						TTamaNum1=i;
						LSR1.tm[i].fTama |=_TSET;	//レーザーを再び発射セット
					}else if(TTamaNum2==LSR1.TTama){
						TTamaNum2=i;
						LSR1.tm[i].fTama |=_TSET;	//レーザーを再び発射セット
					}else if(!(fJIKI&_WEPN_ON)){
						break;						//画面に弾が存在しなけばループを抜ける
					}
				}
			}
			break;
		default:
			break;	
	}
	
	//ミサイルの発射と移動
	
	//発射できる状態になっているミサイルの番号保存用
	static	DWORD	TTamaNum[12]={
		MSR1.TTama,MSR1.TTama,MSR1.TTama,MSR1.TTama,MSR1.TTama,MSR1.TTama,
		MSR1.TTama,MSR1.TTama,MSR1.TTama,MSR1.TTama,MSR1.TTama,MSR1.TTama
	};

	WORD	Kakudo=100;	//ミサイルの発射角度
	bool	ret=TRUE;	//12発分発射可能な弾が確保されているかチェックループ用	
	
	switch(Wpd.flag&_MSR_LVCHK){	//装備のレベルを調べている	
		case _MSR1_ON:
			//-----------------ミサイルの発射------------------------//
			
			if(!MSR1.WTamaCnt){
				//ミサイルが指定された数以上に画面に存在しないようにする処理
				for(i=0;i<12;i++){	//12発分発射可能な弾が確保されているかチェック
					if(TTamaNum[i]>=MSR1.TTama)	ret=FALSE;
				}
				
				if(ret)
				{
					//画面上に弾が発射されているフラグオン
					fJIKI |=_WEPN_ON;
					
					//ミサイルの発射された座標を初期化
					//画面座標をセット
					for(i=0;i<12;i++){
						//発射中のフラグを立てる
						MSR1.tm[TTamaNum[i]].fTama |=_TON;		
						//弾アニメーションを初期化
						MSR1.tm[TTamaNum[i]].AnimeCnt=0;		
						MSR1.tm[TTamaNum[i]].FrameCnt=MSR1.tm[TTamaNum[i]].AnimeFrame[0];	
						//初期座標
						if(i<3){
							MSR1.tm[TTamaNum[i]].Cdn.x=Cdn.x-8;
							MSR1.tm[TTamaNum[i]].Cdn.y=Cdn.y-8;
						}else if(i<6){
							MSR1.tm[TTamaNum[i]].Cdn.x=Cdn.x-4;
							MSR1.tm[TTamaNum[i]].Cdn.y=Cdn.y-10;
						}else if(i<9){
							MSR1.tm[TTamaNum[i]].Cdn.x=Cdn.x+8;
							MSR1.tm[TTamaNum[i]].Cdn.y=Cdn.y-10;
						}else if(i<12){
							MSR1.tm[TTamaNum[i]].Cdn.x=Cdn.x+12;
							MSR1.tm[TTamaNum[i]].Cdn.y=Cdn.y-8;
						}
						//弾の移動計算用に画面座標を取得
						MSR1.tm[TTamaNum[i]].dCdn.x=MSR1.tm[TTamaNum[i]].Cdn.x;
						MSR1.tm[TTamaNum[i]].dCdn.y=MSR1.tm[TTamaNum[i]].Cdn.y;
						//絶対座標をセット
						MSR1.tm[TTamaNum[i]].MapCdn.x=MainCdn.x+MSR1.tm[TTamaNum[i]].Cdn.x;
						MSR1.tm[TTamaNum[i]].MapCdn.y=MainCdn.y+MSR1.tm[TTamaNum[i]].Cdn.y;
						//移動速度の初期化
						MSR1.tm[TTamaNum[i]].Mov.x=16*Cos[Kakudo];
						MSR1.tm[TTamaNum[i]].Mov.y=16*Sin[Kakudo];
						Kakudo -=2;
						if(i==5)	Kakudo +=2;//発射角度90度のミサイルを２発発射する処理	
					
						//MSR1.TTamaで初期化
						//この処理はTTamaNumが上書きされない限り新しく弾を発射できないようにしています
						TTamaNum[i]=MSR1.TTama;
					
						//発射音
						lpDSBuf[_FIRE]->Stop();
						lpDSBuf[_FIRE]->SetCurrentPosition(0);
						lpDSBuf[_FIRE]->Play(0,0,0);
					}
					MSR1.WTamaCnt=MSR1.WTama;	//発射までのカウンタを再セット
				}
			}

			//-------------------ミサイルの移動----------------------//
			
			for(i=0;(DWORD)i<MSR1.TTama;i++){	
				//発射中フラグの立っていない弾は処理しない
				if(MSR1.tm[i].fTama&_TON)
				{
					TamaS=TRUE;	//ミサイルは画面内に存在している
					
					//ミサイルを移動させる
					MSR1.tm[i].dCdn.x +=MSR1.tm[i].Mov.x;
					MSR1.tm[i].dCdn.y -=MSR1.tm[i].Mov.y;
					MSR1.tm[i].Cdn.x=(LONG)MSR1.tm[i].dCdn.x;
					MSR1.tm[i].Cdn.y=(LONG)MSR1.tm[i].dCdn.y;
					MSR1.tm[i].MapCdn.x=MainCdn.x+MSR1.tm[i].Cdn.x;
					MSR1.tm[i].MapCdn.y=MainCdn.y+MSR1.tm[i].Cdn.y;	
					//ミサイルが画面の外に移動していないかチェック
					if(	MSR1.tm[i].Cdn.x+MSR1.tm[i].Width<0	||
						MSR1.tm[i].Cdn.x>WinXMax			||
						MSR1.tm[i].Cdn.y+MSR1.tm[i].Height<0)
					{
							MSR1.tm[i].fTama &=_TOFF;	//フラグをクリア
							continue;
					}
				
					//SPサーフェスからの転送矩形範囲をセット
					MSR1.tm[i].SrcR=TamSPJIKI[MSR1.tm[i].AnimePat[MSR1.tm[i].AnimeCnt]];
							
					MSR1.tm[i].DesR.left  =MSR1.tm[i].Cdn.x;
					MSR1.tm[i].DesR.top   =MSR1.tm[i].Cdn.y;
					MSR1.tm[i].DesR.right =MSR1.tm[i].Cdn.x+MSR1.tm[i].Width;
					MSR1.tm[i].DesR.bottom=MSR1.tm[i].Cdn.y+MSR1.tm[i].Height;
							
					//画面外の部分は描画しないようにする処理
					if(MSR1.tm[i].DesR.left<0){	
						MSR1.tm[i].SrcR.left=MSR1.tm[i].SrcR.left-MSR1.tm[i].DesR.left;
						MSR1.tm[i].DesR.left=0;
					}
					if(MSR1.tm[i].DesR.right>WinXMax){
						MSR1.tm[i].SrcR.right=MSR1.tm[i].SrcR.right-(MSR1.tm[i].DesR.right-WinXMax);
						MSR1.tm[i].DesR.right=WinXMax;
					}
					if(MSR1.tm[i].DesR.top<0){
						MSR1.tm[i].SrcR.top=MSR1.tm[i].SrcR.top-MSR1.tm[i].DesR.top;
						MSR1.tm[i].DesR.top=0;
					}
					
					//フレームを1減らして0になったらアニメパターンを進める
					if(!(MSR1.tm[i].FrameCnt--)){
						MSR1.tm[i].AnimeCnt=(MSR1.tm[i].AnimeCnt+1)%MSR1.tm[i].Anime;//アニメパターンを進める
						MSR1.tm[i].FrameCnt=MSR1.tm[i].AnimeFrame[MSR1.tm[i].AnimeCnt];//表示するアニメパターンのフレーム数セット
					}
				
				}else if(MSR1.tm[i].fTama==_TOFF){	
					//再発射可能なミサイルの番号を保存する
					for(j=0;(DWORD)j<12;j++){
						if(TTamaNum[j]==MSR1.TTama){
							TTamaNum[j]=i;
							MSR1.tm[i].fTama |=_TSET;	//ミサイルを再び発射セット
							break;
						}
					}
					//画面に弾が存在していない時はループを抜ける
					if(!(TTamaNum[11]==MSR1.TTama) && !(fJIKI&_WEPN_ON)){
						break;
					}
				}
			}
			break;
		default:
			break;	
	}
	
	//弾が存在していなかったら主砲発射フラグをオフ
	if(!TamaS)	fJIKI &=_WEPN_OFF;
}

//////////////////////////////////////////////////////////////////////
//	主砲の弾と敵キャラとの当たり判定
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::MainWepnAtari()
{
	short i,j;

	//発射されているレーザーと敵キャラの当たり判定
	switch (Wpd.flag&_LSR_LVCHK){
		case _LSR1_ON:
			for(i=0;(DWORD)i<LSR1.TTama;i++){
				//未発射の弾とは当たり判定をしない
				if(LSR1.tm[i].fTama&_TON){
					if(!(Atari(LSR1.tm[i].DesR))){
						JikiTekiDmg(_LSR1DMG);	//敵キャラにダメージ
					}
				
					//破壊可能な弾との当たり判定
					if(SP_Tamas3){
						for(j=0;(DWORD)j<SP_Tamas3->TTama;j++){
							if(SP_Tamas3->tm3[j].fTama&_TON){
								if(!(Atari(LSR1.tm[i].DesR,SP_Tamas3->tm3[j].DesR))){
									JikiTamas3Dmg(_LSR1DMG,j);
								}
							}
						}
					}
				}
			}
			break;
		default:
			break;
	}

	//発射されているミサイルと敵キャラの当たり判定
	switch (Wpd.flag&_MSR_LVCHK){
		case _MSR1_ON:
			for(i=0;(DWORD)i<MSR1.TTama;i++){
				//未発射の弾とは当たり判定をしない
				if(MSR1.tm[i].fTama&_TON){
					if(!(Atari(MSR1.tm[i].DesR))){
						//ミサイルを消滅させる
						MSR1.tm[i].fTama=_TOFF;		//フラグをクリア
						JikiTekiDmg(_MSR1DMG);	//敵キャラにダメージ
					}
				
					//破壊可能な弾との当たり判定
					if(SP_Tamas3){
						for(j=0;(DWORD)j<SP_Tamas3->TTama;j++){
							if(SP_Tamas3->tm3[j].fTama&_TON){
								if(!(Atari(MSR1.tm[i].DesR,SP_Tamas3->tm3[j].DesR))){
									//ミサイルを消滅させる
									MSR1.tm[i].fTama=_TOFF;		//フラグをクリア
									JikiTamas3Dmg(_MSR1DMG,j);
								}
							}
						}
					}
				}
			}
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////
//	主砲の弾を全て発射待機状態に戻す
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::MainWepnReset()
{
	MainWepnCntReset(); //主砲発射までのカウンタをリセット
	
	short i;
	
	//全てのレーザーのフラグを発射待機状態にセット
	switch (Wpd.flag&_LSR_LVCHK){
		case _LSR1_ON:
			for(i=0;(DWORD)i<LSR1.TTama;i++){
				LSR1.tm[i].fTama=_TOFF;	//フラグをクリア
			}
			break;
		default:
			break;
	}
	//全てのミサイルのフラグを発射待機状態にセット		
	switch (Wpd.flag&_MSR_LVCHK){
		case _MSR1_ON:
			for(i=0;(DWORD)i<MSR1.TTama;i++){
				MSR1.tm[i].fTama=_TOFF;	//フラグをクリア
			}
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////
//	主砲の弾を描画する
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::MainWepnRend()
{	
	short i;
	
	//レーザーの描画
	switch (Wpd.flag&_LSR_LVCHK){
		case _LSR1_ON:	//レーザー１
			for(i=0;(DWORD)i<LSR1.TTama;i++){	
				//発射中フラグの立っていない弾は描画しない
				if(LSR1.tm[i].fTama&_TON)
				{	
					lpDDSBack->BltFast(	LSR1.tm[i].DesR.left,LSR1.tm[i].DesR.top,
										lpDDS_JIKI,&LSR1.tm[i].SrcR,
										DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
				}
			}	
			break;
		default:
			break;
	}
	//ミサイルの描画
	switch (Wpd.flag&_MSR_LVCHK){
		case _MSR1_ON:	//ミサイル１
			for(i=0;(DWORD)i<MSR1.TTama;i++){	
				//発射中フラグの立っていない弾は描画しない
				if(MSR1.tm[i].fTama&_TON)
				{	
					lpDDSBack->BltFast(	MSR1.tm[i].DesR.left,MSR1.tm[i].DesR.top,
										lpDDS_JIKI,&MSR1.tm[i].SrcR,
										DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
				}
			}	
			break;
		default:
			break;
	}
}