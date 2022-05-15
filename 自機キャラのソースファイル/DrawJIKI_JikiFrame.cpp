/*=============================================================================
	自機本体に関するアニメーションシーンの処理(描画はしていない)
==============================================================================*/

#include <windows.h>
#include <ddraw.h>

#include "Macros.h"
#include "Draw.h"
#include "Struct.h"
#include "DrawJIKI.h"
#include "DrawJIKIDataEXT.h"
#include "DrawMacros.h"

//////////////////////////////////////////////////////////////////////
//	自機の爆破シーンのアニメーション処理をする
//////////////////////////////////////////////////////////////////////
void DrawJIKI::JikiBakuFrame()
{
	// SPサーフェスからの転送矩形範囲をセット
	//画面からはみ出す部分は転送しない処理をしています
	Baku.SrcR = SpJIKI[Baku.AnimePat[Baku.AnimeCnt]];

	Baku.DesR.left = Cdn.x - 22;
	Baku.DesR.top = Cdn.y - 16;
	Baku.DesR.right = Cdn.x + 42;
	Baku.DesR.bottom = Cdn.y + 48;

	if (Baku.DesR.left < 0)
	{
		Baku.SrcR.left = Baku.SrcR.left - Baku.DesR.left;
		Baku.DesR.left = 0;
	}
	else if (Baku.DesR.right > WinXMax)
	{
		Baku.SrcR.right = Baku.SrcR.right - (Baku.DesR.right - WinXMax);
		Baku.DesR.right = WinXMax;
	}
	if (Baku.DesR.top < 0)
	{
		Baku.SrcR.top = Baku.SrcR.top - Baku.DesR.top;
		Baku.DesR.top = 0;
	}
	else if (Baku.DesR.bottom > WinYMax)
	{
		Baku.SrcR.bottom = Baku.SrcR.bottom - (Baku.DesR.bottom - WinYMax);
		Baku.DesR.bottom = WinYMax;
	}

	//爆発シーンの矩形範囲をセット
	if (!(Baku.FrameCnt--))
	{
		//爆発アニメーションが終了したかチェック
		if ((Baku.AnimeCnt + 1) == Baku.Anime)
		{
			//爆発時の自機アニメーションデータを初期値に戻す
			Baku.AnimeCnt = 0;
			Baku.FrameCnt = Baku.AnimeFrame[0];
			//ゲーム一時停止フラグオン(コンティニュー処理へ)
			fJIKI |= _GAME_STOP_ON;
		}
		else
		{
			Baku.AnimeCnt = (Baku.AnimeCnt + 1) % Baku.Anime; //アニメパターンを進める
			Baku.FrameCnt = Baku.AnimeFrame[Baku.AnimeCnt];	  //表示するアニメパターンのフレーム数セット
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	自機のアーム発射シーンのアニメーション処理
//////////////////////////////////////////////////////////////////////
void DrawJIKI::JikiArmFrame()
{
	//自機の部分の矩形範囲をセット
	// SPサーフェスからの転送矩形範囲をセット
	ArmJIKI.SrcR = SpJIKI[ArmJIKI.AnimePat[ArmJIKI.AnimeCnt]];

	//画面からはみ出す部分は転送しない処理をしています
	ArmJIKI.DesR.left = Cdn.x - 22;
	ArmJIKI.DesR.top = Cdn.y + 16;
	ArmJIKI.DesR.right = Cdn.x + 42;
	ArmJIKI.DesR.bottom = Cdn.y + 32;

	if (ArmJIKI.DesR.left < 0)
	{
		ArmJIKI.SrcR.left = ArmJIKI.SrcR.left - ArmJIKI.DesR.left;
		ArmJIKI.DesR.left = 0;
	}
	else if (ArmJIKI.DesR.right > WinXMax)
	{
		ArmJIKI.SrcR.right = ArmJIKI.SrcR.right - (ArmJIKI.DesR.right - WinXMax);
		ArmJIKI.DesR.right = WinXMax;
	}

	//アニメーションを進める
	if (!(ArmJIKI.FrameCnt--))
	{
		ArmJIKI.AnimeCnt = (ArmJIKI.AnimeCnt + 1) % ArmJIKI.Anime; //アニメパターンを進める
		ArmJIKI.FrameCnt = ArmJIKI.AnimeFrame[ArmJIKI.AnimeCnt];   //表示するアニメパターンのフレーム数セット
	}

	//アームの部分の矩形範囲をセット
	// SPサーフェスからの転送矩形範囲をセット
	Arm.SrcR = SpJIKI[Arm.AnimePat[Arm.AnimeCnt]];

	//画面からはみ出す部分は転送しない処理をしています
	Arm.DesR.left = Cdn.x - 6;
	Arm.DesR.top = Cdn.y - (Arm.SrcR.bottom - Arm.SrcR.top) + 16;
	Arm.DesR.right = Cdn.x + 26;
	Arm.DesR.bottom = Cdn.y + 16;

	if (Arm.DesR.left < 0)
	{
		Arm.SrcR.left = Arm.SrcR.left - Arm.DesR.left;
		Arm.DesR.left = 0;
	}
	else if (Arm.DesR.right > WinXMax)
	{
		Arm.SrcR.right = Arm.SrcR.right - (Arm.DesR.right - WinXMax);
		Arm.DesR.right = WinXMax;
	}
	if (Arm.DesR.top < 0)
	{
		Arm.SrcR.top = Arm.SrcR.top - Arm.DesR.top;
		Arm.DesR.top = 0;
	}

	//アニメーションを進める
	if (!(Arm.FrameCnt--))
	{
		//アーム発射のアニメーションが終了したかチェック
		if ((Arm.AnimeCnt + 1) == Arm.Anime)
		{
			//アーム発射時の自機アニメーションデータを初期値に戻す
			ArmJIKI.AnimeCnt = 0;
			ArmJIKI.FrameCnt = ArmJIKI.AnimeFrame[0];
			//アーム発射時のアームアニメーションデータを初期値に戻す
			Arm.AnimeCnt = 0;
			Arm.FrameCnt = Arm.AnimeFrame[0];
			//アーム発射フラグをオフ
			fJIKI &= _ARM_OFF;
		}
		else
		{
			Arm.AnimeCnt = (Arm.AnimeCnt + 1) % Arm.Anime; //アニメパターンを進める
			Arm.FrameCnt = Arm.AnimeFrame[Arm.AnimeCnt];   //表示するアニメパターンのフレーム数セット
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	自機の弾吸収シーンのアニメーション処理をする
//////////////////////////////////////////////////////////////////////
void DrawJIKI::JikiTmkyFrame()
{
	if (fJIKI & _FUEL_ON)
	{
		// SPサーフェスからの転送矩形範囲をセット
		//画面からはみ出す部分は転送しない処理をしています
		TmKySy.SrcR = SpJIKI[TmKySy.AnimePat[TmKySy.AnimeCnt]];
		TmKySy.DesR.left = Cdn.x - 22;
		TmKySy.DesR.top = Cdn.y + 16;
		TmKySy.DesR.right = Cdn.x + 42;
		TmKySy.DesR.bottom = Cdn.y + 32;

		if (TmKySy.DesR.left < 0)
		{
			TmKySy.SrcR.left = TmKySy.SrcR.left - TmKySy.DesR.left;
			TmKySy.DesR.left = 0;
		}
		else if (TmKySy.DesR.right > WinXMax)
		{
			TmKySy.SrcR.right = TmKySy.SrcR.right - (TmKySy.DesR.right - WinXMax);
			TmKySy.DesR.right = WinXMax;
		}

		//自機の部分の矩形範囲をセット
		if (!(TmKySy.FrameCnt--))
		{
			TmKySy.AnimeCnt = (TmKySy.AnimeCnt + 1) % TmKySy.Anime; //アニメパターンを進める
			TmKySy.FrameCnt = TmKySy.AnimeFrame[TmKySy.AnimeCnt];	//表示するアニメパターンのフレーム数セット
		}

		//弾吸収中断フラグの立っている場合の処理
	}
	else if (fJIKI & _FUELD_ON)
	{
		//弾吸収中断フラグオフ
		fJIKI &= _FUELD_OFF;

		//アニメーションデータを初期化
		TmKySy.AnimeCnt = 0;
		TmKySy.FrameCnt = TmKySy.AnimeFrame[0];
	}
}