/*=============================================================================
	���@�{�̂Ɋւ��鏉�����A�����蔻��A�`�擙�̃\�[�X�t�@�C��
==============================================================================*/

#include	<windows.h>
#include	<math.h>
#include	<ddraw.h>
#include	<dsound.h>

#include	"Macros.h"
#include	"Draw.h"	
#include	"Struct.h"
#include	"DrawJIKI.h"
#include	"DrawJIKIDataEXT.h"
#include	"DrawMacros.h"

extern	LPDIRECTDRAWSURFACE7	lpDDSBack;		//�o�b�N�T�[�t�F�X

//�_�C���N�g�T�E���h�o�b�t�@�Đ��p�}�N����`
#define	_JIKIBAKU	0
#define	_TEKIBAKU	1
#define	_FUELARM	2
#define	_FIRE		3
#define	_TAMAKYU	4
#define	_BOSSBAKU	5

extern	LPDIRECTSOUNDBUFFER		lpDSBuf[6]; //�_�C���N�g�T�E���h�o�b�t�@

//�W���C�X�e�B�N�֘A
extern	BYTE	JoyNew,JoyOld;
BYTE	Joy_Get(void);

//////////////////////////////////////////////////////////////////////
//	���@�Ɋւ���l�X�ȃf�[�^������������
//////////////////////////////////////////////////////////////////////
BOOL	DrawJIKI::InitJikiData()
{	
	
	fJIKI=0;		//���@�̏�ԃt���O��������
	Target=0;		//�j��ڕW�ɂȂ�G�L�����ԍ��̏�����
	InvincCnt=0;	//���G���Ԃ̃J�E���^������
	ContinueCnt=0;	//�R���e�B�j���[�J�E���^��������

	//�X�e�[�W�ԍ��ɂ���ď����t���O��ω�������
	switch(StageNum){
		case 0:
			break;
		case 1:
			break;
		case 2:	
			Target=0;		//�j�󂷂�ƃX�e�[�W�N���A�ɂȂ�G�L�����̔ԍ�������
			break;
		default:
			return	FALSE;
			break;
	}

	//���@�̉�ʍ��W��������(���[�̐^������X�^�[�g)
	Cdn.x=(WinXMax/2)-(_JIKI_W/2);
	Cdn.y=WinYMax-32;
	
	//���@�̐�΍��W��������
	MapCdn.x=MainCdn.x+Cdn.x;
	MapCdn.y=MainCdn.y+Cdn.y;
	
	//���@�̈ړ����x��������
	Mov.x=_JIKIMOVE;
	Mov.y=_JIKIMOVE;

	//EXP�l�ƒe�z���Ɏg�p����R���̏�����
	EXP=0;		
	EXP_R=EXP<<16;
	FUEL=0;
	FUEL_R=FUEL<<16;

	//�G�L�����̏���������
	SP_Num=0;		//�G�L�����̔ԍ�
	SP_fEvt=0;		//�G�L�����̎��
	SP_fSPChr=0;	//�G�L�����̏��
	SP_Cdn.x=0;		//�G�L�����̍��W
	SP_Cdn.y=0;
	SP_Width=0;		//�G�L�����̉���
	SP_Height=0;	//�G�L�����̏c��
	SP_Shld=NULL;	//�G�L�����̎c��ϋv�͂ւ̃|�C���^
	SP_Tamas=NULL;	//�G�L�����̒e�̏�Ԃ��Ǘ�����TAMAS�\���̔z��̃|�C���^
	SP_Tamas2=NULL;//�G�L�����̒e�̏�Ԃ��Ǘ�����TAMAS2�\���̔z��̃|�C���^
	SP_Tamas3=NULL;//�G�L�����̒e�̏�Ԃ��Ǘ�����TAMAS3�\���̔z��̃|�C���^
	
	//////////////////////////////////////////////////////////////////////
	//-------------���@�̑����Ɋւ���\���̂̏�����-----------------------

	Wpd.flag=0;
	Wpd.LSR=0;	
	Wpd.MSR=0;
	Wpd.HMM=0;	
	Wpd.HNB=0;
	Wpd.OPT=0;
	Wpd.LSR_temp=0;
	Wpd.MSR_temp=0;
	Wpd.HMM_temp=0;
	Wpd.HNB_temp=0;	
	Wpd.OPT_temp=0;		

	//////////////////////////////////////////////////////////////////////
	//----------�A�[�����ˎ��̎��@�A�j���[�V�����f�[�^�̏�����------------

	//�A�j���p�^�[�����̏�����
	ArmJIKI.Anime=AmeJIKI[0];					
	//�A�j���p�^�[���z��̏�����
	ArmJIKI.AnimePat=new BYTE[ArmJIKI.Anime];
	if(!ArmJIKI.AnimePat)	return	FALSE;
	memcpy(ArmJIKI.AnimePat,&PatJIKI[0][0],sizeof(BYTE)*ArmJIKI.Anime);
	//�A�j���؂�ւ��^�C�~���O�z��̏�����
	ArmJIKI.AnimeFrame=new BYTE[ArmJIKI.Anime];
	if(!ArmJIKI.AnimeFrame)	return	FALSE;
	memcpy(ArmJIKI.AnimeFrame,&FramJIKI[0][0],sizeof(BYTE)*ArmJIKI.Anime);
	//���ݕ\������A�j���p�^�[���̏�����
	ArmJIKI.AnimeCnt=0;
	//���݂̃A�j���[�V�����t���[���̏�����
	ArmJIKI.FrameCnt=ArmJIKI.AnimeFrame[0];

	//////////////////////////////////////////////////////////////////////
	//---------�A�[�����ˎ��̃A�[���A�j���[�V�����f�[�^�̏�����-----------
	
	Arm.Anime=AmeJIKI[1];					
	Arm.AnimePat=new BYTE[Arm.Anime];
	if(!Arm.AnimePat)		return	FALSE;
	memcpy(Arm.AnimePat,&PatJIKI[1][0],sizeof(BYTE)*Arm.Anime);
	Arm.AnimeFrame=new BYTE[Arm.Anime];
	if(!Arm.AnimeFrame)		return	FALSE;
	memcpy(Arm.AnimeFrame,&FramJIKI[1][0],sizeof(BYTE)*Arm.Anime);
	Arm.AnimeCnt=0;
	Arm.FrameCnt=Arm.AnimeFrame[0];

	//////////////////////////////////////////////////////////////////////
	//---------���@�����Ă��ꂽ���̃A�j���[�V�����f�[�^�̏�����	---------
	
	Baku.Anime=AmeJIKI[2];
	Baku.AnimePat=new BYTE[Baku.Anime];
	if(!Baku.AnimePat)		return	FALSE;
	memcpy(Baku.AnimePat,&PatJIKI[2][0],sizeof(BYTE)*Baku.Anime);
	Baku.AnimeFrame=new BYTE[Baku.Anime];
	if(!Baku.AnimeFrame)	return	FALSE;
	memcpy(Baku.AnimeFrame,&FramJIKI[2][0],sizeof(BYTE)*Baku.Anime);
	Baku.AnimeCnt=0;
	Baku.FrameCnt=Baku.AnimeFrame[0];
	
	//////////////////////////////////////////////////////////////////////
	//---------�e�z�����̃A�j���[�V�����f�[�^�̏�����---------------------	
	
	TmKySy.Anime=AmeJIKI[3];					
	TmKySy.AnimePat=new BYTE[TmKySy.Anime];
	if(!TmKySy.AnimePat)		return	FALSE;
	memcpy(TmKySy.AnimePat,&PatJIKI[3][0],sizeof(BYTE)*TmKySy.Anime);
	TmKySy.AnimeFrame=new BYTE[TmKySy.Anime];
	if(!TmKySy.AnimeFrame)	return	FALSE;
	memcpy(TmKySy.AnimeFrame,&FramJIKI[3][0],sizeof(BYTE)*TmKySy.Anime);
	TmKySy.AnimeCnt=0;
	TmKySy.FrameCnt=TmKySy.AnimeFrame[0];

	//////////////////////////////////////////////////////////////////////
	//--------------�T�[�t�F�X����]�������`�͈͂̏�����----------------
	
	//�����̎��@�̋�`�͈�
	JIKIChr=SpJIKI[0];
	
	//FUEL�Q�[�W�̌��݂̋�`��������
	GAGE_F.left   =SpJIKI[22].left+(128-FUEL);
	GAGE_F.top    =SpJIKI[22].top;
	GAGE_F.right  =SpJIKI[22].right;
	GAGE_F.bottom =SpJIKI[22].bottom;
	//FUEL�Q�[�W�̋󔒕����̋�`��������
	GAGE_FK=SpJIKI[21];	
	//FUEL�Q�[�W�̘g�����̋�`��������
	GAGE_FW=SpJIKI[20];
	
	//EXP�Q�[�W�̌��݂̋�`��������
	GAGE_E.left   =SpJIKI[19].left;
	GAGE_E.top    =SpJIKI[19].top;
	GAGE_E.right  =SpJIKI[19].right-(128-EXP);
	GAGE_E.bottom =SpJIKI[19].bottom;
	//EXP�Q�[�W�̋󔒕����̋�`��������
	GAGE_EK=SpJIKI[18];	
	//EXP�Q�[�W�̘g�����̋�`��������
	GAGE_EW=SpJIKI[17];	
	
	//�X�R�A�ƕ\����`�͈͂�������
	SCORE=0;		//�X�R�A�̏�����
	ScoreCalcul();	//�X�R�A�̕\����`�͈͂�������
	
	return	TRUE;
}

