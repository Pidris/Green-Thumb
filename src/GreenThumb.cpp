#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <resources/resource.h>
#include <standard/UI.h>
#include <MOD.h>
#include <fstream>

HINSTANCE s_hInstance;
HWND mainWindow;
HMENU menuBar;
HMENU ctxMenu;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	AllocConsole();
	FILE* pCout;
	freopen_s(&pCout, "conout$", "w", stdout);

	s_hInstance = hInstance;

	const wchar_t CLASS_NAME[] = L"win";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	mainWindow = CreateWindowEx(
		0, CLASS_NAME,
		L".MOD Utils",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
		NULL, NULL, hInstance, NULL
	);
	
	menuBar = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU_BAR));
	SetMenu(mainWindow, menuBar);
	ctxMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_CONTEXT_MENU));

	HWND bButton = CreateWindow(L"BUTTON", L"Clicker", UI::BaseStyle, 10, 10, UI::Button.Width, UI::Button.Height, mainWindow, NULL, hInstance, NULL);
	
	if (mainWindow == NULL){
		return 0;
	}

	ShowWindow(mainWindow, nCmdShow);

	MOD bulbaby;
	if (!bulbaby.ReadFile("chappy.mod"))
		MessageBox(mainWindow, L"Failed to open model", L"Error", MB_OK | MB_ICONERROR);

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

BOOL CALLBACK InformationDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
				case IDCANCEL:
					EndDialog(hwnd, wParam);
					return TRUE;
			}
	}
	return FALSE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg){
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_PAINT:{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			// All painting occurs here, between BeginPaint and EndPaint.

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

			EndPaint(hwnd, &ps);
			return 0;
		}

		case WM_COMMAND: {
			if(wParam == ID_FILE_EXIT) PostQuitMessage(0);
			if(wParam == ID_HELP_ABOUT) DialogBox(s_hInstance, MAKEINTRESOURCE(IDD_HELP_ABOUT), mainWindow, (DLGPROC)InformationDialogProc);
			if (wParam == ID_RENDERING_WIREFRAME) MessageBox(mainWindow, L"Wireframe rendering is not yet implemented", L"oops", MB_OK | MB_ICONERROR);
			return 0;
		}

		case WM_RBUTTONUP: {
			POINT pos;
			GetCursorPos(&pos);
			TrackPopupMenu(GetSubMenu(ctxMenu, 0), TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, 0, hwnd, NULL);
			return 0;
		}

		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}