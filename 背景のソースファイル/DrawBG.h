/*====================================================================
	背景クラスの定義
=====================================================================*/

class DrawBG : protected Draw
{
	DWORD fBG;		//背景の状態を示すフラグ
	DWORD JK_fJIKI; //自機の状態フラグのコピー

	int ScrCnt; //画面のスクロールカウンタ
	int ScrSpd; //画面のスクロールスピード
	int ScrWt;	//スクロール停止カウンタ

public:
	BOOL InitBG();			 // DrawBGを初期化
	void MoveBG();			 //絶対座標を移動させ画面スクロール
	void FrameBG(DWORD);	 //引数で自機フラグを受け取り描画準備
	void RendBG();			 //背景の描画
	DWORD GetfBG();			 //現在のBGフラグを返す
	void SetfBG(DWORD, int); //引数で渡されたフラグをセット

	DrawBG();
	~DrawBG();
};