#define UNICODE
#pragma comment(linker,"/opt:nowin98")
#include<windows.h>
#include <limits.h>
#ifdef __cplusplus
extern "C" {
#endif
#include"mt19937ar.h"
#ifdef __cplusplus
}
#endif

#define WM_ENDTHREAD (WM_APP)
TCHAR szClassName[]=TEXT("Window");
BOOL bEndThred1;
LONGLONG dwCount;
LONGLONG dwPayment;
LONGLONG dwPrize;
LONGLONG dwLoss;

DWORD WINAPI ThreadFunc1(LPVOID lpV)
{
	HWND hWnd = (HWND)lpV;
	UINT r;
	while(!bEndThred1)
	{
		r = genrand_int32();
		if(r >= 4290000000 )continue;

		dwCount++;
		dwPayment += 300;

		if(r%10==0) //7�� 300�~
		{
			dwPrize += 300;
		}

		if(r%100==0) //6�� 3,000�~
		{
			dwPrize += 3000;
		}

		if(r%1000==0) //5�� 10,000�~
		{
			dwPrize += 10000;
		}

		if(r%100000==0) //4�� 500,000�~
		{
			dwPrize += 500000;
		}

		if(r%500000==0) //3�� 1,000,000�~
		{
			dwPrize += 1000000;
		}

		if(r%10000000==0) //2�� 100,000,000�~
		{
			dwPrize += 100000000;
		}

		if(r%5000000==0) //1�� 200,000,000�~
		{
			dwPrize += 200000000;
		}

		if(r%2500000==0) //1���O�� 50,000,000�~
		{
			dwPrize += 50000000;
		}

		if(r%50000==0&&(r/50000)%100!=0) //1���g�� 100,000�~
		{
			dwPrize += 100000;
		}

		dwLoss = dwPrize - dwPayment;
		InvalidateRect(hWnd,0,0);
		Sleep(50);
		
	}
	PostMessage(hWnd,WM_ENDTHREAD,0,0);
	return 0;
}

LRESULT CALLBACK WndProc(
						 HWND hWnd,
						 UINT msg,
						 WPARAM wParam,
						 LPARAM lParam)
{
	static TCHAR szText[1024];
	static DWORD d;
	static HANDLE hThread=0;
	static HWND hButton1,hButton2,hButton3;
	switch(msg)
	{
	case WM_CREATE:
		hButton1=CreateWindow(
			TEXT("BUTTON"),
			TEXT("�J�n(&S)"),
			WS_VISIBLE|WS_CHILD,
			10,10,100,30,
			hWnd,
			(HMENU)100,
			((LPCREATESTRUCT)lParam)->hInstance,
			0);
		hButton2=CreateWindow(
			TEXT("BUTTON"),
			TEXT("���Z�b�g(&R)"),
			WS_VISIBLE|WS_CHILD,
			10,50,100,30,
			hWnd,
			(HMENU)101,
			((LPCREATESTRUCT)lParam)->hInstance,
			0);
		init_genrand((unsigned long)GetTickCount());
		break;
	case WM_COMMAND:
		if(LOWORD(wParam)==100)
		{
			EnableWindow(hButton1,FALSE);
			bEndThred1 = FALSE;
			hThread=CreateThread(NULL,0,ThreadFunc1,(LPVOID)hWnd,0,&d);
			SetWindowText(hButton2,TEXT("��~(&T)"));
		}
		else if(LOWORD(wParam)==101)
		{
			if(hThread)
			{
				bEndThred1=TRUE;
			}
			else
			{
				dwCount=0;
				dwPayment=0;
				dwPrize=0;
				dwLoss=0;
				InvalidateRect(hWnd,0,1);
			}
		}
		break;
	case WM_ENDTHREAD:
		WaitForSingleObject(hThread,INFINITE);
		CloseHandle(hThread);
		hThread=0;
		EnableWindow(hButton1,TRUE);
		SetWindowText(hButton2,TEXT("���Z�b�g(&R)"));
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc=BeginPaint(hWnd,&ps);

			COLORREF crOldBkColor=SetBkColor(hdc,GetSysColor(COLOR_3DFACE));
			DWORD dwOldBkMode=SetBkMode(hdc,OPAQUE);

			wsprintf(szText,TEXT("�w�����F %I64d��"),dwCount);
			TextOut(hdc,120,10,szText,lstrlen(szText));

			wsprintf(szText,TEXT("�o���z�F %I64d�~"),dwPayment);
			TextOut(hdc,120,40,szText,lstrlen(szText));

			wsprintf(szText,TEXT("�z�����F %I64d�~"),dwPrize);
			TextOut(hdc,120,70,szText,lstrlen(szText));

			wsprintf(szText,TEXT("�����z�F %I64d�~"),dwLoss);
			TextOut(hdc,120,100,szText,lstrlen(szText));

			SetBkMode(hdc,dwOldBkMode);
			SetBkColor(hdc,crOldBkColor);

			EndPaint(hWnd,&ps);
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		if(hThread)
		{
			bEndThred1=TRUE;
		}
		PostQuitMessage(0);
		break;
	default:
		return DefDlgProc(hWnd,msg,wParam,lParam);
	}
	return 0;
}

EXTERN_C void __cdecl WinMainCRTStartup()
{
	MSG msg;
	HINSTANCE hInstance=GetModuleHandle(0);
	WNDCLASS wndclass={
		CS_HREDRAW|CS_VREDRAW,
		WndProc,
		0,
		DLGWINDOWEXTRA,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	RECT rect;
	SetRect(&rect,0,0,256+64,128+10);
	AdjustWindowRect(
		&rect,
		WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU,
		0);
	HWND hWnd=CreateWindow(
			szClassName,
			TEXT("�󂭂��V�~�����[�^�["),
			WS_OVERLAPPED|
			WS_CAPTION|
			WS_SYSMENU|
			WS_CLIPCHILDREN,
			CW_USEDEFAULT,
			0,
			rect.right-rect.left,
			rect.bottom-rect.top,
			0,
			0,
			hInstance,
			0
		);
	ShowWindow(hWnd,SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while(GetMessage(&msg,0,0,0))
	{
		if(!IsDialogMessage(hWnd,&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	ExitProcess(msg.wParam);
}

void main(){}
