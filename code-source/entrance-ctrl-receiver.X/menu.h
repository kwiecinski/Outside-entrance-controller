/* 
 * 
 * 
 *
 * 
 */

#ifndef MENU_H
#define	MENU_H


void Menu_Init(MenuParamPonterStruct *menudef);
void Menu_Handler(MenuParamPonterStruct *menudef, KeyPointerStruct *keydef, 
                 TimeStruct *time);
void Send_7Seg_Text(char *text, unsigned char decimal_point);

#endif	/* MENU_H */

