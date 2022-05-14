/*=============================================================================
	�`��Ǘ��A���@�A�G�L�����A�w�i�A�p���b�g�Ǘ��N���X�p�̃}�N����`
==============================================================================*/
//�X�^�[�g����X�e�[�W�ԍ�
#define	_StNum		0			//0:�v�����[�O��ʁA1:�^�C�g����ʁA2:�X�e�[�W1

//�X�e�[�W�̊J�n�ƏI����\���}�N��(Draw.fSTAGE�t���O�p)
#define _STAGE_STARTWAIT_ON		0x00000001		//���̃X�e�[�W���n�܂������Ƃ������t���O
#define _STAGE_STARTWAIT_OFF	0xFFFFFFFE		//���̃X�e�[�W���n�܂������Ƃ������t���O�I�t
#define	_STAGE_START_ON			0x00000002		//���̃X�e�[�W�̊J�n�������ł��邱�Ƃ������t���O
#define _STAGE_START_OFF		0xFFFFFFFD		//���̃X�e�[�W�̊J�n�������ł��邱�Ƃ������I�t
#define	_STAGE_ENDWAIT_ON		0x00000004		//�X�e�[�W���I���������Ƃ������t���O
#define	_STAGE_ENDWAIT_OFF		0xFFFFFFFB		//�X�e�[�W���I���������Ƃ������t���O�I�t
#define	_STAGE_END_ON			0x00000008		//�X�e�[�W�̏I���������ł��邱�Ƃ������t���O
#define	_STAGE_END_OFF			0xFFFFFFF7		//�X�e�[�W�̏I���������ł��邱�Ƃ������t���O�I�t
#define	_GAME_JUMP_ON			0x00000010		//���C���̃Q�[����������؍s��Ȃ����Ƃ������t���O
#define	_GAME_JUMP_OFF			0xFFFFFFEF		//���C���̃Q�[����������؍s��Ȃ����Ƃ������t���O�I�t
#define	_GAME_OVER_ON			0x00000020		//�Q�[���I�[�o�[�ɂȂ������������t���O
#define	_GAME_OVER_OFF			0xFFFFFFDF		//�Q�[���I�[�o�[�ɂȂ������������t���O�I�t
#define	_GAME_CLEAR_ON			0x00000040		//�Q�[���N���A�ɂȂ������������t���O
#define	_GAME_CLEAR_OFF			0xFFFFFFBF		//�Q�[���N���A�ɂȂ������������t���O�I�t
#define	_GAME_EXIT				0x80000000		//�Q�[�����I���������Ƃ������t���O(�v���O�����I��)		
#define	_fSTAGE_CHK				0x8000007F		//���ݗ����Ă���t���O���`�F�b�N
#define	_fSTAGE_CHK_InitSTAGE	0x0000000F		//InitSTAGE���Ń`�F�b�N�Ɏg�p

//FaInOut::SetFade(****)�����̈����Ƃ��Ďg�p����}�N��
#define	_SetDef		0x00000001	//�z���C�g�A�E�g�A�u���b�N�A�E�g�̏������s��	
#define	_SetWhiteIn	0x00000002	//�z���C�g�C���̏������s��
#define	_SetBlackIn	0x00000004	//�u���b�N�C���̏������s��

//�`�悷��w�i��ω�������}�N��(DrawBG::fBG�t���O�p)
#define	_SetBG00_ON		0x00000001
#define	_SetBG00_OFF	0xFFFFFFFE
#define	_SetBG01_ON		0x00000002
#define	_SetBG01_OFF	0xFFFFFFFD

//�W���C�X�e�B�b�N�p�}�N��
#define	JOY_UP		1
#define	JOY_DOWN	2
#define	JOY_LEFT	4
#define	JOY_RIGHT	8
#define	JOY_A		16
#define	JOY_B		32

//�Q�[�����d�͉����x
#define	_G			0.097970775

