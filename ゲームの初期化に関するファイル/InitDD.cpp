/*====================================================================
	DirectDraw�������Ɋւ���\�[�X�t�@�C��
=====================================================================*/

#include <windows.h>
#include <ddraw.h>

#include "Macros.h"

extern HINSTANCE hInst;	   //�C���X�^���X�n���h��
extern HWND hWnd;		   //���C���E�B���h�E�n���h��
extern BOOL fFullscreen;   //�t���X�N���[��(1)or�E�B���h�E�t���O(0)
extern DWORD DisplayColor; //�X�N���[���̌��݂̃r�b�g�J���[�l

extern LPDIRECTDRAW7 lpDD;				  // DirectDraw�I�u�W�F�N�g
extern LPDIRECTDRAWSURFACE7 lpDDSPrimary, //�v���C�}���T�[�t�F�[�X
	lpDDSBack;							  //�o�b�N�T�[�t�F�[�X
extern LPDIRECTDRAWPALETTE lpDDpal;		  // DirectDraw�p���b�g�I�u�W�F�N�g
extern LPDIRECTDRAWCLIPPER lpDDClip;	  //�N���b�p�[(�E�B���h�E�\���p)

PALETTEENTRY ape[256]; // 256�F�p���b�g

BOOL FullChgWindow(void);
BOOL LoadPalette(char *);
//�񋓊֐��̃v���g�^�C�v�錾
HRESULT WINAPI EnumModesCallback2(LPDDSURFACEDESC2, LPVOID);

