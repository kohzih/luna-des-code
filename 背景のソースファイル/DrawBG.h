/*====================================================================
	�w�i�N���X�̒�`
=====================================================================*/

class DrawBG : protected Draw
{
	DWORD fBG;		//�w�i�̏�Ԃ������t���O
	DWORD JK_fJIKI; //���@�̏�ԃt���O�̃R�s�[

	int ScrCnt; //��ʂ̃X�N���[���J�E���^
	int ScrSpd; //��ʂ̃X�N���[���X�s�[�h
	int ScrWt;	//�X�N���[����~�J�E���^

public:
	BOOL InitBG();			 // DrawBG��������
	void MoveBG();			 //��΍��W���ړ�������ʃX�N���[��
	void FrameBG(DWORD);	 //�����Ŏ��@�t���O���󂯎��`�揀��
	void RendBG();			 //�w�i�̕`��
	DWORD GetfBG();			 //���݂�BG�t���O��Ԃ�
	void SetfBG(DWORD, int); //�����œn���ꂽ�t���O���Z�b�g

	DrawBG();
	~DrawBG();
};