/*=============================================================================
	��C�̔��ˁA�����蔻��A�`�擙�Ɋւ���\�[�X�t�@�C��
==============================================================================*/
#include	<windows.h>
#include	<dsound.h>
#include	<ddraw.h>

#include	"Macros.h"
#include	"Draw.h"	
#include	"Struct.h"
#include	"DrawJIKI.h"
#include	"DrawJIKIDataEXT.h"
#include	"DrawMacros.h"

//�_�C���N�g�T�E���h�o�b�t�@�Đ��p�}�N����`
#define	_JIKIBAKU	0
#define	_TEKIBAKU	1
#define	_FUELARM	2
#define	_FIRE		3
#define	_TAMAKYU	4
#define	_BOSSBAKU	5

extern	LPDIRECTSOUNDBUFFER		lpDSBuf[6]; //�_�C���N�g�T�E���h�o�b�t�@
extern	LPDIRECTDRAWSURFACE7	lpDDSBack;		//�o�b�N�T�[�t�F�X

//////////////////////////////////////////////////////////////////////
//	��C���甭�˂����e�I�u�W�F�N�g�̏�����
//////////////////////////////////////////////////////////////////////
BOOL	DrawJIKI::InitMainWepn()
{	
	//////////////////////////////////////////////////////////////////////
	//------���[�U�[�P�̃X�e�[�^�X���Ǘ�����\���̂̏�����---------------
	
	//�e�X�e�[�^�X�\���̂ɒl���Z�b�g���Ēe�����m�ۂ����A�e�I�u�W�F�N�g��������
	LSR1.TTama	=6;				//��ʏ�ɑ��݂ł���e�̐�
	LSR1.WTama	=10;			//�e�𔭎˂���Ԋu
	LSR1.WTamaCnt=LSR1.WTama;	//�e���˂܂ł̃J�E���^

	TAMA	tama;				//TAMA�\���̂ɑO�����Ēl���Z�b�g���ď���������

	tama.fTama=_TOFF;			//�e�̏�����Ԃ�ݒ�
	tama.Width=4;				//�e�̉���
	tama.Height=72;				//�e�̏c��
	tama.Mov.x=0.0;				//�e�̈ړ����x��������
	tama.Mov.y=20.0;	
	tama.dCdn.x=0;				//�e�̉�ʍ��W�v�Z�p
	tama.dCdn.y=0;
	tama.Cdn.x=0;				//�e�̌��݂̉�ʍ��W��0�ŏ�����
	tama.Cdn.y=0;
	tama.MapCdn.x=0;			//�e�̌��݂̐�΍��W��0�ŏ�����
	tama.MapCdn.y=0;
	tama.Anime=TamAmeJIKI[0];	//�e�A�j���[�V�����p�^�[�����̏�����	
	//�e�A�j���[�V�����p�^�[���z��̏�����
	memcpy(&tama.AnimePat[0],&TamPatJIKI[0][0],sizeof(BYTE)*10);
	//�e�A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	memcpy(&tama.AnimeFrame[0],&TamFrameJIKI[0][0],sizeof(BYTE)*10);
	tama.AnimeCnt=0;					//�ŏ��ɕ\������A�j���[�V�����p�^�[����������
	tama.FrameCnt=tama.AnimeFrame[0];	//�ŏ��̃A�j���[�V�����t���[���J�E���g��������	
	tama.SrcR.left=0;			//�]������`��0�ŏ�����
	tama.SrcR.top=0;
	tama.SrcR.right=0;
	tama.SrcR.bottom=0;
	tama.DesR=tama.SrcR;		//�]�����`��0�ŏ�����
	
	//���˂ł���e����TAMA�\���̂��m��
	LSR1.tm=new TAMA[LSR1.TTama];
	if(!LSR1.tm)		return	FALSE;
	//�m�ۂ����̈�̐擪�A�h���X���R�s�[
	char *pTama;
	pTama=(char*)LSR1.tm;		
	//�e�I�u�W�F�N�g���m�ۂ������A������ 
	for(int i=0;(DWORD)i<LSR1.TTama;i++){
		memcpy(pTama,&tama,sizeof(TAMA));	//�m�ۂ����̈�̐擪����R�s�[
		pTama +=sizeof(TAMA);				//���ɃR�s�[����Ƃ���܂ŃA�h���X��i�߂�
	}

	//////////////////////////////////////////////////////////////////////	
	//-----�~�T�C��1�̃X�e�[�^�X���Ǘ�����\���̂̏�����------------------

	MSR1.TTama	=60;			//��ʏ�ɑ��݂ł���e�̐�
	MSR1.WTama	=8;				//�e�𔭎˂���Ԋu
	MSR1.WTamaCnt=MSR1.WTama;	//�e���˂܂ł̃J�E���^

	tama.fTama=_TOFF;			//�e�̏�����Ԃ�ݒ�
	tama.Width=16;				//�e�̉���
	tama.Height=32;				//�e�̏c��
	tama.Mov.x=0.0;				//�e�̈ړ����x��������
	tama.Mov.y=0.0;	
	tama.dCdn.x=0;				//�e�̉�ʍ��W�v�Z�p
	tama.dCdn.y=0;
	tama.Cdn.x=0;				//�e�̌��݂̉�ʍ��W��0�ŏ�����
	tama.Cdn.y=0;
	tama.MapCdn.x=0;			//�e�̌��݂̐�΍��W��0�ŏ�����
	tama.MapCdn.y=0;
	tama.Anime=TamAmeJIKI[1];	//�e�A�j���[�V�����p�^�[�����̏�����
	memcpy(&tama.AnimePat[0],&TamPatJIKI[1][0],sizeof(BYTE)*10);	//�e�A�j���[�V�����p�^�[���z��̏�����
	memcpy(&tama.AnimeFrame[0],&TamFrameJIKI[1][0],sizeof(BYTE)*10);//�e�A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	tama.AnimeCnt=0;					//�ŏ��ɕ\������A�j���[�V�����p�^�[����������
	tama.FrameCnt=tama.AnimeFrame[0];	//�ŏ��̃A�j���[�V�����t���[���J�E���g��������	
	tama.SrcR.left=0;					//�]������`��0�ŏ�����
	tama.SrcR.top=0;
	tama.SrcR.right=0;
	tama.SrcR.bottom=0;
	tama.DesR=tama.SrcR;				//�]�����`��0�ŏ�����
	
	//���˂ł���e����TAMA�\���̂��m��
	MSR1.tm=new TAMA[MSR1.TTama];
	if(!MSR1.tm)		return	FALSE;
	//�m�ۂ����̈�̐擪�A�h���X���R�s�[
	pTama=(char*)MSR1.tm;		
	//�e�I�u�W�F�N�g���m�ۂ������A������ 
	for(i=0;(DWORD)i<MSR1.TTama;i++){
		memcpy(pTama,&tama,sizeof(TAMA));	//�m�ۂ����̈�̐擪����R�s�[
		pTama +=sizeof(TAMA);				//���ɃR�s�[����Ƃ���܂ŃA�h���X��i�߂�
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////
//	���I�Ɋm�ۂ��郁�����̈�ւ̃|�C���^��NULL�ŏ�����
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::InitWepnTm()
{
	LSR1.tm=NULL;
	MSR1.tm=NULL;
}

//////////////////////////////////////////////////////////////////////
//	�m�ۂ����������̈�����
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::MainWepnRelease()
{	
	RELEASE_dels(MSR1.tm);
	RELEASE_dels(LSR1.tm);
}

//////////////////////////////////////////////////////////////////////
//	��C���˂܂ł̃J�E���g��i�߂�
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::MainWepnCnt()	
{
	//���ݎg�p���Ă����C���`�F�b�N���ăt���O�̗����Ă����C�̏���������
	switch(Wpd.flag&_MAINWP_CHK){
		case _LSR_ON:
			switch (Wpd.flag&_LSR_LVCHK){
				case _LSR1_ON:
					LSR1.WTamaCnt--;	//���˂܂ł̃J�E���g�_�E��
					break;
				default:
					break;
			}
			break;
		case _MSR_ON:
			switch (Wpd.flag&_MSR_LVCHK){
				case _MSR1_ON:
					MSR1.WTamaCnt--;	//���˂܂ł̃J�E���g�_�E��
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////
//	��C���˂܂ł̃J�E���^�����Z�b�g������
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::MainWepnCntReset()	
{
	//���[�U�[�̔��˃J�E���g�����Z�b�g
	switch (Wpd.flag&_LSR_LVCHK){
		case _LSR1_ON:
			LSR1.WTamaCnt=LSR1.WTama;	//���˂܂ł̃J�E���^���ăZ�b�g
			break;
		default:
			break;
	}
	
	//�~�T�C���̔��˃J�E���g�����Z�b�g
	switch (Wpd.flag&_MSR_LVCHK){
		case _MSR1_ON:
			MSR1.WTamaCnt=MSR1.WTama;	//���˂܂ł̃J�E���^���ăZ�b�g
			break;
		default:
			break;		
	}
}

//////////////////////////////////////////////////////////////////////
//	��C���˂̏����ƈړ����������܂�
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::MainWepn()	
{
	short	i,j;
	
	//��ʂɒe�����݂��Ă��邩���Ă��Ȃ������ׂ�t���O
	bool	TamaS=FALSE;	
	
	//���˂ł����ԂɂȂ��Ă��郌�[�U�[�̔ԍ��ۑ��p
	static	DWORD	TTamaNum1=LSR1.TTama,TTamaNum2=LSR1.TTama;

	//���[�U�[�̔��˂ƈړ�
	switch(Wpd.flag&_LSR_LVCHK){	//�����̃��x���𒲂ׂĂ���
		case _LSR1_ON:
			//-----------------���[�U�[�̔��ˏ���-------------------//
			
			if(!LSR1.WTamaCnt){
				//���[�U�[���w�肳�ꂽ���ȏ�ɉ�ʂɑ��݂��Ȃ��悤�ɂ��鏈��
				if(TTamaNum1<LSR1.TTama && TTamaNum2<LSR1.TTama){
					//��ʏ�ɒe�����˂���Ă���t���O�I��
					fJIKI |=_WEPN_ON;
					//���˒��̃t���O�𗧂Ă�
					LSR1.tm[TTamaNum1].fTama |=_TON;
					LSR1.tm[TTamaNum2].fTama |=_TON;
					//�e�A�j���[�V������������	
					LSR1.tm[TTamaNum1].AnimeCnt=0;
					LSR1.tm[TTamaNum1].FrameCnt=LSR1.tm[TTamaNum1].AnimeFrame[0];	
					LSR1.tm[TTamaNum2].AnimeCnt=0;
					LSR1.tm[TTamaNum2].FrameCnt=LSR1.tm[TTamaNum2].AnimeFrame[0];	
					//���[�U�[�̔��˂��ꂽ���W��������
					//��ʍ��W���Z�b�g
					LSR1.tm[TTamaNum1].Cdn.x=Cdn.x-2;
					LSR1.tm[TTamaNum1].Cdn.y=Cdn.y-(LSR1.tm[TTamaNum1].Height-32);
					LSR1.tm[TTamaNum2].Cdn.x=Cdn.x+(_JIKI_W-2);
					LSR1.tm[TTamaNum2].Cdn.y=Cdn.y-(LSR1.tm[TTamaNum2].Height-32);
					//�e�̈ړ��v�Z�p�ɉ�ʍ��W���擾
					LSR1.tm[TTamaNum1].dCdn.x=LSR1.tm[TTamaNum1].Cdn.x;
					LSR1.tm[TTamaNum1].dCdn.y=LSR1.tm[TTamaNum1].Cdn.y;
					LSR1.tm[TTamaNum2].dCdn.x=LSR1.tm[TTamaNum2].Cdn.x;
					LSR1.tm[TTamaNum2].dCdn.y=LSR1.tm[TTamaNum2].Cdn.y;
					//��΍��W���Z�b�g
					LSR1.tm[TTamaNum1].MapCdn.x=MainCdn.x+LSR1.tm[TTamaNum1].Cdn.x;
					LSR1.tm[TTamaNum1].MapCdn.y=MainCdn.y+LSR1.tm[TTamaNum1].Cdn.y;
					LSR1.tm[TTamaNum2].MapCdn.x=MainCdn.x+LSR1.tm[TTamaNum2].Cdn.x;
					LSR1.tm[TTamaNum2].MapCdn.y=MainCdn.y+LSR1.tm[TTamaNum2].Cdn.y;
				
					TTamaNum1=LSR1.TTama;			//LSR1.TTama�ŏ�����
					TTamaNum2=LSR1.TTama;			//���̏�����TTamaNum���㏑������Ȃ�����
													//�V�����e�𔭎˂ł��Ȃ��悤�ɂ��Ă��܂�
					//���ˉ�
					lpDSBuf[_FIRE]->Stop();
					lpDSBuf[_FIRE]->SetCurrentPosition(0);
					lpDSBuf[_FIRE]->Play(0,0,0);
				}
				LSR1.WTamaCnt=LSR1.WTama;	//���˂܂ł̃J�E���^���ăZ�b�g
			}
			
			//--------------------���[�U�[�̈ړ�����-------------------//

			for(i=0;(DWORD)i<LSR1.TTama;i++){	
				//���˒��t���O�̗����Ă��Ȃ��e�͏������Ȃ�
				if(LSR1.tm[i].fTama&_TON)
				{
					TamaS=TRUE;	//���[�U�[�͉�ʓ��ɑ��݂��Ă���
					
					//���[�U�[���ړ�������
					LSR1.tm[i].dCdn.x +=LSR1.tm[i].Mov.x;
					LSR1.tm[i].dCdn.y -=LSR1.tm[i].Mov.y;
					LSR1.tm[i].Cdn.x=(LONG)LSR1.tm[i].dCdn.x;
					LSR1.tm[i].Cdn.y=(LONG)LSR1.tm[i].dCdn.y;
					LSR1.tm[i].MapCdn.x=MainCdn.x+LSR1.tm[i].Cdn.x;
					LSR1.tm[i].MapCdn.y=MainCdn.y+LSR1.tm[i].Cdn.y;	
					//���[�U�[����ʂ̊O�Ɉړ����Ă��Ȃ����`�F�b�N
					if(LSR1.tm[i].Cdn.y+LSR1.tm[i].Height<0){
						LSR1.tm[i].fTama &=_TOFF;	//�t���O���N���A
						continue;
					}
				
					//SP�T�[�t�F�X����̓]����`�͈͂��Z�b�g
					LSR1.tm[i].SrcR=TamSPJIKI[LSR1.tm[i].AnimePat[LSR1.tm[i].AnimeCnt]];
							
					LSR1.tm[i].DesR.left  =LSR1.tm[i].Cdn.x;
					LSR1.tm[i].DesR.top   =LSR1.tm[i].Cdn.y;
					LSR1.tm[i].DesR.right =LSR1.tm[i].Cdn.x+LSR1.tm[i].Width;
					LSR1.tm[i].DesR.bottom=LSR1.tm[i].Cdn.y+LSR1.tm[i].Height;
							
					//��ʊO�̕����͕`�悵�Ȃ��悤�ɂ��鏈��
					if(LSR1.tm[i].DesR.left<0){	
						LSR1.tm[i].SrcR.left=LSR1.tm[i].SrcR.left-LSR1.tm[i].DesR.left;
						LSR1.tm[i].DesR.left=0;
					}else if(LSR1.tm[i].DesR.right>WinXMax){
						LSR1.tm[i].SrcR.right=LSR1.tm[i].SrcR.right-(LSR1.tm[i].DesR.right-WinXMax);
						LSR1.tm[i].DesR.right=WinXMax;	
					}
					if(LSR1.tm[i].DesR.top<0){
						LSR1.tm[i].SrcR.top=LSR1.tm[i].SrcR.top-LSR1.tm[i].DesR.top;
						LSR1.tm[i].DesR.top=0;
					}
					//�t���[����1���炵��0�ɂȂ�����A�j���p�^�[����i�߂�
					if(!(LSR1.tm[i].FrameCnt--)){
						LSR1.tm[i].AnimeCnt=(LSR1.tm[i].AnimeCnt+1)%LSR1.tm[i].Anime;//�A�j���p�^�[����i�߂�
						LSR1.tm[i].FrameCnt=LSR1.tm[i].AnimeFrame[LSR1.tm[i].AnimeCnt];//�\������A�j���p�^�[���̃t���[�����Z�b�g
					}
				}else if(LSR1.tm[i].fTama==_TOFF){	
					//�Ĕ��ˉ\�ȃ��[�U�[�̔ԍ���ۑ�����
					if(TTamaNum1==LSR1.TTama){
						TTamaNum1=i;
						LSR1.tm[i].fTama |=_TSET;	//���[�U�[���Ăє��˃Z�b�g
					}else if(TTamaNum2==LSR1.TTama){
						TTamaNum2=i;
						LSR1.tm[i].fTama |=_TSET;	//���[�U�[���Ăє��˃Z�b�g
					}else if(!(fJIKI&_WEPN_ON)){
						break;						//��ʂɒe�����݂��Ȃ��΃��[�v�𔲂���
					}
				}
			}
			break;
		default:
			break;	
	}
	
	//�~�T�C���̔��˂ƈړ�
	
	//���˂ł����ԂɂȂ��Ă���~�T�C���̔ԍ��ۑ��p
	static	DWORD	TTamaNum[12]={
		MSR1.TTama,MSR1.TTama,MSR1.TTama,MSR1.TTama,MSR1.TTama,MSR1.TTama,
		MSR1.TTama,MSR1.TTama,MSR1.TTama,MSR1.TTama,MSR1.TTama,MSR1.TTama
	};

	WORD	Kakudo=100;	//�~�T�C���̔��ˊp�x
	bool	ret=TRUE;	//12�������ˉ\�Ȓe���m�ۂ���Ă��邩�`�F�b�N���[�v�p	
	
	switch(Wpd.flag&_MSR_LVCHK){	//�����̃��x���𒲂ׂĂ���	
		case _MSR1_ON:
			//-----------------�~�T�C���̔���------------------------//
			
			if(!MSR1.WTamaCnt){
				//�~�T�C�����w�肳�ꂽ���ȏ�ɉ�ʂɑ��݂��Ȃ��悤�ɂ��鏈��
				for(i=0;i<12;i++){	//12�������ˉ\�Ȓe���m�ۂ���Ă��邩�`�F�b�N
					if(TTamaNum[i]>=MSR1.TTama)	ret=FALSE;
				}
				
				if(ret)
				{
					//��ʏ�ɒe�����˂���Ă���t���O�I��
					fJIKI |=_WEPN_ON;
					
					//�~�T�C���̔��˂��ꂽ���W��������
					//��ʍ��W���Z�b�g
					for(i=0;i<12;i++){
						//���˒��̃t���O�𗧂Ă�
						MSR1.tm[TTamaNum[i]].fTama |=_TON;		
						//�e�A�j���[�V������������
						MSR1.tm[TTamaNum[i]].AnimeCnt=0;		
						MSR1.tm[TTamaNum[i]].FrameCnt=MSR1.tm[TTamaNum[i]].AnimeFrame[0];	
						//�������W
						if(i<3){
							MSR1.tm[TTamaNum[i]].Cdn.x=Cdn.x-8;
							MSR1.tm[TTamaNum[i]].Cdn.y=Cdn.y-8;
						}else if(i<6){
							MSR1.tm[TTamaNum[i]].Cdn.x=Cdn.x-4;
							MSR1.tm[TTamaNum[i]].Cdn.y=Cdn.y-10;
						}else if(i<9){
							MSR1.tm[TTamaNum[i]].Cdn.x=Cdn.x+8;
							MSR1.tm[TTamaNum[i]].Cdn.y=Cdn.y-10;
						}else if(i<12){
							MSR1.tm[TTamaNum[i]].Cdn.x=Cdn.x+12;
							MSR1.tm[TTamaNum[i]].Cdn.y=Cdn.y-8;
						}
						//�e�̈ړ��v�Z�p�ɉ�ʍ��W���擾
						MSR1.tm[TTamaNum[i]].dCdn.x=MSR1.tm[TTamaNum[i]].Cdn.x;
						MSR1.tm[TTamaNum[i]].dCdn.y=MSR1.tm[TTamaNum[i]].Cdn.y;
						//��΍��W���Z�b�g
						MSR1.tm[TTamaNum[i]].MapCdn.x=MainCdn.x+MSR1.tm[TTamaNum[i]].Cdn.x;
						MSR1.tm[TTamaNum[i]].MapCdn.y=MainCdn.y+MSR1.tm[TTamaNum[i]].Cdn.y;
						//�ړ����x�̏�����
						MSR1.tm[TTamaNum[i]].Mov.x=16*Cos[Kakudo];
						MSR1.tm[TTamaNum[i]].Mov.y=16*Sin[Kakudo];
						Kakudo -=2;
						if(i==5)	Kakudo +=2;//���ˊp�x90�x�̃~�T�C�����Q�����˂��鏈��	
					
						//MSR1.TTama�ŏ�����
						//���̏�����TTamaNum���㏑������Ȃ�����V�����e�𔭎˂ł��Ȃ��悤�ɂ��Ă��܂�
						TTamaNum[i]=MSR1.TTama;
					
						//���ˉ�
						lpDSBuf[_FIRE]->Stop();
						lpDSBuf[_FIRE]->SetCurrentPosition(0);
						lpDSBuf[_FIRE]->Play(0,0,0);
					}
					MSR1.WTamaCnt=MSR1.WTama;	//���˂܂ł̃J�E���^���ăZ�b�g
				}
			}

			//-------------------�~�T�C���̈ړ�----------------------//
			
			for(i=0;(DWORD)i<MSR1.TTama;i++){	
				//���˒��t���O�̗����Ă��Ȃ��e�͏������Ȃ�
				if(MSR1.tm[i].fTama&_TON)
				{
					TamaS=TRUE;	//�~�T�C���͉�ʓ��ɑ��݂��Ă���
					
					//�~�T�C�����ړ�������
					MSR1.tm[i].dCdn.x +=MSR1.tm[i].Mov.x;
					MSR1.tm[i].dCdn.y -=MSR1.tm[i].Mov.y;
					MSR1.tm[i].Cdn.x=(LONG)MSR1.tm[i].dCdn.x;
					MSR1.tm[i].Cdn.y=(LONG)MSR1.tm[i].dCdn.y;
					MSR1.tm[i].MapCdn.x=MainCdn.x+MSR1.tm[i].Cdn.x;
					MSR1.tm[i].MapCdn.y=MainCdn.y+MSR1.tm[i].Cdn.y;	
					//�~�T�C������ʂ̊O�Ɉړ����Ă��Ȃ����`�F�b�N
					if(	MSR1.tm[i].Cdn.x+MSR1.tm[i].Width<0	||
						MSR1.tm[i].Cdn.x>WinXMax			||
						MSR1.tm[i].Cdn.y+MSR1.tm[i].Height<0)
					{
							MSR1.tm[i].fTama &=_TOFF;	//�t���O���N���A
							continue;
					}
				
					//SP�T�[�t�F�X����̓]����`�͈͂��Z�b�g
					MSR1.tm[i].SrcR=TamSPJIKI[MSR1.tm[i].AnimePat[MSR1.tm[i].AnimeCnt]];
							
					MSR1.tm[i].DesR.left  =MSR1.tm[i].Cdn.x;
					MSR1.tm[i].DesR.top   =MSR1.tm[i].Cdn.y;
					MSR1.tm[i].DesR.right =MSR1.tm[i].Cdn.x+MSR1.tm[i].Width;
					MSR1.tm[i].DesR.bottom=MSR1.tm[i].Cdn.y+MSR1.tm[i].Height;
							
					//��ʊO�̕����͕`�悵�Ȃ��悤�ɂ��鏈��
					if(MSR1.tm[i].DesR.left<0){	
						MSR1.tm[i].SrcR.left=MSR1.tm[i].SrcR.left-MSR1.tm[i].DesR.left;
						MSR1.tm[i].DesR.left=0;
					}
					if(MSR1.tm[i].DesR.right>WinXMax){
						MSR1.tm[i].SrcR.right=MSR1.tm[i].SrcR.right-(MSR1.tm[i].DesR.right-WinXMax);
						MSR1.tm[i].DesR.right=WinXMax;
					}
					if(MSR1.tm[i].DesR.top<0){
						MSR1.tm[i].SrcR.top=MSR1.tm[i].SrcR.top-MSR1.tm[i].DesR.top;
						MSR1.tm[i].DesR.top=0;
					}
					
					//�t���[����1���炵��0�ɂȂ�����A�j���p�^�[����i�߂�
					if(!(MSR1.tm[i].FrameCnt--)){
						MSR1.tm[i].AnimeCnt=(MSR1.tm[i].AnimeCnt+1)%MSR1.tm[i].Anime;//�A�j���p�^�[����i�߂�
						MSR1.tm[i].FrameCnt=MSR1.tm[i].AnimeFrame[MSR1.tm[i].AnimeCnt];//�\������A�j���p�^�[���̃t���[�����Z�b�g
					}
				
				}else if(MSR1.tm[i].fTama==_TOFF){	
					//�Ĕ��ˉ\�ȃ~�T�C���̔ԍ���ۑ�����
					for(j=0;(DWORD)j<12;j++){
						if(TTamaNum[j]==MSR1.TTama){
							TTamaNum[j]=i;
							MSR1.tm[i].fTama |=_TSET;	//�~�T�C�����Ăє��˃Z�b�g
							break;
						}
					}
					//��ʂɒe�����݂��Ă��Ȃ����̓��[�v�𔲂���
					if(!(TTamaNum[11]==MSR1.TTama) && !(fJIKI&_WEPN_ON)){
						break;
					}
				}
			}
			break;
		default:
			break;	
	}
	
	//�e�����݂��Ă��Ȃ��������C���˃t���O���I�t
	if(!TamaS)	fJIKI &=_WEPN_OFF;
}

//////////////////////////////////////////////////////////////////////
//	��C�̒e�ƓG�L�����Ƃ̓����蔻��
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::MainWepnAtari()
{
	short i,j;

	//���˂���Ă��郌�[�U�[�ƓG�L�����̓����蔻��
	switch (Wpd.flag&_LSR_LVCHK){
		case _LSR1_ON:
			for(i=0;(DWORD)i<LSR1.TTama;i++){
				//�����˂̒e�Ƃ͓����蔻������Ȃ�
				if(LSR1.tm[i].fTama&_TON){
					if(!(Atari(LSR1.tm[i].DesR))){
						JikiTekiDmg(_LSR1DMG);	//�G�L�����Ƀ_���[�W
					}
				
					//�j��\�Ȓe�Ƃ̓����蔻��
					if(SP_Tamas3){
						for(j=0;(DWORD)j<SP_Tamas3->TTama;j++){
							if(SP_Tamas3->tm3[j].fTama&_TON){
								if(!(Atari(LSR1.tm[i].DesR,SP_Tamas3->tm3[j].DesR))){
									JikiTamas3Dmg(_LSR1DMG,j);
								}
							}
						}
					}
				}
			}
			break;
		default:
			break;
	}

	//���˂���Ă���~�T�C���ƓG�L�����̓����蔻��
	switch (Wpd.flag&_MSR_LVCHK){
		case _MSR1_ON:
			for(i=0;(DWORD)i<MSR1.TTama;i++){
				//�����˂̒e�Ƃ͓����蔻������Ȃ�
				if(MSR1.tm[i].fTama&_TON){
					if(!(Atari(MSR1.tm[i].DesR))){
						//�~�T�C�������ł�����
						MSR1.tm[i].fTama=_TOFF;		//�t���O���N���A
						JikiTekiDmg(_MSR1DMG);	//�G�L�����Ƀ_���[�W
					}
				
					//�j��\�Ȓe�Ƃ̓����蔻��
					if(SP_Tamas3){
						for(j=0;(DWORD)j<SP_Tamas3->TTama;j++){
							if(SP_Tamas3->tm3[j].fTama&_TON){
								if(!(Atari(MSR1.tm[i].DesR,SP_Tamas3->tm3[j].DesR))){
									//�~�T�C�������ł�����
									MSR1.tm[i].fTama=_TOFF;		//�t���O���N���A
									JikiTamas3Dmg(_MSR1DMG,j);
								}
							}
						}
					}
				}
			}
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////
//	��C�̒e��S�Ĕ��ˑҋ@��Ԃɖ߂�
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::MainWepnReset()
{
	MainWepnCntReset(); //��C���˂܂ł̃J�E���^�����Z�b�g
	
	short i;
	
	//�S�Ẵ��[�U�[�̃t���O�𔭎ˑҋ@��ԂɃZ�b�g
	switch (Wpd.flag&_LSR_LVCHK){
		case _LSR1_ON:
			for(i=0;(DWORD)i<LSR1.TTama;i++){
				LSR1.tm[i].fTama=_TOFF;	//�t���O���N���A
			}
			break;
		default:
			break;
	}
	//�S�Ẵ~�T�C���̃t���O�𔭎ˑҋ@��ԂɃZ�b�g		
	switch (Wpd.flag&_MSR_LVCHK){
		case _MSR1_ON:
			for(i=0;(DWORD)i<MSR1.TTama;i++){
				MSR1.tm[i].fTama=_TOFF;	//�t���O���N���A
			}
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////
//	��C�̒e��`�悷��
//////////////////////////////////////////////////////////////////////
void	DrawJIKI::MainWepnRend()
{	
	short i;
	
	//���[�U�[�̕`��
	switch (Wpd.flag&_LSR_LVCHK){
		case _LSR1_ON:	//���[�U�[�P
			for(i=0;(DWORD)i<LSR1.TTama;i++){	
				//���˒��t���O�̗����Ă��Ȃ��e�͕`�悵�Ȃ�
				if(LSR1.tm[i].fTama&_TON)
				{	
					lpDDSBack->BltFast(	LSR1.tm[i].DesR.left,LSR1.tm[i].DesR.top,
										lpDDS_JIKI,&LSR1.tm[i].SrcR,
										DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
				}
			}	
			break;
		default:
			break;
	}
	//�~�T�C���̕`��
	switch (Wpd.flag&_MSR_LVCHK){
		case _MSR1_ON:	//�~�T�C���P
			for(i=0;(DWORD)i<MSR1.TTama;i++){	
				//���˒��t���O�̗����Ă��Ȃ��e�͕`�悵�Ȃ�
				if(MSR1.tm[i].fTama&_TON)
				{	
					lpDDSBack->BltFast(	MSR1.tm[i].DesR.left,MSR1.tm[i].DesR.top,
										lpDDS_JIKI,&MSR1.tm[i].SrcR,
										DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
				}
			}	
			break;
		default:
			break;
	}
}