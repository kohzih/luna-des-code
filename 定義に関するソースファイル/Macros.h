/*====================================================================
	�v���O�����S�̂ɉe����^����}�N����`
=====================================================================*/

//�f�o�b�O�p�}�N��
#define	DEBUG	0						//0-�f�o�b�O����
										//1-�t���[�����[�g�\��,�Q�[�����[�v�G���[�\��
										//2-VRAM�󂫗e�ʕ\��
										//3-�F�̏��\��

//�E�B���h�E�Ɋւ���}�N��
#define	WinXMax	640						//�E�B���h�E��
#define	WinYMax	480						//�E�B���h�E��
#define	BPP		8						//BPP(�t���X�N���[���p)	

//DirextX�I�u�W�F�N�g���
#define	RELEASE(x)		if((x)){(x)->Release();x=NULL;}
//�I�u�W�F�N�g���(�ϐ�1�p)
#define	RELEASE_del(a)	if((a)){delete	(a);a=NULL;}
//�I�u�W�F�N�g���(�z��p)
#define	RELEASE_dels(b)	if((b)){delete	[](b);b=NULL;}