/*=============================================================================
	フェードインアウトなどのパレット管理クラスの定義
==============================================================================*/

//---------------------------------------------------------
// フェードイン,フェードアウトに使用する構造体
//---------------------------------------------------------
typedef	struct	tag_PALETTECONTROL{
		int	r,g,b;
}PALETTECONTROL;

class FaInOut  
{
	BOOL	WFadeF;			//ホワイトアウト、インの管理フラグ
	BOOL	BFadeF;			//ブラックアウト、インの管理フラグ
	BOOL	LBFadeF;		//LittleBlack管理用のフラグ

	short	StartPal;		//変化を開始させる先頭パレット番号	
	short	PalEntry;		//変化させるパレットの個数
	int		PalCnt;			//現在の進行状況(PalCnt>>16で0～255)
	
	PALETTEENTRY	apeH1[256],		//目標の色
					apeH2[256];		//作業用
	PALETTECONTROL	apeH3[256];		//作業用小数点対応パレット

public:	
		
	BOOL	SetPaletteEntry();		//現在のパレットを取得
	void	SetFade(DWORD);			//各フェードインアウトを即実行できるように設定する
	BOOL	WhiteOut(int);			//ホワイトアウト
	BOOL	WhiteIn(int);			//ホワイトイン
	BOOL	BlackOut(int);			//ブラックアウト
	BOOL	BlackIn(int);			//ブラックイン
	BOOL	LittleBlack(int,bool);	//画面を引数で指定された値分、暗くする

	FaInOut(short,short);
	~FaInOut();
};
