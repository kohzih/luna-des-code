/*=============================================================================
	�z�[�~���O�~�T�C���̔��ˁA�����蔻��A�`�擙�Ɋւ���\�[�X�t�@�C��
==============================================================================*/
#include <windows.h>
#include <math.h>
#include <ddraw.h>

#include "Macros.h"
#include "Draw.h"
#include "Struct.h"
#include "DrawJIKI.h"
#include "DrawJIKIDataEXT.h"
#include "DrawMacros.h"

extern LPDIRECTDRAWSURFACE7 lpDDSBack; //�o�b�N�T�[�t�F�X

//////////////////////////////////////////////////////////////////////
//	�z�[�~���O�~�T�C���I�u�W�F�N�g�̏�����
//////////////////////////////////////////////////////////////////////
BOOL DrawJIKI::InitHmm()
{
	//���E���甭�˂����~�T�C���̃��b�N�I���󋵂������t���O�̏�����
	HMMDAT.fHMM[0] = 0;
	HMMDAT.fHMM[1] = 0;

	//////////////////////////////////////////////////////////////////////
	//--�z�[�~���O�~�T�C���̃X�e�[�^�X���Ǘ�����\���̂̏�����------------

	//�e�X�e�[�^�X�\���̂ɒl���Z�b�g���Ēe�����m�ۂ����A�e�I�u�W�F�N�g��������
	HMM1.TTama = 30;			//��ʏ�ɑ��݂ł���e�̐�
	HMM1.WTama = 10;			//�e�𔭎˂���Ԋu
	HMM1.WTamaCnt = HMM1.WTama; //�e���˂܂ł̃J�E���^

	HMTAMA tama; // HMTAMA�\���̂ɑO�����Ēl���Z�b�g���ď���������

	tama.fTama = _TOFF;			//�e�̏�����Ԃ�ݒ�
	tama.STama = 2;				//�ǔ����Ă���G�L�����̍��W��V�����擾����Ԋu
	tama.STamaCnt = tama.STama; //�V�����擾����Ԋu�̃J�E���^
	tama.LockTekiNum = 0;		//�e�̃^�[�Q�b�g�̓G�L�����̔ԍ��̏�����
	tama.Kakudo = 0;			//���݂̒e�̊p�x�̏�����
	tama.TekiKakudo = 0;		//�G�L�����ƒe�̈ʒu�Ƃ̊p�x�̏�����
	tama.Width = 16;			//�e�̉���
	tama.Height = 16;			//�e�̏c��
	tama.Mov.x = 0.0;			//�e�̈ړ����x��������
	tama.Mov.y = 0.0;
	tama.dCdn.x = 0; //�e�̉�ʍ��W�v�Z�p
	tama.dCdn.y = 0;
	tama.Cdn.x = 0; //�e�̌��݂̉�ʍ��W��0�ŏ�����
	tama.Cdn.y = 0;
	tama.MapCdn.x = 0; //�e�̌��݂̐�΍��W��0�ŏ�����
	tama.MapCdn.y = 0;
	//�e�A�j���[�V�����p�^�[���z��̏�����
	memcpy(&tama.AnimePat[0], &HmmPatJIKI[0], sizeof(BYTE) * 24);
	tama.SrcR.left = 0; //�]������`��0�ŏ�����
	tama.SrcR.top = 0;
	tama.SrcR.right = 0;
	tama.SrcR.bottom = 0;
	tama.DesR = tama.SrcR; //�]�����`��0�ŏ�����

	//���˂ł���e����TAMA�\���̂��m��
	HMM1.tm = new HMTAMA[HMM1.TTama];
	if (!HMM1.tm)
		return FALSE;
	//�m�ۂ����̈�̐擪�A�h���X���R�s�[
	char *pTama;
	pTama = (char *)HMM1.tm;
	//�e�I�u�W�F�N�g���m�ۂ������A������
	for (int i = 0; (DWORD)i < HMM1.TTama; i++)
	{
		memcpy(pTama, &tama, sizeof(HMTAMA)); //�m�ۂ����̈�̐擪����R�s�[
		pTama += sizeof(HMTAMA);			  //���ɃR�s�[����Ƃ���܂ŃA�h���X��i�߂�
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	���I�Ɋm�ۂ��郁�����̈�ւ̃|�C���^��NULL�ŏ�����
//////////////////////////////////////////////////////////////////////
void DrawJIKI::InitHmmTm()
{
	HMM1.tm = NULL;
}

//////////////////////////////////////////////////////////////////////
//	�m�ۂ����������̈�����
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmRelease()
{
	RELEASE_dels(HMM1.tm);
}

//////////////////////////////////////////////////////////////////////
//	�z�[�~���O�~�T�C�����˂܂ł̃J�E���g��i�߂�
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmCnt()
{
	//���@�Ɉ�ԋ߂��G�L�����̔ԍ��ƍ��W�̎擾�J�n
	HMMDAT.fHMM[0] |= _LOCKHMM_ON;
	HMMDAT.fHMM[1] |= _LOCKHMM_ON;
	HMMDAT.fHMM[0] &= _LOCKST_OFF;
	HMMDAT.fHMM[1] &= _LOCKST_OFF;

	//�z�[�~���O�~�T�C���̌��݂̃��x�����擾����
	switch (Wpd.flag & _HMM_LVCHK)
	{
	case _HMM1_ON:
		HMM1.WTamaCnt--; //���˂܂ł̃J�E���g�_�E��
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
//	�z�[�~���O�~�T�C�����˂܂ł̃J�E���g�����Z�b�g������
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmCntReset()
{
	//���@�Ɉ�ԋ߂��G�L�����̔ԍ��ƍ��W�̎擾����
	HMMDAT.fHMM[0] &= _LOCKHMM_OFF;
	HMMDAT.fHMM[1] &= _LOCKHMM_OFF;

	//�z�[�~���O�~�T�C�����˂܂ł̃J�E���g�����Z�b�g
	switch (Wpd.flag & _HMM_LVCHK)
	{
	case _HMM1_ON:
		HMM1.WTamaCnt = HMM1.WTama; //���˂܂ł̃J�E���g���Z�b�g
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
//	�z�[�~���O�~�T�C����S�Ĕ��ˑҋ@��Ԃɖ߂�
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmReset()
{
	HmmCntReset(); //���˂܂ł̃J�E���^�����Z�b�g

	short i;

	//�S�Ẵz�[�~���O�~�T�C���̒e�t���O�𔭎ˑҋ@��ԂɃZ�b�g
	switch (Wpd.flag & _HMM_LVCHK)
	{
	case _HMM1_ON:
		for (i = 0; (DWORD)i < HMM1.TTama; i++)
		{
			HMM1.tm[i].fTama = _TOFF; //�t���O���N���A
		}
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
//	���b�N�I�����邽�߂Ɏ��@�Ɉ�ԋ߂������ɂ���G�L�����̏����擾����
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmGetTekiInfo()
{
	short hsX[2] = {-20, 40}; //�z�[�~���O�~�T�C���𔭎˂��鎩�@���E�̖C��̈ʒu
	LONG x = 0, x1 = 0, y = 0, y1 = 0;

	for (short i = 0; i < 2; i++)
	{
		//���b�N�I�����J�n����Ă��邩�`�F�b�N
		if (HMMDAT.fHMM[i] & _LOCKHMM_ON)
		{
			//��ԍŏ��ɔ�r�ׂ̈ɕK�v�ɂȂ�G�L�����̏����擾
			if (!(HMMDAT.fHMM[i] & _LOCKST_ON))
			{
				HMMDAT.fHMM[i] |= _LOCKST_ON;
				HMMDAT.TekiNum[i] = SP_Num;
				HMMDAT.TekiCdn[i] = SP_Cdn;
				continue;
			}
			//���݊i�[����Ă����Ԏ��@�ɋ߂��G���W�Ǝ��̓G���W�̒l���r���邽�߂�
			// X���W��Y���W�̘a�����߂�
			x = HMMDAT.TekiCdn[i].x - (Cdn.x + hsX[i]) < 0 ? -(HMMDAT.TekiCdn[i].x - (Cdn.x + hsX[i])) : HMMDAT.TekiCdn[i].x - (Cdn.x + hsX[i]);
			y = HMMDAT.TekiCdn[i].y - Cdn.y < 0 ? -(HMMDAT.TekiCdn[i].y - Cdn.y) : HMMDAT.TekiCdn[i].y - Cdn.y;
			x1 = SP_Cdn.x - (Cdn.x + hsX[i]) < 0 ? -(SP_Cdn.x - (Cdn.x + hsX[i])) : SP_Cdn.x - (Cdn.x + hsX[i]);
			y1 = SP_Cdn.y - Cdn.y < 0 ? -(SP_Cdn.y - Cdn.y) : SP_Cdn.y - Cdn.y;
			//�a�̔�r�A���̓G���W�̒l�̂ق������@�ɋ߂���ΐV�����ۑ�����
			if ((x + y) > (x1 + y1))
			{
				HMMDAT.TekiNum[i] = SP_Num;
				HMMDAT.TekiCdn[i] = SP_Cdn;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	���ɔ��˂��ꂽ�z�[�~���O�~�T�C���̈ړ��ʂ����߂�
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmMovCnt()
{
	double X = 0, Y = 0;
	double x = 0, y = 0;

	for (int i = 0; (DWORD)i < HMM1.TTama; i++)
	{
		//���b�N�I�����Ă���G�L������T��
		if (HMM1.tm[i].LockTekiNum == SP_Num && HMM1.tm[i].STamaCnt == 0)
		{
			//�V���ɓG�L�����Ƃ̈ʒu�֌W�𒲂ג���(�L�����̒��S�_�Ŕ�r)
			X = (SP_Cdn.x + (SP_Width >> 1)) - (HMM1.tm[i].Cdn.x + (HMM1.tm[i].Width >> 1));
			Y = (SP_Cdn.y + (SP_Height >> 1)) - (HMM1.tm[i].Cdn.y + (HMM1.tm[i].Height >> 1));
			if (X == 0)
			{
				if (Y < 0)
					HMM1.tm[i].TekiKakudo = 90;
				else if (Y > 0)
					HMM1.tm[i].TekiKakudo = 270;
			}
			else if (Y == 0)
			{
				if (X < 0)
					HMM1.tm[i].TekiKakudo = 180;
				else if (X > 0)
					HMM1.tm[i].TekiKakudo = 0;
			}
			else
			{
				//�G�L�����ƒe�Ƃ̊p�x���v�Z
				x = X < 0 ? -X : X;
				y = Y < 0 ? -Y : Y;
				HMM1.tm[i].TekiKakudo = TanKaku(y / x); //�^���W�F���g�l���p�x�ɕϊ�
				//���܂����p�x�����ꂼ��̏ی��ɑΉ������p�x�ɕϊ�(��1�ی��͂��̂܂�)
				if (X < 0)
				{
					if (Y < 0)
						HMM1.tm[i].TekiKakudo = 180 - HMM1.tm[i].TekiKakudo; //��2�ی�
					else if (Y > 0)
						HMM1.tm[i].TekiKakudo = 180 + HMM1.tm[i].TekiKakudo; //��3�ی�
				}
				else if (X > 0)
				{
					if (Y > 0)
						HMM1.tm[i].TekiKakudo = 360 - HMM1.tm[i].TekiKakudo; //��4�ی�
				}
			}
			//�e�̈ړ��ʂ��v�Z(�����̋߂������ŉ�])
			if (HMM1.tm[i].Kakudo > HMM1.tm[i].TekiKakudo)
			{
				if (HMM1.tm[i].Kakudo - HMM1.tm[i].TekiKakudo > 180)
				{ //�����v����
					HMM1.tm[i].Kakudo += 15;
				}
				else
				{ //���v����
					HMM1.tm[i].Kakudo -= 15;
				}
			}
			else if (HMM1.tm[i].Kakudo < HMM1.tm[i].TekiKakudo)
			{
				if (HMM1.tm[i].TekiKakudo - HMM1.tm[i].Kakudo > 180)
				{ //���v����
					HMM1.tm[i].Kakudo -= 15;
				}
				else
				{ //�����v����
					HMM1.tm[i].Kakudo += 15;
				}
			}
			//�p�x�̕␳(0-345�x�Ɍ��肷�鏈��)
			if (HMM1.tm[i].Kakudo == 360)
				HMM1.tm[i].Kakudo = 0;
			else if (HMM1.tm[i].Kakudo > 360)
				HMM1.tm[i].Kakudo = 15;
			else if (HMM1.tm[i].Kakudo < 0)
				HMM1.tm[i].Kakudo = 345;

			//�ړ��ʂ��v�Z
			HMM1.tm[i].Mov.x = _HMM_MOV * Cos[HMM1.tm[i].Kakudo];
			HMM1.tm[i].Mov.y = -(_HMM_MOV * Sin[HMM1.tm[i].Kakudo]);
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	�z�[�~���O�~�T�C���ƓG�L�����Ƃ̓����蔻��
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmAtari()
{
	short i, j;

	//���˂���Ă���z�[�~���O�~�T�C���ƓG�L�����̓����蔻��
	switch (Wpd.flag & _HMM_LVCHK)
	{
	case _HMM1_ON:
		for (i = 0; (DWORD)i < HMM1.TTama; i++)
		{
			//�����˂̒e�Ƃ͓����蔻������Ȃ�
			if (HMM1.tm[i].fTama & _TON)
			{
				//�G�L����
				if (!(Atari(HMM1.tm[i].DesR)))
				{
					//�z�[�~���O�~�T�C�������ł�����
					HMM1.tm[i].fTama &= _TOFF; //�t���O���N���A
					JikiTekiDmg(_HMM1DMG);	   //�G�L�����Ƀ_���[�W
				}

				//�j��\�Ȓe�Ƃ̓����蔻��
				if (SP_Tamas3)
				{
					for (j = 0; (DWORD)j < SP_Tamas3->TTama; j++)
					{
						if (SP_Tamas3->tm3[j].fTama & _TON)
						{
							if (!(Atari(HMM1.tm[i].DesR, SP_Tamas3->tm3[j].DesR)))
							{
								//�z�[�~���O�~�T�C�������ł�����
								HMM1.tm[i].fTama &= _TOFF; //�t���O���N���A
								JikiTamas3Dmg(_HMM1DMG, j);
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
//	�z�[�~���O�~�T�C�����˂ƈړ��̏���
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmAttack()
{
	//��ʂɒe�����݂��Ă��邩���Ă��Ȃ������ׂ�t���O
	bool TamaS = FALSE;

	//���˂ł����ԂɂȂ��Ă���z�[�~���O�~�T�C���̔ԍ��ۑ��p
	static DWORD TTamaNum1 = HMM1.TTama, TTamaNum2 = HMM1.TTama;

	short i;

	switch (Wpd.flag & _HMM_LVCHK)
	{ //�����̃��x���𒲂ׂĂ���
	case _HMM1_ON:
		if (!HMM1.WTamaCnt)
		{
			//�z�[�~���O�~�T�C�����w�肳�ꂽ���ȏ�ɉ�ʂɑ��݂��Ȃ��悤�ɂ��鏈��
			if (TTamaNum1 < HMM1.TTama && TTamaNum2 < HMM1.TTama)
			{
				//��ʏ�ɒe�����˂���Ă���t���O�I��
				fJIKI |= _SUBHMM_ON;
				//���˒��̃t���O�𗧂Ă�
				HMM1.tm[TTamaNum1].fTama |= _TON;
				HMM1.tm[TTamaNum2].fTama |= _TON;
				//�ǔ��J�E���^��������
				HMM1.tm[TTamaNum1].STamaCnt = HMM1.tm[TTamaNum1].STama;
				HMM1.tm[TTamaNum2].STamaCnt = HMM1.tm[TTamaNum2].STama;
				//���b�N�I������ׂ��G�L�����̔ԍ��̏�����
				HMM1.tm[TTamaNum1].LockTekiNum = HMMDAT.TekiNum[0];
				HMM1.tm[TTamaNum2].LockTekiNum = HMMDAT.TekiNum[1];
				//���˂��ꂽ�p�x�ƈړ��ʂ̏�����
				HMM1.tm[TTamaNum1].Kakudo = 135;
				HMM1.tm[TTamaNum1].Mov.x = _HMM_MOV * Cos[HMM1.tm[TTamaNum1].Kakudo];
				HMM1.tm[TTamaNum1].Mov.y = -(_HMM_MOV * Sin[HMM1.tm[TTamaNum1].Kakudo]);
				HMM1.tm[TTamaNum2].Kakudo = 45;
				HMM1.tm[TTamaNum2].Mov.x = _HMM_MOV * Cos[HMM1.tm[TTamaNum2].Kakudo];
				HMM1.tm[TTamaNum2].Mov.y = -(_HMM_MOV * Sin[HMM1.tm[TTamaNum2].Kakudo]);
				//���˂��ꂽ���W��������
				//��ʍ��W���Z�b�g
				HMM1.tm[TTamaNum1].Cdn.x = Cdn.x - (HMM1.tm[TTamaNum1].Width - 2);
				HMM1.tm[TTamaNum1].Cdn.y = Cdn.y + HMM1.tm[TTamaNum1].Height;
				HMM1.tm[TTamaNum2].Cdn.x = (Cdn.x + _JIKI_W) - 2;
				HMM1.tm[TTamaNum2].Cdn.y = Cdn.y + HMM1.tm[TTamaNum2].Height;
				//�e�̈ړ��v�Z�p�ɉ�ʍ��W���擾
				HMM1.tm[TTamaNum1].dCdn.x = HMM1.tm[TTamaNum1].Cdn.x;
				HMM1.tm[TTamaNum1].dCdn.y = HMM1.tm[TTamaNum1].Cdn.y;
				HMM1.tm[TTamaNum2].dCdn.x = HMM1.tm[TTamaNum2].Cdn.x;
				HMM1.tm[TTamaNum2].dCdn.y = HMM1.tm[TTamaNum2].Cdn.y;
				//��΍��W���Z�b�g
				HMM1.tm[TTamaNum1].MapCdn.x = MainCdn.x + HMM1.tm[TTamaNum1].Cdn.x;
				HMM1.tm[TTamaNum1].MapCdn.y = MainCdn.y + HMM1.tm[TTamaNum1].Cdn.y;
				HMM1.tm[TTamaNum2].MapCdn.x = MainCdn.x + HMM1.tm[TTamaNum2].Cdn.x;
				HMM1.tm[TTamaNum2].MapCdn.y = MainCdn.y + HMM1.tm[TTamaNum2].Cdn.y;

				TTamaNum1 = HMM1.TTama; // HMM1.TTama�ŏ�����
				TTamaNum2 = HMM1.TTama; //���̏�����TTamaNum���㏑������Ȃ�����
										//�V�����e�𔭎˂ł��Ȃ��悤�ɂ��Ă��܂�
			}
			HMM1.WTamaCnt = HMM1.WTama; //���˂܂ł̃J�E���^���ăZ�b�g
		}

		for (i = 0; (DWORD)i < HMM1.TTama; i++)
		{
			//���˒��t���O�̗����Ă��Ȃ��e�͏������Ȃ�
			if (HMM1.tm[i].fTama & _TON)
			{
				TamaS = TRUE; //�z�[�~���O�~�T�C���͉�ʓ��ɑ��݂��Ă���

				// STamaCnt=0�̎��̏�����HmmMovCnt()�Ŏg�p���Ă��܂��B
				if (!(HMM1.tm[i].STamaCnt))
					HMM1.tm[i].STamaCnt = HMM1.tm[i].STama; //�J�E���^���ăZ�b�g

				HMM1.tm[i].STamaCnt--; //�G�L������ǔ����鏈���ւ̃J�E���g��i�߂�

				//�z�[�~���O�~�T�C�����ړ�������
				HMM1.tm[i].dCdn.x += HMM1.tm[i].Mov.x;
				HMM1.tm[i].dCdn.y += HMM1.tm[i].Mov.y;
				HMM1.tm[i].Cdn.x = (LONG)HMM1.tm[i].dCdn.x;
				HMM1.tm[i].Cdn.y = (LONG)HMM1.tm[i].dCdn.y;
				HMM1.tm[i].MapCdn.x = MainCdn.x + HMM1.tm[i].Cdn.x;
				HMM1.tm[i].MapCdn.y = MainCdn.y + HMM1.tm[i].Cdn.y;

				//�z�[�~���O�~�T�C������ʂ̊O�Ɉړ����Ă��Ȃ����`�F�b�N
				if (HMM1.tm[i].Cdn.x + HMM1.tm[i].Width < 0 ||
					HMM1.tm[i].Cdn.x > WinXMax ||
					HMM1.tm[i].Cdn.y + HMM1.tm[i].Height < 0 ||
					HMM1.tm[i].Cdn.y > WinYMax)
				{
					HMM1.tm[i].fTama = _TOFF; //�t���O���N���A
					continue;
				}

				//���݂̒e�p�x�ɑΉ������T�[�t�F�X����̓]����`�͈͂��Z�b�g
				HMM1.tm[i].SrcR = TamSPJIKI[HMM1.tm[i].AnimePat[HMM1.tm[i].Kakudo / 15]];

				HMM1.tm[i].DesR.left = HMM1.tm[i].Cdn.x;
				HMM1.tm[i].DesR.top = HMM1.tm[i].Cdn.y;
				HMM1.tm[i].DesR.right = HMM1.tm[i].Cdn.x + HMM1.tm[i].Width;
				HMM1.tm[i].DesR.bottom = HMM1.tm[i].Cdn.y + HMM1.tm[i].Height;

				//��ʊO�̕����͕`�悵�Ȃ��悤�ɂ��鏈��
				if (HMM1.tm[i].DesR.left < 0)
				{
					HMM1.tm[i].SrcR.left = HMM1.tm[i].SrcR.left - HMM1.tm[i].DesR.left;
					HMM1.tm[i].DesR.left = 0;
				}
				else if (HMM1.tm[i].DesR.right > WinXMax)
				{
					HMM1.tm[i].SrcR.right = HMM1.tm[i].SrcR.right - (HMM1.tm[i].DesR.right - WinXMax);
					HMM1.tm[i].DesR.right = WinXMax;
				}
				if (HMM1.tm[i].DesR.top < 0)
				{
					HMM1.tm[i].SrcR.top = HMM1.tm[i].SrcR.top - HMM1.tm[i].DesR.top;
					HMM1.tm[i].DesR.top = 0;
				}
				else if (HMM1.tm[i].DesR.bottom > WinYMax)
				{
					HMM1.tm[i].SrcR.bottom = HMM1.tm[i].SrcR.bottom - (HMM1.tm[i].DesR.bottom - WinYMax);
					HMM1.tm[i].DesR.bottom = WinYMax;
				}
			}
			else if (HMM1.tm[i].fTama == _TOFF)
			{
				//�Ĕ��ˉ\�ȃz�[�~���O�~�T�C���̔ԍ���ۑ�����
				if (TTamaNum1 == HMM1.TTama)
				{
					TTamaNum1 = i;
					HMM1.tm[i].fTama |= _TSET; //�z�[�~���O�~�T�C�����Ăє��˃Z�b�g
				}
				else if (TTamaNum2 == HMM1.TTama)
				{
					TTamaNum2 = i;
					HMM1.tm[i].fTama |= _TSET;
				}
				else if (!(fJIKI & _SUBHMM_ON))
				{
					break; //��ʂɒe�����݂��Ă��Ȃ���΃��[�v�𔲂���
				}
			}
		}
		break;
	default:
		break;
	}
	//�e�����݂��Ă��Ȃ�������z�[�~���O�~�T�C�����˃t���O���I�t
	if (!TamaS)
		fJIKI &= _SUBHMM_OFF;
}

//////////////////////////////////////////////////////////////////////
//	�z�[�~���O�~�T�C����`�悷��
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HmmRend()
{
	short i;
	//�z�[�~���O�~�T�C���̕`��
	switch (Wpd.flag & _HMM_LVCHK)
	{
	case _HMM1_ON: //�z�[�~���O�~�T�C���P
		for (i = 0; (DWORD)i < HMM1.TTama; i++)
		{
			//���˒��t���O�̗����Ă��Ȃ��e�͕`�悵�Ȃ�
			if (HMM1.tm[i].fTama & _TON)
			{
				lpDDSBack->BltFast(HMM1.tm[i].DesR.left, HMM1.tm[i].DesR.top,
								   lpDDS_JIKI, &HMM1.tm[i].SrcR,
								   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			}
		}
		break;
	default:
		break;
	}
}