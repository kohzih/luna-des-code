/*====================================================================
	WinMain�֐��Ɋւ���\�[�X�t�@�C��(��������v���O�����̋N��)
=====================================================================*/

#define INITGUID
#include <windows.h>
#include <ddraw.h>
#include <dsound.h>
#include <dmusicc.h>
#include <dmusici.h>
#include <dinput.h>

#include "Macros.h"
#include ".\\BitMap\\resource.h"
#include "Globals.h"

char *szClassName = "LUNAR DESTINY beta"; //�N���X��
char *szWindowCaption = "LUNAR DESTINY";  //�^�C�g����

//�֐��̃v���g�^�C�v�錾
BOOL MakeWindowClass(HINSTANCE, WNDPROC, char *, char *);
void ShowUpdateWindow(HWND, int);
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
int MessageLoop(void);

BOOL InitDD(void);
BOOL InitDS(void);
BOOL InitDM(void);
BOOL InitDI(void);
void ReleaseDD(void);
void ReleaseDS(void);
void ReleaseDM(void);
void ReleaseDI(void);

BOOL InitETC(void);
void ReleaseETC(void);

//------------------------------------------------
//�E�B�����C���֐�
//------------------------------------------------
int WINAPI WinMain(HINSTANCE hThisInst, //���݂̃C���X�^���X�n���h��
				   HINSTANCE hPrevInst, //�ȑO�̃C���X�^���X�n���h��
				   LPSTR lpszArgs,		//
				   int nWinMode)		//�E�B���h�E�̕\�����
{
	//�N���X��`
	if (!MakeWindowClass(hThisInst,					  //�C���X�^���X�n���h��
						 WindowProc,				  //�E�B���h�E�v���V�[�W��
						 szClassName,				  //�N���X��
						 MAKEINTRESOURCE(IDI_ICON1))) //�A�C�R��
		return FALSE;

	hInst = hThisInst; //�C���X�^���X�n���h����ۑ�

	//�E�B���h�E�g�̒���(�E�B���h�E�\���p)
	RECT rc;
	DWORD width;
	DWORD height;

	SetRect(&rc, 0, 0, WinXMax, WinYMax);
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;

	//�E�B���h�E���쐬����
	hWnd = CreateWindow(
		szClassName,		   //�N���X��
		szWindowCaption,	   //�^�C�g����
		WS_POPUP | WS_BORDER | //�E�B���h�E�X�^�C��
			WS_CAPTION | WS_SYSMENU,
		0, 32, width, height, //���W�ƕ�����
		NULL, NULL,			  //�e�E�B���h�E�n���h���ƃ��j���[�n���h��
		hThisInst,			  // WM_CREATE���ɎQ�Ƃ���E�B���h�E�쐬���
		NULL);

	// ShowWinsow + UpdateWindow�֐�
	ShowUpdateWindow(hWnd, nWinMode);

	// DirectDraw������
	if (!InitDD())
	{
		MessageBox(hWnd, "DirectDraw������", "�G���[", MB_OK | MB_APPLMODAL);
		//�I�u�W�F�N�g���
		ReleaseDD();
		return FALSE;
	}
	// DirectSound������
	if (!InitDS())
	{

		MessageBox(hWnd, "DirectSound������", "�G���[", MB_OK | MB_APPLMODAL);
		ReleaseDS();
		ReleaseDD();
		return FALSE;
	}
	// DirectMusic������
	if (!InitDM())
	{
		MessageBox(hWnd, "DirectMusic������", "�G���[", MB_OK | MB_APPLMODAL);
		ReleaseDM();
		ReleaseDS();
		ReleaseDD();
		return FALSE;
	}
	// DirectInput������
	if (!InitDI())
	{

		MessageBox(hWnd, "DirectInput������", "�G���[", MB_OK | MB_APPLMODAL);
		ReleaseDI();
		ReleaseDM();
		ReleaseDS();
		ReleaseDD();
		return FALSE;
	}
	//����I�u�W�F�N�g�̏�����
	if (!InitETC())
	{
		MessageBox(hWnd, "����I�u�W�F�N�g�̏�����", "�G���[", MB_OK | MB_APPLMODAL);
		ReleaseETC();
		ReleaseDI();
		ReleaseDM();
		ReleaseDS();
		ReleaseDD();
		return FALSE;
	}

	//���b�Z�[�W���[�v
	return MessageLoop();
}

//---------------------------------------------------------
//�E�B���h�E�N���X��`�֐�
//---------------------------------------------------------
BOOL MakeWindowClass(HINSTANCE hThisInst, //�C���X�^���X�n���h��
					 WNDPROC WindowProc,  //�E�B���h�E�v���V�[�W��
					 char *szClassName,	  //�N���X��
					 char *Icon)		  //�A�C�R��
{
	WNDCLASS wc;

	wc.hInstance = hThisInst;		//�C���X�^���X�n���h��
	wc.lpszClassName = szClassName; //�N���X��
	wc.lpfnWndProc = WindowProc;	//�E�B���h�E�v���V�[�W��
	wc.style = CS_BYTEALIGNCLIENT | //�E�B���h�E�X�^�C��
			   CS_VREDRAW | CS_HREDRAW;
	wc.hIcon = LoadIcon(hThisInst, Icon);					//�A�C�R���̃n���h��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				//�J�[�\���̃n���h��
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //�E�B���h�E�̔w�i�F
	wc.lpszMenuName = NULL;									//���j���[��
	wc.cbClsExtra = 0;										//�N���X�g�����
	wc.cbWndExtra = 0;										//�E�B���h�E�g�����
	if (!RegisterClass(&wc))
		return FALSE; //�N���X��`��o�^

	return TRUE;
}

//---------------------------------------------------------
// ShowWindow�֐��@+�@UpdateWindow�֐�
//---------------------------------------------------------
void ShowUpdateWindow(HWND hwnd, int nWinMode)
{
	ShowWindow(hwnd, nWinMode);
	UpdateWindow(hwnd);
}