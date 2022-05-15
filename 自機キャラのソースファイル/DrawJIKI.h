/*====================================================================
	���@�N���X�̒�`
=====================================================================*/
#include "DrawJIKIStruct.h"

class DrawJIKI : protected Draw
{
	DWORD fJIKI;  //���@�̏�Ԃ�\�킷�t���O
	DWORD Target; //�j�󂷂�ƃX�e�[�W�N���A�ɂȂ�G�L�����̔ԍ�
	DWORD SCORE;  //���݂̃X�R�A

	int InvincCnt;	 //���G���Ԃ̃J�E���^
	int ContinueCnt; // RendCONTINUE()���Ŏg�p����J�E���^

	POINT MapCdn; //���@�̌��݂̐�΍��W�̓������|�C���g�\����
	POINT Cdn;	  //���݂̉�ʍ��W�̓������|�C���g�\����
	POINT Mov;	  //�ړ����x�̓������|�C���g�\����
	POINT CurCdn; //�p���[�A�b�v��ʂ̃J�[�\���̕\�����W

	BYTE EXP;	 // EXP(Eclipse Xeres Pleuvoir)�̗�
	LONG EXP_R;	 // EXP�̗�(16bit�E�ɃV�t�g����EXP�ɑ��)
	BYTE FUEL;	 //�e�z���R���c��
	LONG FUEL_R; //�e�z���R���c��(16bit�E�ɃV�t�g����FUEL�ɑ��)

	SPANIME ArmJIKI; //�A�[�����ˎ��̎��@�̃A�j���[�V����
	SPANIME Arm;	 //�A�[�����ˎ��̃A�[���A�j���[�V����
	SPANIME Baku;	 //���@�����Ă��ꂽ���̃A�j���[�V����
	SPANIME TmKySy;	 //�e�z�����̎��@�̃A�j���[�V����

	WPDATA Wpd;	   //���݂̎��@�̑������x�����Ǘ�����\����
	TAMAS LSR1;	   //���[�U�[1�̃A�j���[�V�����ƃX�e�[�^�X�Ǘ�
	TAMAS MSR1;	   //�~�T�C��1�̃A�j���[�V�����ƃX�e�[�^�X�Ǘ�
	TAMAS2_2 HNB1; //�ԉ�1�̃A�j���[�V�����ƃX�e�[�^�X�Ǘ�
	HMTAMAS HMM1;  //�z�[�~���O�~�T�C��1�̃A�j���[�V�����ƃX�e�[�^�X�Ǘ�
	HMDATA HMMDAT; //�z�[�~���O�~�T�C���̏����ɕK�v�ȃf�[�^
	OPTDATA OPT;   //�I�v�V�������Ǘ�����\����

	RECT JIKIChr;	 //�ʏ펞�̎��@�̋�`�͈�
	RECT SCORE_R[9]; //�X�R�A�̕����̋�`�͈�
	RECT GAGE_F;	 // FUEL�Q�[�W�̌��݂̋�`�͈�
	RECT GAGE_FK;	 // FUEL�Q�[�W�̋󔒕����̋�`�͈�
	RECT GAGE_FW;	 // FUEL�Q�[�W�̘g�����̋�`�͈�
	RECT GAGE_E;	 // EXP�Q�[�W�̌��݂̋�`�͈�
	RECT GAGE_EK;	 // EXP�Q�[�W�̋󔒕����̋�`�͈�
	RECT GAGE_EW;	 // EXP�Q�[�W�̘g�����̋�`�͈�
	RECT GAGE_P;	 // PWU�Q�[�W�̌��݂̋�`�͈�
	RECT GAGE_P2;	 // PWU�Q�[�W�̌��݂̋�`�͈�2
	RECT GAGE_PW;	 // PWU�Q�[�W�̘g�����̋�`�͈�
	RECT OPTWAKU;	 // OPT�p�̘g�̋�`�͈�

	//---------------�ȉ��̃��\�b�h��DrawJIKI_Jiki.cpp�ɋL�q-------------------
	BOOL InitJikiData();			//���@�̗l�X�ȃf�[�^������������
	void InitJikiTm();				//���I�Ɋm�ۂ��郁�����̈�ւ̃|�C���^��NULL�ŏ�����
	void JikiTmRelease();			//�m�ۂ����������̈�����
	void JikiTekitamaAtari();		//���@�ƓG�̒e�Ƃ̓����蔻��
	void JikiTekiAtari();			//���@�ƓG�Ƃ̓����蔻��
	BOOL Atari(RECT);				//�����œn���ꂽ���W�Ƃ̓����蔻����s��
	BOOL Atari(RECT, RECT);			//�����œn���ꂽ���W�Ƃ̓����蔻����s��
	void JikiTekiDmg(int);			//�����Ŏw�肳�ꂽ�_���[�W�l��G�L�����ɗ^����
	void JikiTamas3Dmg(int, short); //�����Ŏw�肳�ꂽ�_���[�W�l��G�e�ɗ^����
	void InitPWUP();				//���@�̃p���[�A�b�v��ʂ�����������
	void RendPWUP();				//���@�̃p���[�A�b�v��ʂ�`��
	void RendCONTINUE();			//�R���e�B�j���[��ʂ�`�悷��
	void InitCONTINUE();			//�R���e�B�j���[�ɔ������������s��
	void ScoreCalcul();				//���݂̃X�R�A�\���̂��߂̋�`�͈͂��Z�b�g
	//-------------------------------------------------------------------------

	//---------------�ȉ��̃��\�b�h��DrawJIKI_JikiFrame.cpp�ɋL�q--------------
	void JikiBakuFrame(); //���@�̔��j�V�[���̃A�j���[�V��������
	void JikiArmFrame();  //���@�̃A�[�����˃V�[���̃A�j���[�V��������
	void JikiTmkyFrame(); //���@�̒e�z���V�[���̃A�j���[�V��������
	//-------------------------------------------------------------------------

	//---------------�ȉ��̃��\�b�h��DrawJIKI_MainWepn.cpp�ɋL�q---------------
	BOOL InitMainWepn();	 //��C���甭�˂����e�I�u�W�F�N�g�̏�����
	void InitWepnTm();		 //���I�Ɋm�ۂ��郁�����̈�ւ̃|�C���^��NULL�ŏ�����
	void MainWepnRelease();	 //�m�ۂ����������̈�����
	void MainWepnCnt();		 //��C���˂܂ł̃J�E���g��i�߂�
	void MainWepnCntReset(); //��C���˂܂ł̃J�E���^�����Z�b�g������
	void MainWepnReset();	 //��C�̒e��S�Ĕ��ˑҋ@��Ԃɖ߂�
	void MainWepn();		 //��C���˂ƈړ��̏���
	void MainWepnAtari();	 //��C�̒e�ƓG�L�����Ƃ̓����蔻��
	void MainWepnRend();	 //��C���甭�˂��ꂽ�e�̕`��
	//-------------------------------------------------------------------------

	//---------------�ȉ��̃��\�b�h��DrawJIKI_Hmm.cpp�ɋL�q--------------------
	BOOL InitHmm();		   //�z�[�~���O�~�T�C���I�u�W�F�N�g�̏�����
	void InitHmmTm();	   //���I�Ɋm�ۂ��郁�����̈�ւ̃|�C���^��NULL�ŏ�����
	void HmmRelease();	   //�m�ۂ����������̈�����
	void HmmCnt();		   //�z�[�~���O�~�T�C�����˂܂ł̃J�E���g��i�߂�
	void HmmCntReset();	   //�z�[�~���O�~�T�C�����˂܂ł̃J�E���g�����Z�b�g������
	void HmmReset();	   //�S�Ẵz�[�~���O�~�T�C���𔭎ˑҋ@��Ԃɖ߂�
	void HmmGetTekiInfo(); //���b�N�I�����邽�߂Ɏ��@�Ɉ�ԋ߂������ɂ���G�L�����̏����擾����
	void HmmMovCnt();	   //���ɔ��˂��ꂽ�z�[�~���O�~�T�C���̈ړ��ʂ����߂�
	void HmmAttack();	   //�z�[�~���O�~�T�C�����˂ƈړ��̏���
	void HmmAtari();	   //�G�L�����Ɠ����蔻��
	void HmmRend();		   //�z�[�~���O�~�T�C���̕`�揈��
	//-------------------------------------------------------------------------