//---------------------------------------------------------
// DirectDraw������
//---------------------------------------------------------
BOOL InitDD()
{

	HRESULT ret;
	DDSURFACEDESC2 ddsd;
	DDSCAPS2 ddsCaps;

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////

	// DD�I�u�W�F�N�g�쐬
	ret = DirectDrawCreateEx(NULL, (void **)&lpDD, IID_IDirectDraw7, NULL);
	if (ret != DD_OK)
	{
		MessageBox(hWnd, "001", "�G���[", MB_OK | MB_APPLMODAL);
		return FALSE;
	}

	//�w�肳�ꂽ��ʃ��[�h���g�p�\���`�F�b�N
	HRESULT ret2;

	ret2 = lpDD->EnumDisplayModes(DDEDM_REFRESHRATES,
								  NULL, (LPVOID)&ret,
								  EnumModesCallback2);
	if (ret2 == DDERR_INVALIDOBJECT)
		MessageBox(hWnd, "DDERR_INVALIDOBJECT", "ret2", MB_OK | MB_APPLMODAL);
	if (ret2 == DDERR_INVALIDPARAMS)
		MessageBox(hWnd, "DDERR_INVALIDPARAMS", "ret2", MB_OK | MB_APPLMODAL);

	if (ret != TRUE)
	{
		MessageBox(hWnd,
				   "�c�O�Ȃ���Q�[���̓�����𖞂����Ă��܂���",
				   "�Q�[�����ł��܂���I�I",
				   MB_OK | MB_APPLMODAL);
		return FALSE;
	}

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////

	//�������x���ݒ�
	//�t���X�N���[�����[�h
	if (fFullscreen)
	{
		ret = lpDD->SetCooperativeLevel(hWnd,
										DDSCL_FULLSCREEN |
											DDSCL_EXCLUSIVE |
											DDSCL_ALLOWMODEX |
											DDSCL_ALLOWREBOOT);
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "002", "�G���[", MB_OK | MB_APPLMODAL);
			return FALSE;
		}
		//�f�B�X�v���C���[�h�ݒ�
		ret = lpDD->SetDisplayMode(WinXMax, WinYMax, BPP, 0, 0);
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "003", "�G���[", MB_OK | MB_APPLMODAL);
			return FALSE;
		}
	}
	//�E�B���h�E���[�h
	else
	{
		ret = lpDD->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "004", "�G���[", MB_OK | MB_APPLMODAL);
			return FALSE;
		}
	}

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////

	//�v���C�}���T�[�t�F�X�̍쐬

	//�t���X�N���[�����[�h
	if (fFullscreen)
	{

		//�v���C�}���T�[�t�F�X�̍쐬
		memset(&ddsd, 0, sizeof(ddsd)); // ddsd�̒���0�ŏ�����
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;	   //�o�b�N�o�b�t�@�쐬
		ddsd.dwBackBufferCount = 1;							   //�o�b�N�o�b�t�@��
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |		   //�v���C�}���T�[�t�F�[�X�w��
							  DDSCAPS_COMPLEX |				   //�����T�[�t�F�[�X�w��
							  DDSCAPS_FLIP;					   //�t���b�v�w��
		ret = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL); //�T�[�t�F�[�X�w��
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "005", "�G���[", MB_OK | MB_APPLMODAL);
			return FALSE;
		}

		//�o�b�N�T�[�t�F�X���쐬
		ZeroMemory(&ddsCaps, sizeof(ddsCaps));
		ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
		ret = lpDDSPrimary->GetAttachedSurface(&ddsCaps, &lpDDSBack);
		//�����T�[�t�F�X�̃o�b�N�T�[�t�F�[�X���擾
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "006", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_INVALIDOBJECT)
				MessageBox(hWnd, "DDERR_INVALIDOBJECT", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_INVALIDPARAMS)
				MessageBox(hWnd, "DDERR_INVALIDPARAMS", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOTFOUND)
				MessageBox(hWnd, "DDERR_NOTFOUND", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_SURFACELOST)
				MessageBox(hWnd, "DDERR_SURFACELOST", "�G���[", MB_OK | MB_APPLMODAL);
			return FALSE;
		}
		// 1�s�N�Z��������̃r�b�g�����Z�b�g
		DisplayColor = BPP;
	}
	//�E�B���h�E���[�h
	else
	{
		//�v���C�}���T�[�t�F�X�̍쐬
		memset(&ddsd, 0, sizeof(ddsd)); // ddsd�̒���0�ŏ�����
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |		   //�v���C�}���T�[�t�F�[�X�w��
							  DDSCAPS_SYSTEMMEMORY;			   //�V�X�e���������Ɋm��
		ret = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL); //�T�[�t�F�[�X�w��
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "007", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_INCOMPATIBLEPRIMARY)
				MessageBox(hWnd, "A", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_INVALIDCAPS)
				MessageBox(hWnd, "B", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_INVALIDOBJECT)
				MessageBox(hWnd, "C", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_INVALIDPARAMS)
				MessageBox(hWnd, "D", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_INVALIDPIXELFORMAT)
				MessageBox(hWnd, "E", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOALPHAHW)
				MessageBox(hWnd, "F", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOCOOPERATIVELEVELSET)
				MessageBox(hWnd, "G", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NODIRECTDRAWHW)
				MessageBox(hWnd, "H", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOEMULATION)
				MessageBox(hWnd, "I", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOEXCLUSIVEMODE)
				MessageBox(hWnd, "J", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOFLIPHW)
				MessageBox(hWnd, "K", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOMIPMAPHW)
				MessageBox(hWnd, "L", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOOVERLAYHW)
				MessageBox(hWnd, "M", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_NOZBUFFERHW)
				MessageBox(hWnd, "N", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_OUTOFMEMORY)
				MessageBox(hWnd, "O", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_OUTOFVIDEOMEMORY)
				MessageBox(hWnd, "P", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_PRIMARYSURFACEALREADYEXISTS)
				MessageBox(hWnd, "Q", "�G���[", MB_OK | MB_APPLMODAL);
			if (ret == DDERR_UNSUPPORTEDMODE)
				MessageBox(hWnd, "R", "�G���[", MB_OK | MB_APPLMODAL);

			return FALSE;
		}

		//���݂̉�ʂ̃J���[�ݒ���擾
		ddsd.dwSize = sizeof(ddsd);
		lpDDSPrimary->GetSurfaceDesc(&ddsd);
		DisplayColor = ddsd.ddpfPixelFormat.dwRGBBitCount;
		//���݂̃f�B�X�v���X���[�h��256�F���[�h�ȊO�̏ꍇ�̏���
		if (!(DisplayColor == 8))
		{
			MessageBox(hWnd,
					   "�t���X�N���[�����[�h�ֈڍs���܂�",
					   "��ʂ�256�F���[�h�ł͂���܂���",
					   MB_OK | MB_APPLMODAL);
			fFullscreen = 1; //�t���O���t���X�N���[�����[�h�ɃZ�b�g
			RELEASE(lpDDSPrimary);
			RELEASE(lpDD);

			return InitDD(); //����������蒼��
		}

		//�o�b�N�T�[�t�F�X�I�I�t�X�N���[���T�[�t�F�X�̍쐬
		memset(&ddsd, 0, sizeof(ddsd)); // ddsd�̒���0�ŏ�����
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		ddsd.dwWidth = WinXMax;
		ddsd.dwHeight = WinYMax;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		ret = lpDD->CreateSurface(&ddsd, &lpDDSBack, NULL);
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "008", "�G���[", MB_OK | MB_APPLMODAL);
			return FALSE;
		}

		//�N���b�p�[�̐ݒ�
		ret = lpDD->CreateClipper(0, &lpDDClip, NULL);
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "009", "�G���[", MB_OK | MB_APPLMODAL);
			return FALSE;
		}
		ret = lpDDClip->SetHWnd(0, hWnd);
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "010", "�G���[", MB_OK | MB_APPLMODAL);
			return FALSE;
		}
		ret = lpDDSPrimary->SetClipper(lpDDClip);
		if (ret != DD_OK)
		{
			MessageBox(hWnd, "011", "�G���[", MB_OK | MB_APPLMODAL);
			return FALSE;
		}
	}

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////

	//�v���C�}���T�[�t�F�X�Ƀp���b�g��ݒ�
	ret = LoadPalette(".\\BitMap\\Prolog.bmp");
	if (!ret)
	{
		MessageBox(hWnd, "LoadPalette", "�G���[", MB_OK | MB_APPLMODAL);
		return FALSE;
	}

	//�������A�f�o�C�X���𒲂ׂ�
	/*
	DDCAPS	hwCaps,swCaps;					//�n�[�h�E�F�A�A�\�t�g�E�F�A

	memset(&hwCaps,0,sizeof(DDCAPS));
	hwCaps.dwSize=sizeof(DDCAPS);
	memset(&swCaps,0,sizeof(DDCAPS));
	swCaps.dwSize=sizeof(DDCAPS);

	lpDD->GetCaps(&hwCaps,&swCaps);

	TotalMem=hwCaps.dwVidMemTotal;			//VRAM�̋󂫗e�ʂ𒲂ׂ�
	FreeMem=hwCaps.dwVidMemFree;
	*/
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

