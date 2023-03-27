// Built with Visual Studio
// Made by 플래그모
// GitHub: PragmoB

#include <iostream>
#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <conio.h>
#include <tlhelp32.h>
#include <mmsystem.h>
#include <digitalv.h>

#include <vector>

#include "framework.h"
#include "Sans.h"

#pragma comment(lib, "winmm.lib")

using namespace std;
HWND h_notepad, h_edit;

MCI_OPEN_PARMS mciOpen;
MCI_PLAY_PARMS mciPlay;

int IDidx;
const int IDlen = 6;
int sansID[IDlen];

void ResetNotepad();
DWORD FindProcessID(LPCTSTR szProcessName); // 프로세스ID를 구하는 함수
void write(const wchar_t* string, int delay); // 대본을 쓰는 함수

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	wchar_t sans[] = L"sans0.wav";
	mciOpen.lpstrElementName = sans;
	mciOpen.lpstrDeviceType = L"waveaudio";

	for (int i = 0; i < IDlen; i++)
	{
		((char*)sans)[8] = i + 0x30;
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mciOpen);
		sansID[i] = mciOpen.wDeviceID;
	}

	system("start notepad");
	Sleep(500);
	ResetNotepad(); // 메모장 핸들 초기화

	// 여기에 대본 작성
	write(L"░░░░░▄▄▀▀▀▀▀▀▀▀▀▄▄░░░░░\n", 0);
	write(L"░░░░█░░░░░░░░░░░░░█░░░░\n", 0);
	write(L"░░░█░░▄▄▄░░░░░▄▄▄░░█░░░\n", 0);
	write(L"░░░█░░███░░▄░░███░░█░░░\n", 0);
	write(L"░░░▄█░▄░░░▀▀▀░░░▄░█▄░░░\n", 0);
	write(L"░░░█░░▀█▀█▀█▀█▀█▀░░█░░░\n", 0);
	write(L"░░░▄██▄▄▀▀▀▀▀▀▀▄▄██▄░░░\n", 0);
	write(L"░▄█▒█▀▀█▀▀▀█▀▀▀█▀▀█▒█▄░\n", 0);
	write(L"▄▀▒▄▄▀▄▄█▀▀▄▀▀█▄▄▀▄▄▒▀▄\n", 0);
	write(L"█▒▒▒▒▀▄▒█▄░░░▄█▒▄▀▒▒▒▒█\n", 0);
	write(L"░▀▄▄▒█▒▒█▄▄▄▄▄█▒▒█▒▄▄▀░\n", 0);
	write(L"░░░▀██▄▄███████▄▄██▀░░░\n", 0);
	write(L"░░░████████▀████████░░░\n", 0);
	write(L"░░▄▄█▀▀▀▀█░░░█▀▀▀▀█▄▄░░\n", 0);
	write(L"░░▀▄▄▄▄▄▀▀░░░▀▀▄▄▄▄▄▀░░\n", 0);

	write(L"..................................\n", 120);
	Sleep(250);
	write(L"안녕? 나는 샌즈\n", 100);
	write(L"너의 컴퓨터 안에 들어오는데 성공했어\n", 100);
	Sleep(400);
	write(L"이제부터 이 컴을 터뜨려볼거야!\n", 100);
	Sleep(600);
	write(L"뭐? 나가달라고?\n", 100);
	Sleep(1000);
	write(L"그럼 날 한번 죽여보던가\n", 150);
	Sleep(200);
	write(L"하하하하하하하하하하 하하하하\n", 70);

	mciOpen.lpstrElementName = L"Meg.mp3";
	mciOpen.lpstrDeviceType = L"mpegvideo";
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (ULONGLONG)(LPVOID)&mciOpen);
	mciSendCommand(mciOpen.wDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_DGV_PLAY_REPEAT, (ULONGLONG)(LPVOID)&mciPlay);

	// 메모장 후킹 준비
	HINSTANCE inst = LoadLibrary(L"MouseHook.dll");
	if (inst == NULL)
	{
		MessageBox(NULL, L"DLL 로드 실패", L"에러", NULL);
		exit(1);
	}
	FARPROC UninstallHook;
	BOOL(WINAPI *InstallHook)(HWND);
	//BOOL(WINAPI *UninstallHook)();
	InstallHook = (BOOL(WINAPI*)(HWND))GetProcAddress(inst, "InstallHook");
	UninstallHook = GetProcAddress(inst, "UninstallHook");
	if (InstallHook == NULL || UninstallHook == NULL)
	{
		MessageBox(NULL, L"프로시저 주소 없음", L"에러", NULL);
		exit(1);
	}
	if (!InstallHook(h_notepad))
	{
		char buffer[100] = "";
		MessageBox(NULL, L"후킹 실패", L"후킹 실패", MB_OK);
	}
	while (TRUE)
	{
		if (GetForegroundWindow() == h_notepad) // 샌즈가 선택되었으면
		{
			HWND tmp_notepad = h_notepad, tmp_edit = h_edit;

			STARTUPINFOA si = { sizeof(si) };
			PROCESS_INFORMATION pi;
			char notepad_name[] = "Notepad";
			CreateProcessA(NULL, notepad_name, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

			Sleep(60);
			h_notepad = NULL;
			DWORD dwProcessId;

			// Notepad 클래스의 윈도우를 탐색하며 생성한 메모장의 프로세스 ID와 같은 것을 찾아냄
			do
			{
				h_notepad = FindWindowEx(NULL, h_notepad, L"Notepad", NULL);

				GetWindowThreadProcessId(h_notepad, &dwProcessId);
				if (pi.dwProcessId == dwProcessId)
					break;
			} while (h_notepad != NULL);

			do
			{
				h_edit = FindWindowEx(h_notepad, NULL, L"Edit", NULL);
				if (!h_edit)
					h_edit = FindWindowEx(h_notepad, NULL, L"RichEditD2DPT", NULL);
				if (!h_edit)
				{
					// 윈도우 11에서 1번 업데이트 된 버전의 경우
					h_edit = FindWindowEx(h_notepad, NULL, L"NotepadTextBox", NULL);
					h_edit = FindWindowEx(h_edit, NULL, L"RichEditD2DPT", NULL);
				}
			} while (!h_edit);

			write(L"날 만지지마", 150);
			h_notepad = tmp_notepad;
			h_edit = tmp_edit;
		}
		if (FindProcessID(L"notepad.exe") == -1 && FindProcessID(L"Notepad.exe") == -1) // 메모장이 꺼졌으면
		{
			MessageBox(NULL, L"아 젠장", L"Sans.exe", MB_OK);
			MessageBox(NULL, L"하지만 만나서 반가웠어.....", L"Sans.exe", MB_OK);
			MessageBox(NULL, L"You win!", L"정보", MB_OK);
			MessageBox(NULL, L"https://www.youtube.com/channel/UCSOgYTzjF0lByRtgcYyVPLw", L"Made by 플래그모", NULL);
			UninstallHook();
			exit(1);
		}
		Sleep(250);
	}
	while (1)
		Sleep(10000);
}