	//---------------�ȉ��̃��\�b�h��DrawJIKI_Hnb.cpp�ɋL�q--------------------
	BOOL InitHnb();		//�ԉ΃I�u�W�F�N�g�̏�����
	void InitHnbTm();	//���I�Ɋm�ۂ��郁�����̈�ւ̃|�C���^��NULL�ŏ�����
	void HnbRelease();	//�m�ۂ����������̈�����
	void HnbCnt();		//�ԉΔ��˂܂ł̃J�E���g��i�߂�
	void HnbCntReset(); //�ԉΔ��˂܂ł̃J�E���g�����Z�b�g������
	void HnbReset();	//�S�Ẳԉ΂𔭎ˑҋ@��Ԃɖ߂�
	void HnbAttack();	//�ԉΔ��˂ƈړ��̏���
	void HnbAtari();	//�ԉ΂̓G�L�����Ƃ̓����蔻��
	void HnbRend();		//�ԉ΂̕`�揈��
	//-------------------------------------------------------------------------

	//---------------�ȉ��̃��\�b�h��DrawJIKI_Opt.cpp�ɋL�q--------------------
	BOOL InitOpt();			  //�I�v�V�����I�u�W�F�N�g�̏�����
	void InitOptAtkDatTm();	  //���I�Ɋm�ۂ��郁�����̈�ւ̃|�C���^��NULL�ŏ�����
	void OptRelease();		  //�m�ۂ����������̈�����
	void InitOptCdn();		  //�I�v�V�����̕\�����W�̏�����
	void OptMovCnt(BYTE);	  //�I�v�V�����̈ړ��ʂ����߂�
	void OptMov();			  //�I�v�V�����̈ړ�����
	void OptAttackCnt();	  //�e���˂܂ł̃J�E���g��i�߂�
	void OptAttackCntReset(); //�e���˂܂ł̃J�E���g�����Z�b�g����
	void OptAttackReset();	  //�S�ẴI�v�V�����̔��˂����e�𔭎ˑҋ@��Ԃɖ߂�
	void OptAttack();		  //�I�v�V�����̍U������
	void OptAttackAtari();	  //���˂����e�Ƃ̓����蔻����s�Ȃ�
	void OptRend();			  //�I�v�V�����̕`�揈��
	void OptWepRend();		  //�I�v�V�����̔��˂����e�̕`��
							  //-------------------------------------------------------------------------

public:
	DWORD SP_Num;	   //�G�L�����̔ԍ�
	DWORD SP_fEvt;	   //�G�L�����̎��
	DWORD SP_fSPChr;   //�G�L�����̏��
	POINT SP_Cdn;	   //�G�L�����̍��W
	short SP_Width;	   //�G�L�����̉���
	short SP_Height;   //�G�L�����̏c��
	short *SP_Shld;	   //�G�L�����̎c��ϋv�͂ւ̃|�C���^
	TAMAS *SP_Tamas;   //�G�L�����̒e�̏�Ԃ��Ǘ�����TAMAS�\���̔z��̃|�C���^
	TAMAS2 *SP_Tamas2; //�G�L�����̒e�̏�Ԃ��Ǘ�����TAMAS2�\���̔z��̃|�C���^
	TAMAS3 *SP_Tamas3; //�G�L�����̒e�̏�Ԃ��Ǘ�����TAMAS3�\���̔z��̃|�C���^

	//---------------�ȉ��̃��\�b�h��DrawJIKI.cpp�ɋL�q---------------------------
	BOOL InitJIKI();	//���@�I�u�W�F�N�g������
	void MoveJIKI();	//���@�̈ړ��ƒe���˂܂ł̃J�E���g�_�E��
	DWORD AtariJIKI();	//���@�ƓG�L�����̓����蔻��
	void FrameJIKI();	//���@�Ɋւ���O���t�B�b�N�̕`��Ɏg�p����e�l���X�V����
	void RendJIKI();	//���@�Ɋւ���O���t�B�b�N��`��
	DWORD GetfJIKI();	//���@�̏�ԃf�[�^��߂�l�Ƃ��ĕԂ�
	POINT GetJIKICdn(); //���@�̉�ʍ��W��߂�l�Ƃ��ĕԂ�

	DrawJIKI();
	virtual ~DrawJIKI();
	//-------------------------------------------------------------------------
};
