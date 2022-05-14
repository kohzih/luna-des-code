/*=============================================================================
	�t�F�[�h�C���A�E�g�Ȃǂ̃p���b�g�Ǘ��N���X�̒�`
==============================================================================*/

//---------------------------------------------------------
// �t�F�[�h�C��,�t�F�[�h�A�E�g�Ɏg�p����\����
//---------------------------------------------------------
typedef	struct	tag_PALETTECONTROL{
		int	r,g,b;
}PALETTECONTROL;

class FaInOut  
{
	BOOL	WFadeF;			//�z���C�g�A�E�g�A�C���̊Ǘ��t���O
	BOOL	BFadeF;			//�u���b�N�A�E�g�A�C���̊Ǘ��t���O
	BOOL	LBFadeF;		//LittleBlack�Ǘ��p�̃t���O

	short	StartPal;		//�ω����J�n������擪�p���b�g�ԍ�	
	short	PalEntry;		//�ω�������p���b�g�̌�
	int		PalCnt;			//���݂̐i�s��(PalCnt>>16��0�`255)
	
	PALETTEENTRY	apeH1[256],		//�ڕW�̐F
					apeH2[256];		//��Ɨp
	PALETTECONTROL	apeH3[256];		//��Ɨp�����_�Ή��p���b�g

public:	
		
	BOOL	SetPaletteEntry();		//���݂̃p���b�g���擾
	void	SetFade(DWORD);			//�e�t�F�[�h�C���A�E�g�𑦎��s�ł���悤�ɐݒ肷��
	BOOL	WhiteOut(int);			//�z���C�g�A�E�g
	BOOL	WhiteIn(int);			//�z���C�g�C��
	BOOL	BlackOut(int);			//�u���b�N�A�E�g
	BOOL	BlackIn(int);			//�u���b�N�C��
	BOOL	LittleBlack(int,bool);	//��ʂ������Ŏw�肳�ꂽ�l���A�Â�����

	FaInOut(short,short);
	~FaInOut();
};