void write(const wchar_t* string, int delay)
{
	size_t len = wcslen(string);
	for (int j = 0; j < len; j++)
	{
		wchar_t letter = string[j];
		if (letter != L' ' && delay != 0) // 공백은 샌즈소리 안냄, 딜레이가 0인경우는 그림이므로 안냄
		{
			// 사운드 채널을 돌아가면서 쓴다
			mciSendCommand(sansID[IDidx], MCI_SEEK, MCI_SEEK_TO_START, NULL);
			mciSendCommand(sansID[IDidx++], MCI_PLAY, MCI_NOTIFY, (ULONGLONG)(LPVOID)&mciPlay);
			if (IDidx >= IDlen) // 사운드 채널 처음부터 다시 사용
				IDidx = 0;

		}

		cout << "WM_IME_CHAR 메시지 보냄" << endl;

		/* RichEdit의 경우 줄바꿈은 wm_keydown으로만 가능 */
		if (letter == L'\n')
			PostMessageA(h_edit, WM_KEYDOWN, VK_RETURN, 0);
		else
			PostMessageW(h_edit, WM_IME_CHAR, letter, GCS_RESULTSTR);

		Sleep(delay); // delay세컨드만큼 대기
	}
}
void ResetNotepad()
{
	h_notepad = FindWindow(L"Notepad", NULL);
	while (!h_notepad) // 메모장이 켜질때까지 무한루프
	{
		cout << "메모장 핸들" << endl;
		Sleep(150);
		h_notepad = FindWindow(L"Notepad", NULL); // 메모장 윈도우의 핸들(접근권한)요청
	}

	// 메모장 안에서 키보드 입력을 받는 윈도우 핸들을 구함
	h_edit = FindWindowEx(h_notepad, NULL, L"Edit", NULL);
	if (!h_edit)
		h_edit = FindWindowEx(h_notepad, NULL, L"RichEditD2DPT", NULL);
	if (!h_edit)
	{
		// 윈도우 11에서 1번 업데이트 된 버전의 경우
		h_edit = FindWindowEx(h_notepad, NULL, L"NotepadTextBox", NULL);
		h_edit = FindWindowEx(h_edit, NULL, L"RichEditD2DPT", NULL);
	}

	while (!h_edit)
	{
		MessageBox(NULL, L"메모장 발견 실패", L"오류", MB_OK | MB_ICONERROR);
		h_edit = FindWindowEx(h_notepad, NULL, L"Edit", NULL);
		if (!h_edit)
			h_edit = FindWindowEx(h_notepad, NULL, L"RichEditD2DPT", NULL);
		if (!h_edit)
		{
			// 윈도우 11에서 1번 업데이트 된 버전의 경우
			h_edit = FindWindowEx(h_notepad, NULL, L"NotepadTextBox", NULL);
			h_edit = FindWindowEx(h_edit, NULL, L"RichEditD2DPT", NULL);
		}
	}
}
DWORD FindProcessID(LPCTSTR szProcessName)
{
	DWORD dwPID = -1;
	HANDLE hSnapShot = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 pe;

	pe.dwSize = sizeof(PROCESSENTRY32);
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

	Process32First(hSnapShot, &pe); // 전체 프로세스의 정보들을 받아옴
	do
	{
		if (!wcscmp(szProcessName, pe.szExeFile)) // 프로세스 이름 ex) notepad.exe 이 맞는지 비교
		{
			dwPID = pe.th32ProcessID;
			break;
		}
	} while (Process32Next(hSnapShot, &pe)); // 다음 포로세스 정보를 받아옴

	CloseHandle(hSnapShot);

	return dwPID;
}
// 하하하하하하하
