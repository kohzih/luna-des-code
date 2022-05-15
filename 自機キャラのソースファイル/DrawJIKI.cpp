/*=============================================================================
	自機のメイン処理に関するソースファイル
==============================================================================*/
#include <windows.h>
#include <ddraw.h>
#include <dsound.h>

#include "Macros.h"
#include "Draw.h"
#include "Struct.h"
#include "DrawJIKI.h"
#include "DrawJIKIData.h"
#include "DrawMacros.h"

//ダイレクトサウンドバッファ再生用マクロ定義
#define _JIKIBAKU 0
#define _TEKIBAKU 1
#define _FUELARM 2
#define _FIRE 3
#define _TAMAKYU 4
#define _BOSSBAKU 5

extern LPDIRECTSOUNDBUFFER lpDSBuf[6]; //ダイレクトサウンドバッファ
extern LPDIRECTDRAWSURFACE7 lpDDSBack; //バックサーフェス

//ジョイスティク関連
BYTE JoyNew = 0, JoyOld = 0;
BYTE Joy_Get(void);

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DrawJIKI::DrawJIKI()
{
	InitJikiTm();
	InitWepnTm();
	InitHmmTm();
	InitHnbTm();
	InitOptAtkDatTm();
}

DrawJIKI::~DrawJIKI()
{
	OptRelease();
	HnbRelease();
	HmmRelease();
	MainWepnRelease();
	JikiTmRelease();
}

