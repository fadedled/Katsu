
#include <katsu/kt.h>
#include "../../video_common.h"
#include "../../joypad_common.h"
#include <windows.h>
#include <strsafe.h>
//#include <shellscalingapi.h>
#include "../../opengl/gl.h"
#include <wingdi.h>
#include <stdio.h>
#include <stdlib.h>

/* WGL context creation defines */
#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB        0x00000001

typedef HGLRC (WINAPI * WGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);
typedef const char* (WINAPI * WGLGETEXTENSIONSSTRINGARBPROC)(HDC);
typedef BOOL (WINAPI * WGLSWAPINTERVALEXTPROC)(int);


struct KTVideoX11_t {
	HWND hwin;
	HDC hdc;
	HGLRC glc;

} kt_win;

u32 wkey_mapping[16];

void __kt_WinResize(HWND hWnd, int nWidth, int nHeight)
{
	RECT cli_rect, win_rect;
	GetClientRect(hWnd, &cli_rect);
	GetWindowRect(hWnd, &win_rect);
	u32 brd_x = (win_rect.right - win_rect.left) - cli_rect.right;
	u32 brd_y = (win_rect.bottom - win_rect.top) - cli_rect.bottom;
	MoveWindow(hWnd, win_rect.left, win_rect.top, nWidth + brd_x, nHeight + brd_y, TRUE);
}


static LRESULT CALLBACK __kt_WinCallback(HWND win, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg) {
		case WM_SIZE: {
			vstate.frame_w = LOWORD(lparam);
			vstate.frame_h = HIWORD(lparam);
		} break;
		case WM_WINDOWPOSCHANGING: {
			RECT cli_rect, win_rect;
			GetClientRect(kt_win.hwin, &cli_rect);
			vstate.frame_w = cli_rect.right - cli_rect.left;
			vstate.frame_h = cli_rect.bottom - cli_rect.top;
			__kt_UpdateOutputDims();
		} break;
		case WM_DESTROY: {
		} break;
		case WM_CLOSE: {
			kt_Exit(0);
		} break;
		case WM_ACTIVATEAPP: {
		} break;
		case WM_PAINT: {
		} break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN: {
			//Obtain scan code and see if there is a mapping
			u32 keyFlags = HIWORD(lparam);
			u32 keycode = LOBYTE(keyFlags);
			if ((keyFlags & KF_EXTENDED) == KF_EXTENDED) {
				keycode |= 0xE000;
			}
			for (u32 k = 0; k < 14; ++k) {
				if (wkey_mapping[k] == keycode) {
					joy_state[0].btn |= (1 << k);
				}
			}
		} break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			//Obtain scan code and see if there is a mapping
			u32 keyFlags = HIWORD(lparam);
			u32 keycode = LOBYTE(keyFlags);
			if ((keyFlags & KF_EXTENDED) == KF_EXTENDED) {
				keycode |= 0xE000;
			}
			for (u32 k = 0; k < 14; ++k) {
				if (wkey_mapping[k] == keycode) {
					joy_state[0].btn &= ~(1 << k);
				}
			}
		} break;
		default:
			return DefWindowProc(win, msg, wparam, lparam);
	}

	return 0;
}



