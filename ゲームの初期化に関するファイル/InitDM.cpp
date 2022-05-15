/*====================================================================
	DirectMusic�������Ɋւ���\�[�X�t�@�C��
=====================================================================*/

#include <windows.h>
#include <direct.h>
#include <dsound.h>
#include <dmusicc.h>
#include <dmusici.h>

#include "Macros.h"

extern HWND hWnd; //���C���E�B���h�E�n���h��
extern LPDIRECTSOUND lpDS;

extern LPDIRECTMUSIC lpDMusic;
extern IDirectMusicPerformance *lpDMPerf;
extern IDirectMusicLoader *lpDMLoader;
extern IDirectMusicComposer *lpDMComposer;

// DirectMusic�̃Z�O�����g
extern IDirectMusicSegment *lpDMSegment[5];		  //�Z�O�����g
extern IDirectMusicSegmentState *lpDMSegState[5]; //�Z�O�����g�̏��
extern MUSIC_TIME SegLength[5];					  //�Z�O�����g�̒���
extern MUSIC_TIME SegStartTime[5];				  //�Z�O�����g�̊J�n����
extern MUSIC_TIME SegEndTime[5];				  //�Z�O�����g�̏I������

#define MULTI_TO_WIDE(x, y) MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, y, -1, x, _MAX_PATH);

