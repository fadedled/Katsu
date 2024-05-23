![](./res/images/klogo.png)

**Katsu** is a light cross-platform C library for 2D video game programming. It 
includes an API for 2D rendering, a simple input system, and sound output. 
Tile renderers of old school computers and video game consoles are the basis 
for Katsu's 2D rendering capabilities.


## Features

+ 4bpp 8x8 pixel tile format
+ 128 Color palettes, 16 colors each
+ 16 64x64 tilemaps
+ Affine transformations for tilemaps and sprite
+ Horizontal line effects
+ Window effects
+ Other old-school effects (Mosaic, palette manipulation, etc.)


## Build from source

Katsu is meant to be a static library since it's so small, to build Katsu from source do the following:

### Windows

Run the batch file ```build_win.bat```, this will generate the static library ```lib\katsu.lib```.

### Linux

Run the ```make``` command, this will generate the static library ```lib\libkatsu.a```.

## Installing Katsu for your project

Simply copy the static library to your projects ```lib``` folder, you must also copy the ```include\katsu``` to your projects ```include``` folder.


## Using and compiling your project

The following is a very basic example for creating a window and controlling the back color with the directional pad and the A button:

```c
#include <katsu/kt.h>

int main() {
    u32 r_comp = 0, g_comp = 0x80, b_comp = 0;
    /* Init Katsu */
	if (kt_Init()) {
		return 0;
	}
	/* Loop forever */
	while (1) {
		kt_Poll();  /* Poll joypads and window events */
        /* Get the held buttons of the first joypad */
        u32 btns = kt_JoyButtonHeld(0);
        /* Use the bits to modify the R and B components */
		r_comp += ((btns >> JOY_BIT_RIGHT) & 0x1) - ((btns >> JOY_BIT_LEFT) & 0x1);
		b_comp += ((btns >> JOY_BIT_DOWN) & 0x1) - ((btns >> JOY_BIT_UP) & 0x1);
        /* Swap G and B components of the back color if A is held */		
		if (btns & JOY_A) {
			kt_BackColor(r_comp, g_comp, b_comp);
		} else {
			kt_BackColor(r_comp, b_comp, g_comp);
		}
		kt_Draw();  /* Draw the image */
	}
	return 0;
}
```

With the following you can do a basic compilation of your project:

### Windows

You will need Visual Studios MSVC or MinGW installed, link with ```User32.lib```, ```Opengl32.lib``` and ```katsu.lib```. If you are using the console/terminal you should set your environment to use MSVC. Then compile with:

```
$> cl -Tc <your_source_code> -I".\include" -link User32.lib Opengl32.lib katsu.lib -OUT:mygame.exe
$> mygame.exe
```

### Linux

You will need the development packages for X11, OpenGL, and PulseAudio when compiling your project.

You can download them from your package manager as ```libx11-dev```, ```libgl1-mesa-dev``` 
and ```libpulse-dev``` for Debian-based distros or as ```libX11-devel```, ```mesa-libGL-devel``` 
and ```pulseaudio-libs-devel``` for RedHat-based distros. After they are installed, you can compile and run from the terminal with:

```
$> gcc <your_source_code> -o mygame -I./include -L./lib -lGL -lpthread -lpulse-simple -lX11 -ldl -lkatsu
$> ./mygame
```

## Keyboard keys

All Katsu applications use the same 17 keys:

- The four arrow keys and Z, X, A, S, Q, W, D, E, Enter, and RightShift
- Press Esc to exit form full screen


## Developer Manual

The Katsu Developer Manual will come at a future date.

## Assets

Katsu asks for particular formats; if you want to create assets that follow these 
formats, you must make your own tools (though a Katsu asset builder is planned). 
But there might exist some 4bpp tile editors out there.

## Donation
**The Katsu project is and will always be free!**

If you liked Katsu and you want to support its development with a donation, you can do so here:

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/donate?hosted_button_id=H9AEAYCDAYCB6)






