/*	Public domain	*/

#ifndef _AGAR_WIDGET_NOTEBOOK_H_
#define _AGAR_WIDGET_NOTEBOOK_H_

#ifdef _AGAR_INTERNAL
#include <gui/widget.h>
#include <gui/box.h>
#include <gui/text.h>
#else
#include <agar/gui/widget.h>
#include <agar/gui/box.h>
#include <agar/gui/text.h>
#endif

#include "begin_code.h"

enum ag_notebook_tab_alignment {
	AG_NOTEBOOK_TABS_TOP,
	AG_NOTEBOOK_TABS_BOTTOM,
	AG_NOTEBOOK_TABS_LEFT,
	AG_NOTEBOOK_TABS_RIGHT
};

typedef struct ag_notebook_tab {
	struct ag_box box;
	int label;
	TAILQ_ENTRY(ag_notebook_tab) tabs;
} AG_NotebookTab;

typedef struct ag_notebook {
	struct ag_widget wid;
	enum ag_notebook_tab_alignment tab_align;
	int flags;
#define AG_NOTEBOOK_HFILL 	0x01	/* Expand to fill available width */
#define AG_NOTEBOOK_VFILL 	0x02	/* Expand to fill available height */
#define AG_NOTEBOOK_HIDE_TABS	0x04	/* Don't display the tabs. */
#define AG_NOTEBOOK_EXPAND	(AG_NOTEBOOK_HFILL|AG_NOTEBOOK_VFILL)
	AG_Mutex lock;
	int bar_w, bar_h;		/* Dimensions of tab button bar */
	int cont_w, cont_h;		/* Dimensions of largest container */
	int spacing, padding;		/* Spacing for tabs */
	AG_Font *tabFont;		/* Font for tab labels */
	struct ag_notebook_tab *sel_tab;
	TAILQ_HEAD(,ag_notebook_tab) tabs;
} AG_Notebook;

__BEGIN_DECLS
extern const AG_WidgetClass agNotebookClass;
extern const AG_WidgetClass agNotebookTabClass;

AG_Notebook *AG_NotebookNew(void *, Uint);
void AG_NotebookSetPadding(AG_Notebook *, int);
void AG_NotebookSetSpacing(AG_Notebook *, int);
void AG_NotebookSetTabAlignment(AG_Notebook *, enum ag_notebook_tab_alignment);
void AG_NotebookSetTabFont(AG_Notebook *, AG_Font *);
void AG_NotebookSetTabVisibility(AG_Notebook *, int);

AG_NotebookTab *AG_NotebookAddTab(AG_Notebook *, const char *,
		                  enum ag_box_type);

#define AG_NotebookAddTabVert(nb,lbl) \
	AG_NotebookAddTab((nb),(lbl),AG_BOX_VERT)
#define AG_NotebookAddTabHoriz(nb,lbl) \
	AG_NotebookAddTab((nb),(lbl),AG_BOX_HORIZ)

void AG_NotebookDelTab(AG_Notebook *, AG_NotebookTab *);
void AG_NotebookSelectTab(AG_Notebook *, AG_NotebookTab *);
__END_DECLS

#include "close_code.h"
#endif /* _AGAR_WIDGET_NOTEBOOK_H_ */
