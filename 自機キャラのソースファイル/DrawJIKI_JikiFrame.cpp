/*=============================================================================
	���@�{�̂Ɋւ���A�j���[�V�����V�[���̏���(�`��͂��Ă��Ȃ�)
==============================================================================*/

#include <windows.h>
#include <ddraw.h>

#include "Macros.h"
#include "Draw.h"
#include "Struct.h"
#include "DrawJIKI.h"
#include "DrawJIKIDataEXT.h"
#include "DrawMacros.h"

//////////////////////////////////////////////////////////////////////
//	���@�̔��j�V�[���̃A�j���[�V��������������
//////////////////////////////////////////////////////////////////////
void DrawJIKI::JikiBakuFrame()
{
	// SP�T�[�t�F�X����̓]����`�͈͂��Z�b�g
	//��ʂ���͂ݏo�������͓]�����Ȃ����������Ă��܂�
	Baku.SrcR = SpJIKI[Baku.AnimePat[Baku.AnimeCnt]];

	Baku.DesR.left = Cdn.x - 22;
	Baku.DesR.top = Cdn.y - 16;
	Baku.DesR.right = Cdn.x + 42;
	Baku.DesR.bottom = Cdn.y + 48;

	if (Baku.DesR.left < 0)
	{
		Baku.SrcR.left = Baku.SrcR.left - Baku.DesR.left;
		Baku.DesR.left = 0;
	}
	else if (Baku.DesR.right > WinXMax)
	{
		Baku.SrcR.right = Baku.SrcR.right - (Baku.DesR.right - WinXMax);
		Baku.DesR.right = WinXMax;
	}
	if (Baku.DesR.top < 0)
	{
		Baku.SrcR.top = Baku.SrcR.top - Baku.DesR.top;
		Baku.DesR.top = 0;
	}
	else if (Baku.DesR.bottom > WinYMax)
	{
		Baku.SrcR.bottom = Baku.SrcR.bottom - (Baku.DesR.bottom - WinYMax);
		Baku.DesR.bottom = WinYMax;
	}

	//�����V�[���̋�`�͈͂��Z�b�g
	if (!(Baku.FrameCnt--))
	{
		//�����A�j���[�V�������I���������`�F�b�N
		if ((Baku.AnimeCnt + 1) == Baku.Anime)
		{
			//�������̎��@�A�j���[�V�����f�[�^�������l�ɖ߂�
			Baku.AnimeCnt = 0;
			Baku.FrameCnt = Baku.AnimeFrame[0];
			//�Q�[���ꎞ��~�t���O�I��(�R���e�B�j���[������)
			fJIKI |= _GAME_STOP_ON;
		}
		else
		{
			Baku.AnimeCnt = (Baku.AnimeCnt + 1) % Baku.Anime; //�A�j���p�^�[����i�߂�
			Baku.FrameCnt = Baku.AnimeFrame[Baku.AnimeCnt];	  //�\������A�j���p�^�[���̃t���[�����Z�b�g
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	���@�̃A�[�����˃V�[���̃A�j���[�V��������
//////////////////////////////////////////////////////////////////////
void DrawJIKI::JikiArmFrame()
{
	//���@�̕����̋�`�͈͂��Z�b�g
	// SP�T�[�t�F�X����̓]����`�͈͂��Z�b�g
	ArmJIKI.SrcR = SpJIKI[ArmJIKI.AnimePat[ArmJIKI.AnimeCnt]];

	//��ʂ���͂ݏo�������͓]�����Ȃ����������Ă��܂�
	ArmJIKI.DesR.left = Cdn.x - 22;
	ArmJIKI.DesR.top = Cdn.y + 16;
	ArmJIKI.DesR.right = Cdn.x + 42;
	ArmJIKI.DesR.bottom = Cdn.y + 32;

	if (ArmJIKI.DesR.left < 0)
	{
		ArmJIKI.SrcR.left = ArmJIKI.SrcR.left - ArmJIKI.DesR.left;
		ArmJIKI.DesR.left = 0;
	}
	else if (ArmJIKI.DesR.right > WinXMax)
	{
		ArmJIKI.SrcR.right = ArmJIKI.SrcR.right - (ArmJIKI.DesR.right - WinXMax);
		ArmJIKI.DesR.right = WinXMax;
	}

	//�A�j���[�V������i�߂�
	if (!(ArmJIKI.FrameCnt--))
	{
		ArmJIKI.AnimeCnt = (ArmJIKI.AnimeCnt + 1) % ArmJIKI.Anime; //�A�j���p�^�[����i�߂�
		ArmJIKI.FrameCnt = ArmJIKI.AnimeFrame[ArmJIKI.AnimeCnt];   //�\������A�j���p�^�[���̃t���[�����Z�b�g
	}

	//�A�[���̕����̋�`�͈͂��Z�b�g
	// SP�T�[�t�F�X����̓]����`�͈͂��Z�b�g
	Arm.SrcR = SpJIKI[Arm.AnimePat[Arm.AnimeCnt]];

	//��ʂ���͂ݏo�������͓]�����Ȃ����������Ă��܂�
	Arm.DesR.left = Cdn.x - 6;
	Arm.DesR.top = Cdn.y - (Arm.SrcR.bottom - Arm.SrcR.top) + 16;
	Arm.DesR.right = Cdn.x + 26;
	Arm.DesR.bottom = Cdn.y + 16;

	if (Arm.DesR.left < 0)
	{
		Arm.SrcR.left = Arm.SrcR.left - Arm.DesR.left;
		Arm.DesR.left = 0;
	}
	else if (Arm.DesR.right > WinXMax)
	{
		Arm.SrcR.right = Arm.SrcR.right - (Arm.DesR.right - WinXMax);
		Arm.DesR.right = WinXMax;
	}
	if (Arm.DesR.top < 0)
	{
		Arm.SrcR.top = Arm.SrcR.top - Arm.DesR.top;
		Arm.DesR.top = 0;
	}

	//�A�j���[�V������i�߂�
	if (!(Arm.FrameCnt--))
	{
		//�A�[�����˂̃A�j���[�V�������I���������`�F�b�N
		if ((Arm.AnimeCnt + 1) == Arm.Anime)
		{
			//�A�[�����ˎ��̎��@�A�j���[�V�����f�[�^�������l�ɖ߂�
			ArmJIKI.AnimeCnt = 0;
			ArmJIKI.FrameCnt = ArmJIKI.AnimeFrame[0];
			//�A�[�����ˎ��̃A�[���A�j���[�V�����f�[�^�������l�ɖ߂�
			Arm.AnimeCnt = 0;
			Arm.FrameCnt = Arm.AnimeFrame[0];
			//�A�[�����˃t���O���I�t
			fJIKI &= _ARM_OFF;
		}
		else
		{
			Arm.AnimeCnt = (Arm.AnimeCnt + 1) % Arm.Anime; //�A�j���p�^�[����i�߂�
			Arm.FrameCnt = Arm.AnimeFrame[Arm.AnimeCnt];   //�\������A�j���p�^�[���̃t���[�����Z�b�g
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	���@�̒e�z���V�[���̃A�j���[�V��������������
//////////////////////////////////////////////////////////////////////
void DrawJIKI::JikiTmkyFrame()
{
	if (fJIKI & _FUEL_ON)
	{
		// SP�T�[�t�F�X����̓]����`�͈͂��Z�b�g
		//��ʂ���͂ݏo�������͓]�����Ȃ����������Ă��܂�
		TmKySy.SrcR = SpJIKI[TmKySy.AnimePat[TmKySy.AnimeCnt]];
		TmKySy.DesR.left = Cdn.x - 22;
		TmKySy.DesR.top = Cdn.y + 16;
		TmKySy.DesR.right = Cdn.x + 42;
		TmKySy.DesR.bottom = Cdn.y + 32;

		if (TmKySy.DesR.left < 0)
		{
			TmKySy.SrcR.left = TmKySy.SrcR.left - TmKySy.DesR.left;
			TmKySy.DesR.left = 0;
		}
		else if (TmKySy.DesR.right > WinXMax)
		{
			TmKySy.SrcR.right = TmKySy.SrcR.right - (TmKySy.DesR.right - WinXMax);
			TmKySy.DesR.right = WinXMax;
		}

		//���@�̕����̋�`�͈͂��Z�b�g
		if (!(TmKySy.FrameCnt--))
		{
			TmKySy.AnimeCnt = (TmKySy.AnimeCnt + 1) % TmKySy.Anime; //�A�j���p�^�[����i�߂�
			TmKySy.FrameCnt = TmKySy.AnimeFrame[TmKySy.AnimeCnt];	//�\������A�j���p�^�[���̃t���[�����Z�b�g
		}

		//�e�z�����f�t���O�̗����Ă���ꍇ�̏���
	}
	else if (fJIKI & _FUELD_ON)
	{
		//�e�z�����f�t���O�I�t
		fJIKI &= _FUELD_OFF;

		//�A�j���[�V�����f�[�^��������
		TmKySy.AnimeCnt = 0;
		TmKySy.FrameCnt = TmKySy.AnimeFrame[0];
	}
}