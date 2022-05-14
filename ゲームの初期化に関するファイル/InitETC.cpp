/*====================================================================
	����I�u�W�F�N�g�������Ɋւ���\�[�X�t�@�C��
=====================================================================*/

#include	<windows.h>
#include	<ddraw.h>

#include	"Macros.h"
#include	"Globals2.h"

//---------------------------------------------------------
//�F�X�Ȏ���I�u�W�F�N�g�̏�����
//---------------------------------------------------------
BOOL	InitETC()
{
	HRESULT		ret;
	
	//�`��Ǘ��I�u�W�F�N�g�̍쐬
	char *FileName[]={
		//���@�摜�r�b�g�}�b�v�t�@�C����
		".\\BitMap\\Ch001.bmp",
		//�v�����[�O�p�r�b�g�}�b�v�t�@�C����
		".\\BitMap\\Prolog.bmp",	
		//�^�C�g���p�r�b�g�}�b�v�t�@�C����
		".\\BitMap\\TitleMain.bmp",".\\BitMap\\TitleMoji.bmp",
		//�X�e�[�W1�p�e�t�@�C����
		".\\BitMap\\Ch002.bmp",".\\BitMap\\Bg001.bmp",
		".\\BitMap\\Bg002.bmp",".\\Data\\stage1.mdt",
		//�Q�[���I�[�o�[��ʗp�r�b�g�}�b�v�t�@�C����
		".\\BitMap\\GameOver.bmp",
		//�Q�[���N���A��ʗp�r�b�g�}�b�v�t�@�C����
		".\\BitMap\\TheEnd.bmp"
	};
	
	lpDraw=new Draw(&FileName[0],ret);
	if(!lpDraw || !ret)		return FALSE;
	
	//���@�X�v���C�g�Ǘ��I�u�W�F�N�g�̍쐬
	lpJIKI=new DrawJIKI;
	if(!lpJIKI)		return	FALSE;

	//�G�L�����I�u�W�F�N�g�͂܂��쐬���Ȃ�
	lpSP=NULL;
	
	//�w�i�X�N���[���I�u�W�F�N�g�̍쐬
	lpBG=new DrawBG;
	if(!lpBG)		return	FALSE;

	//�t�F�[�h�C���A�E�g�I�u�W�F�N�g�̍쐬
	lpFInOut=new FaInOut(10,236);
	if(!lpFInOut)	return	FALSE;
	
	return	TRUE;
}

//---------------------------------------------------------
//����I�u�W�F�N�g���
//---------------------------------------------------------
void	ReleaseETC(void)
{	
	RELEASE_del(lpFInOut);	
	RELEASE_del(lpBG);
	RELEASE_del(lpJIKI);
	RELEASE_dels(lpSP);	
	lpDraw->ReleaseALL();
	RELEASE_del(lpDraw);
}
