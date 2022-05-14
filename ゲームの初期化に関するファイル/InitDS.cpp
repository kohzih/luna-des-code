/*====================================================================
	DirectSound初期化に関するソースファイル
=====================================================================*/

#include	<windows.h>
#include	<dsound.h>

#include	"Macros.h"
#include	"wavread.h"

extern	HWND		hWnd;		//メインウィンドウハンドル

extern	LPDIRECTSOUND			lpDS;
extern	LPDIRECTSOUNDBUFFER		lpDSBPrimary;
extern	LPDIRECTSOUNDBUFFER		lpDSBuf[6];

WAVEFORMATEX  *pwfx;          // ウェーブ フォーマット情報。
HMMIO         hmmio;          // ファイル ハンドル。
MMCKINFO      mmckinfoData;   // チャンク情報。
MMCKINFO      mmckinfoParent; // 親チャンクの情報。

HRESULT WaveOpenFile( CHAR*, HMMIO*, WAVEFORMATEX**, MMCKINFO*);
HRESULT WaveStartDataRead( HMMIO*, MMCKINFO*, MMCKINFO* );
HRESULT WaveReadFile( HMMIO, UINT, BYTE*, MMCKINFO*, UINT* );

BOOL	CreateBufferFromWaveFile(char*, DWORD);

//---------------------------------------------------------
//DirectSound初期化
//---------------------------------------------------------
BOOL	InitDS()
{
	HRESULT			ret;
	
	char			*SoundFileName[]={	".\\Sound\\Jikibaku.wav",//自機爆発
										".\\Sound\\Tekibaku.wav",//敵爆発
										".\\Sound\\FuelArm.wav",//アーム発射
										".\\Sound\\Fire.wav",	//主砲発射
										".\\Sound\\Tamakyu.wav",//弾吸収中
										".\\Sound\\Bossbaku.wav"};//ボスキャラ爆発

	//DirectSoundオブジェクト作成
	ret=DirectSoundCreate(NULL,&lpDS,NULL);
	if(ret != DS_OK)		return	FALSE;
	
	//協調レベル設定
	ret=lpDS->SetCooperativeLevel(hWnd,DSSCL_PRIORITY);
	if(ret != DS_OK)		return	FALSE;
	
	//プライマリバッファの作成
	DSBUFFERDESC                dsbdesc;
	
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
    dsbdesc.dwSize = sizeof(DSBUFFERDESC);
    dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
    ret=lpDS->CreateSoundBuffer(&dsbdesc, &lpDSBPrimary, NULL);
    if(ret != DS_OK)	   return FALSE;	
	
	// プライマリ バッファのフォーマットを設定する。
	WAVEFORMATEX wfx;
    memset(&wfx, 0, sizeof(WAVEFORMATEX)); 
    wfx.wFormatTag = WAVE_FORMAT_PCM; 
    wfx.nChannels = 2; 
    wfx.nSamplesPerSec = 44100; 
    wfx.wBitsPerSample = 8; 
    wfx.nBlockAlign = wfx.wBitsPerSample / 8 * wfx.nChannels;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	ret = lpDSBPrimary->SetFormat(&wfx); 
	if(ret != DS_OK	)				return FALSE;
	
	//ウェーブファイルを読み込んでバックバッファを作成
	for(int i=0;i<6;i++)
	{
		ret=CreateBufferFromWaveFile(SoundFileName[i],i);
		if(ret != TRUE)		return	FALSE;
	}
	
	return TRUE;
} 

//---------------------------------------------------------
//バックバッファ作成
//---------------------------------------------------------
BOOL	CreateBufferFromWaveFile(char	*FileName, DWORD dwBuf)
{

	//この関数はファイルへのハンドルを取得し、
	//そのファイルが RIFF フォーマットであることを確認し、
	//ウェーブ フォーマットに関する情報を取得する。
	if(WaveOpenFile(FileName, &hmmio, &pwfx, &mmckinfoParent) != 0)
	{	
		mmioClose( hmmio, 0 );
		pwfx   = NULL;
		return FALSE; 
	}
	
	//この関数は mmckinfoData 内のデータ チャンクに関する情報を返す。
	//ファイル ポインタを使ってデータ チャンクに到達する。
	//この関数はデータ チャンク用の MMCKINFO 構造体への書き込みを行うので、
	//利用可能なデータの量を把握できる。
	if (WaveStartDataRead(&hmmio, &mmckinfoData, &mmckinfoParent) != 0)
	{	
		mmioClose( hmmio, 0 );
		pwfx   = NULL;
		return FALSE; 
	}

	//読み込んだウェーブファイルと同じフォーマットでセカンダリサウンドバッファを作成する。
	DSBUFFERDESC         dsbdesc;
 
	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); 
	dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
	dsbdesc.dwFlags = DSBCAPS_STATIC; 
	dsbdesc.dwBufferBytes = mmckinfoData.cksize;  
	dsbdesc.lpwfxFormat = pwfx; 
 
	if FAILED(lpDS->CreateSoundBuffer(&dsbdesc, &lpDSBuf[dwBuf], NULL))
	{
		mmioClose( hmmio, 0 );
		pwfx   = NULL;
		return FALSE; 
	}

	//この場合は、アプリケーションはデータのストリーミングを行わず、
	//単純にスタティック バッファへの書き込みを行うので、
	//バッファ全体を先頭からロックする。
	//ラップ アラウンド (先頭へ戻る) も発生しないので、
	//必要なのは 1 つのポインタとバイト カウントである。

	LPVOID lpvAudio1;
	DWORD  dwBytes1;
 
	if FAILED(lpDSBuf[dwBuf]->Lock(
		    0,              // ロック開始のオフセット。
			0,              // ロックのサイズ。この場合は無視される。
			&lpvAudio1,     // ロック開始のアドレス。
			&dwBytes1,      // ロックされたバイト数。
			NULL,           // ラップ アラウンドの開始。不使用。
			NULL,           // ラップ アラウンドのサイズ。不使用。
			DSBLOCK_ENTIREBUFFER))  // フラグ。
	{
		// エラー処理。
		mmioClose( hmmio, 0 );
		pwfx   = NULL;
		return FALSE; 
	}


	//この関数は、ファイルからバッファのポインタへデータをコピーし、成功した場合は 0 を返す。 

	UINT cbBytesRead;
 
	if (WaveReadFile(
			hmmio,              // ファイル ハンドル。
			dwBytes1,           // 読み込むべきバイト数。
			(BYTE *) lpvAudio1, // コピー先。
		    &mmckinfoData,      // ファイル チャンク情報。
			&cbBytesRead))      // 実際に読み込まれたバイト数。
	{
		// 0 以外の値が返された場合の失敗を処理する。
		mmioClose( hmmio, 0 );
		pwfx   = NULL;
		return FALSE; 
	}

	//最後にアプリケーションはバッファをアンロックし、ウェーブ ファイルを閉じる。

	lpDSBuf[dwBuf]->Unlock(lpvAudio1, dwBytes1, NULL, 0);
	mmioClose( hmmio, 0 );
	pwfx   = NULL;
    
	return	TRUE;
}

//---------------------------------------------------------
//DirectSoundオブジェクト解放
//---------------------------------------------------------
void	ReleaseDS(void)
{	
		for(int i=0;i<6;i++)
			RELEASE(lpDSBuf[i]);
		RELEASE(lpDS);
}
 



							
	