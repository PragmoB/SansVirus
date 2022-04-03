// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.
#include "pch.h"

HHOOK g_MHook = NULL;
HHOOK g_MWHooK = NULL;
HINSTANCE g_hInst = NULL;

HWND h_notepad, h_edit;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	g_hInst = hModule;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		h_notepad = FindWindow(L"Notepad", NULL);
		h_edit = FindWindowEx(h_notepad, NULL, L"Edit", NULL);
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

// 마우스 후킹 프로시저
LRESULT CALLBACK MouseHook(int code, WPARAM w, LPARAM l) {

	//if (code >= HC_ACTION)
	{
		if (w == WM_NCLBUTTONDOWN || w == WM_LBUTTONDOWN || w == WM_NCMOUSEMOVE || w == WM_MOUSEMOVE)
		{
			MessageBox(NULL, L"만지지마 이 하찮은 인간아", L"sans.exe", MB_OK);
			return NULL;
		}
	}
	return CallNextHookEx(g_MHook, code, w, l);
}

extern "C" __declspec(dllexport) BOOL WINAPI InstallHook(HWND notepad)
{
	int dwID = GetWindowThreadProcessId(notepad, NULL);

	g_MHook = SetWindowsHookEx(WH_MOUSE, MouseHook, g_hInst, dwID);
	if (g_MHook)
		return TRUE;

	return FALSE;
}
extern "C" __declspec(dllexport) BOOL WINAPI UninstallHook()
{
	UnhookWindowsHookEx(g_MHook);

	return TRUE;
}
