/*====================================================================
	�E�B���h�E�v���V�[�W���̃\�[�X�t�@�C��
=====================================================================*/

#include	<windows.h>
#include	<ddraw.h>

#include	"Macros.h"
#include	"FaInOut.h"

extern	HWND		hWnd;						//���C���E�B���h�E�n���h��
extern	BOOL		fActive;					//�A�N�e�B�u�t���O(�E�B���h�E�\���p)

extern	LPDIRECTDRAWSURFACE7	lpDDSPrimary;	//�v���C�}���T�[�t�F�[�X
extern	LPDIRECTDRAWPALETTE		lpDDpal;		//DirectDraw�p���b�g�I�u�W�F�N�g

extern	FaInOut			*lpFInOut;		//�t�F�[�h�C���A�E�g�I�u�W�F�N�g

void	ReleaseDD(void);
void	ReleaseDS(void);
void	ReleaseDM(void);
void	ReleaseDI(void);
void	ReleaseETC(void);

//---------------------------------------------------------
//�E�B���h�E�v���V�[�W��
//---------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND	hwnd,		//�E�B���h�E�@�n���h��
							UINT	message,	//���b�Z�[�W
							WPARAM	wParam,		//�ėp���� W
							LPARAM	lParam		//�ėp���� L
							)
{	
	
	switch(message){
		case	WM_CREATE:
			//�E�B���h�E�쐬
			break;
		
		case	WM_PALETTECHANGED:	//(�E�B���h�E�\���p)
			//�p���b�g�̕ύX�m�F���b�Z�[�W
			if(wParam == (DWORD)hWnd)	break;
			
			break;
		
		case	WM_QUERYNEWPALETTE:	//(�E�B���h�E�\���p)
			//�p���b�g���V�����\�z���ꂽ�m�F���b�Z�[�W
			if(lpDDSPrimary && lpFInOut)	
			{
				lpDDSPrimary->SetPalette(lpDDpal);	//�p���b�g�Z�b�g
				lpFInOut->SetPaletteEntry();		//�p���b�g���擾
			}
			break;
		
		case	WM_ACTIVATEAPP:	//�E�B���h�E�\���p
			//���݃E�B���h�E���A�N�e�B�u����A�N�e�B�u�����󂯎�郁�b�Z�[�W
			fActive=LOWORD(wParam);
			break;

		case	WM_DESTROY:
			//�E�B���h�E�j��
			
			ReleaseETC();
			ReleaseDI();
			ReleaseDM();
			ReleaseDS();
			ReleaseDD();
			
			PostQuitMessage(0);					//�v���O�����̏I����������
			break;
		
		default:
			//���̑�
			return	DefWindowProc(hwnd,message,wParam,lParam);
	}
	return 0;
}
