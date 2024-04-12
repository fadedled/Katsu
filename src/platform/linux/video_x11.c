
#include <katsu/kt.h>
#include "../../video_common.h"
#include "../../joypad_common.h"
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <stdio.h>


struct KTVideoX11_t {
	Display *dpy;
	Window win;
	GLXContext glc;
	//Atoms
	Atom wmdelwin;
} kt_x11;

KeyCode xkey_mapping[16];

u32  __kt_VideoInit(void)
{
	//Open the X11 display
	kt_x11.dpy = XOpenDisplay(NULL);
	if (kt_x11.dpy == NULL) {
		return 1;
	}

	XVisualInfo vinfo;

	int def_screen = XDefaultScreen(kt_x11.dpy);
	int def_depth = XDefaultDepth(kt_x11.dpy, def_screen);
	if (!XMatchVisualInfo(kt_x11.dpy, def_screen, def_depth, TrueColor, &vinfo)) {
		return 1;
	}

	XSetWindowAttributes win_attr;
	win_attr.colormap = XDefaultColormap(kt_x11.dpy, vinfo.screen);
	win_attr.background_pixel = None;
	win_attr.border_pixel = 0;
	win_attr.event_mask = KeyPressMask | KeyReleaseMask | StructureNotifyMask;
	//XXX: ResizeRedirectMask? ExposureMask?

	kt_x11.win = XCreateWindow(kt_x11.dpy, XDefaultRootWindow(kt_x11.dpy),
							   0, 0, VIDEO_MAX_WIDTH, VIDEO_MAX_HEIGHT,
							   0, vinfo.depth, InputOutput, vinfo.visual,
							   CWColormap | CWEventMask | CWBackPixel | CWBorderPixel, &win_attr);


	kt_x11.wmdelwin = XInternAtom(kt_x11.dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(kt_x11.dpy, kt_x11.win, &kt_x11.wmdelwin, 1);

	if (!kt_x11.win) {
		return 2;
	}
	//Create the OpenGL context
	kt_x11.glc = glXCreateContext(kt_x11.dpy, &vinfo, NULL, True);
	glXMakeCurrent(kt_x11.dpy, kt_x11.win, kt_x11.glc);

	//Generate the keyboard mappings
	joy_state[0].active = 1;
	xkey_mapping[0] = XKeysymToKeycode(kt_x11.dpy, XK_Left);
	xkey_mapping[1] = XKeysymToKeycode(kt_x11.dpy, XK_Right);
	xkey_mapping[2] = XKeysymToKeycode(kt_x11.dpy, XK_Up);
	xkey_mapping[3] = XKeysymToKeycode(kt_x11.dpy, XK_Down);
	xkey_mapping[4] = XKeysymToKeycode(kt_x11.dpy, XK_Z);
	xkey_mapping[5] = XKeysymToKeycode(kt_x11.dpy, XK_X);
	xkey_mapping[6] = XKeysymToKeycode(kt_x11.dpy, XK_A);
	xkey_mapping[7] = XKeysymToKeycode(kt_x11.dpy, XK_S);
	xkey_mapping[8] = XKeysymToKeycode(kt_x11.dpy, XK_Q);
	xkey_mapping[9] = XKeysymToKeycode(kt_x11.dpy, XK_W);
	xkey_mapping[10] = XKeysymToKeycode(kt_x11.dpy, XK_E);
	xkey_mapping[11] = XKeysymToKeycode(kt_x11.dpy, XK_D);
	xkey_mapping[12] = XKeysymToKeycode(kt_x11.dpy, XK_Return);
	xkey_mapping[13] = XKeysymToKeycode(kt_x11.dpy, XK_Shift_R);


	XMapWindow(kt_x11.dpy, kt_x11.win);
	XFlush(kt_x11.dpy);
	return KT_OK;
}


void __kt_VideoExit(void)
{
	XUnmapWindow(kt_x11.dpy, kt_x11.win);

	//Destroy the OpenGL context
	glXMakeCurrent(kt_x11.dpy, None, NULL);
	glXDestroyContext(kt_x11.dpy, kt_x11.glc);

	XDestroyWindow(kt_x11.dpy, kt_x11.win);
	XFlush(kt_x11.dpy);
	XCloseDisplay(kt_x11.dpy);
}


void __kt_VideoAttrSet(u32 attr, u32 val)
{
	//XStoreName(kt_x11.dpy, kt_x11.win, "Window Name");
}



void __kt_VideoPoll(void)
{
	XEvent xev;

	//Check that the user requested a window close
	while (XCheckTypedWindowEvent(kt_x11.dpy, kt_x11.win, ClientMessage, &xev)) {
		XClientMessageEvent cm_ev = xev.xclient;
		if (cm_ev.data.l[0] == kt_x11.wmdelwin) {
			kt_Exit(0);
		}
	}

	//Check the rest of the events
	while (XCheckWindowEvent(kt_x11.dpy, kt_x11.win,
							 KeyPressMask| KeyReleaseMask | StructureNotifyMask, &xev)) {
		switch(xev.type) {
			case KeyPress: {
				XKeyEvent key_ev = xev.xkey;
				for (u32 k = 0; k < 14; ++k) {
					if (xkey_mapping[k] == key_ev.keycode) {
						joy_state[0].btn |= (1 << k);
					}
				}

			} break;
			case KeyRelease:{
				XKeyEvent key_ev = xev.xkey;
				for (u32 k = 0; k < 14; ++k) {
					if (xkey_mapping[k] == key_ev.keycode) {
						joy_state[0].btn &= ~(1 << k);
					}
				}
			} break;
			case ConfigureNotify:{
				XConfigureEvent conf_ev = xev.xconfigure;
			} break;
		}
	}
}


void __kt_VideoSwapBuffers(void)
{
	glXSwapBuffers(kt_x11.dpy, kt_x11.win);
}
