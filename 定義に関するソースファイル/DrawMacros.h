/*=============================================================================
	描画管理、自機、敵キャラ、背景、パレット管理クラス用のマクロ定義
==============================================================================*/
//スタートするステージ番号
#define	_StNum		0			//0:プロローグ画面、1:タイトル画面、2:ステージ1

//ステージの開始と終了を表すマクロ(Draw.fSTAGEフラグ用)
#define _STAGE_STARTWAIT_ON		0x00000001		//次のステージが始まったことを示すフラグ
#define _STAGE_STARTWAIT_OFF	0xFFFFFFFE		//次のステージが始まったことを示すフラグオフ
#define	_STAGE_START_ON			0x00000002		//次のステージの開始処理中であることを示すフラグ
#define _STAGE_START_OFF		0xFFFFFFFD		//次のステージの開始処理中であることを示すオフ
#define	_STAGE_ENDWAIT_ON		0x00000004		//ステージが終了したことを示すフラグ
#define	_STAGE_ENDWAIT_OFF		0xFFFFFFFB		//ステージが終了したことを示すフラグオフ
#define	_STAGE_END_ON			0x00000008		//ステージの終了処理中であることを示すフラグ
#define	_STAGE_END_OFF			0xFFFFFFF7		//ステージの終了処理中であることを示すフラグオフ
#define	_GAME_JUMP_ON			0x00000010		//メインのゲーム処理を一切行わないことを示すフラグ
#define	_GAME_JUMP_OFF			0xFFFFFFEF		//メインのゲーム処理を一切行わないことを示すフラグオフ
#define	_GAME_OVER_ON			0x00000020		//ゲームオーバーになった事を示すフラグ
#define	_GAME_OVER_OFF			0xFFFFFFDF		//ゲームオーバーになった事を示すフラグオフ
#define	_GAME_CLEAR_ON			0x00000040		//ゲームクリアになった事を示すフラグ
#define	_GAME_CLEAR_OFF			0xFFFFFFBF		//ゲームクリアになった事を示すフラグオフ
#define	_GAME_EXIT				0x80000000		//ゲームが終了したことを示すフラグ(プログラム終了)		
#define	_fSTAGE_CHK				0x8000007F		//現在立っているフラグをチェック
#define	_fSTAGE_CHK_InitSTAGE	0x0000000F		//InitSTAGE内でチェックに使用

//FaInOut::SetFade(****)部分の引数として使用するマクロ
#define	_SetDef		0x00000001	//ホワイトアウト、ブラックアウトの準備を行う	
#define	_SetWhiteIn	0x00000002	//ホワイトインの準備を行う
#define	_SetBlackIn	0x00000004	//ブラックインの準備を行う

//描画する背景を変化させるマクロ(DrawBG::fBGフラグ用)
#define	_SetBG00_ON		0x00000001
#define	_SetBG00_OFF	0xFFFFFFFE
#define	_SetBG01_ON		0x00000002
#define	_SetBG01_OFF	0xFFFFFFFD

//ジョイスティック用マクロ
#define	JOY_UP		1
#define	JOY_DOWN	2
#define	JOY_LEFT	4
#define	JOY_RIGHT	8
#define	JOY_A		16
#define	JOY_B		32

//ゲーム内重力加速度
#define	_G			0.097970775

