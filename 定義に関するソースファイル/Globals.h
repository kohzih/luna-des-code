/*====================================================================
	グローバル変数1
=====================================================================*/

//---------------------------------------------------------
// Windows関連　グローバル
//---------------------------------------------------------
HINSTANCE hInst; //インスタンスハンドル
HWND hWnd;		 //メインウィンドウハンドル

BOOL fActive = 0;	  //アクティブフラグ(ウィンドウ表示用)
BOOL fFullscreen = 1; //フルスクリーン(1)orウィンドウフラグ(0)
BOOL fInputDevice;	  //入力デバイスチェックフラグ

DWORD DisplayColor;		 //ディスプレイのカラーパレット
DWORD TotalMem, FreeMem; // VRAMの空き容量の値を格納

//---------------------------------------------------------
// DirectDraw関連　グローバル
//---------------------------------------------------------
LPDIRECTDRAW7 lpDD = NULL; // DirectDrawオブジェクト

LPDIRECTDRAWSURFACE7 lpDDSPrimary = NULL, //プライマリサーフェース
	lpDDSBack = NULL;					  //バックサーフェース

LPDIRECTDRAWPALETTE lpDDpal = NULL; // DirectDrawパレットオブジェクト

LPDIRECTDRAWCLIPPER lpDDClip = NULL; //クリッパー(ウィンドウ表示用)

//---------------------------------------------------------
// DirectSound関連
//---------------------------------------------------------
LPDIRECTSOUND lpDS = NULL;
LPDIRECTSOUNDBUFFER lpDSBPrimary = NULL;
LPDIRECTSOUNDBUFFER lpDSBuf[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

//---------------------------------------------------------
// DirectMusic関連
//---------------------------------------------------------
LPDIRECTMUSIC lpDMusic = NULL;
IDirectMusicPerformance *lpDMPerf = NULL;
IDirectMusicLoader *lpDMLoader = NULL;
IDirectMusicComposer *lpDMComposer = NULL;

IDirectMusicSegment *lpDMSegment[5] = {NULL, NULL, NULL, NULL, NULL};		//セグメント
IDirectMusicSegmentState *lpDMSegState[5] = {NULL, NULL, NULL, NULL, NULL}; //セグメントの状態
MUSIC_TIME SegLength[5] = {0, 0, 0, 0, 0};									//セグメントの長さ
MUSIC_TIME SegStartTime[5] = {0, 0, 0, 0, 0};								//セグメントの開始時間
MUSIC_TIME SegEndTime[5] = {0, 0, 0, 0, 0};									//セグメントの終了時間

//---------------------------------------------------------
// DirectInput関連
//---------------------------------------------------------
LPDIRECTINPUT7 lpDI = NULL;
LPDIRECTINPUTDEVICE7 lpDIJoystick = NULL;
LPDIRECTINPUTDEVICE7 lpDIKeyboard = NULL;