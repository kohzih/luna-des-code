/*====================================================================
	�e�X�e�[�W�f�[�^�̏������Ɋւ���\�[�X�t�@�C��
=====================================================================*/

#include <windows.h>
#include <ddraw.h>
#include <dmusici.h>

#include "Macros.h"
#include "FaInOut.h"
#include "Draw.h"
#include "DrawBG.h"
#include "Struct.h"
#include "DrawSP.h"
#include "DrawJIKI.h"
#include "DrawMacros.h"

extern HWND hWnd; //���C���E�B���h�E�n���h��

extern IDirectMusicPerformance *lpDMPerf;
extern IDirectMusicComposer *lpDMComposer;
//�Ȃ��Ǘ�����}�N��
#define _TITLE 0
#define _STAGE1 1
#define _CONTINUE 2
#define _BOSS 3
#define _ENDING 4

extern IDirectMusicSegment *lpDMSegment[5];		  //�Z�O�����g
extern IDirectMusicSegmentState *lpDMSegState[5]; //�Z�O�����g�̏��
extern MUSIC_TIME SegStartTime[5];

extern FaInOut *lpFInOut;
extern Draw *lpDraw;
extern DrawSP *lpSP;
extern DrawJIKI *lpJIKI;
extern DrawBG *lpBG;

//�W���C�X�e�B�N�֘A
extern BYTE JoyNew, JoyOld;
BYTE Joy_Get(void);

#if DEBUG >= 2
extern LPDIRECTDRAW7 lpDD;		// DirectDraw�I�u�W�F�N�g
extern DWORD TotalMem, FreeMem; // VRAM�̋󂫗e�ʂ̒l���i�[
#endif

