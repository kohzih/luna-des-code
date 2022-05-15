/*=============================================================================
	��{�N���X�̏����Ɋւ���\�[�X�t�@�C��
==============================================================================*/
#include <windows.h>
#include <math.h>
#include <ddraw.h>

#include "Macros.h"
#include "Draw.h"
#include "DrawMacros.h"

extern HINSTANCE hInst;	 //�C���X�^���X�n���h��
extern HWND hWnd;		 //���C���E�B���h�E�n���h��
extern BOOL fFullscreen; //�t���X�N���[��(1)or�E�B���h�E�t���O(0)

extern LPDIRECTDRAW7 lpDD;
extern LPDIRECTDRAWSURFACE7 lpDDSBack;

LPDIRECTDRAWSURFACE7 Draw::lpDDS_JIKI; //���@�֘A�̉摜�p�T�[�t�F�X
LPDIRECTDRAWSURFACE7 Draw::lpDDS_SP;   //�G�L�����p�摜�p�T�[�t�F�X
LPDIRECTDRAWSURFACE7 Draw::lpDDS_BG;   //�w�i�摜�p�T�[�t�F�X
LPDIRECTDRAWSURFACE7 Draw::lpDDS_BG2;  //�w�i�摜�p�T�[�t�F�X2
POINT Draw::MainCdn;				   //�Q�[�����̐�΍��W
DWORD Draw::fSTAGE;					   //�e�X�e�[�W�̊J�n�I���������t���O
int Draw::StageNum;					   //���݂̃X�e�[�W�ԍ�
DWORD Draw::MapSizeX;				   // BG�}�b�v�f�[�^�̉��T�C�Y�i���ۂ̒l��MapSizeX*ChipSizeX�h�b�g�j
DWORD Draw::MapSizeY;				   // BG�}�b�v�f�[�^�̏c�T�C�Y�i���ۂ̒l��MapSizeY*ChipSizeY�h�b�g�j
DWORD Draw::TipSizeX;				   // BG�`�b�v��̉��̃T�C�Y
DWORD Draw::TipSizeY;				   // BG�`�b�v��̏c�̃T�C�Y
BGMAP *Draw::Parts;					   // BG�}�b�v�\���̔z��̐擪�A�h���X
double *Draw::Sin;					   // 0-361�x���̃T�C���l�z��̐擪�A�h���X���i�[����|�C���^
double *Draw::Cos;					   // 0-361�x���̃R�T�C���l�z��̐擪�A�h���X���i�[����|�C���^
double *Draw::Tan;					   // 0-89�x���̃^���W�F���g�l�z��̐擪�A�h���X���i�[����|�C���^
DWORD Draw::EventCnt;				   // Parts�\���̔z��Ɋ܂܂��C�x���g(DrawSP�I�u�W�F�N�g)�̐�
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

Draw::Draw()
{
}

