/*=============================================================================
	���@�̃��C�������Ɋւ���\�[�X�t�@�C��
==============================================================================*/
#include <windows.h>
#include <ddraw.h>
#include <dsound.h>

#include "Macros.h"
#include "Draw.h"
#include "Struct.h"
#include "DrawJIKI.h"
#include "DrawJIKIData.h"
#include "DrawMacros.h"

//�_�C���N�g�T�E���h�o�b�t�@�Đ��p�}�N����`
#define _JIKIBAKU 0
#define _TEKIBAKU 1
#define _FUELARM 2
#define _FIRE 3
#define _TAMAKYU 4
#define _BOSSBAKU 5

extern LPDIRECTSOUNDBUFFER lpDSBuf[6]; //�_�C���N�g�T�E���h�o�b�t�@
extern LPDIRECTDRAWSURFACE7 lpDDSBack; //�o�b�N�T�[�t�F�X

//�W���C�X�e�B�N�֘A
BYTE JoyNew = 0, JoyOld = 0;
BYTE Joy_Get(void);

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

DrawJIKI::DrawJIKI()
{
	InitJikiTm();
	InitWepnTm();
	InitHmmTm();
	InitHnbTm();
	InitOptAtkDatTm();
}

DrawJIKI::~DrawJIKI()
{
	OptRelease();
	HnbRelease();
	HmmRelease();
	MainWepnRelease();
	JikiTmRelease();
}

