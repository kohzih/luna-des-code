/*====================================================================
	�G�L����or�X�v���C�g�I�u�W�F�N�g�̏����Ɋւ���\�[�X�t�@�C��
=====================================================================*/

#include	<windows.h>
#include	<ddraw.h>

#include	"Macros.h"
#include	"Draw.h"
#include	"Struct.h"
#include	"DrawSP.h"	
#include	"DrawSPData.h"
#include	"DrawMacros.h"

extern	LPDIRECTDRAWSURFACE7	lpDDSBack;

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

DrawSP::DrawSP()
{
	Teki01=NULL;
	Teki02=NULL;
	Boss01=NULL;
}

DrawSP::~DrawSP()
{	
	RELEASE_del(Teki01);
	RELEASE_del(Teki02);
	RELEASE_del(Boss01);
}

//////////////////////////////////////////////////////////////////////
// �G�L�����̎�ނɂ���Ă��ꂼ�ꃁ�������m�ۂ��e�����o�ϐ���������
//////////////////////////////////////////////////////////////////////
BOOL	DrawSP::InitSP(POINT cdn,WORD et)
{
	//�ď������̏ꍇ�K�v�ȏ���
	RELEASE_del(Teki01);
	RELEASE_del(Teki02);
	RELEASE_del(Boss01);

	//�G�L�����̎�ނ�������
	fEvt=(DWORD)et;		
	
	//���@�̃f�[�^��������
	JK_fJIKI=0;		//���@�̏�ԃt���O�̃R�s�[
	JK_Cdn.x=0;		//���@�̉�ʍ��W�̃R�s�[
	JK_Cdn.y=0;

	BOOL	ret=TRUE;
	
	//�G�L�����̎�ނɂ���ď������𕪂��܂�
	switch (fEvt){
		//�G�L����01�̏�����
		case 1:
			Teki01=new TEKI01;
			if(!Teki01)	return	FALSE;
			
			ret=Teki01->InitTeki01(cdn);
			if(!ret)	return	FALSE;

			break;
		//�G�L����02�̏�����
		case 2:
			Teki02=new TEKI02;
			if(!Teki02)	return	FALSE;
			
			ret=Teki02->InitTeki02(cdn);
			if(!ret)	return	FALSE;

			break;
		//�{�X�L����01�̏�����
		case 3:
			Boss01=new BOSS01;
			if(!Boss01)	return	FALSE;
			
			ret=Boss01->InitBoss01(cdn);
			if(!ret)	return	FALSE;

			break;
		default:
			break;
	}
	
	return	TRUE;
}

