/*====================================================================
	�Q�[���̃��C�������Ɋւ���\�[�X�t�@�C��
=====================================================================*/

#include <windows.h>
#include <ddraw.h>

#include "Macros.h"
#include "DrawMacros.h"
#include "FaInOut.h"
#include "Draw.h"
#include "DrawBG.h"
#include "Struct.h"
#include "DrawSP.h"
#include "DrawJIKI.h"
#include "DrawMacros.h"

extern HWND hWnd;				//���C���E�B���h�E�n���h��
extern BOOL fFullscreen;		//�t���X�N���[��(1)or�E�B���h�E�t���O(0)
extern DWORD DisplayColor;		//�X�N���[���̌��݂̃r�b�g�J���[�l
extern DWORD TotalMem, FreeMem; // VRAM�̋󂫗e�ʂ̒l���i�[

extern LPDIRECTDRAWSURFACE7 lpDDSPrimary, //�v���C�}���T�[�t�F�[�X
	lpDDSBack;							  //�o�b�N�T�[�t�F�[�X

extern FaInOut *lpFInOut; //�t�F�[�h�C���A�E�g�I�u�W�F�N�g
extern Draw *lpDraw;	  //��{�`��Ǘ��I�u�W�F�N�g
extern DrawSP *lpSP;	  //�G�L�����Ǘ��I�u�W�F�N�g
extern DrawJIKI *lpJIKI;  //���@�̃X�v���C�g�Ǘ��I�u�W�F�N�g
extern DrawBG *lpBG;	  //�w�i�Ǘ��I�u�W�F�N�g

//�֐��̃v���g�^�C�v�錾
void FrameRateDraw(void);						 //�t���[�����[�g�Œ�
void DrawText(char *, int, int, COLORREF);		 //�e�L�X�g�\��
void DrawText(char *, long, int, int, COLORREF); //�e�L�X�g+�ϐ��\��
BOOL InitSTAGE(void);							 //�X�e�[�W�̏���������

//---------------------------------------------------------
//�Q�[�����̃��C������
//---------------------------------------------------------
BOOL GameMain(void)
{
	//�v���C�}���T�[�t�F�X���X�g�`�F�b�N
	if (!lpDDSPrimary == NULL && lpDDSPrimary->IsLost() == DDERR_SURFACELOST)
		lpDDSPrimary->Restore();
	//�o�b�N�T�[�t�F�X�N���A
	DDBLTFX ddbltfx;
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = 0; //��
	ddbltfx.dwFillColor |= ddbltfx.dwFillColor << 8;
	ddbltfx.dwFillColor |= ddbltfx.dwFillColor << 16;
	lpDDSBack->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbltfx);

	BOOL ret = TRUE;

	//�Q�[���I���t���O�̃`�F�b�N
	if (lpDraw->GetfSTAGE() & _GAME_EXIT)
		return FALSE;

	//�e�X�e�[�W�̊J�n�A�I���A�C�x���g�̏���
	ret = InitSTAGE();
	if (!ret)
	{
		MessageBox(hWnd, "InitSTAGE", "ERROR", MB_OK | MB_APPLMODAL);
		return FALSE;
	}

	//�ȉ��Q�[���̃��C������
	lpBG->MoveBG();		//�w�i�X�N���[��
	lpJIKI->MoveJIKI(); //���@�̈ړ�

	for (int i = 0; (DWORD)i < lpDraw->EventCnt; i++)
	{
		lpSP[i].MoveSP(); //�G�L�����̈ړ�
		lpSP[i].TamaSP(); //�G�e�̏���
		//�G�L�����̏����擾
		lpJIKI->SP_Num = (DWORD)i;				//�G�L�����̔ԍ�
		lpSP[i].GetTekiData(lpJIKI->SP_fEvt,	//�G�L�����̎��
							lpJIKI->SP_fSPChr,	//�G�L�����̏��
							lpJIKI->SP_Cdn,		//�G�L�����̍��W
							lpJIKI->SP_Width,	//�G�L�����̉���
							lpJIKI->SP_Height,	//�G�L�����̏c��
							lpJIKI->SP_Shld,	//�G�L�����̑ϋv��
							lpJIKI->SP_Tamas,	//�G�L�����̔��˂����e���
							lpJIKI->SP_Tamas2,	//�G�L�����̔��˂����e���2
							lpJIKI->SP_Tamas3); //�G�L�����̔��˂����e���3
		//���@�Ƃ̓����蔻��𒲂ׂ���ɃL�����̕`�揀������
		lpSP[i].FrameSP(lpJIKI->AtariJIKI(), lpJIKI->GetfJIKI(), lpJIKI->GetJIKICdn());
	}

	lpBG->FrameBG(lpJIKI->GetfJIKI()); //�w�i�`�揀������
	lpJIKI->FrameJIKI();			   //���@�̕`�揀������

	lpBG->RendBG(); //�w�i�̕`��
	for (i = 0; (DWORD)i < lpDraw->EventCnt; i++)
		lpSP[i].RendSP(); //�G�L�����̕`��
	lpJIKI->RendJIKI();	  //���@�̕`��

#if DEBUG >= 2
	// VRAM�̋󂫗e�ʂ�\��
	DrawText("Tmem", TotalMem, 0, 118, RGB(0, 0, 0));
	DrawText("Fmem", FreeMem, 0, 136, RGB(0, 0, 0));
#endif

#if DEBUG >= 3
	//��ʂ̐F�֘A
	DrawText("DisplayColor", DisplayColor, 0, 154, RGB(0, 0, 0));
#endif

	//�t���[�����[�g�\�����Œ�
	FrameRateDraw();

	//�T�[�t�F�X�̃t���b�s���O�i�ŏI�o�́j
	RECT rcDst, rcSrc;

	if (fFullscreen)
		lpDDSPrimary->Flip(NULL, DDFLIP_WAIT);
	else
	{
		SetRect(&rcSrc, 0, 0, WinXMax, WinYMax);

		GetClientRect(hWnd, &rcDst);
		ClientToScreen(hWnd, (LPPOINT)&rcDst);
		ClientToScreen(hWnd, (LPPOINT)&rcDst + 1);

		ddbltfx.dwDDFX = DDBLTFX_NOTEARING;
		lpDDSPrimary->Blt(&rcDst, lpDDSBack, &rcSrc, DDBLT_WAIT, &ddbltfx);
	}

	return TRUE;
}