/////////////////////////////////////////////////////////////////////
// ���@�̊e�p�����[�^�[��������
/////////////////////////////////////////////////////////////////////
BOOL DrawJIKI::InitJIKI()
{
	//�ď������ɕK�v�ȏ���
	OptRelease();
	HnbRelease();
	HmmRelease();
	MainWepnRelease();
	JikiTmRelease();

	BOOL ret = TRUE;

	//���@�Ɋւ���l�X�ȃI�u�W�F�N�g�̏�����
	ret = InitJikiData();
	if (!ret)
		return FALSE;

	//��C���甭�˂����e�I�u�W�F�N�g�̏�����
	ret = InitMainWepn();
	if (!ret)
		return FALSE;

	//�z�[�~���O�~�T�C���I�u�W�F�N�g�̏�����
	ret = InitHmm();
	if (!ret)
		return FALSE;

	//�ԉ΃I�u�W�F�N�g�̏�����
	ret = InitHnb();
	if (!ret)
		return FALSE;

	//�I�v�V�����I�u�W�F�N�g�̏�����
	ret = InitOpt();
	if (!ret)
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// ���@�̈ړ��ƒe���˂܂ł̃J�E���g�_�E��
/////////////////////////////////////////////////////////////////////
void DrawJIKI::MoveJIKI()
{
	//�X�e�[�W�t���O�ɉ����t���O�������Ă���ꍇ���^�[��
	if (fSTAGE & _fSTAGE_CHK)
	{
		lpDSBuf[_TAMAKYU]->Stop(); //���[�v���Ă���e�z�����̒�~
		return;
	}
	//�Q�[�����ꎞ��~������t���O�������Ă���ꍇ���^�[��
	if (fJIKI & _GAME_STOP_ON)
	{
		lpDSBuf[_TAMAKYU]->Stop(); //���[�v���Ă���e�z�����̒�~
		return;
	}

	//���@���ăt���O�������Ă���ꍇ�͈ړ��������s��Ȃ�
	if (!(fJIKI & _END_ON))
	{
		//�W���C�X�e�B�b�Nor�L�[�|�[�h����̓��͂��擾
		JoyOld = JoyNew;
		JoyNew = Joy_Get();

		//���@�̈ړ�
		if (JoyNew & JOY_UP)
		{					//�オ������Ă���
			Cdn.y -= Mov.y; //�ړ��ʂ�������
		}
		else if (JoyNew & JOY_DOWN)
		{ //����������Ă���
			Cdn.y += Mov.y;
		}
		if (JoyNew & JOY_RIGHT)
		{ //�E��������Ă���
			Cdn.x += Mov.x;
		}
		else if (JoyNew & JOY_LEFT)
		{ //����������Ă���
			Cdn.x -= Mov.x;
		}

		//�e�z�����łȂ�����A�������ꂽ��e��U���t���O���I��
		if (JoyNew & JOY_A && !(fJIKI & _FUEL_ON))
		{
			MainWepnCnt(); //��C���˂܂ł̃J�E���g��i�߂�
			switch (Wpd.flag & _SUBWP_CHK)
			{ //���ݑI������Ă���T�u�E�F�|���̔��˃J�E���g��i�߂�
			case _HMM_ON:
				HmmCnt(); //�z�[�~���O�~�T�C��
				break;
			case _HNB_ON:
				HnbCnt(); //�ԉ�
				break;
			default:
				break;
			}
			if (fJIKI & _OPT_ON)
				OptAttackCnt(); //�I�v�V�����̒e���˂̃J�E���g��i�߂�
			// A�������ꂽ�u�Ԃł������ꍇ�A�[�����˃t���O���I��
			if ((JoyNew ^ JoyOld) & (JoyNew & JOY_A))
				fJIKI |= _ARM_ON;
		} // A�������ꂽ�u�Ԋe�U���p�J�E���^�����Z�b�g
		else if ((JoyNew ^ JoyOld) & (JoyOld & JOY_A))
		{
			//�e����ʂɑ��݂��Ȃ��Ȃ�܂�_WEPN_ON�t���O�͉������Ȃ�
			MainWepnCntReset(); //��C���˂܂ł̃J�E���g���Z�b�g
			switch (Wpd.flag & _SUBWP_CHK)
			{ //���ݑI������Ă���T�u�E�F�|���̔��˃J�E���g�����Z�b�g
			case _HMM_ON:
				HmmCntReset(); //�z�[�~���O�~�T�C��
				break;
			case _HNB_ON:
				HnbCntReset(); //�ԉ�
			default:
				break;
			}
			if (fJIKI & _OPT_ON)
				OptAttackCntReset(); //�I�v�V�����̒e���˂̃J�E���g�����Z�b�g
		}
		//�e�z���R�������鎞��B�������ꂽ�u��
		if ((JoyNew ^ JoyOld) & (JoyNew & JOY_B) && FUEL_R)
		{
			fJIKI |= _FUEL_ON; //�e�z�����t���O���I��
			//�e�z���T�E���h
			lpDSBuf[_TAMAKYU]->Play(0, 0, DSBPLAY_LOOPING);
			//�e�z�����t���O�������Ă��鎞��B����������ςȂ��ł���
		}
		else if (JoyNew & JoyOld & JOY_B && fJIKI & _FUEL_ON)
		{
			if (0 > FUEL_R - _FUELKY)
			{							   // FUEL�l��0�ȉ��Ȃ��
				FUEL_R = 0;				   // FUEL�l0
				fJIKI &= _FUEL_OFF;		   //�e�z�����t���O�I�t
				fJIKI |= _FUELD_ON;		   //�e�z�����f�t���O�I��
				lpDSBuf[_TAMAKYU]->Stop(); //���[�v���Ă���e�z�����̒�~
			}
			else
			{
				FUEL_R -= _FUELKY; // FUEL�l��0�łȂ��Ȃ�B�������Ă����FUEL�Q�[�W�����炷
			}
			FUEL = (BYTE)(FUEL_R >> 16); // FUEL�Ɍ��݂̔R���̒l���Z�b�g
			//�e�z�����t���O�������Ă��鎞��B�������ꂽ
		}
		else if ((JoyNew ^ JoyOld) & (JoyOld & JOY_B) && fJIKI & _FUEL_ON)
		{
			fJIKI &= _FUEL_OFF;		   //�e�z�����t���O�I�t
			fJIKI |= _FUELD_ON;		   //�e�z�����f�t���O�I��
			lpDSBuf[_TAMAKYU]->Stop(); //���[�v���Ă���e�z�����̒�~
		}

		//�I�v�V�����̈ړ�
		if (fJIKI & _OPT_ON)
		{
			OptMovCnt(JoyNew); //�I�v�V�����̈ړ��p�Ɍ��݂̎��@�̍��W���擾
			OptMov();		   //�I�v�V�����̈ړ�
		}
	}
	else
	{
		lpDSBuf[_TAMAKYU]->Stop(); //���[�v���Ă���e�z�����̒�~
	}

	//���@��ʍ��W�̕␳
	if (Cdn.x < 0)
		Cdn.x = 0;
	else if (Cdn.x + _JIKI_W > WinXMax)
		Cdn.x = WinXMax - _JIKI_W;
	if (Cdn.y < 0)
		Cdn.y = 0;
	else if (Cdn.y + 32 > WinYMax)
		Cdn.y = WinYMax - 32;

	//���݂̐�΍��W�����߂�
	MapCdn.x = MainCdn.x + Cdn.x;
	MapCdn.y = MainCdn.y + Cdn.y;
}

///////////////////////////////////////////////////////////////////////////////////
// �G�L�����A�e�Ƃ̂����蔻��𒲂ׂĐڐG���Ă���ꍇ�͓G�A���@�����̃t���O���Z�b�g
// �����ł̖߂�l�͓G�L�����̏�Ԃ�ύX�����܂��B
// �G�e�͎��@�ɐڐG�����ꍇ�A�G�e��ԃt���O�͂����ŕύX���Ă��܂��B
////////////////////////////////////////////////////////////////////////////////////
DWORD DrawJIKI::AtariJIKI()
{
	//�X�e�[�W�t���O�ɉ����t���O�������Ă���ꍇ���^�[��
	if (fSTAGE & _fSTAGE_CHK)
		return SP_fSPChr;
	//���@��ԃt���O�ɃQ�[���ꎞ��~�t���O�������Ă���ꍇ���^�[��
	if (fJIKI & _GAME_STOP_ON)
		return SP_fSPChr;
	//���@�Ɍ��ăt���O�̗����Ă���ꍇ�͓����蔻����s��Ȃ�
	if (fJIKI & _END_ON)
		return SP_fSPChr;

	//�w�肳�ꂽ�G�L�������j�󂳂�Ă���ꍇ�Q�[���I��
	if (SP_Num == Target)
	{
		if ((SP_fSPChr & _SPDESCHK) == (DWORD)_SPDES)
		{
			fJIKI &= _BOSS_OFF;			 //�{�X�Ɛ퓬���t���O�I�t
			fSTAGE |= _STAGE_ENDWAIT_ON; //�X�e�[�W�I���t���O�I��
			fSTAGE |= _GAME_CLEAR_ON;	 //�X�e�[�W�N���A�t���O�I��
			return SP_fSPChr;
		}
	}

	//��ʂɒe�����݂��Ă���ꍇ�A���@�ƓG�̒e�Ƃ̓����蔻����s��
	JikiTekitamaAtari();
	if (fJIKI & _END_ON)
		return SP_fSPChr; //���@�����Ă��ꂽ�ꍇ���^�[��

	//�G�L�����̕\���t���O�������Ă��Ȃ��ꍇ�̓��^�[��
	if (!(SP_fSPChr & _SPON))
		return SP_fSPChr;
	else if (SP_fSPChr & _SPDES)
		return SP_fSPChr;
	//���@�t���O�ɓG�L�������ł������Ă���ꍇ��ʏ�̓G�L�����Ɉ��̃_���[�W
	else if (fJIKI & _PWUPD_ON)
	{
		JikiTekiDmg(_PWUPDDMG);
		//�G�L�����̔j��t���O�������Ă���ꍇ�̓��^�[��
		if (SP_fSPChr & _SPDES)
			return SP_fSPChr;
	}

	//�{�X�L��������ʂɏo�����Ă��邩�ǂ����`�F�b�N
	if (SP_Num == Target)
	{
		if (!(fJIKI & _TARGET_ON) && !(fJIKI & _BOSS_ON))
		{
			fJIKI |= _TARGET_ON; //�{�X�L�����o���t���O�I��
			fJIKI |= _BOSS_ON;	 //�{�X��ɓ˓��������Ƃ������t���O�I��
		}
		else if (fJIKI & _TARGET_ON && fJIKI & _BOSS_ON)
		{
			fJIKI &= _TARGET_OFF; //�{�X�L�����o���t���O�I�t
		}
	}

	//�z�[�~���O�~�T�C�����˗p�Ƀ��b�N�I�����邽�߂ɕK�v�ȓG�L�����̏����擾����
	HmmGetTekiInfo();

	//�G�L�����Ǝ�C�̒e�Ƃ̓����蔻��
	if (fJIKI & _WEPN_ON)
	{
		MainWepnAtari();
		if (SP_fSPChr & _SPDES)
			return SP_fSPChr;
	}
	//�G�L�����ƃz�[�~���O�~�T�C���Ƃ̓����蔻��ƒǔ��p�̈ړ��ʂ̎擾
	if (fJIKI & _SUBHMM_ON)
	{
		HmmMovCnt(); //�G�L�����ǔ��p�̈ړ��ʂ̎擾
		HmmAtari();	 //�G�L�����Ƃ̓����蔻��
		if (SP_fSPChr & _SPDES)
			return SP_fSPChr;
	}
	//�G�L�����Ɖԉ΂Ƃ̓����蔻��
	if (fJIKI & _SUBHNB_ON)
	{
		HnbAtari();
		if (SP_fSPChr & _SPDES)
			return SP_fSPChr;
	}
	//�G�L�����ƃI�v�V�����̔��˂����e�Ƃ̓����蔻��
	if (fJIKI & _OPTWEP_ON)
	{
		OptAttackAtari();
		if (SP_fSPChr & _SPDES)
			return SP_fSPChr;
	}

	//�G�L�����Ǝ��@�Ƃ̓����蔻��
	JikiTekiAtari();

	return SP_fSPChr;
}

//////////////////////////////////////////////////////////////////////
// ���@�Ɋւ���O���t�B�b�N�̕`��Ɏg�p����e�l���X�V����
//////////////////////////////////////////////////////////////////////
void DrawJIKI::FrameJIKI()
{
	//���@�̏�ԃt���O���`�F�b�N���Ď��@�̋�`�͈͂��Z�b�g

	//--------�X�e�[�W�t���O�ɉ����t���O�������Ă���ꍇ���^�[��-----
	if (fSTAGE & _fSTAGE_CHK)
		return;
	//------�Q�[�����ꎞ��~������t���O�������Ă���ꍇ���^�[��-----
	if (fJIKI & _GAME_STOP_ON)
		return;

	//---------���@�����Ă���Ă���ꍇ�����V�[���̏���--------------
	if (fJIKI & _END_ON)
	{
		JikiBakuFrame();
		return;
	}
	//---------�ʏ��Ԃ̎��@�̋�`�͈͂��Z�b�g ---------------------
	if (!fJIKI || !(fJIKI & 0x0000001F))
	{
		JIKIChr = SpJIKI[0];
	}

	//----------�A�[�����˒��̃A�j���[�V��������---------------------
	if (fJIKI & _ARM_ON)
		JikiArmFrame();

	//----------��C�̒e�̔��ˁA�ړ��A�A�j���[�V�������� ------------
	MainWepn();

	//----�z�[�~���O�~�T�C���̒e�̔��ˁA�ړ��A�A�j���[�V��������-----
	HmmAttack();

	//----------�ԉ΂̔��ˁA�ړ��A�A�j���[�V��������-----------------
	HnbAttack();

	//----------�I�v�V�����̒e�̔��ˁA�ړ��A�A�j���[�V�������� ------
	if (fJIKI & _OPT_ON)
		OptAttack();

	//----------�e�z�����̎��@�A�j���[�V��������---------------------
	JikiTmkyFrame();

	// FUEL�Q�[�W�̃T�[�t�F�X����̓]����`�͈͂����݂�FUEL�̃T�C�Y�Ɏw��
	GAGE_F.left = SpJIKI[22].left + (128 - FUEL);
	GAGE_F.top = SpJIKI[22].top;
	GAGE_F.right = SpJIKI[22].right;
	GAGE_F.bottom = SpJIKI[22].bottom;

	// EXP�Q�[�W�̃T�[�t�F�X����̓]����`�͈͂����݂�EXP�̃T�C�Y�Ɏw��
	GAGE_E.left = SpJIKI[19].left;
	GAGE_E.top = SpJIKI[19].top;
	GAGE_E.right = SpJIKI[19].right - (128 - EXP);
	GAGE_E.bottom = SpJIKI[19].bottom;

	//�X�R�A�\���̂��߂̋�`�͈͂�ݒ�
	ScoreCalcul();
}

//////////////////////////////////////////////////////////////////////
// ���@�Ɗe���ʃX�e�[�^�X�̕`��
//////////////////////////////////////////////////////////////////////
void DrawJIKI::RendJIKI()
{
	//�X�e�[�W�t���O�ɉ��������Ă���ꍇ�̓��^�[��
	if (fSTAGE & _fSTAGE_CHK)
		return;

	//--���@�����Ă��ꔚ������A�j���[�V�����ƃR���e�B�j���[��ʂ̕`��--
	if (fJIKI & _END_ON)
	{
		//�R���e�B�j���[���
		if (fJIKI & _GAME_STOP_ON)
		{
			RendCONTINUE();
		}
		else if (fJIKI & _CONTINUE_ON)
		{
			fJIKI &= _CONTINUE_OFF; //�R���e�B�j���[�ɔ��������������I��
			fJIKI &= _END_OFF;		//���@�̌��ăt���O�I�t
			//�����V�[��
		}
		else
		{
			lpDDSBack->BltFast(Baku.DesR.left, Baku.DesR.top,
							   lpDDS_JIKI, &Baku.SrcR,
							   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		}
	}
	else
	{
		//------------- �I�v�V��������������Ă��鎞�̓]�� -------------
		if (fJIKI & _OPT_ON)
			OptRend();

		//���@�����G��Ԃ̎��ɓ_�ł����鏈��
		if (fJIKI & _INVINCIBLE_ON && !(fJIKI & _GAME_STOP_ON))
		{
			if (!(InvincCnt--))
			{ //���G���Ԃ����炷
				InvincCnt = 0;
				fJIKI &= _INVINCIBLE_OFF;
			}
		}

		if ((InvincCnt % 6) < 3)
		{
			//------------------ �ʏ�̏�Ԃ̓]�� -------------------------
			if (!fJIKI || !(fJIKI & 0x0000001F))
			{
				lpDDSBack->BltFast(Cdn.x, Cdn.y,
								   lpDDS_JIKI, &JIKIChr,
								   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			}
			//------------------ �A�[�����˒��̓]��-------------------------
			else if (fJIKI & _ARM_ON)
			{
				lpDDSBack->BltFast(ArmJIKI.DesR.left, ArmJIKI.DesR.top,
								   lpDDS_JIKI, &ArmJIKI.SrcR,
								   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
				lpDDSBack->BltFast(Arm.DesR.left, Arm.DesR.top,
								   lpDDS_JIKI, &Arm.SrcR,
								   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			}
			//--------------------- �e�z�����̓]�� -------------------------
			if (fJIKI & _FUEL_ON)
			{
				lpDDSBack->BltFast(TmKySy.DesR.left, TmKySy.DesR.top,
								   lpDDS_JIKI, &TmKySy.SrcR,
								   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			}
		}

		//-------------- ��ʂɒe�����݂��Ă��鎞�̓]�� ----------------
		if (fJIKI & _WEPN_ON)
			MainWepnRend(); //��C�̒e�̕`��
		if (fJIKI & _SUBHMM_ON)
			HmmRend(); //�z�[�~���O�~�T�C���̕`��
		if (fJIKI & _SUBHNB_ON)
			HnbRend(); //�ԉ΂̕`�揈��
		if (fJIKI & _OPTWEP_ON)
			OptWepRend(); //�I�v�V�����̔��˂����e�̕`��
	}

	// FUEL�Q�[�W�̋󔒕����̓]��
	lpDDSBack->BltFast(506, 0,
					   lpDDS_JIKI, &GAGE_FK,
					   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	// FUEL�Q�[�W�̕����̓]��
	lpDDSBack->BltFast(506 + (128 - FUEL), 0,
					   lpDDS_JIKI, &GAGE_F,
					   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	// FUEL�Q�[�W�̘g�����̓]��
	lpDDSBack->BltFast(512, 0,
					   lpDDS_JIKI, &GAGE_FW,
					   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	// EXP�Q�[�W�̋󔒕����̓]��
	lpDDSBack->BltFast(6, 0,
					   lpDDS_JIKI, &GAGE_EK,
					   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	// EXP�Q�[�W�̕����̓]��
	lpDDSBack->BltFast(6, 0,
					   lpDDS_JIKI, &GAGE_E,
					   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	// EXP�Q�[�W�̘g�����̓]��
	lpDDSBack->BltFast(0, 0,
					   lpDDS_JIKI, &GAGE_EW,
					   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	//�X�R�A�����̓]��
	for (int i = 0, j = 248; i < 9; i++, j += 16)
	{
		lpDDSBack->BltFast(j, 2,
						   lpDDS_JIKI, &SCORE_R[i],
						   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}

	/*--- �p���[�A�b�v��ʏ������������͓G�L�����S�Ńt���O�̗����Ă���ꍇ
	�p���[�A�b�v�����֐��ɔ��--------------------------------------*/
	if (fJIKI & _PWUP_ON || fJIKI & _PWUPD_ON)
	{
		RendPWUP(); //���@�̃p���[�A�b�v������ʂ�`�悷��
	}
}

//////////////////////////////////////////////////////////////////////
// ���@�̏�ԃt���O�̒l��߂�l�Ƃ��ĕԂ��֐�
//////////////////////////////////////////////////////////////////////
DWORD DrawJIKI::GetfJIKI()
{
	return fJIKI;
}

//////////////////////////////////////////////////////////////////////
// ���@�̉�ʍ��W�̒l��߂�l�Ƃ��ĕԂ��֐�
//////////////////////////////////////////////////////////////////////
POINT DrawJIKI::GetJIKICdn()
{
	return Cdn;
}
