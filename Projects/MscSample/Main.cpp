extern "C" {
#include "pico/stdlib.h"
#include "bsp/board.h"
#include "tusb.h"
}

#include <cstdio>

int main()
{
    stdio_init_all();
    board_init();
    tusb_init();

    printf("MscSample: start\n");

    while (true)
    {
        tud_task();
    }
}
