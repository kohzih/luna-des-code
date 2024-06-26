/*======================================================================
						自機に関する構造体定義
=======================================================================*/

//---------------------------------------------------------------
// 自機の各種装備レベルを格納する構造体
//---------------------------------------------------------------
typedef struct tag_WpData
{
	DWORD flag;		//装備の各レベルを示すフラグ
	short LSR;		//現在のレーザーの割り振られたEXP値
	short MSR;		//現在のミサイルの割り振られたEXP値
	short HMM;		//現在のホーミングミサイルの割り振られたEXP値
	short HNB;		//現在の空中機雷の割り振られたEXP値
	short OPT;		//現在のオプションの割り振られたEXP値
	short LSR_temp; //計算用
	short MSR_temp; //計算用
	short HMM_temp; //計算用
	short HNB_temp; //計算用
	short OPT_temp; //計算用
} WPDATA;

////////////////////////////////////////////////////////////////////////////////////
//	ホーミングミサイル用の構造体定義	/////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------
// 弾のアニメーションを管理する構造体
//----------------------------------------------------------
typedef struct tag_HmmAnime
{
	DWORD fTama;	   //弾の状態を表わすフラグ
	DWORD STama;	   //追尾している敵キャラの座標を新しく取得する間隔
	DWORD STamaCnt;	   //新しく取得する間隔のカウンタ
	DWORD LockTekiNum; //現在ロックオンしている敵キャラの番号
	short Kakudo;	   //現在の弾の角度
	short TekiKakudo;  //ロックオンしている敵キャラと弾の位置との角度(目標値)
	short Width;	   //弾の横幅
	short Height;	   //弾の縦幅
	DPOINT Mov;		   //弾の移動速度の入ったDPOINT構造体
	DPOINT dCdn;	   //弾の座標計算用のDPOINT構造体
	POINT Cdn;		   //弾の現在の画面座標の入ったPOINT構造体
	POINT MapCdn;	   //弾の現在の絶対座標の入ったPOINT構造体
	BYTE AnimePat[24]; //アニメーションパターン配列
	RECT SrcR;		   //転送元サーフェスの矩形範囲
	RECT DesR;		   //転送先サーフェスの矩形範囲
} HMTAMA;

//---------------------------------------------------------------
// 弾のステータスを管理する構造体
//---------------------------------------------------------------
typedef struct tag_HmmTamaSts
{
	DWORD TTama;	//連続で発射できる弾の数
	DWORD WTama;	//弾を発射する間隔
	DWORD WTamaCnt; //弾発射までのカウンタ

	HMTAMA *tm; // TTama分確保した弾のステータスを管理するTAMA構造体配列のポインタ

} HMTAMAS;

//----------------------------------------------------------------
//	ホーミングミサイルの処理に必要なデータを管理する構造体
//----------------------------------------------------------------
typedef struct tag_HmmData
{
	DWORD fHMM[2];	  //左右から発射されるミサイルのロックオン状況を示すフラグ
	DWORD TekiNum[2]; //現在自機に一番近い敵キャラの番号２つ
	POINT TekiCdn[2]; //現在自機に一番近い敵キャラの座標２つ
} HMDATA;

////////////////////////////////////////////////////////////////////////////////////
//	オプション用の構造体定義	/////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------
// オプションのアニメーションを管理する構造体
//----------------------------------------------------------
typedef struct tag_OptAnime
{
	DWORD fOPT;			 //オプションの状態を表わすフラグ
	short Width;		 //オプションの横幅
	short Height;		 //オプションの縦幅
	DPOINT dCdn;		 //オプションの座標計算用のDPOINT構造体
	POINT Cdn;			 //オプションの現在の画面座標の入ったPOINT構造体
	POINT MapCdn;		 //オプションの現在の絶対座標の入ったPOINT構造体
	BYTE Anime;			 //スプライトのアニメーションパターン数
	BYTE AnimePat[10];	 //アニメーションパターン配列
	BYTE AnimeFrame[10]; //アニメーション切り替えタイミング配列
	BYTE AnimeCnt;		 //現在表示するアニメーションパターン
	BYTE FrameCnt;		 //現在のアニメーションフレーム
	RECT SrcR;			 //転送元サーフェスの矩形範囲
	RECT DesR;			 //転送先サーフェスの矩形範囲
} OPTANI;

//----------------------------------------------------------------
//	オプションの攻撃用データを管理する構造体
//----------------------------------------------------------------
typedef struct tag_OptWep
{
	TAMAS Opt_LSR1;
	TAMAS Opt_MSR1;
} OPTATK;

//----------------------------------------------------------------
// オプションを管理する構造体
//----------------------------------------------------------------
typedef struct tag_OptData
{
	OPTANI Jewel[4];   //各オプション描画データ
	OPTATK AtkDat[4];  //オプション攻撃用データ
	POINT KoteCdn[5];  //固定移動時に使用する過去の自機の座標
	POINT TuibCdn[60]; //追尾移動時に使用する過去の自機の座表
	short OptLV;	   //現在装備されているオプションの数
	short KoteCnt;	   //固定フラグの立っているオプションに使用される移動用カウンタ
	short TuibCnt;	   //追尾フラグの立っているオプションに使用される移動用カウンタ
	short KaitKaku[4]; //回転フラグの立っているオプションに使用される回転角度
	BYTE KaitenF[4];   //回転方向を示すフラグ
} OPTDATA;