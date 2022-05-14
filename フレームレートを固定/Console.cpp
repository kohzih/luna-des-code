/*====================================================================
	�����̓��o�͂ƃt���[�����[�g�Œ�֐��̃\�[�X�t�@�C��
=====================================================================*/

#include	<windows.h>
#include	<ddraw.h>

#include	"Macros.h"

extern	LPDIRECTDRAWSURFACE7	lpDDSBack;			//�o�b�N�T�[�t�F�[�X

//---------------------------------------------------------
//�e�L�X�g�\���֐�
//---------------------------------------------------------
void	DrawText(char *text,int x,int y,COLORREF rgb)
{
	HDC		hdc;
	lpDDSBack->GetDC(&hdc);
	SetTextColor(hdc,rgb);
	SetBkMode(hdc,OPAQUE);			//�w�i�����@TRANSPARENT�@�w��
	TextOut(hdc,x,y,text,lstrlen(text));
	lpDDSBack->ReleaseDC(hdc);
}

//---------------------------------------------------------
//�ϐ��\���֐�
//---------------------------------------------------------
void	DrawText(char *text,long d,int x,int y,COLORREF rgb)
{
	HDC		hdc;
	char	msg[30];		

	lpDDSBack->GetDC(&hdc);
	
	SetTextColor(hdc,rgb);
	SetBkMode(hdc,OPAQUE);			//�w�i�s����
	wsprintf(msg,"%s=(%d)",text,d);
	TextOut(hdc,x,y,msg,lstrlen(msg));
	
	lpDDSBack->ReleaseDC(hdc);
}

//---------------------------------------------------------
//�t���[�����[�g�Œ聕�\���֐�
//---------------------------------------------------------
void	FrameRateDraw(void)
{
	static	DWORD	framerate;
	static	DWORD	timeNew=0,timeOld=0,timeOld2=0;
					//���݂̌o�ߎ��ԁA�P�b���r�p���ԁA�P�^�U�O�b��r�p����

#if	DEBUG >=1
			HDC		hdc;
	static	char	string[64];
#endif

	framerate	++;					//�t���[���J�E���g���P���₷
	
	while(timeNew-timeOld2<16)		//1/60�b�o�߂��Ă��邩�`�F�b�N
	{								//�o�߂���܂Ń��[�v����
		timeNew=timeGetTime();
	}
	timeOld2=timeNew;

	if(timeNew-timeOld>1000)		//�P�b�Ԃɉ���t���[���J�E���g���A�b�v�������\��
	{

#if	DEBUG >=1		
		wsprintf(string,"%dFPS   ",framerate);
#endif		
		framerate=0;
		timeOld=timeNew;
	}
	
#if	DEBUG >= 1
	lpDDSBack->GetDC(&hdc);	
	TextOut(hdc,0,100,string,lstrlen(string));
	lpDDSBack->ReleaseDC(hdc);	
#endif
	
}

