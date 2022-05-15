/*====================================================================
	各ステージデータの初期化に関するソースファイル
=====================================================================*/

#include <windows.h>
#include <ddraw.h>
#include <dmusici.h>

#include "Macros.h"
#include "FaInOut.h"
#include "Draw.h"
#include "DrawBG.h"
#include "Struct.h"
#include "DrawSP.h"
#include "DrawJIKI.h"
#include "DrawMacros.h"

extern HWND hWnd; //メインウィンドウハンドル

extern IDirectMusicPerformance *lpDMPerf;
extern IDirectMusicComposer *lpDMComposer;
//曲を管理するマクロ
#define _TITLE 0
#define _STAGE1 1
#define _CONTINUE 2
#define _BOSS 3
#define _ENDING 4

extern IDirectMusicSegment *lpDMSegment[5];		  //セグメント
extern IDirectMusicSegmentState *lpDMSegState[5]; //セグメントの状態
extern MUSIC_TIME SegStartTime[5];

extern FaInOut *lpFInOut;
extern Draw *lpDraw;
extern DrawSP *lpSP;
extern DrawJIKI *lpJIKI;
extern DrawBG *lpBG;

//ジョイスティク関連
extern BYTE JoyNew, JoyOld;
BYTE Joy_Get(void);

#if DEBUG >= 2
extern LPDIRECTDRAW7 lpDD;		// DirectDrawオブジェクト
extern DWORD TotalMem, FreeMem; // VRAMの空き容量の値を格納
#endif

