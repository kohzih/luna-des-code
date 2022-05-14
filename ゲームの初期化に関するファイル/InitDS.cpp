/*====================================================================
	DirectSound�������Ɋւ���\�[�X�t�@�C��
=====================================================================*/

#include	<windows.h>
#include	<dsound.h>

#include	"Macros.h"
#include	"wavread.h"

extern	HWND		hWnd;		//���C���E�B���h�E�n���h��

extern	LPDIRECTSOUND			lpDS;
extern	LPDIRECTSOUNDBUFFER		lpDSBPrimary;
extern	LPDIRECTSOUNDBUFFER		lpDSBuf[6];

WAVEFORMATEX  *pwfx;          // �E�F�[�u �t�H�[�}�b�g���B
HMMIO         hmmio;          // �t�@�C�� �n���h���B
MMCKINFO      mmckinfoData;   // �`�����N���B
MMCKINFO      mmckinfoParent; // �e�`�����N�̏��B

HRESULT WaveOpenFile( CHAR*, HMMIO*, WAVEFORMATEX**, MMCKINFO*);
HRESULT WaveStartDataRead( HMMIO*, MMCKINFO*, MMCKINFO* );
HRESULT WaveReadFile( HMMIO, UINT, BYTE*, MMCKINFO*, UINT* );

BOOL	CreateBufferFromWaveFile(char*, DWORD);

//---------------------------------------------------------
//DirectSound������
//---------------------------------------------------------
BOOL	InitDS()
{
	HRESULT			ret;
	
	char			*SoundFileName[]={	".\\Sound\\Jikibaku.wav",//���@����
										".\\Sound\\Tekibaku.wav",//�G����
										".\\Sound\\FuelArm.wav",//�A�[������
										".\\Sound\\Fire.wav",	//��C����
										".\\Sound\\Tamakyu.wav",//�e�z����
										".\\Sound\\Bossbaku.wav"};//�{�X�L��������

	//DirectSound�I�u�W�F�N�g�쐬
	ret=DirectSoundCreate(NULL,&lpDS,NULL);
	if(ret != DS_OK)		return	FALSE;
	
	//�������x���ݒ�
	ret=lpDS->SetCooperativeLevel(hWnd,DSSCL_PRIORITY);
	if(ret != DS_OK)		return	FALSE;
	
	//�v���C�}���o�b�t�@�̍쐬
	DSBUFFERDESC                dsbdesc;
	
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
    dsbdesc.dwSize = sizeof(DSBUFFERDESC);
    dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
    ret=lpDS->CreateSoundBuffer(&dsbdesc, &lpDSBPrimary, NULL);
    if(ret != DS_OK)	   return FALSE;	
	
	// �v���C�}�� �o�b�t�@�̃t�H�[�}�b�g��ݒ肷��B
	WAVEFORMATEX wfx;
    memset(&wfx, 0, sizeof(WAVEFORMATEX)); 
    wfx.wFormatTag = WAVE_FORMAT_PCM; 
    wfx.nChannels = 2; 
    wfx.nSamplesPerSec = 44100; 
    wfx.wBitsPerSample = 8; 
    wfx.nBlockAlign = wfx.wBitsPerSample / 8 * wfx.nChannels;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	ret = lpDSBPrimary->SetFormat(&wfx); 
	if(ret != DS_OK	)				return FALSE;
	
	//�E�F�[�u�t�@�C����ǂݍ���Ńo�b�N�o�b�t�@���쐬
	for(int i=0;i<6;i++)
	{
		ret=CreateBufferFromWaveFile(SoundFileName[i],i);
		if(ret != TRUE)		return	FALSE;
	}
	
	return TRUE;
} 

