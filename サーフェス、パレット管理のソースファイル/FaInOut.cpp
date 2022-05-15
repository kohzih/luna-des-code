/*=============================================================================
	�t�F�[�h�C���A�E�g�Ȃǂ̃p���b�g�Ǘ������̃\�[�X�t�@�C��
==============================================================================*/
#include <windows.h>
#include <ddraw.h>

#include "Macros.h"

#include "FaInOut.h"
#include "DrawMacros.h"

extern LPDIRECTDRAWPALETTE lpDDpal; // DirectDraw�p���b�g�I�u�W�F�N�g

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

FaInOut::FaInOut(short StP, short PalE)
{
	//���݂̐ݒ�̓A�E�g����C����
	WFadeF = FALSE;	 //�z���C�g�A�E�g����X�^�[�g
	BFadeF = FALSE;	 //�u���b�N�A�E�g����X�^�[�g
	LBFadeF = FALSE; // LittleBlack�Ǘ��p�̃t���O������
	PalCnt = 0;		 //���݂̐i�s��(PalCnt>>16��0�`255)

	StartPal = StP;	 //�ω����J�n������擪�p���b�g�ԍ�
	PalEntry = PalE; //�ω�������p���b�g�̌�

	lpDDpal->GetEntries(0, 0, 256, apeH1); //���݂̃p���b�g��apeH1�ɃR�s�[

	memcpy(apeH2, apeH1, sizeof(PALETTEENTRY) * 256); //���݂̃p���b�g��apeH2�ɃR�s�[

	for (int i = 0; i < 256; i++) //���݂̃p���b�g��apeH3�ɃR�s�[
	{
		apeH3[i].r = (int)apeH2[i].peRed << 16;
		apeH3[i].g = (int)apeH2[i].peGreen << 16;
		apeH3[i].b = (int)apeH2[i].peBlue << 16;
	}
}

FaInOut::~FaInOut()
{
}

