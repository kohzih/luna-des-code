/*====================================================================
	�{�X�L����01�̏����Ɋւ���\�[�X�t�@�C��
=====================================================================*/
#include	<windows.h>
#include	<ddraw.h>
#include	<dsound.h>
#include	<time.h>

#include	"Macros.h"
#include	"Draw.h"	
#include	"Struct.h"
#include	"BOSS01.h"
#include	"DrawSPDataEXT.h"
#include	"DrawMacros.h"

//�_�C���N�g�T�E���h�o�b�t�@�Đ��p�}�N����`
#define	_JIKIBAKU	0
#define	_TEKIBAKU	1
#define	_FUELARM	2
#define	_FIRE		3
#define	_TAMAKYU	4
#define	_BOSSBAKU	5

extern	LPDIRECTSOUNDBUFFER		lpDSBuf[6]; //�_�C���N�g�T�E���h�o�b�t�@

extern	LPDIRECTDRAWSURFACE7	lpDDSBack;
void	DrawText(char*,long,int,int,COLORREF);	//�e�L�X�g+�ϐ��\��
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

BOSS01::BOSS01()
{
	B_Msr.tm2			=NULL;
	B_Block.tm3			=NULL;
}

BOSS01::~BOSS01()
{	
	RELEASE_dels(B_Msr.tm2);
	RELEASE_dels(B_Block.tm3);
}

