/*====================================================================
	敵キャラ01クラスの定義
=====================================================================*/

class TEKI01 : protected Draw   
{
	DWORD	fSPChr;			//敵キャラ01の状態を表すフラグ

	short	Width;			//敵キャラ01の横幅
	short	Height;			//敵キャラ01の縦幅
	short	Shld;			//敵キャラ01破壊までの耐久力
	short	ShldCnt;		//敵キャラ01破壊までの耐久力のカウンタ
	short	ATmt;			//攻撃態勢に入るまでのウェイト数
	short	ATmtCnt;		//攻撃態勢に入るまでのウェイト数のカウンタ
	short	ATt;			//攻撃を続ける時間
	short	ATtCnt;			//攻撃を続ける時間のカウンタ
	DWORD	TTamaNum;		//次ぎに発射する弾番号保存用
	
	DPOINT	Mov;			//敵キャラ01の移動速度の入った構造体
	DPOINT	dCdn;			//敵キャラ01の座標計算用の構造体
	POINT	Cdn;			//敵キャラ01の現在の画面座標の入ったポイント構造体
	POINT	MapCdn;			//敵キャラ01の現在の絶対座標の入ったポイント構造体	
	
	TAMAS	TMsr;			//敵キャラ01弾のステータスを管理するTAMAS構造体		
	SPANIME	TChr;			//敵キャラ01アニメーションを管理する構造体
	SPANIME	TBaku;			//敵キャラ01爆発アニメーションを管理する構造体

public:
	//-----------以下のメソッドはTEKI01.cppに記述------------------------------//
	BOOL	InitTeki01(POINT);	//敵キャラ01の初期化
	void	MovTeki01();		//敵キャラ01の移動処理
	void	AttackTeki01();		//敵キャラ01の攻撃処理
	void	FrameTeki01(DWORD);	//敵キャラ01の描画準備を行なう処理
	void	RendTeki01();		//敵キャラ01の描画
	
	DWORD	GetTeki01fSPChr();		//敵キャラ01状態フラグを返す
	POINT	GetTeki01Cdn();			//敵キャラ01の画面座標を返す
	short	GetTeki01Width();		//敵キャラ01の横幅を返す
	short	GetTeki01Height();		//敵キャラ01の縦幅を返す
	short*	GetTeki01ShldCnt_P();	//敵キャラ01の残り耐久力の値へのポインタを返す
	TAMAS*	GetTeki01TMsr_P();		//弾の状態を管理するTAMAS構造体へのポインタを返す

	TEKI01();
	~TEKI01();
	//-------------------------------------------------------------------------//
};