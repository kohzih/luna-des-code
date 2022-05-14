/*=============================================================================
	�w�i�̏����Ɋւ���\�[�X�t�@�C��
==============================================================================*/

#include	<windows.h>
#include	<ddraw.h>
//#include	<dinput.h>

#include	"Macros.h"
#include	"Draw.h"
#include	"DrawBG.h"
#include	"DrawMacros.h"

extern	LPDIRECTDRAWSURFACE7	lpDDSBack;

//extern	BYTE	JoyNew,JoyOld;
//			BYTE	Joy_Get(void);

/////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

DrawBG::DrawBG()
{
	fBG=0;
	JK_fJIKI=0;	
	ScrCnt=0;
	ScrSpd=0;
	ScrWt=0;
}

DrawBG::~DrawBG()
{

}

//////////////////////////////////////////////////////////////////////
// DrawBG�I�u�W�F�N�g������������֐�
//////////////////////////////////////////////////////////////////////
BOOL	DrawBG::InitBG()
{
	//���݂̃X�e�[�W�ԍ��ɂ���ď����𕪊򂳂���
	switch(StageNum){
		case 0:
		case 1:	
			ScrCnt=0;	//�w�i�X�N���[���J�E���^
			ScrSpd=0;	//�w�i�X�N���[���X�s�[�h
			ScrWt=0;	//�X�N���[���E�F�C�g�J�E���^
			break;
		case 2:
			ScrCnt=0;
			ScrSpd=1;
			ScrWt=0;
			break;
		case 1000:
		case 2000:
			ScrCnt=0;
			ScrSpd=0;
			ScrWt=0;
			break;
		default:
			break;
	}
	
	return	TRUE;
}

//////////////////////////////////////////////////////////////////////
// ��΍��W���ړ�������(�w�i�X�N���[��)
//////////////////////////////////////////////////////////////////////
void	DrawBG::MoveBG()
{
	//�X�e�[�W�t���O�ɉ����t���O�������Ă���ꍇ���^�[��
	if(fSTAGE&_fSTAGE_CHK)		return;	
	//�Q�[�����ꎞ��~������t���O�������Ă���ꍇ���^�[��
	if(JK_fJIKI&_GAME_STOP_ON)		return;

/*
	//�W���C�X�e�B�b�Nor�L�[�|�[�h�ɂ��w�i�̈ړ�
	JoyOld=JoyNew;
	JoyNew=Joy_Get();

	if(JoyNew & JOY_UP)					//�������			
	{
		MainCdn.y -=ScrSpd;						
	}
	if(JoyNew & JOY_DOWN)				//��������
	{
		MainCdn.y +=ScrSpd;								
	}
*/	
	//��ʂ���Ɍ����ăX�N���[��
	MainCdn.y -=ScrSpd;
	
	//��΍��Wy����ʂ̏�[�ɂȂ������ʃX�N���[���X�g�b�v
	if(MainCdn.y<0)	MainCdn.y=0; 
	
	//��΍��Wy����ʂ̏�[�ɂȂ������ʃX�N���[���X�g�b�v
	//MainCdn.y =MainCdn.y<0 ? 0:MainCdn.y;
	//��΍��Wy����ʂ̉��[�ɂȂ������ʃX�N���[���X�g�b�v
	//MainCdn.y =MainCdn.y>(long)(MapSizeY*TipSizeY-WinYMax) ? MapSizeY*TipSizeY-WinYMax : MainCdn.y;

/*
	MainCdn.x +=x;
	if(MainCdn.x<0)		MainCdn.x=0;
	if( MainCdn.x>(long)(MapSizeX*TipSizeX-WinXMax-1) )	MainCdn.x=MapSizeX*TipSizeX-WinXMax;
*/	
}

