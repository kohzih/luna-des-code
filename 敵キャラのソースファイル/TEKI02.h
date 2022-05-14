/*====================================================================
	敵キャラ02クラスの定義
=====================================================================*/

class TEKI02 : protected Draw   
{
	DWORD	fSPChr;			//敵キャラ02の状態を表すフラグ
	
	short	Width;			//敵キャラ02の横幅
	short	Height;			//敵キャラ02の縦幅
	short	Shld;			//敵キャラ02破壊までの耐久力
	short	ShldCnt;		//敵キャラ02残り耐久力のカウンタ
	short	ATmt;			//攻撃態勢に入るまでのウェイト数
	short	ATmtCnt;		//攻撃態勢に入るまでのウェイト数のカウンタ
	short	ATt;			//攻撃を続ける時間
	short	ATtCnt;			//攻撃を続ける時間のカウンタ
	DWORD	TTamaNum;		//次ぎに発射する弾番号保存用
	
	short	ONTime;			//画面に表示されてからの経過時間
	short	TamaKaku;		//弾を発射する角度
	
	DPOINT	Mov;			//敵キャラ02の移動速度の入った構造体
	DPOINT	dCdn;			//敵キャラ02の座標計算用の構造体
	POINT	Cdn;			//敵キャラ02の現在の画面座標の入ったポイント構造体
	POINT	MapCdn;			//敵キャラ02の現在の絶対座標の入ったポイント構造体	
	
	TAMAS	TMsr;			//敵キャラ02弾のステータスを管理するTAMAS構造体
	SPANIME	TChr;			//敵キャラ02アニメーションを管理する構造体
	SPANIME	TBaku;			//敵キャラ02爆発アニメーションを管理する構造体
	
public:
	//--------------以下のメソッドはTEKI02.cppに記述---------------------------//
	BOOL	InitTeki02(POINT);	//敵キャラ02の初期化
	void	MovTeki02();		//敵キャラ02の移動処理
	void	AttackTeki02();		//敵キャラ02の攻撃処理
	void	FrameTeki02(DWORD);	//敵キャラ02の描画準備を行なう処理
	void	RendTeki02();		//敵キャラ02の描画
	
	DWORD	GetTeki02fSPChr();		//敵キャラ02状態フラグを返す
	POINT	GetTeki02Cdn();			//敵キャラ02の画面座標を返す
	short	GetTeki02Width();		//敵キャラ02の横幅を返す
	short	GetTeki02Height();		//敵キャラ02の縦幅を返す
	short*	GetTeki02ShldCnt_P();	//敵キャラ02の残り耐久力の値へのポインタを返す
	TAMAS*	GetTeki02TMsr_P();		//弾の状態を管理するTAMAS構造体へのポインタを返す
	
	TEKI02();
	~TEKI02();
	//-------------------------------------------------------------------------//
};

