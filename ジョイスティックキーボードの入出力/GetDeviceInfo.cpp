/*====================================================================
	�W���C�X�e�B�b�N�ƃL�[�{�[�h����̓��͂Ɋւ���\�[�X�t�@�C��
=====================================================================*/

#include <windows.h>
#include <dinput.h>

#include "Macros.h"

extern BOOL fInputDevice;

extern LPDIRECTINPUT7 lpDI;
extern LPDIRECTINPUTDEVICE7 lpDIJoystick;
extern LPDIRECTINPUTDEVICE7 lpDIKeyboard;

//�W���C�X�e�B�b�N�p
#define JOY_UP 1
#define JOY_DOWN 2
#define JOY_LEFT 4
#define JOY_RIGHT 8
#define JOY_A 16
#define JOY_B 32

//�L�[�{�[�h����p
#define KEYDOWN(name, key) (name[key] & 0x80)

//---------------------------------------------------------
//�W���C�X�e�B�b�N&�L�[�{�[�h����֐�
//---------------------------------------------------------
/*
���W���C�X�e�B�b�N�֐��̎g����
�P�D�{�^����������Ă��邱�Ƃ𒲂ׂ�
	if(JoyNew & ���ׂ�{�^��)
		������Ă��鎞�̏���
�Q�D�{�^���������ꂽ�u�Ԃ𒲂ׂ�
	if((JoyNew^JoyOld)&(JoyNew&���ׂ�{�^��))
		�����ꂽ�u�Ԃ̏���
�R�D�{�^���������ꂽ�u�Ԃ𒲂ׂ�
	if((JoyNew^JoyOld)&(JoyOld&���ׂ�{�^��))
		�����ꂽ�u�Ԃ̏���
�S�D�{�^������������ςȂ��ł��邱�Ƃ𒲂ׂ�
	if(JoyNew & JoyOld & ���ׂ�{�^��)
		��������ςȂ��̏���
*/

BYTE Joy_Get(void)
{
	HRESULT ret;
	char buffer[256]; //�L�[�{�[�h�̏����i�[����z��
	BYTE Joy = 0;	  //�L�[�̓��̓`�F�b�N�p

	//�W���C�X�e�B�b�N���q�����Ă��邩�`�F�b�N
	if (fInputDevice)
	{
		DIJOYSTATE js; //�W���C�X�e�B�b�N�̏����i�[����\����

		ret = lpDIJoystick->Poll();
		ret = lpDIJoystick->GetDeviceState(sizeof(DIJOYSTATE), (LPVOID)&js);
		if (ret == DIERR_INPUTLOST)
		{
			lpDIJoystick->Acquire();
			lpDIJoystick->Poll();
			lpDIJoystick->GetDeviceState(sizeof(DIJOYSTATE), (LPVOID)&js);
		}
		//�L�[�{�[�h�̃��X�g�`�F�b�N�̓��b�Z�[�W���[�v���ōs���Ă���̂Ń`�F�b�N���Ȃ�
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
		//�W���C�X�e�B�b�N���q�����Ă��Ȃ��ꍇ�̏���
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
