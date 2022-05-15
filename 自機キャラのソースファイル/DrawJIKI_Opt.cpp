/*=============================================================================
�@�I�v�V�����̈ړ��A�����蔻��A�`�擙�Ɋւ���\�[�X�t�@�C��
==============================================================================*/
#include <windows.h>
#include <ddraw.h>

#include "Macros.h"
#include "Draw.h"
#include "Struct.h"
#include "DrawJIKI.h"
#include "DrawJIKIDataEXT.h"
#include "DrawMacros.h"

extern LPDIRECTDRAWSURFACE7 lpDDSBack; //�o�b�N�T�[�t�F�X

#define _KOSIZE 5  //�I�v�V�������Œ�ړ����Ɏg�p����ߋ��̎��@�̍��W���i�[����\���̂̐�
#define _TUSIZE 60 //�I�v�V�������ǔ��ړ����Ɏg�p����ߋ��̎��@�̍��W���i�[����\���̂̐�

//////////////////////////////////////////////////////////////////////
//	OPTDATA�\���̂̊e�����o�̏�����
//////////////////////////////////////////////////////////////////////
BOOL DrawJIKI::InitOpt()
{
	short i, j;

	OPT.OptLV = 0;		 //�I�v�V�����̑�������Ă��鐔��������
	OPT.KoteCnt = 0;	 // KoteCdn�̕ۑ���̃J�E���^�̏�����
	OPT.TuibCnt = 0;	 // TuibCdn�̕ۑ���̃J�E���^�̏�����
	OPT.KaitKaku[0] = 0; //��]�t���O�̗����Ă���I�v�V�����Ɏg�p������]�p�x�̏�����
	OPT.KaitKaku[1] = 90;
	OPT.KaitKaku[2] = 180;
	OPT.KaitKaku[3] = 270;
	for (i = 0; i < 4; i++)
	{ //��]�����������t���O�̏�����
		OPT.KaitenF[i] = 0;
	}

	///////////////////////////////////////////////////////////
	//------------------------OPT.Jewel�̏�����----------------

	OPTANI oa; // OPTANI�\���̂ɑO�����Ēl���Z�b�g����OPT.Jewel������������

	oa.fOPT = 0;	//�I�v�V�����̏�����Ԃ�ݒ�
	oa.Width = 20;	//�I�v�V�����̉���
	oa.Height = 32; //�I�v�V�����̏c��
	oa.dCdn.x = 0;	//�I�v�V�����̉�ʍ��W�v�Z�p
	oa.dCdn.y = 0;
	oa.Cdn.x = 0; //�I�v�V�����̌��݂̉�ʍ��W��0�ŏ�����
	oa.Cdn.y = 0;
	oa.MapCdn.x = 0; //�I�v�V�����̌��݂̐�΍��W��0�ŏ�����
	oa.MapCdn.y = 0;
	oa.Anime = TamAmeJIKI[2];										   //�I�v�V�����A�j���[�V�����p�^�[�����̏�����
	memcpy(&oa.AnimePat[0], &TamPatJIKI[2][0], sizeof(BYTE) * 10);	   //�I�v�V�����A�j���[�V�����p�^�[���z��̏�����
	memcpy(&oa.AnimeFrame[0], &TamFrameJIKI[2][0], sizeof(BYTE) * 10); //�I�v�V�����A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	oa.AnimeCnt = 0;												   //�ŏ��ɕ\������A�j���[�V�����p�^�[����������
	oa.FrameCnt = oa.AnimeFrame[0];									   //�ŏ��̃A�j���[�V�����t���[���J�E���g��������
	oa.SrcR.left = 0;												   //�]������`��0�ŏ�����
	oa.SrcR.top = 0;
	oa.SrcR.right = 0;
	oa.SrcR.bottom = 0;
	oa.DesR = oa.SrcR; //�]�����`��0�ŏ�����

	//�I�v�V�����I�u�W�F�N�g��4���A������
	for (i = 0; (DWORD)i < 4; i++)
	{
		memcpy(&OPT.Jewel[i], &oa, sizeof(OPTANI)); //�m�ۂ����̈�̐擪����R�s�[
	}

	//////////////////////////////////////////////////////////////////////
	//------OPT.AtkDat[0-3].Opt_LSR1���Ǘ�����I�u�W�F�N�g�̏�����--------
	TAMA tama; // TAMA�\���̂ɑO�����Ēl���Z�b�g���ď���������

	tama.fTama = _TOFF; //�e�̏�����Ԃ�ݒ�
	tama.Width = 4;		//�e�̉���
	tama.Height = 72;	//�e�̏c��
	tama.Mov.x = 0.0;	//�e�̈ړ����x��������
	tama.Mov.y = 20.0;
	tama.dCdn.x = 0; //�e�̉�ʍ��W�v�Z�p
	tama.dCdn.y = 0;
	tama.Cdn.x = 0; //�e�̌��݂̉�ʍ��W��0�ŏ�����
	tama.Cdn.y = 0;
	tama.MapCdn.x = 0; //�e�̌��݂̐�΍��W��0�ŏ�����
	tama.MapCdn.y = 0;
	tama.Anime = TamAmeJIKI[0]; //�e�A�j���[�V�����p�^�[�����̏�����
	//�e�A�j���[�V�����p�^�[���z��̏�����
	memcpy(&tama.AnimePat[0], &TamPatJIKI[0][0], sizeof(BYTE) * 10);
	//�e�A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	memcpy(&tama.AnimeFrame[0], &TamFrameJIKI[0][0], sizeof(BYTE) * 10);
	tama.AnimeCnt = 0;					//�ŏ��ɕ\������A�j���[�V�����p�^�[����������
	tama.FrameCnt = tama.AnimeFrame[0]; //�ŏ��̃A�j���[�V�����t���[���J�E���g��������
	tama.SrcR.left = 0;					//�]������`��0�ŏ�����
	tama.SrcR.top = 0;
	tama.SrcR.right = 0;
	tama.SrcR.bottom = 0;
	tama.DesR = tama.SrcR; //�]�����`��0�ŏ�����

	char *pTama;

	for (i = 0; i < 4; i++)
	{
		//�e�X�e�[�^�X�\���̂ɒl���Z�b�g���Ēe�����m�ۂ����A�e�I�u�W�F�N�g��������
		OPT.AtkDat[i].Opt_LSR1.TTama = 3;								//��ʏ�ɑ��݂ł���e�̐�
		OPT.AtkDat[i].Opt_LSR1.WTama = 10;								//�e�𔭎˂���Ԋu
		OPT.AtkDat[i].Opt_LSR1.WTamaCnt = OPT.AtkDat[i].Opt_LSR1.WTama; //�e���˂܂ł̃J�E���^

		//���˂ł���e����TAMA�\���̂��m��
		OPT.AtkDat[i].Opt_LSR1.tm = new TAMA[OPT.AtkDat[i].Opt_LSR1.TTama];
		if (!OPT.AtkDat[i].Opt_LSR1.tm)
			return FALSE;
		//�m�ۂ����̈�̐擪�A�h���X���R�s�[
		pTama = (char *)OPT.AtkDat[i].Opt_LSR1.tm;
		//�e�I�u�W�F�N�g���m�ۂ������A������
		for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_LSR1.TTama; j++)
		{
			memcpy(pTama, &tama, sizeof(TAMA)); //�m�ۂ����̈�̐擪����R�s�[
			pTama += sizeof(TAMA);				//���ɃR�s�[����Ƃ���܂ŃA�h���X��i�߂�
		}
	}

	//////////////////////////////////////////////////////////////////////
	//-------------OPT.MSR1���Ǘ�����I�u�W�F�N�g�̏�����---------------

	tama.fTama = _TOFF; //�e�̏�����Ԃ�ݒ�
	tama.Width = 16;	//�e�̉���
	tama.Height = 32;	//�e�̏c��
	tama.Mov.x = 0.0;	//�e�̈ړ����x��������
	tama.Mov.y = 16.0;
	tama.dCdn.x = 0; //�e�̉�ʍ��W�v�Z�p
	tama.dCdn.y = 0;
	tama.Cdn.x = 0; //�e�̌��݂̉�ʍ��W��0�ŏ�����
	tama.Cdn.y = 0;
	tama.MapCdn.x = 0; //�e�̌��݂̐�΍��W��0�ŏ�����
	tama.MapCdn.y = 0;
	tama.Anime = TamAmeJIKI[1]; //�e�A�j���[�V�����p�^�[�����̏�����
	//�e�A�j���[�V�����p�^�[���z��̏�����
	memcpy(&tama.AnimePat[0], &TamPatJIKI[1][0], sizeof(BYTE) * 10);
	//�e�A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	memcpy(&tama.AnimeFrame[0], &TamFrameJIKI[1][0], sizeof(BYTE) * 10);
	tama.AnimeCnt = 0;					//�ŏ��ɕ\������A�j���[�V�����p�^�[����������
	tama.FrameCnt = tama.AnimeFrame[0]; //�ŏ��̃A�j���[�V�����t���[���J�E���g��������
	tama.SrcR.left = 0;					//�]������`��0�ŏ�����
	tama.SrcR.top = 0;
	tama.SrcR.right = 0;
	tama.SrcR.bottom = 0;
	tama.DesR = tama.SrcR; //�]�����`��0�ŏ�����

	for (i = 0; i < 4; i++)
	{
		//�e�X�e�[�^�X�\���̂ɒl���Z�b�g���Ēe�����m�ۂ����A�e�I�u�W�F�N�g��������
		OPT.AtkDat[i].Opt_MSR1.TTama = 6;								//��ʏ�ɑ��݂ł���e�̐�
		OPT.AtkDat[i].Opt_MSR1.WTama = 8;								//�e�𔭎˂���Ԋu
		OPT.AtkDat[i].Opt_MSR1.WTamaCnt = OPT.AtkDat[i].Opt_MSR1.WTama; //�e���˂܂ł̃J�E���^

		//���˂ł���e����TAMA�\���̂��m��
		OPT.AtkDat[i].Opt_MSR1.tm = new TAMA[OPT.AtkDat[i].Opt_MSR1.TTama];
		if (!OPT.AtkDat[i].Opt_MSR1.tm)
			return FALSE;
		//�m�ۂ����̈�̐擪�A�h���X���R�s�[
		pTama = (char *)OPT.AtkDat[i].Opt_MSR1.tm;
		//�e�I�u�W�F�N�g���m�ۂ������A������
		for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_MSR1.TTama; j++)
		{
			memcpy(pTama, &tama, sizeof(TAMA)); //�m�ۂ����̈�̐擪����R�s�[
			pTama += sizeof(TAMA);				//���ɃR�s�[����Ƃ���܂ŃA�h���X��i�߂�
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	���I�Ɋm�ۂ��郁�����̈�ւ̃|�C���^��NULL�ŏ�����
//////////////////////////////////////////////////////////////////////
void DrawJIKI::InitOptAtkDatTm()
{
	for (short i = 0; i < 4; i++)
	{
		OPT.AtkDat[i].Opt_MSR1.tm = NULL;
		OPT.AtkDat[i].Opt_LSR1.tm = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
//	�m�ۂ��Ă����e�I�u�W�F�N�g�̃������̈�����
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptRelease()
{
	for (short i = 0; i < 4; i++)
	{
		RELEASE_dels(OPT.AtkDat[i].Opt_MSR1.tm);
		RELEASE_dels(OPT.AtkDat[i].Opt_LSR1.tm);
	}
}

//////////////////////////////////////////////////////////////////////
//	�I�v�V�����̕\�����W�̏�����(�p���[�A�b�v������ʂ�EXIT���鎞�Ɏg�p)
//////////////////////////////////////////////////////////////////////
void DrawJIKI::InitOptCdn()
{
	switch (Wpd.flag & _OPT_LVCHK)
	{ //�I�v�V�����̃��x���𒲂ׂĂ���
	case _OPT1_ON:
		OPT.OptLV = 1;
		break;
	case _OPT2_ON:
		OPT.OptLV = 2;
		break;
	case _OPT3_ON:
		OPT.OptLV = 3;
		break;
	case _OPT4_ON:
		OPT.OptLV = 4;
		break;
	default:
		OPT.OptLV = 0;
		break;
	}

	BYTE fKO = 0, fKA = 0, fTU = 0; //�Œ�A��]�A�ǔ��t���O�̗����Ă���I�v�V�����̕\�����W�������p�t���O
	short i, j;
	short kotsu = 0; //�Œ�t���O�̗����Ă���I�v�V�����̐��J�E���g�p

	if (OPT.OptLV)
	{
		for (i = 0; i < OPT.OptLV; i++)
		{
			switch (OPT.Jewel[i].fOPT & _fOPT_CHK)
			{ //�Œ�A��]�A�ǔ��̂ǂ̃t���N�������Ă��邩�`�F�b�N
			case _OPTKO_ON:
				if (fKO)
					;
				else
				{
					//�I�v�V�������Œ�ړ����Ɏg�p����ߋ��̎��@�̍��W���i�[����\���̂̏�����
					for (j = 0; j < _KOSIZE; j++)
					{
						OPT.KoteCdn[j] = Cdn;
					}
					fKO = 1; //�����������Q�񂵂Ȃ��悤�ɂ��邽�߂Ƀt���O�𗧂Ă�
				}
				//�I�v�V�����̌��݂̈ʒu��������
				if (kotsu == 0)
				{ //�Œ�t���O�������Ă���1�ڂ̃I�v�V�����̏������W
					OPT.Jewel[i].dCdn.x = (double)Cdn.x - 40;
					OPT.Jewel[i].dCdn.y = (double)Cdn.y;
				}
				else if (kotsu == 1)
				{ //�Œ�t���O�������Ă���2�ڂ̃I�v�V�����̏������W
					OPT.Jewel[i].dCdn.x = (double)Cdn.x + 40;
					OPT.Jewel[i].dCdn.y = (double)Cdn.y;
				}
				else if (kotsu == 2)
				{ //�Œ�t���O�������Ă���3�ڂ̃I�v�V�����̏������W
					OPT.Jewel[i].dCdn.x = (double)Cdn.x - 20;
					OPT.Jewel[i].dCdn.y = (double)Cdn.y - 40;
				}
				else if (kotsu == 3)
				{ //�Œ�t���O�������Ă���4�ڂ̃I�v�V�����̏������W
					OPT.Jewel[i].dCdn.x = (double)Cdn.x + 20;
					OPT.Jewel[i].dCdn.y = (double)Cdn.y - 40;
				}
				kotsu++;
				break;
			case _OPTKA_ON:
				if (fKA)
					;
				else
				{
					//��]�t���O�̗����Ă���I�v�V�����Ɏg�p������]�p�x�̏�����
					OPT.KaitKaku[0] = 0;
					OPT.KaitKaku[1] = 45;
					OPT.KaitKaku[2] = 90;
					OPT.KaitKaku[3] = 135;
					fKA = 1;
				}
				//��]�����������t���O��������
				OPT.KaitenF[i] = 0;

				//��]�t���O�������Ă���I�v�V�����̏������W
				OPT.Jewel[i].dCdn.x = (double)Cdn.x;
				OPT.Jewel[i].dCdn.y = (double)Cdn.y;

				break;
			case _OPTTU_ON:
				if (fTU)
					;
				else
				{
					//�I�v�V�������ǔ��ړ����Ɏg�p����ߋ��̎��@�̍��W���i�[����\���̂̏�����
					for (j = 0; j < _TUSIZE; j++)
					{
						OPT.TuibCdn[j] = Cdn;
					}
					fTU = 1;
				}
				OPT.Jewel[i].dCdn.x = (double)Cdn.x;
				OPT.Jewel[i].dCdn.y = (double)Cdn.y;

				break;
			default:
				break;
			}
			//�f�B�X�v���C�̕\�����W�ɒ�������
			OPT.Jewel[i].Cdn.x = (LONG)OPT.Jewel[i].dCdn.x;
			OPT.Jewel[i].Cdn.y = (LONG)OPT.Jewel[i].dCdn.y;
			OPT.Jewel[i].MapCdn.x = MainCdn.x + OPT.Jewel[i].Cdn.x;
			OPT.Jewel[i].MapCdn.y = MainCdn.y + OPT.Jewel[i].Cdn.y;
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	���@�̉ߋ��̍��W���w�肳�ꂽ�������ۑ�����
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptMovCnt(BYTE joyst)
{
	//�ǔ��I�v�V�����p
	if (joyst & 0x0F) //�W���C�X�e�B�b�N�ɏ㉺���E�����ꂩ�̓��͂̂������ꍇ�̂�
	{				  //���@�̌��݂̍��W���擾
		OPT.TuibCdn[OPT.TuibCnt] = Cdn;
		OPT.TuibCnt++;
		if (OPT.TuibCnt > _TUSIZE - 1)
			OPT.TuibCnt = 0;
	}

	//�Œ�I�v�V�����p
	OPT.KoteCdn[OPT.KoteCnt] = Cdn;
	OPT.KoteCnt++;
	if (OPT.KoteCnt > _KOSIZE - 1)
		OPT.KoteCnt = 0;
}

//////////////////////////////////////////////////////////////////////
//	�I�v�V�����̈ړ��ƃA�j���[�V��������
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptMov()
{
	short kc = 0, tc = 0; //�Œ�A�ǔ��J�E���^��Ɨp
	short kotsu = 0;	  //�Œ�t���O�̗����Ă���I�v�V�����̐��J�E���g�p
	short kaisu = 0;	  //��]�t���O�̗����Ă���I�v�V�����̐��J�E���g�p
	short tuisu = 1;	  //�ǔ��t���O�̗����Ă���I�v�V�����̐��J�E���g�p

	for (short i = 0; i < OPT.OptLV; i++)
	{
		switch (OPT.Jewel[i].fOPT & _fOPT_CHK)
		{				//�ړ��p�^�[���̃`�F�b�N
		case _OPTKO_ON: //�Œ�
			kc = OPT.KoteCnt - (_KOSIZE - 1);
			if (kc < 0)
				kc = kc + _KOSIZE; // 0�`_KOSIZE�����[�v�����鏈��
			//�ړ������鏈��
			if (kotsu == 0)
			{
				OPT.Jewel[i].dCdn.x = (double)OPT.KoteCdn[kc].x - 40;
				OPT.Jewel[i].dCdn.y = (double)OPT.KoteCdn[kc].y;
			}
			else if (kotsu == 1)
			{
				OPT.Jewel[i].dCdn.x = (double)OPT.KoteCdn[kc].x + 40;
				OPT.Jewel[i].dCdn.y = (double)OPT.KoteCdn[kc].y;
			}
			else if (kotsu == 2)
			{
				OPT.Jewel[i].dCdn.x = (double)OPT.KoteCdn[kc].x - 20;
				OPT.Jewel[i].dCdn.y = (double)OPT.KoteCdn[kc].y - 40;
			}
			else if (kotsu == 3)
			{
				OPT.Jewel[i].dCdn.x = (double)OPT.KoteCdn[kc].x + 20;
				OPT.Jewel[i].dCdn.y = (double)OPT.KoteCdn[kc].y - 40;
			}
			kotsu++;
			break;
		case _OPTKA_ON: //��]
			OPT.Jewel[i].dCdn.y = (double)Cdn.y;
			if (!OPT.KaitenF[kaisu])
			{ //��]�����t���O���`�F�b�N
				//���@�̍����ŉ�]
				OPT.Jewel[i].dCdn.x = (double)Cdn.x - 32;
				OPT.Jewel[i].dCdn.x += 32 * Cos[OPT.KaitKaku[kaisu]];
				OPT.Jewel[i].dCdn.y += -32 * Sin[OPT.KaitKaku[kaisu]];

				OPT.KaitKaku[kaisu] += 10; //�p�x��10����
				if (OPT.KaitKaku[kaisu] > 360)
				{ // 360���傫���Ȃ�Ȃ��悤����
					OPT.KaitKaku[kaisu] = 180;
					OPT.KaitenF[kaisu] = 1; //��]�����t���O���Z�b�g
				}
			}
			else if (OPT.KaitenF[kaisu] == 1)
			{
				//���@�̉E���ŉ�](�㔼��)
				OPT.Jewel[i].dCdn.x = (double)Cdn.x + 32;
				OPT.Jewel[i].dCdn.x += 32 * Cos[OPT.KaitKaku[kaisu]];
				OPT.Jewel[i].dCdn.y += 32 * Sin[OPT.KaitKaku[kaisu]];

				OPT.KaitKaku[kaisu] += 10;
				if (OPT.KaitKaku[kaisu] > 360)
				{
					OPT.KaitKaku[kaisu] = 0;
					OPT.KaitenF[kaisu] = 2;
				}
			}
			else if (OPT.KaitenF[kaisu] == 2)
			{
				//���@�̉E���ŉ�](������)
				OPT.Jewel[i].dCdn.x = (double)Cdn.x + 32;
				OPT.Jewel[i].dCdn.x += 32 * Cos[OPT.KaitKaku[kaisu]];
				OPT.Jewel[i].dCdn.y += 32 * Sin[OPT.KaitKaku[kaisu]];

				OPT.KaitKaku[kaisu] += 10;
				if (OPT.KaitKaku[kaisu] > 180)
				{
					OPT.KaitKaku[kaisu] = 0;
					OPT.KaitenF[kaisu] = 0;
				}
			}
			kaisu++;
			break;
		case _OPTTU_ON: //�ǔ�
			//���W�����o������(�Ӗ��AJuwel1==���@��12��O�̍��W,Juwel2==24��O.....)
			tc = OPT.TuibCnt - (tuisu * 12);
			if (tc < 0)
				tc = tc + _TUSIZE; // 0�`_TUSIZE�����[�v�����鏈��
			//�ړ������鏈��
			OPT.Jewel[i].dCdn.x = (double)OPT.TuibCdn[tc].x;
			OPT.Jewel[i].dCdn.y = (double)OPT.TuibCdn[tc].y;
			tuisu++;
			break;
		default:
			return;
			break;
		}
		//�f�B�X�v���C�̕\�����W�ɒ�������
		OPT.Jewel[i].Cdn.x = (LONG)OPT.Jewel[i].dCdn.x;
		OPT.Jewel[i].Cdn.y = (LONG)OPT.Jewel[i].dCdn.y;
		OPT.Jewel[i].MapCdn.x = MainCdn.x + OPT.Jewel[i].Cdn.x;
		OPT.Jewel[i].MapCdn.y = MainCdn.y + OPT.Jewel[i].Cdn.y;

		//�T�[�t�F�X����̓]����`�͈͂��Z�b�g
		OPT.Jewel[i].SrcR = TamSPJIKI[OPT.Jewel[i].AnimePat[OPT.Jewel[i].AnimeCnt]];

		OPT.Jewel[i].DesR.left = OPT.Jewel[i].Cdn.x;
		OPT.Jewel[i].DesR.top = OPT.Jewel[i].Cdn.y;
		OPT.Jewel[i].DesR.right = OPT.Jewel[i].Cdn.x + OPT.Jewel[i].Width;
		OPT.Jewel[i].DesR.bottom = OPT.Jewel[i].Cdn.y + OPT.Jewel[i].Height;

		//��ʊO�̕����͕`�悵�Ȃ��悤�ɂ��鏈��
		if (OPT.Jewel[i].DesR.left < 0)
		{
			OPT.Jewel[i].SrcR.left = OPT.Jewel[i].SrcR.left - OPT.Jewel[i].DesR.left;
			OPT.Jewel[i].DesR.left = 0;
		}
		else if (OPT.Jewel[i].DesR.right > WinXMax)
		{
			OPT.Jewel[i].SrcR.right = OPT.Jewel[i].SrcR.right - (OPT.Jewel[i].DesR.right - WinXMax);
			OPT.Jewel[i].DesR.right = WinXMax;
		}
		if (OPT.Jewel[i].Cdn.y < 0)
		{
			OPT.Jewel[i].SrcR.top = OPT.Jewel[i].SrcR.top - OPT.Jewel[i].DesR.top;
			OPT.Jewel[i].DesR.top = 0;
		}
		else if (OPT.Jewel[i].DesR.bottom > WinYMax)
		{
			OPT.Jewel[i].SrcR.bottom = OPT.Jewel[i].SrcR.bottom - (OPT.Jewel[i].DesR.bottom - WinYMax);
			OPT.Jewel[i].DesR.bottom = WinYMax;
		}
		//�t���[����1���炵��0�ɂȂ�����A�j���p�^�[����i�߂�
		if (!(OPT.Jewel[i].FrameCnt--))
		{
			OPT.Jewel[i].AnimeCnt = (OPT.Jewel[i].AnimeCnt + 1) % OPT.Jewel[i].Anime; //�A�j���p�^�[����i�߂�
			OPT.Jewel[i].FrameCnt = OPT.Jewel[i].AnimeFrame[OPT.Jewel[i].AnimeCnt];	  //�\������A�j���p�^�[���̃t���[�����Z�b�g
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	�I�v�V�����̍U���܂ł̃J�E���g��i�߂�
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptAttackCnt()
{
	//���ݎg�p���Ă����C���`�F�b�N���ăt���O�̗����Ă����C�̏���������
	switch (Wpd.flag & _MAINWP_CHK)
	{
	case _LSR_ON:
		switch (Wpd.flag & _LSR_LVCHK)
		{ //���[�U�[�̃��x���𒲂ׂĂ���
		case _LSR1_ON:
			switch (OPT.OptLV)
			{ //�I�v�V�����������Ă��邩���ׂĂ���
			case 4:
				OPT.AtkDat[3].Opt_LSR1.WTamaCnt--; //���˂܂ł̃J�E���g�_�E��
			case 3:
				OPT.AtkDat[2].Opt_LSR1.WTamaCnt--;
			case 2:
				OPT.AtkDat[1].Opt_LSR1.WTamaCnt--;
			case 1:
				OPT.AtkDat[0].Opt_LSR1.WTamaCnt--;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case _MSR_ON:
		switch (Wpd.flag & _MSR_LVCHK)
		{
		case _MSR1_ON:
			switch (OPT.OptLV)
			{ //�I�v�V�����������Ă��邩���ׂĂ���
			case 4:
				OPT.AtkDat[3].Opt_MSR1.WTamaCnt--; //���˂܂ł̃J�E���g�_�E��
			case 3:
				OPT.AtkDat[2].Opt_MSR1.WTamaCnt--;
			case 2:
				OPT.AtkDat[1].Opt_MSR1.WTamaCnt--;
			case 1:
				OPT.AtkDat[0].Opt_MSR1.WTamaCnt--;
				break;
			default:
				break;
			}
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
//	�I�v�V�����̍U���܂ł̃J�E���g�����Z�b�g
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptAttackCntReset()
{
	//���[�U�[�̔��˃J�E���g�����Z�b�g
	switch (Wpd.flag & _LSR_LVCHK)
	{
	case _LSR1_ON:
		switch (OPT.OptLV)
		{ //�I�v�V�����������Ă��邩���ׂĂ���
		case 4:
			//���˂܂ł̃J�E���g�����Z�b�g
			OPT.AtkDat[3].Opt_LSR1.WTamaCnt = OPT.AtkDat[3].Opt_LSR1.WTama;
		case 3:
			OPT.AtkDat[2].Opt_LSR1.WTamaCnt = OPT.AtkDat[2].Opt_LSR1.WTama;
		case 2:
			OPT.AtkDat[1].Opt_LSR1.WTamaCnt = OPT.AtkDat[1].Opt_LSR1.WTama;
		case 1:
			OPT.AtkDat[0].Opt_LSR1.WTamaCnt = OPT.AtkDat[0].Opt_LSR1.WTama;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	//�~�T�C���̔��˃J�E���g�����Z�b�g
	switch (Wpd.flag & _MSR_LVCHK)
	{
	case _MSR1_ON:
		switch (OPT.OptLV)
		{ //�I�v�V�����������Ă��邩���ׂĂ���
		case 4:
			//���˂܂ł̃J�E���g�����Z�b�g
			OPT.AtkDat[3].Opt_MSR1.WTamaCnt = OPT.AtkDat[3].Opt_MSR1.WTama;
		case 3:
			OPT.AtkDat[2].Opt_MSR1.WTamaCnt = OPT.AtkDat[2].Opt_MSR1.WTama;
		case 2:
			OPT.AtkDat[1].Opt_MSR1.WTamaCnt = OPT.AtkDat[1].Opt_MSR1.WTama;
		case 1:
			OPT.AtkDat[0].Opt_MSR1.WTamaCnt = OPT.AtkDat[0].Opt_MSR1.WTama;
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
//	�I�v�V�����̒e��S�Ĕ��ˑҋ@��Ԃɖ߂�
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptAttackReset()
{
	OptAttackCntReset(); //���˂܂ł̃J�E���^�����Z�b�g

	short i, j;

	//�S�ẴI�v�V�����̒e�t���O�𔭎ˑҋ@��ԂɃZ�b�g
	switch (Wpd.flag & _LSR_LVCHK)
	{ //�����̃��x���𒲂ׂĂ���
	case _LSR1_ON:
		for (i = 0; i < OPT.OptLV; i++)
		{
			for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_LSR1.TTama; j++)
			{
				OPT.AtkDat[i].Opt_LSR1.tm[j].fTama = _TOFF;
			}
		}
		break;
	default:
		break;
	}
	switch (Wpd.flag & _MSR_LVCHK)
	{ //�����̃��x���𒲂ׂĂ���
	case _MSR1_ON:
		for (i = 0; i < OPT.OptLV; i++)
		{
			for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_MSR1.TTama; j++)
			{
				OPT.AtkDat[i].Opt_MSR1.tm[j].fTama = _TOFF;
			}
		}
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
//	�I�v�V�����̍U������
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptAttack()
{
	//��ʂɒe�����݂��Ă��邩���Ă��Ȃ������ׂ�t���O
	bool TamaS = FALSE;

	//���˂ł����ԂɂȂ��Ă��郌�[�U�[�̔ԍ��ۑ��p
	static DWORD LsrNum[4] = {
		OPT.AtkDat[0].Opt_LSR1.TTama, OPT.AtkDat[1].Opt_LSR1.TTama,
		OPT.AtkDat[2].Opt_LSR1.TTama, OPT.AtkDat[3].Opt_LSR1.TTama};
	//���˂ł����ԂɂȂ��Ă���~�T�C���̔ԍ��ۑ��p
	static DWORD MsrNum[4] = {
		OPT.AtkDat[0].Opt_MSR1.TTama, OPT.AtkDat[1].Opt_MSR1.TTama,
		OPT.AtkDat[2].Opt_MSR1.TTama, OPT.AtkDat[3].Opt_MSR1.TTama};

	short i, j; //���[�v�J�E���^�p

	switch (Wpd.flag & _LSR_LVCHK)
	{ //�����̃��x���𒲂ׂĂ���
	case _LSR1_ON:
		for (i = 0; i < OPT.OptLV; i++)
		{
			if (!OPT.AtkDat[i].Opt_LSR1.WTamaCnt)
			{
				//���[�U�[���w�肳�ꂽ���ȏ�ɉ�ʂɑ��݂��Ȃ��悤�ɂ��鏈��
				if (LsrNum[i] < OPT.AtkDat[i].Opt_LSR1.TTama)
				{
					//��ʏ�ɒe�����˂���Ă���t���O�I��
					fJIKI |= _OPTWEP_ON;
					//���˒��̃t���O�𗧂Ă�
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].fTama |= _TON;
					//�e�A�j���[�V������������
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].AnimeCnt = 0;
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].FrameCnt = OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].AnimeFrame[0];
					//���[�U�[�̔��˂��ꂽ���W��������
					//��ʍ��W���Z�b�g
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].Cdn.x = OPT.Jewel[i].Cdn.x + 8;
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].Cdn.y = OPT.Jewel[i].Cdn.y - (OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].Height - 32);
					//�e�̈ړ��v�Z�p�ɉ�ʍ��W���擾
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].dCdn.x = OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].Cdn.x;
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].dCdn.y = OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].Cdn.y;
					//��΍��W���Z�b�g
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].MapCdn.x = MainCdn.x + OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].Cdn.x;
					OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].MapCdn.y = MainCdn.y + OPT.AtkDat[i].Opt_LSR1.tm[LsrNum[i]].Cdn.y;

					//���̏�����TTamaNum���㏑������Ȃ�����
					//�V�����e�𔭎˂ł��Ȃ��悤�ɂ��Ă��܂�
					LsrNum[i] = OPT.AtkDat[i].Opt_LSR1.TTama;
				}
				OPT.AtkDat[i].Opt_LSR1.WTamaCnt = OPT.AtkDat[i].Opt_LSR1.WTama; //���˂܂ł̃J�E���^���ăZ�b�g
			}

			for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_LSR1.TTama; j++)
			{
				//���˒��t���O�̗����Ă��Ȃ��e�͏������Ȃ�
				if (OPT.AtkDat[i].Opt_LSR1.tm[j].fTama & _TON)
				{
					TamaS = TRUE; //���[�U�[�͉�ʓ��ɑ��݂��Ă���

					//���[�U�[���ړ�������
					OPT.AtkDat[i].Opt_LSR1.tm[j].dCdn.x += OPT.AtkDat[i].Opt_LSR1.tm[j].Mov.x;
					OPT.AtkDat[i].Opt_LSR1.tm[j].dCdn.y -= OPT.AtkDat[i].Opt_LSR1.tm[j].Mov.y;
					OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.x = (LONG)OPT.AtkDat[i].Opt_LSR1.tm[j].dCdn.x;
					OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.y = (LONG)OPT.AtkDat[i].Opt_LSR1.tm[j].dCdn.y;
					OPT.AtkDat[i].Opt_LSR1.tm[j].MapCdn.x = MainCdn.x + OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.x;
					OPT.AtkDat[i].Opt_LSR1.tm[j].MapCdn.y = MainCdn.y + OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.y;

					//���[�U�[����ʂ̊O�Ɉړ����Ă��Ȃ����`�F�b�N
					if (OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.x + OPT.AtkDat[i].Opt_LSR1.tm[j].Width < 0 ||
						OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.x > WinXMax ||
						OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.y + OPT.AtkDat[i].Opt_LSR1.tm[j].Height < 0)
					{
						OPT.AtkDat[i].Opt_LSR1.tm[j].fTama &= _TOFF; //�t���O���N���A
						continue;
					}

					// SP�T�[�t�F�X����̓]����`�͈͂��Z�b�g
					OPT.AtkDat[i].Opt_LSR1.tm[j].SrcR = TamSPJIKI[OPT.AtkDat[i].Opt_LSR1.tm[j].AnimePat[OPT.AtkDat[i].Opt_LSR1.tm[j].AnimeCnt]];

					OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.left = OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.x;
					OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.top = OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.y;
					OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.right = OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.x + LSR1.tm[i].Width;
					OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.bottom = OPT.AtkDat[i].Opt_LSR1.tm[j].Cdn.y + LSR1.tm[i].Height;

					//��ʊO�̕����͕`�悵�Ȃ��悤�ɂ��鏈��
					if (OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.left < 0)
					{
						OPT.AtkDat[i].Opt_LSR1.tm[j].SrcR.left = OPT.AtkDat[i].Opt_LSR1.tm[j].SrcR.left - OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.left;
						OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.left = 0;
					}
					else if (OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.right > WinXMax)
					{
						OPT.AtkDat[i].Opt_LSR1.tm[j].SrcR.right = OPT.AtkDat[i].Opt_LSR1.tm[j].SrcR.right - (OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.right - WinXMax);
						OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.right = WinXMax;
					}
					if (OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.top < 0)
					{
						OPT.AtkDat[i].Opt_LSR1.tm[j].SrcR.top = OPT.AtkDat[i].Opt_LSR1.tm[j].SrcR.top - OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.top;
						OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.top = 0;
					}

					//�t���[����1���炵��0�ɂȂ�����A�j���p�^�[����i�߂�
					if (!(OPT.AtkDat[i].Opt_LSR1.tm[j].FrameCnt--))
					{
						OPT.AtkDat[i].Opt_LSR1.tm[j].AnimeCnt = (OPT.AtkDat[i].Opt_LSR1.tm[j].AnimeCnt + 1) % OPT.AtkDat[i].Opt_LSR1.tm[j].Anime; //�A�j���p�^�[����i�߂�
						OPT.AtkDat[i].Opt_LSR1.tm[j].FrameCnt = OPT.AtkDat[i].Opt_LSR1.tm[j].AnimeFrame[OPT.AtkDat[i].Opt_LSR1.tm[j].AnimeCnt];	  //�\������A�j���p�^�[���̃t���[�����Z�b�g
					}
				}
				else if (OPT.AtkDat[i].Opt_LSR1.tm[j].fTama == _TOFF)
				{
					//�Ĕ��ˉ\�ȃ��[�U�[�̔ԍ���ۑ�����
					if (LsrNum[i] == OPT.AtkDat[i].Opt_LSR1.TTama)
					{
						LsrNum[i] = j;
						OPT.AtkDat[i].Opt_LSR1.tm[j].fTama |= _TSET; //���[�U�[���Ăє��˃Z�b�g
					}
					else if (!(fJIKI & _OPTWEP_ON))
					{
						break; //��ʂɒe�����݂��Ȃ���΃��[�v�𔲂���
					}
				}
			}
		}
		break;
	default:
		break;
	}

	switch (Wpd.flag & _MSR_LVCHK)
	{ //�����̃��x���𒲂ׂĂ���
	case _MSR1_ON:
		for (i = 0; i < OPT.OptLV; i++)
		{
			if (!OPT.AtkDat[i].Opt_MSR1.WTamaCnt)
			{
				//�~�T�C�����w�肳�ꂽ���ȏ�ɉ�ʂɑ��݂��Ȃ��悤�ɂ��鏈��
				if (MsrNum[i] < OPT.AtkDat[i].Opt_MSR1.TTama)
				{
					//��ʏ�ɒe�����˂���Ă���t���O�I��
					fJIKI |= _OPTWEP_ON;
					//���˒��̃t���O�𗧂Ă�
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].fTama |= _TON;
					//�e�A�j���[�V������������
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].AnimeCnt = 0;
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].FrameCnt = OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].AnimeFrame[0];
					//�~�T�C���̔��˂��ꂽ���W��������
					//��ʍ��W���Z�b�g
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].Cdn.x = OPT.Jewel[i].Cdn.x + 2;
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].Cdn.y = OPT.Jewel[i].Cdn.y - 10;
					//�e�̈ړ��v�Z�p�ɉ�ʍ��W���擾
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].dCdn.x = OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].Cdn.x;
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].dCdn.y = OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].Cdn.y;
					//��΍��W���Z�b�g
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].MapCdn.x = MainCdn.x + OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].Cdn.x;
					OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].MapCdn.y = MainCdn.y + OPT.AtkDat[i].Opt_MSR1.tm[MsrNum[i]].Cdn.y;

					//�V�����e�𔭎˂ł��Ȃ��悤�ɂ��Ă��܂�
					//���̏�����TTamaNum���㏑������Ȃ�����
					MsrNum[i] = OPT.AtkDat[i].Opt_MSR1.TTama;
				}
				OPT.AtkDat[i].Opt_MSR1.WTamaCnt = OPT.AtkDat[i].Opt_MSR1.WTama; //���˂܂ł̃J�E���^���ăZ�b�g
			}

			for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_MSR1.TTama; j++)
			{
				//���˒��t���O�̗����Ă��Ȃ��e�͏������Ȃ�
				if (OPT.AtkDat[i].Opt_MSR1.tm[j].fTama & _TON)
				{
					TamaS = TRUE; //�~�T�C���͉�ʓ��ɑ��݂��Ă���

					//�~�T�C�����ړ�������
					OPT.AtkDat[i].Opt_MSR1.tm[j].dCdn.x += OPT.AtkDat[i].Opt_MSR1.tm[j].Mov.x;
					OPT.AtkDat[i].Opt_MSR1.tm[j].dCdn.y -= OPT.AtkDat[i].Opt_MSR1.tm[j].Mov.y;
					OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.x = (LONG)OPT.AtkDat[i].Opt_MSR1.tm[j].dCdn.x;
					OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.y = (LONG)OPT.AtkDat[i].Opt_MSR1.tm[j].dCdn.y;
					OPT.AtkDat[i].Opt_MSR1.tm[j].MapCdn.x = MainCdn.x + OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.x;
					OPT.AtkDat[i].Opt_MSR1.tm[j].MapCdn.y = MainCdn.y + OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.y;

					//�~�T�C������ʂ̊O�Ɉړ����Ă��Ȃ����`�F�b�N
					if (OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.x + OPT.AtkDat[i].Opt_MSR1.tm[j].Width < 0 ||
						OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.x > WinXMax ||
						OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.y + OPT.AtkDat[i].Opt_MSR1.tm[j].Height < 0)
					{
						OPT.AtkDat[i].Opt_MSR1.tm[j].fTama &= _TOFF; //�t���O���N���A
						continue;
					}

					// SP�T�[�t�F�X����̓]����`�͈͂��Z�b�g
					OPT.AtkDat[i].Opt_MSR1.tm[j].SrcR = TamSPJIKI[OPT.AtkDat[i].Opt_MSR1.tm[j].AnimePat[OPT.AtkDat[i].Opt_MSR1.tm[j].AnimeCnt]];

					OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.left = OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.x;
					OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.top = OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.y;
					OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.right = OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.x + OPT.AtkDat[i].Opt_MSR1.tm[j].Width;
					OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.bottom = OPT.AtkDat[i].Opt_MSR1.tm[j].Cdn.y + OPT.AtkDat[i].Opt_MSR1.tm[j].Height;

					//��ʊO�̕����͕`�悵�Ȃ��悤�ɂ��鏈��
					if (OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.left < 0)
					{
						OPT.AtkDat[i].Opt_MSR1.tm[j].SrcR.left = OPT.AtkDat[i].Opt_MSR1.tm[j].SrcR.left - OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.left;
						OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.left = 0;
					}
					if (OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.right > WinXMax)
					{
						OPT.AtkDat[i].Opt_MSR1.tm[j].SrcR.right = OPT.AtkDat[i].Opt_MSR1.tm[j].SrcR.right - (OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.right - WinXMax);
						OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.right = WinXMax;
					}
					if (OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.top < 0)
					{
						OPT.AtkDat[i].Opt_MSR1.tm[j].SrcR.top = OPT.AtkDat[i].Opt_MSR1.tm[j].SrcR.top - OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.top;
						OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.top = 0;
					}

					//�t���[����1���炵��0�ɂȂ�����A�j���p�^�[����i�߂�
					if (!(OPT.AtkDat[i].Opt_MSR1.tm[j].FrameCnt--))
					{
						OPT.AtkDat[i].Opt_MSR1.tm[j].AnimeCnt = (OPT.AtkDat[i].Opt_MSR1.tm[j].AnimeCnt + 1) % OPT.AtkDat[i].Opt_MSR1.tm[j].Anime; //�A�j���p�^�[����i�߂�
						OPT.AtkDat[i].Opt_MSR1.tm[j].FrameCnt = OPT.AtkDat[i].Opt_MSR1.tm[j].AnimeFrame[OPT.AtkDat[i].Opt_MSR1.tm[j].AnimeCnt];	  //�\������A�j���p�^�[���̃t���[�����Z�b�g
					}
				}
				else if (OPT.AtkDat[i].Opt_MSR1.tm[j].fTama == _TOFF)
				{
					//�Ĕ��ˉ\�ȃ~�T�C���̔ԍ���ۑ�����
					if (MsrNum[i] == OPT.AtkDat[i].Opt_MSR1.TTama)
					{
						MsrNum[i] = j;
						OPT.AtkDat[i].Opt_MSR1.tm[j].fTama |= _TSET; //�~�T�C�����Ăє��˃Z�b�g
					}
					else if (!(fJIKI & _OPTWEP_ON))
					{
						break; //��ʂɒe�����݂��Ă��Ȃ��ꍇ�̓��[�v�𔲂���
					}
				}
			}
		}
		break;
	default:
		break;
	}

	//�e�����݂��Ă��Ȃ��������C���˃t���O���I�t
	if (!TamaS)
		fJIKI &= _OPTWEP_OFF;
}

