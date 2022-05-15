/*====================================================================
	DirectMusic初期化に関するソースファイル
=====================================================================*/

#include <windows.h>
#include <direct.h>
#include <dsound.h>
#include <dmusicc.h>
#include <dmusici.h>

#include "Macros.h"

extern HWND hWnd; //メインウィンドウハンドル
extern LPDIRECTSOUND lpDS;

extern LPDIRECTMUSIC lpDMusic;
extern IDirectMusicPerformance *lpDMPerf;
extern IDirectMusicLoader *lpDMLoader;
extern IDirectMusicComposer *lpDMComposer;

// DirectMusicのセグメント
extern IDirectMusicSegment *lpDMSegment[5];		  //セグメント
extern IDirectMusicSegmentState *lpDMSegState[5]; //セグメントの状態
extern MUSIC_TIME SegLength[5];					  //セグメントの長さ
extern MUSIC_TIME SegStartTime[5];				  //セグメントの開始時間
extern MUSIC_TIME SegEndTime[5];				  //セグメントの終了時間

#define MULTI_TO_WIDE(x, y) MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, y, -1, x, _MAX_PATH);

//--------------------------------------------------------------
//	DirectMusicの初期化
//--------------------------------------------------------------
BOOL InitDM()
{
	HRESULT ret = S_OK;

	// COMの初期化
	ret = CoInitialize(NULL);
	if (ret != S_OK)
		return FALSE;

	// IDirectMusicPerformanceオブジェクトの作成
	ret = CoCreateInstance(CLSID_DirectMusicPerformance,
						   NULL,
						   CLSCTX_INPROC,
						   IID_IDirectMusicPerformance2,
						   (void **)&lpDMPerf);
	if (ret != S_OK)
		return FALSE;

	// IDirectMusicComposerオブジェクトの作成
	ret = CoCreateInstance(CLSID_DirectMusicComposer,
						   NULL,
						   CLSCTX_INPROC,
						   IID_IDirectMusicComposer,
						   (void **)&lpDMComposer);
	if (ret != S_OK)
		return FALSE;

	// IDirectMusicオブジェクトの初期化(ゲーム内でDirectSound使用)
	ret = lpDMPerf->Init(&lpDMusic, lpDS, NULL);
	////	ret=lpDMPerf->Init(&lpDMusic,NULL,hWnd);
	if (ret != S_OK)
		return FALSE;

	ret = lpDMPerf->AddPort(NULL);
	if (ret != S_OK)
		return FALSE;

	// DirectMusicLoaderオブジェクトの作成と初期化
	ret = CoCreateInstance(CLSID_DirectMusicLoader,
						   NULL,
						   CLSCTX_INPROC,
						   IID_IDirectMusicLoader,
						   (void **)&lpDMLoader);
	if (ret != S_OK)
		return FALSE;

	//------ファイルの検索パスをカレントディレクトリに設定する処理----//
	char szDir[_MAX_PATH];
	WCHAR wszDir[_MAX_PATH];

	//カレントディレクトリの取得
	if (_getcwd(szDir, _MAX_PATH) == NULL)
		return FALSE;
	MULTI_TO_WIDE(wszDir, szDir);

	//設定
	lpDMLoader->SetSearchDirectory(GUID_DirectMusicAllTypes, wszDir, FALSE);

	//------最適な出力ポートを選択する処理----------------------------//
	GUID guidSynthGUID;

	//デフォルトポートの取得
	ret = lpDMusic->GetDefaultPort(&guidSynthGUID);
	if (ret != S_OK)
		return FALSE;

	//デフォルトポートの作成
	LPDIRECTMUSICPORT lpDMPort = NULL;

	DMUS_PORTPARAMS dmos;

	ZeroMemory(&dmos, sizeof(dmos));
	dmos.dwSize = sizeof(DMUS_PORTPARAMS);
	dmos.dwChannelGroups = 1;
	dmos.dwValidParams = DMUS_PORTPARAMS_CHANNELGROUPS;

	ret = lpDMusic->CreatePort(guidSynthGUID,
							   &dmos,
							   &lpDMPort,
							   NULL);
	if (ret != S_OK)
	{
		RELEASE(lpDMPort);
		return FALSE;
	}

	//ポートの能力を取得
	DMUS_PORTCAPS dmpc;

	ZeroMemory(&dmpc, sizeof(dmpc));
	dmpc.dwSize = sizeof(DMUS_PORTCAPS);

	ret = lpDMPort->GetCaps(&dmpc);
	if (ret != S_OK)
	{
		RELEASE(lpDMPort);
		return FALSE;
	}

	//取得したポートの能力を調べる
	if ((dmpc.dwClass != DMUS_PC_OUTPUTCLASS) || //出力ポートではない
		!(dmpc.dwFlags & DMUS_PC_DLS))			 // DLSレベル1をサポートしていない
	{
		//ポートの能力が不足しているので作成したポートを解放
		RELEASE(lpDMPort);

		//条件に合うポートを列挙する
		ret = S_OK;

		for (int i = 0; ret == S_OK; i++)
		{
			ZeroMemory(&dmpc, sizeof(dmpc));
			dmpc.dwSize = sizeof(DMUS_PORTCAPS);

			// i番目のポートの情報を取得
			ret = lpDMusic->EnumPort(i, &dmpc);
			//情報の取得に成功したかどうかチェック
			if (SUCCEEDED(ret))
			{
				//成功した場合、条件に合うかどうかチェック
				if ((dmpc.dwClass != DMUS_PC_OUTPUTCLASS) || //出力ポートではない
					!(dmpc.dwFlags & DMUS_PC_DLS))			 // DLSレベル1をサポートしていない
				{
					//条件に合う場合ポートを作成する
					CopyMemory(&guidSynthGUID, &dmpc.guidPort, sizeof(GUID));

					ZeroMemory(&dmos, sizeof(dmos));
					dmos.dwSize = sizeof(DMUS_PORTPARAMS);
					dmos.dwChannelGroups = 1;
					dmos.dwValidParams = DMUS_PORTPARAMS_CHANNELGROUPS;

					ret = lpDMusic->CreatePort(guidSynthGUID, &dmos, &lpDMPort, NULL);
					if (ret != S_OK)
					{
						RELEASE(lpDMPort);
						return FALSE;
					}
					break;
				}
			}
		}
		//条件に合うポートが一つも見つからない場合
		if (FAILED(ret))
		{
			RELEASE(lpDMPort);
			return FALSE;
		}
	}

	ret = lpDMPort->Activate(TRUE);
	if (ret != S_OK)
		return FALSE;
	ret = lpDMPerf->AddPort(lpDMPort); //ポートの選択
	if (ret != S_OK)
		return FALSE;
	ret = lpDMPerf->AssignPChannelBlock(0, lpDMPort, 1);
	if (ret != S_OK)
		return FALSE;
	RELEASE(lpDMPort); //ポート解放

	//------セグメントの作成処理-----------------------------------//
	//再生する標準MIDIファイル名
	WCHAR *FileName[] = {
		L".\\Midi\\Title.mid",
		L".\\Midi\\Stage1.mid",
		L".\\Midi\\Continue.mid",
		L".\\Midi\\Boss.mid",
		L".\\Midi\\Ending.mid"};

	//セグメントのロード
	// DMUS_OBJECTDESC構造体に、ロードするオブジェクトについて記述する
	DMUS_OBJECTDESC ObjDesc;
	ObjDesc.dwSize = sizeof(DMUS_OBJECTDESC);				  //構造体のサイズを設定
	ObjDesc.guidClass = CLSID_DirectMusicSegment;			  // DirectMusicSegmentを指定
	ObjDesc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_FILENAME; //構造体の有効な情報を示すフラグを設定
	for (short i = 0; i < 5; i++)
	{
		wcscpy(ObjDesc.wszFileName, FileName[i]); //ファイル名を指定
		ret = lpDMLoader->GetObject(&ObjDesc, IID_IDirectMusicSegment2, (void **)&lpDMSegment[i]);
		if (ret != S_OK)
			return FALSE;
		//パラメーターの設定
		lpDMSegment[i]->SetParam(GUID_StandardMIDIFile, 0xFFFFFFFF, 0, 0, NULL);
		lpDMSegment[i]->SetParam(GUID_Download, 0xFFFFFFFF, 0, 0, (void *)lpDMPerf); // DLSコレクションのダウンロード
		//セグメントの長さの取得
		lpDMSegment[i]->GetLength(&SegLength[i]);
		SegEndTime[i] = SegLength[i];
		//ループ設定
		lpDMSegment[i]->SetLoopPoints(SegStartTime[i], SegEndTime[i]);
		lpDMSegment[i]->SetRepeats(10);
	}

	return TRUE;
}

//------------------------------------------------------------------
//	DirectMusicの終了処理
//------------------------------------------------------------------
void ReleaseDM()
{
	//セグメント演奏停止
	lpDMPerf->Stop(NULL, NULL, 0, 0);
	for (short i = 0; i < 5; i++)
	{
		// DLSのアンロード
		lpDMSegment[i]->SetParam(GUID_Unload, 0xFFFFFFFF, 0, 0, (void **)&lpDMPerf);
		//セグメント解放
		RELEASE(lpDMSegState[i]);
		RELEASE(lpDMSegment[i]);
	}
	//パフォーマンスの解放
	lpDMPerf->CloseDown();
	RELEASE(lpDMPerf);
	//ローダー解放
	RELEASE(lpDMLoader);
	//コンポサー解放
	RELEASE(lpDMComposer);
	// DirectMusic解放
	RELEASE(lpDMusic);
	// COM解放
	CoUninitialize();
}