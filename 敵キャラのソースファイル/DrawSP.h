/*====================================================================
	�G�L�����N���X�̒�`
=====================================================================*/

#include "TEKI01.h"
#include "TEKI02.h"
#include "BOSS01.h"

class DrawSP : protected Draw
{
	DWORD fEvt;		//���̓G�L�����̎�ނ�\�킷�t���O
	DWORD JK_fJIKI; //���@�̏�ԃt���O�̃R�s�[
	POINT JK_Cdn;	//���@�̉�ʍ��W�̃R�s�[

	TEKI01 *Teki01; //�G�L�����̎�ނ�01�������ꍇ�Ɋm�ۂ���郁�����̈�ւ̃|�C���^
	TEKI02 *Teki02; //�G�L�����̎�ނ�02�������ꍇ�Ɋm�ۂ���郁�����̈�ւ̃|�C���^
	BOSS01 *Boss01; //�G�L�����̎�ނ�03�������ꍇ�Ɋm�ۂ���郁�����̈�ւ̃|�C���^

public:
	//------------------�ȉ��̃��\�b�h��DrawSP.cpp�ɋL�q-----------------------//
	BOOL InitSP(POINT, WORD);		   //������MapCdn�ƃC�x���g��������
	void MoveSP();					   //�G�L�����𓮂���
	void TamaSP();					   //�G�L�����G�̒e���Ǘ�����
	void FrameSP(DWORD, DWORD, POINT); //�G�L�����̕`�揀��
	void RendSP();					   //�G�L������`�悷��
	void GetTekiData(DWORD &fevt,	   //�����Ɋe�l�������Ď��@�I�u�W�F�N�g�ɓn��
					 DWORD &fspchr,
					 POINT &cdn,
					 short &width,
					 short &height,
					 short *&shldcnt,
					 TAMAS *&tamas,
					 TAMAS2 *&tamas2,
					 TAMAS3 *&tamas3);

	DrawSP();
	~DrawSP();
	//-------------------------------------------------------------------------//
};
