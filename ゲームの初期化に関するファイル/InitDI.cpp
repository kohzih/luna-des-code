/*====================================================================
	DirectInput�������Ɋւ���\�[�X�t�@�C��
=====================================================================*/

#include	<windows.h>
#include	<dinput.h>

#include	"Macros.h"

extern	HINSTANCE	hInst;			//�C���X�^���X�n���h��
extern	HWND		hWnd;			//���C���E�B���h�E�n���h��
extern	BOOL		fInputDevice;	//���̓f�o�C�X�t���O
extern	BOOL		fFullscreen;	//�t���X�N���[��(1)or�E�B���h�E�t���O(0)

extern	LPDIRECTINPUT7			lpDI;         
extern	LPDIRECTINPUTDEVICE7	lpDIJoystick;
extern	LPDIRECTINPUTDEVICE7	lpDIKeyboard;

//�W���C�X�e�B�b�N�񋓊֐�
BOOL CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE,LPVOID);
//DIPROPDWORD�v���p�e�B�\���̂�����������֐�
HRESULT	SetDIDwordProperty(REFGUID,DWORD,DWORD,DWORD);
//�}�E�X�\������֐�
void	MouseOnOff(BOOL);

//---------------------------------------------------------
//DirectInput������
//---------------------------------------------------------
BOOL	InitDI()
{
	HRESULT ret;

	// DirectInput�I�u�W�F�N�g���쐬
    ret = DirectInputCreateEx( hInst, DIRECTINPUT_VERSION, IID_IDirectInput7, (void**)&lpDI, NULL);
    if (ret != DI_OK)	return FALSE;
	
	//�W���C�X�e�B�b�N�p
	
	//�W���C�X�e�B�b�N��T��
	lpDI->EnumDevices(DIDEVTYPE_JOYSTICK,(LPDIENUMDEVICESCALLBACK)GetJoystickCallback,NULL,DIEDFL_ATTACHEDONLY);
	if (lpDIJoystick == NULL)	fInputDevice=0;	//�W���C�X�e�B�b�N�����t����Ȃ�
	else{
			//�W���C�X�e�B�b�N����
			fInputDevice=1;	

			//�W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
			ret = lpDIJoystick->SetDataFormat(&c_dfDIJoystick);
			if (ret != DI_OK)	return FALSE;

			//���[�h��ݒ�i�t�H�A�O���E���h���r�����[�h�j
			ret = lpDIJoystick->SetCooperativeLevel(hWnd,DISCL_FOREGROUND|DISCL_EXCLUSIVE);
			if (ret != DI_OK)	return FALSE;

			//���̒l�͈̔͂�ݒ�
			DIPROPRANGE diprg;
			
			ZeroMemory(&diprg,sizeof(diprg));
			diprg.diph.dwSize		=sizeof(diprg); 
			diprg.diph.dwHeaderSize	=sizeof(diprg.diph); 
			diprg.diph.dwObj		=DIJOFS_X; 
			diprg.diph.dwHow		=DIPH_BYOFFSET; 
			diprg.lMin				=-1000;
			diprg.lMax				=+1000;
			
			//X���̐ݒ�
			ret=lpDIJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);
			if (ret != DI_OK)	return FALSE;
			//Y���̐ݒ�
			diprg.diph.dwObj		=DIJOFS_Y;
			ret=lpDIJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);
			if (ret != DI_OK)	return FALSE;
			
			//X���̃f�b�h�]�[���̐ݒ�
			ret=SetDIDwordProperty(DIPROP_DEADZONE,DIJOFS_X,DIPH_BYOFFSET,5000);
			if (ret != DI_OK)	return FALSE;
			//Y���̃f�b�h�]�[���̐ݒ�
			ret=SetDIDwordProperty(DIPROP_DEADZONE,DIJOFS_Y,DIPH_BYOFFSET,5000);
			if (ret != DI_OK)	return FALSE;
			
			//���̓f�o�C�X�̃A�N�Z�X�����擾
			ret=lpDIJoystick->Acquire();
			if (ret != DI_OK)	return FALSE;
	}

	//�L�[�{�[�h�p
	
	//DirectInputDevice�I�u�W�F�N�g���쐬
    ret = lpDI->CreateDeviceEx( GUID_SysKeyboard, IID_IDirectInputDevice7, (void**)&lpDIKeyboard, NULL );
    if (ret != DI_OK)	return FALSE;
	
	//�L�[�{�[�h�E�f�o�C�X����f�[�^���擾
	ret = lpDIKeyboard->SetDataFormat( &c_dfDIKeyboard );
    if (ret != DI_OK)	return FALSE;
	
	//�������x���ݒ�
	ret = lpDIKeyboard->SetCooperativeLevel( hWnd,DISCL_EXCLUSIVE | DISCL_FOREGROUND );
    if (ret != DI_OK)	return FALSE;
	
	// ���̓f�o�C�X�̃A�N�Z�X�����擾�B
	ret = lpDIKeyboard->Acquire();
	if (ret != DI_OK)	return FALSE;
	
	//�}�E�X�J�[�\������	
	
	//�t���X�N���[�����̂݃}�E�X�J�[�\��������
	if(fFullscreen)		MouseOnOff(FALSE);		//�}�E�X�J�[�\��������
	
	return	TRUE;
}

//---------------------------------------------------------
//DirectInput�I�u�W�F�N�g���
//---------------------------------------------------------
void	ReleaseDI(void)
{	
	//�t���X�N���[����Ԃ̎��ɃI�u�W�F�N�g�̉�����s��ꂽ�ꍇ
	//�}�E�X�J�[�\����\��������
	if(fFullscreen)		MouseOnOff(TRUE);		//�}�E�X�J�[�\����\��
	//Release() ���Ăяo���O�͕K���f�o�C�X�̃A�N�Z�X�����������
	if(!lpDIJoystick==NULL)		lpDIJoystick->Unacquire();
	RELEASE(lpDIJoystick);
	if(!lpDIKeyboard==NULL)		lpDIKeyboard->Unacquire();
	RELEASE(lpDIKeyboard);
	RELEASE(lpDI);
}

//------------------------------------------------------
//	�W���C�X�e�B�b�N�E�f�o�C�X��񋓁i�R�[���o�b�N�֐��j
//------------------------------------------------------
BOOL CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE lpddi,LPVOID pvRef)
{
	HRESULT ret;
	ret=lpDI->CreateDeviceEx(lpddi->guidInstance, IID_IDirectInputDevice7, (void**)&lpDIJoystick, NULL);
	if (ret != DI_OK)	return DIENUM_CONTINUE;		// ���̃f�o�C�X���

	return DIENUM_STOP;	// �f�o�C�X�̗񋓂𒆎~
}
 
//----------------------------------------------------------
// DIPROPDWORD�v���p�e�B�\���̂�����������֐�
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
//�}�E�X�J�[�\���������֐�
//-----------------------------------------------
void	MouseOnOff(BOOL Mflag)
{
    static BOOL      Mm = TRUE;

    //�}�E�X�J�[�\�������Ɏw�肳�ꂽ��ԂȂ�΃��^�[��
	if(Mm==Mflag)	return;
	
	switch(Mflag)
    {
        case TRUE:      // �\������
			ShowCursor(TRUE);
			Mm = TRUE;
			break;
        
		case FALSE:     // ��\���ɂ���
			ShowCursor(FALSE);
			Mm = FALSE;
            break;
    }
}
