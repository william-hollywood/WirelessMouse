#include "WirelessMouseDesktopV2.h"

#include <stdio.h>
#include <windows.h>

#include "MouseControl.h"
#include "icons.h"

#define THIS_CLASSNAME "Tray Icon Window"
#define ID_ABOUT 2000
#define ID_EXIT 2001

// #define DEBUG

#ifndef DEBUG
#define printf(...)
#endif

#define BUFLEN 512 // Max length of buffer
#define PORT 8192  // The port on which to listen for incoming data

HANDLE udpThread;

static BOOL g_bModalState = FALSE; // Is messagebox shown

//===================================================================================
// ShowPopupMenu
//===================================================================================
BOOL ShowPopupMenu(HWND hWnd, POINT *curpos, int wDefaultItem) {
	(void)wDefaultItem;
	// ADD MENU
	// ITEMS.------------------------------------------------------------------
	HMENU hPop = CreatePopupMenu();
	if (g_bModalState) {
		return FALSE;
	}
	InsertMenu(hPop, 0, MF_BYPOSITION | MF_STRING, ID_CMD, "Change Password");
	InsertMenu(hPop, 1, MF_BYPOSITION | MF_STRING, ID_EXIT, "Exit");

	// CAN DO WITHOUT
	// STUFF.------------------------------------------------------------
	SetMenuDefaultItem(hPop, ID_ABOUT, FALSE);
	SetFocus(hWnd);
	SendMessage(hWnd, WM_INITMENUPOPUP, (WPARAM)hPop, 0);

	// SHOW POPUP
	// MENU.----------------------------------------------------------------- GET
	// CURSOR POSITION TO CREATE POPUP THERE.-------------------------------------
	POINT pt;
	if (!curpos) {
		GetCursorPos(&pt);
		curpos = &pt;
	}

	// DISPLAT MENU AND WAIT FOR
	// SELECTION.-----------------------------------------
	WORD cmd = TrackPopupMenu(hPop, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, curpos->x,
							  curpos->y, 0, hWnd, NULL);

	// SEND MESSAGE MAPPED TO SELECTED
	// ITEM.----------------------------------------
	SendMessage(hWnd, WM_COMMAND, cmd, 0);

	DestroyMenu(hPop);

	return 0;
}

