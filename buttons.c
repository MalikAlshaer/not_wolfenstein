#include "config.h"
#include <stdio.h>

void ResumePause(void) {
    // not the best solution
    // just to keep button functions together
    PauseGame();
}

void VolumeOnOff(void) {
    volume_paused = (volume_paused + 1) % 2;
    printf("\%d\n", volume_paused);
}