//���@��`�͈͗p�}�N��
#define	_JIKI_W		20				//���@�X�v���C�g�̉���
//���@�ړ��֘A�̃}�N��
#define	_JIKIMOVE	3				//���@�̈ړ���
//EXP�֘A�}�N��
#define	_EXPUP		0x00008000		//�e����z���������ɏ㏸����EXP�̒l
#define	_EXPSH		0x000A0000		//�G�̒e�ƐڐG�������Ɍ���EXP�̒l
//FUEL�֘A�}�N��
#define	_FUELUP		0x000A0000		//�e����A�[���ŋz���������ɑ�����FUEL�Q�[�W�̗�
#define	_FUELKY		0x00008000		//�G�e�z�����̒e�z���ɏ����FUEL�l(FUEL�Q�[�W�̌���X�s�[�h)
#define	_FUELMV		10				//�G�e�z�����̒e���ڋ߂��Ă���X�s�[�h
//���@�̍U���֘A�̃}�N��
#define	_PWUPDDMG	0x0000000A		//�p���[�A�b�v��ʂ𔲂������̃_���[�W��(10)
#define	_ARMDMG		0x00000005		//�A�[���ōU���������̃_���[�W��(5)
#define	_LSR1DMG	0x00000005		//���[�U�[LV1 1��������̃_���[�W��(5)
#define	_MSR1DMG	0x00000003		//�~�T�C��LV1 1��������̃_���[�W��(3)
#define	_HMM1DMG	0x0000000A		//�z�[�~���O�~�T�C��LV1 1��������̃_���[�W��(10)
#define	_HNB1DMG	0x00000014		//�ԉ�LV1 1��������̃_���[�W��(20)
//���@�e�̈ړ���
#define	_HMM_MOV	0x00000008		//�z�[�~���O�~�T�C���̈ړ���

//���@�̏�ԊǗ��t���O�̃}�N����`(DrawJIKI::fJIKI�t���O�p)
#define	_NORMAL		0x00000000		//�������
#define	_ARM_ON		0x00000001		//�A�[������
#define	_ARM_OFF	0xFFFFFFFE		//�A�[�����˃t���O�I�t
#define	_FUEL_ON	0x00000002		//�e�z����			
#define	_FUEL_OFF	0xFFFFFFFD		//�e�z�����t���O�I�t
#define	_FUELD_ON	0x00000004		//�e�z�����f�t���O
#define	_FUELD_OFF	0xFFFFFFFB		//�e�z�����f�t���O�I�t
#define	_PWUP_ON	0x00000008		//�p���[�A�b�v������ʒ����Ƃ������t���O
#define	_PWUP_OFF	0xFFFFFFF7		//�p���[�A�b�v������ʒ����Ƃ������t���O�I�t
#define	_PWUPD_ON	0x00000010		//���@�p���[�A�b�v�ɔ�����ʂ̓G�L���������ł�����t���O
#define	_PWUPD_OFF	0xFFFFFFEF		//���@�p���[�A�b�v�ɔ�����ʂ̓G�L���������ł�����t���O�I�t
#define	_WEPN_ON	0x00000020		//��ʏ�Ɏ�C�̒e�����݂��Ă��鎖�������t���O
#define	_WEPN_OFF	0xFFFFFFDF		//��ʏ�Ɏ�C�̒e�����݂��Ă��鎖�������t���O�I�t		
#define	_SUBHMM_ON	0x00000040		//��ʏ�Ƀz�[�~���O�~�T�C�������݂��Ă��鎖�������t���O
#define	_SUBHMM_OFF	0xFFFFFFBF		//��ʏ�Ƀz�[�~���O�~�T�C�������݂��Ă��鎖�������t���O�I�t
#define	_SUBHNB_ON	0x00000080		//��ʏ�ɉԉ΂����݂��Ă��鎖�������t���O
#define	_SUBHNB_OFF	0xFFFFFF7F		//��ʏ�ɉԉ΂����݂��Ă��鎖�������t���O�I�t
#define	_OPT_ON		0x00000100		//�I�v�V��������������Ă��鎖�������t���O
#define	_OPT_OFF	0xFFFFFEFF		//�I�v�V��������������Ă��鎖�������t���O�I�t	
#define	_OPTWEP_ON	0x00000200		//��ʏ�ɃI�v�V�������˂����e�����݂��Ă��鎖�������t���O
#define	_OPTWEP_OFF	0xFFFFFDFF		//��ʏ�ɃI�v�V�������˂����e�����݂��Ă��鎖�������t���O�I�t
#define	_CONTINUE_ON	0x00000400	//�R���e�B�j���[���I�����ꂽ���������t���O
#define	_CONTINUE_OFF	0xFFFFFBFF	//�R���e�B�j���[���I�����ꂽ���������t���O�I�t
#define	_INVINCIBLE_ON	0x00000800	//�R���e�B�j���[�ɔ�����莞�Ԗ��G��Ԃł��邱�Ƃ������t���O
#define	_INVINCIBLE_OFF	0xFFFFF7FF	//�R���e�B�j���[�ɔ�����莞�Ԗ��G��Ԃł��邱�Ƃ������t���O�I�t
#define	_TARGET_ON	0x00001000		//�{�X�L�������o���������Ƃ������t���O
#define	_TARGET_OFF	0xFFFFEFFF		//�{�X�L�������o���������Ƃ������t���O�I�t
#define	_BOSS_ON	0x00002000		//�{�X�Ɛ퓬���ł��邱�Ƃ������t���O
#define	_BOSS_OFF	0xFFFFDFFF		//�{�X�Ɛ퓬���ł��邱�Ƃ������t���O�I�t
#define	_END_ON		0x40000000		//���@�����Ă��ꂽ���Ƃ������t���O
#define	_END_OFF	0xBFFFFFFF		//���@�����Ă��ꂽ���Ƃ������t���O�I�t
#define	_GAME_STOP_ON	0x80000000	//�Q�[�����ꎞ��~������t���O
#define	_GAME_STOP_OFF	0x7FFFFFFF	//�Q�[�����ꎞ��~������t���O�I�t