void PrintError(DWORD dwError) {
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
				  dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("Printing error: %s\n", (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
//===================================================================================
// RemoveTrayIcon
//===================================================================================
void RemoveTrayIcon(HWND hWnd, UINT uID) {
	NOTIFYICONDATA nid;
	nid.hWnd = hWnd;
	nid.uID = uID;

	Shell_NotifyIcon(NIM_DELETE, &nid);
}

//===================================================================================
// AddTrayIcon
//===================================================================================
void AddTrayIcon(HWND hWnd, UINT uID, UINT uCallbackMsg, UINT uIcon) {
	(void)uIcon;
	// CREATE SYSTEN TRAY
	// ICON.---------------------------------------------------------------------
	NOTIFYICONDATA nid;
	nid.hWnd = hWnd;
	nid.uID = uID;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = uCallbackMsg;
	int offset = LookupIconIdFromDirectoryEx((unsigned char *)Mouse, TRUE, 0, 0, LR_DEFAULTCOLOR);
	nid.hIcon = CreateIconFromResourceEx((unsigned char *)Mouse + offset, Mouse_len - offset, TRUE, 0x00030000, 0, 0,
										 LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

	strcpy_s(nid.szTip, 128, "Wireless Mouse Desktop App");

	// SEND MESSAGE TO SYSTEM TRAY TO ADD
	// ICON.--------------------------------------------
	Shell_NotifyIcon(NIM_ADD, &nid);
}

//===================================================================================
// WndProc
//===================================================================================
static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CREATE:
		AddTrayIcon(hWnd, 1, WM_APP, 0);
		return 0;

	case WM_CLOSE:
		RemoveTrayIcon(hWnd, 1);
		PostQuitMessage(0);								  // Sends WM_QUIT message
		return DefWindowProc(hWnd, uMsg, wParam, lParam); // Calls WM_DESTROY

	case WM_COMMAND:
		if (g_bModalState) {
			return 1;
		}
		switch (LOWORD(wParam)) {
		case ID_CMD:
			g_bModalState = TRUE;
			MessageBox(hWnd, TEXT("Change Password"), TEXT("Default"), MB_ICONINFORMATION | MB_OK);
			g_bModalState = FALSE;
			return 0;

		case ID_EXIT:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			return 0;
		}
		return 0;

	case WM_APP:
		switch (lParam) {
		case WM_LBUTTONDBLCLK:
			MessageBox(hWnd, TEXT("Hi!"), TEXT("Title"), MB_ICONINFORMATION | MB_OK);
			return 0;

		case WM_RBUTTONUP:
			SetForegroundWindow(hWnd);
			ShowPopupMenu(hWnd, NULL, -1);
			PostMessage(hWnd, WM_APP + 1, 0, 0);
			return 0;
		}
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//===================================================================================
// WinMain
//===================================================================================
int Window_Create(HINSTANCE hInst, HINSTANCE prev, LPSTR cmdline, int show) {
	(void)prev;
	(void)cmdline;
	(void)show;
	// CHECK IF PREVIOUS ISTANCE IS
	// RUNNING.-----------------------------------------------------
	HWND hPrev = NULL;
	if ((hPrev = FindWindow(THIS_CLASSNAME, TEXT("Title")))) {
		MessageBox(NULL, TEXT("Previous instance alredy running!"), TEXT("Warning"), MB_OK);
		return 0;
	}

	// REGISTER
	// WINDOW.--------------------------------------------------------------------------
	WNDCLASSEX wclx;
	memset(&wclx, 0, sizeof(wclx));

	wclx.cbSize = sizeof(wclx);
	wclx.style = WS_EX_TOOLWINDOW;
	wclx.lpfnWndProc = &WndProc;
	wclx.cbClsExtra = 0;
	wclx.cbWndExtra = 0;
	wclx.hInstance = hInst;
	// wclx.hIcon        = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_TRAYICON ) );
	// wclx.hIconSm      = LoadSmallIcon( hInstance, IDI_TRAYICON );
	wclx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wclx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

	wclx.lpszMenuName = NULL;
	wclx.lpszClassName = THIS_CLASSNAME;

	RegisterClassEx(&wclx);

	// CREATE
	// WINDOW.----------------------------------------------------------------------------
	HWND hWnd = CreateWindowEx(0, THIS_CLASSNAME, TEXT(""), WS_DISABLED, 100, 100, 250, 150, NULL, NULL, hInst, NULL);
	if (!hWnd) {
		MessageBox(NULL, "Can't create window!", TEXT("Warning!"), MB_ICONERROR | MB_OK | MB_TOPMOST);
		return 1;
	}

	// MONITOR MESSAGE
	// QUEUE.--------------------------------------------------------------------
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// DESTROY
	// WINDOW.---------------------------------------------------------------------------
	UnregisterClass(THIS_CLASSNAME, hInst);

	return (int)msg.wParam;
}

int udp_thread_func(LPVOID lpParam) {
	(void)lpParam;
	SOCKET s;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	char buf[BUFLEN];
	WSADATA wsa;

	slen = sizeof(si_other);

	// Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	// Create a socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");

	// Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	// Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Bind done");

	// keep listening for data
	while (1) {
		printf("Waiting for data...\n");

		// clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);

		// try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_other, &slen)) == SOCKET_ERROR) {
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		// print details of the client/peer and the data received
		printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		if (recv_len == sizeof(data_t)) {
			HandleUDP((data_t *)buf);
		} else {
			printf("Received packet of invalid size: %d expected %llu\n", recv_len, sizeof(data_t));
		}
	}

	closesocket(s);
	WSACleanup();

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	(void)hInstance;
	(void)hPrevInstance;
	(void)lpCmdLine;
	(void)nCmdShow;
	udpThread = CreateThread(NULL, 0, ((LPTHREAD_START_ROUTINE)&udp_thread_func), NULL, 0, NULL);
	if (!udpThread) {
		return -1;
	}
	return Window_Create(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOW);
}
