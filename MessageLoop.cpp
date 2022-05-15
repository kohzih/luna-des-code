/*====================================================================
	���b�Z�[�W���[�v��
	�L�[���͂ɂ��t���X�N���[���ƃE�B���h�E���[�h�̐؂�ւ����s��
	�֐��̃\�[�X�t�@�C��
=====================================================================*/

#include <windows.h>
#include <ddraw.h>
#include <dinput.h>

#include "Macros.h"
#include "Draw.h"

extern HWND hWnd;		 //���C���E�B���h�E�n���h��
extern BOOL fActive;	 //�A�N�e�B�u�t���O(�E�B���h�E�\���p)
extern BOOL fFullscreen; //�t���X�N���[��(1)or�E�B���h�E�t���O(0)

extern LPDIRECTDRAW7 lpDD; // DirectDraw�I�u�W�F�N�g
extern LPDIRECTINPUTDEVICE7 lpDIKeyboard;

extern Draw *lpDraw; //�`��Ǘ��I�u�W�F�N�g

//�L�[�{�[�h����p
#define KEYDOWN(name, key) (name[key] & 0x80)

BOOL GameMain(void);

BOOL FullChgWindow(void);
BOOL InitDD(void);
BOOL InitDS(void);
BOOL InitDM(void);
BOOL InitDI(void);
void ReleaseDD(void);
void ReleaseDS(void);
void ReleaseDM(void);
void ReleaseDI(void);

//---------------------------------------------------------
//���b�Z�[�W���[�v
//---------------------------------------------------------
int MessageLoop(void)
{
	MSG msg;

	HRESULT ret;
	char buffer[256];

	while (1)
	{ //���b�Z�[�W���擾����
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//���b�Z�[�W�����鎞

			// WM_QUIT���b�Z�[�W�͏����̏I��
			if (msg.message == WM_QUIT)
				break;
			//���b�Z�[�W���������ăE�B���h�E�v���V�[�W���ɓ]������
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//���b�Z�[�W��������
			//�E�B���h�E���A�N�e�B�u���ǂ����`�F�b�N
			if (fActive)
			{
				ret = lpDIKeyboard->Poll();
				ret = lpDIKeyboard->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);
				if (ret == DIERR_INPUTLOST)
					lpDIKeyboard->Acquire();

				// ESC�L�[��������Ă����狭���I��
				if (KEYDOWN(buffer, DIK_ESCAPE))
				{
					DestroyWindow(hWnd);
					break;
				}
#if DEBUG >= 1
				//���ECTRL�L�[��������Ă�����E�B���h�E�؂�ւ�
				if (KEYDOWN(buffer, DIK_RCONTROL) || KEYDOWN(buffer, DIK_LCONTROL))
				{
					//�E�B���h�E���[�h��؂�ւ�
					if (!FullChgWindow())
					{
						DestroyWindow(hWnd);
						break;
					}
				}
#endif
				//�Q�[���̃��C������
				ret = GameMain();
				if (!ret)
				{
#if DEBUG >= 1
					//	MessageBox(	hWnd,"�Q�[�����ŕs���ȏ������s���܂���","ERROR",MB_OK|MB_APPLMODAL);
#endif
					DestroyWindow(hWnd);
					break;
				}
			}
			else
			{
				//���̃X���b�h�ɏ����𗬂�
				WaitMessage();
			}
		}
	}
	//���b�Z�[�W���[�v�̏I��
	return msg.wParam;
}

