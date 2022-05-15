/*=============================================================================
	�ԉ΂̔��ˁA�����蔻��A�`�擙�Ɋւ���\�[�X�t�@�C��
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
//	�ԉ΃I�u�W�F�N�g�̏�����
//////////////////////////////////////////////////////////////////////
BOOL DrawJIKI::InitHnb()
{
	//----------------�ԉ΃X�e�[�^�X���Ǘ�����\���̂̏�����------------------

	//�e�X�e�[�^�X�\���̂ɒl���Z�b�g���Ēe�����m�ۂ����A�e�I�u�W�F�N�g��������
	HNB1.TTama[0] = 120;		//��ʏ�ɑ��݂ł��鏬��
	HNB1.TTama[1] = 2;			//��ʏ�ɑ��݂ł�����
	HNB1.WTama = 30;			//�e�𔭎˂���Ԋu
	HNB1.WTamaCnt = HMM1.WTama; //�e���˂܂ł̃J�E���^

	TAMA2 tama2; // TAMA�\���̂ɑO�����Ēl���Z�b�g���ď���������

	//-----------------���ʂ̏�����--------------------------------------------

	tama2.fTama = _TOFF; //�e�̏�����Ԃ�ݒ�
	tama2.Width = 16;	 //�e�̉���
	tama2.Height = 16;	 //�e�̏c��
	tama2.Kakudo = 0;	 //�e�̔��ˊp�x
	tama2.Time = 0;		 //�e�����˂���Ă���̌o�ߎ���
	tama2.Mov.x = 0.0;	 //�e�̈ړ����x��������
	tama2.Mov.y = 0.0;
	tama2.dCdn.x = 0; //�e�̉�ʍ��W�v�Z�p
	tama2.dCdn.y = 0;
	tama2.Cdn.x = 0; //�e�̌��݂̉�ʍ��W��0�ŏ�����
	tama2.Cdn.y = 0;
	tama2.MapCdn.x = 0; //�e�̌��݂̐�΍��W��0�ŏ�����
	tama2.MapCdn.y = 0;
	tama2.Anime = TamAmeJIKI[4]; //�e�A�j���[�V�����p�^�[�����̏�����
	//�e�A�j���[�V�����p�^�[���z��̏�����
	memcpy(&tama2.AnimePat[0], &TamPatJIKI[4][0], sizeof(BYTE) * 10);
	//�e�A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	memcpy(&tama2.AnimeFrame[0], &TamFrameJIKI[4][0], sizeof(BYTE) * 10);
	tama2.AnimeCnt = 0;					  //�ŏ��ɕ\������A�j���[�V�����p�^�[����������
	tama2.FrameCnt = tama2.AnimeFrame[0]; //�ŏ��̃A�j���[�V�����t���[���J�E���g��������
	tama2.SrcR.left = 0;				  //�]������`��0�ŏ�����
	tama2.SrcR.top = 0;
	tama2.SrcR.right = 0;
	tama2.SrcR.bottom = 0;
	tama2.DesR = tama2.SrcR; //�]�����`��0�ŏ�����

	char *pTama;
	short i;

	//���˂ł���e����TAMA�\���̂��m��
	HNB1.tm2[0] = new TAMA2[HNB1.TTama[0]];
	if (!HNB1.tm2[0])
		return FALSE;
	//�m�ۂ����̈�̐擪�A�h���X���R�s�[
	pTama = (char *)HNB1.tm2[0];
	//�e�I�u�W�F�N�g���m�ۂ������A������
	for (i = 0; (DWORD)i < HNB1.TTama[0]; i++)
	{
		memcpy(pTama, &tama2, sizeof(TAMA2)); //�m�ۂ����̈�̐擪����R�s�[
		pTama += sizeof(TAMA2);				  //���ɃR�s�[����Ƃ���܂ŃA�h���X��i�߂�
	}

	//-----------------��ʂ̏�����--------------------------------------------

	tama2.Width = 32;			 //�e�̉���
	tama2.Height = 32;			 //�e�̏c��
	tama2.Anime = TamAmeJIKI[3]; //�e�A�j���[�V�����p�^�[�����̏�����
	//�e�A�j���[�V�����p�^�[���z��̏�����
	memcpy(&tama2.AnimePat[0], &TamPatJIKI[3][0], sizeof(BYTE) * 10);
	//�e�A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	memcpy(&tama2.AnimeFrame[0], &TamFrameJIKI[3][0], sizeof(BYTE) * 10);

	//���˂ł���e����TAMA�\���̂��m��
	HNB1.tm2[1] = new TAMA2[HNB1.TTama[1]];
	if (!HNB1.tm2[1])
		return FALSE;
	//�m�ۂ����̈�̐擪�A�h���X���R�s�[
	pTama = (char *)HNB1.tm2[1];
	//�e�I�u�W�F�N�g���m�ۂ������A������
	for (i = 0; (DWORD)i < HNB1.TTama[1]; i++)
	{
		memcpy(pTama, &tama2, sizeof(TAMA2)); //�m�ۂ����̈�̐擪����R�s�[
		pTama += sizeof(TAMA2);				  //���ɃR�s�[����Ƃ���܂ŃA�h���X��i�߂�
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	���I�Ɋm�ۂ��郁�����̈�ւ̃|�C���^��NULL�ŏ�����
//////////////////////////////////////////////////////////////////////
void DrawJIKI::InitHnbTm()
{
	HNB1.tm2[0] = NULL;
	HNB1.tm2[1] = NULL;
}

//////////////////////////////////////////////////////////////////////
//	�m�ۂ����������̈�����
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HnbRelease()
{
	RELEASE_dels(HNB1.tm2[0]);
	RELEASE_dels(HNB1.tm2[1]);
}

//////////////////////////////////////////////////////////////////////
//	�ԉΔ��˂܂ł̃J�E���g��i�߂�
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HnbCnt()
{
	//�ԉ΂̌��݂̃��x�����擾����
	switch (Wpd.flag & _HNB_LVCHK)
	{
	case _HNB1_ON:
		HNB1.WTamaCnt--; //���˂܂ł̃J�E���g�_�E��
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
//	�ԉΔ��˂܂ł̃J�E���g�����Z�b�g������
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HnbCntReset()
{
	//�ԉ΂̌��݂̃��x�����擾����
	switch (Wpd.flag & _HNB_LVCHK)
	{
	case _HNB1_ON:
		HNB1.WTamaCnt = HNB1.WTama; //���˂܂ł̃J�E���g���Z�b�g
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
//	�ԉ΂�S�Ĕ��ˑҋ@��Ԃɖ߂�
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HnbReset()
{
	HnbCntReset(); //���˂܂ł̃J�E���^�����Z�b�g

	short i, j;

	//�S�Ẳԉ΂̒e�t���O�𔭎ˑҋ@��ԂɃZ�b�g
	switch (Wpd.flag & _HNB_LVCHK)
	{
	case _HNB1_ON:
		for (i = 0; i < 2; i++)
		{
			for (j = 0; (DWORD)j < HNB1.TTama[i]; j++)
			{
				HNB1.tm2[i][j].fTama = _TOFF; //�t���O���N���A
			}
		}
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
//	�ԉΔ��˂ƈړ��̏���
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HnbAttack()
{
	//���˂ł����ԂɂȂ��Ă��鏬�ʉԉ΂̔ԍ��ۑ��p
	static DWORD KoDama[60] = {
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0],
		HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0], HNB1.TTama[0]};
	//���˂ł����ԂɂȂ��Ă����ʉԉ΂̔ԍ��ۑ��p
	static DWORD ODama[2] = {
		HNB1.TTama[1], HNB1.TTama[1]};

	bool ret = FALSE;					//�ԉ΂��w�肳�ꂽ�����ˑҋ@��ԂɂȂ��Ă��邩���ׂ�t���O
	bool TamaS = FALSE;					//��ʂɒe�����݂��Ă��邩���Ă��Ȃ������ׂ�t���O
	short TamaKaku1 = 0, TamaKaku2 = 0; //�������̏��ʂ̊p�x(�E�ƍ�)
	short i, j;

	//------�e�̔��ˏ���-------------------------------------------------------

	switch (Wpd.flag & _HNB_LVCHK)
	{ //�����̃��x���𒲂ׂĂ���
	case _HNB1_ON:
		if (!HNB1.WTamaCnt)
		{
			//�ԉ΂��w�肳�ꂽ�����ˑҋ@��ԂɂȂ��Ă��邩���ׂ�
			for (i = 0; i < 60; i++)
			{
				if (KoDama[i] == HNB1.TTama[0])
					ret = TRUE;
			}
			for (i = 0; i < 2; i++)
			{
				if (ODama[i] == HNB1.TTama[1])
					ret = TRUE;
			}
			//�w�肳�ꂽ�����ˑҋ@��ԂɂȂ��Ă���Δ��ˏ���
			if (!ret)
			{
				//��ʏ�ɒe�����˂���Ă���t���O�I��
				fJIKI |= _SUBHNB_ON;

				//�e�̔��˂��ꂽ���W�Ɗp�x�̏�����
				//����
				for (i = 0; i < 60; i++)
				{
					//���˒��̃t���O�𗧂Ă�
					HNB1.tm2[0][KoDama[i]].fTama |= _TON;
					//�o�ߎ��Ԃ��N���A
					HNB1.tm2[0][KoDama[i]].Time = 0;
					//�e�A�j���[�V������������
					HNB1.tm2[0][KoDama[i]].AnimeCnt = 0;
					HNB1.tm2[0][KoDama[i]].FrameCnt = HNB1.tm2[0][KoDama[i]].AnimeFrame[0];
					//���˂������W�̏�����
					if (i < 30)
					{
						HNB1.tm2[0][KoDama[i]].Cdn.x = Cdn.x - 24;
						HNB1.tm2[0][KoDama[i]].Cdn.y = Cdn.y + 8;
						HNB1.tm2[0][KoDama[i]].Kakudo = 100;
					}
					else if (i >= 30)
					{
						HNB1.tm2[0][KoDama[i]].Cdn.x = Cdn.x + (_JIKI_W + 8);
						HNB1.tm2[0][KoDama[i]].Cdn.y = Cdn.y + 8;
						HNB1.tm2[0][KoDama[i]].Kakudo = 80;
					}
					//�e�̈ړ��v�Z�p�ɉ�ʍ��W���擾
					HNB1.tm2[0][KoDama[i]].dCdn.x = HNB1.tm2[0][KoDama[i]].Cdn.x;
					HNB1.tm2[0][KoDama[i]].dCdn.y = HNB1.tm2[0][KoDama[i]].Cdn.y;
					//��΍��W���Z�b�g
					HNB1.tm2[0][KoDama[i]].MapCdn.x = MainCdn.x + HNB1.tm2[0][KoDama[i]].Cdn.x;
					HNB1.tm2[0][KoDama[i]].MapCdn.y = MainCdn.y + HNB1.tm2[0][KoDama[i]].Cdn.y;

					//���̏�����KoDama���㏑������Ȃ�����V�����e�𔭎˂ł��Ȃ��悤�ɂ��Ă��܂�
					KoDama[i] = HNB1.TTama[0];
				}

				//���
				for (i = 0; i < 2; i++)
				{
					//���˒��̃t���O�𗧂Ă�
					HNB1.tm2[1][ODama[i]].fTama |= _TON;
					//�o�ߎ��Ԃ��N���A
					HNB1.tm2[1][ODama[i]].Time = 0;
					//�e�A�j���[�V������������
					HNB1.tm2[1][ODama[i]].AnimeCnt = 0;
					HNB1.tm2[1][ODama[i]].FrameCnt = HNB1.tm2[1][ODama[i]].AnimeFrame[0];
					//���˂������W��������
					if (i == 0)
					{
						HNB1.tm2[1][ODama[i]].Cdn.x = Cdn.x - HNB1.tm2[1][ODama[0]].Width;
						HNB1.tm2[1][ODama[i]].Cdn.y = Cdn.y;
						HNB1.tm2[1][ODama[i]].Kakudo = 100;
					}
					else if (i == 1)
					{
						HNB1.tm2[1][ODama[i]].Cdn.x = Cdn.x + _JIKI_W;
						HNB1.tm2[1][ODama[i]].Cdn.y = Cdn.y;
						HNB1.tm2[1][ODama[i]].Kakudo = 80;
					}
					HNB1.tm2[1][ODama[i]].dCdn.x = HNB1.tm2[1][ODama[i]].Cdn.x;
					HNB1.tm2[1][ODama[i]].dCdn.y = HNB1.tm2[1][ODama[i]].Cdn.y;
					HNB1.tm2[1][ODama[i]].MapCdn.x = MainCdn.x + HNB1.tm2[1][ODama[i]].Cdn.x;
					HNB1.tm2[1][ODama[i]].MapCdn.y = MainCdn.y + HNB1.tm2[1][ODama[i]].Cdn.y;

					//���̏�����ODama���㏑������Ȃ�����V�����e�𔭎˂ł��Ȃ��悤�ɂ��Ă��܂�
					ODama[i] = HNB1.TTama[1];
				}
			}
			HNB1.WTamaCnt = HNB1.WTama; //���˂܂ł̃J�E���^���ăZ�b�g
		}

		//----��������e�̈ړ�����---------------------------------------------

		//��ʂ̈ړ�����
		for (i = 0; (DWORD)i < HNB1.TTama[1]; i++)
		{
			//���˒��t���O�̗����Ă��Ȃ��e�͏������Ȃ�
			if (HNB1.tm2[1][i].fTama & _TON)
			{
				TamaS = TRUE;		   //�ԉ΂͉�ʓ��ɑ��݂��Ă���
				HNB1.tm2[1][i].Time++; //���˂���Ă���̌o�ߎ��Ԃ�i�߂�
				//��ʂ���������ʒu�܂ňړ��������ǂ������ׂ�
				if (HNB1.tm2[1][i].Time > 81)
				{
					HNB1.tm2[1][i].fTama = _TOFF;							//�t���O���N���A
					HNB1.tm2[1][i].Time = 0;								//�o�ߎ��Ԃ��N���A
					HNB1.tm2[1][i].AnimeCnt = 0;							//�ŏ��ɕ\������A�j���[�V�����p�^�[����������
					HNB1.tm2[1][i].FrameCnt = HNB1.tm2[1][i].AnimeFrame[0]; //�ŏ��̃A�j���[�V�����t���[���J�E���g��������
					continue;
				}
				//��ʂ̈ړ��ʂ�����
				HNB1.tm2[1][i].Mov.x = 5 * Cos[HNB1.tm2[1][i].Kakudo];
				HNB1.tm2[1][i].Mov.y = -((5 * Sin[HNB1.tm2[1][i].Kakudo]) - (HNB1.tm2[1][i].Time * _G));
				//��ʂ��ړ�������
				HNB1.tm2[1][i].dCdn.x += HNB1.tm2[1][i].Mov.x;
				HNB1.tm2[1][i].dCdn.y += HNB1.tm2[1][i].Mov.y;
				HNB1.tm2[1][i].Cdn.x = (LONG)HNB1.tm2[1][i].dCdn.x;
				HNB1.tm2[1][i].Cdn.y = (LONG)HNB1.tm2[1][i].dCdn.y;
				HNB1.tm2[1][i].MapCdn.x = MainCdn.x + HNB1.tm2[1][i].Cdn.x;
				HNB1.tm2[1][i].MapCdn.y = MainCdn.y + HNB1.tm2[1][i].Cdn.y;

				//��ʂ���ʂ̊O�Ɉړ����Ă��Ȃ����`�F�b�N
				if (HNB1.tm2[1][i].Cdn.x + HNB1.tm2[1][i].Width < 0 ||
					HNB1.tm2[1][i].Cdn.x > WinXMax ||
					HNB1.tm2[1][i].Cdn.y + HNB1.tm2[1][i].Height < 0 ||
					HNB1.tm2[1][i].Cdn.y > WinYMax)
				{
					HNB1.tm2[1][i].fTama = _TOFF; //�t���O���N���A
					continue;
				}

				// SP�T�[�t�F�X����̓]����`�͈͂��Z�b�g
				HNB1.tm2[1][i].SrcR = TamSPJIKI[HNB1.tm2[1][i].AnimePat[HNB1.tm2[1][i].AnimeCnt]];

				HNB1.tm2[1][i].DesR.left = HNB1.tm2[1][i].Cdn.x;
				HNB1.tm2[1][i].DesR.top = HNB1.tm2[1][i].Cdn.y;
				HNB1.tm2[1][i].DesR.right = HNB1.tm2[1][i].Cdn.x + HNB1.tm2[1][i].Width;
				HNB1.tm2[1][i].DesR.bottom = HNB1.tm2[1][i].Cdn.y + HNB1.tm2[1][i].Height;

				//��ʊO�̕����͕`�悵�Ȃ��悤�ɂ��鏈��
				if (HNB1.tm2[1][i].DesR.left < 0)
				{
					HNB1.tm2[1][i].SrcR.left = HNB1.tm2[1][i].SrcR.left - HNB1.tm2[1][i].DesR.left;
					HNB1.tm2[1][i].DesR.left = 0;
				}
				else if (HNB1.tm2[1][i].DesR.right > WinXMax)
				{
					HNB1.tm2[1][i].SrcR.right = HNB1.tm2[1][i].SrcR.right - (HNB1.tm2[1][i].DesR.right - WinXMax);
					HNB1.tm2[1][i].DesR.right = WinXMax;
				}
				if (HNB1.tm2[1][i].DesR.top < 0)
				{
					HNB1.tm2[1][i].SrcR.top = HNB1.tm2[1][i].SrcR.top - HNB1.tm2[1][i].DesR.top;
					HNB1.tm2[1][i].DesR.top = 0;
				}

				//�t���[����1���炵��0�ɂȂ�����A�j���p�^�[����i�߂�
				if (!(HNB1.tm2[1][i].FrameCnt--))
				{
					HNB1.tm2[1][i].AnimeCnt = (HNB1.tm2[1][i].AnimeCnt + 1) % HNB1.tm2[1][i].Anime; //�A�j���p�^�[����i�߂�
					HNB1.tm2[1][i].FrameCnt = HNB1.tm2[1][i].AnimeFrame[HNB1.tm2[1][i].AnimeCnt];	//�\������A�j���p�^�[���̃t���[�����Z�b�g
				}
			}
			else if (HNB1.tm2[1][i].fTama == _TOFF)
			{
				for (j = 0; (DWORD)j < 2; j++)
				{
					if (ODama[j] == HNB1.TTama[1])
					{
						ODama[j] = i;
						HNB1.tm2[1][i].fTama |= _TSET; //�Ăє��˃Z�b�g
						break;
					}
				}
				if (!(ODama[1] == HNB1.TTama[1]) && !(fJIKI & _SUBHNB_ON))
				{
					break; //��ʂɒe�����݂��Ă��Ȃ��ꍇ���[�v�𔲂���
				}
			}
		}

		//���ʂ̈ړ��ʂ�����
		for (i = 0; (DWORD)i < HNB1.TTama[0]; i++)
		{
			//���˒��t���O�̗����Ă��Ȃ��e�͏������Ȃ�
			if (HNB1.tm2[0][i].fTama & _TON)
			{
				TamaS = TRUE;		   //�ԉ΂͉�ʓ��ɑ��݂��Ă���
				HNB1.tm2[0][i].Time++; //���˂���Ă���̌o�ߎ��Ԃ�i�߂�

				if (HNB1.tm2[0][i].Time > 80 && !(HNB1.tm2[0][i].fTama & _TSAKU_ON))
				{
					HNB1.tm2[0][i].fTama |= _TSAKU_ON; //��ʂ������t���O�I��
					if (HNB1.tm2[0][i].Kakudo == 80)
					{									   //��
						HNB1.tm2[0][i].Kakudo = TamaKaku1; //���������p�x���Z�b�g
						TamaKaku1 = TamaKaku1 + 12 > 348 ? 0 : TamaKaku1 + 12;
					}
					else if (HNB1.tm2[0][i].Kakudo == 100)
					{									   //�E
						HNB1.tm2[0][i].Kakudo = TamaKaku2; //���������p�x���Z�b�g
						TamaKaku2 = TamaKaku2 + 12 > 348 ? 0 : TamaKaku2 + 12;
					}
				}

				//���ʂ̈ړ��ʌ���
				if (!(HNB1.tm2[0][i].fTama & _TSAKU_ON))
				{ //��ʂ̒��ňړ���
					HNB1.tm2[0][i].Mov.x = 5 * Cos[HNB1.tm2[0][i].Kakudo];
					HNB1.tm2[0][i].Mov.y = -((5 * Sin[HNB1.tm2[0][i].Kakudo]) - (HNB1.tm2[0][i].Time * _G));
				}
				else
				{ //��ʔ�����
					HNB1.tm2[0][i].Mov.x = 10 * Cos[HNB1.tm2[0][i].Kakudo];
					HNB1.tm2[0][i].Mov.y = -10 * Sin[HNB1.tm2[0][i].Kakudo];
				}

				//���ʂ��ړ�������
				HNB1.tm2[0][i].dCdn.x += HNB1.tm2[0][i].Mov.x;
				HNB1.tm2[0][i].dCdn.y += HNB1.tm2[0][i].Mov.y;
				HNB1.tm2[0][i].Cdn.x = (LONG)HNB1.tm2[0][i].dCdn.x;
				HNB1.tm2[0][i].Cdn.y = (LONG)HNB1.tm2[0][i].dCdn.y;
				HNB1.tm2[0][i].MapCdn.x = MainCdn.x + HNB1.tm2[0][i].Cdn.x;
				HNB1.tm2[0][i].MapCdn.y = MainCdn.y + HNB1.tm2[0][i].Cdn.y;

				//���ʂ���ʂ̊O�Ɉړ����Ă��Ȃ����`�F�b�N
				if (!(HNB1.tm2[0][i].fTama & _TSAKU_ON))
				{
					//��ʂ̒��̎�
					if (HNB1.tm2[0][i].Cdn.x + HNB1.tm2[0][i].Width < -8 ||
						HNB1.tm2[0][i].Cdn.x > WinXMax + 8 ||
						HNB1.tm2[0][i].Cdn.y + HNB1.tm2[0][i].Height < -8 ||
						HNB1.tm2[0][i].Cdn.y > WinYMax + 8)
					{
						HNB1.tm2[0][i].fTama = _TOFF; //�t���O���N���A
						continue;
					}
				}
				else
				{
					//����������
					if (HNB1.tm2[0][i].Cdn.x + HNB1.tm2[0][i].Width < 0 ||
						HNB1.tm2[0][i].Cdn.x > WinXMax ||
						HNB1.tm2[0][i].Cdn.y + HNB1.tm2[0][i].Height < 0 ||
						HNB1.tm2[0][i].Cdn.y > WinYMax)
					{
						HNB1.tm2[0][i].fTama = _TOFF; //�t���O���N���A
						continue;
					}
				}

				// SP�T�[�t�F�X����̓]����`�͈͂��Z�b�g
				HNB1.tm2[0][i].SrcR = TamSPJIKI[HNB1.tm2[0][i].AnimePat[HNB1.tm2[0][i].AnimeCnt]];

				HNB1.tm2[0][i].DesR.left = HNB1.tm2[0][i].Cdn.x;
				HNB1.tm2[0][i].DesR.top = HNB1.tm2[0][i].Cdn.y;
				HNB1.tm2[0][i].DesR.right = HNB1.tm2[0][i].Cdn.x + HNB1.tm2[0][i].Width;
				HNB1.tm2[0][i].DesR.bottom = HNB1.tm2[0][i].Cdn.y + HNB1.tm2[0][i].Height;

				//��ʊO�̕����͕`�悵�Ȃ��悤�ɂ��鏈��
				if (HNB1.tm2[0][i].DesR.left < 0)
				{
					HNB1.tm2[0][i].SrcR.left = HNB1.tm2[0][i].SrcR.left - HNB1.tm2[0][i].DesR.left;
					HNB1.tm2[0][i].DesR.left = 0;
				}
				else if (HNB1.tm2[0][i].DesR.right > WinXMax)
				{
					HNB1.tm2[0][i].SrcR.right = HNB1.tm2[0][i].SrcR.right - (HNB1.tm2[0][i].DesR.right - WinXMax);
					HNB1.tm2[0][i].DesR.right = WinXMax;
				}
				if (HNB1.tm2[0][i].DesR.top < 0)
				{
					HNB1.tm2[0][i].SrcR.top = HNB1.tm2[0][i].SrcR.top - HNB1.tm2[0][i].DesR.top;
					HNB1.tm2[0][i].DesR.top = 0;
				}
				else if (HNB1.tm2[0][i].DesR.bottom > WinYMax)
				{
					HNB1.tm2[0][i].SrcR.bottom = HNB1.tm2[0][i].SrcR.bottom - (HNB1.tm2[0][i].DesR.bottom - WinYMax);
					HNB1.tm2[0][i].DesR.bottom = WinYMax;
				}

				//�t���[����1���炵��0�ɂȂ�����A�j���p�^�[����i�߂�
				if (!(HNB1.tm2[0][i].FrameCnt--))
				{
					HNB1.tm2[0][i].AnimeCnt = (HNB1.tm2[0][i].AnimeCnt + 1) % HNB1.tm2[0][i].Anime; //�A�j���p�^�[����i�߂�
					HNB1.tm2[0][i].FrameCnt = HNB1.tm2[0][i].AnimeFrame[HNB1.tm2[0][i].AnimeCnt];	//�\������A�j���p�^�[���̃t���[�����Z�b�g
				}
			}
			else if (HNB1.tm2[0][i].fTama == _TOFF)
			{
				//�Ĕ��ˉ\�ȉԉ΂̔ԍ���ۑ�����
				for (j = 0; (DWORD)j < 60; j++)
				{
					if (KoDama[j] == HNB1.TTama[0])
					{
						KoDama[j] = i;
						HNB1.tm2[0][i].fTama |= _TSET; //�Ăє��˃Z�b�g
						break;
					}
				}
				if (!(KoDama[59] == HNB1.TTama[0]) && !(fJIKI & _SUBHNB_ON))
				{
					break; //��ʂɒe�����݂��Ă��Ȃ��ꍇ�̓��[�v�𔲂���
				}
			}
		}
		break;
	default:
		break;
	}
	//�e�����݂��Ă��Ȃ�������ԉΔ��˃t���O���I�t
	if (!TamaS)
		fJIKI &= _SUBHNB_OFF;
}

//////////////////////////////////////////////////////////////////////
//	�G�L�����Ɖԉ΂Ƃ̓����蔻��
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HnbAtari()
{
	short i, j, k;

	//���˂���Ă���ԉ΂ƓG�L�����̓����蔻��
	switch (Wpd.flag & _HNB_LVCHK)
	{
	case _HNB1_ON:
		//��ʂ̓����蔻��
		for (i = 0; (DWORD)i < HNB1.TTama[1]; i++)
		{
			//�����˂̒e�Ƃ͓����蔻������Ȃ�
			if (HNB1.tm2[1][i].fTama & _TON)
			{
				if (!(Atari(HNB1.tm2[1][i].DesR)))
				{
					//��ʂ𔚔�������
					HNB1.tm2[1][i].fTama = _TOFF; //�t���O���N���A

					for (j = 0; (DWORD)j < HNB1.TTama[0]; j++)
					{
						if (HNB1.tm2[0][j].fTama & _TON && !(HNB1.tm2[0][j].fTama & _TSAKU_ON))
						{
							//�E�̑�ʂ��G�ɓ��������ꍇ
							if (HNB1.tm2[1][i].Kakudo == 80 && HNB1.tm2[0][j].Kakudo == 80)
							{
								HNB1.tm2[0][j].Time += 80; //�����܂ł̎��Ԃ�i�߂�
								//���̑�ʂ��G�ɓ��������ꍇ
							}
							else if (HNB1.tm2[1][i].Kakudo == 100 && HNB1.tm2[0][j].Kakudo == 100)
							{
								HNB1.tm2[0][j].Time += 80; //�����܂ł̎��Ԃ�i�߂�
							}
						}
					}
				}

				//�j��\�Ȓe�Ƃ̓����蔻��
				if (SP_Tamas3)
				{
					for (j = 0; (DWORD)j < SP_Tamas3->TTama; j++)
					{
						if (SP_Tamas3->tm3[j].fTama & _TON)
						{
							if (!(Atari(HNB1.tm2[1][i].DesR, SP_Tamas3->tm3[j].DesR)))
							{
								//��ʂ𔚔�������
								HNB1.tm2[1][i].fTama = _TOFF; //�t���O���N���A
								for (k = 0; (DWORD)k < HNB1.TTama[0]; k++)
								{
									if (HNB1.tm2[0][k].fTama & _TON && !(HNB1.tm2[0][k].fTama & _TSAKU_ON))
									{
										//�E�̑�ʂ��G�ɓ��������ꍇ
										if (HNB1.tm2[1][i].Kakudo == 80 && HNB1.tm2[0][k].Kakudo == 80)
										{
											HNB1.tm2[0][k].Time += 80; //�����܂ł̎��Ԃ�i�߂�
											//���̑�ʂ��G�ɓ��������ꍇ
										}
										else if (HNB1.tm2[1][i].Kakudo == 100 && HNB1.tm2[0][k].Kakudo == 100)
										{
											HNB1.tm2[0][k].Time += 80; //�����܂ł̎��Ԃ�i�߂�
										}
									}
								}
							}
						}
					}
				}
			}
		}

		//���ʂ̓����蔻��
		for (i = 0; (DWORD)i < HNB1.TTama[0]; i++)
		{
			//�����˂̒e�Ƃ͓����蔻������Ȃ�
			if (HNB1.tm2[0][i].fTama & _TON)
			{
				if (!(Atari(HNB1.tm2[0][i].DesR)))
				{
					//���ʂ����ł�����
					HNB1.tm2[0][i].fTama = _TOFF; //�t���O���N���A
					JikiTekiDmg(_HNB1DMG);		  //�G�L�����Ƀ_���[�W
				}

				//�j��\�Ȓe�Ƃ̓����蔻��
				if (SP_Tamas3)
				{
					for (j = 0; (DWORD)j < SP_Tamas3->TTama; j++)
					{
						if (SP_Tamas3->tm3[j].fTama & _TON)
						{
							if (!(Atari(HNB1.tm2[0][i].DesR, SP_Tamas3->tm3[j].DesR)))
							{
								//���ʂ����ł�����
								HNB1.tm2[0][i].fTama = _TOFF; //�t���O���N���A
								JikiTamas3Dmg(_HNB1DMG, j);
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
//	�ԉ΂̕`�揈��
//////////////////////////////////////////////////////////////////////
void DrawJIKI::HnbRend()
{
	short i;

	switch (Wpd.flag & _HNB_LVCHK)
	{ //�����̃��x���𒲂ׂĂ���
	case _HNB1_ON:
		for (i = 0; (DWORD)i < HNB1.TTama[0]; i++)
		{
			//���˒��t���O�̗����Ă��Ȃ��e�͕`�悵�Ȃ�
			if (HNB1.tm2[0][i].fTama & _TON)
			{
				lpDDSBack->BltFast(HNB1.tm2[0][i].DesR.left, HNB1.tm2[0][i].DesR.top,
								   lpDDS_JIKI, &HNB1.tm2[0][i].SrcR,
								   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			}
		}
		for (i = 0; (DWORD)i < HNB1.TTama[1]; i++)
		{
			if (HNB1.tm2[1][i].fTama & _TON)
			{
				lpDDSBack->BltFast(HNB1.tm2[1][i].DesR.left, HNB1.tm2[1][i].DesR.top,
								   lpDDS_JIKI, &HNB1.tm2[1][i].SrcR,
								   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			}
		}
		break;
	default:
		break;
	}
}