u32  __kt_VideoInit(void)
{
	//XXX: No
	vstate.frame_w = KT_VIDEO_STD_WIDTH;
	vstate.frame_h = KT_VIDEO_STD_HEIGHT;

	/* Open a window */
	HGLRC glc_old;
	const LPCTSTR CLASS_NAME = TEXT("kt_WinClass");
	WNDCLASS wclass = {0};
	wclass.style = CS_OWNDC;
	wclass.lpfnWndProc = __kt_WinCallback;
	wclass.hInstance = GetModuleHandle(0);
	wclass.lpszClassName = CLASS_NAME;

	if (!RegisterClass(&wclass)) {
		return 1;
	}

	kt_win.hwin = CreateWindow(CLASS_NAME, "",
							   WS_OVERLAPPEDWINDOW | WS_VISIBLE,
							   CW_USEDEFAULT, CW_USEDEFAULT,
							   KT_VIDEO_STD_WIDTH, KT_VIDEO_STD_HEIGHT,
							   NULL, NULL, wclass.hInstance, NULL);
	if (!kt_win.hwin){
		return 1;
	}
	__kt_WinResize(kt_win.hwin, KT_VIDEO_STD_WIDTH, KT_VIDEO_STD_HEIGHT);
	kt_VideoFrameSet(KT_VIDEO_FRAME_2X);
	kt_win.hdc = GetDC(kt_win.hwin);
	//SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

	//Create OpenGL context
	PIXELFORMATDESCRIPTOR pfd = {0};
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW
				| PFD_SUPPORT_OPENGL
				| PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;
	SetPixelFormat(kt_win.hdc, ChoosePixelFormat(kt_win.hdc, &pfd), &pfd);
	if (!(glc_old = wglCreateContext(kt_win.hdc))) {
		return 1;
	}
	wglMakeCurrent(kt_win.hdc, glc_old);

	//Get OpenGL 4.3
	WGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
	WGLGETEXTENSIONSSTRINGARBPROC  wglGetExtensionsStringARB = NULL;
	wglCreateContextAttribsARB = (WGLCREATECONTEXTATTRIBSARBPROC)
				wglGetProcAddress("wglCreateContextAttribsARB");
	wglGetExtensionsStringARB = (WGLGETEXTENSIONSSTRINGARBPROC)
				wglGetProcAddress("wglGetExtensionsStringARB");

	const char* wglExts = wglGetExtensionsStringARB(kt_win.hdc);
	if (strstr(wglExts, "WGL_ARB_create_context") && wglCreateContextAttribsARB) {
		int context_attribs[7] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0};
		/* Create OpenGL context and make current */
		wglDeleteContext(glc_old);
		kt_win.glc = wglCreateContextAttribsARB(kt_win.hdc, 0, context_attribs);
		if (!kt_win.glc) {
			return 1;
		}
		wglMakeCurrent(kt_win.hdc, kt_win.glc);
	}
	else {
		return 1;
	}
	/* Set Vsync, it won't cry if we can't get it */
	if (strstr(wglExts, "WGL_EXT_swap_control ")) {
		WGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
		wglSwapIntervalEXT = (WGLSWAPINTERVALEXTPROC)
						wglGetProcAddress("wglSwapIntervalEXT");
		wglSwapIntervalEXT(1);
	}

	//Generate the keyboard mappings
	joy_state[0].active = 1;
	wkey_mapping[0]  = 0xE04B;  // Left
	wkey_mapping[1]  = 0xE04D;  // Right
	wkey_mapping[2]  = 0xE048;  // Up
	wkey_mapping[3]  = 0xE050;  // Down
	wkey_mapping[4]  = 0x002C;  // Z
	wkey_mapping[5]  = 0x002D;  // X
	wkey_mapping[6]  = 0x001E;  // A
	wkey_mapping[7]  = 0x001F;  // S
	wkey_mapping[8]  = 0x0010;  // Q
	wkey_mapping[9]  = 0x0011;  // W
	wkey_mapping[10] = 0x0012;  // E
	wkey_mapping[11] = 0x0020;  // D
	wkey_mapping[12] = 0x001C;  // Return
	wkey_mapping[13] = 0x0036;  // Shift_R

	kt_Reset();
	ShowWindow(kt_win.hwin, SW_SHOW);
	return KT_OK;
}


void __kt_VideoExit(void)
{
	//Destroy the OpenGL context
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(kt_win.glc);
	PostQuitMessage(0);
}


void __kt_VideoAttrSet(u32 attr, void *val)
{
	switch (attr) {
		case KT_VIDEO_ATTR_TITLE: {
			SetWindowTextA(kt_win.hwin, (const char *) val);
		} break;
		case KT_VIDEO_ATTR_FRAME: {
			u32 frame = *((u32*)val);
			u32 w = KT_VIDEO_STD_WIDTH * (frame + 1);
			u32 h = KT_VIDEO_STD_HEIGHT * (frame + 1);
			if (frame == KT_VIDEO_FRAME_FULLSCREEN) {

			}
			else {
				__kt_WinResize(kt_win.hwin, w, h);
			}
		} break;
	}
}



void __kt_VideoPoll(void)
{
	MSG msg = {0};
	//Check events
	GetMessage(&msg, NULL, 0, 0);
	TranslateMessage(&msg);
	DispatchMessage(&msg);
}


void __kt_VideoSwapBuffers(void)
{
	SwapBuffers(kt_win.hdc);
}