//////////////////////////////////////////////////////////////////////
//	���˂����e�ƓG�L�����Ƃ̓����蔻����s�Ȃ�
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptAttackAtari()
{
	short i, j, k;

	//���˂���Ă��郌�[�U�[�ƓG�L�����̓����蔻��
	switch (Wpd.flag & _LSR_LVCHK)
	{
	case _LSR1_ON:
		for (i = 0; i < OPT.OptLV; i++)
		{ //���ݑ������Ă���I�v�V�����̐��������[�v
			for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_LSR1.TTama; j++)
			{
				//�����˂̒e�Ƃ͓����蔻������Ȃ�
				if (OPT.AtkDat[i].Opt_LSR1.tm[j].fTama & _TON)
				{
					if (!(Atari(OPT.AtkDat[i].Opt_LSR1.tm[j].DesR)))
					{
						JikiTekiDmg(_LSR1DMG); //�G�L�����Ƀ_���[�W
					}

					//�j��\�Ȓe�Ƃ̓����蔻��
					if (SP_Tamas3)
					{
						for (k = 0; (DWORD)k < SP_Tamas3->TTama; k++)
						{
							if (SP_Tamas3->tm3[k].fTama & _TON)
							{
								if (!(Atari(OPT.AtkDat[i].Opt_LSR1.tm[j].DesR, SP_Tamas3->tm3[k].DesR)))
								{
									JikiTamas3Dmg(_LSR1DMG, k);
								}
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
	switch (Wpd.flag & _MSR_LVCHK)
	{
	case _MSR1_ON:
		for (i = 0; i < OPT.OptLV; i++)
		{ //���ݑ������Ă���I�v�V�����̐��������[�v
			for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_MSR1.TTama; j++)
			{
				//�����˂̒e�Ƃ͓����蔻������Ȃ�
				if (OPT.AtkDat[i].Opt_MSR1.tm[j].fTama & _TON)
				{
					if (!(Atari(OPT.AtkDat[i].Opt_MSR1.tm[j].DesR)))
					{
						//�~�T�C�������ł�����
						OPT.AtkDat[i].Opt_MSR1.tm[j].fTama &= _TOFF; //�t���O���N���A
						JikiTekiDmg(_MSR1DMG);						 //�G�L�����Ƀ_���[�W
					}

					//�j��\�Ȓe�Ƃ̓����蔻��
					if (SP_Tamas3)
					{
						for (k = 0; (DWORD)k < SP_Tamas3->TTama; k++)
						{
							if (SP_Tamas3->tm3[k].fTama & _TON)
							{
								if (!(Atari(OPT.AtkDat[i].Opt_MSR1.tm[j].DesR, SP_Tamas3->tm3[k].DesR)))
								{
									//�~�T�C�������ł�����
									OPT.AtkDat[i].Opt_MSR1.tm[j].fTama &= _TOFF; //�t���O���N���A
									JikiTamas3Dmg(_MSR1DMG, k);
								}
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
//	�I�v�V������`�悷��
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptRend()
{
	for (short i = 0; i < OPT.OptLV; i++)
	{
		//�I�v�V�����̕`��
		lpDDSBack->BltFast(OPT.Jewel[i].DesR.left, OPT.Jewel[i].DesR.top,
						   lpDDS_JIKI, &OPT.Jewel[i].SrcR,
						   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}
}

//////////////////////////////////////////////////////////////////////
//	�I�v�V�����̔��˂����e�̕`��
//////////////////////////////////////////////////////////////////////
void DrawJIKI::OptWepRend()
{
	short i, j;

	//���[�U�[�̕`��
	switch (Wpd.flag & _LSR_LVCHK)
	{
	case _LSR1_ON: //���[�U�[�P
		for (i = 0; i < OPT.OptLV; i++)
		{
			for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_LSR1.TTama; j++)
			{
				//���˒��t���O�̗����Ă��Ȃ��e�͕`�悵�Ȃ�
				if (OPT.AtkDat[i].Opt_LSR1.tm[j].fTama & _TON)
				{
					lpDDSBack->BltFast(OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.left, OPT.AtkDat[i].Opt_LSR1.tm[j].DesR.top,
									   lpDDS_JIKI, &OPT.AtkDat[i].Opt_LSR1.tm[j].SrcR,
									   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
				}
			}
		}
		break;
	default:
		break;
	}
	//�~�T�C���̕`��
	switch (Wpd.flag & _MSR_LVCHK)
	{
	case _MSR1_ON: //�~�T�C���P
		for (i = 0; i < OPT.OptLV; i++)
		{
			for (j = 0; (DWORD)j < OPT.AtkDat[i].Opt_MSR1.TTama; j++)
			{
				//���˒��t���O�̗����Ă��Ȃ��e�͕`�悵�Ȃ�
				if (OPT.AtkDat[i].Opt_MSR1.tm[j].fTama & _TON)
				{
					lpDDSBack->BltFast(OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.left, OPT.AtkDat[i].Opt_MSR1.tm[j].DesR.top,
									   lpDDS_JIKI, &OPT.AtkDat[i].Opt_MSR1.tm[j].SrcR,
									   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
				}
			}
		}
		break;
	default:
		break;
	}
}