//////////////////////////////////////////////////////////////////////
// �e�X�e�[�W�̊J�n�A�I���A�C�x���g���̏��������s�Ȃ�
//////////////////////////////////////////////////////////////////////
BOOL InitSTAGE()
{

	int i, j;
	long Vol = 0; // BGM�̃}�X�^�[�{�����[���̑����A�������x���̒l
	BOOL ret = TRUE;
	static int WaitCnt = 0;

	//------------------------------------------------------------------//
	//				�X�e�[�W�I�������̏���								//
	//------------------------------------------------------------------//
	if (lpDraw->GetfSTAGE() & _STAGE_ENDWAIT_ON)
	{
		lpDraw->SetfSTAGE(_STAGE_ENDWAIT_OFF, 0); //�X�e�[�W�I�����}�t���O�I�t
		lpDraw->SetfSTAGE(_STAGE_END_ON, 1);	  //�X�e�[�W�I���������t���O�I��

		switch (lpDraw->GetStageNum())
		{
		case 0: //�v�����[�O���
			break;

		case 1: //�^�C�g�����
				// BGM��~
			lpDMPerf->Stop(lpDMSegment[_TITLE], lpDMSegState[_TITLE], 0, DMUS_SEGF_DEFAULT);
			break;

		case 2: //�X�e�[�W1
			//�Q�[���I�[�o�[�ɂȂ����ꍇ
			if (lpDraw->GetfSTAGE() & _GAME_OVER_ON)
			{
				lpFInOut->SetFade(_SetDef); //���̐F�ɖ߂�
				//�Q�[�����N���A���Ă���ꍇ
			}
			else if (lpDraw->GetfSTAGE() & _GAME_CLEAR_ON)
			{
				// EndingBGM�Đ��J�n
				lpDMSegment[_ENDING]->SetStartPoint(0);
				lpDMComposer->AutoTransition(
					lpDMPerf, lpDMSegment[_ENDING], DMUS_COMMANDT_END, DMUS_SEGF_DEFAULT,
					NULL, NULL, &lpDMSegState[_ENDING], NULL);
			}
			break;

		case 1000: //�Q�[���I�[�o�[���
			// BGM��~
			lpDMPerf->Stop(lpDMSegment[_CONTINUE], lpDMSegState[_CONTINUE], 0, DMUS_SEGF_DEFAULT);
			break;

		case 2000: //�Q�[���N���A���
			// BGM��~
			lpDMPerf->Stop(lpDMSegment[_ENDING], lpDMSegState[_ENDING], 0, DMUS_SEGF_DEFAULT);
			break;

		default:
			return FALSE;
			break;
		}
		return TRUE;
	}

	//------------------------------------------------------------------//
	//					�X�e�[�W�̏I������								//
	//------------------------------------------------------------------//
	if (lpDraw->GetfSTAGE() & _STAGE_END_ON)
	{
		switch (lpDraw->GetStageNum())
		{
		case 0: //�v�����[�O���
			ret = lpFInOut->BlackOut(0x00020000);
			//				ret=lpFInOut->BlackOut(0x01020000);
			//�u���b�N�A�E�g���I�������ꍇ�A�X�e�[�W�I���������t���O�I�t
			if (ret)
			{
				lpDraw->SetfSTAGE(_STAGE_END_OFF, 0);
				lpDraw->SetfSTAGE(_STAGE_STARTWAIT_ON, 1); //�X�e�[�W�J�n�t���O�I��
				lpBG->SetfBG(0, 0);						   // BG�t���O�N���A
				lpDraw->SetStageNum(1);					   //�X�e�[�W�ԍ���i�߂鏈��
			}
			else
			{
				return TRUE;
			}
			break;

		case 1: //�^�C�g�����
			ret = lpFInOut->BlackOut(0x00020000);
			//				ret=lpFInOut->BlackOut(0x01020000);
			//�u���b�N�A�E�g���I�������ꍇ�A�X�e�[�W�I���������t���O�I�t
			if (ret)
			{
				lpDraw->SetfSTAGE(_STAGE_END_OFF, 0);
				lpDraw->SetfSTAGE(_STAGE_STARTWAIT_ON, 1); //�X�e�[�W�J�n�t���O�I��
				lpBG->SetfBG(0, 0);						   // BG�t���O�N���A
				lpDraw->SetStageNum(2);
			}
			else
			{
				return TRUE;
			}
			break;

		case 2: //�X�e�[�W1
			//�Q�[���I�[�o�[�ŏI�������ꍇ
			if (lpDraw->GetfSTAGE() & _GAME_OVER_ON)
			{
				ret = lpFInOut->BlackOut(0x00050000);
				//					ret=lpFInOut->BlackOut(0x01050000);
				if (ret)
				{
					lpDraw->SetfSTAGE(_STAGE_END_OFF, 0);
					lpDraw->SetfSTAGE(_STAGE_STARTWAIT_ON, 1); //�X�e�[�W�J�n�t���O�I��
					lpBG->SetfBG(0, 0);						   // BG�t���O�N���A
					lpDraw->SetStageNum(1000);
				}
				else
				{
					return TRUE;
				}
				//�Q�[���N���A�ŏI�������ꍇ
			}
			else if (lpDraw->GetfSTAGE() & _GAME_CLEAR_ON)
			{
				if (WaitCnt++ > 200)
				{
					//					if(WaitCnt++>2){
					ret = lpFInOut->WhiteOut(0x00005000);
					//						ret=lpFInOut->WhiteOut(0x01005000);
					if (ret)
					{
						lpDraw->SetfSTAGE(_STAGE_END_OFF, 0);
						lpDraw->SetfSTAGE(_STAGE_STARTWAIT_ON, 1); //�X�e�[�W�J�n�t���O�I��
						lpBG->SetfBG(0, 0);
						lpDraw->SetStageNum(2000);
						WaitCnt = 0;
					}
					else
					{
						return TRUE;
					}
				}
			}
			break;

		case 1000: //�Q�[���I�[�o�[���
			ret = lpFInOut->BlackOut(0x00050000);
			//				ret=lpFInOut->BlackOut(0x01050000);
			//�u���b�N�A�E�g���I�������ꍇ�A�X�e�[�W�I���������t���O�I�t
			if (ret)
			{
				lpDraw->SetfSTAGE(_GAME_OVER_OFF, 0); //�Q�[���I�[�o�[�t���O�I�t
				lpDraw->SetfSTAGE(_STAGE_END_OFF, 0);
				lpDraw->SetfSTAGE(_STAGE_STARTWAIT_ON, 1); //�X�e�[�W�J�n�t���O�I��
				lpBG->SetfBG(0, 0);						   // BG�t���O�N���A
				lpDraw->SetStageNum(1);					   //�X�e�[�W�ԍ���i�߂鏈��
			}
			else
			{
				return TRUE;
			}
			break;

		case 2000: //�Q�[���N���A���
			ret = lpFInOut->BlackOut(0x00050000);
			//				ret=lpFInOut->BlackOut(0x01050000);
			//�u���b�N�A�E�g���I�������ꍇ�A�X�e�[�W�I���������t���O�I�t
			if (ret)
			{
				lpDraw->SetfSTAGE(_GAME_CLEAR_OFF, 0); //�Q�[���N���A�t���O�I�t
				lpDraw->SetfSTAGE(_STAGE_END_OFF, 0);
				lpDraw->SetfSTAGE(_STAGE_STARTWAIT_ON, 1); //�X�e�[�W�J�n�t���O�I��
				lpBG->SetfBG(0, 0);						   // BG�t���O�N���A
				lpDraw->SetStageNum(1);					   //�X�e�[�W�ԍ���i�߂鏈��
			}
			else
			{
				return TRUE;
			}
			break;

		default:
			return FALSE;
			break;
		}
	}

	//------------------------------------------------------------------//
	//				�X�e�[�W�J�n�����̏���								//
	//------------------------------------------------------------------//

	if (lpDraw->GetfSTAGE() & _STAGE_STARTWAIT_ON)
	{
		lpDraw->SetfSTAGE(_STAGE_STARTWAIT_OFF, 0); //�X�e�[�W�J�n���}�t���O�I�t
		lpDraw->SetfSTAGE(_STAGE_START_ON, 1);		//�X�e�[�W�J�n�������t���O�I��

		//���C���̃Q�[����������؍s��Ȃ����Ƃ������t���O�I��
		lpDraw->SetfSTAGE(_GAME_JUMP_ON, 1);
		// Draw�I�u�W�F�N�g��������
		ret = lpDraw->Init();
		if (!ret)
			return FALSE;
		//�w�i�I�u�W�F�N�g��������
		ret = lpBG->InitBG();
		if (!ret)
			return FALSE;

		DWORD MsX = 0, MsY = 0, CsX = 0, CsY = 0; //�}�b�v�f�[�^�i�[�p
		WORD Event = 0;							  //�C�x���g�f�[�^�i�[�p

		//�Q�[���J�n���̏���
		switch (lpDraw->GetStageNum())
		{
		case 0: //�v�����[�O���
			//�u���b�N�C������
			lpFInOut->SetFade(_SetBlackIn);
			break;

		case 1: //�^�C�g�����
			lpFInOut->SetFade(_SetBlackIn);
			// BGM�Đ��J�n
			lpDMSegment[_TITLE]->SetStartPoint(0);
			lpDMPerf->PlaySegment(lpDMSegment[_TITLE], DMUS_SEGF_DEFAULT, 0, &lpDMSegState[_TITLE]);
			break;

		case 2: //�X�e�[�W1
			lpFInOut->SetFade(_SetBlackIn);
			// BGM�Đ��J�n
			lpDMSegment[_STAGE1]->SetStartPoint(0);
			lpDMPerf->PlaySegment(lpDMSegment[_STAGE1], DMUS_SEGF_DEFAULT, 0, &lpDMSegState[_STAGE1]);

			//���C���̃Q�[����������؍s��Ȃ����Ƃ������t���O�I�t
			lpDraw->SetfSTAGE(_GAME_JUMP_OFF, 0);
			//�X�e�[�W1�^�C�g�������\��
			lpBG->SetfBG(_SetBG00_ON, 1);
			//���@�I�u�W�F�N�g��������
			ret = lpJIKI->InitJIKI();
			if (!ret)
				return FALSE;

			//----------�X�v���C�g�I�u�W�F�N�g��������-------------//

			//�}�b�v�̏c���̑傫���ƃ`�b�v�T�C�Y�����߂�
			lpDraw->GetMapSizeData(MsX, MsY, CsX, CsY);

			//�m�ۂ���X�v���C�g�I�u�W�F�N�g�̐������߂�
			// Parts�\���̔z����̃C�x���g�̐���ۑ�
			for (i = 0; (DWORD)i < MsX * MsY; i++)
			{
				lpDraw->GetEventData(i, Event);
				if (Event)
					lpDraw->EventCnt++;
			}

			//�C�x���g�̐������X�v���C�g�Ǘ��I�u�W�F�N�g�̍쐬(�C�x���g����0�̏ꍇ�͏������Ȃ�)
			if (lpDraw->EventCnt == 0)
			{
				RELEASE_dels(lpSP);
			}
			else
			{
				lpSP = new DrawSP[lpDraw->EventCnt];
				if (!lpSP)
					return FALSE;

				// BGMAP�\���̔z��(Parts)������C�x���g�����������ׂāA
				//�C�x���g�̔z�u����Ă���ʒu�ŃI�u�W�F�N�g��������

				POINT cdn; //�C�x���g�̍��W�p

				for (i = 0, j = 0; (DWORD)i < MsX * MsY; i++)
				{
					lpDraw->GetEventData(i, Event);
					if (Event)
					{
						//�p�[�c�̔ԍ������΍��W�̂ǂ��ŃC�x���g���N�����Ă���̂��v�Z
						cdn.x = (i % MsX) * CsX;
						cdn.y = (i / MsX) * CsY;

						//�X�v���C�g�I�u�W�F�N�g��������
						ret = lpSP[j].InitSP(cdn, Event);
						if (!ret)
							return FALSE;
						//�����̏������҂��X�v���C�g�I�u�W�F�N�g���Z�b�g
						j++;
					}
				}
			}
			break;

		case 1000: //�Q�[���I�[�o�[���
			//�u���b�N�C������
			lpFInOut->SetFade(_SetBlackIn);
			break;
		case 2000: //�Q�[���N���A���
			//�z���C�g�C������
			lpFInOut->SetFade(_SetWhiteIn);
			break;
		default:
			return FALSE;
			break;
		}
		return TRUE;
	}

	//------------------------------------------------------------------//
	//						�X�e�[�W�̊J�n����							//
	//------------------------------------------------------------------//
	if (lpDraw->GetfSTAGE() & _STAGE_START_ON)
	{
		//�X�e�[�W���X�^�[�g����܂ł̏���
		switch (lpDraw->GetStageNum())
		{
		case 0: //�v�����[�O���
			//�u���b�N�C�����I�������ꍇ�A�X�e�[�W�J�n�������t���O�I�t
			ret = lpFInOut->BlackIn(0x00020000);
			//				ret=lpFInOut->BlackIn(0x01020000);
			if (ret)
				lpDraw->SetfSTAGE(_STAGE_START_OFF, 0);
			break;

		case 1: //�^�C�g�����
			//�u���b�N�C�����I�������ꍇ�A�X�e�[�W�J�n�������t���O�I�t
			ret = lpFInOut->BlackIn(0x00010000);
			//				ret=lpFInOut->BlackIn(0x01010000);
			if (ret)
				lpDraw->SetfSTAGE(_STAGE_START_OFF, 0);
			break;

		case 2: //�X�e�[�W1
			ret = lpFInOut->BlackIn(0x00020000);
			//				ret=lpFInOut->BlackIn(0x01020000);
			if (ret)
			{
				if (WaitCnt++ > 200)
				{
					//					if(WaitCnt++>2){
					//�X�e�[�W1�I�[�v�j���O�����\���I�t
					lpBG->SetfBG(_SetBG00_OFF, 0);
					lpDraw->SetfSTAGE(_STAGE_START_OFF, 0);
					WaitCnt = 0;
				}
			}
			break;
		case 1000: //�Q�[���I�[�o�[���
			ret = lpFInOut->BlackIn(0x00050000);
			//				ret=lpFInOut->BlackIn(0x01050000);
			if (ret)
				lpDraw->SetfSTAGE(_STAGE_START_OFF, 0);
			break;
		case 2000: //�Q�[���N���A���
			ret = lpFInOut->WhiteIn(0x00008000);
			//				ret=lpFInOut->WhiteIn(0x01008000);
			if (ret)
				lpDraw->SetfSTAGE(_STAGE_START_OFF, 0);
			break;
		default:
			return FALSE;
			break;
		}
		return TRUE;
	}

	//------------------------------------------------------------------//
	//				�e�X�e�[�W���ł̃C�x���g����						//
	//------------------------------------------------------------------//
	switch (lpDraw->GetStageNum())
	{
	case 0: //�v�����[�O���
		if (!(lpDraw->GetfSTAGE() & _fSTAGE_CHK_InitSTAGE))
		{
			//��ʂ����΂炭��~������
			if (WaitCnt++ > 200)
			{
				//				if(WaitCnt++>2){
				WaitCnt = 0;
				lpDraw->SetfSTAGE(_STAGE_ENDWAIT_ON, 1);
			}
		}
		break;

	case 1: //�^�C�g�����
		if (!(lpDraw->GetfSTAGE() & _fSTAGE_CHK_InitSTAGE))
		{
			//�W���C�X�e�B�b�Nor�L�[�|�[�h����̓��͂��擾
			JoyOld = JoyNew;
			JoyNew = Joy_Get();
			if ((JoyNew ^ JoyOld) & (JoyNew & JOY_UP))
			{ //�オ�����ꂽ
				if (!(lpBG->GetfBG() & _SetBG00_ON))
					lpBG->SetfBG(_SetBG00_ON, 1);
				else
					lpBG->SetfBG(_SetBG00_OFF, 0);
			}
			else if ((JoyNew ^ JoyOld) & (JoyNew & JOY_DOWN))
			{ //���������ꂽ
				if (!(lpBG->GetfBG() & _SetBG00_ON))
					lpBG->SetfBG(_SetBG00_ON, 1);
				else
					lpBG->SetfBG(_SetBG00_OFF, 0);
			}
			// A�������ƃQ�[���X�^�[�g
			if ((JoyNew ^ JoyOld) & (JoyNew & JOY_A))
			{
				if (lpBG->GetfBG() & _SetBG00_ON)
					lpDraw->SetfSTAGE(_GAME_EXIT, 1);
				else
					lpDraw->SetfSTAGE(_STAGE_ENDWAIT_ON, 1);
			}
		}
		break;

	case 2: //�X�e�[�W1
		//�Q�[�����ꎞ��~������t���O�������Ă��邩�ǂ����`�F�b�N
		if (!(lpDraw->GetfSTAGE() & _fSTAGE_CHK_InitSTAGE) && lpJIKI->GetfJIKI() & _GAME_STOP_ON)
		{

			//���@���p���[�A�b�v��ʏ������̏ꍇ��ʂ������Â�����
			if (lpJIKI->GetfJIKI() & _PWUP_ON)
			{
				ret = lpFInOut->LittleBlack(0x00140000, 0); //��ʂ������Â�����
				if (!ret)
				{
					// BGM�̃{�����[�����i��
					Vol = -1000;
					lpDMPerf->SetGlobalParam(GUID_PerfMasterVolume, &Vol, (DWORD)sizeof(long));
				}
			}

			//�R���e�B�j���[��ʎ��̏���
			if (lpJIKI->GetfJIKI() & _END_ON)
			{
				//��ʂ������Â�����
				ret = lpFInOut->LittleBlack(0x00140000, 0);
				if (!ret)
				{
					// BGM���ꎞ��~������
					if (lpJIKI->GetfJIKI() & _BOSS_ON)
					{														//�{�X��
						lpDMSegState[_BOSS]->GetSeek(&SegStartTime[_BOSS]); //��~���ꂽ�ʒu���擾
						lpDMPerf->Stop(lpDMSegment[_BOSS], lpDMSegState[_BOSS], 0, DMUS_SEGF_DEFAULT);
					}
					else
					{ //�ʏ�V�[��
						lpDMSegState[_STAGE1]->GetSeek(&SegStartTime[_STAGE1]);
						lpDMPerf->Stop(lpDMSegment[_STAGE1], lpDMSegState[_STAGE1], 0, DMUS_SEGF_DEFAULT);
					}
					//�R���e�B�j���[��ʂ�BGM���Đ�
					lpDMSegment[_CONTINUE]->SetStartPoint(0);
					lpDMPerf->PlaySegment(lpDMSegment[_CONTINUE], DMUS_SEGF_DEFAULT | DMUS_SEGF_SECONDARY, 0, &lpDMSegState[_CONTINUE]);
				}
			}
		}

		//�p���[�A�b�v��ʂ��I���������ʂƉ������ɖ߂�����
		if (lpJIKI->GetfJIKI() & _PWUPD_ON)
		{
			lpFInOut->SetFade(_SetDef); //���̐F�ɖ߂�
			// BGM�̃{�����[�������̃��x���ɖ߂�
			Vol = 0;
			lpDMPerf->SetGlobalParam(GUID_PerfMasterVolume, &Vol, (DWORD)sizeof(long));
		}

		//�R���e�B�j���[���I�����ꂽ�ꍇ��ʂƉ������ɖ߂�����
		if (lpJIKI->GetfJIKI() & _CONTINUE_ON)
		{
			lpFInOut->SetFade(_SetDef); //���̐F�ɖ߂�
			//�R���e�B�j���[�V�[����BGM��~
			lpDMPerf->Stop(lpDMSegment[_CONTINUE], lpDMSegState[_CONTINUE], 0, DMUS_SEGF_DEFAULT);
			// BGM�ĊJ
			if (lpJIKI->GetfJIKI() & _BOSS_ON)
			{ //�{�X��
				lpDMSegment[_BOSS]->SetStartPoint(SegStartTime[_BOSS]);
				lpDMPerf->PlaySegment(lpDMSegment[_BOSS], DMUS_SEGF_DEFAULT, 0, &lpDMSegState[_BOSS]);
			}
			else
			{
				lpDMSegment[_STAGE1]->SetStartPoint(SegStartTime[_STAGE1]);
				lpDMPerf->PlaySegment(lpDMSegment[_STAGE1], DMUS_SEGF_DEFAULT, 0, &lpDMSegState[_STAGE1]);
			}
		}

		//�{�X�L���������ꂽ��BGM�ύX
		if (lpJIKI->GetfJIKI() & _TARGET_ON)
		{
			lpDMSegment[_BOSS]->SetStartPoint(0);
			lpDMComposer->AutoTransition(
				lpDMPerf, lpDMSegment[_BOSS], DMUS_COMMANDT_END, DMUS_SEGF_DEFAULT,
				NULL, NULL, &lpDMSegState[_BOSS], NULL);
		}
		break;
	case 1000: //�Q�[���I�[�o�[���
		if (!(lpDraw->GetfSTAGE() & _fSTAGE_CHK_InitSTAGE))
		{
			//��ʂ����΂炭��~������
			if (WaitCnt++ > 100)
			{
				//				if(WaitCnt++>1){
				WaitCnt = 0;
				lpDraw->SetfSTAGE(_STAGE_ENDWAIT_ON, 1);
			}
		}
		break;
	case 2000: //�Q�[���N���A���
		if (!(lpDraw->GetfSTAGE() & _fSTAGE_CHK_InitSTAGE))
		{
			//�W���C�X�e�B�b�Nor�L�[�|�[�h����̓��͂��擾
			JoyOld = JoyNew;
			JoyNew = Joy_Get();
			// A�������^�C�g����ʂ֖߂�
			if ((JoyNew ^ JoyOld) & (JoyNew & JOY_A))
			{
				WaitCnt = 0;
				lpDraw->SetfSTAGE(_STAGE_ENDWAIT_ON, 1);
			}
		}
		break;
	default:
		break;
	}

	//�I�t�X�N���[���T�[�t�F�X�̃��X�g�`�F�b�N
	ret = lpDraw->RestoreJIKI();
	if (!ret)
	{
		MessageBox(hWnd, "RestoreJIKI", "ERROR", MB_OK | MB_APPLMODAL);
		return FALSE;
	}
	ret = lpDraw->RestoreSP();
	if (!ret)
	{
		MessageBox(hWnd, "RestoreSP", "ERROR", MB_OK | MB_APPLMODAL);
		return FALSE;
	}
	ret = lpDraw->RestoreBG();
	if (!ret)
	{
		MessageBox(hWnd, "RestoreBG", "ERROR", MB_OK | MB_APPLMODAL);
		return FALSE;
	}

// VRAM�̋󂫗e�ʂ𒲂ׂ�
#if DEBUG >= 2
	DDCAPS hwCaps, swCaps;
	memset(&hwCaps, 0, sizeof(DDCAPS));
	hwCaps.dwSize = sizeof(DDCAPS);
	memset(&swCaps, 0, sizeof(DDCAPS));
	swCaps.dwSize = sizeof(DDCAPS);
	lpDD->GetCaps(&hwCaps, &swCaps);
	TotalMem = hwCaps.dwVidMemTotal;
	FreeMem = hwCaps.dwVidMemFree;
#endif

	return TRUE;
}