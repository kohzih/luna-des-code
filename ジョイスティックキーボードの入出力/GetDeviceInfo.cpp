/*====================================================================
	ジョイスティックとキーボードからの入力に関するソースファイル
=====================================================================*/

#include <windows.h>
#include <dinput.h>

#include "Macros.h"

extern BOOL fInputDevice;

extern LPDIRECTINPUT7 lpDI;
extern LPDIRECTINPUTDEVICE7 lpDIJoystick;
extern LPDIRECTINPUTDEVICE7 lpDIKeyboard;

//ジョイスティック用
#define JOY_UP 1
#define JOY_DOWN 2
#define JOY_LEFT 4
#define JOY_RIGHT 8
#define JOY_A 16
#define JOY_B 32

//キーボード操作用
#define KEYDOWN(name, key) (name[key] & 0x80)

//---------------------------------------------------------
//ジョイスティック&キーボード操作関数
//---------------------------------------------------------
/*
◎ジョイスティック関数の使い方
１．ボタンが押されていることを調べる
	if(JoyNew & 調べるボタン)
		押されている時の処理
２．ボタンが押された瞬間を調べる
	if((JoyNew^JoyOld)&(JoyNew&調べるボタン))
		押された瞬間の処理
３．ボタンが離された瞬間を調べる
	if((JoyNew^JoyOld)&(JoyOld&調べるボタン))
		離された瞬間の処理
４．ボタンが押されっぱなしであることを調べる
	if(JoyNew & JoyOld & 調べるボタン)
		押されっぱなしの処理
*/

BYTE Joy_Get(void)
{
	HRESULT ret;
	char buffer[256]; //キーボードの情報を格納する配列
	BYTE Joy = 0;	  //キーの入力チェック用

	//ジョイスティックが繋がっているかチェック
	if (fInputDevice)
	{
		DIJOYSTATE js; //ジョイスティックの情報を格納する構造体

		ret = lpDIJoystick->Poll();
		ret = lpDIJoystick->GetDeviceState(sizeof(DIJOYSTATE), (LPVOID)&js);
		if (ret == DIERR_INPUTLOST)
		{
			lpDIJoystick->Acquire();
			lpDIJoystick->Poll();
			lpDIJoystick->GetDeviceState(sizeof(DIJOYSTATE), (LPVOID)&js);
		}
		//キーボードのロストチェックはメッセージループ内で行っているのでチェックしない
		lpDIKeyboard->Poll();
		lpDIKeyboard->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);

		if (js.lY < 0 || KEYDOWN(buffer, DIK_UP))
			Joy |= JOY_UP;
		if (js.lY > 0 || KEYDOWN(buffer, DIK_DOWN))
			Joy |= JOY_DOWN;
		if (js.lX < 0 || KEYDOWN(buffer, DIK_LEFT))
			Joy |= JOY_LEFT;
		if (js.lX > 0 || KEYDOWN(buffer, DIK_RIGHT))
			Joy |= JOY_RIGHT;
		if (js.rgbButtons[0] & 0x80 || KEYDOWN(buffer, DIK_Z))
			Joy |= JOY_A;
		if (js.rgbButtons[1] & 0x80 || KEYDOWN(buffer, DIK_X))
			Joy |= JOY_B;
	}
	else
	{
		//ジョイスティックが繋がっていない場合の処理
		lpDIKeyboard->Poll();
		lpDIKeyboard->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);

		if (KEYDOWN(buffer, DIK_UP))
			Joy |= JOY_UP;
		if (KEYDOWN(buffer, DIK_DOWN))
			Joy |= JOY_DOWN;
		if (KEYDOWN(buffer, DIK_LEFT))
			Joy |= JOY_LEFT;
		if (KEYDOWN(buffer, DIK_RIGHT))
			Joy |= JOY_RIGHT;
		if (KEYDOWN(buffer, DIK_Z))
			Joy |= JOY_A;
		if (KEYDOWN(buffer, DIK_X))
			Joy |= JOY_B;
	}

	return Joy;
}