//���@�̊e�푕���Ɋւ���t���O�̃}�N����`(DrawJIKI::Wpd.flag�t���O�p)
#define	_LSR1_ON	0x00000001	//���[�U�[���x���P�������t���O
#define	_LSR1_OFF	0xFFFFFFFE
#define	_LSR2_ON	0x00000002	//	�V			�Q
#define	_LSR2_OFF	0xFFFFFFFD
#define	_LSR3_ON	0x00000004
#define	_LSR3_OFF	0xFFFFFFFB
#define	_LSR4_ON	0x00000008
#define	_LSR4_OFF	0xFFFFFFF7
#define	_LSR5_ON	0x00000010
#define	_LSR5_OFF	0xFFFFFFEF
#define	_LSR_LVCHK	0x0000001F	//���݂̃��[�U�[���x���`�F�b�N�p
#define	_MSR1_ON	0x00000020	//�~�T�C�����x���P
#define	_MSR1_OFF	0xFFFFFFDF
#define	_MSR2_ON	0x00000040	//	�V			�Q
#define	_MSR2_OFF	0xFFFFFFBF
#define	_MSR3_ON	0x00000080
#define	_MSR3_OFF	0xFFFFFF7F
#define	_MSR4_ON	0x00000100
#define	_MSR4_OFF	0xFFFFFEFF
#define	_MSR5_ON	0x00000200
#define	_MSR5_OFF	0xFFFFFDFF
#define	_MSR_LVCHK	0x000003E0	//���݂̃~�T�C�����x���`�F�b�N�p
#define	_HMM1_ON	0x00000400	//�z�[�~���O�~�T�C�����x���P
#define	_HMM1_OFF	0xFFFFFBFF
#define	_HMM2_ON	0x00000800
#define	_HMM2_OFF	0xFFFFF7FF
#define	_HMM3_ON	0x00001000
#define	_HMM3_OFF	0xFFFFEFFF
#define	_HMM4_ON	0x00002000
#define	_HMM4_OFF	0xFFFFDFFF
#define	_HMM5_ON	0x00004000
#define	_HMM5_OFF	0xFFFFBFFF
#define	_HMM_LVCHK	0x00007C00	//���݂̃z�[�~���O�~�T�C�����x���`�F�b�N�p
#define	_HNB1_ON	0x00008000	//�ԉ΃��x���P
#define	_HNB1_OFF	0xFFFF7FFF
#define	_HNB2_ON	0x00010000
#define	_HNB2_OFF	0xFFFEFFFF
#define	_HNB3_ON	0x00020000
#define	_HNB3_OFF	0xFFFDFFFF
#define	_HNB4_ON	0x00040000
#define	_HNB4_OFF	0xFFFBFFFF
#define	_HNB5_ON	0x00080000
#define	_HNB5_OFF	0xFFF7FFFF
#define	_HNB_LVCHK	0x000F8000	//���݂̉ԉ΃��x���`�F�b�N�p
#define	_OPT1_ON	0x00100000	//�I�v�V�������x���P
#define	_OPT1_OFF	0xFFEFFFFF
#define	_OPT2_ON	0x00200000
#define	_OPT2_OFF	0xFFDFFFFF
#define	_OPT3_ON	0x00400000
#define	_OPT3_OFF	0xFFBFFFFF
#define	_OPT4_ON	0x00800000
#define	_OPT4_OFF	0xFF7FFFFF
#define	_OPT_LVCHK	0x00F00000
#define	_LSR_ON		0x01000000	//��C�Ƀ��[�U�[���I������Ă���
#define	_LSR_OFF	0xFEFFFFFF
#define	_MSR_ON		0x02000000	//��C�Ƀ~�T�C�����I������Ă���
#define	_MSR_OFF	0xFDFFFFFF
#define	_MAINWP_CHK	0x03000000	//��C�Ɍ��݂ǂ��炪�I������Ă��邩�`�F�b�N�p
#define	_HMM_ON		0x04000000	//�T�u�E�F�|���Ƀz�[�~���O�~�T�C�����I������Ă���
#define	_HMM_OFF	0xFBFFFFFF
#define	_HNB_ON		0x08000000	//�T�u�E�F�|���ɉԉ΂��I������Ă���
#define	_HNB_OFF	0xF7FFFFFF
#define	_SUBWP_CHK	0x0C000000	//�T�u�E�F�|���Ɍ��݂ǂ��炪�I������Ă��邩�`�F�b�N�p

