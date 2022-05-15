/*====================================================================
	ボスキャラ01クラスの定義
=====================================================================*/

//ボスキャラアニメーションを管理する構造体
typedef struct tag_BossAnime
{
	POINT Local;  //ボスキャラを描画するのに基準となる座標からの距離
	short Width;  //パーツの横幅
	short Height; //パーツの縦幅
	RECT SrcR;	  //転送元サーフェスの矩形範囲
	RECT DesR;	  //転送先サーフェスの矩形範囲
} BOSSANIME;

//ボスキャラアニメーションを管理する構造体2
typedef struct tag_BossAnime2
{
	POINT Local;		 //ボスキャラを描画するのに基準となる座標からの距離
	short Width;		 //パーツの横幅
	short Height;		 //パーツの縦幅
	BYTE Anime;			 //スプライトのアニメーションパターン数
	BYTE AnimePat[10];	 //アニメーションパターン配列のアドレス
	BYTE AnimeFrame[10]; //アニメーション切り替えタイミング配列のアドレス
	BYTE AnimeCnt;		 //現在表示するアニメーションパターン
	BYTE FrameCnt;		 //現在のアニメーションフレーム
	RECT SrcR;			 //転送元サーフェスの矩形範囲
	RECT DesR;			 //転送先サーフェスの矩形範囲
} BOSSANIME2;

//////////////////////////////////////////////////////////////////////
//	クラス定義
//////////////////////////////////////////////////////////////////////

class BOSS01 : protected Draw
{
	DWORD fSPChr; //ボスキャラ01の状態を表すフラグ

	short Width;		   //ボスキャラ01の横幅
	short Height;		   //ボスキャラ01の縦幅
	short Shld;			   //ボスキャラ01破壊までの耐久力
	short ShldCnt;		   //ボスキャラ01残り耐久力のカウンタ
	short ATmt[2];		   //攻撃態勢に入るまでのウェイト数配列(攻撃パターンは2種類)
	short ATmtCnt[2];	   //攻撃態勢に入るまでのウェイト数のカウンタ配列(攻撃パターンは2種類)
	short ATt[2];		   //攻撃を続ける時間配列(攻撃パターンは2種類)
	short ATtCnt[2];	   //攻撃を続ける時間のカウンタ配列(攻撃パターンは2種類)
	DWORD AttackTama1[12]; //発射待機状態にする弾番号保存用(攻撃パターン1)
	DWORD AttackTama2;	   //発射待機状態にする弾番号保存用(攻撃パターン2)

	short ONTime; //画面に表示されてからの経過時間

	short MovKakudo; //ボスキャラ01の移動角度
	DPOINT Mov;		 //ボスキャラ01の移動速度の入った構造体
	DPOINT dCdn;	 //ボスキャラ01の座標計算用の構造体
	POINT Cdn;		 //ボスキャラ01の現在の画面座標の入ったポイント構造体
	POINT MapCdn;	 //ボスキャラ01の現在の絶対座標の入ったポイント構造体

	TAMAS2 B_Msr;	//ボスキャラ01弾のステータスを管理するTAMAS_Kai構造体
	short T_Ichi;	// B_Blockを発射する位置
	TAMAS3 B_Block; //ボスキャラ01弾のステータスを管理するTAMAS_Kai2構造体

	BOSSANIME Parts1;  //ボスキャラ01を構成する描画用データ(パーツ1)
	BOSSANIME Parts2;  //ボスキャラ01を構成する描画用データ(パーツ2)
	BOSSANIME2 Parts3; //ボスキャラ01を構成する描画用データ(パーツ3)
	BOSSANIME2 Parts4; //ボスキャラ01を構成する描画用データ(パーツ4)
	BOSSANIME Parts5;  //ボスキャラ01を構成する描画用データ(パーツ5)
	BOSSANIME2 Parts6; //ボスキャラ01を構成する描画用データ(パーツ6)

	BOSSANIME2 BossBaku1[12];  //ボスキャラ01爆発アニメーションを管理する構造体1
	BOSSANIME2 BossBaku2[200]; //ボスキャラ01爆発アニメーションを管理する構造体2

public:
	//--------------以下のメソッドはBOSS01.cppに記述---------------------------//
	BOOL InitBoss01(POINT);	   //ボスキャラ01の初期化
	void MovBoss01();		   //ボスキャラ01の移動処理
	void Attack1Boss01(POINT); //ボスキャラ01の攻撃処理1
	void Attack2Boss01();	   //ボスキャラ01の攻撃処理2
	void FrameBoss01(DWORD);   //ボスキャラ01の描画準備を行なう処理
	void RendBoss01();		   //ボスキャラ01の描画

	DWORD GetBoss01fSPChr();	  //ボスキャラ01状態フラグを返す
	POINT GetBoss01Cdn();		  //ボスキャラ01の画面座標を返す
	short GetBoss01Width();		  //ボスキャラ01の横幅を返す
	short GetBoss01Height();	  //ボスキャラ01の縦幅を返す
	short *GetBoss01ShldCnt_P();  //ボスキャラ01の残り耐久力の値へのポインタを返す
	TAMAS2 *GetBoss01B_Msr_P();	  //弾の状態を管理するTAMAS2構造体へのポインタを返す
	TAMAS3 *GetBoss01B_Block_P(); //弾の状態を管理するTAMAS3構造体へのポインタを返す

	BOSS01();
	~BOSS01();
	//-------------------------------------------------------------------------//
};
