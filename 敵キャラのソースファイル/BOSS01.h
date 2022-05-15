/*====================================================================
	�{�X�L����01�N���X�̒�`
=====================================================================*/

//�{�X�L�����A�j���[�V�������Ǘ�����\����
typedef struct tag_BossAnime
{
	POINT Local;  //�{�X�L������`�悷��̂Ɋ�ƂȂ���W����̋���
	short Width;  //�p�[�c�̉���
	short Height; //�p�[�c�̏c��
	RECT SrcR;	  //�]�����T�[�t�F�X�̋�`�͈�
	RECT DesR;	  //�]����T�[�t�F�X�̋�`�͈�
} BOSSANIME;

//�{�X�L�����A�j���[�V�������Ǘ�����\����2
typedef struct tag_BossAnime2
{
	POINT Local;		 //�{�X�L������`�悷��̂Ɋ�ƂȂ���W����̋���
	short Width;		 //�p�[�c�̉���
	short Height;		 //�p�[�c�̏c��
	BYTE Anime;			 //�X�v���C�g�̃A�j���[�V�����p�^�[����
	BYTE AnimePat[10];	 //�A�j���[�V�����p�^�[���z��̃A�h���X
	BYTE AnimeFrame[10]; //�A�j���[�V�����؂�ւ��^�C�~���O�z��̃A�h���X
	BYTE AnimeCnt;		 //���ݕ\������A�j���[�V�����p�^�[��
	BYTE FrameCnt;		 //���݂̃A�j���[�V�����t���[��
	RECT SrcR;			 //�]�����T�[�t�F�X�̋�`�͈�
	RECT DesR;			 //�]����T�[�t�F�X�̋�`�͈�
} BOSSANIME2;

//////////////////////////////////////////////////////////////////////
//	�N���X��`
//////////////////////////////////////////////////////////////////////

class BOSS01 : protected Draw
{
	DWORD fSPChr; //�{�X�L����01�̏�Ԃ�\���t���O

	short Width;		   //�{�X�L����01�̉���
	short Height;		   //�{�X�L����01�̏c��
	short Shld;			   //�{�X�L����01�j��܂ł̑ϋv��
	short ShldCnt;		   //�{�X�L����01�c��ϋv�͂̃J�E���^
	short ATmt[2];		   //�U���Ԑ��ɓ���܂ł̃E�F�C�g���z��(�U���p�^�[����2���)
	short ATmtCnt[2];	   //�U���Ԑ��ɓ���܂ł̃E�F�C�g���̃J�E���^�z��(�U���p�^�[����2���)
	short ATt[2];		   //�U���𑱂��鎞�Ԕz��(�U���p�^�[����2���)
	short ATtCnt[2];	   //�U���𑱂��鎞�Ԃ̃J�E���^�z��(�U���p�^�[����2���)
	DWORD AttackTama1[12]; //���ˑҋ@��Ԃɂ���e�ԍ��ۑ��p(�U���p�^�[��1)
	DWORD AttackTama2;	   //���ˑҋ@��Ԃɂ���e�ԍ��ۑ��p(�U���p�^�[��2)

	short ONTime; //��ʂɕ\������Ă���̌o�ߎ���

	short MovKakudo; //�{�X�L����01�̈ړ��p�x
	DPOINT Mov;		 //�{�X�L����01�̈ړ����x�̓������\����
	DPOINT dCdn;	 //�{�X�L����01�̍��W�v�Z�p�̍\����
	POINT Cdn;		 //�{�X�L����01�̌��݂̉�ʍ��W�̓������|�C���g�\����
	POINT MapCdn;	 //�{�X�L����01�̌��݂̐�΍��W�̓������|�C���g�\����

	TAMAS2 B_Msr;	//�{�X�L����01�e�̃X�e�[�^�X���Ǘ�����TAMAS_Kai�\����
	short T_Ichi;	// B_Block�𔭎˂���ʒu
	TAMAS3 B_Block; //�{�X�L����01�e�̃X�e�[�^�X���Ǘ�����TAMAS_Kai2�\����

	BOSSANIME Parts1;  //�{�X�L����01���\������`��p�f�[�^(�p�[�c1)
	BOSSANIME Parts2;  //�{�X�L����01���\������`��p�f�[�^(�p�[�c2)
	BOSSANIME2 Parts3; //�{�X�L����01���\������`��p�f�[�^(�p�[�c3)
	BOSSANIME2 Parts4; //�{�X�L����01���\������`��p�f�[�^(�p�[�c4)
	BOSSANIME Parts5;  //�{�X�L����01���\������`��p�f�[�^(�p�[�c5)
	BOSSANIME2 Parts6; //�{�X�L����01���\������`��p�f�[�^(�p�[�c6)

	BOSSANIME2 BossBaku1[12];  //�{�X�L����01�����A�j���[�V�������Ǘ�����\����1
	BOSSANIME2 BossBaku2[200]; //�{�X�L����01�����A�j���[�V�������Ǘ�����\����2

public:
	//--------------�ȉ��̃��\�b�h��BOSS01.cpp�ɋL�q---------------------------//
	BOOL InitBoss01(POINT);	   //�{�X�L����01�̏�����
	void MovBoss01();		   //�{�X�L����01�̈ړ�����
	void Attack1Boss01(POINT); //�{�X�L����01�̍U������1
	void Attack2Boss01();	   //�{�X�L����01�̍U������2
	void FrameBoss01(DWORD);   //�{�X�L����01�̕`�揀�����s�Ȃ�����
	void RendBoss01();		   //�{�X�L����01�̕`��

	DWORD GetBoss01fSPChr();	  //�{�X�L����01��ԃt���O��Ԃ�
	POINT GetBoss01Cdn();		  //�{�X�L����01�̉�ʍ��W��Ԃ�
	short GetBoss01Width();		  //�{�X�L����01�̉�����Ԃ�
	short GetBoss01Height();	  //�{�X�L����01�̏c����Ԃ�
	short *GetBoss01ShldCnt_P();  //�{�X�L����01�̎c��ϋv�͂̒l�ւ̃|�C���^��Ԃ�
	TAMAS2 *GetBoss01B_Msr_P();	  //�e�̏�Ԃ��Ǘ�����TAMAS2�\���̂ւ̃|�C���^��Ԃ�
	TAMAS3 *GetBoss01B_Block_P(); //�e�̏�Ԃ��Ǘ�����TAMAS3�\���̂ւ̃|�C���^��Ԃ�

	BOSS01();
	~BOSS01();
	//-------------------------------------------------------------------------//
};
