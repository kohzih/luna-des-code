/*====================================================================
	�G�L����02�̏����Ɋւ���\�[�X�t�@�C��
=====================================================================*/
#include <windows.h>
#include <ddraw.h>

#include "Macros.h"
#include "Draw.h"
#include "Struct.h"
#include "TEKI02.h"
#include "DrawSPDataEXT.h"
#include "DrawMacros.h"

extern LPDIRECTDRAWSURFACE7 lpDDSBack;

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TEKI02::TEKI02()
{
	TChr.AnimePat = NULL;
	TChr.AnimeFrame = NULL;
	TBaku.AnimePat = NULL;
	TBaku.AnimeFrame = NULL;
	TMsr.tm = NULL;
}

TEKI02::~TEKI02()
{
	RELEASE_dels(TMsr.tm);
	RELEASE_dels(TBaku.AnimeFrame);
	RELEASE_dels(TBaku.AnimePat);
	RELEASE_dels(TChr.AnimeFrame);
	RELEASE_dels(TChr.AnimePat);
}

//////////////////////////////////////////////////////////////////////
//	�G�L����02�N���X�̃����o�ϐ��̏�����
//	����:�G�L����02�̔z�u����Ă����΍��W
//////////////////////////////////////////////////////////////////////
BOOL TEKI02::InitTeki02(POINT Tcdn)
{
	//�ď������ɕK�v�ȏ���
	RELEASE_dels(TMsr.tm);
	RELEASE_dels(TBaku.AnimeFrame);
	RELEASE_dels(TBaku.AnimePat);
	RELEASE_dels(TChr.AnimeFrame);
	RELEASE_dels(TChr.AnimePat);

	//�G�L�����̏�ԃt���O�̏�����
	fSPChr = 0;

	//�G�L�����̔z�u����Ă����΍��W��������
	MapCdn = Tcdn;
	//�G�L�����̔z�u����Ă����ʍ��W��������
	Cdn.x = MapCdn.x - MainCdn.x;
	Cdn.y = MapCdn.y - MainCdn.y;
	//���W�v�Z�p�̍\���̂�������
	dCdn.x = MapCdn.x;
	dCdn.y = MapCdn.y;

	//�G�L����02�̑傫���̏�����
	Width = 32;
	Height = 64;

	Shld = 100;		//�G�L����02�̑ϋv��
	ShldCnt = Shld; //�G�L����02�̎c��ϋv�͂̃J�E���^��������
	ATmt = 30;		//�U���Ԑ��ɓ���܂ł̃E�F�C�g��
	ATmtCnt = ATmt; //�U���Ԑ��ɓ���܂ł̃E�F�C�g���̃J�E���^��������
	ATt = 200;		//�U���𑱂��鎞��
	ATtCnt = ATt;	//�U���𑱂��鎞�Ԃ̃J�E���^��������

	//��ʂɕ\������Ă���̌o�ߎ��Ԃ�������
	ONTime = 0;
	//�e�𔭎˂���p�x��������
	TamaKaku = 120;

	//�G�L����02�̈ړ����x�̏�����
	Mov.x = 0.0;
	Mov.y = 0.0;

	//�G�L����02�����ݕ\�����Ă����ʏ�ɑ��݂��Ă���̂��ǂ����`�F�b�N
	if (MainCdn.y > MapCdn.y + Height)
	{
		fSPChr &= _SPOFF;
	}
	else if (MainCdn.y + WinYMax < MapCdn.y)
	{
		fSPChr &= _SPOFF;
	}
	else if (MainCdn.x > MapCdn.x + Width)
	{
		fSPChr &= _SPOFF;
	}
	else if (MainCdn.x + WinXMax < MapCdn.x)
	{
		fSPChr &= _SPOFF;
	}
	else
		fSPChr |= _SPON; //�\���t���O���I��

	//�e�X�e�[�^�X�\���̂ɒl���Z�b�g���Ēe�����m�ۂ����A�e�I�u�W�F�N�g��������
	TMsr.TTama = 30;			//��ʂɑ��݂ł���e�̐�
	TMsr.WTama = 3;				//�e�𔭎˂���Ԋu
	TMsr.WTamaCnt = TMsr.WTama; //�ŏ��̒e���˂܂ł̃J�E���^

	TTamaNum = TMsr.TTama; //�����ɔ��˂���ׂ��e�ԍ���ۑ��ϐ���������

	// TAMA�\���̂ɑO�����Ēl���Z�b�g���Ēe�f�[�^������������
	TAMA tama;
	char *pTama;
	int i;

	tama.fTama = _TOFF; //�e�̏�����Ԃ�ݒ�
	tama.Width = 16;	//�e�̉���
	tama.Height = 16;	//�e�̏c��
	tama.Mov.x = 0.0;	//�e�̈ړ����x��0�ŏ�����
	tama.Mov.y = 0.0;
	tama.dCdn.x = 0.0; //�e�̍��W�v�Z�p�̍��W��0�ŏ�����
	tama.dCdn.y = 0.0;
	tama.Cdn.x = 0; //�e�̌��݂̉�ʍ��W��0�ŏ�����
	tama.Cdn.y = 0;
	tama.MapCdn.x = 0; //�e�̌��݂̐�΍��W��0�ŏ�����
	tama.MapCdn.y = 0;
	tama.Anime = TamAme[1];											 //�e�A�j���[�V�����p�^�[�����̏�����
	memcpy(&tama.AnimePat[0], &TamPat[1][0], sizeof(BYTE) * 10);	 //�e�A�j���[�V�����p�^�[���z��̏�����
	memcpy(&tama.AnimeFrame[0], &TamFrame[1][0], sizeof(BYTE) * 10); //�e�A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	tama.AnimeCnt = 0;												 //�ŏ��ɕ\������A�j���[�V�����p�^�[����������
	tama.FrameCnt = tama.AnimeFrame[0];								 //�ŏ��̃A�j���[�V�����t���[����������
	tama.SrcR.left = 0;												 //�]������`��0�ŏ�����
	tama.SrcR.top = 0;
	tama.SrcR.right = 0;
	tama.SrcR.bottom = 0;
	tama.DesR = tama.SrcR; //�]�����`��0�ŏ�����

	//���˂ł���e����TAMA�\���̂��m��
	TMsr.tm = new TAMA[TMsr.TTama];
	if (!TMsr.tm)
		return FALSE;

	//�m�ۂ����̈�̐擪�A�h���X���R�s�[
	pTama = (char *)TMsr.tm;

	//�e�I�u�W�F�N�g���m�ۂ������A������
	for (i = 0; (DWORD)i < TMsr.TTama; i++)
	{
		memcpy(pTama, &tama, sizeof(TAMA)); //�m�ۂ����̈�̐擪����R�s�[
		pTama += sizeof(TAMA);				//���ɃR�s�[����Ƃ���܂ŃA�h���X��i�߂�
	}

	//�G�L����02�̃A�j���[�V�����p�^�[�����̏�����
	TChr.Anime = Ame[1];
	//�A�j���[�V�����p�^�[���z��̏�����
	TChr.AnimePat = new BYTE[TChr.Anime];
	if (!TChr.AnimePat)
		return FALSE;
	memcpy(TChr.AnimePat, &Pat[1][0], sizeof(BYTE) * TChr.Anime);
	//�A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	TChr.AnimeFrame = new BYTE[TChr.Anime];
	if (!TChr.AnimeFrame)
		return FALSE;
	memcpy(TChr.AnimeFrame, &Frame[1][0], sizeof(BYTE) * TChr.Anime);
	//�ŏ��ɕ\������A�j���[�V�����p�^�[����������
	TChr.AnimeCnt = 0;
	//�ŏ��̃A�j���[�V�����t���[����������
	TChr.FrameCnt = TChr.AnimeFrame[0];

	//�����A�j���[�V�����̏�����
	TBaku.Anime = BakuAme[1];
	//�A�j���[�V�����p�^�[���z��̏�����
	TBaku.AnimePat = new BYTE[TBaku.Anime];
	if (!TBaku.AnimePat)
		return FALSE;
	memcpy(TBaku.AnimePat, &BakuPat[1][0], sizeof(BYTE) * TBaku.Anime);
	//�A�j���[�V�����؂�ւ��^�C�~���O�z��̏�����
	TBaku.AnimeFrame = new BYTE[TBaku.Anime];
	if (!TBaku.AnimeFrame)
		return FALSE;
	memcpy(TBaku.AnimeFrame, &BakuFrame[1][0], sizeof(BYTE) * TBaku.Anime);
	//�ŏ��ɕ\������A�j���[�V�����p�^�[����������
	TBaku.AnimeCnt = 0;
	//�ŏ��̃A�j���[�V�����t���[����������
	TBaku.FrameCnt = TBaku.AnimeFrame[0];

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	�G�L����02�̈ړ�����
//////////////////////////////////////////////////////////////////////
void TEKI02::MovTeki02()
{
	//�G�L�������j�󂳂�Ă���Ȃ烊�^�[��
	if ((fSPChr & _SPDESCHK) == (DWORD)_SPDES)
		return;

	//�G�L���������݂̕\����ʔ͈͂ɓ����Ă��邩�`�F�b�N
	if (MainCdn.y > MapCdn.y + Height)
		fSPChr &= _SPOFF;
	else if (MainCdn.y + WinYMax < MapCdn.y)
		fSPChr &= _SPOFF;
	else if (MainCdn.x > MapCdn.x + Width)
		fSPChr &= _SPOFF;
	else if (MainCdn.x + WinXMax > MapCdn.x)
	{
		//�\���t���O�I��
		fSPChr |= _SPON;
		//��ʂɕ\������Ă���̌o�ߎ��Ԃ�������
		ONTime++;
		//�ړ��ʂ�������(�j�󂳂ꂽ�ꍇ�͈ړ��ʂ������Ȃ�)
		if (!(fSPChr & _SPDES))
		{
			//���݂̈ړ��ʂ����߂�
			if (ONTime < 30)
			{
				Mov.x = 0;
				Mov.y = 3;
			}
			else if (ONTime > 30 && ONTime < 230)
			{
				Mov.x = 0;
				if (!MainCdn.y)
					Mov.y = 0;
				else
					Mov.y = -1;
			}
			else
			{
				Mov.x = 0;
				if (!MainCdn.y)
					Mov.y = 0;
				else
					Mov.y = 3;
			}
			//��΍��W�Ɉړ��ʂ�������
			dCdn.x += Mov.x;
			dCdn.y += Mov.y;
			MapCdn.x = (LONG)dCdn.x;
			MapCdn.y = (LONG)dCdn.y;
		}
		Cdn.x = MapCdn.x - MainCdn.x;
		Cdn.y = MapCdn.y - MainCdn.y;
	}
}

//////////////////////////////////////////////////////////////////////
//	�G�L����02�̍U������
//////////////////////////////////////////////////////////////////////
void TEKI02::AttackTeki02()
{
	BOOL ret = FALSE;

	//----------------------------------------------------------//
	//					    �e�̈ړ�����						//
	//----------------------------------------------------------//

	//���ɔ��˂���Ă���e�̍��W��i�߂�
	for (int i = 0; (DWORD)i < TMsr.TTama; i++)
	{
		//���ɔ��˂���Ă���e�̂ݏ�������
		if (TMsr.tm[i].fTama & _TON)
		{
			//��x�ł��e�ړ��������s��ꂽ�ꍇret��TRUE����
			ret = TRUE;

			//�e���ړ�������
			TMsr.tm[i].dCdn.x += TMsr.tm[i].Mov.x;
			TMsr.tm[i].dCdn.y += TMsr.tm[i].Mov.y;
			//�e�̐�΍��W��i�߂�
			TMsr.tm[i].MapCdn.x = (LONG)TMsr.tm[i].dCdn.x;
			TMsr.tm[i].MapCdn.y = (LONG)TMsr.tm[i].dCdn.y;
			//�e�̉�ʍ��W�����߂�
			TMsr.tm[i].Cdn.x = TMsr.tm[i].MapCdn.x - MainCdn.x;
			TMsr.tm[i].Cdn.y = TMsr.tm[i].MapCdn.y - MainCdn.y;

			//�e����ʊO�Ɉړ����ĂȂ����`�F�b�N
			if (TMsr.tm[i].MapCdn.x + TMsr.tm[i].Width < MainCdn.x ||
				TMsr.tm[i].MapCdn.x > MainCdn.x + WinXMax ||
				TMsr.tm[i].MapCdn.y + TMsr.tm[i].Height < MainCdn.y ||
				TMsr.tm[i].MapCdn.y > MainCdn.y + WinYMax)
			{
				TMsr.tm[i].fTama = _TOFF; //�e�t���O�I�t
				continue;
			}
		}
		else if (TMsr.tm[i].fTama == _TOFF)
		{
			if (TTamaNum == TMsr.TTama)
			{
				TTamaNum = i;
				TMsr.tm[i].fTama |= _TSET; //�Ăє��˃Z�b�g
			}
			else if (!(fSPChr & _TAMA1ON))
			{ //��ʒe���݃t���O�������Ă��Ȃ��ꍇ�͒e�̈ړ����������Ȃ�
				break;
			}
		}
	}

	// ret�̒l��FALSE�̂܂܂������ꍇ�A��ʂɒe�͑��݂��Ă��Ȃ�
	if (!ret)
		fSPChr &= _TAMA1OFF; //��ʒe���݃t���O���I�t

	//�G�L�������\������Ă��Ȃ��ꍇ���^�[��
	if (!(fSPChr & _SPON))
		return;
	//�G�L�����̔j��t���O�������Ă���ꍇ���^�[��
	else if (fSPChr & _SPDES)
		return;

	//----------------------------------------------------------//
	//					    �e�̔��ˏ���						//
	//----------------------------------------------------------//

	//�G�L����02�̍U���ł��鎞��
	if (ONTime > 30)
	{
		//�G�L�������U���Ԑ��ɓ����Ă��邩�`�F�b�N
		if (fSPChr & _ATCKON)
		{
			if (!(TMsr.WTamaCnt--))
			{
				//���ݔ��ˉ\�Ȓe�����邩�`�F�b�N
				if (TTamaNum < TMsr.TTama)
				{
					//��ʒe���݃t���O�I��
					fSPChr |= _TAMA1ON;
					//���˂����e�̔��˒��t���O�𗧂Ă�
					TMsr.tm[TTamaNum].fTama |= _TON;
					//�e�A�j���[�V������������
					TMsr.tm[TTamaNum].AnimeCnt = 0;
					TMsr.tm[TTamaNum].FrameCnt = TMsr.tm[TTamaNum].AnimeFrame[0];
					//�e�̔��˂��ꂽ��΍��W��������
					TMsr.tm[TTamaNum].MapCdn.x = MapCdn.x + 4;
					TMsr.tm[TTamaNum].MapCdn.y = MapCdn.y + 16;
					//�e�̈ړ��v�Z�p�ɐ�΍��W���擾
					TMsr.tm[TTamaNum].dCdn.x = TMsr.tm[TTamaNum].MapCdn.x;
					TMsr.tm[TTamaNum].dCdn.y = TMsr.tm[TTamaNum].MapCdn.y;
					//�e�̉�ʍ��W��������
					TMsr.tm[TTamaNum].Cdn.x = TMsr.tm[TTamaNum].MapCdn.x - MainCdn.x;
					TMsr.tm[TTamaNum].Cdn.y = TMsr.tm[TTamaNum].MapCdn.y - MainCdn.y;

					//�e�̔��˂����p�x��0-360�x�̊Ԃɐ������鏈��
					TamaKaku -= 10;
					if (TamaKaku < 0)
						TamaKaku = 360 + TamaKaku;

					//�e�̈ړ����x��������
					TMsr.tm[TTamaNum].Mov.x = 5 * Cos[TamaKaku];
					//��ʂ���ɃX�N���[�����Ă��邽�߁A�e��������ɔ��˂���ꍇ��
					//�X�s�[�h�𓯂��Ɍ�����ׁAY�����̈ړ��ʂ𑝂₷
					if (TamaKaku > 0 && TamaKaku < 180)
					{
						TMsr.tm[TTamaNum].Mov.y = -6 * Sin[TamaKaku];
					}
					else
					{
						TMsr.tm[TTamaNum].Mov.y = -5 * Sin[TamaKaku];
					}

					//�Ăє��ˉ\�Ȓe�ԍ���ۑ����邽�߂ɏ�����
					TTamaNum = TMsr.TTama;
				}
				TMsr.WTamaCnt = TMsr.WTama; //���˂܂ł̃J�E���g���ĂуZ�b�g
			}

			//----------------------------------------------------------//
			//	�G�L�����̍U���Ԑ��ɓ���܂ł̃J�E���g���s�Ȃ�����		//
			//----------------------------------------------------------//

			if (!(ATtCnt--))
			{						//�U�����Ԃ����炵�Ă���
				fSPChr &= _ATCKOFF; //�U���Ԑ��t���O�I�t
				ATmtCnt = ATmt;		//���ꂼ��̃J�E���^���ĂуZ�b�g
				ATtCnt = ATt;
				TMsr.WTamaCnt = TMsr.WTama;
			}
		}
		else
		{
			//�U���Ԑ��ɓ���܂ł̃J�E���^�����炷
			if (!(ATmtCnt--))
				fSPChr |= _ATCKON;
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	�G�L����02�̕`�揀�����s�Ȃ�����
//	�����͏�Ԃ�\�킷�t���O
//////////////////////////////////////////////////////////////////////
void TEKI02::FrameTeki02(DWORD Tflag)
{
	fSPChr = Tflag;

	//----------------------------------------------------------//
	//					�e�̕`�揀��							//
	//----------------------------------------------------------//

	//��ʒe���݃t���O�������Ă���ꍇ�̂ݒe��`��
	if (fSPChr & _TAMA1ON)
	{
		for (int i = 0; (DWORD)i < TMsr.TTama; i++)
		{
			//���˂���Ă��Ȃ��e�̂ݕ`�悷��
			if (TMsr.tm[i].fTama & _TON)
			{
				// SP�T�[�t�F�X����̓]����`�͈͂��Z�b�g
				TMsr.tm[i].SrcR = TamSP[TMsr.tm[i].AnimePat[TMsr.tm[i].AnimeCnt]];

				TMsr.tm[i].DesR.left = TMsr.tm[i].Cdn.x;
				TMsr.tm[i].DesR.top = TMsr.tm[i].Cdn.y;
				TMsr.tm[i].DesR.right = TMsr.tm[i].Cdn.x + TMsr.tm[i].Width;
				TMsr.tm[i].DesR.bottom = TMsr.tm[i].Cdn.y + TMsr.tm[i].Height;

				if (TMsr.tm[i].DesR.left < 0)
				{
					TMsr.tm[i].SrcR.left = TMsr.tm[i].SrcR.left - TMsr.tm[i].DesR.left;
					TMsr.tm[i].DesR.left = 0;
				}
				else if (TMsr.tm[i].DesR.right > WinXMax)
				{
					TMsr.tm[i].SrcR.right = TMsr.tm[i].SrcR.right - (TMsr.tm[i].DesR.right - WinXMax);
					TMsr.tm[i].DesR.right = WinXMax;
				}
				if (TMsr.tm[i].DesR.top < 0)
				{
					TMsr.tm[i].SrcR.top = TMsr.tm[i].SrcR.top - TMsr.tm[i].DesR.top;
					TMsr.tm[i].DesR.top = 0;
				}
				else if (TMsr.tm[i].DesR.bottom > WinYMax)
				{
					TMsr.tm[i].SrcR.bottom = TMsr.tm[i].SrcR.bottom - (TMsr.tm[i].DesR.bottom - WinYMax);
					TMsr.tm[i].DesR.bottom = WinYMax;
				}

				//�t���[����1���炵��0�ɂȂ�����A�j���p�[�^�[����i�߂�
				if (!(TMsr.tm[i].FrameCnt--))
				{
					//�A�j���p�^�[����i�߂�
					TMsr.tm[i].AnimeCnt = (TMsr.tm[i].AnimeCnt + 1) % TMsr.tm[i].Anime;
					//�\������A�j���p�^�[���̃t���[�����Z�b�g
					TMsr.tm[i].FrameCnt = TMsr.tm[i].AnimeFrame[TMsr.tm[i].AnimeCnt];
				}
			}
		}
	}

	//----------------------------------------------------------//
	//					�G�L�����̕`�揀��						//
	//----------------------------------------------------------//

	//�G�L�����\���t���O�������Ă���ꍇ�`�揀��
	if (fSPChr & _SPON)
	{
		//�G�L�����j��̃A�j���[�V����
		if (fSPChr & _SPDES)
		{
			// SP�T�[�t�F�X����̓]����`�͈͂��Z�b�g
			TBaku.SrcR = BakuSP[TBaku.AnimePat[TBaku.AnimeCnt]];

			TBaku.DesR.left = Cdn.x - 16; //�����̉摜�������傫�����ߍ��W��␳
			TBaku.DesR.top = Cdn.y - 16;
			TBaku.DesR.right = (Cdn.x - 16) + 64;
			TBaku.DesR.bottom = (Cdn.y - 16) + 64;

			//��ʂ���͂ݏo�������͓]�����Ȃ�����
			if (TBaku.DesR.left < 0)
			{
				TBaku.SrcR.left = TBaku.SrcR.left - TBaku.DesR.left;
				TBaku.DesR.left = 0;
			}
			else if (TBaku.DesR.right > WinXMax)
			{
				TBaku.SrcR.right = TBaku.SrcR.right - (TBaku.DesR.right - WinXMax);
				TBaku.DesR.right = WinXMax;
			}
			if (TBaku.DesR.top < 0)
			{
				TBaku.SrcR.top = TBaku.SrcR.top - TBaku.DesR.top;
				TBaku.DesR.top = 0;
			}
			else if (Cdn.y + Height > WinYMax)
			{
				TBaku.SrcR.bottom = TBaku.SrcR.bottom - (TBaku.DesR.bottom - WinYMax);
				TBaku.DesR.bottom = WinYMax;
			}

			//�t���[����1���炵��0�ɂȂ�����A�j���p�[�^�[����i�߂�
			if (!(TBaku.FrameCnt--))
			{
				//�A�j���[�V�����V�[����1���[�v������I��
				if ((TBaku.AnimeCnt + 1) == TBaku.Anime)
				{
					//�����A�j���[�V�����f�[�^�������l�ɖ߂�
					TBaku.AnimeCnt = 0;
					TBaku.FrameCnt = TBaku.AnimeFrame[0];
					fSPChr &= _SPOFF; //�X�v���C�g�̕\�����I�t
				}
				else
				{
					TBaku.AnimeCnt = (TBaku.AnimeCnt + 1) % TBaku.Anime; //�A�j���p�^�[����i�߂�
					TBaku.FrameCnt = TBaku.AnimeFrame[TBaku.AnimeCnt];	 //�\������A�j���p�^�[���̃t���[�����Z�b�g
				}
			}
		}
		else
		{
			//�ʏ펞�̕`��
			//�_���[�W���󂯂Ă��邩�ǂ����`�F�b�N
			if (fSPChr & _HITON)
			{
				TChr.SrcR = SP[18]; //�_���[�W�V�[��
				fSPChr &= _HITOFF;
			}
			else
			{
				TChr.SrcR = SP[TChr.AnimePat[TChr.AnimeCnt]]; //�ʏ�V�[��
			}

			TChr.DesR.left = Cdn.x;
			TChr.DesR.top = Cdn.y;
			TChr.DesR.right = Cdn.x + Width;
			TChr.DesR.bottom = Cdn.y + Height;

			//��ʂ���͂ݏo�������͓]�����Ȃ�����
			if (TChr.DesR.left < 0)
			{
				TChr.SrcR.left = TChr.SrcR.left - TChr.DesR.left;
				TChr.DesR.left = 0;
			}
			else if (TChr.DesR.right > WinXMax)
			{
				TChr.SrcR.right = TChr.SrcR.right - (TChr.DesR.right - WinXMax);
				TChr.DesR.right = WinXMax;
			}
			if (TChr.DesR.top < 0)
			{
				TChr.SrcR.top = TChr.SrcR.top - TChr.DesR.top;
				TChr.DesR.top = 0;
			}
			else if (TChr.DesR.bottom > WinYMax)
			{
				TChr.SrcR.bottom = TChr.SrcR.bottom - (TChr.DesR.bottom - WinYMax);
				TChr.DesR.bottom = WinYMax;
			}

			//�t���[����1���炵��0�ɂȂ�����A�j���p�[�^�[����i�߂�
			if (!(TChr.FrameCnt--))
			{
				TChr.AnimeCnt = (TChr.AnimeCnt + 1) % TChr.Anime; //�A�j���p�^�[����i�߂�
				TChr.FrameCnt = TChr.AnimeFrame[TChr.AnimeCnt];	  //�\������A�j���p�^�[���̃t���[�����Z�b�g
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
//	�G�L����02�̕`��
//////////////////////////////////////////////////////////////////////
void TEKI02::RendTeki02()
{
	//----------------------------------------------------------//
	//						�e�̕`��							//
	//----------------------------------------------------------//

	//��ʒe���݃t���O�������Ă��Ȃ��ꍇ�͒e��`�悵�Ȃ�
	if (fSPChr & _TAMA1ON)
	{
		for (int i = 0; (DWORD)i < TMsr.TTama; i++)
		{
			//���˒��łȂ��e�͕`�悵�Ȃ�
			if (TMsr.tm[i].fTama & _TON)
			{
				lpDDSBack->BltFast(TMsr.tm[i].DesR.left, TMsr.tm[i].DesR.top,
								   lpDDS_SP, &TMsr.tm[i].SrcR,
								   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			}
		}
	}

	//----------------------------------------------------------//
	//					�G�L�����̕`��							//
	//----------------------------------------------------------//

	//�G�L�����\���t���O�̗����Ă��Ȃ��ꍇ�̓��^�[��
	if (!(fSPChr & _SPON))
		return;

	//�G�L�����̕`��
	//�j��t���O�̗����Ă����ꍇ�̓]��
	if (fSPChr & _SPDES)
	{
		lpDDSBack->BltFast(TBaku.DesR.left, TBaku.DesR.top,
						   lpDDS_SP, &TBaku.SrcR,
						   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

		//�ʏ�̓]��
	}
	else if (fSPChr & _SPON)
	{
		lpDDSBack->BltFast(TChr.DesR.left, TChr.DesR.top,
						   lpDDS_SP, &TChr.SrcR,
						   DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}
}

//////////////////////////////////////////////////////////////////////
//	�G�L������ԃt���O��Ԃ�
//////////////////////////////////////////////////////////////////////
DWORD TEKI02::GetTeki02fSPChr()
{
	return fSPChr;
}
//////////////////////////////////////////////////////////////////////
//	�G�L�����̉�ʍ��W��Ԃ�
//////////////////////////////////////////////////////////////////////
POINT TEKI02::GetTeki02Cdn()
{
	return Cdn;
}
//////////////////////////////////////////////////////////////////////
//	�G�L�����̉�����Ԃ�
//////////////////////////////////////////////////////////////////////
short TEKI02::GetTeki02Width()
{
	return Width;
}
//////////////////////////////////////////////////////////////////////
//	�G�L�����̏c����Ԃ�
//////////////////////////////////////////////////////////////////////
short TEKI02::GetTeki02Height()
{
	return Height;
}
//////////////////////////////////////////////////////////////////////
//	�G�L�����̎c��ϋv�͂̒l�ւ̃|�C���^��Ԃ�
//////////////////////////////////////////////////////////////////////
short *TEKI02::GetTeki02ShldCnt_P()
{
	return &ShldCnt;
}
//////////////////////////////////////////////////////////////////////
//	�G�L�����̒e�̏�Ԃ��Ǘ�����TAMAS�\���̂ւ̃|�C���^��Ԃ�
//////////////////////////////////////////////////////////////////////
TAMAS *TEKI02::GetTeki02TMsr_P()
{
	return &TMsr;
}