//自機矩形範囲用マクロ
#define	_JIKI_W		20				//自機スプライトの横幅
//自機移動関連のマクロ
#define	_JIKIMOVE	3				//自機の移動量
//EXP関連マクロ
#define	_EXPUP		0x00008000		//弾一個を吸収した時に上昇するEXPの値
#define	_EXPSH		0x000A0000		//敵の弾と接触した時に減るEXPの値
//FUEL関連マクロ
#define	_FUELUP		0x000A0000		//弾一個をアームで吸収した時に増えるFUELゲージの量
#define	_FUELKY		0x00008000		//敵弾吸収時の弾吸収に消費するFUEL値(FUELゲージの減るスピード)
#define	_FUELMV		10				//敵弾吸収時の弾が接近してくるスピード
//自機の攻撃関連のマクロ
#define	_PWUPDDMG	0x0000000A		//パワーアップ画面を抜けた時のダメージ量(10)
#define	_ARMDMG		0x00000005		//アームで攻撃した時のダメージ量(5)
#define	_LSR1DMG	0x00000005		//レーザーLV1 1発当たりのダメージ量(5)
#define	_MSR1DMG	0x00000003		//ミサイルLV1 1発当たりのダメージ量(3)
#define	_HMM1DMG	0x0000000A		//ホーミングミサイルLV1 1発当たりのダメージ量(10)
#define	_HNB1DMG	0x00000014		//花火LV1 1発あたりのダメージ量(20)
//自機弾の移動量
#define	_HMM_MOV	0x00000008		//ホーミングミサイルの移動量

//自機の状態管理フラグのマクロ定義(DrawJIKI::fJIKIフラグ用)
#define	_NORMAL		0x00000000		//初期状態
#define	_ARM_ON		0x00000001		//アーム発射
#define	_ARM_OFF	0xFFFFFFFE		//アーム発射フラグオフ
#define	_FUEL_ON	0x00000002		//弾吸収中			
#define	_FUEL_OFF	0xFFFFFFFD		//弾吸収中フラグオフ
#define	_FUELD_ON	0x00000004		//弾吸収中断フラグ
#define	_FUELD_OFF	0xFFFFFFFB		//弾吸収中断フラグオフ
#define	_PWUP_ON	0x00000008		//パワーアップ処理画面中ことを示すフラグ
#define	_PWUP_OFF	0xFFFFFFF7		//パワーアップ処理画面中ことを示すフラグオフ
#define	_PWUPD_ON	0x00000010		//自機パワーアップに伴う画面の敵キャラを消滅させるフラグ
#define	_PWUPD_OFF	0xFFFFFFEF		//自機パワーアップに伴う画面の敵キャラを消滅させるフラグオフ
#define	_WEPN_ON	0x00000020		//画面上に主砲の弾が存在している事を示すフラグ
#define	_WEPN_OFF	0xFFFFFFDF		//画面上に主砲の弾が存在している事を示すフラグオフ		
#define	_SUBHMM_ON	0x00000040		//画面上にホーミングミサイルが存在している事を示すフラグ
#define	_SUBHMM_OFF	0xFFFFFFBF		//画面上にホーミングミサイルが存在している事を示すフラグオフ
#define	_SUBHNB_ON	0x00000080		//画面上に花火が存在している事を示すフラグ
#define	_SUBHNB_OFF	0xFFFFFF7F		//画面上に花火が存在している事を示すフラグオフ
#define	_OPT_ON		0x00000100		//オプションが装備されている事を示すフラグ
#define	_OPT_OFF	0xFFFFFEFF		//オプションが装備されている事を示すフラグオフ	
#define	_OPTWEP_ON	0x00000200		//画面上にオプション発射した弾が存在している事を示すフラグ
#define	_OPTWEP_OFF	0xFFFFFDFF		//画面上にオプション発射した弾が存在している事を示すフラグオフ
#define	_CONTINUE_ON	0x00000400	//コンティニューが選択された事を示すフラグ
#define	_CONTINUE_OFF	0xFFFFFBFF	//コンティニューが選択された事を示すフラグオフ
#define	_INVINCIBLE_ON	0x00000800	//コンティニューに伴う一定時間無敵状態であることを示すフラグ
#define	_INVINCIBLE_OFF	0xFFFFF7FF	//コンティニューに伴う一定時間無敵状態であることを示すフラグオフ
#define	_TARGET_ON	0x00001000		//ボスキャラが出現したことを示すフラグ
#define	_TARGET_OFF	0xFFFFEFFF		//ボスキャラが出現したことを示すフラグオフ
#define	_BOSS_ON	0x00002000		//ボスと戦闘中であることを示すフラグ
#define	_BOSS_OFF	0xFFFFDFFF		//ボスと戦闘中であることを示すフラグオフ
#define	_END_ON		0x40000000		//自機が撃墜されたことを示すフラグ
#define	_END_OFF	0xBFFFFFFF		//自機が撃墜されたことを示すフラグオフ
#define	_GAME_STOP_ON	0x80000000	//ゲームを一時停止させるフラグ
#define	_GAME_STOP_OFF	0x7FFFFFFF	//ゲームを一時停止させるフラグオフ