//---------------------------------------------------------
//�o�b�N�o�b�t�@�쐬
//---------------------------------------------------------
BOOL	CreateBufferFromWaveFile(char	*FileName, DWORD dwBuf)
{

	//���̊֐��̓t�@�C���ւ̃n���h�����擾���A
	//���̃t�@�C���� RIFF �t�H�[�}�b�g�ł��邱�Ƃ��m�F���A
	//�E�F�[�u �t�H�[�}�b�g�Ɋւ�������擾����B
	if(WaveOpenFile(FileName, &hmmio, &pwfx, &mmckinfoParent) != 0)
	{	
		mmioClose( hmmio, 0 );
		pwfx   = NULL;
		return FALSE; 
	}
	
	//���̊֐��� mmckinfoData ���̃f�[�^ �`�����N�Ɋւ������Ԃ��B
	//�t�@�C�� �|�C���^���g���ăf�[�^ �`�����N�ɓ��B����B
	//���̊֐��̓f�[�^ �`�����N�p�� MMCKINFO �\���̂ւ̏������݂��s���̂ŁA
	//���p�\�ȃf�[�^�̗ʂ�c���ł���B
	if (WaveStartDataRead(&hmmio, &mmckinfoData, &mmckinfoParent) != 0)
	{	
		mmioClose( hmmio, 0 );
		pwfx   = NULL;
		return FALSE; 
	}

	//�ǂݍ��񂾃E�F�[�u�t�@�C���Ɠ����t�H�[�}�b�g�ŃZ�J���_���T�E���h�o�b�t�@���쐬����B
	DSBUFFERDESC         dsbdesc;
 
	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); 
	dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
	dsbdesc.dwFlags = DSBCAPS_STATIC; 
	dsbdesc.dwBufferBytes = mmckinfoData.cksize;  
	dsbdesc.lpwfxFormat = pwfx; 
 
	if FAILED(lpDS->CreateSoundBuffer(&dsbdesc, &lpDSBuf[dwBuf], NULL))
	{
		mmioClose( hmmio, 0 );
		pwfx   = NULL;
		return FALSE; 
	}

	//���̏ꍇ�́A�A�v���P�[�V�����̓f�[�^�̃X�g���[�~���O���s�킸�A
	//�P���ɃX�^�e�B�b�N �o�b�t�@�ւ̏������݂��s���̂ŁA
	//�o�b�t�@�S�̂�擪���烍�b�N����B
	//���b�v �A���E���h (�擪�֖߂�) ���������Ȃ��̂ŁA
	//�K�v�Ȃ̂� 1 �̃|�C���^�ƃo�C�g �J�E���g�ł���B

	LPVOID lpvAudio1;
	DWORD  dwBytes1;
 
	if FAILED(lpDSBuf[dwBuf]->Lock(
		    0,              // ���b�N�J�n�̃I�t�Z�b�g�B
			0,              // ���b�N�̃T�C�Y�B���̏ꍇ�͖��������B
			&lpvAudio1,     // ���b�N�J�n�̃A�h���X�B
			&dwBytes1,      // ���b�N���ꂽ�o�C�g���B
			NULL,           // ���b�v �A���E���h�̊J�n�B�s�g�p�B
			NULL,           // ���b�v �A���E���h�̃T�C�Y�B�s�g�p�B
			DSBLOCK_ENTIREBUFFER))  // �t���O�B
	{
		// �G���[�����B
		mmioClose( hmmio, 0 );
		pwfx   = NULL;
		return FALSE; 
	}


	//���̊֐��́A�t�@�C������o�b�t�@�̃|�C���^�փf�[�^���R�s�[���A���������ꍇ�� 0 ��Ԃ��B 

	UINT cbBytesRead;
 
	if (WaveReadFile(
			hmmio,              // �t�@�C�� �n���h���B
			dwBytes1,           // �ǂݍ��ނׂ��o�C�g���B
			(BYTE *) lpvAudio1, // �R�s�[��B
		    &mmckinfoData,      // �t�@�C�� �`�����N���B
			&cbBytesRead))      // ���ۂɓǂݍ��܂ꂽ�o�C�g���B
	{
		// 0 �ȊO�̒l���Ԃ��ꂽ�ꍇ�̎��s����������B
		mmioClose( hmmio, 0 );
		pwfx   = NULL;
		return FALSE; 
	}

	//�Ō�ɃA�v���P�[�V�����̓o�b�t�@���A�����b�N���A�E�F�[�u �t�@�C�������B

	lpDSBuf[dwBuf]->Unlock(lpvAudio1, dwBytes1, NULL, 0);
	mmioClose( hmmio, 0 );
	pwfx   = NULL;
    
	return	TRUE;
}

//---------------------------------------------------------
//DirectSound�I�u�W�F�N�g���
//---------------------------------------------------------
void	ReleaseDS(void)
{	
		for(int i=0;i<6;i++)
			RELEASE(lpDSBuf[i]);
		RELEASE(lpDS);
}
 



							
	