//�z�[�~���O�~�T�C���̔��ˏ󋵂������t���O(DrawJIKI::HMMDAT.fHMM�t���O�p)
#define	_LOCKHMM_ON	0x00000001	//�G�L�����̃��b�N�I�����t���O
#define	_LOCKHMM_OFF 0xFFFFFFFE	//�G�L�����̃��b�N�I�����t���O����
#define	_LOCKST_ON	0x00000002	//���b�N�I���J�n����������t���O
#define	_LOCKST_OFF	0xFFFFFFFD	//���b�N�I���J�n����������t���O����			

//�I�v�V�����̍s���p�^�[�����Ǘ�����t���O(DrawJIKI::OPT.Jewel[0-3].fOPT�t���O�p
#define	_OPTKO_ON	0x00000001	//�Œ�
#define	_OPTKO_OFF	0xFFFFFFFE
#define _OPTKA_ON	0x00000002	//��]
#define	_OPTKA_OFF	0xFFFFFFFD
#define	_OPTTU_ON	0x00000004	//�ǔ�
#define	_OPTTU_OFF	0xFFFFFFFB
#define	_fOPT_CHK	0x00000007	//�`�F�b�N�p

//�G�L�����̏�ԂɊւ���t���O�̃}�N����`(DrawSP:fSPChr�t���O�p)
#define	_SPON		0x00000001	//�G�L�����\��
#define	_SPOFF		0xFFFFFFFE	//�G�L�����\���t���O�I�t
#define	_ATCKON		0x00000002	//�U�����t���O�I��
#define	_ATCKOFF	0xFFFFFFFD	//�U�����t���O�I�t
#define _HITON		0x00000004	//���@�̒e�ɐڐG���ă_���[�W���󂯂����������t���O
#define _HITOFF		0xFFFFFFFB	//�_���[�W���󂯂����Ƃ������t���O�I�t
#define	_TAMA1ON	0x00000008	//��ʒe���݃t���O1(TAMA�p)
#define	_TAMA1OFF	0xFFFFFFF7	//��ʒe���݃t���O1�I�t
#define	_TAMA2ON	0x00000010	//��ʒe���݃t���O2	(TAMA2�p)
#define	_TAMA2OFF	0xFFFFFFEF	//��ʒe���݃t���O2�I�t
#define	_TAMA3ON	0x00000020	//��ʒe���݃t���O3(TAMA3�p)
#define	_TAMA3OFF	0xFFFFFFDF	//��ʒe���݃t���O3�I�t
#define	_SPDES_a	0x40000000	//�G�L�����j��A�j����2�i�K
#define	_SPDESOFF_a	0xBFFFFFFF	//�G�L�����j��A�j����2�i�K�I�t
#define	_SPDES		0x80000000	//�G�L�����j��
#define	_SPDESOFF	0x7FFFFFFF	//�G�L�����j��t���O�I�t
#define	_SPDESCHK	0x80000001	//�G�L���������S�ɔj�󂳂�Ă��邩�`�F�b�N�p

//�e�I�u�W�F�N�g�Ɋւ���t���O�̃}�N����`(TAMA.fTama�t���O�p)
#define	_TSET		0x00000001	//������
#define	_TON		0x00000002	//���˒�
#define	_TOFF		0x00000000	//���ˍϏ���
#define	_TSAKU_ON	0x00000004	//�ԉ��y��(�ԉΗp)
#define	_TSAKU_OFF	0xFFFFFFFB	//�ԉ��y��I�t(�ԉΗp)
#define	_THIT_ON	0x00000008	//�e���_���[�W���󂯂����Ƃ������t���O(TAMA3�\���̗p)
#define	_THIT_OFF	0xFFFFFFF7	//�e���_���[�W���󂯂����Ƃ������t���O�I�t
#define	_TBAKU_ON	0x00000010	//�e���j�󂳂ꂽ���������t���O(TAMA3�\���̗p)
#define	_TBAKU_OFF	0xFFFFFFEF	//�e���j�󂳂ꂽ���������t���O�I�t

