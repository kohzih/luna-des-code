/*====================================================================
	�G�L����02�N���X�̒�`
=====================================================================*/

class TEKI02 : protected Draw   
{
	DWORD	fSPChr;			//�G�L����02�̏�Ԃ�\���t���O
	
	short	Width;			//�G�L����02�̉���
	short	Height;			//�G�L����02�̏c��
	short	Shld;			//�G�L����02�j��܂ł̑ϋv��
	short	ShldCnt;		//�G�L����02�c��ϋv�͂̃J�E���^
	short	ATmt;			//�U���Ԑ��ɓ���܂ł̃E�F�C�g��
	short	ATmtCnt;		//�U���Ԑ��ɓ���܂ł̃E�F�C�g���̃J�E���^
	short	ATt;			//�U���𑱂��鎞��
	short	ATtCnt;			//�U���𑱂��鎞�Ԃ̃J�E���^
	DWORD	TTamaNum;		//�����ɔ��˂���e�ԍ��ۑ��p
	
	short	ONTime;			//��ʂɕ\������Ă���̌o�ߎ���
	short	TamaKaku;		//�e�𔭎˂���p�x
	
	DPOINT	Mov;			//�G�L����02�̈ړ����x�̓������\����
	DPOINT	dCdn;			//�G�L����02�̍��W�v�Z�p�̍\����
	POINT	Cdn;			//�G�L����02�̌��݂̉�ʍ��W�̓������|�C���g�\����
	POINT	MapCdn;			//�G�L����02�̌��݂̐�΍��W�̓������|�C���g�\����	
	
	TAMAS	TMsr;			//�G�L����02�e�̃X�e�[�^�X���Ǘ�����TAMAS�\����
	SPANIME	TChr;			//�G�L����02�A�j���[�V�������Ǘ�����\����
	SPANIME	TBaku;			//�G�L����02�����A�j���[�V�������Ǘ�����\����
	
public:
	//--------------�ȉ��̃��\�b�h��TEKI02.cpp�ɋL�q---------------------------//
	BOOL	InitTeki02(POINT);	//�G�L����02�̏�����
	void	MovTeki02();		//�G�L����02�̈ړ�����
	void	AttackTeki02();		//�G�L����02�̍U������
	void	FrameTeki02(DWORD);	//�G�L����02�̕`�揀�����s�Ȃ�����
	void	RendTeki02();		//�G�L����02�̕`��
	
	DWORD	GetTeki02fSPChr();		//�G�L����02��ԃt���O��Ԃ�
	POINT	GetTeki02Cdn();			//�G�L����02�̉�ʍ��W��Ԃ�
	short	GetTeki02Width();		//�G�L����02�̉�����Ԃ�
	short	GetTeki02Height();		//�G�L����02�̏c����Ԃ�
	short*	GetTeki02ShldCnt_P();	//�G�L����02�̎c��ϋv�͂̒l�ւ̃|�C���^��Ԃ�
	TAMAS*	GetTeki02TMsr_P();		//�e�̏�Ԃ��Ǘ�����TAMAS�\���̂ւ̃|�C���^��Ԃ�
	
	TEKI02();
	~TEKI02();
	//-------------------------------------------------------------------------//
};