//---------------------------------------------------------
//�r�b�g�}�b�v����p���b�g��ǂݍ��ފ֐�
//---------------------------------------------------------
BOOL LoadPalette(char *FileName)
{
	//���[�h�Ɏg�p����ϐ�
	HDC hdc, memDC;
	HBITMAP hBit;
	BITMAP bm;

	//�V�X�e���p���b�g�̎擾
	hdc = GetDC(0);
	GetSystemPaletteEntries(hdc, 0, 256, ape);
	ReleaseDC(0, hdc);

	//�J���[�p���b�g�̓ǂݍ���
	lpDDSPrimary->GetDC(&hdc);
	memDC = CreateCompatibleDC(hdc);

	//�r�b�g�}�b�v���[�h
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
		MessageBox(hWnd, "File Open Error(Not256ColorBitMap)", FileName, MB_OK | MB_APPLMODAL);
		return FALSE;
	}

	SelectObject(memDC, hBit);
	//�摜����ǂݍ���0�Ԃ���236�Ԃ܂ł̃p���b�g���A
	//���ɃV�X�e���p���b�g��ǂݍ���ape�\���̕ϐ���10�Ԃ���㏑��
	GetDIBColorTable(memDC, 0, 236, (RGBQUAD *)&ape[10]);

	DeleteDC(memDC);
	lpDDSPrimary->ReleaseDC(hdc);

	//�p���b�g�f�[�^�̓���ւ��A�V�X�e���p���b�g�͕ύX���Ȃ�
	for (int i = 10; i < 246; i++)
	{
		BYTE temp;
		//�ƐԂ����ւ�
		temp = ape[i].peRed;
		ape[i].peRed = ape[i].peBlue;
		ape[i].peBlue = temp;
		//�t���O��ݒ�
		ape[i].peFlags = PC_RESERVED;
	}

	//�p���b�g�I�u�W�F�N�g�̍쐬
	lpDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, ape, &lpDDpal, 0);
	lpDDSPrimary->SetPalette(lpDDpal);

	return TRUE;
}

//---------------------------------------------------------
//�w�肳�ꂽ��ʃ��[�h���g�p�\�Ȏ��ɌĂяo�����񋓊֐�
//---------------------------------------------------------
HRESULT WINAPI EnumModesCallback2(LPDDSURFACEDESC2 LPddsd, LPVOID lpContext)
{
	if (LPddsd->dwWidth == (UINT)WinXMax &&
		LPddsd->dwHeight == (UINT)WinYMax &&
		LPddsd->ddpfPixelFormat.dwRGBBitCount == (UINT)BPP)
	{
		*(DWORD *)lpContext = TRUE;
		return DDENUMRET_CANCEL;
	}
	*(DWORD *)lpContext = FALSE;
	return DDENUMRET_OK;
}

//---------------------------------------------------------
// DirectDraw�I�u�W�F�N�g���(�쐬�������̋t������{�ɉ��)
//---------------------------------------------------------
void ReleaseDD(void)
{
	RELEASE(lpDDpal);
	RELEASE(lpDDClip)
	RELEASE(lpDDSBack);
	RELEASE(lpDDSPrimary);
	RELEASE(lpDD);
}
