/*====================================================================
	�O���[�o���ϐ�1
=====================================================================*/

//---------------------------------------------------------
// Windows�֘A�@�O���[�o��
//---------------------------------------------------------
HINSTANCE hInst; //�C���X�^���X�n���h��
HWND hWnd;		 //���C���E�B���h�E�n���h��

BOOL fActive = 0;	  //�A�N�e�B�u�t���O(�E�B���h�E�\���p)
BOOL fFullscreen = 1; //�t���X�N���[��(1)or�E�B���h�E�t���O(0)
BOOL fInputDevice;	  //���̓f�o�C�X�`�F�b�N�t���O

DWORD DisplayColor;		 //�f�B�X�v���C�̃J���[�p���b�g
DWORD TotalMem, FreeMem; // VRAM�̋󂫗e�ʂ̒l���i�[

//---------------------------------------------------------
// DirectDraw�֘A�@�O���[�o��
//---------------------------------------------------------
LPDIRECTDRAW7 lpDD = NULL; // DirectDraw�I�u�W�F�N�g

LPDIRECTDRAWSURFACE7 lpDDSPrimary = NULL, //�v���C�}���T�[�t�F�[�X
	lpDDSBack = NULL;					  //�o�b�N�T�[�t�F�[�X

LPDIRECTDRAWPALETTE lpDDpal = NULL; // DirectDraw�p���b�g�I�u�W�F�N�g

LPDIRECTDRAWCLIPPER lpDDClip = NULL; //�N���b�p�[(�E�B���h�E�\���p)

//---------------------------------------------------------
// DirectSound�֘A
//---------------------------------------------------------
LPDIRECTSOUND lpDS = NULL;
LPDIRECTSOUNDBUFFER lpDSBPrimary = NULL;
LPDIRECTSOUNDBUFFER lpDSBuf[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

//---------------------------------------------------------
// DirectMusic�֘A
//---------------------------------------------------------
LPDIRECTMUSIC lpDMusic = NULL;
IDirectMusicPerformance *lpDMPerf = NULL;
IDirectMusicLoader *lpDMLoader = NULL;
IDirectMusicComposer *lpDMComposer = NULL;

IDirectMusicSegment *lpDMSegment[5] = {NULL, NULL, NULL, NULL, NULL};		//�Z�O�����g
IDirectMusicSegmentState *lpDMSegState[5] = {NULL, NULL, NULL, NULL, NULL}; //�Z�O�����g�̏��
MUSIC_TIME SegLength[5] = {0, 0, 0, 0, 0};									//�Z�O�����g�̒���
MUSIC_TIME SegStartTime[5] = {0, 0, 0, 0, 0};								//�Z�O�����g�̊J�n����
MUSIC_TIME SegEndTime[5] = {0, 0, 0, 0, 0};									//�Z�O�����g�̏I������

//---------------------------------------------------------
// DirectInput�֘A
//---------------------------------------------------------
LPDIRECTINPUT7 lpDI = NULL;
LPDIRECTINPUTDEVICE7 lpDIJoystick = NULL;
LPDIRECTINPUTDEVICE7 lpDIKeyboard = NULL;