//自機の各種装備に関するフラグのマクロ定義(DrawJIKI::Wpd.flagフラグ用)
#define	_LSR1_ON	0x00000001	//レーザーレベル１を示すフラグ
#define	_LSR1_OFF	0xFFFFFFFE
#define	_LSR2_ON	0x00000002	//	〃			２
#define	_LSR2_OFF	0xFFFFFFFD
#define	_LSR3_ON	0x00000004
#define	_LSR3_OFF	0xFFFFFFFB
#define	_LSR4_ON	0x00000008
#define	_LSR4_OFF	0xFFFFFFF7
#define	_LSR5_ON	0x00000010
#define	_LSR5_OFF	0xFFFFFFEF
#define	_LSR_LVCHK	0x0000001F	//現在のレーザーレベルチェック用
#define	_MSR1_ON	0x00000020	//ミサイルレベル１
#define	_MSR1_OFF	0xFFFFFFDF
#define	_MSR2_ON	0x00000040	//	〃			２
#define	_MSR2_OFF	0xFFFFFFBF
#define	_MSR3_ON	0x00000080
#define	_MSR3_OFF	0xFFFFFF7F
#define	_MSR4_ON	0x00000100
#define	_MSR4_OFF	0xFFFFFEFF
#define	_MSR5_ON	0x00000200
#define	_MSR5_OFF	0xFFFFFDFF
#define	_MSR_LVCHK	0x000003E0	//現在のミサイルレベルチェック用
#define	_HMM1_ON	0x00000400	//ホーミングミサイルレベル１
#define	_HMM1_OFF	0xFFFFFBFF
#define	_HMM2_ON	0x00000800
#define	_HMM2_OFF	0xFFFFF7FF
#define	_HMM3_ON	0x00001000
#define	_HMM3_OFF	0xFFFFEFFF
#define	_HMM4_ON	0x00002000
#define	_HMM4_OFF	0xFFFFDFFF
#define	_HMM5_ON	0x00004000
#define	_HMM5_OFF	0xFFFFBFFF
#define	_HMM_LVCHK	0x00007C00	//現在のホーミングミサイルレベルチェック用
#define	_HNB1_ON	0x00008000	//花火レベル１
#define	_HNB1_OFF	0xFFFF7FFF
#define	_HNB2_ON	0x00010000
#define	_HNB2_OFF	0xFFFEFFFF
#define	_HNB3_ON	0x00020000
#define	_HNB3_OFF	0xFFFDFFFF
#define	_HNB4_ON	0x00040000
#define	_HNB4_OFF	0xFFFBFFFF
#define	_HNB5_ON	0x00080000
#define	_HNB5_OFF	0xFFF7FFFF
#define	_HNB_LVCHK	0x000F8000	//現在の花火レベルチェック用
#define	_OPT1_ON	0x00100000	//オプションレベル１
#define	_OPT1_OFF	0xFFEFFFFF
#define	_OPT2_ON	0x00200000
#define	_OPT2_OFF	0xFFDFFFFF
#define	_OPT3_ON	0x00400000
#define	_OPT3_OFF	0xFFBFFFFF
#define	_OPT4_ON	0x00800000
#define	_OPT4_OFF	0xFF7FFFFF
#define	_OPT_LVCHK	0x00F00000
#define	_LSR_ON		0x01000000	//主砲にレーザーが選択されている
#define	_LSR_OFF	0xFEFFFFFF
#define	_MSR_ON		0x02000000	//主砲にミサイルが選択されている
#define	_MSR_OFF	0xFDFFFFFF
#define	_MAINWP_CHK	0x03000000	//主砲に現在どちらが選択されているかチェック用
#define	_HMM_ON		0x04000000	//サブウェポンにホーミングミサイルが選択されている
#define	_HMM_OFF	0xFBFFFFFF
#define	_HNB_ON		0x08000000	//サブウェポンに花火が選択されている
#define	_HNB_OFF	0xF7FFFFFF
#define	_SUBWP_CHK	0x0C000000	//サブウェポンに現在どちらが選択されているかチェック用

