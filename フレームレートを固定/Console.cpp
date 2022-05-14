/*====================================================================
	文字の入出力とフレームレート固定関数のソースファイル
=====================================================================*/

#include	<windows.h>
#include	<ddraw.h>

#include	"Macros.h"

extern	LPDIRECTDRAWSURFACE7	lpDDSBack;			//バックサーフェース

//---------------------------------------------------------
//テキスト表示関数
//---------------------------------------------------------
void	DrawText(char *text,int x,int y,COLORREF rgb)
{
	HDC		hdc;
	lpDDSBack->GetDC(&hdc);
	SetTextColor(hdc,rgb);
	SetBkMode(hdc,OPAQUE);			//背景透明　TRANSPARENT　指定
	TextOut(hdc,x,y,text,lstrlen(text));
	lpDDSBack->ReleaseDC(hdc);
}

//---------------------------------------------------------
//変数表示関数
//---------------------------------------------------------
void	DrawText(char *text,long d,int x,int y,COLORREF rgb)
{
	HDC		hdc;
	char	msg[30];		

	lpDDSBack->GetDC(&hdc);
	
	SetTextColor(hdc,rgb);
	SetBkMode(hdc,OPAQUE);			//背景不透明
	wsprintf(msg,"%s=(%d)",text,d);
	TextOut(hdc,x,y,msg,lstrlen(msg));
	
	lpDDSBack->ReleaseDC(hdc);
}

//---------------------------------------------------------
//フレームレート固定＆表示関数
//---------------------------------------------------------
void	FrameRateDraw(void)
{
	static	DWORD	framerate;
	static	DWORD	timeNew=0,timeOld=0,timeOld2=0;
					//現在の経過時間、１秒後比較用時間、１／６０秒比較用時間

#if	DEBUG >=1
			HDC		hdc;
	static	char	string[64];
#endif

	framerate	++;					//フレームカウントを１増やす
	
	while(timeNew-timeOld2<16)		//1/60秒経過しているかチェック
	{								//経過するまでループ処理
		timeNew=timeGetTime();
	}
	timeOld2=timeNew;

	if(timeNew-timeOld>1000)		//１秒間に何回フレームカウントがアップしたか表示
	{

#if	DEBUG >=1		
		wsprintf(string,"%dFPS   ",framerate);
#endif		
		framerate=0;
		timeOld=timeNew;
	}
	
#if	DEBUG >= 1
	lpDDSBack->GetDC(&hdc);	
	TextOut(hdc,0,100,string,lstrlen(string));
	lpDDSBack->ReleaseDC(hdc);	
#endif
	
}

