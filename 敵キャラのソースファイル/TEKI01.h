/*====================================================================
	�G�L����01�N���X�̒�`
=====================================================================*/

class TEKI01 : protected Draw   
{
	DWORD	fSPChr;			//�G�L����01�̏�Ԃ�\���t���O

	short	Width;			//�G�L����01�̉���
	short	Height;			//�G�L����01�̏c��
	short	Shld;			//�G�L����01�j��܂ł̑ϋv��
	short	ShldCnt;		//�G�L����01�j��܂ł̑ϋv�͂̃J�E���^
	short	ATmt;			//�U���Ԑ��ɓ���܂ł̃E�F�C�g��
	short	ATmtCnt;		//�U���Ԑ��ɓ���܂ł̃E�F�C�g���̃J�E���^
	short	ATt;			//�U���𑱂��鎞��
	short	ATtCnt;			//�U���𑱂��鎞�Ԃ̃J�E���^
	DWORD	TTamaNum;		//�����ɔ��˂���e�ԍ��ۑ��p
	
	DPOINT	Mov;			//�G�L����01�̈ړ����x�̓������\����
	DPOINT	dCdn;			//�G�L����01�̍��W�v�Z�p�̍\����
	POINT	Cdn;			//�G�L����01�̌��݂̉�ʍ��W�̓������|�C���g�\����
	POINT	MapCdn;			//�G�L����01�̌��݂̐�΍��W�̓������|�C���g�\����	
	
	TAMAS	TMsr;			//�G�L����01�e�̃X�e�[�^�X���Ǘ�����TAMAS�\����		
	SPANIME	TChr;			//�G�L����01�A�j���[�V�������Ǘ�����\����
	SPANIME	TBaku;			//�G�L����01�����A�j���[�V�������Ǘ�����\����

public:
	//-----------�ȉ��̃��\�b�h��TEKI01.cpp�ɋL�q------------------------------//
	BOOL	InitTeki01(POINT);	//�G�L����01�̏�����
	void	MovTeki01();		//�G�L����01�̈ړ�����
	void	AttackTeki01();		//�G�L����01�̍U������
	void	FrameTeki01(DWORD);	//�G�L����01�̕`�揀�����s�Ȃ�����
	void	RendTeki01();		//�G�L����01�̕`��
	
	DWORD	GetTeki01fSPChr();		//�G�L����01��ԃt���O��Ԃ�
	POINT	GetTeki01Cdn();			//�G�L����01�̉�ʍ��W��Ԃ�
	short	GetTeki01Width();		//�G�L����01�̉�����Ԃ�
	short	GetTeki01Height();		//�G�L����01�̏c����Ԃ�
	short*	GetTeki01ShldCnt_P();	//�G�L����01�̎c��ϋv�͂̒l�ւ̃|�C���^��Ԃ�
	TAMAS*	GetTeki01TMsr_P();		//�e�̏�Ԃ��Ǘ�����TAMAS�\���̂ւ̃|�C���^��Ԃ�

	TEKI01();
	~TEKI01();
	//-------------------------------------------------------------------------//
};