//////////////////////////////////////////////////////////////////////
// 各ステージの開始、終了、イベント等の初期化を行なう
//////////////////////////////////////////////////////////////////////
BOOL InitSTAGE()
{

	int i, j;
	long Vol = 0; // BGMのマスターボリュームの増幅、減衰レベルの値
	BOOL ret = TRUE;
	static int WaitCnt = 0;

	//------------------------------------------------------------------//
	//				ステージ終了処理の準備								//
	//------------------------------------------------------------------//
	if (lpDraw->GetfSTAGE() & _STAGE_ENDWAIT_ON)
	{
		lpDraw->SetfSTAGE(_STAGE_ENDWAIT_OFF, 0); //ステージ終了合図フラグオフ
		lpDraw->SetfSTAGE(_STAGE_END_ON, 1);	  //ステージ終了処理中フラグオン

		switch (lpDraw->GetStageNum())
		{
		case 0: //プロローグ画面
			break;

		case 1: //タイトル画面
				// BGM停止
			lpDMPerf->Stop(lpDMSegment[_TITLE], lpDMSegState[_TITLE], 0, DMUS_SEGF_DEFAULT);
			break;

		case 2: //ステージ1
			//ゲームオーバーになった場合
			if (lpDraw->GetfSTAGE() & _GAME_OVER_ON)
			{
				lpFInOut->SetFade(_SetDef); //元の色に戻す
				//ゲームをクリアしている場合
			}
			else if (lpDraw->GetfSTAGE() & _GAME_CLEAR_ON)
			{
				// EndingBGM再生開始
				lpDMSegment[_ENDING]->SetStartPoint(0);
				lpDMComposer->AutoTransition(
					lpDMPerf, lpDMSegment[_ENDING], DMUS_COMMANDT_END, DMUS_SEGF_DEFAULT,
					NULL, NULL, &lpDMSegState[_ENDING], NULL);
			}
			break;

		case 1000: //ゲームオーバー画面
			// BGM停止
			lpDMPerf->Stop(lpDMSegment[_CONTINUE], lpDMSegState[_CONTINUE], 0, DMUS_SEGF_DEFAULT);
			break;

		case 2000: //ゲームクリア画面
			// BGM停止
			lpDMPerf->Stop(lpDMSegment[_ENDING], lpDMSegState[_ENDING], 0, DMUS_SEGF_DEFAULT);
			break;

		default:
			return FALSE;
			break;
		}
		return TRUE;
	}

	//------------------------------------------------------------------//
	//					ステージの終了処理								//
	//------------------------------------------------------------------//
	if (lpDraw->GetfSTAGE() & _STAGE_END_ON)
	{
		switch (lpDraw->GetStageNum())
		{
		case 0: //プロローグ画面
			ret = lpFInOut->BlackOut(0x00020000);
			//				ret=lpFInOut->BlackOut(0x01020000);
			//ブラックアウトが終了した場合、ステージ終了処理中フラグオフ
			if (ret)
			{
				lpDraw->SetfSTAGE(_STAGE_END_OFF, 0);
				lpDraw->SetfSTAGE(_STAGE_STARTWAIT_ON, 1); //ステージ開始フラグオン
				lpBG->SetfBG(0, 0);						   // BGフラグクリア
				lpDraw->SetStageNum(1);					   //ステージ番号を進める処理
			}
			else
			{
				return TRUE;
			}
			break;

		case 1: //タイトル画面
			ret = lpFInOut->BlackOut(0x00020000);
			//				ret=lpFInOut->BlackOut(0x01020000);
			//ブラックアウトが終了した場合、ステージ終了処理中フラグオフ
			if (ret)
			{
				lpDraw->SetfSTAGE(_STAGE_END_OFF, 0);
				lpDraw->SetfSTAGE(_STAGE_STARTWAIT_ON, 1); //ステージ開始フラグオン
				lpBG->SetfBG(0, 0);						   // BGフラグクリア
				lpDraw->SetStageNum(2);
			}
			else
			{
				return TRUE;
			}
			break;

		case 2: //ステージ1
			//ゲームオーバーで終了した場合
			if (lpDraw->GetfSTAGE() & _GAME_OVER_ON)
			{
				ret = lpFInOut->BlackOut(0x00050000);
				//					ret=lpFInOut->BlackOut(0x01050000);
				if (ret)
				{
					lpDraw->SetfSTAGE(_STAGE_END_OFF, 0);
					lpDraw->SetfSTAGE(_STAGE_STARTWAIT_ON, 1); //ステージ開始フラグオン
					lpBG->SetfBG(0, 0);						   // BGフラグクリア
					lpDraw->SetStageNum(1000);
				}
				else
				{
					return TRUE;
				}
				//ゲームクリアで終了した場合
			}
			else if (lpDraw->GetfSTAGE() & _GAME_CLEAR_ON)
			{
				if (WaitCnt++ > 200)
				{
					//					if(WaitCnt++>2){
					ret = lpFInOut->WhiteOut(0x00005000);
					//						ret=lpFInOut->WhiteOut(0x01005000);
					if (ret)
					{
						lpDraw->SetfSTAGE(_STAGE_END_OFF, 0);
						lpDraw->SetfSTAGE(_STAGE_STARTWAIT_ON, 1); //ステージ開始フラグオン
						lpBG->SetfBG(0, 0);
						lpDraw->SetStageNum(2000);
						WaitCnt = 0;
					}
					else
					{
						return TRUE;
					}
				}
			}
			break;

		case 1000: //ゲームオーバー画面
			ret = lpFInOut->BlackOut(0x00050000);
			//				ret=lpFInOut->BlackOut(0x01050000);
			//ブラックアウトが終了した場合、ステージ終了処理中フラグオフ
			if (ret)
			{
				lpDraw->SetfSTAGE(_GAME_OVER_OFF, 0); //ゲームオーバーフラグオフ
				lpDraw->SetfSTAGE(_STAGE_END_OFF, 0);
				lpDraw->SetfSTAGE(_STAGE_STARTWAIT_ON, 1); //ステージ開始フラグオン
				lpBG->SetfBG(0, 0);						   // BGフラグクリア
				lpDraw->SetStageNum(1);					   //ステージ番号を進める処理
			}
			else
			{
				return TRUE;
			}
			break;

		case 2000: //ゲームクリア画面
			ret = lpFInOut->BlackOut(0x00050000);
			//				ret=lpFInOut->BlackOut(0x01050000);
			//ブラックアウトが終了した場合、ステージ終了処理中フラグオフ
			if (ret)
			{
				lpDraw->SetfSTAGE(_GAME_CLEAR_OFF, 0); //ゲームクリアフラグオフ
				lpDraw->SetfSTAGE(_STAGE_END_OFF, 0);
				lpDraw->SetfSTAGE(_STAGE_STARTWAIT_ON, 1); //ステージ開始フラグオン
				lpBG->SetfBG(0, 0);						   // BGフラグクリア
				lpDraw->SetStageNum(1);					   //ステージ番号を進める処理
			}
			else
			{
				return TRUE;
			}
			break;

		default:
			return FALSE;
			break;
		}
	}

	//------------------------------------------------------------------//
	//				ステージ開始処理の準備								//
	//------------------------------------------------------------------//

	if (lpDraw->GetfSTAGE() & _STAGE_STARTWAIT_ON)
	{
		lpDraw->SetfSTAGE(_STAGE_STARTWAIT_OFF, 0); //ステージ開始合図フラグオフ
		lpDraw->SetfSTAGE(_STAGE_START_ON, 1);		//ステージ開始処理中フラグオン

		//メインのゲーム処理を一切行わないことを示すフラグオン
		lpDraw->SetfSTAGE(_GAME_JUMP_ON, 1);
		// Drawオブジェクトを初期化
		ret = lpDraw->Init();
		if (!ret)
			return FALSE;
		//背景オブジェクトを初期化
		ret = lpBG->InitBG();
		if (!ret)
			return FALSE;

		DWORD MsX = 0, MsY = 0, CsX = 0, CsY = 0; //マップデータ格納用
		WORD Event = 0;							  //イベントデータ格納用

		//ゲーム開始時の準備
		switch (lpDraw->GetStageNum())
		{
		case 0: //プロローグ画面
			//ブラックイン準備
			lpFInOut->SetFade(_SetBlackIn);
			break;

		case 1: //タイトル画面
			lpFInOut->SetFade(_SetBlackIn);
			// BGM再生開始
			lpDMSegment[_TITLE]->SetStartPoint(0);
			lpDMPerf->PlaySegment(lpDMSegment[_TITLE], DMUS_SEGF_DEFAULT, 0, &lpDMSegState[_TITLE]);
			break;

		case 2: //ステージ1
			lpFInOut->SetFade(_SetBlackIn);
			// BGM再生開始
			lpDMSegment[_STAGE1]->SetStartPoint(0);
			lpDMPerf->PlaySegment(lpDMSegment[_STAGE1], DMUS_SEGF_DEFAULT, 0, &lpDMSegState[_STAGE1]);

			//メインのゲーム処理を一切行わないことを示すフラグオフ
			lpDraw->SetfSTAGE(_GAME_JUMP_OFF, 0);
			//ステージ1タイトル文字表示
			lpBG->SetfBG(_SetBG00_ON, 1);
			//自機オブジェクトを初期化
			ret = lpJIKI->InitJIKI();
			if (!ret)
				return FALSE;

			//----------スプライトオブジェクトを初期化-------------//

			//マップの縦横の大きさとチップサイズを求める
			lpDraw->GetMapSizeData(MsX, MsY, CsX, CsY);

			//確保するスプライトオブジェクトの数を求める
			// Parts構造体配列内のイベントの数を保存
			for (i = 0; (DWORD)i < MsX * MsY; i++)
			{
				lpDraw->GetEventData(i, Event);
				if (Event)
					lpDraw->EventCnt++;
			}

			//イベントの数だけスプライト管理オブジェクトの作成(イベント数が0の場合は処理しない)
			if (lpDraw->EventCnt == 0)
			{
				RELEASE_dels(lpSP);
			}
			else
			{
				lpSP = new DrawSP[lpDraw->EventCnt];
				if (!lpSP)
					return FALSE;

				// BGMAP構造体配列(Parts)を一つずつイベントが無いか調べて、
				//イベントの配置されている位置でオブジェクトを初期化

				POINT cdn; //イベントの座標用

				for (i = 0, j = 0; (DWORD)i < MsX * MsY; i++)
				{
					lpDraw->GetEventData(i, Event);
					if (Event)
					{
						//パーツの番号から絶対座標のどこでイベントが起こっているのか計算
						cdn.x = (i % MsX) * CsX;
						cdn.y = (i / MsX) * CsY;

						//スプライトオブジェクトを初期化
						ret = lpSP[j].InitSP(cdn, Event);
						if (!ret)
							return FALSE;
						//次ぎの初期化待ちスプライトオブジェクトをセット
						j++;
					}
				}
			}
			break;

		case 1000: //ゲームオーバー画面
			//ブラックイン準備
			lpFInOut->SetFade(_SetBlackIn);
			break;
		case 2000: //ゲームクリア画面
			//ホワイトイン準備
			lpFInOut->SetFade(_SetWhiteIn);
			break;
		default:
			return FALSE;
			break;
		}
		return TRUE;
	}

	//------------------------------------------------------------------//
	//						ステージの開始処理							//
	//------------------------------------------------------------------//
	if (lpDraw->GetfSTAGE() & _STAGE_START_ON)
	{
		//ステージがスタートするまでの処理
		switch (lpDraw->GetStageNum())
		{
		case 0: //プロローグ画面
			//ブラックインが終了した場合、ステージ開始処理中フラグオフ
			ret = lpFInOut->BlackIn(0x00020000);
			//				ret=lpFInOut->BlackIn(0x01020000);
			if (ret)
				lpDraw->SetfSTAGE(_STAGE_START_OFF, 0);
			break;

		case 1: //タイトル画面
			//ブラックインが終了した場合、ステージ開始処理中フラグオフ
			ret = lpFInOut->BlackIn(0x00010000);
			//				ret=lpFInOut->BlackIn(0x01010000);
			if (ret)
				lpDraw->SetfSTAGE(_STAGE_START_OFF, 0);
			break;

		case 2: //ステージ1
			ret = lpFInOut->BlackIn(0x00020000);
			//				ret=lpFInOut->BlackIn(0x01020000);
			if (ret)
			{
				if (WaitCnt++ > 200)
				{
					//					if(WaitCnt++>2){
					//ステージ1オープニング文字表示オフ
					lpBG->SetfBG(_SetBG00_OFF, 0);
					lpDraw->SetfSTAGE(_STAGE_START_OFF, 0);
					WaitCnt = 0;
				}
			}
			break;
		case 1000: //ゲームオーバー画面
			ret = lpFInOut->BlackIn(0x00050000);
			//				ret=lpFInOut->BlackIn(0x01050000);
			if (ret)
				lpDraw->SetfSTAGE(_STAGE_START_OFF, 0);
			break;
		case 2000: //ゲームクリア画面
			ret = lpFInOut->WhiteIn(0x00008000);
			//				ret=lpFInOut->WhiteIn(0x01008000);
			if (ret)
				lpDraw->SetfSTAGE(_STAGE_START_OFF, 0);
			break;
		default:
			return FALSE;
			break;
		}
		return TRUE;
	}

	//------------------------------------------------------------------//
	//				各ステージ内でのイベント処理						//
	//------------------------------------------------------------------//
	switch (lpDraw->GetStageNum())
	{
	case 0: //プロローグ画面
		if (!(lpDraw->GetfSTAGE() & _fSTAGE_CHK_InitSTAGE))
		{
			//画面をしばらく停止させる
			if (WaitCnt++ > 200)
			{
				//				if(WaitCnt++>2){
				WaitCnt = 0;
				lpDraw->SetfSTAGE(_STAGE_ENDWAIT_ON, 1);
			}
		}
		break;

	case 1: //タイトル画面
		if (!(lpDraw->GetfSTAGE() & _fSTAGE_CHK_InitSTAGE))
		{
			//ジョイスティックorキーポードからの入力を取得
			JoyOld = JoyNew;
			JoyNew = Joy_Get();
			if ((JoyNew ^ JoyOld) & (JoyNew & JOY_UP))
			{ //上が押された
				if (!(lpBG->GetfBG() & _SetBG00_ON))
					lpBG->SetfBG(_SetBG00_ON, 1);
				else
					lpBG->SetfBG(_SetBG00_OFF, 0);
			}
			else if ((JoyNew ^ JoyOld) & (JoyNew & JOY_DOWN))
			{ //下が押された
				if (!(lpBG->GetfBG() & _SetBG00_ON))
					lpBG->SetfBG(_SetBG00_ON, 1);
				else
					lpBG->SetfBG(_SetBG00_OFF, 0);
			}
			// Aが押すとゲームスタート
			if ((JoyNew ^ JoyOld) & (JoyNew & JOY_A))
			{
				if (lpBG->GetfBG() & _SetBG00_ON)
					lpDraw->SetfSTAGE(_GAME_EXIT, 1);
				else
					lpDraw->SetfSTAGE(_STAGE_ENDWAIT_ON, 1);
			}
		}
		break;

	case 2: //ステージ1
		//ゲームを一時停止させるフラグが立っているかどうかチェック
		if (!(lpDraw->GetfSTAGE() & _fSTAGE_CHK_InitSTAGE) && lpJIKI->GetfJIKI() & _GAME_STOP_ON)
		{

			//自機がパワーアップ画面処理中の場合画面を少し暗くする
			if (lpJIKI->GetfJIKI() & _PWUP_ON)
			{
				ret = lpFInOut->LittleBlack(0x00140000, 0); //画面を少し暗くする
				if (!ret)
				{
					// BGMのボリュームを絞る
					Vol = -1000;
					lpDMPerf->SetGlobalParam(GUID_PerfMasterVolume, &Vol, (DWORD)sizeof(long));
				}
			}

			//コンティニュー画面時の処理
			if (lpJIKI->GetfJIKI() & _END_ON)
			{
				//画面を少し暗くする
				ret = lpFInOut->LittleBlack(0x00140000, 0);
				if (!ret)
				{
					// BGMを一時停止させる
					if (lpJIKI->GetfJIKI() & _BOSS_ON)
					{														//ボス戦
						lpDMSegState[_BOSS]->GetSeek(&SegStartTime[_BOSS]); //停止された位置を取得
						lpDMPerf->Stop(lpDMSegment[_BOSS], lpDMSegState[_BOSS], 0, DMUS_SEGF_DEFAULT);
					}
					else
					{ //通常シーン
						lpDMSegState[_STAGE1]->GetSeek(&SegStartTime[_STAGE1]);
						lpDMPerf->Stop(lpDMSegment[_STAGE1], lpDMSegState[_STAGE1], 0, DMUS_SEGF_DEFAULT);
					}
					//コンティニュー画面のBGMを再生
					lpDMSegment[_CONTINUE]->SetStartPoint(0);
					lpDMPerf->PlaySegment(lpDMSegment[_CONTINUE], DMUS_SEGF_DEFAULT | DMUS_SEGF_SECONDARY, 0, &lpDMSegState[_CONTINUE]);
				}
			}
		}

		//パワーアップ画面が終了したら画面と音を元に戻す処理
		if (lpJIKI->GetfJIKI() & _PWUPD_ON)
		{
			lpFInOut->SetFade(_SetDef); //元の色に戻す
			// BGMのボリュームを元のレベルに戻す
			Vol = 0;
			lpDMPerf->SetGlobalParam(GUID_PerfMasterVolume, &Vol, (DWORD)sizeof(long));
		}

		//コンティニューが選択された場合画面と音を元に戻す処理
		if (lpJIKI->GetfJIKI() & _CONTINUE_ON)
		{
			lpFInOut->SetFade(_SetDef); //元の色に戻す
			//コンティニューシーンのBGM停止
			lpDMPerf->Stop(lpDMSegment[_CONTINUE], lpDMSegState[_CONTINUE], 0, DMUS_SEGF_DEFAULT);
			// BGM再開
			if (lpJIKI->GetfJIKI() & _BOSS_ON)
			{ //ボス戦
				lpDMSegment[_BOSS]->SetStartPoint(SegStartTime[_BOSS]);
				lpDMPerf->PlaySegment(lpDMSegment[_BOSS], DMUS_SEGF_DEFAULT, 0, &lpDMSegState[_BOSS]);
			}
			else
			{
				lpDMSegment[_STAGE1]->SetStartPoint(SegStartTime[_STAGE1]);
				lpDMPerf->PlaySegment(lpDMSegment[_STAGE1], DMUS_SEGF_DEFAULT, 0, &lpDMSegState[_STAGE1]);
			}
		}

		//ボスキャラが現れたらBGM変更
		if (lpJIKI->GetfJIKI() & _TARGET_ON)
		{
			lpDMSegment[_BOSS]->SetStartPoint(0);
			lpDMComposer->AutoTransition(
				lpDMPerf, lpDMSegment[_BOSS], DMUS_COMMANDT_END, DMUS_SEGF_DEFAULT,
				NULL, NULL, &lpDMSegState[_BOSS], NULL);
		}
		break;
	case 1000: //ゲームオーバー画面
		if (!(lpDraw->GetfSTAGE() & _fSTAGE_CHK_InitSTAGE))
		{
			//画面をしばらく停止させる
			if (WaitCnt++ > 100)
			{
				//				if(WaitCnt++>1){
				WaitCnt = 0;
				lpDraw->SetfSTAGE(_STAGE_ENDWAIT_ON, 1);
			}
		}
		break;
	case 2000: //ゲームクリア画面
		if (!(lpDraw->GetfSTAGE() & _fSTAGE_CHK_InitSTAGE))
		{
			//ジョイスティックorキーポードからの入力を取得
			JoyOld = JoyNew;
			JoyNew = Joy_Get();
			// Aが押すタイトル画面へ戻る
			if ((JoyNew ^ JoyOld) & (JoyNew & JOY_A))
			{
				WaitCnt = 0;
				lpDraw->SetfSTAGE(_STAGE_ENDWAIT_ON, 1);
			}
		}
		break;
	default:
		break;
	}

	//オフスクリーンサーフェスのロストチェック
	ret = lpDraw->RestoreJIKI();
	if (!ret)
	{
		MessageBox(hWnd, "RestoreJIKI", "ERROR", MB_OK | MB_APPLMODAL);
		return FALSE;
	}
	ret = lpDraw->RestoreSP();
	if (!ret)
	{
		MessageBox(hWnd, "RestoreSP", "ERROR", MB_OK | MB_APPLMODAL);
		return FALSE;
	}
	ret = lpDraw->RestoreBG();
	if (!ret)
	{
		MessageBox(hWnd, "RestoreBG", "ERROR", MB_OK | MB_APPLMODAL);
		return FALSE;
	}

// VRAMの空き容量を調べる
#if DEBUG >= 2
	DDCAPS hwCaps, swCaps;
	memset(&hwCaps, 0, sizeof(DDCAPS));
	hwCaps.dwSize = sizeof(DDCAPS);
	memset(&swCaps, 0, sizeof(DDCAPS));
	swCaps.dwSize = sizeof(DDCAPS);
	lpDD->GetCaps(&hwCaps, &swCaps);
	TotalMem = hwCaps.dwVidMemTotal;
	FreeMem = hwCaps.dwVidMemFree;
#endif

	return TRUE;
}