//---------------------------------------------------------
//�Q�[����ʂ��E�B���h�E���[�h�ƃt���X�N���[����؂�ւ���֐�
//---------------------------------------------------------
BOOL FullChgWindow()
{
	//���܂Ńt���X�N���[�����[�h�������ꍇ�̓E�B���h�E���[�h�ɐݒ�
	if (fFullscreen)
	{
		//�v���C�}���T�[�t�F�X�̃f�B�X�v���C�f�o�C�X�n�[�h�E�F�A�̃��[�h���A
		// IDirectDraw7::SetDisplayMode���\�b�h���Ăяo�����O�̏�ԂɃ��Z�b�g
		lpDD->RestoreDisplayMode();

		//�E�B���h�E���č\������

		RECT rcWindow;
		DWORD width;
		DWORD height;

		SetRect(&rcWindow, 0, 0, WinXMax, WinYMax);
		AdjustWindowRect(&rcWindow, WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU, FALSE);
		width = rcWindow.right - rcWindow.left;
		height = rcWindow.bottom - rcWindow.top;
		//�E�B���h�E�̈ʒu�A�T�C�Y��ݒ肵����
		SetWindowPos(hWnd, 0, 100, 36, width, height, SWP_NOZORDER | SWP_DRAWFRAME);

		//���ݎg�p���̃I�u�W�F�N�g���������
		lpDraw->ReleaseSuf();
		ReleaseDI();
		ReleaseDM();
		ReleaseDS();
		ReleaseDD();

		//�X�N���[�����[�h�t���O���E�B���h�E���[�h�ɐݒ�
		fFullscreen = 0;

		//�I�u�W�F�N�g����蒼��
		if (!InitDD()) //�G���[���o���ꍇ�̏���
		{
			ReleaseDD();
			fFullscreen = 1; //���[�h�����ɖ߂�
			if (!InitDD())	 //���̃��[�h�ō쐬������
			{
				return FALSE;
			}
			if (!InitDS())
			{
				return FALSE;
			}
			if (!InitDM())
			{
				return FALSE;
			}
			if (!InitDI())
			{
				return FALSE;
			}
			if (!lpDraw->ReloadSuf())
			{
				return FALSE;
			}
			return TRUE;
		}
		// DirectSound������
		if (!InitDS())
		{
			return FALSE;
		}
		// DirectMusic������
		if (!InitDM())
		{
			return FALSE;
		}
		// DirectInput������
		if (!InitDI())
		{
			return FALSE;
		}
		//�I�t�X�N���[���T�[�t�F�X�������[�h
		if (!lpDraw->ReloadSuf())
		{
			return FALSE;
		}
		return TRUE;
	}

	//���܂ŃE�B���h�E���[�h�������ꍇ�t���X�N���[�����[�h�ɐݒ�
	if (!fFullscreen)
	{
		lpDraw->ReleaseSuf();
		ReleaseDI();
		ReleaseDM();
		ReleaseDS();
		ReleaseDD();

		//�X�N���[�����[�h�t���O���t���X�N���[���ɐݒ�
		fFullscreen = 1;

		//�I�u�W�F�N�g����蒼��
		if (!InitDD()) //�G���[���o���ꍇ�̏���
		{
			//�ȉ��E�B���h�E�쐬
			lpDD->RestoreDisplayMode();

			//�E�B���h�E���č\������
			RECT rcWindow;
			DWORD width;
			DWORD height;

			SetRect(&rcWindow, 0, 0, WinXMax, WinYMax);
			AdjustWindowRect(&rcWindow, WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU, FALSE);
			width = rcWindow.right - rcWindow.left;
			height = rcWindow.bottom - rcWindow.top;
			//�E�B���h�E�̈ʒu�A�T�C�Y��ݒ肵����
			SetWindowPos(hWnd, 0, 100, 36, width, height, SWP_NOZORDER | SWP_DRAWFRAME);

			ReleaseDD();

			fFullscreen = 0; //���[�h�����ɖ߂�
			if (!InitDD())	 //���̃��[�h�ō쐬������
			{
				return FALSE;
			}
			if (!InitDS())
			{
				return FALSE;
			}
			if (!InitDM())
			{
				return FALSE;
			}
			if (!InitDI())
			{
				return FALSE;
			}
			if (!lpDraw->ReloadSuf())
			{
				return FALSE;
			}
			return TRUE;
		}
		// DirectSound������
		if (!InitDS())
		{
			return FALSE;
		}
		// DirectMusic������
		if (!InitDM())
		{
			return FALSE;
		}
		// DirectInput������
		if (!InitDI())
		{
			return FALSE;
		}
		//�I�t�X�N���[���T�[�t�F�X�������[�h
		if (!lpDraw->ReloadSuf())
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}