///////////////////////////////////////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef INPUT_HANDLER_H
#define	INPUT_HANDLER_H

void LineState(void);
void CheckGateCloseSwitch(void);

#define GATE_CLOSE_OUTPUT_HIGH      PORTBbits.RB3=1
#define GATE_CLOSE_OUTPUT_LOW      PORTBbits.RB3=0

#endif	/* INPUT_HANDLER_H */