//////////////////////////////////////////////////////////////////////
// �󂯎�����t���O�ɂ���ėl�X�ȏ���������
//////////////////////////////////////////////////////////////////////
void	DrawBG::FrameBG(DWORD fJ)
{
	JK_fJIKI=fJ;		//���@�̏�ԃt���O���R�s�[
	
	//�X�e�[�W�t���O�ɉ����t���O�������Ă���ꍇ���^�[��
	if(fSTAGE&_fSTAGE_CHK)		return;	
	//�Q�[�����ꎞ��~������t���O�������Ă���ꍇ���^�[��
	if(JK_fJIKI&_GAME_STOP_ON)		return;

	//**********���̉��ɏ������L������\��**********

	//**********************************************
}

//////////////////////////////////////////////////////////////////////
// �w�i�`��
// (X���̃X�N���[�����T�|�[�g���Ă���)
//////////////////////////////////////////////////////////////////////
void	DrawBG::RendBG()
{
	int		i;
	RECT	rc;

	switch(StageNum){
		//�v�����[�O��ʂ̕`��
		case 0:
			lpDDSBack->BltFast(240,64,lpDDS_BG,NULL,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			break;
		//�^�C�g����ʂ̕`��
		case 1:
			lpDDSBack->BltFast(0,0,lpDDS_BG,NULL,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			if(fBG&_SetBG00_ON){
				rc.left=96;		//END�I��
				rc.top=0;
				rc.right=192;
				rc.bottom=64;
			}else{
				rc.left=0;		//START�I��
				rc.top=0;
				rc.right=96;
				rc.bottom=64;
			}
			lpDDSBack->BltFast(272,284,lpDDS_BG2,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			break;
		//�X�e�[�W1�̕`��
		case 2:
			//��΍��W���猻�ݕ`�悷�ׂ��w�i�̈ʒu���v�Z
			int PartsNum;
	
			PartsNum=(MainCdn.x / TipSizeX)+(MainCdn.y / TipSizeY * MapSizeX);
	
			//��΍��W���猻�ݕ`�悷���ʒ[�w�i�X�v���C�g�̌�������v�Z
			int	PartsLackX,PartsLackY;
	
			PartsLackX=MainCdn.x % TipSizeX;
			PartsLackY=MainCdn.y % TipSizeY;
	
			//�ŏ�i���[��`��
			SetRect(&rc,
					Parts[PartsNum].X+PartsLackX,
					Parts[PartsNum].Y+PartsLackY,
					Parts[PartsNum].X+TipSizeX,
					Parts[PartsNum].Y+TipSizeY);
	
			lpDDSBack->BltFast(	0,0,
								lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);	
	
			//�ŏ�i�̕`��
			for(i=1;(DWORD)i<WinXMax/MapSizeX;i++)
			{
				SetRect(&rc,
						Parts[PartsNum+i].X,
						Parts[PartsNum+i].Y+PartsLackY,
						Parts[PartsNum+i].X+TipSizeX,
						Parts[PartsNum+i].Y+TipSizeY);

				lpDDSBack->BltFast(	i*TipSizeX-PartsLackX,0,
									lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			}

			//�ŏ�i�E�[�̕`��
			SetRect(&rc,
					Parts[PartsNum+(WinXMax/TipSizeX)].X,
					Parts[PartsNum+(WinXMax/TipSizeX)].Y+PartsLackY,
					Parts[PartsNum+(WinXMax/TipSizeX)].X+PartsLackX,
					Parts[PartsNum+(WinXMax/TipSizeX)].Y+TipSizeY);
		
			lpDDSBack->BltFast(	(WinXMax/TipSizeX)*TipSizeX-PartsLackX,0,
								lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);

			//�`�悷��BG�̈ʒu��i�߂�
			PartsNum +=MapSizeX;
	
			//���i�̕`��
			for(i=1;(DWORD)i<WinYMax/TipSizeY;i++)
			{
				//���[
				SetRect(&rc,
						Parts[PartsNum].X+PartsLackX,
						Parts[PartsNum].Y,
						Parts[PartsNum].X+TipSizeX,
						Parts[PartsNum].Y+TipSizeY);
		
				lpDDSBack->BltFast(	0,i*TipSizeY-PartsLackY,
									lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		
				//���i
				for(int j=1;(DWORD)j<WinXMax/TipSizeX;j++)
				{
					SetRect(&rc,
							Parts[PartsNum+j].X,
							Parts[PartsNum+j].Y,
							Parts[PartsNum+j].X+TipSizeX,
							Parts[PartsNum+j].Y+TipSizeY);
					lpDDSBack->BltFast(	j*TipSizeX-PartsLackX,i*TipSizeY-PartsLackY,
										lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
				}
		
				//�E�[
				SetRect(&rc,
						Parts[PartsNum+(WinXMax/TipSizeX)].X,
						Parts[PartsNum+(WinXMax/TipSizeX)].Y,
						Parts[PartsNum+(WinXMax/TipSizeX)].X+PartsLackX,
						Parts[PartsNum+(WinXMax/TipSizeX)].Y+TipSizeY);
		
				lpDDSBack->BltFast(	(WinXMax/TipSizeX)*TipSizeX-PartsLackX,i*TipSizeY-PartsLackY,
									lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
				//�`�悷��ʒu��i�߂�
				PartsNum +=MapSizeX;
			}

			//�ŉ��i���[�̕`��
			SetRect(&rc,
					Parts[PartsNum].X+PartsLackX,
					Parts[PartsNum].Y,
					Parts[PartsNum].X+TipSizeX,
					Parts[PartsNum].Y+PartsLackY);
	
			lpDDSBack->BltFast(	0,WinYMax-PartsLackY,
								lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);

			//�ŉ��i���i�̕`��
			for(i=1;i<(int)(WinXMax/TipSizeX);i++)
			{
				SetRect(&rc,
						Parts[PartsNum+i].X,
						Parts[PartsNum+i].Y,
						Parts[PartsNum+i].X+TipSizeX,
						Parts[PartsNum+i].Y+PartsLackY);
		
				lpDDSBack->BltFast(	i*TipSizeX-PartsLackX,WinYMax-PartsLackY,
							lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			}

			//�ŉ��i�E�[�̕`��
			SetRect(&rc,
					Parts[PartsNum+(WinXMax/TipSizeX)].X,
					Parts[PartsNum+(WinXMax/TipSizeX)].Y,
					Parts[PartsNum+(WinXMax/TipSizeX)].X+PartsLackX,
					Parts[PartsNum+(WinXMax/TipSizeX)].Y+PartsLackY);
	
			lpDDSBack->BltFast(	(WinXMax/TipSizeX)*TipSizeX-PartsLackX,WinYMax-PartsLackY,
								lpDDS_BG,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			
			//�X�e�[�W1�I�[�v�j���O�����\��
			if(fBG&_SetBG00_ON){
				lpDDSBack->BltFast(165,145,lpDDS_BG2,NULL,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			}
			break;
		//�Q�[���I�[�o�[��ʂ̕`��
		case 1000:
			lpDDSBack->BltFast(	225,224,
								lpDDS_BG,NULL,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			break;
		//�Q�[���N���A��ʂ̕`��
		case 2000:
			lpDDSBack->BltFast(	0,0,
								lpDDS_BG,NULL,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////
// ���݂�fBG�t���O��Ԃ�
// �߂�l:�X�e�[�W�ԍ�
//////////////////////////////////////////////////////////////////////
DWORD	DrawBG::GetfBG()
{
	return	fBG;
}

//////////////////////////////////////////////////////////////////////
// �����œn���ꂽ�t���O���Z�b�g����
// ����:BG�t���O,set=0:AND set=1:OR �ŃZ�b�g����
//////////////////////////////////////////////////////////////////////
void	DrawBG::SetfBG(DWORD flag,int set)
{
	switch(set){
		case 0:
			fBG &=flag;
			break;
		case 1:
			fBG |=flag;
			break;
		default:
			break;
	}
}