/////////////////////////////////////////////////////////////////////
// 自機の各パラメーターを初期化
/////////////////////////////////////////////////////////////////////
BOOL DrawJIKI::InitJIKI()
{
	//再初期化に必要な処理
	OptRelease();
	HnbRelease();
	HmmRelease();
	MainWepnRelease();
	JikiTmRelease();

	BOOL ret = TRUE;

	//自機に関する様々なオブジェクトの初期化
	ret = InitJikiData();
	if (!ret)
		return FALSE;

	//主砲から発射される弾オブジェクトの初期化
	ret = InitMainWepn();
	if (!ret)
		return FALSE;

	//ホーミングミサイルオブジェクトの初期化
	ret = InitHmm();
	if (!ret)
		return FALSE;

	//花火オブジェクトの初期化
	ret = InitHnb();
	if (!ret)
		return FALSE;

	//オプションオブジェクトの初期化
	ret = InitOpt();
	if (!ret)
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// 自機の移動と弾発射までのカウントダウン
/////////////////////////////////////////////////////////////////////
void DrawJIKI::MoveJIKI()
{
	//ステージフラグに何かフラグが立っている場合リターン
	if (fSTAGE & _fSTAGE_CHK)
	{
		lpDSBuf[_TAMAKYU]->Stop(); //ループしている弾吸収音の停止
		return;
	}
	//ゲームを一時停止させるフラグが立っている場合リターン
	if (fJIKI & _GAME_STOP_ON)
	{
		lpDSBuf[_TAMAKYU]->Stop(); //ループしている弾吸収音の停止
		return;
	}

	//自機撃墜フラグが立っている場合は移動処理を行わない
	if (!(fJIKI & _END_ON))
	{
		//ジョイスティックorキーポードからの入力を取得
		JoyOld = JoyNew;
		JoyNew = Joy_Get();

		//自機の移動
		if (JoyNew & JOY_UP)
		{					//上が押されている
			Cdn.y -= Mov.y; //移動量を加える
		}
		else if (JoyNew & JOY_DOWN)
		{ //下が押されている
			Cdn.y += Mov.y;
		}
		if (JoyNew & JOY_RIGHT)
		{ //右が押されている
			Cdn.x += Mov.x;
		}
		else if (JoyNew & JOY_LEFT)
		{ //左が押されている
			Cdn.x -= Mov.x;
		}

		//弾吸収中でない時にAが押されたら各種攻撃フラグをオン
		if (JoyNew & JOY_A && !(fJIKI & _FUEL_ON))
		{
			MainWepnCnt(); //主砲発射までのカウントを進める
			switch (Wpd.flag & _SUBWP_CHK)
			{ //現在選択されているサブウェポンの発射カウントを進める
			case _HMM_ON:
				HmmCnt(); //ホーミングミサイル
				break;
			case _HNB_ON:
				HnbCnt(); //花火
				break;
			default:
				break;
			}
			if (fJIKI & _OPT_ON)
				OptAttackCnt(); //オプションの弾発射のカウントを進める
			// Aが押された瞬間であった場合アーム発射フラグをオン
			if ((JoyNew ^ JoyOld) & (JoyNew & JOY_A))
				fJIKI |= _ARM_ON;
		} // Aが離された瞬間各攻撃用カウンタをリセット
		else if ((JoyNew ^ JoyOld) & (JoyOld & JOY_A))
		{
			//弾が画面に存在しなくなるまで_WEPN_ONフラグは解除しない
			MainWepnCntReset(); //主砲発射までのカウントリセット
			switch (Wpd.flag & _SUBWP_CHK)
			{ //現在選択されているサブウェポンの発射カウントをリセット
			case _HMM_ON:
				HmmCntReset(); //ホーミングミサイル
				break;
			case _HNB_ON:
				HnbCntReset(); //花火
			default:
				break;
			}
			if (fJIKI & _OPT_ON)
				OptAttackCntReset(); //オプションの弾発射のカウントをリセット
		}
		//弾吸収燃料がある時にBが押された瞬間
		if ((JoyNew ^ JoyOld) & (JoyNew & JOY_B) && FUEL_R)
		{
			fJIKI |= _FUEL_ON; //弾吸収中フラグをオン
			//弾吸収サウンド
			lpDSBuf[_TAMAKYU]->Play(0, 0, DSBPLAY_LOOPING);
			//弾吸収中フラグが立っている時にBが押されっぱなしである
		}
		else if (JoyNew & JoyOld & JOY_B && fJIKI & _FUEL_ON)
		{
			if (0 > FUEL_R - _FUELKY)
			{							   // FUEL値が0以下ならば
				FUEL_R = 0;				   // FUEL値0
				fJIKI &= _FUEL_OFF;		   //弾吸収中フラグオフ
				fJIKI |= _FUELD_ON;		   //弾吸収中断フラグオン
				lpDSBuf[_TAMAKYU]->Stop(); //ループしている弾吸収音の停止
			}
			else
			{
				FUEL_R -= _FUELKY; // FUEL値が0でないならBを押している間FUELゲージを減らす
			}
			FUEL = (BYTE)(FUEL_R >> 16); // FUELに現在の燃料の値をセット
			//弾吸収中フラグが立っている時にBが離された
		}
		else if ((JoyNew ^ JoyOld) & (JoyOld & JOY_B) && fJIKI & _FUEL_ON)
		{
			fJIKI &= _FUEL_OFF;		   //弾吸収中フラグオフ
			fJIKI |= _FUELD_ON;		   //弾吸収中断フラグオン
			lpDSBuf[_TAMAKYU]->Stop(); //ループしている弾吸収音の停止
		}

		//オプションの移動
		if (fJIKI & _OPT_ON)
		{
			OptMovCnt(JoyNew); //オプションの移動用に現在の自機の座標を取得
			OptMov();		   //オプションの移動
		}
	}
	else
	{
		lpDSBuf[_TAMAKYU]->Stop(); //ループしている弾吸収音の停止
	}

	//自機画面座標の補正
	if (Cdn.x < 0)
		Cdn.x = 0;
	else if (Cdn.x + _JIKI_W > WinXMax)
		Cdn.x = WinXMax - _JIKI_W;
	if (Cdn.y < 0)
		Cdn.y = 0;
	else if (Cdn.y + 32 > WinYMax)
		Cdn.y = WinYMax - 32;

	//現在の絶対座標を求める
	MapCdn.x = MainCdn.x + Cdn.x;
	MapCdn.y = MainCdn.y + Cdn.y;
}

///////////////////////////////////////////////////////////////////////////////////
// 敵キャラ、弾とのあたり判定を調べて接触している場合は敵、自機両方のフラグをセット
// ここでの戻り値は敵キャラの状態を変更させます。
// 敵弾は自機に接触した場合、敵弾状態フラグはここで変更しています。
////////////////////////////////////////////////////////////////////////////////////
DWORD DrawJIKI::AtariJIKI()
{
	//ステージフラグに何かフラグが立っている場合リターン
	if (fSTAGE & _fSTAGE_CHK)
		return SP_fSPChr;
	//自機状態フラグにゲーム一時停止フラグが立っている場合リターン
	if (fJIKI & _GAME_STOP_ON)
		return SP_fSPChr;
	//自機に撃墜フラグの立っている場合は当たり判定を行わない
	if (fJIKI & _END_ON)
		return SP_fSPChr;

	//指定された敵キャラが破壊されている場合ゲーム終了
	if (SP_Num == Target)
	{
		if ((SP_fSPChr & _SPDESCHK) == (DWORD)_SPDES)
		{
			fJIKI &= _BOSS_OFF;			 //ボスと戦闘中フラグオフ
			fSTAGE |= _STAGE_ENDWAIT_ON; //ステージ終了フラグオン
			fSTAGE |= _GAME_CLEAR_ON;	 //ステージクリアフラグオン
			return SP_fSPChr;
		}
	}

	//画面に弾が存在している場合、自機と敵の弾との当たり判定を行う
	JikiTekitamaAtari();
	if (fJIKI & _END_ON)
		return SP_fSPChr; //自機が撃墜された場合リターン

	//敵キャラの表示フラグが立っていない場合はリターン
	if (!(SP_fSPChr & _SPON))
		return SP_fSPChr;
	else if (SP_fSPChr & _SPDES)
		return SP_fSPChr;
	//自機フラグに敵キャラ消滅が立っている場合画面上の敵キャラに一定のダメージ
	else if (fJIKI & _PWUPD_ON)
	{
		JikiTekiDmg(_PWUPDDMG);
		//敵キャラの破壊フラグが立っている場合はリターン
		if (SP_fSPChr & _SPDES)
			return SP_fSPChr;
	}

	//ボスキャラが画面に出現しているかどうかチェック
	if (SP_Num == Target)
	{
		if (!(fJIKI & _TARGET_ON) && !(fJIKI & _BOSS_ON))
		{
			fJIKI |= _TARGET_ON; //ボスキャラ出現フラグオン
			fJIKI |= _BOSS_ON;	 //ボス戦に突入したことを示すフラグオン
		}
		else if (fJIKI & _TARGET_ON && fJIKI & _BOSS_ON)
		{
			fJIKI &= _TARGET_OFF; //ボスキャラ出現フラグオフ
		}
	}

	//ホーミングミサイル発射用にロックオンするために必要な敵キャラの情報を取得する
	HmmGetTekiInfo();

	//敵キャラと主砲の弾との当たり判定
	if (fJIKI & _WEPN_ON)
	{
		MainWepnAtari();
		if (SP_fSPChr & _SPDES)
			return SP_fSPChr;
	}
	//敵キャラとホーミングミサイルとの当たり判定と追尾用の移動量の取得
	if (fJIKI & _SUBHMM_ON)
	{
		HmmMovCnt(); //敵キャラ追尾用の移動量の取得
		HmmAtari();	 //敵キャラとの当たり判定
		if (SP_fSPChr & _SPDES)
			return SP_fSPChr;
	}
	//敵キャラと花火との当たり判定
	if (fJIKI & _SUBHNB_ON)
	{
		HnbAtari();
		if (SP_fSPChr & _SPDES)
			return SP_fSPChr;
	}
	//敵キャラとオプションの発射した弾との当たり判定
	if (fJIKI & _OPTWEP_ON)
	{
		OptAttackAtari();
		if (SP_fSPChr & _SPDES)
			return SP_fSPChr;
	}

	//敵キャラと自機との当たり判定
	JikiTekiAtari();

	return SP_fSPChr;
}

//////////////////////////////////////////////////////////////////////
// 自機に関するグラフィックの描画に使用する各値を更新する
//////////////////////////////////////////////////////////////////////
void DrawJIKI::FrameJIKI()
{
	//自機の状態フラグをチェックして自機の矩形範囲をセット

	//--------ステージフラグに何かフラグが立っている場合リターン-----
	if (fSTAGE & _fSTAGE_CHK)
		return;
	//------ゲームを一時停止させるフラグが立っている場合リターン-----
	if (fJIKI & _GAME_STOP_ON)
		return;

	//---------自機が撃墜されている場合爆発シーンの処理--------------
	if (fJIKI & _END_ON)
	{
		JikiBakuFrame();
		return;
	}
	//---------通常状態の自機の矩形範囲をセット ---------------------
	if (!fJIKI || !(fJIKI & 0x0000001F))
	{
		JIKIChr = SpJIKI[0];
	}

	//----------アーム発射中のアニメーション処理---------------------
	if (fJIKI & _ARM_ON)
		JikiArmFrame();

	//----------主砲の弾の発射、移動、アニメーション処理 ------------
	MainWepn();

	//----ホーミングミサイルの弾の発射、移動、アニメーション処理-----
	HmmAttack();

	//----------花火の発射、移動、アニメーション処理-----------------
	HnbAttack();

	//----------オプションの弾の発射、移動、アニメーション処理 ------
	if (fJIKI & _OPT_ON)
		OptAttack();

	//----------弾吸収中の自機アニメーション処理---------------------
	JikiTmkyFrame();

	// FUELゲージのサーフェスからの転送矩形範囲を現在のFUELのサイズに指定
	GAGE_F.left = SpJIKI[22].left + (128 - FUEL);
	GAGE_F.top = SpJIKI[22].top;
	GAGE_F.right = SpJIKI[22].right;
	GAGE_F.bottom = SpJIKI[22].bottom;

	// EXPゲージのサーフェスからの転送矩形範囲を現在のEXPのサイズに指定
	GAGE_E.left = SpJIKI[19].left;
	GAGE_E.top = SpJIKI[19].top;
	GAGE_E.right = SpJIKI[19].right - (128 - EXP);
	GAGE_E.bottom = SpJIKI[19].bottom;

	//スコア表示のための矩形範囲を設定
	ScoreCalcul();
}

//////////////////////////////////////////////////////////////////////
// 自機と各種画面ステータスの描画
//////////////////////////////////////////////////////////////////////
void DrawJIKI::RendJIKI()
{
	//ステージフラグに何か立っている場合はリターン
	if (fSTAGE & _fSTAGE_CHK)
		return;

	//--自機が撃墜され爆発するアニメーションとコンティニュー画面の描画--
	if (fJIKI & _END_ON)
	{
		//コンティニュー画面
		if (fJIKI & _GAME_STOP_ON)
		{
			RendCONTINUE();
		}
		else if (fJIKI & _CONTINUE_ON)
		{
			fJIKI &= _CONTINUE_OFF; //コンティニューに伴う初期化処理終了
			fJIKI &= _END_OFF;		//自機の撃墜フラグオフ
			//爆発シーン
		}
		else
		{
			lpDDSBack->BltFast(Baku.DesR.left, Baku.DesR.top,
							   lpDDS_JIKI, &Baku.SrcR,
							   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		}
	}
	else
	{
		//------------- オプションが装備されている時の転送 -------------
		if (fJIKI & _OPT_ON)
			OptRend();

		//自機が無敵状態の時に点滅させる処理
		if (fJIKI & _INVINCIBLE_ON && !(fJIKI & _GAME_STOP_ON))
		{
			if (!(InvincCnt--))
			{ //無敵時間を減らす
				InvincCnt = 0;
				fJIKI &= _INVINCIBLE_OFF;
			}
		}

		if ((InvincCnt % 6) < 3)
		{
			//------------------ 通常の状態の転送 -------------------------
			if (!fJIKI || !(fJIKI & 0x0000001F))
			{
				lpDDSBack->BltFast(Cdn.x, Cdn.y,
								   lpDDS_JIKI, &JIKIChr,
								   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			}
			//------------------ アーム発射中の転送-------------------------
			else if (fJIKI & _ARM_ON)
			{
				lpDDSBack->BltFast(ArmJIKI.DesR.left, ArmJIKI.DesR.top,
								   lpDDS_JIKI, &ArmJIKI.SrcR,
								   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
				lpDDSBack->BltFast(Arm.DesR.left, Arm.DesR.top,
								   lpDDS_JIKI, &Arm.SrcR,
								   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			}
			//--------------------- 弾吸収中の転送 -------------------------
			if (fJIKI & _FUEL_ON)
			{
				lpDDSBack->BltFast(TmKySy.DesR.left, TmKySy.DesR.top,
								   lpDDS_JIKI, &TmKySy.SrcR,
								   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			}
		}

		//-------------- 画面に弾が存在している時の転送 ----------------
		if (fJIKI & _WEPN_ON)
			MainWepnRend(); //主砲の弾の描画
		if (fJIKI & _SUBHMM_ON)
			HmmRend(); //ホーミングミサイルの描画
		if (fJIKI & _SUBHNB_ON)
			HnbRend(); //花火の描画処理
		if (fJIKI & _OPTWEP_ON)
			OptWepRend(); //オプションの発射した弾の描画
	}

	// FUELゲージの空白部分の転送
	lpDDSBack->BltFast(506, 0,
					   lpDDS_JIKI, &GAGE_FK,
					   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	// FUELゲージの部分の転送
	lpDDSBack->BltFast(506 + (128 - FUEL), 0,
					   lpDDS_JIKI, &GAGE_F,
					   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	// FUELゲージの枠部分の転送
	lpDDSBack->BltFast(512, 0,
					   lpDDS_JIKI, &GAGE_FW,
					   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	// EXPゲージの空白部分の転送
	lpDDSBack->BltFast(6, 0,
					   lpDDS_JIKI, &GAGE_EK,
					   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	// EXPゲージの部分の転送
	lpDDSBack->BltFast(6, 0,
					   lpDDS_JIKI, &GAGE_E,
					   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	// EXPゲージの枠部分の転送
	lpDDSBack->BltFast(0, 0,
					   lpDDS_JIKI, &GAGE_EW,
					   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	//スコア部分の転送
	for (int i = 0, j = 248; i < 9; i++, j += 16)
	{
		lpDDSBack->BltFast(j, 2,
						   lpDDS_JIKI, &SCORE_R[i],
						   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}

	/*--- パワーアップ画面処理中もしくは敵キャラ全滅フラグの立っている場合
	パワーアップ処理関数に飛ぶ--------------------------------------*/
	if (fJIKI & _PWUP_ON || fJIKI & _PWUPD_ON)
	{
		RendPWUP(); //自機のパワーアップ処理画面を描画する
	}
}

//////////////////////////////////////////////////////////////////////
// 自機の状態フラグの値を戻り値として返す関数
//////////////////////////////////////////////////////////////////////
DWORD DrawJIKI::GetfJIKI()
{
	return fJIKI;
}

//////////////////////////////////////////////////////////////////////
// 自機の画面座標の値を戻り値として返す関数
//////////////////////////////////////////////////////////////////////
POINT DrawJIKI::GetJIKICdn()
{
	return Cdn;
}