Draw::Draw(char **FileName, HRESULT &ret)
{
	lpDDS_JIKI = NULL; //�e�T�[�t�F�X�̏�����
	lpDDS_SP = NULL;
	lpDDS_BG = NULL;
	lpDDS_BG2 = NULL;

	fSTAGE = 0;					   //�t���O���N���A
	fSTAGE |= _STAGE_STARTWAIT_ON; //���̃X�e�[�W���n�܂������Ƃ������t���O�I��
	StageNum = _StNum;			   //�ŏ��ɃX�^�[�g����X�e�[�W�ԍ�

	MainCdn.x = 0; //��΍��W�̏�����
	MainCdn.y = 0;
	MapSizeX = 0; //�w�i�}�b�v�f�[�^�̉��T�C�Y�i���ۂ̒l��MapSizeX*ChipSizeX�h�b�g�j
	MapSizeY = 0; //�w�i�}�b�v�f�[�^�̏c�T�C�Y�i���ۂ̒l��MapSizeY*ChipSizeY�h�b�g�j
	TipSizeX = 0; //�w�i�`�b�v��̉��̃T�C�Y
	TipSizeY = 0; //�w�i�`�b�v��̏c�̃T�C�Y

	Parts = NULL;
	Sin = NULL;
	Cos = NULL;
	Tan = NULL;

	EventCnt = 0; //�}�b�v��ɔz�u����Ă���C�x���g��������

	JIKIFileName = NULL; //�t�@�C���l�[���ۑ��p�̃|�C���^��������
	PrologFileName = NULL;
	for (short i = 0; i < 2; i++)
	{
		TitleFileName[i] = NULL;
	}
	for (i = 0; i < 4; i++)
	{
		St1FileName[i] = NULL;
	}
	GameOverFileName = NULL;
	GameClearFileName = NULL;

	//�ǂݍ��񂾃r�b�g�}�b�v�̖��O���R�s�[
	int len; //������̒������i�[����ϐ�

	//���@�`��p�r�b�g�}�b�v�t�@�C���l�[��
	len = strlen(FileName[0]);
	JIKIFileName = new char[len + 1];
	if (JIKIFileName == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(JIKIFileName, FileName[0]);

	//�v�����[�O�p�t�@�C���l�[��
	len = strlen(FileName[1]);
	PrologFileName = new char[len + 1];
	if (PrologFileName == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(PrologFileName, FileName[1]);

	//�^�C�g���p�t�@�C���l�[��
	len = strlen(FileName[2]);
	TitleFileName[0] = new char[len + 1];
	if (TitleFileName[0] == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(TitleFileName[0], FileName[2]);

	len = strlen(FileName[3]);
	TitleFileName[1] = new char[len + 1];
	if (TitleFileName[1] == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(TitleFileName[1], FileName[3]);

	//�X�e�[�W1�p�e�t�@�C���l�[��
	len = strlen(FileName[4]);
	St1FileName[0] = new char[len + 1];
	if (St1FileName[0] == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(St1FileName[0], FileName[4]);

	len = strlen(FileName[5]);
	St1FileName[1] = new char[len + 1];
	if (St1FileName[1] == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(St1FileName[1], FileName[5]);

	len = strlen(FileName[6]);
	St1FileName[2] = new char[len + 1];
	if (St1FileName[2] == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(St1FileName[2], FileName[6]);

	len = strlen(FileName[7]);
	St1FileName[3] = new char[len + 1];
	if (St1FileName[3] == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(St1FileName[3], FileName[7]);

	//�Q�[���I�[�o�[��ʕ`��p�r�b�g�}�b�v�t�@�C����
	len = strlen(FileName[8]);
	GameOverFileName = new char[len + 1];
	if (GameOverFileName == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(GameOverFileName, FileName[8]);

	//�Q�[���N���A��ʗp�r�b�g�}�b�v�t�@�C����
	len = strlen(FileName[9]);
	GameClearFileName = new char[len + 1];
	if (GameClearFileName == NULL)
	{
		ret = FALSE;
		return;
	}
	strcpy(GameClearFileName, FileName[9]);

	//�T�C���A�R�T�C���A�^���W�F���g�l���i�[����̈���m��
	Sin = new double[361];
	if (Sin == NULL)
	{
		ret = FALSE;
		return;
	}
	Cos = new double[361];
	if (Cos == NULL)
	{
		ret = FALSE;
		return;
	}
	Tan = new double[90];
	if (Tan == NULL)
	{
		ret = FALSE;
		return;
	}
	//�T�C���R�T�C���l�����߂�
	for (i = 0; i < 361; i++)
	{
		Cos[i] = cos(3.1415926535 / 180 * i);
		Sin[i] = sin(3.1415926535 / 180 * i);
	}
	//�^���W�F���g�l�����߂�
	for (i = 0; i < 90; i++)
	{
		Tan[i] = tan(3.1415926535 / 180 * i);
	}
	//�����𗘗p�����߂�l
	ret = TRUE;
}

Draw::~Draw()
{
}

//////////////////////////////////////////////////////////////////////
// Draw�N���X�̊e�����o��������
//////////////////////////////////////////////////////////////////////
BOOL Draw::Init()
{
	//�X�e�[�W�𕡐�������ꍇ�ɕK�v�ȏ���
	RELEASE(lpDDS_JIKI);
	RELEASE(lpDDS_SP);
	RELEASE(lpDDS_BG);
	RELEASE(lpDDS_BG2);
	RELEASE_dels(Parts);
	EventCnt = 0;

	BOOL ret = TRUE;

	switch (StageNum)
	{
	//�v�����[�O��ʂ̏����ݒ�
	case 0:
		ret = LoadBitMap(PrologFileName, lpDDS_BG); // lpDDS_BG�T�[�t�F�X�̍쐬
		if (!ret)
			return FALSE;
		break;
	//�^�C�g����ʂ̏����ݒ�
	case 1:
		ret = LoadBitMap(TitleFileName[0], lpDDS_BG); // lpDDS_BG�T�[�t�F�X�̍쐬
		if (!ret)
			return FALSE;
		ret = LoadBitMap(TitleFileName[1], lpDDS_BG2);
		if (!ret)
			return FALSE;
		break;
	//�X�e�[�W1�̎��̏����ݒ�
	case 2:
		ret = LoadBitMap(JIKIFileName, lpDDS_JIKI); // lpDDS_JIKI�T�[�t�F�X�̍쐬
		if (!ret)
			return FALSE;
		ret = LoadBitMap(St1FileName[0], lpDDS_SP); // lpDDS_SP�T�[�t�F�X�̍쐬
		if (!ret)
			return FALSE;
		ret = LoadBitMap(St1FileName[1], lpDDS_BG); // lpDDS_BG�T�[�t�F�X�̍쐬
		if (!ret)
			return FALSE;
		ret = LoadBitMap(St1FileName[2], lpDDS_BG2); // lpDDS_BG2�T�[�t�F�X�̍쐬
		if (!ret)
			return FALSE;
		ret = LoadMapData(St1FileName[3]); //�}�b�v�f�[�^�����[�h
		if (!ret)
			return FALSE;

		//���݂̃}�b�v�f�[�^����Q�[���J�n�̏������W���v�Z
		MainCdn.x = 0;
		MainCdn.y = (MapSizeY * TipSizeY) - WinYMax;
		break;
	//�Q�[���I�[�o�[��ʗp�̏����ݒ�
	case 1000:
		ret = LoadBitMap(GameOverFileName, lpDDS_BG); // lpDDS_BG�T�[�t�F�X�̍쐬
		if (!ret)
			return FALSE;
		break;
	//�Q�[���N���A��ʗp�̏����ݒ�
	case 2000:
		ret = LoadBitMap(GameClearFileName, lpDDS_BG); // lpDDS_BG�T�[�t�F�X�̍쐬
		if (!ret)
			return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
// Draw::Init(int);�Ŋm�ۂ����������̈���������
//////////////////////////////////////////////////////////////////////
BOOL Draw::ReleaseALL()
{
	RELEASE(lpDDS_JIKI);
	RELEASE(lpDDS_SP);
	RELEASE(lpDDS_BG);
	RELEASE(lpDDS_BG2);
	RELEASE_dels(JIKIFileName);
	RELEASE_dels(PrologFileName);
	for (short i = 0; i < 2; i++)
	{
		RELEASE_dels(TitleFileName[i]);
	}
	for (i = 0; i < 4; i++)
	{
		RELEASE_dels(St1FileName[i]);
	}
	RELEASE_dels(GameOverFileName);
	RELEASE_dels(GameClearFileName);
	RELEASE_dels(Parts);
	RELEASE_dels(Tan);
	RELEASE_dels(Cos);
	RELEASE_dels(Sin);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	���@�`��p�T�[�t�F�X�̃��X�g���`�F�b�N����֐�
//////////////////////////////////////////////////////////////////////
BOOL Draw::RestoreJIKI()
{
	HRESULT ret;

	//�X�e�[�W�ԍ��ɂ���ď����𕪊�
	switch (StageNum)
	{
	case 0:
	case 1:
		break;
	case 2:
		if (!lpDDS_JIKI)
			return FALSE;

		ret = lpDDS_JIKI->IsLost();
		if (ret == DD_OK)
			break; //�T�[�t�F�X�����X�g���Ă��Ȃ����`�F�b�N

		ret = lpDDS_JIKI->Restore(); // lpDDS_SP�T�[�t�F�X�̃��X�g�A
		if (ret != DD_OK)
			return FALSE;
		ret = LoadBitMap(JIKIFileName, lpDDS_JIKI); //�摜�������[�h
		if (ret != DD_OK)
			return FALSE;
		break;
	case 1000:
	case 2000:
		break;
	default:
		break;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// �X�v���C�g�T�[�t�F�X�̃��X�g���`�F�b�N����֐�
//////////////////////////////////////////////////////////////////////

BOOL Draw::RestoreSP()
{
	HRESULT ret;

	//�X�e�[�W�ԍ��ɂ���ă��X�g�A�����𕪊�
	switch (StageNum)
	{
	case 0:
	case 1:
		break;
	case 2:
		if (!lpDDS_SP)
			return FALSE;

		ret = lpDDS_SP->IsLost();
		if (ret == DD_OK)
			break; //�T�[�t�F�X�����X�g���Ă��Ȃ����`�F�b�N

		ret = lpDDS_SP->Restore(); // lpDDS_SP�T�[�t�F�X�̃��X�g�A
		if (ret != DD_OK)
			return FALSE;
		ret = LoadBitMap(St1FileName[0], lpDDS_SP); //�摜�������[�h
		if (ret != DD_OK)
			return FALSE;

		break;
	case 1000:
	case 2000:
		break;
	default:
		break;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Draw::lpDDS_BG�T�[�t�F�X�����X�g�A����֐�
//////////////////////////////////////////////////////////////////////

BOOL Draw::RestoreBG()
{
	HRESULT ret;

	//�X�e�[�W�ԍ��ɂ���ă��X�g�A�����𕪊�
	switch (StageNum)
	{
	case 0:
		if (!lpDDS_BG)
			return FALSE;

		ret = lpDDS_BG->IsLost();
		if (ret != DD_OK)
		{
			ret = lpDDS_BG->Restore(); // lpDDS_BG�T�[�t�F�X�̃��X�g�A
			if (ret != DD_OK)
				return FALSE;
			ret = LoadBitMap(PrologFileName, lpDDS_BG); //�摜�������[�h
			if (ret != DD_OK)
				return FALSE;
		}
		break;
	case 1:
		if (!lpDDS_BG)
			return FALSE;
		if (!lpDDS_BG2)
			return FALSE;

		ret = lpDDS_BG->IsLost();
		if (ret != DD_OK)
		{
			ret = lpDDS_BG->Restore(); // lpDDS_BG�T�[�t�F�X�̃��X�g�A
			if (ret != DD_OK)
				return FALSE;
			ret = LoadBitMap(TitleFileName[0], lpDDS_BG); //�摜�������[�h
			if (ret != DD_OK)
				return FALSE;
		}
		ret = lpDDS_BG2->IsLost();
		if (ret != DD_OK)
		{
			ret = lpDDS_BG2->Restore(); // lpDDS_BG�T�[�t�F�X�̃��X�g�A
			if (ret != DD_OK)
				return FALSE;
			ret = LoadBitMap(TitleFileName[1], lpDDS_BG2); //�摜�������[�h
			if (ret != DD_OK)
				return FALSE;
		}
		break;
	case 2:
		if (!lpDDS_BG)
			return FALSE;
		if (!lpDDS_BG2)
			return FALSE;

		ret = lpDDS_BG->IsLost();
		if (ret != DD_OK)
		{
			ret = lpDDS_BG->Restore(); // lpDDS_BG�T�[�t�F�X�̃��X�g�A
			if (ret != DD_OK)
				return FALSE;
			ret = LoadBitMap(St1FileName[1], lpDDS_BG); //�摜�������[�h
			if (ret != DD_OK)
				return FALSE;
		}
		ret = lpDDS_BG2->IsLost();
		if (ret != DD_OK)
		{
			ret = lpDDS_BG2->Restore(); // lpDDS_BG�T�[�t�F�X�̃��X�g�A
			if (ret != DD_OK)
				return FALSE;
			ret = LoadBitMap(St1FileName[2], lpDDS_BG2); //�摜�������[�h
			if (ret != DD_OK)
				return FALSE;
		}
		break;
	case 1000:
		if (!lpDDS_BG)
			return FALSE;
		ret = lpDDS_BG->IsLost();
		if (ret != DD_OK)
		{
			ret = lpDDS_BG->Restore(); // lpDDS_BG�T�[�t�F�X�̃��X�g�A
			if (ret != DD_OK)
				return FALSE;
			ret = LoadBitMap(GameOverFileName, lpDDS_BG); //�摜�������[�h
			if (ret != DD_OK)
				return FALSE;
		}
		break;
	case 2000:
		if (!lpDDS_BG)
			return FALSE;
		ret = lpDDS_BG->IsLost();
		if (ret != DD_OK)
		{
			ret = lpDDS_BG->Restore(); // lpDDS_BG�T�[�t�F�X�̃��X�g�A
			if (ret != DD_OK)
				return FALSE;
			ret = LoadBitMap(GameClearFileName, lpDDS_BG); //�摜�������[�h
			if (ret != DD_OK)
				return FALSE;
		}
		break;
	default:
		break;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// �I�t�X�N���[���T�[�t�F�X�����
//////////////////////////////////////////////////////////////////////
BOOL Draw::ReleaseSuf()
{
	RELEASE(lpDDS_JIKI);
	RELEASE(lpDDS_SP);
	RELEASE(lpDDS_BG);
	RELEASE(lpDDS_BG2);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// �I�t�X�N���[���T�[�t�F�X���č\�z
//////////////////////////////////////////////////////////////////////
BOOL Draw::ReloadSuf()
{
	HRESULT ret;

	switch (StageNum)
	{
	//�v�����[�O��ʗp�̃T�[�t�F�X�̃����[�h
	case 0:
		ret = LoadBitMap(PrologFileName, lpDDS_BG); // lpDDS_BG�T�[�t�F�X�̍쐬
		if (!ret)
			return FALSE;
		break;
	//�^�C�g����ʎ��̃T�[�t�F�X�̃����[�h
	case 1:
		ret = LoadBitMap(TitleFileName[0], lpDDS_BG); // lpDDS_BG�T�[�t�F�X�̍쐬
		if (!ret)
			return FALSE;
		ret = LoadBitMap(TitleFileName[1], lpDDS_BG2); // lpDDS_BG2�T�[�t�F�X�̍쐬
		if (!ret)
			return FALSE;
		break;
	//�X�e�[�W1
	case 2:
		ret = LoadBitMap(JIKIFileName, lpDDS_JIKI); // lpDDS_JIKI�T�[�t�F�X�̍쐬
		if (!ret)
			return FALSE;
		ret = LoadBitMap(St1FileName[0], lpDDS_SP); // lpDDS_SP�T�[�t�F�X�̍쐬
		if (!ret)
			return FALSE;
		ret = LoadBitMap(St1FileName[1], lpDDS_BG); // lpDDS_BG�T�[�t�F�X�̍쐬
		if (!ret)
			return FALSE;
		ret = LoadBitMap(St1FileName[2], lpDDS_BG2); // lpDDS_BG2�T�[�t�F�X�̍쐬
		if (!ret)
			return FALSE;
		break;
	//�Q�[���I�[�o�[
	case 1000:
		ret = LoadBitMap(GameOverFileName, lpDDS_BG); // lpDDS_BG�T�[�t�F�X�̍쐬
		if (!ret)
			return FALSE;
		break;
	//�Q�[���N���A
	case 2000:
		ret = LoadBitMap(GameClearFileName, lpDDS_BG); // lpDDS_BG�T�[�t�F�X�̍쐬
		if (!ret)
			return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// �r�b�g�}�b�v�����[�h����֐�
//////////////////////////////////////////////////////////////////////
BOOL Draw::LoadBitMap(char *FileName, LPDIRECTDRAWSURFACE7 &lpDDsuf)
{
	//�r�b�g�}�b�v���[�h
	HBITMAP hBit;
	BITMAP bm;

	hBit = (HBITMAP)LoadImage(hInst,								  //�C���X�^���X�n���h��
							  FileName,								  //�t�@�C����
							  IMAGE_BITMAP,							  // IMAGE_BITMAP�w��
							  0, 0,									  //�ǂݍ��ޕ��ƍ���(0�w��őS�ēǂݍ���)
							  LR_CREATEDIBSECTION | LR_LOADFROMFILE); //�t���O
	//�t�@�C���I�[�v�����s
	if (!hBit)
	{
		MessageBox(hWnd, "File Open Error", FileName, MB_OK);
		return FALSE;
	}

	GetObject(hBit, sizeof(bm), &bm);

	//�p���b�g���Q�T�U�F�ȊO�̃r�b�g�}�b�v�����[�h�����ꍇ
	if (!(bm.bmBitsPixel == 8))
	{
		MessageBox(hWnd, "File Open Error(Not 8bit BitMap)", FileName, MB_OK | MB_APPLMODAL);
		return FALSE;
	}

	HRESULT ret;
	LPDIRECTDRAWSURFACE7 lpDDtemp = NULL;
	DDSURFACEDESC2 ddsd;

	//�I�t�X�N���[���T�[�t�F�X�����݂��Ȃ���΍쐬
	if (lpDDsuf == NULL)
	{
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		ddsd.dwWidth = bm.bmWidth;
		ddsd.dwHeight = (bm.bmHeight < 0 ? bm.bmHeight * -1 : bm.bmHeight);
		if (fFullscreen)
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		else
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		ret = lpDD->CreateSurface(&ddsd, &lpDDsuf, 0);
		if (ret != DD_OK)
			return FALSE;

		//�����F�̐ݒ�(�p���b�g�O��)
		DDCOLORKEY ddck;

		ZeroMemory(&ddck, sizeof(ddck));
		ddck.dwColorSpaceLowValue = 0;
		ddck.dwColorSpaceHighValue = 0;
		ret = lpDDsuf->SetColorKey(DDCKEY_SRCBLT, &ddck);
		if (ret != DD_OK)
			return FALSE;
	}

	//�ǂݍ��񂾃r�b�g�}�b�v���I�t�X�N���[���T�[�t�F�X�ɓo�^

	//�r�f�I��������2M�ȉ��̃r�f�I�J�[�h�̃T�[�t�F�X���b�N�G���[�΍��p����
	//��x�V�X�e���������ɃI�t�X�N���[���T�[�t�F�X������ē]�����A
	//�����]�����邱�Ƃŉ摜��ǂݍ���
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ret = lpDDsuf->GetSurfaceDesc(&ddsd); //�]����̃T�[�t�F�X�̋L�q���擾
	if (ret != DD_OK)
		return FALSE;

	//�������̊m�ې���V�X�e���������ɋ���������
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

	//�ꎞ��Ɨp�I�t�X�N���[���T�[�t�F�X�쐬
	ret = lpDD->CreateSurface(&ddsd, &lpDDtemp, 0);
	if (ret != DD_OK)
		return FALSE;

	//�ꎞ��Ɨp�I�t�X�N���[���T�[�t�F�X�̓����F�̐ݒ�(�p���b�g�O��)
	DDCOLORKEY ddck_t;

	ZeroMemory(&ddck_t, sizeof(ddck_t));
	ddck_t.dwColorSpaceLowValue = 0;
	ddck_t.dwColorSpaceHighValue = 0;
	ret = lpDDtemp->SetColorKey(DDCKEY_SRCBLT, &ddck_t);
	if (ret != DD_OK)
		return FALSE;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ret = lpDDtemp->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL); //�����������b�N
	if (ret != DD_OK)
		return FALSE;

	char *lpDst, //�摜�]����
		*lpSrc;	 //�摜�]����

	lpSrc = (char *)bm.bmBits;

	//�ǂݍ��񂾉摜���t���܂ɓo�^����Ă����̎��̏���
	if (bm.bmHeight > 0)
	{
		lpDst = (char *)ddsd.lpSurface + ddsd.lPitch * ddsd.dwHeight;
		for (long i = 0; (UINT)i < ddsd.dwHeight; i++)
		{
			lpDst -= ddsd.lPitch;
			for (long j = 0; j < ddsd.lPitch; j++)
			{
				if (*lpSrc)
					*lpDst = *lpSrc + 10; //�p���b�g���P�O�i�߂���Ԃœo�^
				else
					*lpDst = *lpSrc; //�O�̎��͓����F�̂��߂��̂܂ܓo�^
				lpDst++;
				lpSrc++;
			}
			lpDst -= ddsd.lPitch;
		}
	}
	else
	{
		lpDst = (char *)ddsd.lpSurface;
		for (int i = 0; (UINT)i < ddsd.lPitch * ddsd.dwHeight; i++)
		{
			if (*lpSrc)
				*lpDst = *lpSrc + 10; //�p���b�g���P�O�i�߂���Ԃœo�^
			else
				*lpDst = *lpSrc; //�O�̎��͓����F�̂��߂��̂܂ܓo�^
			lpDst++;
			lpSrc++;
		}
	}

	lpDDtemp->Unlock(0); //���������b�N�̉���
	DeleteObject(hBit);	 //�I�u�W�F�N�g�̍폜

	ret = lpDDsuf->Blt(NULL, lpDDtemp, NULL, DDBLT_WAIT, 0); //�ǂݍ��񂾉摜��]��
	if (ret != DD_OK)
		return FALSE;

	RELEASE(lpDDtemp); //�ꎞ��Ɨp�I�u�W�F�N�g���
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// �}�b�v�f�[�^��ǂݍ��ފ֐�
//////////////////////////////////////////////////////////////////////
BOOL Draw::LoadMapData(char *FileName)
{
	//�t�@�C���I�[�v��
	SetLastError(0);

	HANDLE hFile;

	hFile = CreateFile(FileName,
					   GENERIC_READ, 0, NULL,
					   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//�I�[�v���Ɏ��s
	if (GetLastError())
	{
		MessageBox(hWnd, "File Open Error", "�t�@�C����������܂���", MB_OK);
		CloseHandle(hFile);
		return FALSE;
	}

	DWORD ByteCount;
	//�w�b�_�[�������W�����v
	SetFilePointer(hFile, 48, NULL, FILE_BEGIN);
	//�}�b�v�f�[�^�̃t�H�[�}�b�g������ǂݍ���
	ReadFile(hFile, &MapSizeX, sizeof(MapSizeX), &ByteCount, NULL);
	ReadFile(hFile, &MapSizeY, sizeof(MapSizeY), &ByteCount, NULL);
	ReadFile(hFile, &TipSizeX, sizeof(TipSizeX), &ByteCount, NULL);
	ReadFile(hFile, &TipSizeY, sizeof(TipSizeY), &ByteCount, NULL);
	//�f�[�^�ǂݍ��ݗ̈�̊m��
	Parts = new BGMAP[MapSizeX * MapSizeY];
	if (!Parts)
		return FALSE;
	//�f�[�^��ǂݍ���
	ReadFile(hFile, Parts, sizeof(BGMAP) * MapSizeX * MapSizeY, &ByteCount, NULL);

	//�t�@�C���N���[�Y
	CloseHandle(hFile);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ���݂̃}�b�v���\������BGMAP�\���̂̐������߂�
//////////////////////////////////////////////////////////////////////
BOOL Draw::GetMapSizeData(DWORD &ms, DWORD &my, DWORD &cx, DWORD &cy)
{
	ms = MapSizeX;
	my = MapSizeY;
	cx = TipSizeX;
	cy = TipSizeY;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// �����Ŏw�肳�ꂽ�ꏊ��BGMAP�\���̂̃C�x���g�f�[�^�����߂�
//////////////////////////////////////////////////////////////////////
BOOL Draw::GetEventData(int i, WORD &event)
{
	event = Parts[i].Evt;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// �����œn���ꂽ�t���O���Z�b�g����
// ����:�X�e�[�W�t���O,set=0:AND set=1:OR �ŃZ�b�g����
//////////////////////////////////////////////////////////////////////
void Draw::SetfSTAGE(DWORD flag, int set)
{
	switch (set)
	{
	case 0:
		fSTAGE &= flag;
		break;
	case 1:
		fSTAGE |= flag;
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////
// ���݂̃X�e�[�W�t���O��Ԃ�
// �߂�l:�X�e�[�W�t���O
//////////////////////////////////////////////////////////////////////
DWORD Draw::GetfSTAGE()
{
	return fSTAGE;
}

//////////////////////////////////////////////////////////////////////
// ���݂̃X�e�[�W�ԍ���Ԃ�
// �߂�l:�X�e�[�W�ԍ�
//////////////////////////////////////////////////////////////////////
int Draw::GetStageNum()
{
	return StageNum;
}

//////////////////////////////////////////////////////////////////////
// �����œn���ꂽ�X�e�[�W�ԍ����Z�b�g
// ����:�X�e�[�W�ԍ�
//////////////////////////////////////////////////////////////////////
void Draw::SetStageNum(int StNum)
{
	StageNum = StNum;
}

//////////////////////////////////////////////////////////////////////
// �^���W�F���g�̒l���p�x�ɒ����B�����̓^���W�F���g�l�B�߂�l�͊p�x�B
//////////////////////////////////////////////////////////////////////
short Draw::TanKaku(double tan)
{
	short i = 0;

	if (tan <= Tan[9])
	{
		for (i = 0; i < 10; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan <= Tan[19])
	{
		for (i = 10; i < 20; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan <= Tan[29])
	{
		for (i = 20; i < 30; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan <= Tan[39])
	{
		for (i = 30; i < 40; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan <= Tan[49])
	{
		for (i = 40; i < 50; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan <= Tan[59])
	{
		for (i = 50; i < 60; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan <= Tan[69])
	{
		for (i = 60; i < 70; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan <= Tan[79])
	{
		for (i = 70; i < 80; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan <= Tan[89])
	{
		for (i = 80; i < 90; i++)
		{
			if (tan <= Tan[i])
				return (i);
		}
	}
	else if (tan > Tan[89])
	{
		return (89); // tan�l��89�x���傫���ꍇ��89��Ԃ�
	}
	return 0;
}