/*====================================================================
	DirectInput初期化に関するソースファイル
=====================================================================*/

#include	<windows.h>
#include	<dinput.h>

#include	"Macros.h"

extern	HINSTANCE	hInst;			//インスタンスハンドル
extern	HWND		hWnd;			//メインウィンドウハンドル
extern	BOOL		fInputDevice;	//入力デバイスフラグ
extern	BOOL		fFullscreen;	//フルスクリーン(1)orウィンドウフラグ(0)

extern	LPDIRECTINPUT7			lpDI;         
extern	LPDIRECTINPUTDEVICE7	lpDIJoystick;
extern	LPDIRECTINPUTDEVICE7	lpDIKeyboard;

//ジョイスティック列挙関数
BOOL CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE,LPVOID);
//DIPROPDWORDプロパティ構造体を初期化する関数
HRESULT	SetDIDwordProperty(REFGUID,DWORD,DWORD,DWORD);
//マウス表示制御関数
void	MouseOnOff(BOOL);

//---------------------------------------------------------
//DirectInput初期化
//---------------------------------------------------------
BOOL	InitDI()
{
	HRESULT ret;

	// DirectInputオブジェクトを作成
    ret = DirectInputCreateEx( hInst, DIRECTINPUT_VERSION, IID_IDirectInput7, (void**)&lpDI, NULL);
    if (ret != DI_OK)	return FALSE;
	
	//ジョイスティック用
	
	//ジョイスティックを探す
	lpDI->EnumDevices(DIDEVTYPE_JOYSTICK,(LPDIENUMDEVICESCALLBACK)GetJoystickCallback,NULL,DIEDFL_ATTACHEDONLY);
	if (lpDIJoystick == NULL)	fInputDevice=0;	//ジョイスティックが見付からない
	else{
			//ジョイスティック発見
			fInputDevice=1;	

			//ジョイスティック用のデータ・フォーマットを設定
			ret = lpDIJoystick->SetDataFormat(&c_dfDIJoystick);
			if (ret != DI_OK)	return FALSE;

			//モードを設定（フォアグラウンド＆排他モード）
			ret = lpDIJoystick->SetCooperativeLevel(hWnd,DISCL_FOREGROUND|DISCL_EXCLUSIVE);
			if (ret != DI_OK)	return FALSE;

			//軸の値の範囲を設定
			DIPROPRANGE diprg;
			
			ZeroMemory(&diprg,sizeof(diprg));
			diprg.diph.dwSize		=sizeof(diprg); 
			diprg.diph.dwHeaderSize	=sizeof(diprg.diph); 
			diprg.diph.dwObj		=DIJOFS_X; 
			diprg.diph.dwHow		=DIPH_BYOFFSET; 
			diprg.lMin				=-1000;
			diprg.lMax				=+1000;
			
			//X軸の設定
			ret=lpDIJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);
			if (ret != DI_OK)	return FALSE;
			//Y軸の設定
			diprg.diph.dwObj		=DIJOFS_Y;
			ret=lpDIJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);
			if (ret != DI_OK)	return FALSE;
			
			//X軸のデッドゾーンの設定
			ret=SetDIDwordProperty(DIPROP_DEADZONE,DIJOFS_X,DIPH_BYOFFSET,5000);
			if (ret != DI_OK)	return FALSE;
			//Y軸のデッドゾーンの設定
			ret=SetDIDwordProperty(DIPROP_DEADZONE,DIJOFS_Y,DIPH_BYOFFSET,5000);
			if (ret != DI_OK)	return FALSE;
			
			//入力デバイスのアクセス権を取得
			ret=lpDIJoystick->Acquire();
			if (ret != DI_OK)	return FALSE;
	}

	//キーボード用
	
	//DirectInputDeviceオブジェクトを作成
    ret = lpDI->CreateDeviceEx( GUID_SysKeyboard, IID_IDirectInputDevice7, (void**)&lpDIKeyboard, NULL );
    if (ret != DI_OK)	return FALSE;
	
	//キーボード・デバイスからデータを取得
	ret = lpDIKeyboard->SetDataFormat( &c_dfDIKeyboard );
    if (ret != DI_OK)	return FALSE;
	
	//協調レベル設定
	ret = lpDIKeyboard->SetCooperativeLevel( hWnd,DISCL_EXCLUSIVE | DISCL_FOREGROUND );
    if (ret != DI_OK)	return FALSE;
	
	// 入力デバイスのアクセス権を取得。
	ret = lpDIKeyboard->Acquire();
	if (ret != DI_OK)	return FALSE;
	
	//マウスカーソル制御	
	
	//フルスクリーン時のみマウスカーソルを消す
	if(fFullscreen)		MouseOnOff(FALSE);		//マウスカーソルを消す
	
	return	TRUE;
}

//---------------------------------------------------------
//DirectInputオブジェクト解放
//---------------------------------------------------------
void	ReleaseDI(void)
{	
	//フルスクリーン状態の時にオブジェクトの解放が行われた場合
	//マウスカーソルを表示させる
	if(fFullscreen)		MouseOnOff(TRUE);		//マウスカーソルを表示
	//Release() を呼び出す前は必ずデバイスのアクセス権を解放する
	if(!lpDIJoystick==NULL)		lpDIJoystick->Unacquire();
	RELEASE(lpDIJoystick);
	if(!lpDIKeyboard==NULL)		lpDIKeyboard->Unacquire();
	RELEASE(lpDIKeyboard);
	RELEASE(lpDI);
}

//------------------------------------------------------
//	ジョイスティック・デバイスを列挙（コールバック関数）
//------------------------------------------------------
BOOL CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE lpddi,LPVOID pvRef)
{
	HRESULT ret;
	ret=lpDI->CreateDeviceEx(lpddi->guidInstance, IID_IDirectInputDevice7, (void**)&lpDIJoystick, NULL);
	if (ret != DI_OK)	return DIENUM_CONTINUE;		// 次のデバイスを列挙

	return DIENUM_STOP;	// デバイスの列挙を中止
}
 
//----------------------------------------------------------
// DIPROPDWORDプロパティ構造体を初期化する関数
//----------------------------------------------------------
HRESULT	SetDIDwordProperty(REFGUID guidProperty,DWORD dwObject, DWORD dwHow, DWORD dwValue)
{
   DIPROPDWORD dipdw;

   ZeroMemory(&dipdw,sizeof(dipdw));
   dipdw.diph.dwSize       = sizeof(dipdw);
   dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
   dipdw.diph.dwObj        = dwObject;
   dipdw.diph.dwHow        = dwHow;
   dipdw.dwData            = dwValue;

   return lpDIJoystick->SetProperty(guidProperty, &dipdw.diph);

}

//-----------------------------------------------
//マウスカーソルを消す関数
//-----------------------------------------------
void	MouseOnOff(BOOL Mflag)
{
    static BOOL      Mm = TRUE;

    //マウスカーソルが既に指定された状態ならばリターン
	if(Mm==Mflag)	return;
	
	switch(Mflag)
    {
        case TRUE:      // 表示する
			ShowCursor(TRUE);
			Mm = TRUE;
			break;
        
		case FALSE:     // 非表示にする
			ShowCursor(FALSE);
			Mm = FALSE;
            break;
    }
}
