
#include <xc.h>
#include "main.h"
#include "utils.h"
#include "interrupts.h"

void Wait_ms(unsigned int time)
{
    g_generic_timer=(125*(unsigned long int)time)/32;
    while(g_generic_timer);
}