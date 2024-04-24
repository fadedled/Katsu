
#include <katsu/kt.h>
#include "../../video_common.h"
#include "../../joypad_common.h"
#include <windows.h>
#include <wingdi.h>
#include <strsafe.h>
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
	//Atoms
	Atom wmdelwin;
} kt_win;

KeyCode xkey_mapping[16];

static LRESULT CALLBACK __kt_WinCallback(HWND win, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg) {
		case WM_SIZE: {
			vstate.frame_w = LOWORD(lparam);
			vstate.frame_h = HIWORD(lparam);
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
		case default:
			return DefWindowProc(win, msg, wparam, lparam);
	}
	return 0;
}

u32  __kt_VideoInit(void)
{
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
							   VIDEO_MAX_WIDTH, VIDEO_MAX_HEIGHT,
							   NULL, NULL, wclass.hInstance, NULL);
	if (!kt_win.hwin){
		return 1;
	}

	kt_win.hdc = GetDC(kt_win.hwin);


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
	SetPixelFormat(plWin.hdc, ChoosePixelFormat(plWin.hdc, &pfd), &pfd);
	if (!(glc_old = wglCreateContext(plWin.hdc))) {
		return PL_NO_GL_CONTEXT;
	}
	wglMakeCurrent(plWin.hdc, glc_old);

	//Get OpenGL 4.3
	WGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
	WGLGETEXTENSIONSSTRINGARBPROC  wglGetExtensionsStringARB = NULL;
	wglCreateContextAttribsARB = (WGLCREATECONTEXTATTRIBSARBPROC)
				wglGetProcAddress("wglCreateContextAttribsARB");
	wglGetExtensionsStringARB = (WGLGETEXTENSIONSSTRINGARBPROC)
				wglGetProcAddress("wglGetExtensionsStringARB");

	const char* wglExts = wglGetExtensionsStringARB(plWin.hdc);
	if (strstr(wglExts, "WGL_ARB_create_context") && wglCreateContextAttribsARB) {
		int context_attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0};
		/* Create OpenGL context and make current */
		wglDeleteContext(glc_old);
		plWin.glc = wglCreateContextAttribsARB(plWin.hdc, 0, context_attribs);
		if (!plWin.glc) {
			return 1;
		}
		wglMakeCurrent(plWin.hdc, plWin.glc);
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

	kt_Reset();
	showWindow(kt_win.hwin, SW_SHOW);
	return KT_OK;
}


void __kt_VideoExit(void)
{
	//Destroy the OpenGL context
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(plWin.glc);
	PostQuitMessage(0);
}


void __kt_VideoAttrSet(u32 attr, void *val)
{
	switch (attr) {
		case VIDEO_ATTR_TITLE: {
			SetWindowTextA(kt_win.hwin, (const char *) val);
		} break;
		case VIDEO_ATTR_FRAME: {
			//XXX: implement
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

	XEvent xev;
}


void __kt_VideoSwapBuffers(void)
{
	SwapBuffers(kt_win.hdc);
}
