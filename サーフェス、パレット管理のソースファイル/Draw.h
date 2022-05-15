/*=============================================================================
	���@�A�G�L�����A�w�i�N���X�̊�{�N���X�̒�`
==============================================================================*/

//----------------------------------------------------------
// BG�̓]������摜�̋�`������W�ƃC�x���g���i�[����\����
//----------------------------------------------------------
typedef struct tag_BgMap
{
	WORD X;	  // lpDDBGData�̓]������摜��`�͈͂̍���X���W
	WORD Y;	  //						�@�@�@�@�@�@�@�@Y���W
	WORD Evt; //�C�x���g
} BGMAP;

class Draw
{
protected:
	static LPDIRECTDRAWSURFACE7 lpDDS_JIKI; //���@�L�����p�摜�T�[�t�F�X
	static LPDIRECTDRAWSURFACE7 lpDDS_SP;	//�G�L�����p�摜�T�[�t�F�X
	static LPDIRECTDRAWSURFACE7 lpDDS_BG;	//�w�i�摜�p�T�[�t�F�X
	static LPDIRECTDRAWSURFACE7 lpDDS_BG2;	//�w�i�摜�p�T�[�t�F�X2
	static DWORD fSTAGE;					//�e�X�e�[�W�̊J�n�ƏI���������t���O
	static int StageNum;					//���݂̃X�e�[�W�ԍ�
	static POINT MainCdn;					//�Q�[�����̐�΍��W
	static DWORD MapSizeX;					//�w�i�}�b�v�f�[�^�̉��T�C�Y�i���ۂ̒l��MapSizeX*ChipSizeX�h�b�g�j
	static DWORD MapSizeY;					//�w�i�}�b�v�f�[�^�̏c�T�C�Y�i���ۂ̒l��MapSizeY*ChipSizeY�h�b�g�j
	static DWORD TipSizeX;					//�w�i�`�b�v��̉��̃T�C�Y
	static DWORD TipSizeY;					//�w�i�`�b�v��̏c�̃T�C�Y
	static BGMAP *Parts;					//�w�i�}�b�v�\���̔z��̐擪�A�h���X
	static double *Sin;						// 0-360�x���̃T�C���l�z��̐擪�A�h���X���i�[����|�C���^
	static double *Cos;						// 0-360�x���̃R�T�C���l�z��̐擪�A�h���X���i�[����|�C���^
	static double *Tan;						// 0-89�x���̃^���W�F���g�l�z��̐擪�A�h���X���i�[����|�C���^

private:
	char *JIKIFileName;		 //���@�̕`��p�r�b�g�}�b�v�̃t�@�C����
	char *PrologFileName;	 //�v�����[�O��ʕ`��p�̃t�@�C����
	char *TitleFileName[2];	 //�^�C�g����ʕ`��p�̃t�@�C����(�^�C�g�������A�^�C�g�����)
	char *St1FileName[4];	 //�X�[�e�W1�`��p�̃t�@�C����(�G�L�����A�w�i�A�}�b�v�f�[�^)
	char *GameOverFileName;	 //�Q�[���I�[�o�[��ʕ`��p�̃r�b�g�}�b�v�t�@�C����
	char *GameClearFileName; //�Q�[���N���A��ʕ`��p�̃r�b�g�}�b�v�t�@�C����

	//�r�b�g�}�b�v��ǂݍ���ŃI�t�X�N���[���T�[�t�F�X���쐬
	BOOL LoadBitMap(char *, LPDIRECTDRAWSURFACE7 &lpDDsuf);
	//�}�b�v�f�[�^��ǂݍ���
	BOOL LoadMapData(char *);

public:
	static DWORD EventCnt; // Parts�\���̔z��Ɋ܂܂��C�x���g(DrawSP�I�u�W�F�N�g)�̐�

	BOOL Init();													 //���W�A�e�J�E���^�A�T�[�t�F�X�̏�����
	BOOL ReleaseALL();												 // Draw�I�u�W�F�N�g�̑S�����o�����
	BOOL RestoreJIKI();												 //���@�p�T�[�t�F�X�����X�g�A����֐�
	BOOL RestoreSP();												 // SP�p�T�[�t�F�X�����X�g�A����֐�
	BOOL RestoreBG();												 // BG�p�T�[�t�F�X�����X�g�A����֐�
	BOOL GetMapSizeData(DWORD &mx, DWORD &my, DWORD &cx, DWORD &cy); //�}�b�v�̍\���f�[�^�����߂�
	BOOL GetEventData(int, WORD &event);							 //�����Ŏw�肳�ꂽ�ꏊ��BGMAP�\���̂̃C�x���g�f�[�^�����߂�
	DWORD GetfSTAGE();												 //���݂̃X�e�[�W�t���O��Ԃ�
	void SetfSTAGE(DWORD, int);										 //�����œn���ꂽ�t���O���Z�b�g
	int GetStageNum();												 //���݂̃X�e�[�W�ԍ���Ԃ�
	void SetStageNum(int);											 //�����œn���ꂽ�X�e�[�W�ԍ��Z�b�g
	short TanKaku(double);											 //�^���W�F���g�̒l���p�x�ɒ���

	//�E�B���h�E���[�h�ƃt���X�N���[���؂�ւ��p�����o�֐�
	BOOL ReleaseSuf(); //�I�t�X�N���[���T�[�t�F�X�����
	BOOL ReloadSuf();  //�I�t�X�N���[���T�[�t�F�X���č\�z

	Draw();								 //�h���N���X�p�R���X�g���N�^
	Draw(char **FileName, HRESULT &ret); //��{�N���X�p�R���X�g���N�^
	virtual ~Draw();
};
