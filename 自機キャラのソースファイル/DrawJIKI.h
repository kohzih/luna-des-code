/*====================================================================
	自機クラスの定義
=====================================================================*/
#include "DrawJIKIStruct.h"

class DrawJIKI : protected Draw
{
	DWORD fJIKI;  //自機の状態を表わすフラグ
	DWORD Target; //破壊するとステージクリアになる敵キャラの番号
	DWORD SCORE;  //現在のスコア

	int InvincCnt;	 //無敵時間のカウンタ
	int ContinueCnt; // RendCONTINUE()内で使用するカウンタ

	POINT MapCdn; //自機の現在の絶対座標の入ったポイント構造体
	POINT Cdn;	  //現在の画面座標の入ったポイント構造体
	POINT Mov;	  //移動速度の入ったポイント構造体
	POINT CurCdn; //パワーアップ画面のカーソルの表示座標

	BYTE EXP;	 // EXP(Eclipse Xeres Pleuvoir)の量
	LONG EXP_R;	 // EXPの量(16bit右にシフトしてEXPに代入)
	BYTE FUEL;	 //弾吸収燃料残り
	LONG FUEL_R; //弾吸収燃料残り(16bit右にシフトしてFUELに代入)

	SPANIME ArmJIKI; //アーム発射時の自機のアニメーション
	SPANIME Arm;	 //アーム発射時のアームアニメーション
	SPANIME Baku;	 //自機が撃墜された時のアニメーション
	SPANIME TmKySy;	 //弾吸収中の自機のアニメーション

	WPDATA Wpd;	   //現在の自機の装備レベルを管理する構造体
	TAMAS LSR1;	   //レーザー1のアニメーションとステータス管理
	TAMAS MSR1;	   //ミサイル1のアニメーションとステータス管理
	TAMAS2_2 HNB1; //花火1のアニメーションとステータス管理
	HMTAMAS HMM1;  //ホーミングミサイル1のアニメーションとステータス管理
	HMDATA HMMDAT; //ホーミングミサイルの処理に必要なデータ
	OPTDATA OPT;   //オプションを管理する構造体

	RECT JIKIChr;	 //通常時の自機の矩形範囲
	RECT SCORE_R[9]; //スコアの部分の矩形範囲
	RECT GAGE_F;	 // FUELゲージの現在の矩形範囲
	RECT GAGE_FK;	 // FUELゲージの空白部分の矩形範囲
	RECT GAGE_FW;	 // FUELゲージの枠部分の矩形範囲
	RECT GAGE_E;	 // EXPゲージの現在の矩形範囲
	RECT GAGE_EK;	 // EXPゲージの空白部分の矩形範囲
	RECT GAGE_EW;	 // EXPゲージの枠部分の矩形範囲
	RECT GAGE_P;	 // PWUゲージの現在の矩形範囲
	RECT GAGE_P2;	 // PWUゲージの現在の矩形範囲2
	RECT GAGE_PW;	 // PWUゲージの枠部分の矩形範囲
	RECT OPTWAKU;	 // OPT用の枠の矩形範囲

	//---------------以下のメソッドはDrawJIKI_Jiki.cppに記述-------------------
	BOOL InitJikiData();			//自機の様々なデータを初期化する
	void InitJikiTm();				//動的に確保するメモリ領域へのポインタをNULLで初期化
	void JikiTmRelease();			//確保したメモリ領域を解放
	void JikiTekitamaAtari();		//自機と敵の弾との当たり判定
	void JikiTekiAtari();			//自機と敵との当たり判定
	BOOL Atari(RECT);				//引数で渡された座標との当たり判定を行う
	BOOL Atari(RECT, RECT);			//引数で渡された座標との当たり判定を行う
	void JikiTekiDmg(int);			//引数で指定されたダメージ値を敵キャラに与える
	void JikiTamas3Dmg(int, short); //引数で指定されたダメージ値を敵弾に与える
	void InitPWUP();				//自機のパワーアップ画面を初期化する
	void RendPWUP();				//自機のパワーアップ画面を描画
	void RendCONTINUE();			//コンティニュー画面を描画する
	void InitCONTINUE();			//コンティニューに伴う初期化を行う
	void ScoreCalcul();				//現在のスコア表示のための矩形範囲をセット
	//-------------------------------------------------------------------------

	//---------------以下のメソッドはDrawJIKI_JikiFrame.cppに記述--------------
	void JikiBakuFrame(); //自機の爆破シーンのアニメーション処理
	void JikiArmFrame();  //自機のアーム発射シーンのアニメーション処理
	void JikiTmkyFrame(); //自機の弾吸収シーンのアニメーション処理
	//-------------------------------------------------------------------------