//ホーミングミサイルの発射状況を示すフラグ(DrawJIKI::HMMDAT.fHMMフラグ用)
#define	_LOCKHMM_ON	0x00000001	//敵キャラのロックオン中フラグ
#define	_LOCKHMM_OFF 0xFFFFFFFE	//敵キャラのロックオン中フラグ解除
#define	_LOCKST_ON	0x00000002	//ロックオン開始直後を示すフラグ
#define	_LOCKST_OFF	0xFFFFFFFD	//ロックオン開始直後を示すフラグ解除			

//オプションの行動パターンを管理するフラグ(DrawJIKI::OPT.Jewel[0-3].fOPTフラグ用
#define	_OPTKO_ON	0x00000001	//固定
#define	_OPTKO_OFF	0xFFFFFFFE
#define _OPTKA_ON	0x00000002	//回転
#define	_OPTKA_OFF	0xFFFFFFFD
#define	_OPTTU_ON	0x00000004	//追尾
#define	_OPTTU_OFF	0xFFFFFFFB
#define	_fOPT_CHK	0x00000007	//チェック用

//敵キャラの状態に関するフラグのマクロ定義(DrawSP:fSPChrフラグ用)
#define	_SPON		0x00000001	//敵キャラ表示
#define	_SPOFF		0xFFFFFFFE	//敵キャラ表示フラグオフ
#define	_ATCKON		0x00000002	//攻撃中フラグオン
#define	_ATCKOFF	0xFFFFFFFD	//攻撃中フラグオフ
#define _HITON		0x00000004	//自機の弾に接触してダメージを受けた事を示すフラグ
#define _HITOFF		0xFFFFFFFB	//ダメージを受けたことを示すフラグオフ
#define	_TAMA1ON	0x00000008	//画面弾存在フラグ1(TAMA用)
#define	_TAMA1OFF	0xFFFFFFF7	//画面弾存在フラグ1オフ
#define	_TAMA2ON	0x00000010	//画面弾存在フラグ2	(TAMA2用)
#define	_TAMA2OFF	0xFFFFFFEF	//画面弾存在フラグ2オフ
#define	_TAMA3ON	0x00000020	//画面弾存在フラグ3(TAMA3用)
#define	_TAMA3OFF	0xFFFFFFDF	//画面弾存在フラグ3オフ
#define	_SPDES_a	0x40000000	//敵キャラ破壊アニメ第2段階
#define	_SPDESOFF_a	0xBFFFFFFF	//敵キャラ破壊アニメ第2段階オフ
#define	_SPDES		0x80000000	//敵キャラ破壊
#define	_SPDESOFF	0x7FFFFFFF	//敵キャラ破壊フラグオフ
#define	_SPDESCHK	0x80000001	//敵キャラが完全に破壊されているかチェック用

//弾オブジェクトに関するフラグのマクロ定義(TAMA.fTamaフラグ用)
#define	_TSET		0x00000001	//未発射
#define	_TON		0x00000002	//発射中
#define	_TOFF		0x00000000	//発射済消滅
#define	_TSAKU_ON	0x00000004	//花火炸裂(花火用)
#define	_TSAKU_OFF	0xFFFFFFFB	//花火炸裂オフ(花火用)
#define	_THIT_ON	0x00000008	//弾がダメージを受けたことを示すフラグ(TAMA3構造体用)
#define	_THIT_OFF	0xFFFFFFF7	//弾がダメージを受けたことを示すフラグオフ
#define	_TBAKU_ON	0x00000010	//弾が破壊された事を示すフラグ(TAMA3構造体用)
#define	_TBAKU_OFF	0xFFFFFFEF	//弾が破壊された事を示すフラグオフ