//--------------------------------------------------------------
//	DirectMusic�̏�����
//--------------------------------------------------------------
BOOL InitDM()
{
	HRESULT ret = S_OK;

	// COM�̏�����
	ret = CoInitialize(NULL);
	if (ret != S_OK)
		return FALSE;

	// IDirectMusicPerformance�I�u�W�F�N�g�̍쐬
	ret = CoCreateInstance(CLSID_DirectMusicPerformance,
						   NULL,
						   CLSCTX_INPROC,
						   IID_IDirectMusicPerformance2,
						   (void **)&lpDMPerf);
	if (ret != S_OK)
		return FALSE;

	// IDirectMusicComposer�I�u�W�F�N�g�̍쐬
	ret = CoCreateInstance(CLSID_DirectMusicComposer,
						   NULL,
						   CLSCTX_INPROC,
						   IID_IDirectMusicComposer,
						   (void **)&lpDMComposer);
	if (ret != S_OK)
		return FALSE;

	// IDirectMusic�I�u�W�F�N�g�̏�����(�Q�[������DirectSound�g�p)
	ret = lpDMPerf->Init(&lpDMusic, lpDS, NULL);
	////	ret=lpDMPerf->Init(&lpDMusic,NULL,hWnd);
	if (ret != S_OK)
		return FALSE;

	ret = lpDMPerf->AddPort(NULL);
	if (ret != S_OK)
		return FALSE;

	// DirectMusicLoader�I�u�W�F�N�g�̍쐬�Ə�����
	ret = CoCreateInstance(CLSID_DirectMusicLoader,
						   NULL,
						   CLSCTX_INPROC,
						   IID_IDirectMusicLoader,
						   (void **)&lpDMLoader);
	if (ret != S_OK)
		return FALSE;

	//------�t�@�C���̌����p�X���J�����g�f�B���N�g���ɐݒ肷�鏈��----//
	char szDir[_MAX_PATH];
	WCHAR wszDir[_MAX_PATH];

	//�J�����g�f�B���N�g���̎擾
	if (_getcwd(szDir, _MAX_PATH) == NULL)
		return FALSE;
	MULTI_TO_WIDE(wszDir, szDir);

	//�ݒ�
	lpDMLoader->SetSearchDirectory(GUID_DirectMusicAllTypes, wszDir, FALSE);

	//------�œK�ȏo�̓|�[�g��I�����鏈��----------------------------//
	GUID guidSynthGUID;

	//�f�t�H���g�|�[�g�̎擾
	ret = lpDMusic->GetDefaultPort(&guidSynthGUID);
	if (ret != S_OK)
		return FALSE;

	//�f�t�H���g�|�[�g�̍쐬
	LPDIRECTMUSICPORT lpDMPort = NULL;

	DMUS_PORTPARAMS dmos;

	ZeroMemory(&dmos, sizeof(dmos));
	dmos.dwSize = sizeof(DMUS_PORTPARAMS);
	dmos.dwChannelGroups = 1;
	dmos.dwValidParams = DMUS_PORTPARAMS_CHANNELGROUPS;

	ret = lpDMusic->CreatePort(guidSynthGUID,
							   &dmos,
							   &lpDMPort,
							   NULL);
	if (ret != S_OK)
	{
		RELEASE(lpDMPort);
		return FALSE;
	}

	//�|�[�g�̔\�͂��擾
	DMUS_PORTCAPS dmpc;

	ZeroMemory(&dmpc, sizeof(dmpc));
	dmpc.dwSize = sizeof(DMUS_PORTCAPS);

	ret = lpDMPort->GetCaps(&dmpc);
	if (ret != S_OK)
	{
		RELEASE(lpDMPort);
		return FALSE;
	}

	//�擾�����|�[�g�̔\�͂𒲂ׂ�
	if ((dmpc.dwClass != DMUS_PC_OUTPUTCLASS) || //�o�̓|�[�g�ł͂Ȃ�
		!(dmpc.dwFlags & DMUS_PC_DLS))			 // DLS���x��1���T�|�[�g���Ă��Ȃ�
	{
		//�|�[�g�̔\�͂��s�����Ă���̂ō쐬�����|�[�g�����
		RELEASE(lpDMPort);

		//�����ɍ����|�[�g��񋓂���
		ret = S_OK;

		for (int i = 0; ret == S_OK; i++)
		{
			ZeroMemory(&dmpc, sizeof(dmpc));
			dmpc.dwSize = sizeof(DMUS_PORTCAPS);

			// i�Ԗڂ̃|�[�g�̏����擾
			ret = lpDMusic->EnumPort(i, &dmpc);
			//���̎擾�ɐ����������ǂ����`�F�b�N
			if (SUCCEEDED(ret))
			{
				//���������ꍇ�A�����ɍ������ǂ����`�F�b�N
				if ((dmpc.dwClass != DMUS_PC_OUTPUTCLASS) || //�o�̓|�[�g�ł͂Ȃ�
					!(dmpc.dwFlags & DMUS_PC_DLS))			 // DLS���x��1���T�|�[�g���Ă��Ȃ�
				{
					//�����ɍ����ꍇ�|�[�g���쐬����
					CopyMemory(&guidSynthGUID, &dmpc.guidPort, sizeof(GUID));

					ZeroMemory(&dmos, sizeof(dmos));
					dmos.dwSize = sizeof(DMUS_PORTPARAMS);
					dmos.dwChannelGroups = 1;
					dmos.dwValidParams = DMUS_PORTPARAMS_CHANNELGROUPS;

					ret = lpDMusic->CreatePort(guidSynthGUID, &dmos, &lpDMPort, NULL);
					if (ret != S_OK)
					{
						RELEASE(lpDMPort);
						return FALSE;
					}
					break;
				}
			}
		}
		//�����ɍ����|�[�g�����������Ȃ��ꍇ
		if (FAILED(ret))
		{
			RELEASE(lpDMPort);
			return FALSE;
		}
	}

	ret = lpDMPort->Activate(TRUE);
	if (ret != S_OK)
		return FALSE;
	ret = lpDMPerf->AddPort(lpDMPort); //�|�[�g�̑I��
	if (ret != S_OK)
		return FALSE;
	ret = lpDMPerf->AssignPChannelBlock(0, lpDMPort, 1);
	if (ret != S_OK)
		return FALSE;
	RELEASE(lpDMPort); //�|�[�g���

	//------�Z�O�����g�̍쐬����-----------------------------------//
	//�Đ�����W��MIDI�t�@�C����
	WCHAR *FileName[] = {
		L".\\Midi\\Title.mid",
		L".\\Midi\\Stage1.mid",
		L".\\Midi\\Continue.mid",
		L".\\Midi\\Boss.mid",
		L".\\Midi\\Ending.mid"};

	//�Z�O�����g�̃��[�h
	// DMUS_OBJECTDESC�\���̂ɁA���[�h����I�u�W�F�N�g�ɂ��ċL�q����
	DMUS_OBJECTDESC ObjDesc;
	ObjDesc.dwSize = sizeof(DMUS_OBJECTDESC);				  //�\���̂̃T�C�Y��ݒ�
	ObjDesc.guidClass = CLSID_DirectMusicSegment;			  // DirectMusicSegment���w��
	ObjDesc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_FILENAME; //�\���̗̂L���ȏ��������t���O��ݒ�
	for (short i = 0; i < 5; i++)
	{
		wcscpy(ObjDesc.wszFileName, FileName[i]); //�t�@�C�������w��
		ret = lpDMLoader->GetObject(&ObjDesc, IID_IDirectMusicSegment2, (void **)&lpDMSegment[i]);
		if (ret != S_OK)
			return FALSE;
		//�p�����[�^�[�̐ݒ�
		lpDMSegment[i]->SetParam(GUID_StandardMIDIFile, 0xFFFFFFFF, 0, 0, NULL);
		lpDMSegment[i]->SetParam(GUID_Download, 0xFFFFFFFF, 0, 0, (void *)lpDMPerf); // DLS�R���N�V�����̃_�E�����[�h
		//�Z�O�����g�̒����̎擾
		lpDMSegment[i]->GetLength(&SegLength[i]);
		SegEndTime[i] = SegLength[i];
		//���[�v�ݒ�
		lpDMSegment[i]->SetLoopPoints(SegStartTime[i], SegEndTime[i]);
		lpDMSegment[i]->SetRepeats(10);
	}

	return TRUE;
}

//------------------------------------------------------------------
//	DirectMusic�̏I������
//------------------------------------------------------------------
void ReleaseDM()
{
	//�Z�O�����g���t��~
	lpDMPerf->Stop(NULL, NULL, 0, 0);
	for (short i = 0; i < 5; i++)
	{
		// DLS�̃A�����[�h
		lpDMSegment[i]->SetParam(GUID_Unload, 0xFFFFFFFF, 0, 0, (void **)&lpDMPerf);
		//�Z�O�����g���
		RELEASE(lpDMSegState[i]);
		RELEASE(lpDMSegment[i]);
	}
	//�p�t�H�[�}���X�̉��
	lpDMPerf->CloseDown();
	RELEASE(lpDMPerf);
	//���[�_�[���
	RELEASE(lpDMLoader);
	//�R���|�T�[���
	RELEASE(lpDMComposer);
	// DirectMusic���
	RELEASE(lpDMusic);
	// COM���
	CoUninitialize();
}