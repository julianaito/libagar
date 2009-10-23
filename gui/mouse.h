/*	Public domain	*/

#ifndef _AGAR_GUI_MOUSE_H_
#define _AGAR_GUI_MOUSE_H_

typedef enum ag_mouse_button {
	AG_MOUSE_NONE		= 0x00,
	AG_MOUSE_LEFT		= 0x01,
	AG_MOUSE_MIDDLE		= 0x02,
	AG_MOUSE_RIGHT		= 0x03,
	AG_MOUSE_WHEELUP	= 0x04,
	AG_MOUSE_WHEELDOWN	= 0x05,
	AG_MOUSE_X1		= 0x06,
	AG_MOUSE_X2		= 0x07,
	AG_MOUSE_ANY		= 0xff
} AG_MouseButton;

#define AG_MOUSE_BUTTON(b)	(1<<((b)-1))
#define AG_MOUSE_LMASK		AG_MOUSE_BUTTON(1)
#define AG_MOUSE_MMASK		AG_MOUSE_BUTTON(2)
#define AG_MOUSE_RMASK		AG_MOUSE_BUTTON(3)

#endif /* _AGAR_GUI_MOUSE_H_ */