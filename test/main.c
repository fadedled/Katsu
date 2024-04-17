

#include <katsu/kt.h>
#include <stdio.h>
#include <GL/gl.h>




int main() {
	if (kt_Init()) {
		return 0;
	}

	while (1) {
		kt_Poll();

		printf("%04x %04x %04x\n", kt_JoyButtonUp(0)
							, kt_JoyButtonDown(0)
							, kt_JoyButtonHeld(0));
		if (kt_JoyButtonHeld(0) & JOY_A) {
			kt_BackColorSet(0x505050);
		} else {
			kt_BackColorSet(0x202050);
		}

		kt_Draw();
	}

	return 0;
}