//////////////////////////////////////////////////////////////////////
// �G�L�����ʂɍ��W�𓮂�������
//////////////////////////////////////////////////////////////////////
void	DrawSP::MoveSP()
{
	//�X�e�[�W�t���O�ɉ����t���O�������Ă���ꍇ���^�[��
	if(fSTAGE&_fSTAGE_CHK)		return;	
	//�Q�[�����ꎞ��~������t���O�������Ă���ꍇ���^�[��
	if(JK_fJIKI&_GAME_STOP_ON)		return;
	
	switch(fEvt){
		case 1:
			Teki01->MovTeki01();
			break;
		case 2:
			Teki02->MovTeki02();
			break;
		case 3:
			Boss01->MovBoss01();
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////
// �G�̔��˂���e���Ǘ�����
//////////////////////////////////////////////////////////////////////
void	DrawSP::TamaSP()
{	
	//�X�e�[�W�t���O�ɉ����t���O�������Ă���ꍇ���^�[��
	if(fSTAGE&_fSTAGE_CHK)		return;	
	//�Q�[�����ꎞ��~������t���O�������Ă���ꍇ���^�[��
	if(JK_fJIKI&_GAME_STOP_ON)		return;
	
	switch(fEvt){
		case 1:
			Teki01->AttackTeki01();
			break;
		case 2:
			Teki02->AttackTeki02();
			break;
		case 3:
			Boss01->Attack1Boss01(JK_Cdn);
			Boss01->Attack2Boss01();
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////
//	�����œn���ꂽ�ϐ���
//		�C�x���g��ރt���O�A
//		�G�L������ԃt���O�A
//		�G�L�����̉�ʍ��W�A
//		�G�L�����̉����A
//		�G�L�����̏c���A
//		�G�L�����̑ϋv�͂̃A�h���X
//		�G�L�����̒e���Ǘ�����\���̂̃A�h���X
//		�G�L�����̒e���Ǘ�����\���̂̃A�h���X2
//		�G�L�����̒e���Ǘ�����\���̂̃A�h���X3
//	���R�s�[
//////////////////////////////////////////////////////////////////////
void	DrawSP::GetTekiData(DWORD	&fevt,	//�����Ɋe�l�����@�I�u�W�F�N�g�ɓn��
							DWORD	&fspchr,
							POINT	&cdn,
							short	&width,
							short	&height,
							short	*&shldcnt,
							TAMAS	*&tamas,
							TAMAS2	*&tamas2,
							TAMAS3	*&tamas3)
{
	fevt=fEvt;		//�C�x���g�t���O��n��
	
	switch(fEvt){
		case 1:
			fspchr	=Teki01->GetTeki01fSPChr();		//�G�L������ԃt���O
			cdn		=Teki01->GetTeki01Cdn();		//��ʍ��W
			width	=Teki01->GetTeki01Width();		//�G�L�����̉���
			height	=Teki01->GetTeki01Height();		//�G�L�����̏c��
			shldcnt	=Teki01->GetTeki01ShldCnt_P();	//�c��ϋv�͂̒l�ւ̃|�C���^
			tamas	=Teki01->GetTeki01TMsr_P();		//�e�̏�Ԃ��Ǘ�����\���̂ւ̃|�C���^
			tamas2	=NULL;
			tamas3	=NULL;
			break;
		case 2:
			fspchr	=Teki02->GetTeki02fSPChr();		//�G�L������ԃt���O
			cdn		=Teki02->GetTeki02Cdn();		//��ʍ��W
			width	=Teki02->GetTeki02Width();		//�G�L�����̉���
			height	=Teki02->GetTeki02Height();		//�G�L�����̏c��
			shldcnt	=Teki02->GetTeki02ShldCnt_P();	//�c��ϋv�͂̒l�ւ̃|�C���^
			tamas	=Teki02->GetTeki02TMsr_P();		//�e�̏�Ԃ��Ǘ�����\���̂ւ̃|�C���^
			tamas2	=NULL;
			tamas3	=NULL;
			break;
		case 3:
			fspchr	=Boss01->GetBoss01fSPChr();		//�{�X�L������ԃt���O
			cdn		=Boss01->GetBoss01Cdn();		//��ʍ��W
			width	=Boss01->GetBoss01Width();		//�{�X�L�����̉���
			height	=Boss01->GetBoss01Height();		//�{�X�L�����̏c��
			shldcnt	=Boss01->GetBoss01ShldCnt_P();	//�c��ϋv�͂̒l�ւ̃|�C���^
			tamas	=NULL;							//�e�̏�Ԃ��Ǘ�����\���̂ւ̃|�C���^
			tamas2	=Boss01->GetBoss01B_Msr_P();
			tamas3	=Boss01->GetBoss01B_Block_P();
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////
// �C�x���g�ʂɏ����𕪊򂳂��A�j���[�V���������ĕ`��̏���������֐�
//	����:�G�L�����̏�ԃt���O�A���@�̏�ԃt���O�A���@�̉�ʍ��W
//////////////////////////////////////////////////////////////////////
void	DrawSP::FrameSP(DWORD flag,DWORD fJ,POINT cdn)
{
	//���@�̏�ԃt���O���R�s�[
	JK_fJIKI=fJ;
	//���@�̍��W���R�s�[
	JK_Cdn=cdn;
	//�X�e�[�W�t���O�ɉ����t���O�������Ă���ꍇ���^�[��
	if(fSTAGE&_fSTAGE_CHK)		return;	
	//�Q�[�����ꎞ��~������t���O�������Ă���ꍇ���^�[��
	if(JK_fJIKI&_GAME_STOP_ON)		return;
	
	switch(fEvt){
		case 1:
			Teki01->FrameTeki01(flag);
			break;
		case 2:
			Teki02->FrameTeki02(flag);
			break;
		case 3:
			Boss01->FrameBoss01(flag);
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////
// �G�L�����̕`��
//////////////////////////////////////////////////////////////////////
void	DrawSP::RendSP()
{
	//�X�e�[�W�t���O�ɉ����t���O�������Ă���ꍇ���^�[��
	if(fSTAGE&_fSTAGE_CHK)		return;	
	
	switch(fEvt){
		case 1:
			Teki01->RendTeki01();
			break;
		case 2:
			Teki02->RendTeki02();
			break;
		case 3:
			Boss01->RendBoss01();
			break;
		default:
			break;
	}
}	