//////////////////////////////////////////////////////////////////////
//	�{�X�L����01�̊e�f�[�^�̏�����
//	����:�{�X�L����01�̔z�u����Ă����΍��W
//	�߂�l:TRUE-����������		FALSE-���s
//////////////////////////////////////////////////////////////////////
BOOL	BOSS01::InitBoss01(POINT Tcdn)
{
	//�ď������ɕK�v�ȏ���
	RELEASE_dels(B_Msr.tm2);
	RELEASE_dels(B_Block.tm3);

	//�����̏�����
	srand( (unsigned)time( NULL ) );
	
	//��Ԃ�\���t���O��������
	fSPChr=0;	

	Width=576;			//�{�X�L����01�̉���
	Height=320;			//�{�X�L����02�̏c��
	Shld=9000;			//�{�X�L����01�j��܂ł̑ϋv�͂�������
	ShldCnt=Shld;		//�{�X�L����01�c��ϋv�͂̃J�E���^��������
	ATmt[0]=30;			//�U���Ԑ��ɓ���܂ł̃E�F�C�g���z�񏉊���
	ATmt[1]=50;
	ATmtCnt[0]=ATmt[0];	//�U���Ԑ��ɓ���܂ł̃E�F�C�g���̃J�E���^�z�񏉊���
	ATmtCnt[1]=ATmt[1];
	ATt[0]=100;			//�U���𑱂��鎞�Ԕz�񏉊���
	ATt[1]=20;			
	ATtCnt[0]=ATt[0];	//�U���𑱂��鎞�Ԃ̃J�E���^�z�񏉊���
	ATtCnt[1]=ATt[1];

	ONTime=0;	//��ʂɌ���Ă���̎��Ԃ�������

	MovKakudo=0;	//�{�X�L�����̉�]�p�x
	Mov.x=5;		//�{�X�L����01�̈ړ����x�̓������\���̂�������
	Mov.y=5;
			
	MapCdn=Tcdn;				//�{�X�L����01�̔z�u����Ă����΍��W��������		
	Cdn.x=MapCdn.x-MainCdn.x;	//�{�X�L����01�̔z�u����Ă����ʍ��W��������
	Cdn.y=MapCdn.y-MainCdn.y;
	dCdn.x=MapCdn.x;			//���W�v�Z�p�̍\���̂�������
	dCdn.y=MapCdn.y;

	//�{�X�L����01�����ݕ\�����Ă����ʏ�ɑ��݂��Ă���̂��ǂ����`�F�b�N
	if(MainCdn.y>MapCdn.y+Height)			{fSPChr &=_SPOFF;}
	else if(MainCdn.y+WinYMax<MapCdn.y)		{fSPChr &=_SPOFF;}
	else if(MainCdn.x>MapCdn.x+Width)		{fSPChr &=_SPOFF;}
	else if(MainCdn.x+WinXMax<MapCdn.x)		{fSPChr &=_SPOFF;}
	else	fSPChr |=_SPON;		//�\���t���O���I��

	//-------------------------�e�I�u�W�F�N�g��������-------------------------------//
	
	//--------------------------�U���p�^�[��1�p------------------------------------
	B_Msr.TTama=21;				//��ʂɑ��݂ł���e�̐�
	B_Msr.WTama=12;				//�e�𔭎˂���Ԋu
	B_Msr.WTamaCnt=B_Msr.WTama;	//�ŏ��̒e���˂܂ł̃J�E���^
	
	//���ˑҋ@��Ԃɂ���e�ԍ��ۑ��p�ϐ���������(�U���p�^�[��1)
	for(short i=0;i<7;i++){
		AttackTama1[i]=B_Msr.TTama;
	}

	//TAMA2�\���̂ɑO�����Ēl���Z�b�g���Ēe�f�[�^������������
	TAMA2	tama2;
	char	*pTama;
	
	tama2.fTama=_TOFF;		//�e�̏�����Ԃ�ݒ�
	tama2.Width=8;			//�e�̉���
	tama2.Height=8;			//�e�̏c��
	tama2.Kakudo=0;			//�e�̔��ˊp�x
	tama2.Time=0;			//�e�����˂���Ă���̌o�ߎ���		
	tama2.Mov.x=0.0;		//�e�̈ړ����x��0�ŏ�����
	tama2.Mov.y=0.0;
	tama2.dCdn.x=0.0;		//�e�̍��W�v�Z�p�̍��W��0�ŏ�����
	tama2.dCdn.y=0.0;
	tama2.Cdn.x=0;			//�e�̌��݂̉�ʍ��W��0�ŏ�����
	tama2.Cdn.y=0;
	tama2.MapCdn.x=0;		//�e�̌��݂̐�΍��W��0�ŏ�����
	tama2.MapCdn.y=0;
	tama2.Anime=TamAme[0];	//�e�A�j���[�V�����p�^�[�����̏�����	
	memcpy(&tama2.AnimePat[0],&TamPat[0][0],sizeof(BYTE)*10);//�e�A�j���[�V�����p�^�[���z��̏�����
	memcpy(&tama2.AnimeFrame[0],&TamFrame[0][0],sizeof(BYTE)*10);//�e�A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	tama2.AnimeCnt=0;					//�ŏ��ɕ\������A�j���[�V�����p�^�[����������
	tama2.FrameCnt=tama2.AnimeFrame[0];	//�ŏ��̃A�j���[�V�����t���[����������
	tama2.SrcR.left=0;					//�]������`��0�ŏ�����
	tama2.SrcR.top=0;
	tama2.SrcR.right=0;
	tama2.SrcR.bottom=0;
	tama2.DesR=tama2.SrcR;			//�]�����`��0�ŏ�����
	
	//���˂ł���e����TAMA2�\���̂��m��
	B_Msr.tm2=new TAMA2[B_Msr.TTama];
	if(!B_Msr.tm2)		return	FALSE;
	//�m�ۂ����̈�̐擪�A�h���X���R�s�[
	pTama=(char*)B_Msr.tm2;		
	//�e�I�u�W�F�N�g���m�ۂ������A������ 
	for(i=0;(DWORD)i<B_Msr.TTama;i++){
		memcpy(pTama,&tama2,sizeof(TAMA2));	//�m�ۂ����̈�̐擪����R�s�[
		pTama +=sizeof(TAMA2);				//���ɃR�s�[����Ƃ���܂ŃA�h���X��i�߂�
	}
	
	//-----------------------�U���p�^�[��2�p------------------------------
	T_Ichi=512;						//B_Block�𔭎˂���ʒu
	
	B_Block.TTama=100;				//��ʂɑ��݂ł���e�̐�
	B_Block.WTama=1;				//�e�𔭎˂���Ԋu
	B_Block.WTamaCnt=B_Block.WTama;	//�ŏ��̒e���˂܂ł̃J�E���^
	
	AttackTama2=B_Block.TTama;		//���ˑҋ@��Ԃɂ���e�ԍ��ۑ��p�ϐ���������(�U���p�^�[��1)

	//TAMA3�\���̂ɑO�����Ēl���Z�b�g���Ēe�f�[�^������������
	TAMA3	tama3;
	
	tama3.fTama=_TOFF;		//�e�̏�����Ԃ�ݒ�
	tama3.Width=64;			//�e�̉���
	tama3.Height=32;		//�e�̏c��
	tama3.Kakudo=0;			//�e�̔��ˊp�x(����͖��g�p)
	tama3.Time=0;			//�e�����˂���Ă���̌o�ߎ���		
	tama3.Shld=8;			//�e�j��܂ł̑ϋv��
	tama3.ShldCnt=tama3.Shld;		//�e�c��ϋv�͂̃J�E���^
	tama3.Mov.x=0.0;		//�e�̈ړ����x��0�ŏ�����
	tama3.Mov.y=0.0;
	tama3.dCdn.x=0.0;		//�e�̍��W�v�Z�p�̍��W��0�ŏ�����
	tama3.dCdn.y=0.0;
	tama3.Cdn.x=0;			//�e�̌��݂̉�ʍ��W��0�ŏ�����
	tama3.Cdn.y=0;
	tama3.MapCdn.x=0;		//�e�̌��݂̐�΍��W��0�ŏ�����
	tama3.MapCdn.y=0;
	tama3.Anime=TamAme[2];	//�e�A�j���[�V�����p�^�[�����̏�����	
	memcpy(&tama3.AnimePat[0],&TamPat[2][0],sizeof(BYTE)*10);//�e�A�j���[�V�����p�^�[���z��̏�����
	memcpy(&tama3.AnimeFrame[0],&TamFrame[2][0],sizeof(BYTE)*10);//�e�A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	tama3.AnimeCnt=0;					//�ŏ��ɕ\������A�j���[�V�����p�^�[����������
	tama3.FrameCnt=tama3.AnimeFrame[0];	//�ŏ��̃A�j���[�V�����t���[����������
	tama3.BakuWidth=32;			//�e�̔����V�[��������������
	tama3.BakuHeight=32;		//�e�̔����V�[���c����������
	tama3.BakuAnime=BakuAme[1];	//�����A�j���[�V�����p�^�[������������
	tama3.BakuAnimePat[10];		//�����A�j���[�V�����p�^�[���z���������
	memcpy(&tama3.BakuAnimePat[0],&BakuPat[1][0],sizeof(BYTE)*10);//�����A�j���[�V�����p�^�[���z�񏉊���
	memcpy(&tama3.BakuAnimeFrame[0],&BakuFrame[1][0],sizeof(BYTE)*10);//�����A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	tama3.BakuAnimeCnt=0;		//���ݕ\�����锚���A�j���[�V�����p�^�[����������
	tama3.BakuFrameCnt=tama3.BakuAnimeFrame[0];	//���݂̔����A�j���[�V�����t���[����������
	tama3.SrcR.left=0;			//�]������`��0�ŏ�������������
	tama3.SrcR.top=0;
	tama3.SrcR.right=0;
	tama3.SrcR.bottom=0;
	tama3.DesR=tama3.SrcR;		//�]�����`��0�ŏ�������������
	
	//���˂ł���e����TAMA3�\���̂��m��
	B_Block.tm3=new TAMA3[B_Block.TTama];
	if(!B_Block.tm3)		return	FALSE;
	//�m�ۂ����̈�̐擪�A�h���X���R�s�[
	pTama=(char*)B_Block.tm3;		
	//�e�I�u�W�F�N�g���m�ۂ������A������ 
	for(i=0;(DWORD)i<B_Block.TTama;i++){
		memcpy(pTama,&tama3,sizeof(TAMA3));	//�m�ۂ����̈�̐擪����R�s�[
		pTama +=sizeof(TAMA3);				//���ɃR�s�[����Ƃ���܂ŃA�h���X��i�߂�
	}
	
	//----------------�{�X�L����01���\������`��p�f�[�^�̏�����--------------------//

	//�p�[�c1�̏�����
	Parts1.Local.x=Cdn.x+224;				//�`�悷���ƂȂ���W���狗��
	Parts1.Local.y=Cdn.y;			
	Parts1.Width=96;						//�p�[�c�̉���������
	Parts1.Height=64;						//�p�[�c�̏c��������
	Parts1.SrcR=SP[19];						//�]�����T�[�t�F�X�̋�`�͈͏�����
	Parts1.DesR.left=Parts1.Local.x;		//�]����T�[�t�F�X�̋�`�͈͏�����
	Parts1.DesR.top=Parts1.Local.y;			
	Parts1.DesR.right=Parts1.Local.x+Parts1.Width;
	Parts1.DesR.bottom=Parts1.Local.y+Parts1.Height;
	
	//�p�[�c2�̏�����
	Parts2.Local.x=Cdn.x+96;				//�`�悷���ƂȂ���W���狗��
	Parts2.Local.y=Cdn.y+64;			
	Parts2.Width=384;						//�p�[�c�̉���������
	Parts2.Height=32;						//�p�[�c�̏c��������
	Parts2.SrcR=SP[20];						//�]�����T�[�t�F�X�̋�`�͈͏�����
	Parts2.DesR.left=Parts2.Local.x;		//�]����T�[�t�F�X�̋�`�͈͏�����
	Parts2.DesR.top=Parts2.Local.y;			
	Parts2.DesR.right=Parts2.Local.x+Parts2.Width;
	Parts2.DesR.bottom=Parts2.Local.y+Parts2.Height;

	//�p�[�c3�̏�����
	Parts3.Local.x=Cdn.x;					//�`�悷���ƂȂ���W���狗��
	Parts3.Local.y=Cdn.y+96;				
	Parts3.Width=576;						//�p�[�c�̉���������
	Parts3.Height=64;						//�p�[�c�̏c��������
	Parts3.Anime=BossAnime[0];				//�A�j���[�V�����p�^�[�����̏�����
	memcpy(&Parts3.AnimePat[0],&BossPat[0][0],sizeof(BYTE)*10);//�A�j���[�V�����p�^�[���z��̏�����
	memcpy(&Parts3.AnimeFrame[0],&BossFrame[0][0],sizeof(BYTE)*10);//�A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	Parts3.AnimeCnt=0;						//���ݕ\������A�j���[�V�����p�^�[���평����
	Parts3.FrameCnt=Parts3.AnimeFrame[0];	//���݂̃A�j���[�V�����t���[����������
	Parts3.SrcR=SP[Parts3.AnimePat[Parts3.AnimeCnt]];	//�]�����T�[�t�F�X�̋�`�͈͏�����
	Parts3.DesR.left=Parts3.Local.x;					//�]����T�[�t�F�X�̋�`�͈͏�����
	Parts3.DesR.top=Parts3.Local.y;			
	Parts3.DesR.right=Parts3.Local.x+Parts3.Width;
	Parts3.DesR.bottom=Parts3.Local.y+Parts3.Height;

	//�p�[�c4�̏�����
	Parts4.Local.x=Cdn.x+128;				//�`�悷���ƂȂ���W���狗��
	Parts4.Local.y=Cdn.y+160;				
	Parts4.Width=288;						//�p�[�c�̉���������
	Parts4.Height=32;						//�p�[�c�̏c��������
	Parts4.Anime=BossAnime[1];				//�A�j���[�V�����p�^�[�����̏�����
	memcpy(&Parts4.AnimePat[0],&BossPat[1][0],sizeof(BYTE)*10);//�A�j���[�V�����p�^�[���z��̏�����
	memcpy(&Parts4.AnimeFrame[0],&BossFrame[1][0],sizeof(BYTE)*10);//�A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	Parts4.AnimeCnt=0;						//���ݕ\������A�j���[�V�����p�^�[���평����
	Parts4.FrameCnt=Parts4.AnimeFrame[0];	//���݂̃A�j���[�V�����t���[����������
	Parts4.SrcR=SP[Parts4.AnimePat[Parts4.AnimeCnt]];	//�]�����T�[�t�F�X�̋�`�͈͏�����
	Parts4.DesR.left=Parts4.Local.x;					//�]����T�[�t�F�X�̋�`�͈͏�����
	Parts4.DesR.top=Parts4.Local.y;			
	Parts4.DesR.right=Parts4.Local.x+Parts4.Width;
	Parts4.DesR.bottom=Parts4.Local.y+Parts4.Height;

	//�p�[�c5�̏�����
	Parts5.Local.x=Cdn.x+192;				//�`�悷���ƂȂ���W���狗��
	Parts5.Local.y=Cdn.y+192;			
	Parts5.Width=192;						//�p�[�c�̉���������
	Parts5.Height=32;						//�p�[�c�̏c��������
	Parts5.SrcR=SP[23];						//�]�����T�[�t�F�X�̋�`�͈͏�����
	Parts5.DesR.left=Parts5.Local.x;		//�]����T�[�t�F�X�̋�`�͈͏�����
	Parts5.DesR.top=Parts5.Local.y;			
	Parts5.DesR.right=Parts5.Local.x+Parts5.Width;
	Parts5.DesR.bottom=Parts5.Local.y+Parts5.Height;

	//�p�[�c6�̏�����
	Parts6.Local.x=Cdn.x+224;				//�`�悷���ƂȂ���W���狗��
	Parts6.Local.y=Cdn.y+224;				
	Parts6.Width=128;						//�p�[�c�̉���������
	Parts6.Height=96;						//�p�[�c�̏c��������
	Parts6.Anime=BossAnime[2];				//�A�j���[�V�����p�^�[�����̏�����
	memcpy(&Parts6.AnimePat[0],&BossPat[2][0],sizeof(BYTE)*10);//�A�j���[�V�����p�^�[���z��̏�����
	memcpy(&Parts6.AnimeFrame[0],&BossFrame[2][0],sizeof(BYTE)*10);//�A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	Parts6.AnimeCnt=0;						//���ݕ\������A�j���[�V�����p�^�[���평����
	Parts6.FrameCnt=Parts6.AnimeFrame[0];	//���݂̃A�j���[�V�����t���[����������
	Parts6.SrcR=SP[Parts6.AnimePat[Parts6.AnimeCnt]];	//�]�����T�[�t�F�X�̋�`�͈͏�����
	Parts6.DesR.left=Parts6.Local.x;					//�]����T�[�t�F�X�̋�`�͈͏�����
	Parts6.DesR.top=Parts6.Local.y;			
	Parts6.DesR.right=Parts6.Local.x+Parts6.Width;
	Parts6.DesR.bottom=Parts6.Local.y+Parts6.Height;

	//----------------�����V�[���̃A�j���[�V�����f�[�^�̏�����----------------------//
	
	//����1
	
	BOSSANIME2	baku1;

	baku1.Local.x=0;					//�`�悷���ƂȂ���W���狗��
	baku1.Local.y=0;				
	baku1.Width=32;						//�p�[�c�̉���������
	baku1.Height=32;						//�p�[�c�̏c��������
	baku1.Anime=BakuAme[0];					//�A�j���[�V�����p�^�[�����̏�����
	memcpy(&baku1.AnimePat[0],&BakuPat[0][0],sizeof(BYTE)*10);//�A�j���[�V�����p�^�[���z��̏�����
	memcpy(&baku1.AnimeFrame[0],&BakuFrame[0][0],sizeof(BYTE)*10);//�A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	baku1.AnimeCnt=0;						//���ݕ\������A�j���[�V�����p�^�[���평����
	baku1.FrameCnt=baku1.AnimeFrame[0];		//���݂̃A�j���[�V�����t���[����������
	baku1.SrcR=SP[baku1.AnimePat[baku1.AnimeCnt]];	//�]�����T�[�t�F�X�̋�`�͈͏�����
	baku1.DesR.left=0;				//�]����T�[�t�F�X�̋�`�͈͏�����
	baku1.DesR.top=0;			
	baku1.DesR.right=0;
	baku1.DesR.bottom=0;
	
	for(i=0;i<12;i++){
		baku1.Local.x=rand()%128;	//����������W�𗐐��ŋ��߂ď�����
		baku1.Local.y=rand()%96;
		BossBaku1[i]=baku1;
	}
	
	//����2
	BOSSANIME2	baku2;

	baku2.Local.x=0;					//�`�悷���ƂȂ���W���狗��
	baku2.Local.y=0;				
	baku2.Width=64;						//�p�[�c�̉���������
	baku2.Height=64;					//�p�[�c�̏c��������
	baku2.Anime=BakuAme[1];				//�A�j���[�V�����p�^�[�����̏�����
	memcpy(&baku2.AnimePat[0],&BakuPat[1][0],sizeof(BYTE)*10);//�A�j���[�V�����p�^�[���z��̏�����
	memcpy(&baku2.AnimeFrame[0],&BakuFrame[1][0],sizeof(BYTE)*10);//�A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	baku2.AnimeCnt=0;						//���ݕ\������A�j���[�V�����p�^�[���평����
	baku2.FrameCnt=baku2.AnimeFrame[0];		//���݂̃A�j���[�V�����t���[����������
	baku2.SrcR=SP[baku2.AnimePat[baku2.AnimeCnt]];	//�]�����T�[�t�F�X�̋�`�͈͏�����
	baku2.DesR.left=0;				//�]����T�[�t�F�X�̋�`�͈͏�����
	baku2.DesR.top=0;			
	baku2.DesR.right=0;
	baku2.DesR.bottom=0;
	
	for(i=0;i<200;i++){
		baku2.Local.x=rand()%576;	//����������W�𗐐��ŋ��߂ď�����
		baku2.Local.y=rand()%320;				
		BossBaku2[i]=baku2;
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////
//	�{�X�L����01�̈ړ�����
//////////////////////////////////////////////////////////////////////
void	BOSS01::MovBoss01()
{
	//�{�X�L�������j�󂳂�Ă���Ȃ烊�^�[��
	if((fSPChr&_SPDESCHK)==(DWORD)_SPDES)	return;
	
	//�{�X�L���������݂̕\����ʔ͈͂ɓ����Ă��邩�`�F�b�N
	if(MainCdn.y>MapCdn.y+Height)			fSPChr &=_SPOFF;
	else if(MainCdn.y+WinYMax<MapCdn.y)		fSPChr &=_SPOFF;
	else if(MainCdn.x>MapCdn.x+Width)		fSPChr &=_SPOFF;
	else if(MainCdn.x+WinXMax>MapCdn.x){												
		//�\���t���O�I��
		fSPChr |=_SPON;
		/*
		//���v���ɉ�]
		if(MovKakudo<=180){
			dCdn.x +=Mov.x*Cos[MovKakudo];
			dCdn.y +=Mov.y*Sin[MovKakudo];
		}else if(MovKakudo>180){
			dCdn.x +=Mov.x*Cos[MovKakudo];
			dCdn.y +=-Mov.y*Sin[MovKakudo];
		}
		*/	
		//���݂̍��W�ɕϊ�
		MapCdn.x=(LONG)dCdn.x;
		MapCdn.y=(LONG)dCdn.y;
		Cdn.x=MapCdn.x-MainCdn.x;	//�{�X�L����01�̔z�u����Ă����ʍ��W��������
		Cdn.y=MapCdn.y-MainCdn.y;
	
		//��]�p�x��i�߂�
		MovKakudo=MovKakudo+10>359 ? 0:MovKakudo+10;

		//�e�p�[�c�̍��W���Z�b�g
		Parts1.Local.x=Cdn.x+224;
		Parts1.Local.y=Cdn.y;		
		Parts2.Local.x=Cdn.x+96;
		Parts2.Local.y=Cdn.y+64;
		Parts3.Local.x=Cdn.x;
		Parts3.Local.y=Cdn.y+96;
		Parts4.Local.x=Cdn.x+128;
		Parts4.Local.y=Cdn.y+160;	
		Parts5.Local.x=Cdn.x+192;
		Parts5.Local.y=Cdn.y+192;
		Parts6.Local.x=Cdn.x+224;
		Parts6.Local.y=Cdn.y+224;
	}
}

//////////////////////////////////////////////////////////////////////
//	�{�X�L����01�̍U������1
//////////////////////////////////////////////////////////////////////
void	BOSS01::Attack1Boss01(POINT j_cdn)
{
	bool	ret=FALSE;
	
	//----------------------------------------------------------//
	//					   �~�T�C���̈ړ�����					//
	//----------------------------------------------------------//
	
	//���ɔ��˂���Ă���e�̍��W��i�߂�
	for(short i=0;(DWORD)i<B_Msr.TTama;i++){
		//���ɔ��˂���Ă���e�̂ݏ�������
		if(B_Msr.tm2[i].fTama&_TON)
		{ 
			//��x�ł��e�ړ��������s��ꂽ�ꍇret��TRUE����
			ret=TRUE;
			
			//�e���ړ�������
			B_Msr.tm2[i].dCdn.x +=B_Msr.tm2[i].Mov.x;
			B_Msr.tm2[i].dCdn.y +=B_Msr.tm2[i].Mov.y;				
			//�e�̐�΍��W��i�߂�
			B_Msr.tm2[i].MapCdn.x=(LONG)B_Msr.tm2[i].dCdn.x;
			B_Msr.tm2[i].MapCdn.y=(LONG)B_Msr.tm2[i].dCdn.y;
			//�e�̉�ʍ��W�����߂�
			B_Msr.tm2[i].Cdn.x=B_Msr.tm2[i].MapCdn.x-MainCdn.x;
			B_Msr.tm2[i].Cdn.y=B_Msr.tm2[i].MapCdn.y-MainCdn.y;
				
			//�e����ʊO�Ɉړ����ĂȂ����`�F�b�N
			if(	B_Msr.tm2[i].MapCdn.x+B_Msr.tm2[i].Width<MainCdn.x	||
				B_Msr.tm2[i].MapCdn.x>MainCdn.x+WinXMax				||
				B_Msr.tm2[i].MapCdn.y+B_Msr.tm2[i].Height<MainCdn.y	||
				B_Msr.tm2[i].MapCdn.y>MainCdn.y+WinYMax)
			{
				B_Msr.tm2[i].fTama &=_TOFF;		//�e�t���O�I�t
				continue;
			}
		}else if(B_Msr.tm2[i].fTama==_TOFF){
			//�Ĕ��ˉ\�ȃ~�T�C���̔ԍ���ۑ�����
			for(short j=0;(DWORD)j<7;j++){
				if(AttackTama1[j]==B_Msr.TTama){
					AttackTama1[j]=i;
					B_Msr.tm2[i].fTama |=_TSET;	//�~�T�C�����Ăє��˃Z�b�g
					break;
				}
			}
			//��ʂɒe�����݂��Ă��Ȃ����̓��[�v�𔲂���
			if(!(AttackTama1[6]==B_Msr.TTama) && !(fSPChr&_TAMA2ON)){
				break;
			}
		}
	}
	
	//ret�̒l��FALSE�̂܂܂������ꍇ�A��ʂɒe�͑��݂��Ă��Ȃ�
	if(!ret)	fSPChr &=_TAMA2OFF;		//��ʒe���݃t���O���I�t
	
	//�G�L�������\������Ă��Ȃ��ꍇ���^�[��
	if(!(fSPChr&_SPON))			return;
	//�G�L�����̔j��t���O�������Ă���ꍇ���^�[��
	else if(fSPChr&_SPDES)		return;
	
	//----------------------------------------------------------//
	//					   �~�T�C���̔��ˏ���					//
	//----------------------------------------------------------//
	
	ret=TRUE;
	
	double	X,Y,x,y;
	short	Kakudo;
	
	//�~�T�C���̔��ˏ���
	if(!B_Msr.WTamaCnt--){
		
		//�~�T�C�����w�肳�ꂽ���ȏ�ɉ�ʂɑ��݂��Ȃ��悤�ɂ��鏈��
		for(short i=0;i<7;i++){	//7�������ˉ\�Ȓe���m�ۂ���Ă��邩�`�F�b�N
			if(AttackTama1[i]>=B_Msr.TTama)	ret=FALSE;
		}
		
		//���˂̏���
		if(ret){
			//��ʏ�ɒe�����˂���Ă���t���O�I��
			fSPChr |=_TAMA2ON;
					
			//���@�Ƃ̊p�x�����߂�(�e�̓p�[�c6���甭�˂����)
			X=(j_cdn.x+10)-(Parts6.Local.x+64);//���S�_�Ŕ�r�B64=(Parts6.Width/2)
			Y=(j_cdn.y+16)-(Parts6.Local.y+48);//48=(Parts6.Height/2)
			if(X==0){ 
				if(Y<0)			Kakudo=90;	
				else if(Y>0)	Kakudo=270;
			}else if(Y==0){
				if(X<0)			Kakudo=180;
				else if(X>0)	Kakudo=0;
			}else{
				//���@�Ƃ̊p�x���v�Z
				x=X<0 ? -X:X;
				y=Y<0 ? -Y:Y;
				Kakudo=TanKaku(y/x);	//�^���W�F���g�l���p�x�ɕϊ�
				//���܂����p�x�����ꂼ��̏ی��ɑΉ������p�x�ɕϊ�(��1�ی��͂��̂܂�)
				if(X<0){
					if(Y<0)			Kakudo=180-Kakudo;//��2�ی�
					else if(Y>0)	Kakudo=180+Kakudo;//��3�ی�
				}else if(X>0){
					if(Y>0)			Kakudo=360-Kakudo;//��4�ی�
				}
			}	

			Kakudo -=42;
			if(Kakudo<0)	Kakudo=360+Kakudo;
			
			//�~�T�C���̔��˂��ꂽ���W��������
			//��ʍ��W���Z�b�g
			for(i=0;i<7;i++){
				//���˒��̃t���O�𗧂Ă�
				B_Msr.tm2[AttackTama1[i]].fTama |=_TON;		
				//�e�A�j���[�V������������
				B_Msr.tm2[AttackTama1[i]].AnimeCnt=0;		
				B_Msr.tm2[AttackTama1[i]].FrameCnt=B_Msr.tm2[AttackTama1[i]].AnimeFrame[0];	
				//�������W
				B_Msr.tm2[AttackTama1[i]].Cdn.x=Parts6.Local.x+64;//64=(Parts6.Width/2)
				B_Msr.tm2[AttackTama1[i]].Cdn.y=Parts6.Local.y+48;//48=(Parts6.Height/2)
				//�e�̈ړ��v�Z�p�ɉ�ʍ��W���擾
				B_Msr.tm2[AttackTama1[i]].dCdn.x=MainCdn.x+B_Msr.tm2[AttackTama1[i]].Cdn.x;
				B_Msr.tm2[AttackTama1[i]].dCdn.y=MainCdn.y+B_Msr.tm2[AttackTama1[i]].Cdn.y;
				//��΍��W���Z�b�g
				B_Msr.tm2[AttackTama1[i]].MapCdn.x=(LONG)B_Msr.tm2[AttackTama1[i]].dCdn.x;
				B_Msr.tm2[AttackTama1[i]].MapCdn.y=(LONG)B_Msr.tm2[AttackTama1[i]].dCdn.y;
				//�e�̔��ˊp�x�ƈړ��ʂ�������
				B_Msr.tm2[AttackTama1[i]].Kakudo=Kakudo;
				B_Msr.tm2[AttackTama1[i]].Mov.x=3*Cos[Kakudo];
				B_Msr.tm2[AttackTama1[i]].Mov.y=-3*Sin[Kakudo];
				
				Kakudo +=14;
				if(Kakudo>359)	Kakudo=Kakudo-360;

				//B_Msr.TTama�ŏ�����
				//���̏�����AttackTama1���㏑������Ȃ�����V�����e�𔭎˂ł��Ȃ��悤�ɂ��Ă��܂�
				AttackTama1[i]=B_Msr.TTama;
			}
		}			
		B_Msr.WTamaCnt=B_Msr.WTama;	//���˂܂ł̃J�E���^���ăZ�b�g
	}
	
}

//////////////////////////////////////////////////////////////////////
//	�{�X�L����01�̍U������2
//////////////////////////////////////////////////////////////////////
void	BOSS01::Attack2Boss01()
{
	
	bool	ret=FALSE;
	
	//----------------------------------------------------------//
	//					   �u���b�N�̈ړ�����					//
	//----------------------------------------------------------//
	
	//���ɔ��˂���Ă���e�̍��W��i�߂�
	for(short i=0;(DWORD)i<B_Block.TTama;i++){
		//���ɔ��˂���Ă���e�̂ݏ�������
		if(B_Block.tm3[i].fTama&_TON)
		{ 
			//��x�ł��e�ړ��������s��ꂽ�ꍇret��TRUE����
			ret=TRUE;
			
			//�e���ړ�������
			B_Block.tm3[i].dCdn.x +=B_Block.tm3[i].Mov.x;
			B_Block.tm3[i].dCdn.y +=B_Block.tm3[i].Mov.y;				
			//�e�̐�΍��W��i�߂�
			B_Block.tm3[i].MapCdn.x=(LONG)B_Block.tm3[i].dCdn.x;
			B_Block.tm3[i].MapCdn.y=(LONG)B_Block.tm3[i].dCdn.y;
			//�e�̉�ʍ��W�����߂�
			B_Block.tm3[i].Cdn.x=B_Block.tm3[i].MapCdn.x-MainCdn.x;
			B_Block.tm3[i].Cdn.y=B_Block.tm3[i].MapCdn.y-MainCdn.y;
				
			//�e����ʊO�Ɉړ����ĂȂ����`�F�b�N
			if(	B_Block.tm3[i].MapCdn.x+B_Block.tm3[i].Width<MainCdn.x	||
				B_Block.tm3[i].MapCdn.x>MainCdn.x+WinXMax				||
				B_Block.tm3[i].MapCdn.y+B_Block.tm3[i].Height<MainCdn.y	||
				B_Block.tm3[i].MapCdn.y>MainCdn.y+WinYMax)
			{
				B_Block.tm3[i].fTama=_TOFF;		//�e�t���O�I�t
				continue;
			}
		}else if(B_Block.tm3[i].fTama&_TBAKU_ON){
			ret=TRUE;	//�����A�j���[�V�������I������܂Œe�͑��݂��Ă���
		}else if(B_Block.tm3[i].fTama==_TOFF){
			//�Ĕ��ˉ\�ȃu���b�N�̔ԍ���ۑ�����
			if(AttackTama2==B_Block.TTama){
				AttackTama2=i;
				B_Block.tm3[i].fTama |=_TSET;	//�u���b�N���Ăє��˃Z�b�g
			}else if(!(fSPChr&_TAMA3ON)){
				break;	//��ʒe���݃t���O�������Ă��Ȃ��ꍇ�͒e�̈ړ����������Ȃ�
			}
		}
	}
	
	//ret�̒l��FALSE�̂܂܂������ꍇ�A��ʂɒe�͑��݂��Ă��Ȃ�
	if(!ret)	fSPChr &=_TAMA3OFF;		//��ʒe���݃t���O���I�t
	
	//�G�L�������\������Ă��Ȃ��ꍇ���^�[��
	if(!(fSPChr&_SPON))			return;
	//�G�L�����̔j��t���O�������Ă���ꍇ���^�[��
	else if(fSPChr&_SPDES)		return;
	
	//----------------------------------------------------------//
	//					 �u���b�N�̔��ˏ���						//
	//----------------------------------------------------------//
	
	//�u���b�N�̔��ˏ���
	if(!B_Block.WTamaCnt--){
		//�w�肳�ꂽ���ȏ��ʂɒe�����݂��Ȃ��悤�ɂ��鏈��
		if(AttackTama2<B_Block.TTama){
			//��ʏ�ɒe�����˂���Ă���t���O�I��
			fSPChr |=_TAMA3ON;
			
			//���˒��̃t���O�𗧂Ă�
			B_Block.tm3[AttackTama2].fTama |=_TON;		
			//�e�A�j���[�V������������
			B_Block.tm3[AttackTama2].AnimeCnt=0;		
			B_Block.tm3[AttackTama2].FrameCnt=B_Block.tm3[AttackTama2].AnimeFrame[0];	
			//�������W
			B_Block.tm3[AttackTama2].Cdn.x=Parts3.Local.x+T_Ichi;
			B_Block.tm3[AttackTama2].Cdn.y=Parts3.Local.y;
			//�e�̈ړ��v�Z�p�ɉ�ʍ��W���擾
			B_Block.tm3[AttackTama2].dCdn.x=MainCdn.x+B_Block.tm3[AttackTama2].Cdn.x;
			B_Block.tm3[AttackTama2].dCdn.y=MainCdn.y+B_Block.tm3[AttackTama2].Cdn.y;
			//��΍��W���Z�b�g
			B_Block.tm3[AttackTama2].MapCdn.x=(LONG)B_Block.tm3[AttackTama2].dCdn.x;
			B_Block.tm3[AttackTama2].MapCdn.y=(LONG)B_Block.tm3[AttackTama2].dCdn.y;
			//�ړ��ʂ̏�����
			B_Block.tm3[AttackTama2].Mov.x=0;
			B_Block.tm3[AttackTama2].Mov.y=5;
			//�ϋv�͂̃Z�b�g
			B_Block.tm3[AttackTama2].ShldCnt=B_Block.tm3[AttackTama2].Shld;
			//�j��V�[���̏�����
			B_Block.tm3[AttackTama2].BakuAnimeCnt=0;		
			B_Block.tm3[AttackTama2].BakuFrameCnt=B_Block.tm3[AttackTama2].BakuAnimeFrame[0];
			
			//���ˈʒu��i�߂�
			T_Ichi -=69;	
			if(T_Ichi<374 && T_Ichi>202)	T_Ichi=138;
			else if(T_Ichi<0)				T_Ichi=512;
		
			//B_Block.TTama�ŏ�����
			//���̏�����AttackTama2���㏑������Ȃ�����V�����e�𔭎˂ł��Ȃ��悤�ɂ��Ă��܂�
			AttackTama2=B_Block.TTama;
		}
		B_Block.WTamaCnt=B_Block.WTama;	//���˂܂ł̃J�E���^���ăZ�b�g
	}
}

//////////////////////////////////////////////////////////////////////
//	�{�X�L����01�̕`�揀�����s�Ȃ�����
//	�����͏�Ԃ�\�킷�t���O
//////////////////////////////////////////////////////////////////////
void	BOSS01::FrameBoss01(DWORD Tflag)
{
	fSPChr=Tflag;
	short i;
	
	//----------------------------------------------------------//
	//					�e�̕`�揀��							//
	//----------------------------------------------------------//

	//��ʒe���݃t���O�������Ă���ꍇ�̂ݒe��`��
	if(fSPChr&_TAMA2ON){
		//�~�T�C��
		for(i=0;(DWORD)i<B_Msr.TTama;i++){
			//���˂���Ă��Ȃ��e�̂ݕ`�悷��
			if(B_Msr.tm2[i].fTama&_TON){ 
				//SP�T�[�t�F�X����̓]����`�͈͂��Z�b�g
				B_Msr.tm2[i].SrcR=TamSP[B_Msr.tm2[i].AnimePat[B_Msr.tm2[i].AnimeCnt]];
					
				B_Msr.tm2[i].DesR.left =B_Msr.tm2[i].Cdn.x;
				B_Msr.tm2[i].DesR.top  =B_Msr.tm2[i].Cdn.y;
				B_Msr.tm2[i].DesR.right=B_Msr.tm2[i].Cdn.x+B_Msr.tm2[i].Width;
				B_Msr.tm2[i].DesR.bottom=B_Msr.tm2[i].Cdn.y+B_Msr.tm2[i].Height;
				
				if(B_Msr.tm2[i].DesR.left<0){	
					B_Msr.tm2[i].SrcR.left=B_Msr.tm2[i].SrcR.left-B_Msr.tm2[i].DesR.left;
					B_Msr.tm2[i].DesR.left=0;
				}else if(B_Msr.tm2[i].DesR.right>WinXMax){
					B_Msr.tm2[i].SrcR.right=B_Msr.tm2[i].SrcR.right-(B_Msr.tm2[i].DesR.right-WinXMax);
					B_Msr.tm2[i].DesR.right=WinXMax;
				}
				if(B_Msr.tm2[i].DesR.top<0){
					B_Msr.tm2[i].SrcR.top=B_Msr.tm2[i].SrcR.top-B_Msr.tm2[i].DesR.top;
					B_Msr.tm2[i].DesR.top=0;
				}else if(B_Msr.tm2[i].DesR.bottom>WinYMax){
					B_Msr.tm2[i].SrcR.bottom=B_Msr.tm2[i].SrcR.bottom-(B_Msr.tm2[i].DesR.bottom-WinYMax);
					B_Msr.tm2[i].DesR.bottom=WinYMax;
				}
				
				//�t���[����1���炵��0�ɂȂ�����A�j���p�[�^�[����i�߂�
				if(!(B_Msr.tm2[i].FrameCnt--)){
					//�A�j���p�^�[����i�߂�
					B_Msr.tm2[i].AnimeCnt=(B_Msr.tm2[i].AnimeCnt+1)%B_Msr.tm2[i].Anime;
					//�\������A�j���p�^�[���̃t���[�����Z�b�g
					B_Msr.tm2[i].FrameCnt=B_Msr.tm2[i].AnimeFrame[B_Msr.tm2[i].AnimeCnt];
				}
			}
		}	
	}

	if(fSPChr&_TAMA3ON){
		//�u���b�N
		for(i=0;(DWORD)i<B_Block.TTama;i++){
			//���˂���Ă��Ȃ��e�̂ݕ`�悷��
			if(B_Block.tm3[i].fTama&_TON){ 
				//SP�T�[�t�F�X����̓]����`�͈͂��Z�b�g
				//�_���[�W���󂯂Ă��邩�ǂ����`�F�b�N
				if(B_Block.tm3[i].fTama&_THIT_ON){
					B_Block.tm3[i].SrcR=TamSP[11];	//�_���[�W�V�[��
					B_Block.tm3[i].fTama &=_THIT_OFF;
				}else{
					B_Block.tm3[i].SrcR=TamSP[B_Block.tm3[i].AnimePat[B_Block.tm3[i].AnimeCnt]];
				}
				
				B_Block.tm3[i].DesR.left =B_Block.tm3[i].Cdn.x;
				B_Block.tm3[i].DesR.top  =B_Block.tm3[i].Cdn.y;
				B_Block.tm3[i].DesR.right=B_Block.tm3[i].Cdn.x+B_Block.tm3[i].Width;
				B_Block.tm3[i].DesR.bottom=B_Block.tm3[i].Cdn.y+B_Block.tm3[i].Height;
				
				if(B_Block.tm3[i].DesR.left<0){	
					B_Block.tm3[i].SrcR.left=B_Block.tm3[i].SrcR.left-B_Block.tm3[i].DesR.left;
					B_Block.tm3[i].DesR.left=0;
				}else if(B_Block.tm3[i].DesR.right>WinXMax){
					B_Block.tm3[i].SrcR.right=B_Block.tm3[i].SrcR.right-(B_Block.tm3[i].DesR.right-WinXMax);
					B_Block.tm3[i].DesR.right=WinXMax;
				}
				if(B_Block.tm3[i].DesR.top<0){
					B_Block.tm3[i].SrcR.top=B_Block.tm3[i].SrcR.top-B_Block.tm3[i].DesR.top;
					B_Block.tm3[i].DesR.top=0;
				}else if(B_Block.tm3[i].DesR.bottom>WinYMax){
					B_Block.tm3[i].SrcR.bottom=B_Block.tm3[i].SrcR.bottom-(B_Block.tm3[i].DesR.bottom-WinYMax);
					B_Block.tm3[i].DesR.bottom=WinYMax;
				}
			/*	
				//�t���[����1���炵��0�ɂȂ�����A�j���p�[�^�[����i�߂�
				if(!(B_Block.tm3[i].FrameCnt--)){
					//�A�j���p�^�[����i�߂�
					B_Block.tm3[i].AnimeCnt=(B_Block.tm3[i].AnimeCnt+1)%B_Block.tm3[i].Anime;
					//�\������A�j���p�^�[���̃t���[�����Z�b�g
					B_Block.tm3[i].FrameCnt=B_Block.tm3[i].AnimeFrame[B_Block.tm3[i].AnimeCnt];
				}
			*/
			 }else if(B_Block.tm3[i].fTama&_TBAKU_ON){
				//SP�T�[�t�F�X����̓]����`�͈͂��Z�b�g
				B_Block.tm3[i].SrcR=BakuSP[B_Block.tm3[i].BakuAnimePat[B_Block.tm3[i].BakuAnimeCnt]];
					
				B_Block.tm3[i].DesR.left =B_Block.tm3[i].Cdn.x;
				B_Block.tm3[i].DesR.top  =B_Block.tm3[i].Cdn.y;
				B_Block.tm3[i].DesR.right=B_Block.tm3[i].Cdn.x+B_Block.tm3[i].BakuWidth;
				B_Block.tm3[i].DesR.bottom=B_Block.tm3[i].Cdn.y+B_Block.tm3[i].BakuHeight;
				
				if(B_Block.tm3[i].DesR.left<0){	
					B_Block.tm3[i].SrcR.left=B_Block.tm3[i].SrcR.left-B_Block.tm3[i].DesR.left;
					B_Block.tm3[i].DesR.left=0;
				}else if(B_Block.tm3[i].DesR.right>WinXMax){
					B_Block.tm3[i].SrcR.right=B_Block.tm3[i].SrcR.right-(B_Block.tm3[i].DesR.right-WinXMax);
					B_Block.tm3[i].DesR.right=WinXMax;
				}
				if(B_Block.tm3[i].DesR.top<0){
					B_Block.tm3[i].SrcR.top=B_Block.tm3[i].SrcR.top-B_Block.tm3[i].DesR.top;
					B_Block.tm3[i].DesR.top=0;
				}else if(B_Block.tm3[i].DesR.bottom>WinYMax){
					B_Block.tm3[i].SrcR.bottom=B_Block.tm3[i].SrcR.bottom-(B_Block.tm3[i].DesR.bottom-WinYMax);
					B_Block.tm3[i].DesR.bottom=WinYMax;
				}
				
				//�t���[����1���炵��0�ɂȂ�����A�j���p�[�^�[����i�߂�
				if(!(B_Block.tm3[i].BakuFrameCnt--)){
					//�A�j���[�V�����V�[����1���[�v������I��
					if((B_Block.tm3[i].BakuAnimeCnt+1)==B_Block.tm3[i].BakuAnime){
						B_Block.tm3[i].fTama &=_TOFF;	//�e�̕\�����I�t
					}else{
						//�A�j���p�^�[����i�߂�
						B_Block.tm3[i].BakuAnimeCnt=(B_Block.tm3[i].BakuAnimeCnt+1)%B_Block.tm3[i].BakuAnime;
						//�\������A�j���p�^�[���̃t���[�����Z�b�g
						B_Block.tm3[i].BakuFrameCnt=B_Block.tm3[i].BakuAnimeFrame[B_Block.tm3[i].BakuAnimeCnt];
					}
				}
			}
		}
	}

	//----------------------------------------------------------//
	//					�{�X�L�����̕`�揀��					//
	//----------------------------------------------------------//
	
	//�{�X�L�����\���t���O�������Ă���ꍇ�`�揀��
	if(fSPChr&_SPON){
		//�{�X�L�����j��̃A�j���[�V������2�i�K(�ŏI�i�K)
		if(fSPChr&_SPDES_a)
		{
			for(i=0;i<200;i++){
				BossBaku2[i].SrcR=BakuSP[BossBaku2[i].AnimePat[BossBaku2[i].AnimeCnt]];	//�]�����T�[�t�F�X�̋�`�͈͏�����
				BossBaku2[i].DesR.left=Cdn.x+BossBaku2[i].Local.x;//�]����T�[�t�F�X�̋�`�͈͏�����
				BossBaku2[i].DesR.top=Cdn.y+BossBaku2[i].Local.y;			
				BossBaku2[i].DesR.right=BossBaku2[i].DesR.left+BossBaku2[i].Width;
				BossBaku2[i].DesR.bottom=BossBaku2[i].DesR.top+BossBaku2[i].Height;
				if(BossBaku2[i].DesR.left<0){	
					BossBaku2[i].SrcR.left=BossBaku2[i].SrcR.left-BossBaku2[i].DesR.left;
					BossBaku2[i].DesR.left=0;
				}else if(BossBaku2[i].DesR.right>WinXMax){
					BossBaku2[i].SrcR.right=BossBaku2[i].SrcR.right-(BossBaku2[i].DesR.right-WinXMax);
					BossBaku2[i].DesR.right=WinXMax;
				}
				if(BossBaku2[i].DesR.top<0){
					BossBaku2[i].SrcR.top=BossBaku2[i].SrcR.top-BossBaku2[i].DesR.top;
					BossBaku2[i].DesR.top=0;
				}else if(BossBaku2[i].DesR.bottom>WinYMax){
					BossBaku2[i].SrcR.bottom=BossBaku2[i].SrcR.bottom-(BossBaku2[i].DesR.bottom-WinYMax);
					BossBaku2[i].DesR.bottom=WinYMax;
				}
				//�t���[����1���炵��0�ɂȂ�����A�j���p�[�^�[����i�߂�
				if(!(BossBaku2[i].FrameCnt--)){
					//�A�j���[�V�����V�[����1���[�v������I��
					if((BossBaku2[i].AnimeCnt+1)==BossBaku2[i].Anime){
						//�����A�j���[�V�����f�[�^�������l�ɖ߂�
						BossBaku2[i].AnimeCnt=0;
						BossBaku2[i].FrameCnt=BossBaku2[i].AnimeFrame[0];
						fSPChr &=_SPDESOFF_a;
						fSPChr &=_SPOFF;		//�\���t���O�I�t
					}else{
						BossBaku2[i].AnimeCnt=(BossBaku2[i].AnimeCnt+1)%BossBaku2[i].Anime;	//�A�j���p�^�[����i�߂�
						BossBaku2[i].FrameCnt=BossBaku2[i].AnimeFrame[BossBaku2[i].AnimeCnt];//�\������A�j���p�^�[���̃t���[�����Z�b�g
					}
				}
			}
		}else{
		//�ʏ펞�̕`��
			//�p�[�c1
			Parts1.SrcR=SP[19];						//�]�����T�[�t�F�X�̋�`�͈͏�����
			Parts1.DesR.left=Parts1.Local.x;		//�]����T�[�t�F�X�̋�`�͈͏�����
			Parts1.DesR.top=Parts1.Local.y;			
			Parts1.DesR.right=Parts1.Local.x+Parts1.Width;
			Parts1.DesR.bottom=Parts1.Local.y+Parts1.Height;
			if(Parts1.DesR.left<0){					//��ʂ���͂ݏo�������͓]�����Ȃ�����
				Parts1.SrcR.left=Parts1.SrcR.left-Parts1.DesR.left;
				Parts1.DesR.left=0;
			}else if(Parts1.DesR.right>WinXMax){
				Parts1.SrcR.right=Parts1.SrcR.right-(Parts1.DesR.right-WinXMax);
				Parts1.DesR.right=WinXMax;
			}
			if(Parts1.DesR.top<0){
				Parts1.SrcR.top=Parts1.SrcR.top-Parts1.DesR.top;
				Parts1.DesR.top=0;
			}else if(Parts1.DesR.bottom>WinYMax){
				Parts1.SrcR.bottom=Parts1.SrcR.bottom-(Parts1.DesR.bottom-WinYMax);
				Parts1.DesR.bottom=WinYMax;
			}
			//�p�[�c2
			Parts2.SrcR=SP[20];
			Parts2.DesR.left=Parts2.Local.x;
			Parts2.DesR.top=Parts2.Local.y;			
			Parts2.DesR.right=Parts2.Local.x+Parts2.Width;
			Parts2.DesR.bottom=Parts2.Local.y+Parts2.Height;
			if(Parts2.DesR.left<0){	
				Parts2.SrcR.left=Parts2.SrcR.left-Parts2.DesR.left;
				Parts2.DesR.left=0;
			}else if(Parts2.DesR.right>WinXMax){
				Parts2.SrcR.right=Parts2.SrcR.right-(Parts2.DesR.right-WinXMax);
				Parts2.DesR.right=WinXMax;
			}
			if(Parts2.DesR.top<0){
				Parts2.SrcR.top=Parts2.SrcR.top-Parts2.DesR.top;
				Parts2.DesR.top=0;
			}else if(Parts2.DesR.bottom>WinYMax){
				Parts2.SrcR.bottom=Parts2.SrcR.bottom-(Parts2.DesR.bottom-WinYMax);
				Parts2.DesR.bottom=WinYMax;
			}
			//�p�[�c3
			Parts3.SrcR=SP[Parts3.AnimePat[Parts3.AnimeCnt]];
			Parts3.DesR.left=Parts3.Local.x;
			Parts3.DesR.top=Parts3.Local.y;			
			Parts3.DesR.right=Parts3.Local.x+Parts3.Width;
			Parts3.DesR.bottom=Parts3.Local.y+Parts3.Height;
			if(Parts3.DesR.left<0){	
				Parts3.SrcR.left=Parts3.SrcR.left-Parts3.DesR.left;
				Parts3.DesR.left=0;
			}else if(Parts3.DesR.right>WinXMax){
				Parts3.SrcR.right=Parts3.SrcR.right-(Parts3.DesR.right-WinXMax);
				Parts3.DesR.right=WinXMax;
			}
			if(Parts3.DesR.top<0){
				Parts3.SrcR.top=Parts3.SrcR.top-Parts3.DesR.top;
				Parts3.DesR.top=0;
			}else if(Parts3.DesR.bottom>WinYMax){
				Parts3.SrcR.bottom=Parts3.SrcR.bottom-(Parts3.DesR.bottom-WinYMax);
				Parts3.DesR.bottom=WinYMax;
			}
			//�t���[����1���炵��0�ɂȂ�����A�j���p�[�^�[����i�߂�
			if(!(Parts3.FrameCnt--)){
				Parts3.AnimeCnt=(Parts3.AnimeCnt+1)%Parts3.Anime;	//�A�j���p�^�[����i�߂�
				Parts3.FrameCnt=Parts3.AnimeFrame[Parts3.AnimeCnt];//�\������A�j���p�^�[���̃t���[�����Z�b�g
			}
			//�p�[�c4
			Parts4.SrcR=SP[Parts4.AnimePat[Parts4.AnimeCnt]];
			Parts4.DesR.left=Parts4.Local.x;
			Parts4.DesR.top=Parts4.Local.y;			
			Parts4.DesR.right=Parts4.Local.x+Parts4.Width;
			Parts4.DesR.bottom=Parts4.Local.y+Parts4.Height;
			if(Parts4.DesR.left<0){	
				Parts4.SrcR.left=Parts4.SrcR.left-Parts4.DesR.left;
				Parts4.DesR.left=0;
			}else if(Parts4.DesR.right>WinXMax){
				Parts4.SrcR.right=Parts4.SrcR.right-(Parts4.DesR.right-WinXMax);
				Parts4.DesR.right=WinXMax;
			}
			if(Parts4.DesR.top<0){
				Parts4.SrcR.top=Parts4.SrcR.top-Parts4.DesR.top;
				Parts4.DesR.top=0;
			}else if(Parts4.DesR.bottom>WinYMax){
				Parts4.SrcR.bottom=Parts4.SrcR.bottom-(Parts4.DesR.bottom-WinYMax);
				Parts4.DesR.bottom=WinYMax;
			}
			if(!(Parts4.FrameCnt--)){
				Parts4.AnimeCnt=(Parts4.AnimeCnt+1)%Parts4.Anime;
				Parts4.FrameCnt=Parts4.AnimeFrame[Parts4.AnimeCnt];
			}
			//�p�[�c5
			Parts5.SrcR=SP[23];	
			Parts5.DesR.left=Parts5.Local.x;
			Parts5.DesR.top=Parts5.Local.y;			
			Parts5.DesR.right=Parts5.Local.x+Parts5.Width;
			Parts5.DesR.bottom=Parts5.Local.y+Parts5.Height;
			if(Parts5.DesR.left<0){	
				Parts5.SrcR.left=Parts5.SrcR.left-Parts5.DesR.left;
				Parts5.DesR.left=0;
			}else if(Parts5.DesR.right>WinXMax){
				Parts5.SrcR.right=Parts5.SrcR.right-(Parts5.DesR.right-WinXMax);
				Parts5.DesR.right=WinXMax;
			}
			if(Parts5.DesR.top<0){
				Parts5.SrcR.top=Parts5.SrcR.top-Parts5.DesR.top;
				Parts5.DesR.top=0;
			}else if(Parts5.DesR.bottom>WinYMax){
				Parts5.SrcR.bottom=Parts5.SrcR.bottom-(Parts5.DesR.bottom-WinYMax);
				Parts5.DesR.bottom=WinYMax;
			}
			//�p�[�c6
			if(fSPChr&_SPDES && !(fSPChr&_SPDES_a)){
				for(i=0;i<12;i++){
					BossBaku1[i].SrcR=BakuSP[BossBaku1[i].AnimePat[BossBaku1[i].AnimeCnt]];	//�]�����T�[�t�F�X�̋�`�͈͏�����
					BossBaku1[i].DesR.left=Parts6.Local.x+BossBaku1[i].Local.x;				//�]����T�[�t�F�X�̋�`�͈͏�����
					BossBaku1[i].DesR.top=Parts6.Local.y+BossBaku1[i].Local.y;			
					BossBaku1[i].DesR.right=BossBaku1[i].DesR.left+BossBaku1[i].Width;
					BossBaku1[i].DesR.bottom=BossBaku1[i].DesR.top+BossBaku1[i].Height;
					if(BossBaku1[i].DesR.left<0){	
						BossBaku1[i].SrcR.left=BossBaku1[i].SrcR.left-BossBaku1[i].DesR.left;
						BossBaku1[i].DesR.left=0;
					}else if(BossBaku1[i].DesR.right>WinXMax){
						BossBaku1[i].SrcR.right=BossBaku1[i].SrcR.right-(BossBaku1[i].DesR.right-WinXMax);
						BossBaku1[i].DesR.right=WinXMax;
					}
					if(BossBaku1[i].DesR.top<0){
						BossBaku1[i].SrcR.top=BossBaku1[i].SrcR.top-BossBaku1[i].DesR.top;
						BossBaku1[i].DesR.top=0;
					}else if(BossBaku1[i].DesR.bottom>WinYMax){
						BossBaku1[i].SrcR.bottom=BossBaku1[i].SrcR.bottom-(BossBaku1[i].DesR.bottom-WinYMax);
						BossBaku1[i].DesR.bottom=WinYMax;
					}
					//�t���[����1���炵��0�ɂȂ�����A�j���p�[�^�[����i�߂�
					if(!(BossBaku1[i].FrameCnt--)){
						//�A�j���[�V�����V�[����1���[�v������I��
						if((BossBaku1[i].AnimeCnt+1)==BossBaku1[i].Anime){
							//�����A�j���[�V�����f�[�^�������l�ɖ߂�
							BossBaku1[i].AnimeCnt=0;
							BossBaku1[i].FrameCnt=BossBaku1[i].AnimeFrame[0];
							fSPChr |=_SPDES_a;	//���̔����V�[���֐i��
							//�{�X�L����������
							lpDSBuf[_BOSSBAKU]->Stop();
							lpDSBuf[_BOSSBAKU]->SetCurrentPosition(0);
							lpDSBuf[_BOSSBAKU]->Play(0,0,0);

						}else{
							BossBaku1[i].AnimeCnt=(BossBaku1[i].AnimeCnt+1)%BossBaku1[i].Anime;	//�A�j���p�^�[����i�߂�
							BossBaku1[i].FrameCnt=BossBaku1[i].AnimeFrame[BossBaku1[i].AnimeCnt];//�\������A�j���p�^�[���̃t���[�����Z�b�g
						}
					}
				}	
			}else{	
				//�_���[�W���󂯂Ă��邩�ǂ����`�F�b�N
				if(fSPChr&_HITON){
					Parts6.SrcR=SP[28];	//�_���[�W�V�[��
					fSPChr &=_HITOFF;
				}else{
					Parts6.SrcR=SP[Parts6.AnimePat[Parts6.AnimeCnt]];
				}
				Parts6.DesR.left=Parts6.Local.x;
				Parts6.DesR.top=Parts6.Local.y;			
				Parts6.DesR.right=Parts6.Local.x+Parts6.Width;
				Parts6.DesR.bottom=Parts6.Local.y+Parts6.Height;
				if(Parts6.DesR.left<0){	
					Parts6.SrcR.left=Parts6.SrcR.left-Parts6.DesR.left;
					Parts6.DesR.left=0;
				}else if(Parts6.DesR.right>WinXMax){
					Parts6.SrcR.right=Parts6.SrcR.right-(Parts6.DesR.right-WinXMax);
					Parts6.DesR.right=WinXMax;
				}
				if(Parts6.DesR.top<0){
					Parts6.SrcR.top=Parts6.SrcR.top-Parts6.DesR.top;
					Parts6.DesR.top=0;
				}else if(Parts6.DesR.bottom>WinYMax){
					Parts6.SrcR.bottom=Parts6.SrcR.bottom-(Parts6.DesR.bottom-WinYMax);
					Parts6.DesR.bottom=WinYMax;
				}
				if(!(Parts6.FrameCnt--)){
					Parts6.AnimeCnt=(Parts6.AnimeCnt+1)%Parts6.Anime;
					Parts6.FrameCnt=Parts6.AnimeFrame[Parts6.AnimeCnt];
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	�{�X�L����01�̕`��
//////////////////////////////////////////////////////////////////////
void	BOSS01::RendBoss01()
{
	short i;
	
	//----------------------------------------------------------//
	//						�e�̕`��							//
	//----------------------------------------------------------//

	//��ʒe���݃t���O�������Ă��Ȃ��ꍇ�͒e��`�悵�Ȃ�
	if(fSPChr&_TAMA2ON){
		//�~�T�C��
		for(i=0;(DWORD)i<B_Msr.TTama;i++){
			//���˒��łȂ��e�͕`�悵�Ȃ�
			if(B_Msr.tm2[i].fTama&_TON){
				lpDDSBack->BltFast(	B_Msr.tm2[i].DesR.left,B_Msr.tm2[i].DesR.top,
									lpDDS_SP,&B_Msr.tm2[i].SrcR,
									DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			}
		}
	}

	if(fSPChr&_TAMA3ON){
		//�u���b�N
		for(i=0;(DWORD)i<B_Block.TTama;i++){
			//���˒��łȂ��e�͕`�悵�Ȃ�
			if(B_Block.tm3[i].fTama&_TON || B_Block.tm3[i].fTama&_TBAKU_ON){
				lpDDSBack->BltFast(	B_Block.tm3[i].DesR.left,B_Block.tm3[i].DesR.top,
									lpDDS_SP,&B_Block.tm3[i].SrcR,
									DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			}
		}
	}
	
	//----------------------------------------------------------//
	//					�{�X�L�����̕`��						//
	//----------------------------------------------------------//
	
	//�{�X�L�����\���t���O�̗����Ă��Ȃ��ꍇ�̓��^�[��
	if(!(fSPChr&_SPON))		return;
	
	//�{�X�L�����̕`��
	//�{�X�L�����j��̃A�j���[�V������2�i�K(�ŏI�i�K)
	if(fSPChr&_SPDES_a){
		for(i=0;i<200;i++){
			lpDDSBack->BltFast(	BossBaku2[i].DesR.left,BossBaku2[i].DesR.top,
								lpDDS_SP,&BossBaku2[i].SrcR,
								DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		}
	}else{
		//�p�[�c1
		lpDDSBack->BltFast(	Parts1.DesR.left,Parts1.DesR.top,
							lpDDS_SP,&Parts1.SrcR,
							DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		//�p�[�c2
		lpDDSBack->BltFast(	Parts2.DesR.left,Parts2.DesR.top,
							lpDDS_SP,&Parts2.SrcR,
							DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		//�p�[�c3
		lpDDSBack->BltFast(	Parts3.DesR.left,Parts3.DesR.top,
							lpDDS_SP,&Parts3.SrcR,
							DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		//�p�[�c4
		lpDDSBack->BltFast(	Parts4.DesR.left,Parts4.DesR.top,
							lpDDS_SP,&Parts4.SrcR,
							DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		//�p�[�c5
		lpDDSBack->BltFast(	Parts5.DesR.left,Parts5.DesR.top,
							lpDDS_SP,&Parts5.SrcR,
							DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		//�p�[�c6
		if(fSPChr&_SPDES){
			//�����V�[�����i�K
			for(i=0;i<12;i++){
				lpDDSBack->BltFast(	BossBaku1[i].DesR.left,BossBaku1[i].DesR.top,
									lpDDS_SP,&BossBaku1[i].SrcR,
									DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			}
		}else{
			lpDDSBack->BltFast(	Parts6.DesR.left,Parts6.DesR.top,
								lpDDS_SP,&Parts6.SrcR,
								DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	�{�X�L����01�̏�ԃt���O��Ԃ�
//////////////////////////////////////////////////////////////////////
DWORD	BOSS01::GetBoss01fSPChr()
{
	return	fSPChr;
}

//////////////////////////////////////////////////////////////////////
//	�{�X�L����01�̉�ʍ��W��Ԃ�(�����蔻��̂���̂̓p�[�c6�̕����̂�)
//////////////////////////////////////////////////////////////////////
POINT	BOSS01::GetBoss01Cdn()
{
	return	Parts6.Local;
}

//////////////////////////////////////////////////////////////////////
//	�{�X�L����01�̉�����Ԃ�(�����蔻��̂���̂̓p�[�c6�̕����̂�)
//////////////////////////////////////////////////////////////////////
short	BOSS01::GetBoss01Width()
{
	return	Parts6.Width;
}

//////////////////////////////////////////////////////////////////////
//	�{�X�L����01�̏c����Ԃ�(�����蔻��̂���̂̓p�[�c6�̕����̂�)
//////////////////////////////////////////////////////////////////////
short	BOSS01::GetBoss01Height()
{
	return	Parts6.Height;
}

//////////////////////////////////////////////////////////////////////
//	�{�X�L����01�̎c��ϋv�͂̒l�̃A�h���X��Ԃ�
//////////////////////////////////////////////////////////////////////
short*	BOSS01::GetBoss01ShldCnt_P()
{
	return	&ShldCnt;
}

//////////////////////////////////////////////////////////////////////
//	�e�̏�Ԃ��Ǘ�����TAMAS2�\���̂ւ̃|�C���^��Ԃ�
//////////////////////////////////////////////////////////////////////
TAMAS2*	BOSS01::GetBoss01B_Msr_P()
{
	return	&B_Msr;
}

//////////////////////////////////////////////////////////////////////
//	�e�̏�Ԃ��Ǘ�����TAMAS3�\���̂ւ̃|�C���^��Ԃ�
//////////////////////////////////////////////////////////////////////
TAMAS3*	BOSS01::GetBoss01B_Block_P()
{
	return	&B_Block;
}