	//---------------以下のメソッドはDrawJIKI_MainWepn.cppに記述---------------
	BOOL InitMainWepn();	 //主砲から発射される弾オブジェクトの初期化
	void InitWepnTm();		 //動的に確保するメモリ領域へのポインタをNULLで初期化
	void MainWepnRelease();	 //確保したメモリ領域を解放
	void MainWepnCnt();		 //主砲発射までのカウントを進める
	void MainWepnCntReset(); //主砲発射までのカウンタをリセットさせる
	void MainWepnReset();	 //主砲の弾を全て発射待機状態に戻す
	void MainWepn();		 //主砲発射と移動の処理
	void MainWepnAtari();	 //主砲の弾と敵キャラとの当たり判定
	void MainWepnRend();	 //主砲から発射された弾の描画
	//-------------------------------------------------------------------------

	//---------------以下のメソッドはDrawJIKI_Hmm.cppに記述--------------------
	BOOL InitHmm();		   //ホーミングミサイルオブジェクトの初期化
	void InitHmmTm();	   //動的に確保するメモリ領域へのポインタをNULLで初期化
	void HmmRelease();	   //確保したメモリ領域を解放
	void HmmCnt();		   //ホーミングミサイル発射までのカウントを進める
	void HmmCntReset();	   //ホーミングミサイル発射までのカウントをリセットさせる
	void HmmReset();	   //全てのホーミングミサイルを発射待機状態に戻す
	void HmmGetTekiInfo(); //ロックオンするために自機に一番近い距離にいる敵キャラの情報を取得する
	void HmmMovCnt();	   //既に発射されたホーミングミサイルの移動量を求める
	void HmmAttack();	   //ホーミングミサイル発射と移動の処理
	void HmmAtari();	   //敵キャラと当たり判定
	void HmmRend();		   //ホーミングミサイルの描画処理
	//-------------------------------------------------------------------------

	//---------------以下のメソッドはDrawJIKI_Hnb.cppに記述--------------------
	BOOL InitHnb();		//花火オブジェクトの初期化
	void InitHnbTm();	//動的に確保するメモリ領域へのポインタをNULLで初期化
	void HnbRelease();	//確保したメモリ領域を解放
	void HnbCnt();		//花火発射までのカウントを進める
	void HnbCntReset(); //花火発射までのカウントをリセットさせる
	void HnbReset();	//全ての花火を発射待機状態に戻す
	void HnbAttack();	//花火発射と移動の処理
	void HnbAtari();	//花火の敵キャラとの当たり判定
	void HnbRend();		//花火の描画処理
	//-------------------------------------------------------------------------

	//---------------以下のメソッドはDrawJIKI_Opt.cppに記述--------------------
	BOOL InitOpt();			  //オプションオブジェクトの初期化
	void InitOptAtkDatTm();	  //動的に確保するメモリ領域へのポインタをNULLで初期化
	void OptRelease();		  //確保したメモリ領域を解放
	void InitOptCdn();		  //オプションの表示座標の初期化
	void OptMovCnt(BYTE);	  //オプションの移動量を求める
	void OptMov();			  //オプションの移動処理
	void OptAttackCnt();	  //弾発射までのカウントを進める
	void OptAttackCntReset(); //弾発射までのカウントをリセットする
	void OptAttackReset();	  //全てのオプションの発射した弾を発射待機状態に戻す
	void OptAttack();		  //オプションの攻撃処理
	void OptAttackAtari();	  //発射した弾との当たり判定を行なう
	void OptRend();			  //オプションの描画処理
	void OptWepRend();		  //オプションの発射した弾の描画
							  //-------------------------------------------------------------------------

public:
	DWORD SP_Num;	   //敵キャラの番号
	DWORD SP_fEvt;	   //敵キャラの種類
	DWORD SP_fSPChr;   //敵キャラの状態
	POINT SP_Cdn;	   //敵キャラの座標
	short SP_Width;	   //敵キャラの横幅
	short SP_Height;   //敵キャラの縦幅
	short *SP_Shld;	   //敵キャラの残り耐久力へのポインタ
	TAMAS *SP_Tamas;   //敵キャラの弾の状態を管理するTAMAS構造体配列のポインタ
	TAMAS2 *SP_Tamas2; //敵キャラの弾の状態を管理するTAMAS2構造体配列のポインタ
	TAMAS3 *SP_Tamas3; //敵キャラの弾の状態を管理するTAMAS3構造体配列のポインタ

	//---------------以下のメソッドはDrawJIKI.cppに記述---------------------------
	BOOL InitJIKI();	//自機オブジェクト初期化
	void MoveJIKI();	//自機の移動と弾発射までのカウントダウン
	DWORD AtariJIKI();	//自機と敵キャラの当たり判定
	void FrameJIKI();	//自機に関するグラフィックの描画に使用する各値を更新する
	void RendJIKI();	//自機に関するグラフィックを描画
	DWORD GetfJIKI();	//自機の状態データを戻り値として返す
	POINT GetJIKICdn(); //自機の画面座標を戻り値として返す

	DrawJIKI();
	virtual ~DrawJIKI();
	//-------------------------------------------------------------------------
};