///////////////////////////////////////////////////////////
// �t�F�[�h�C���A�E�g�̊���l�ƂȂ�p���b�g���擾����
///////////////////////////////////////////////////////////
BOOL FaInOut::SetPaletteEntry()
{
	HRESULT ret;
	ret = lpDDpal->GetEntries(0, 0, 256, apeH1); //���݂̃p���b�g��apeH1�ɃR�s�[
	if (ret != DD_OK)
		return FALSE;

	memcpy(apeH2, apeH1, sizeof(PALETTEENTRY) * 256); //���݂̃p���b�g��apeH2�ɃR�s�[

	for (int i = 0; i < 256; i++) //���݂̃p���b�g��apeH3�ɃR�s�[
	{
		apeH3[i].r = (int)apeH2[i].peRed << 16;
		apeH3[i].g = (int)apeH2[i].peGreen << 16;
		apeH3[i].b = (int)apeH2[i].peBlue << 16;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////
// �p���b�g�ƃt���O���e�t�F�[�h�C���A�E�g���\�b�h��
// �����s�ł���悤�ɐݒ肷��
// ����:�t���O
///////////////////////////////////////////////////////////
void FaInOut::SetFade(DWORD SetF)
{
	//�z���C�g�C���̏������s��
	if (SetF & _SetWhiteIn)
	{
		//�t���O���Z�b�g
		WFadeF = TRUE;
		// 0�`9�A246�`255�܂ł̓V�X�e���J���[�̂��ߕύX���Ȃ�
		//�S�Ẵp���b�g�𔒂ɐݒ�
		for (int i = StartPal; i < StartPal + PalEntry; i++)
		{
			apeH2[i].peRed = 255;
			apeH2[i].peGreen = 255;
			apeH2[i].peBlue = 255;
			apeH3[i].r = (int)apeH2[i].peRed << 16;
			apeH3[i].g = (int)apeH2[i].peGreen << 16;
			apeH3[i].b = (int)apeH2[i].peBlue << 16;
		}
		//�K���ύX����p���b�g�̐擪�A�h���X���w��
		lpDDpal->SetEntries(0, StartPal, PalEntry, &apeH2[StartPal]);
		//�u���b�N�C���̏������s��
	}
	else if (SetF & _SetBlackIn)
	{
		BFadeF = TRUE;
		//�S�Ẵp���b�g�����ɐݒ�
		for (int i = StartPal; i < StartPal + PalEntry; i++)
		{
			apeH2[i].peRed = 0;
			apeH2[i].peGreen = 0;
			apeH2[i].peBlue = 0;
			apeH3[i].r = (int)apeH2[i].peRed << 16;
			apeH3[i].g = (int)apeH2[i].peGreen << 16;
			apeH3[i].b = (int)apeH2[i].peBlue << 16;
		}
		//�K���ύX����p���b�g�̐擪�A�h���X���w��
		lpDDpal->SetEntries(0, StartPal, PalEntry, &apeH2[StartPal]);
		//�z���C�g�A�E�g�A�u���b�N�A�E�g�̏������s��
	}
	else if (SetF & _SetDef)
	{
		WFadeF = FALSE;
		BFadeF = FALSE;
		LBFadeF = FALSE;
		//�S�Ẵp���b�g�����ɖ߂�
		for (int i = StartPal; i < StartPal + PalEntry; i++)
		{
			apeH2[i].peRed = apeH1[i].peRed;
			apeH2[i].peGreen = apeH1[i].peGreen;
			apeH2[i].peBlue = apeH1[i].peBlue;
			apeH3[i].r = (int)apeH2[i].peRed << 16;
			apeH3[i].g = (int)apeH2[i].peGreen << 16;
			apeH3[i].b = (int)apeH2[i].peBlue << 16;
		}
		//�K���ύX����p���b�g�̐擪�A�h���X���w��
		lpDDpal->SetEntries(0, StartPal, PalEntry, &apeH2[StartPal]);
	}
}

///////////////////////////////////////////////////////////
//	��ʂ������Â�����(245�ԂƃV�X�e���p���b�g�͕ύX���Ȃ�)
//	����:1:�F�v�f�����炷�l 2:�Â������Ƃ��J��Ԃ��������t���O(0:���̂� 1:�J��Ԃ�)
//	�߂�l:�Â����鏈���̓r���̏ꍇFALSE��Ԃ��B�I�������ꍇTRUE�B
///////////////////////////////////////////////////////////
BOOL FaInOut::LittleBlack(int BLACK, bool flag)
{
	//���݂̂̏ꍇ
	if (!flag && !LBFadeF)
	{
		// 0�`9�܂ł�245�A246�`255�܂ł͕ύX���Ȃ�
		for (int i = StartPal; i < StartPal + PalEntry - 1; i++)
		{
			//�Ԃ̗v�f�����F
			apeH3[i].r = apeH3[i].r - BLACK;
			if (apeH3[i].r >> 16 <= 0)
			{ //���F�������ʗv�f��0��菬�����Ȃ����ꍇ�A0���Z�b�g
				apeH3[i].r = 0;
				apeH2[i].peRed = 0;
			}
			else
				apeH2[i].peRed = apeH3[i].r >> 16;
			//�΂̗v�f�����F
			apeH3[i].g = apeH3[i].g - BLACK;
			if (apeH3[i].g >> 16 <= 0)
			{
				apeH3[i].g = 0;
				apeH2[i].peGreen = 0;
			}
			else
				apeH2[i].peGreen = apeH3[i].g >> 16;
			//�̗v�f�����F
			apeH3[i].b = apeH3[i].b - BLACK;
			if (apeH3[i].b >> 16 <= 0)
			{
				apeH3[i].b = 0;
				apeH2[i].peBlue = 0;
			}
			apeH2[i].peBlue = apeH3[i].b >> 16;
		}

		//�K���ύX����p���b�g�̐擪�A�h���X���w��
		lpDDpal->SetEntries(0, StartPal, PalEntry - 1, &apeH2[10]);

		LBFadeF = TRUE; //�Â������ƏI��

		//���\�b�h�����s��������J��Ԃ��ꍇ
	}
	else if (flag == TRUE && !LBFadeF)
	{
		// 0�`9�܂ł�245�A246�`255�܂ł͕ύX���Ȃ�
		for (int i = StartPal; i < StartPal + PalEntry - 1; i++)
		{
			//�Ԃ̗v�f�����F
			apeH3[i].r = apeH3[i].r - BLACK;
			if (apeH3[i].r >> 16 <= 0)
			{ //���F�������ʗv�f��0��菬�����Ȃ����ꍇ�A0���Z�b�g
				apeH3[i].r = 0;
				apeH2[i].peRed = 0;
			}
			else
				apeH2[i].peRed = apeH3[i].r >> 16;
			//�΂̗v�f�����F
			apeH3[i].g = apeH3[i].g - BLACK;
			if (apeH3[i].g >> 16 <= 0)
			{
				apeH3[i].g = 0;
				apeH2[i].peGreen = 0;
			}
			else
				apeH2[i].peGreen = apeH3[i].g >> 16;
			//�̗v�f�����F
			apeH3[i].b = apeH3[i].b - BLACK;
			if (apeH3[i].b >> 16 <= 0)
			{
				apeH3[i].b = 0;
				apeH2[i].peBlue = 0;
			}
			apeH2[i].peBlue = apeH3[i].b >> 16;
		}

		//�K���ύX����p���b�g�̐擪�A�h���X���w��
		lpDDpal->SetEntries(0, StartPal, PalEntry - 1, &apeH2[10]);
	}
	else
	{
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////
// �z���C�g�A�E�g
// ����:�p���b�g��ω�������X�s�[�h
// �߂�l:TRUE(�z���C�g�A�E�g),FALSE(�z���C�g�A�E�g�̓r��)
///////////////////////////////////////////////////////////
BOOL FaInOut::WhiteOut(int PalSpeed)
{
	//�t���O�������Ă��鎞�͏I��
	if (WFadeF)
		return TRUE;

	if (PalCnt >> 16 > 255) //�p���b�g�̊e�u���b�N��255�񑝐F������I��
	{
		PalCnt = 0;
		WFadeF = 1; //�z���C�g�C���p�t���O�𗧂Ă�
		return TRUE;
	}

	PalCnt += PalSpeed; //�p���b�g�𑝐F������
						//(1��̃��[�v��PalSpeed�����������Ȃ�)

	// 0�`9�A246�`255�܂ł̓V�X�e���J���[�̂��ߕύX���Ȃ�
	for (int i = StartPal; i < StartPal + PalEntry; i++)
	{
		//�Ԃ̗v�f��255��菬�����Ȃ�Α��F
		if (apeH3[i].r >> 16 < 255)
		{
			apeH3[i].r = apeH3[i].r + PalSpeed;
			apeH2[i].peRed = apeH3[i].r >> 16;
			//���F�������ʗv�f��255���傫���Ȃ����ꍇ�A255���Z�b�g
			if (apeH3[i].r >> 16 >= 255)
			{
				apeH3[i].r = 255 << 16;
				apeH2[i].peRed = 255;
			}
		}
		if (apeH3[i].g >> 16 < 255)
		{
			apeH3[i].g = apeH3[i].g + PalSpeed;
			apeH2[i].peGreen = apeH3[i].g >> 16;
			if (apeH3[i].g >> 16 >= 255)
			{
				apeH3[i].g = 255 << 16;
				apeH2[i].peGreen = 255;
			}
		}
		if (apeH3[i].b >> 16 < 255)
		{
			apeH3[i].b = apeH3[i].b + PalSpeed;
			apeH2[i].peBlue = apeH3[i].b >> 16;
			if (apeH3[i].b >> 16 >= 255)
			{
				apeH3[i].b = 255 << 16;
				apeH2[i].peBlue = 255;
			}
		}
	}

	//�K���ύX����p���b�g�̐擪�A�h���X���w��
	lpDDpal->SetEntries(0, StartPal, PalEntry, &apeH2[StartPal]);

	return FALSE;
}
///////////////////////////////////////////////////////////
// �z���C�g�C��
// ����:�p���b�g��ω�������X�s�[�h
// �߂�l:TRUE(�z���C�g�C��),FALSE(�z���C�g�C���̓r��)
///////////////////////////////////////////////////////////
BOOL FaInOut::WhiteIn(int PalSpeed)
{
	//�t���O�������Ă��Ȃ����͏I��
	if (!(WFadeF))
		return TRUE;

	if (PalCnt >> 16 > 255) //�p���b�g�̊e�u���b�N��255�񑝐F������I��
	{
		PalCnt = 0;
		WFadeF = 0; //�t���O���N���A
		return TRUE;
	}

	PalCnt += PalSpeed; //�e�p���b�g�u���b�N��PalSpd�����F
						//(1��̃��[�v��PalSpd�����������Ȃ�)

	// 0�`9�A246�`255�܂ł̓V�X�e���J���[�̂��ߕύX���Ȃ�
	for (int i = StartPal; i < StartPal + PalEntry; i++)
	{
		//�Ԃ̗v�f���ڕW�̐F���傫���Ȃ�Ό��F
		if (apeH3[i].r >> 16 > apeH1[i].peRed)
		{
			apeH3[i].r = apeH3[i].r - PalSpeed;
			apeH2[i].peRed = apeH3[i].r >> 16;
			//���F�������ʗv�f���ڕW�̐F��菬�����Ȃ����ꍇ�A���̐F���Z�b�g
			if (apeH3[i].r >> 16 <= apeH1[i].peRed)
			{
				apeH3[i].r = apeH1[i].peRed << 16;
				apeH2[i].peRed = apeH1[i].peRed;
			}
		}
		if (apeH3[i].g >> 16 > apeH1[i].peGreen)
		{
			apeH3[i].g = apeH3[i].g - PalSpeed;
			apeH2[i].peGreen = apeH3[i].g >> 16;
			if (apeH3[i].g >> 16 <= apeH1[i].peGreen)
			{
				apeH3[i].g = apeH1[i].peGreen << 16;
				apeH2[i].peGreen = apeH1[i].peGreen;
			}
		}
		if (apeH3[i].b >> 16 > apeH1[i].peBlue)
		{
			apeH3[i].b = apeH3[i].b - PalSpeed;
			apeH2[i].peBlue = apeH3[i].b >> 16;
			if (apeH3[i].b >> 16 <= apeH1[i].peBlue)
			{
				apeH3[i].b = apeH1[i].peBlue << 16;
				apeH2[i].peBlue = apeH1[i].peBlue;
			}
		}
	}

	//�K���ύX����p���b�g�̐擪�A�h���X���w��
	lpDDpal->SetEntries(0, StartPal, PalEntry, &apeH2[10]);

	return FALSE;
}

///////////////////////////////////////////////////////////
// �u���b�N�A�E�g
// ����:�p���b�g��ω�������X�s�[�h
// �߂�l:TRUE(�u���b�N�A�E�g�I��),FALSE(�u���b�N�A�E�g�̓r��)
///////////////////////////////////////////////////////////
BOOL FaInOut::BlackOut(int PalSpeed)
{
	//�t���O�������Ă��鎞�͏I��
	if (BFadeF)
		return TRUE;

	//�p���b�g�̊e�u���b�N��255�񑝐F������I��
	if (PalCnt >> 16 > 255)
	{
		PalCnt = 0;
		BFadeF = 1; //�t���O�𗧂Ă�
		return TRUE;
	}

	PalCnt += PalSpeed; //�e�p���b�g�u���b�N��PalSpd�����F
						//(1��̃��[�v��PalSpd�����������Ȃ�)

	// 0�`9�A246�`255�܂ł̓V�X�e���J���[�̂��ߕύX���Ȃ�
	for (int i = StartPal; i < StartPal + PalEntry; i++)
	{
		//�Ԃ̗v�f��0���傫���Ȃ�Ό��F
		if (apeH3[i].r >> 16 > 0)
		{
			apeH3[i].r = apeH3[i].r - PalSpeed;
			apeH2[i].peRed = apeH3[i].r >> 16;
			//���F�������ʗv�f��0��菬�����Ȃ����ꍇ�A0���Z�b�g
			if (apeH3[i].r >> 16 <= 0)
			{
				apeH3[i].r = 0;
				apeH2[i].peRed = 0;
			}
		}
		if (apeH3[i].g >> 16 > 0)
		{
			apeH3[i].g = apeH3[i].g - PalSpeed;
			apeH2[i].peGreen = apeH3[i].g >> 16;
			if (apeH3[i].g >> 16 <= 0)
			{
				apeH3[i].g = 0;
				apeH2[i].peGreen = 0;
			}
		}
		if (apeH3[i].b >> 16 > 0)
		{
			apeH3[i].b = apeH3[i].b - PalSpeed;
			apeH2[i].peBlue = apeH3[i].b >> 16;
			if (apeH3[i].b >> 16 <= 0)
			{
				apeH3[i].b = 0;
				apeH2[i].peBlue = 0;
			}
		}
	}

	//�K���ύX����p���b�g�̐擪�A�h���X���w��
	lpDDpal->SetEntries(0, StartPal, PalEntry, &apeH2[StartPal]);

	return FALSE;
}

///////////////////////////////////////////////////////////
// �u���b�N�C��
// ����:�p���b�g��ω�������X�s�[�h
// �߂�l:TRUE(�u���b�N�C���I��),FALSE(�u���b�N�C���̓r��)
///////////////////////////////////////////////////////////
BOOL FaInOut::BlackIn(int PalSpeed)
{
	//�t���O�������Ă��Ȃ��ꍇ�I��
	if (!(BFadeF))
		return TRUE;

	if (PalCnt >> 16 > 255) //�p���b�g�̊e�u���b�N��255�񑝐F������I��
	{
		PalCnt = 0;
		BFadeF = 0; //�t���O�N���A
		return TRUE;
	}

	PalCnt += PalSpeed; //�e�p���b�g�u���b�N��PalSpd�����F
						//(1��̃��[�v��PalSpd�����������Ȃ�)

	// 0�`9�A246�`255�܂ł̓V�X�e���J���[�̂��ߕύX���Ȃ�
	for (int i = StartPal; i < StartPal + PalEntry; i++)
	{
		//�Ԃ̗v�f���ڕW�̐F��菬�����Ȃ�Α��F
		if (apeH3[i].r >> 16 < apeH1[i].peRed)
		{
			apeH3[i].r = apeH3[i].r + PalSpeed;
			apeH2[i].peRed = apeH3[i].r >> 16;
			//���F�������ʗv�f���ڕW�̐F�����傫���Ȃ����ꍇ�A�ڕW�̐F���Z�b�g
			if (apeH3[i].r >> 16 >= apeH1[i].peRed)
			{
				apeH3[i].r = apeH1[i].peRed << 16;
				apeH2[i].peRed = apeH1[i].peRed;
			}
		}
		if (apeH3[i].g >> 16 < apeH1[i].peGreen)
		{
			apeH3[i].g = apeH3[i].g + PalSpeed;
			apeH2[i].peGreen = apeH3[i].g >> 16;
			if (apeH3[i].g >> 16 >= apeH1[i].peGreen)
			{
				apeH3[i].g = apeH1[i].peGreen << 16;
				apeH2[i].peGreen = apeH1[i].peGreen;
			}
		}
		if (apeH3[i].b >> 16 < apeH1[i].peBlue)
		{
			apeH3[i].b = apeH3[i].b + PalSpeed;
			apeH2[i].peBlue = apeH3[i].b >> 16;
			if (apeH3[i].b >> 16 >= apeH1[i].peBlue)
			{
				apeH3[i].b = apeH1[i].peBlue << 16;
				apeH2[i].peBlue = apeH1[i].peBlue;
			}
		}
	}

	//�K���ύX����p���b�g�̐擪�A�h���X���w��
	lpDDpal->SetEntries(0, StartPal, PalEntry, &apeH2[StartPal]);

	return FALSE;
}