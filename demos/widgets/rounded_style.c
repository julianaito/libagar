/*	Public domain	*/
/*
 * Custom theme example. The default theme is implemented in
 * gui/style_default.c. You will probably want to use that file
 * as a template.
 *
 * See AG_Style(3) for a list of all style functions.
 */

#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/primitive.h>
#include <agar/dev.h>

#include "rounded_style.h"

#define WIDTH(wd)  AGWIDGET(wd)->w
#define HEIGHT(wd) AGWIDGET(wd)->h

static void
TitlebarBackground(void *tbar, int isPressed, int windowIsFocused)
{
	AG_DrawBoxRoundedTop(tbar,
	    AG_RECT(0, 0, WIDTH(tbar), HEIGHT(tbar)),
	    isPressed ? -1 : 1,
	    8,
	    windowIsFocused ? AG_COLOR(TITLEBAR_FOCUSED_COLOR) :
	                      AG_COLOR(TITLEBAR_UNFOCUSED_COLOR));
}

static void
ButtonBackground(void *btn, int isPressed)
{
	AG_DrawBoxRounded(btn,
	    AG_RECT(0, 0, WIDTH(btn), HEIGHT(btn)),
	    isPressed ? -1 : 1,
	    6,
	    AG_COLOR(BUTTON_COLOR));
}

static void
CheckboxButton(void *cbox, int state)
{
	AG_DrawBoxRounded(cbox,
	    AG_RECT(0, 0, HEIGHT(cbox), HEIGHT(cbox)),
	    state ? -1 : 1,
	    6,
	    AG_COLOR(CHECKBOX_COLOR));
}

static void
TextboxBackground(void *tbox, AG_Rect r, int isCombo)
{
	AG_DrawBoxRounded(tbox, r, isCombo?1:-1, 6,
	    AG_COLOR(TEXTBOX_COLOR));
}

/*
 * Note: The layout of AG_Style may change, so we override individual members
 * instead of using a static initializer.
 */
AG_Style myRoundedStyle;

void
InitMyRoundedStyle(AG_Style *s)
{
	*s = agStyleDefault;			/* Inherit from default */
	s->name = "rounded";
	s->version.maj = 1;
	s->version.min = 0;
	s->init = NULL;
	s->destroy = NULL;
	s->TitlebarBackground = TitlebarBackground;
	s->ButtonBackground = ButtonBackground;
	s->CheckboxButton = CheckboxButton;
	s->TextboxBackground = TextboxBackground;
}