//////////////////////////////////////////////////////////////////////
//	���I�Ɋm�ۂ��郁�����̈�ւ̃|�C���^��NULL�ŏ�����
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::InitJikiTm()
{
	ArmJIKI.AnimePat	=NULL;
	ArmJIKI.AnimeFrame	=NULL;
	Arm.AnimePat		=NULL;
	Arm.AnimeFrame		=NULL;
	Baku.AnimePat		=NULL;
	Baku.AnimeFrame		=NULL;
	TmKySy.AnimePat		=NULL;
	TmKySy.AnimeFrame	=NULL;
}

//////////////////////////////////////////////////////////////////////
//	�m�ۂ����������̈�����
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::JikiTmRelease()
{
	RELEASE_dels(TmKySy.AnimeFrame);
	RELEASE_dels(TmKySy.AnimePat);
	RELEASE_dels(Baku.AnimeFrame);
	RELEASE_dels(Baku.AnimePat);
	RELEASE_dels(Arm.AnimeFrame);
	RELEASE_dels(Arm.AnimePat);
	RELEASE_dels(ArmJIKI.AnimeFrame);
	RELEASE_dels(ArmJIKI.AnimePat);
}

//////////////////////////////////////////////////////////////////////
//	�����œn���ꂽ�l�����ɓ����蔻����s���A���������ꍇFALSE��Ԃ�
//////////////////////////////////////////////////////////////////////
BOOL	DrawJIKI::Atari(RECT JIKI)
{
	if(JIKI.top>SP_Cdn.y+(SP_Height-4));
	else if(JIKI.left>SP_Cdn.x+(SP_Width-4));
	else if(JIKI.right<SP_Cdn.x+4);
	else if(JIKI.bottom>SP_Cdn.y-4)	return FALSE;
	
	return	TRUE;
}

