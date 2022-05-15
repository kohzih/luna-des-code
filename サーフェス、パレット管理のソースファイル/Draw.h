/*=============================================================================
	自機、敵キャラ、背景クラスの基本クラスの定義
==============================================================================*/

//----------------------------------------------------------
// BGの転送する画像の矩形左上座標とイベントを格納する構造体
//----------------------------------------------------------
typedef struct tag_BgMap
{
	WORD X;	  // lpDDBGDataの転送する画像矩形範囲の左上X座標
	WORD Y;	  //						　　　　　　　　Y座標
	WORD Evt; //イベント
} BGMAP;

class Draw
{
protected:
	static LPDIRECTDRAWSURFACE7 lpDDS_JIKI; //自機キャラ用画像サーフェス
	static LPDIRECTDRAWSURFACE7 lpDDS_SP;	//敵キャラ用画像サーフェス
	static LPDIRECTDRAWSURFACE7 lpDDS_BG;	//背景画像用サーフェス
	static LPDIRECTDRAWSURFACE7 lpDDS_BG2;	//背景画像用サーフェス2
	static DWORD fSTAGE;					//各ステージの開始と終了を示すフラグ
	static int StageNum;					//現在のステージ番号
	static POINT MainCdn;					//ゲーム内の絶対座標
	static DWORD MapSizeX;					//背景マップデータの横サイズ（実際の値はMapSizeX*ChipSizeXドット）
	static DWORD MapSizeY;					//背景マップデータの縦サイズ（実際の値はMapSizeY*ChipSizeYドット）
	static DWORD TipSizeX;					//背景チップ一個の横のサイズ
	static DWORD TipSizeY;					//背景チップ一個の縦のサイズ
	static BGMAP *Parts;					//背景マップ構造体配列の先頭アドレス
	static double *Sin;						// 0-360度分のサイン値配列の先頭アドレスを格納するポインタ
	static double *Cos;						// 0-360度分のコサイン値配列の先頭アドレスを格納するポインタ
	static double *Tan;						// 0-89度分のタンジェント値配列の先頭アドレスを格納するポインタ

private:
	char *JIKIFileName;		 //自機の描画用ビットマップのファイル名
	char *PrologFileName;	 //プロローグ画面描画用のファイル名
	char *TitleFileName[2];	 //タイトル画面描画用のファイル名(タイトル文字、タイトル画面)
	char *St1FileName[4];	 //スーテジ1描画用のファイル名(敵キャラ、背景、マップデータ)
	char *GameOverFileName;	 //ゲームオーバー画面描画用のビットマップファイル名
	char *GameClearFileName; //ゲームクリア画面描画用のビットマップファイル名

	//ビットマップを読み込んでオフスクリーンサーフェスを作成
	BOOL LoadBitMap(char *, LPDIRECTDRAWSURFACE7 &lpDDsuf);
	//マップデータを読み込む
	BOOL LoadMapData(char *);

public:
	static DWORD EventCnt; // Parts構造体配列に含まれるイベント(DrawSPオブジェクト)の数

	BOOL Init();													 //座標、各カウンタ、サーフェスの初期化
	BOOL ReleaseALL();												 // Drawオブジェクトの全メンバを解放
	BOOL RestoreJIKI();												 //自機用サーフェスをリストアする関数
	BOOL RestoreSP();												 // SP用サーフェスをリストアする関数
	BOOL RestoreBG();												 // BG用サーフェスをリストアする関数
	BOOL GetMapSizeData(DWORD &mx, DWORD &my, DWORD &cx, DWORD &cy); //マップの構成データを求める
	BOOL GetEventData(int, WORD &event);							 //引数で指定された場所のBGMAP構造体のイベントデータを求める
	DWORD GetfSTAGE();												 //現在のステージフラグを返す
	void SetfSTAGE(DWORD, int);										 //引数で渡されたフラグをセット
	int GetStageNum();												 //現在のステージ番号を返す
	void SetStageNum(int);											 //引数で渡されたステージ番号セット
	short TanKaku(double);											 //タンジェントの値を角度に直す

	//ウィンドウモードとフルスクリーン切り替え用メンバ関数
	BOOL ReleaseSuf(); //オフスクリーンサーフェスを解放
	BOOL ReloadSuf();  //オフスクリーンサーフェスを再構築

	Draw();								 //派生クラス用コンストラクタ
	Draw(char **FileName, HRESULT &ret); //基本クラス用コンストラクタ
	virtual ~Draw();
};