//////////////////////////////////////////////////////////////////////
//	�����œn���ꂽ�l�����ɓ����蔻����s���A���������ꍇFALSE��Ԃ�
//////////////////////////////////////////////////////////////////////
BOOL	DrawJIKI::Atari(RECT JIKI,RECT TEKI)
{
	if(JIKI.top>TEKI.bottom);
	else if(JIKI.left>TEKI.right);
	else if(JIKI.right<TEKI.left);
	else if(JIKI.bottom>TEKI.top)	return FALSE;

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////
//	���@�ƓG�̒e�Ƃ̓����蔻����s��
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::JikiTekitamaAtari()
{
	RECT	Jiki,JikiA,Teki;	//�����蔻��Ɏg�p����
	
	//���@�̌��݂̉�ʍ��W��������(+**�͓����蔻��̗V��)
	Jiki.top=Cdn.y+16;
	Jiki.left=Cdn.x+6;
	Jiki.right=Cdn.x+_JIKI_W-6;
	Jiki.bottom=Cdn.y+24;
	
	//���@�̃A�[�������̉�ʍ��W��������
	JikiA.top=Cdn.y-(Arm.SrcR.bottom-Arm.SrcR.top)+16;
	JikiA.left=Cdn.x-4;
	JikiA.right=Cdn.x+24;
	JikiA.bottom=Cdn.y;
	
	short i=0;
	
	//SP_Tamas�Ƃ̓����蔻��
	if(SP_Tamas && SP_fSPChr&_TAMA1ON){
		for(i=0;(DWORD)i<SP_Tamas->TTama;i++){	
			//���˃t���O�̗����Ă��Ȃ��e�Ƃ̓����蔻��͍s��Ȃ�
			if(!(SP_Tamas->tm[i].fTama&_TON))	continue;
			//���@�t���O�ɓG�L�������Ńt���O�������Ă���ꍇ�S�Ă̒e�����ł�����
			else if(fJIKI&_PWUPD_ON){
				//------------------�G�e����-------------------------------------//
				
				SP_Tamas->tm[i].fTama=_TOFF;	//�e������
		
				//---------------------------------------------------------------//
				continue;						//���̒e������
			}
					
			//���݂��̉�ʍ��W�œ����蔻����s��
			
			//�G�L�����̒e���W�̏�����
			Teki.top=SP_Tamas->tm[i].Cdn.y+2;
			Teki.left=SP_Tamas->tm[i].Cdn.x+2;
			Teki.right=(SP_Tamas->tm[i].Cdn.x+SP_Tamas->tm[i].Width)-2;
			Teki.bottom=(SP_Tamas->tm[i].Cdn.y+SP_Tamas->tm[i].Height)-2;
			
			//�ŏ��ɃA�[���Ƃ̓����蔻����s��
			//�A�[�����˒��ł���Γ����菈�����s��
			if(!(fJIKI&_ARM_ON));
			else if(!Atari(JikiA,Teki)){
				//�A�[���ɐڐG�����e�����ł�����
				//------------------�G�e����-------------------------------------//
			
				SP_Tamas->tm[i].fTama=_TOFF;	//�e������
			
				//---------------------------------------------------------------//

				//�A�[���̔R���z���p�����ɐڐG���ď��ł��Ă����ꍇ
				if((Cdn.y-(Arm.SrcR.bottom-Arm.SrcR.top)+16)+32>SP_Tamas->tm[i].Cdn.y+8)
				{
					FUEL_R=FUEL_R+_FUELUP;				//FUEL�l�𑝂₷
					if(FUEL_R>128<<16)	FUEL_R=128<<16;	//128�ȏ�ɂȂ�Ȃ��悤�ݒ�
					FUEL=(BYTE)(FUEL_R>>16);
					SCORE	+=10;		//�X�R�A�A�b�v
					//�e�z���T�E���h
					lpDSBuf[_FUELARM]->Stop();
					lpDSBuf[_FUELARM]->SetCurrentPosition(0);
					lpDSBuf[_FUELARM]->Play(0,0,0);
				}
			}
			
			//�������玩�@�Ƃ̓����蔻��
			//�������Ă���ꍇ�A���@�ƒe�����ł����ă��^�[��
			if(!Atari(Jiki,Teki)){
				//�e�z�����ȊO�ɐڐG�����ꍇ�_���[�W���󂯂�
				if(!(fJIKI&_FUEL_ON)){
					if(!(fJIKI&_INVINCIBLE_ON)){
						//------------------�G�e����-------------------------------------//
						
						SP_Tamas->tm[i].fTama=_TOFF;	//�e������
				
						//---------------------------------------------------------------//
					
						EXP_R -=_EXPSH;				//EXP�l����_���[�W��������
						if(EXP_R<0)	EXP_R=0;		//EXP_R��0�ȉ��ɂȂ�Ȃ��悤�C��	
						EXP=(BYTE)(EXP_R>>16);
						//�e�ɓ���������EXP��0�̏ꍇ�͌���
						if(!EXP){					
							fJIKI |=_END_ON; //------------------------------------------------------
							//���@���ăT�E���h	
							lpDSBuf[_JIKIBAKU]->Stop();
							lpDSBuf[_JIKIBAKU]->SetCurrentPosition(0);
							lpDSBuf[_JIKIBAKU]->Play(0,0,0);
							return;
						}
					}
					continue;
				}
				//------------------�G�e����-------------------------------------//
				
				SP_Tamas->tm[i].fTama=_TOFF;	//�e������
				
				//---------------------------------------------------------------//
				
				//�e���z�������ꍇEXP�l���㏸������
				EXP_R +=_EXPUP;						//EXP�l�㏸
				if(EXP_R>128<<16)	EXP_R=128<<16;	//128�ȏ�ɂȂ�Ȃ��悤�ݒ�
				EXP=(BYTE)(EXP_R>>16);
				
				//�e�z���T�E���h
				lpDSBuf[_FUELARM]->Stop();
				lpDSBuf[_FUELARM]->SetCurrentPosition(0);
				lpDSBuf[_FUELARM]->Play(0,0,0);
				
				//EXP��128���܂����ꍇ�A���@���p���[�A�b�v
				if(EXP==128){
					fJIKI |=_GAME_STOP_ON;	//�Q�[�����ꎞ��~������t���O�I��
					fJIKI |=_PWUP_ON;		//�p���[�A�b�v���t���O�I��
					InitPWUP();				//�p���[�A�b�v��ʂ̏�����
					return;
				}
				continue;
			}
			
			//�e�z�����ł���ꍇ�A�e�̈ړ����������@�Ɍ����鏈��
			if(fJIKI&_FUEL_ON){
				double	a,b,c;	//��Ɨp
				b=(MapCdn.x+6)-SP_Tamas->tm[i].MapCdn.x;
				c=(MapCdn.y+10)-SP_Tamas->tm[i].MapCdn.y;
				a=(b*b)+(c*c);	//�]���藝
				a=sqrt(a);		//a�̕����������߂�
				SP_Tamas->tm[i].Mov.x=b*_FUELMV/a;
				SP_Tamas->tm[i].Mov.y=c*_FUELMV/a;
			}		
			//�e�z�����f�t���O�������Ă���ꍇ�z�����̒e�̈ړ����x��ω�������
			else if(fJIKI&_FUELD_ON){
				SP_Tamas->tm[i].Mov.x=0.0;
				SP_Tamas->tm[i].Mov.y=2.0;
			}
		}
	}

	//SP_Tamas2�Ƃ̓����蔻��
	if(SP_Tamas2 && SP_fSPChr&_TAMA2ON){
		for(i=0;(DWORD)i<SP_Tamas2->TTama;i++){	
			//���˃t���O�̗����Ă��Ȃ��e�Ƃ̓����蔻��͍s��Ȃ�
			if(!(SP_Tamas2->tm2[i].fTama&_TON))	continue;
			//���@�t���O�ɓG�L�������Ńt���O�������Ă���ꍇ�S�Ă̒e�����ł�����
			else if(fJIKI&_PWUPD_ON){
				//------------------�G�e����-------------------------------------//
				
				SP_Tamas2->tm2[i].fTama=_TOFF;	//�e������
			
				//---------------------------------------------------------------//
				continue;						//���̒e������
			}
					
			//���݂��̉�ʍ��W�œ����蔻����s��
			
			//�G�L�����̒e���W�̏�����
			Teki.top=SP_Tamas2->tm2[i].Cdn.y+2;
			Teki.left=SP_Tamas2->tm2[i].Cdn.x+2;
			Teki.right=(SP_Tamas2->tm2[i].Cdn.x+SP_Tamas2->tm2[i].Width)-2;
			Teki.bottom=(SP_Tamas2->tm2[i].Cdn.y+SP_Tamas2->tm2[i].Height)-2;
			
			//�ŏ��ɃA�[���Ƃ̓����蔻����s��
			//�A�[�����˒��ł���Γ����菈�����s��
			if(!(fJIKI&_ARM_ON));
			else if(!Atari(JikiA,Teki)){
				//�A�[���ɐڐG�����e�����ł�����
				//------------------�G�e����-------------------------------------//
				
				SP_Tamas2->tm2[i].fTama=_TOFF;	//�e������
				
				//---------------------------------------------------------------//

				//�A�[���̔R���z���p�����ɐڐG���ď��ł��Ă����ꍇ
				if((Cdn.y-(Arm.SrcR.bottom-Arm.SrcR.top)+16)+32>SP_Tamas2->tm2[i].Cdn.y+8)
				{
					FUEL_R=FUEL_R+_FUELUP;				//FUEL�l�𑝂₷
					if(FUEL_R>128<<16)	FUEL_R=128<<16;	//128�ȏ�ɂȂ�Ȃ��悤�ݒ�
					FUEL=(BYTE)(FUEL_R>>16);
					SCORE	+=10;		//�X�R�A�A�b�v
					//�e�z���T�E���h
					lpDSBuf[_FUELARM]->Stop();
					lpDSBuf[_FUELARM]->SetCurrentPosition(0);
					lpDSBuf[_FUELARM]->Play(0,0,0);
				}
			}
			
			//�������玩�@�Ƃ̓����蔻��
			//�������Ă���ꍇ�A���@�ƒe�����ł����ă��^�[��
			if(!Atari(Jiki,Teki)){
				//�e�z�����ȊO�ɐڐG�����ꍇ�_���[�W���󂯂�
				if(!(fJIKI&_FUEL_ON)){
					if(!(fJIKI&_INVINCIBLE_ON)){
						//------------------�G�e����-------------------------------------//
						
						SP_Tamas2->tm2[i].fTama=_TOFF;	//�e������
					
						//---------------------------------------------------------------//
					
						EXP_R -=_EXPSH;				//EXP�l����_���[�W��������
						if(EXP_R<0)	EXP_R=0;		//EXP_R��0�ȉ��ɂȂ�Ȃ��悤�C��	
						EXP=(BYTE)(EXP_R>>16);
						//�e�ɓ���������EXP��0�̏ꍇ�͌���
						if(!EXP){					
							fJIKI |=_END_ON; //------------------------------------------------------
							//���@���ăT�E���h	
							lpDSBuf[_JIKIBAKU]->Stop();
							lpDSBuf[_JIKIBAKU]->SetCurrentPosition(0);
							lpDSBuf[_JIKIBAKU]->Play(0,0,0);
							return;
						}
					}
					continue;
				}
				//------------------�G�e����-------------------------------------//
				
				SP_Tamas2->tm2[i].fTama=_TOFF;	//�e������
	
				//---------------------------------------------------------------//
				//�e���z�������ꍇEXP�l���㏸������
				EXP_R +=_EXPUP;						//EXP�l�㏸
				if(EXP_R>128<<16)	EXP_R=128<<16;	//128�ȏ�ɂȂ�Ȃ��悤�ݒ�
				EXP=(BYTE)(EXP_R>>16);
				
				//�e�z���T�E���h
				lpDSBuf[_FUELARM]->Stop();
				lpDSBuf[_FUELARM]->SetCurrentPosition(0);
				lpDSBuf[_FUELARM]->Play(0,0,0);
				
				//EXP��128���܂����ꍇ�A���@���p���[�A�b�v
				if(EXP==128){
					fJIKI |=_GAME_STOP_ON;	//�Q�[�����ꎞ��~������t���O�I��
					fJIKI |=_PWUP_ON;		//�p���[�A�b�v���t���O�I��
					InitPWUP();				//�p���[�A�b�v��ʂ̏�����
					return;
				}
				continue;
			}
			
			//�e�z�����ł���ꍇ�A�e�̈ړ����������@�Ɍ����鏈��
			if(fJIKI&_FUEL_ON){
				double	a,b,c;	//��Ɨp
				b=(MapCdn.x+6)-SP_Tamas2->tm2[i].MapCdn.x;
				c=(MapCdn.y+10)-SP_Tamas2->tm2[i].MapCdn.y;
				a=(b*b)+(c*c);	//�]���藝
				a=sqrt(a);		//a�̕����������߂�
				SP_Tamas2->tm2[i].Mov.x=b*_FUELMV/a;
				SP_Tamas2->tm2[i].Mov.y=c*_FUELMV/a;
			}		
			//�e�z�����f�t���O�������Ă���ꍇ�z�����̒e�̈ړ����x��ω�������
			else if(fJIKI&_FUELD_ON){
				SP_Tamas2->tm2[i].Mov.x=0.0;
				SP_Tamas2->tm2[i].Mov.y=2.0;
			}
		}
	}

	//SP_Tamas3�Ƃ̓����蔻��
	if(SP_Tamas3 && SP_fSPChr&_TAMA3ON){
		for(i=0;(DWORD)i<SP_Tamas3->TTama;i++){	
			//���˃t���O�̗����Ă��Ȃ��e�Ƃ̓����蔻��͍s��Ȃ�
			if(!(SP_Tamas3->tm3[i].fTama&_TON))	continue;
			//���@�t���O�ɓG�L�������Ńt���O�������Ă���ꍇ�S�Ă̒e�����ł�����
			else if(fJIKI&_PWUPD_ON){
				//------------------�G�e�Ƀ_���[�W-------------------------------//
				SP_Tamas3->tm3[i].ShldCnt -=_PWUPDDMG;
				if(SP_Tamas3->tm3[i].ShldCnt<=0){
					SP_Tamas3->tm3[i].fTama &=_TOFF;
					SP_Tamas3->tm3[i].fTama |=_TBAKU_ON;

					SCORE +=50;	//�X�R�A�A�b�v
					//�G�L�����j��T�E���h
					lpDSBuf[_TEKIBAKU]->Stop();		
					lpDSBuf[_TEKIBAKU]->SetCurrentPosition(0);
					lpDSBuf[_TEKIBAKU]->Play(0,0,0);
				}else{
					SP_Tamas3->tm3[i].fTama |=_THIT_ON;
				}
				//---------------------------------------------------------------//
				continue;						//���̒e������
			}
					
			//���݂��̉�ʍ��W�œ����蔻����s��
			
			//�G�L�����̒e���W�̏�����
			Teki.top=SP_Tamas3->tm3[i].Cdn.y+2;
			Teki.left=SP_Tamas3->tm3[i].Cdn.x+2;
			Teki.right=(SP_Tamas3->tm3[i].Cdn.x+SP_Tamas3->tm3[i].Width)-2;
			Teki.bottom=(SP_Tamas3->tm3[i].Cdn.y+SP_Tamas3->tm3[i].Height)-2;
			
			//�ŏ��ɃA�[���Ƃ̓����蔻����s��
				
			//�A�[�����˒��ł���Γ����菈�����s��
			if(!(fJIKI&_ARM_ON));
			else if(!Atari(JikiA,Teki)){
				//�A�[���ɐڐG�����e�̓_���[�W
				//------------------�G�e����-------------------------------------//
				SP_Tamas3->tm3[i].ShldCnt -=_ARMDMG;
				if(SP_Tamas3->tm3[i].ShldCnt<=0){
					SP_Tamas3->tm3[i].fTama &=_TOFF;
					SP_Tamas3->tm3[i].fTama |=_TBAKU_ON;

					SCORE +=50;	//�X�R�A�A�b�v
					//�G�e�j��T�E���h
					lpDSBuf[_TEKIBAKU]->Stop();		
					lpDSBuf[_TEKIBAKU]->SetCurrentPosition(0);
					lpDSBuf[_TEKIBAKU]->Play(0,0,0);
				}else{
					SP_Tamas3->tm3[i].fTama |=_THIT_ON;
				}
				//---------------------------------------------------------------//
			}
			
			//�������玩�@�Ƃ̓����蔻��
			//�������Ă���ꍇ�A���@�ƒe�����ł����ă��^�[��
			if(!Atari(Jiki,Teki)){
				if(!(fJIKI&_INVINCIBLE_ON)){
					fJIKI |=_END_ON; //------------------------------------------------------
				//	//���@���ăT�E���h	
					lpDSBuf[_JIKIBAKU]->Stop();
					lpDSBuf[_JIKIBAKU]->SetCurrentPosition(0);
					lpDSBuf[_JIKIBAKU]->Play(0,0,0);
					return;
				}
			}
			continue;
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	���@�ƓG�L�����Ƃ̓����蔻��
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::JikiTekiAtari()
{
	RECT	Jiki,JikiA;
	
	//���@�̌��݂̉�ʍ��W��������(+**�͓����蔻��̗V��)
	Jiki.top=Cdn.y+16;
	Jiki.left=Cdn.x+6;
	Jiki.right=Cdn.x+_JIKI_W-6;
	Jiki.bottom=Cdn.y+24;
	
	//���@�̃A�[�������̉�ʍ��W��������
	JikiA.top=Cdn.y-(Arm.SrcR.bottom-Arm.SrcR.top)+16;
	JikiA.left=Cdn.x-4;
	JikiA.right=Cdn.x+24;
	JikiA.bottom=Cdn.y;
	
	//�G�L�����ƃA�[���Ƃ̓����蔻��
	if(!(fJIKI&_ARM_ON));
	else if(!Atari(JikiA)){
		JikiTekiDmg(_ARMDMG);
		if(SP_fSPChr&_SPDES){
			//�����A�[���̔R���z���p�����ɐڐG���ď��ł��Ă����ꍇ
			if((Cdn.y-(Arm.SrcR.bottom-Arm.SrcR.top)+16)+32>SP_Cdn.y+32){
				FUEL_R=FUEL_R+(_FUELUP*2);	//�R���𑝂₷
				if(FUEL_R>128<<16)	FUEL_R=128<<16;	//128�ȏ�ɂȂ�Ȃ��悤�ɂ���
				FUEL=(BYTE)(FUEL_R>>16);
			}
			return;
		}
	}	

	//�G�L�����Ǝ��@�Ƃ̓����蔻��
	if(!Atari(Jiki) && !(fJIKI&_INVINCIBLE_ON)){		
		fJIKI |=_END_ON; //----------------------------------------------------------------------
		//���@���ăT�E���h
		lpDSBuf[_JIKIBAKU]->Stop();
		lpDSBuf[_JIKIBAKU]->SetCurrentPosition(0);
		lpDSBuf[_JIKIBAKU]->Play(0,0,0);
	}
}

//////////////////////////////////////////////////////////////////////
//	�����œn���ꂽ�_���[�W�ʂ�G�L�����ɗ^����
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::JikiTekiDmg(int DMG)
{
	*SP_Shld =*SP_Shld-DMG;		//�_���[�W��������
	//�ϋv�͂�0��؂����ꍇ
	if(*SP_Shld<=0){	
		SP_fSPChr |=_SPDES;		//�j��t���O�I��
		
		//�G�L�����j��T�E���h
		lpDSBuf[_TEKIBAKU]->Stop();		
		lpDSBuf[_TEKIBAKU]->SetCurrentPosition(0);
		lpDSBuf[_TEKIBAKU]->Play(0,0,0);
		
		switch(SP_fEvt){
			case 1:
				SCORE +=100;	//�X�R�A�A�b�v
				break;
			case 2:
				SCORE +=200;
				break;
			case 3:
				SCORE +=10000;
				break;
			default:
				break;
		}
	}else{
		SP_fSPChr |=_HITON;		//�_���[�W�t���O�I��
	}
}

//////////////////////////////////////////////////////////////////////
//	�����œn���ꂽ�_���[�W�ʂ�G�e�ɗ^����
//	����:�_���[�W�ʁA�ΏۂƂȂ�G�e
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::JikiTamas3Dmg(int DMG,short i)
{
	SP_Tamas3->tm3[i].ShldCnt -=DMG;
	if(SP_Tamas3->tm3[i].ShldCnt<=0){
		SP_Tamas3->tm3[i].fTama &=_TOFF;
		SP_Tamas3->tm3[i].fTama |=_TBAKU_ON;
		SCORE +=50;	//�X�R�A�A�b�v
		//�G�L�����j��T�E���h
		lpDSBuf[_TEKIBAKU]->Stop();		
		lpDSBuf[_TEKIBAKU]->SetCurrentPosition(0);
		lpDSBuf[_TEKIBAKU]->Play(0,0,0);
	}else{
		SP_Tamas3->tm3[i].fTama |=_THIT_ON;
	}
}

//////////////////////////////////////////////////////////////////////
//	���@�̃p���[�A�b�v��ʂ�����������
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::InitPWUP()
{
	//��ʂɕ\������J�[�\���̏�����
	CurCdn.x=84;
	CurCdn.y=100;
	
	//PWU�Q�[�W�̕����̏�����
	GAGE_PW=SpJIKI[23];
	GAGE_P =SpJIKI[24];
	GAGE_P2=SpJIKI[26];

	//OPT�g�p�̏�����
	OPTWAKU=SpJIKI[27];
	
	//�������x���̌��݂̒l���v�Z�p�Ɏ擾
	Wpd.LSR_temp=Wpd.LSR;
	Wpd.MSR_temp=Wpd.MSR;
	Wpd.HMM_temp=Wpd.HMM;
	Wpd.HNB_temp=Wpd.HNB;
	Wpd.OPT_temp=Wpd.OPT;
}

//////////////////////////////////////////////////////////////////////
//	���@�p���[�A�b�v��ʂ�\�����p���[�A�b�v�ɔ����F�X�ȏ�������
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::RendPWUP()
{
	//�p���[�A�b�v�ɔ����G�L�������Ńt���O�̗����Ă���ꍇ�̓t���O���N���A���ă��^�[��
	if(fJIKI&_PWUPD_ON){
		fJIKI &=_PWUPD_OFF;
		return;
	}
	
	//�W���C�X�e�B�b�Nor�L�[�|�[�h����̓��͂��擾
	JoyOld=JoyNew;
	JoyNew=Joy_Get();

	short i=0;

	//////////////////////////////////////////////////////////////////////
	//---------------------- �オ�����ꂽ�u�� ----------------------------	
	if((JoyNew^JoyOld)&(JoyNew&JOY_UP)){
		switch(CurCdn.x){
			case 84:
				if(CurCdn.y==100)		CurCdn.y=450;	//MAIN(�J�[�\���̈ʒu)
				else if(CurCdn.y==188)	CurCdn.y=100;	//SUB
				else if(CurCdn.y==276)	CurCdn.y=188;	//OPT
				else if(CurCdn.y==450)	CurCdn.y=276;	//EXIT
				break;
			case 184:
				if(CurCdn.y==100)		CurCdn.y=124;	//LSR
				else if(CurCdn.y==124)	CurCdn.y=100;	//MSR
				else if(CurCdn.y==188)	CurCdn.y=212;	//HMM
				else if(CurCdn.y==212)	CurCdn.y=188;	//HNB
				else if(CurCdn.y==308)	CurCdn.y=380;	//OPT1
				else if(CurCdn.y==332)	CurCdn.y=308;	//OPT2
				else if(CurCdn.y==356)	CurCdn.y=332;	//OPT3
				else if(CurCdn.y==380)	CurCdn.y=356;	//OPT4
				break;
			default:
				break;
		}
	//////////////////////////////////////////////////////////////////////	
	//--------------------- ���������ꂽ�u�� -----------------------------
	}else if((JoyNew^JoyOld)&(JoyNew&JOY_DOWN)){
		switch(CurCdn.x){
			case 84:
				if(CurCdn.y==100)		CurCdn.y=188;	//MAIN
				else if(CurCdn.y==188)	CurCdn.y=276;	//SUB
				else if(CurCdn.y==276)	CurCdn.y=450;	//OPT
				else if(CurCdn.y==450)	CurCdn.y=100;	//EXIT
				break;
			case 184:
				if(CurCdn.y==100)		CurCdn.y=124;	//LSR	
				else if(CurCdn.y==124)	CurCdn.y=100;	//MSR
				else if(CurCdn.y==188)	CurCdn.y=212;	//HMM
				else if(CurCdn.y==212)	CurCdn.y=188;	//HNB
				else if(CurCdn.y==308)	CurCdn.y=332;	//OPT1
				else if(CurCdn.y==332)	CurCdn.y=356;	//OPT2
				else if(CurCdn.y==356)	CurCdn.y=380;	//OPT3
				else if(CurCdn.y==380)	CurCdn.y=308;	//OPT4
				break;
			default:
				break;
		}					
	}
	//////////////////////////////////////////////////////////////////////	
	//--------------------- �E�������ꂽ�u�� -----------------------------
	if((JoyNew^JoyOld)&(JoyNew&JOY_RIGHT)){	
		if(CurCdn.x==184 && CurCdn.y==308)			i=0;	//OPT1�̈ʒu
		else if(CurCdn.x==184 && CurCdn.y==332)		i=1;	//OPT2�̈ʒu	
		else if(CurCdn.x==184 && CurCdn.y==356)		i=2;	//OPT3�̈ʒu		
		else if(CurCdn.x==184 && CurCdn.y==380)		i=3;	//OPT4�̈ʒu	
		else i=4;

		if(i>=0 && i<=3){
			switch(OPT.Jewel[i].fOPT&_fOPT_CHK){	//�Œ�A��]�A�ǔ��̂ǂ̃t���N�������Ă��邩�`�F�b�N
				case _OPTKO_ON:
					OPT.Jewel[i].fOPT &=_OPTKO_OFF;
					OPT.Jewel[i].fOPT |=_OPTKA_ON;
					break;
				case _OPTKA_ON:
					OPT.Jewel[i].fOPT &=_OPTKA_OFF;
					OPT.Jewel[i].fOPT |=_OPTTU_ON;
					break;
				case _OPTTU_ON:
					OPT.Jewel[i].fOPT &=_OPTTU_OFF;
					OPT.Jewel[i].fOPT |=_OPTKO_ON;
					break;
				default:
					break;
			}
		}
	//////////////////////////////////////////////////////////////////////	
	//---------------------- ���������ꂽ�u�� ----------------------------
	}else if((JoyNew^JoyOld)&(JoyNew&JOY_LEFT)){	
		if(CurCdn.x==184 && CurCdn.y==308)			i=0;	//OPT1�̈ʒu
		else if(CurCdn.x==184 && CurCdn.y==332)		i=1;	//OPT2�̈ʒu	
		else if(CurCdn.x==184 && CurCdn.y==356)		i=2;	//OPT3�̈ʒu		
		else if(CurCdn.x==184 && CurCdn.y==380)		i=3;	//OPT4�̈ʒu
		else i=4;
		
		if(i>=0 && i<=3){
			switch(OPT.Jewel[i].fOPT&_fOPT_CHK){//�Œ�A��]�A�ǔ��̂ǂ̃t���N�������Ă��邩�`�F�b�N
				case _OPTKO_ON:
					OPT.Jewel[i].fOPT &=_OPTKO_OFF;
					OPT.Jewel[i].fOPT |=_OPTTU_ON;
					break;
				case _OPTKA_ON:
					OPT.Jewel[i].fOPT &=_OPTKA_OFF;
					OPT.Jewel[i].fOPT |=_OPTKO_ON;
					break;
				case _OPTTU_ON:
					OPT.Jewel[i].fOPT &=_OPTTU_OFF;
					OPT.Jewel[i].fOPT |=_OPTKA_ON;
					break;
				default:
					break;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////
	//---------------------- �E��������Ă��� ----------------------------
	if(JoyNew&JOY_RIGHT){	
		if(EXP){
			if(CurCdn.x==184){
				switch(CurCdn.y){
					case 100:	//LSR�Q�[�W�A�b�v
						if(Wpd.LSR_temp<320){
							EXP--;
							Wpd.LSR_temp++;
							if(Wpd.LSR_temp>=64)	Wpd.flag |=_LSR1_ON;//���[�U�[1���g�p�\�ɂȂ�
						}
						break;
					case 124:	//MSR�Q�[�W�A�b�v
						if(Wpd.MSR_temp<320){
							EXP--;
							Wpd.MSR_temp++;
							if(Wpd.MSR_temp>=64)	Wpd.flag |=_MSR1_ON;//���[�U�[1���g�p�\�ɂȂ�
						}
						break;
					case 188:	//HMM�Q�[�W�A�b�v
						if(Wpd.HMM_temp<320){
							EXP--;
							Wpd.HMM_temp++;
							if(Wpd.HMM_temp>=64)	Wpd.flag |=_HMM1_ON;//���[�U�[1���g�p�\�ɂȂ�
						}
						break;
					case 212:	//HNB�Q�[�W�A�b�v
						if(Wpd.HNB_temp<320){
							EXP--;
							Wpd.HNB_temp++;
							if(Wpd.HNB_temp>=64)	Wpd.flag |=_HNB1_ON;//���[�U�[1���g�p�\�ɂȂ�
						}
						break;
					default:
						break;
				}
			}else if(CurCdn.x==84 && CurCdn.y==276){	//OPT�Q�[�W�A�b�v
				if(Wpd.OPT_temp<256){
					EXP--;
					Wpd.OPT_temp++;
				}
			}
		}
	//////////////////////////////////////////////////////////////////////	
	//------------------------ ����������Ă���	--------------------------
	}else if(JoyNew&JOY_LEFT){		
		if(CurCdn.x==184){
			switch(CurCdn.y){
				case 100:	//LSR�Q�[�W�_�E��
					if(Wpd.LSR_temp>Wpd.LSR){
						EXP++;
						Wpd.LSR_temp--;
						if(Wpd.LSR_temp<64)	Wpd.flag &=_LSR1_OFF;//���[�U�[1���g�p�s�\�ɂȂ�
					}
					break;
				case 124:	//MSR�Q�[�W�_�E��
					if(Wpd.MSR_temp>Wpd.MSR){
						EXP++;
						Wpd.MSR_temp--;
						if(Wpd.MSR_temp<64)	Wpd.flag &=_MSR1_OFF;//�~�T�C��1���g�p�s�\�ɂȂ�
					}
					break;
				case 188:	//HMM�Q�[�W�_�E��
					if(Wpd.HMM_temp>Wpd.HMM){
						EXP++;
						Wpd.HMM_temp--;
						if(Wpd.HMM_temp<64)	Wpd.flag &=_HMM1_OFF;//�z�[�~���O1���g�p�s�\�ɂȂ�
					}
					break;
				case 212:	//HNB�Q�[�W�_�E��
					if(Wpd.HNB_temp>Wpd.HNB){
						EXP++;
						Wpd.HNB_temp--;
						if(Wpd.HNB_temp<64)	Wpd.flag &=_HNB1_OFF;//�ԉ�1���g�p�s�\�ɂȂ�
					}
					break;
				default:
					break;
			}
		}else if(CurCdn.x==84 && CurCdn.y==276){	//OPT�Q�[�W�_�E��
			if(Wpd.OPT_temp>Wpd.OPT){
				EXP++;
				Wpd.OPT_temp--;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////
	//----------------------- A�������ꂽ�u�� ----------------------------
	if((JoyNew^JoyOld)&(JoyNew&JOY_A)){
		//�J�[�\����MAIN�̈ʒu�ɂ���ꍇ
		if(CurCdn.x==84 && CurCdn.y==100)	CurCdn.x=184;	//LSR�̈ʒu
		else if(CurCdn.x==184 && CurCdn.y==100){	//LSR
			Wpd.flag |=_LSR_ON;		
			Wpd.flag &=_MSR_OFF;
		}else if(CurCdn.x==184 && CurCdn.y==124){	//MSR
			Wpd.flag |=_MSR_ON;
			Wpd.flag &=_LSR_OFF;
		}
		//�J�[�\����SUB�̈ʒu�ɂ���ꍇ
		else if(CurCdn.x==84 && CurCdn.y==188)	CurCdn.x=184;	//HMM�̈ʒu
		else if(CurCdn.x==184 && CurCdn.y==188){	//HMM
			Wpd.flag |=_HMM_ON;
			Wpd.flag &=_HNB_OFF;
		}else if(CurCdn.x==184 && CurCdn.y==212){	//HNB
			Wpd.flag |=_HNB_ON;
			Wpd.flag &=_HMM_OFF;
		}
		//�J�[�\����OPT�̈ʒu�ɂ���ꍇ
		else if(CurCdn.x==84 && CurCdn.y==276){	
			switch(Wpd.OPT_temp/64){	//�I�v�V�����̃��x��
				case 0:
					break;
				case 1:	//���x���P
					fJIKI |=_OPT_ON;		//���@�t���O�ɃI�v�V���������t���O�𗧂Ă�
					Wpd.flag |=_OPT1_ON;	//OPT���x���P�t���O�I��

					//�t���O�������Ă��Ȃ��ꍇ�̓I�v�V�����͌Œ�ɐݒ�
					if(!(OPT.Jewel[0].fOPT&_fOPT_CHK))	OPT.Jewel[0].fOPT |=_OPTKO_ON;
					
					Wpd.OPT=Wpd.OPT_temp;	//A�������Ɗ���U����EXP�͕ύX�ł��Ȃ�
					CurCdn.x=184;	
					CurCdn.y=308;
					break;
				case 2:	//���x���Q
					fJIKI |=_OPT_ON;
					Wpd.flag &=_OPT1_OFF;
					Wpd.flag |=_OPT2_ON;

					for(i=0; i<2;i++){
						if(!(OPT.Jewel[i].fOPT&_fOPT_CHK))	OPT.Jewel[i].fOPT |=_OPTKO_ON;
					}

					Wpd.OPT=Wpd.OPT_temp;
					CurCdn.x=184;	
					CurCdn.y=308;
					break;
				case 3:	//���x���R
					fJIKI |=_OPT_ON;
					Wpd.flag &=_OPT1_OFF;
					Wpd.flag &=_OPT2_OFF;
					Wpd.flag |=_OPT3_ON;

					for(i=0; i<3;i++){
						if(!(OPT.Jewel[i].fOPT&_fOPT_CHK))	OPT.Jewel[i].fOPT |=_OPTKO_ON;
					}
					
					Wpd.OPT=Wpd.OPT_temp;
					CurCdn.x=184;	
					CurCdn.y=308;
					break;
				case 4:	//���x���S
					fJIKI |=_OPT_ON;
					Wpd.flag &=_OPT1_OFF;
					Wpd.flag &=_OPT2_OFF;
					Wpd.flag &=_OPT3_OFF;
					Wpd.flag |=_OPT4_ON;
					
					for(i=0; i<4;i++){
						if(!(OPT.Jewel[i].fOPT&_fOPT_CHK))	OPT.Jewel[i].fOPT |=_OPTKO_ON;
					}
					
					Wpd.OPT=Wpd.OPT_temp;
					CurCdn.x=184;	
					CurCdn.y=308;
					break;
				default:
					break;
			}
		}
		//EXIT
		else if(CurCdn.x==84 && CurCdn.y==450){
			InitOptCdn();	//�I�v�V�����̕\�����W�̏�����
		
			Wpd.LSR=Wpd.LSR_temp;
			Wpd.MSR=Wpd.MSR_temp;
			Wpd.HMM=Wpd.HMM_temp;
			Wpd.HNB=Wpd.HNB_temp;
			Wpd.OPT=Wpd.OPT_temp;
			fJIKI &=_FUEL_OFF;		//�e�z�����t���O�I�t
			fJIKI &=_PWUP_OFF;		//�p���[�A�b�v��ʒ��t���O�I�t
			fJIKI &=_GAME_STOP_OFF;	//�Q�[���ꎞ��~�t���O�I�t
			fJIKI |=_PWUPD_ON;		//�p���[�A�b�v�ɔ����G�L�������Ńt���O�I��
			EXP_R=0;				//EXP_R�l��0�ɖ߂�
			EXP=(BYTE)(EXP_R>>16);
		}
	}
	//////////////////////////////////////////////////////////////////////	
	//----------------------- B�������ꂽ�u�� ----------------------------
	if((JoyNew^JoyOld)&(JoyNew&JOY_B)){
		if(CurCdn.x==184){	//LSR,MSR,HMM,HNB,OPT�̈ʒu
			CurCdn.x=84;							//LSR,HMM
			if(CurCdn.y<=124)		CurCdn.y=100;	//MSR
			else if(CurCdn.y<=212)	CurCdn.y=188;	//HNB
			else if(CurCdn.y<=380)	CurCdn.y=276;	//OPT	
		}
		else if(CurCdn.x>=280)	CurCdn.x=184;		//LSR,MSR,HMM,HNB�Q�[�W�̈ʒu
	}
	
	//////////////////////////////////////////////////////////////////////	
	//-------------------------- �����̕\�� ------------------------------
	//EXP
	lpDDSBack->BltFast(100,50,lpDDS_JIKI,&Mj[14],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(116,50,lpDDS_JIKI,&Mj[33],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(132,50,lpDDS_JIKI,&Mj[25],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//MAIN
	lpDDSBack->BltFast(100,100,lpDDS_JIKI,&Mj[22],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(116,100,lpDDS_JIKI,&Mj[10],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(132,100,lpDDS_JIKI,&Mj[18],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(148,100,lpDDS_JIKI,&Mj[23],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//LSR
	lpDDSBack->BltFast(200,100,lpDDS_JIKI,&Mj[21],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(216,100,lpDDS_JIKI,&Mj[28],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(232,100,lpDDS_JIKI,&Mj[27],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//MSR
	lpDDSBack->BltFast(200,124,lpDDS_JIKI,&Mj[22],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(216,124,lpDDS_JIKI,&Mj[28],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(232,124,lpDDS_JIKI,&Mj[27],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//SUB
	lpDDSBack->BltFast(100,188,lpDDS_JIKI,&Mj[28],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(116,188,lpDDS_JIKI,&Mj[30],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(132,188,lpDDS_JIKI,&Mj[11],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//HMM
	lpDDSBack->BltFast(200,188,lpDDS_JIKI,&Mj[17],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(216,188,lpDDS_JIKI,&Mj[22],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(232,188,lpDDS_JIKI,&Mj[22],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//HNB
	lpDDSBack->BltFast(200,212,lpDDS_JIKI,&Mj[17],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(216,212,lpDDS_JIKI,&Mj[23],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(232,212,lpDDS_JIKI,&Mj[11],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//OPT
	lpDDSBack->BltFast(100,276,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(116,276,lpDDS_JIKI,&Mj[25],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(132,276,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//OPT1
	lpDDSBack->BltFast(200,308,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(216,308,lpDDS_JIKI,&Mj[25],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(232,308,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(248,308,lpDDS_JIKI,&Mj[1],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//KO KA TU 1
	lpDDSBack->BltFast(296,308,lpDDS_JIKI,&Mj[20],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(312,308,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(360,308,lpDDS_JIKI,&Mj[20],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(376,308,lpDDS_JIKI,&Mj[10],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(422,308,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(438,308,lpDDS_JIKI,&Mj[30],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//OPT2
	lpDDSBack->BltFast(200,332,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(216,332,lpDDS_JIKI,&Mj[25],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(232,332,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(248,332,lpDDS_JIKI,&Mj[2],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//KO KA TU 2
	lpDDSBack->BltFast(296,332,lpDDS_JIKI,&Mj[20],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(312,332,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(360,332,lpDDS_JIKI,&Mj[20],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(376,332,lpDDS_JIKI,&Mj[10],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(422,332,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(438,332,lpDDS_JIKI,&Mj[30],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//OPT3
	lpDDSBack->BltFast(200,356,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(216,356,lpDDS_JIKI,&Mj[25],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(232,356,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(248,356,lpDDS_JIKI,&Mj[3],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//KO KA TU 3
	lpDDSBack->BltFast(296,356,lpDDS_JIKI,&Mj[20],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(312,356,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(360,356,lpDDS_JIKI,&Mj[20],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(376,356,lpDDS_JIKI,&Mj[10],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(422,356,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(438,356,lpDDS_JIKI,&Mj[30],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//OPT4
	lpDDSBack->BltFast(200,380,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(216,380,lpDDS_JIKI,&Mj[25],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(232,380,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(248,380,lpDDS_JIKI,&Mj[4],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//KO KA TU 4
	lpDDSBack->BltFast(296,380,lpDDS_JIKI,&Mj[20],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(312,380,lpDDS_JIKI,&Mj[24],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(360,380,lpDDS_JIKI,&Mj[20],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(376,380,lpDDS_JIKI,&Mj[10],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(422,380,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(438,380,lpDDS_JIKI,&Mj[30],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//EXIT
	lpDDSBack->BltFast(100,450,lpDDS_JIKI,&Mj[14],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(116,450,lpDDS_JIKI,&Mj[33],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(132,450,lpDDS_JIKI,&Mj[18],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	lpDDSBack->BltFast(148,450,lpDDS_JIKI,&Mj[29],DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	
	//////////////////////////////////////////////////////////////////////		
	//------------------------- PWU�Q�[�W�̕\�� --------------------------
	//EXP
	GAGE_P.right=(SpJIKI[24].right-192)-(128-EXP);
	lpDDSBack->BltFast(248,50,lpDDS_JIKI,&GAGE_P,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_PW=SpJIKI[23];
	GAGE_PW.right -=192;
	lpDDSBack->BltFast(248,50,lpDDS_JIKI,&GAGE_PW,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//LSR
	GAGE_P.right=SpJIKI[24].right-(320-Wpd.LSR_temp);
	lpDDSBack->BltFast(248,100,lpDDS_JIKI,&GAGE_P,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_P2.right=SpJIKI[24].right-(320-(64*(Wpd.LSR_temp/64)));
	lpDDSBack->BltFast(248,100,lpDDS_JIKI,&GAGE_P2,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_PW=(Wpd.flag&_LSR_ON) ?  SpJIKI[25]:SpJIKI[23];
	lpDDSBack->BltFast(248,100,lpDDS_JIKI,&GAGE_PW,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//MSR
	GAGE_P.right=SpJIKI[24].right-(320-Wpd.MSR_temp);
	lpDDSBack->BltFast(248,124,lpDDS_JIKI,&GAGE_P,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_P2.right=SpJIKI[24].right-(320-(64*(Wpd.MSR_temp/64)));
	lpDDSBack->BltFast(248,124,lpDDS_JIKI,&GAGE_P2,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_PW=(Wpd.flag&_MSR_ON) ?  SpJIKI[25]:SpJIKI[23];
	lpDDSBack->BltFast(248,124,lpDDS_JIKI,&GAGE_PW,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//HMM
	GAGE_P.right=SpJIKI[24].right-(320-Wpd.HMM_temp);
	lpDDSBack->BltFast(248,188,lpDDS_JIKI,&GAGE_P,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_P2.right=SpJIKI[24].right-(320-(64*(Wpd.HMM_temp/64)));
	lpDDSBack->BltFast(248,188,lpDDS_JIKI,&GAGE_P2,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_PW=(Wpd.flag&_HMM_ON) ?  SpJIKI[25]:SpJIKI[23];
	lpDDSBack->BltFast(248,188,lpDDS_JIKI,&GAGE_PW,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//HNB
	GAGE_P.right=SpJIKI[24].right-(320-Wpd.HNB_temp);
	lpDDSBack->BltFast(248,212,lpDDS_JIKI,&GAGE_P,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_P2.right=SpJIKI[24].right-(320-(64*(Wpd.HNB_temp/64)));
	lpDDSBack->BltFast(248,212,lpDDS_JIKI,&GAGE_P2,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_PW=(Wpd.flag&_HNB_ON) ?  SpJIKI[25]:SpJIKI[23];
	lpDDSBack->BltFast(248,212,lpDDS_JIKI,&GAGE_PW,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//OPT
	GAGE_P.right=SpJIKI[24].right-(320-Wpd.OPT_temp);
	lpDDSBack->BltFast(248,276,lpDDS_JIKI,&GAGE_P,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_P2.right=SpJIKI[24].right-(320-(64*(Wpd.OPT_temp/64)));
	lpDDSBack->BltFast(248,276,lpDDS_JIKI,&GAGE_P2,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	GAGE_PW=SpJIKI[23];
	GAGE_PW.right -=64;	//���x����4�܂ł��������׋�`�͈͂�␳
	lpDDSBack->BltFast(248,276,lpDDS_JIKI,&GAGE_PW,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//OPT�g
	short	x=0,y=0;
	switch(Wpd.flag&0x00F00000){
		case _OPT4_ON:
			if(OPT.Jewel[3].fOPT&_OPTKO_ON)			{x=296;	y=380;}
			else if(OPT.Jewel[3].fOPT&_OPTKA_ON)	{x=360; y=380;}
			else if(OPT.Jewel[3].fOPT&_OPTTU_ON)	{x=422; y=380;}
			lpDDSBack->BltFast(x,y,lpDDS_JIKI,&OPTWAKU,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		case _OPT3_ON:
			if(OPT.Jewel[2].fOPT&_OPTKO_ON)			{x=296;	y=356;}
			else if(OPT.Jewel[2].fOPT&_OPTKA_ON)	{x=360; y=356;}
			else if(OPT.Jewel[2].fOPT&_OPTTU_ON)	{x=422; y=356;}
			lpDDSBack->BltFast(x,y,lpDDS_JIKI,&OPTWAKU,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		case _OPT2_ON:
			if(OPT.Jewel[1].fOPT&_OPTKO_ON)			{x=296;	y=332;}
			else if(OPT.Jewel[1].fOPT&_OPTKA_ON)	{x=360; y=332;}
			else if(OPT.Jewel[1].fOPT&_OPTTU_ON)	{x=422; y=332;}
			lpDDSBack->BltFast(x,y,lpDDS_JIKI,&OPTWAKU,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		case _OPT1_ON:
			if(OPT.Jewel[0].fOPT&_OPTKO_ON)			{x=296;	y=308;}
			else if(OPT.Jewel[0].fOPT&_OPTKA_ON)	{x=360; y=308;}
			else if(OPT.Jewel[0].fOPT&_OPTTU_ON)	{x=422; y=308;}
			lpDDSBack->BltFast(x,y,lpDDS_JIKI,&OPTWAKU,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			break;
		default:
			break;
	}

	//////////////////////////////////////////////////////////////////////		
	//----------------------- �J�[�\���̕\�� -----------------------------
	lpDDSBack->BltFast(	CurCdn.x,CurCdn.y,
						lpDDS_JIKI,&Mj[36],
						DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
}

//////////////////////////////////////////////////////////////////////
//	�R���e�B�j���[��ʂ̏����ƕ`��
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::RendCONTINUE()
{
	//�W���C�X�e�B�b�Nor�L�[�|�[�h����̓��͂��擾
	JoyOld=JoyNew;
	JoyNew=Joy_Get();
	
	//A�������ƃQ�[���ĊJ
	if((JoyNew^JoyOld)&(JoyNew&JOY_A)){
		//�R���e�B�j���[�ɔ������@�f�[�^�̏��������s��
		InitCONTINUE();
		//�Q�[���ꎞ��~�t���O�I�t
		fJIKI &=_GAME_STOP_OFF;
		//�R���e�B�j���[���I�����ꂽ���Ƃ������t���O�I��
		fJIKI |=_CONTINUE_ON;
		//��莞�Ԗ��G�ł��邱�Ƃ������t���O�I��
		fJIKI |=_INVINCIBLE_ON;
		//���G���Ԃ��Z�b�g
		InvincCnt=150;
		ContinueCnt=0;
		return;
	//B�������ƃJ�E���^��100����
	}else if((JoyNew^JoyOld)&(JoyNew&JOY_B)){
		ContinueCnt +=120;
	}
	
	ContinueCnt++;	//�R���e�B�j���[�J�E���^��i�߂�
	
	//�^�C���I�[�o�[�̏���
	if(ContinueCnt>1199){
		ContinueCnt=0;
		fSTAGE |=_STAGE_ENDWAIT_ON;	//�X�e�[�W�I���t���O�I��
		fSTAGE |=_GAME_OVER_ON;		//�Q�[���I�[�o�[�t���O�I��
		return;
	}
	
	//����(0-9)�̕\��
	lpDDSBack->BltFast(	314,236,
						lpDDS_JIKI,&Mj[47-(ContinueCnt/120)],
						DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	
	//CONTINUE ?�̕����\��
	lpDDSBack->BltFast(	236,204,
						lpDDS_JIKI,&Mj[37],
						DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
}

//////////////////////////////////////////////////////////////////////
//	�R���e�B�j���[�ɔ������@�f�[�^�̏��������s��
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::InitCONTINUE()
{
	//�K�v�̖����t���O���I�t
	fJIKI &=_ARM_OFF;	//�A�[�����˃t���O�I�t
	fJIKI &=_FUEL_OFF;	//�e�z�����t���O�I�t
	
	//���@�̉�ʍ��W��������(���[�̐^������X�^�[�g)
	Cdn.x=(WinXMax/2)-(_JIKI_W/2);
	Cdn.y=WinYMax-32;
	
	//���@�̐�΍��W��������
	MapCdn.x=MainCdn.x+Cdn.x;
	MapCdn.y=MainCdn.y+Cdn.y;

	MainWepnReset();	//��C�̒e��S�Ĕ��ˑҋ@��Ԃɖ߂�
	HmmReset();			//�z�[�~���O�~�T�C����S�Ĕ��ˑҋ@��Ԃɖ߂�
	HnbReset();			//�ԉ΂�S�Ĕ��ˑҋ@��Ԃɖ߂�
	InitOptCdn();		//�I�v�V�����̕\�����W��������
	OptAttackReset();	//�I�v�V�����̔��˂����e��S�Ĕ��ˑҋ@��Ԗ߂�
}

//////////////////////////////////////////////////////////////////////
// ���݂̃X�R�A����ʂɕ\�����邽�߃X�v���C�g�T�[�t�F�X�̋�`�͈͂ɕϊ�
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::ScoreCalcul()
{
	DWORD	sc=SCORE;			//�X�R�A�̒l���R�s�[
	DWORD	rectnum=0;			//�����̔ԍ��ۑ��p
	
	rectnum=sc / 100000000;		//1���ł̏������߂܂�
	SCORE_R[0]=Mj[rectnum];		//9���ڂ̐����X�v���C�g
	sc	=SCORE % 100000000;		//���̌��̌v�Z�p

	rectnum=sc / 10000000;		//1000���ł̏������߂܂�
	SCORE_R[1]=Mj[rectnum];		//8���ڂ̐����X�v���C�g
	sc	=SCORE % 10000000;

	rectnum=sc / 1000000;		//100���ł̏������߂܂�
	SCORE_R[2]=Mj[rectnum];		//7���ڂ̐����X�v���C�g
	sc	=SCORE % 1000000;

	rectnum=sc / 100000;		//10���ł̏������߂܂�
	SCORE_R[3]=Mj[rectnum];		//6���ڂ̐����X�v���C�g
	sc	=SCORE % 100000;

	rectnum=sc / 10000;			//1���ł̏������߂܂�
	SCORE_R[4]=Mj[rectnum];		//5���ڂ̐����X�v���C�g
	sc	=SCORE % 10000;

	rectnum=sc / 1000;			//1000�ł̏������߂܂�
	SCORE_R[5]=Mj[rectnum];		//4���ڂ̐����X�v���C�g
	sc	=SCORE % 1000;

	rectnum=sc / 100;			//100�ł̏������߂܂�
	SCORE_R[6]=Mj[rectnum];		//3���ڂ̐����X�v���C�g
	sc	=SCORE % 100;

	rectnum=sc / 10;			//10�ł̏������߂܂�
	SCORE_R[7]=Mj[rectnum];		//2���ڂ̐����X�v���C�g

	SCORE_R[8]=Mj[0];			//1���ڂ͏��0
}