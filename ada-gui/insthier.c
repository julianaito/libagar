#include "ctxt.h"
#include "install.h"

struct install_item insthier[] = {

  {INST_MKDIR, 0, 0, ctxt_bindir, 0, 0, 0755},
  {INST_MKDIR, 0, 0, ctxt_incdir, 0, 0, 0755},
  {INST_MKDIR, 0, 0, ctxt_dlibdir, 0, 0, 0755},
  {INST_MKDIR, 0, 0, ctxt_slibdir, 0, 0, 0755},
  {INST_MKDIR, 0, 0, ctxt_repos, 0, 0, 0755},
  {INST_COPY, "ag_button.c", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "ag_draw.c", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "ag_event.c", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "ag_surface.c", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "ag_unit.c", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "ag_widget.c", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "ag_window.c", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-ada-conf.c", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-colors.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-colors.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-draw.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-draw.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-pixelformat.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-pixelformat.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-point.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-point.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-rect.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-rect.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-style.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-style.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-surface.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-surface.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-text.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-text.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-unit.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-unit.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-view.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-view.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-box.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-box.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-button.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-button.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-checkbox.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-checkbox.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-combo.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-combo.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-console.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-console.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-editable.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-editable.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-file_dialog.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-file_dialog.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-fixed.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-fixed.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-fixed_plotter.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-fixed_plotter.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-graph.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-graph.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-hbox.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-hbox.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-hsvpal.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-hsvpal.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-icon.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-icon.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-label.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-label.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-menu.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-menu.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-mpane.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-mpane.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-notebook.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-notebook.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-numerical.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-numerical.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-pane.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-pane.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-pixmap.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-pixmap.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-progress_bar.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-progress_bar.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-radio.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-radio.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-scrollbar.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-scrollbar.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-separator.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-separator.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-slider.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-slider.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-socket.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-socket.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-table.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-table.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-textbox.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-textbox.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-tlist.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-tlist.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-toolbar.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-toolbar.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-ucombo.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-ucombo.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-vbox.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-vbox.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-window.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-window.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui.ads", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui.ads", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-colors.adb", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-colors.adb", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-draw.adb", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-draw.adb", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-surface.adb", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-surface.adb", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-text.adb", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-text.adb", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-unit.adb", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-unit.adb", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-view.adb", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-view.adb", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-box.adb", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-box.adb", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-button.adb", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-button.adb", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-checkbox.adb", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-checkbox.adb", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-combo.adb", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-combo.adb", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-console.adb", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-console.adb", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-editable.adb", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-editable.adb", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-file_dialog.adb", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-file_dialog.adb", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget.adb", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget.adb", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-window.adb", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-window.adb", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui.adb", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui.adb", 0, ctxt_incdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-colors.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-colors.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-draw.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-draw.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-pixelformat.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-pixelformat.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-point.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-point.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-rect.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-rect.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-style.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-style.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-surface.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-surface.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-text.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-text.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-unit.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-unit.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-view.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-view.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-box.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-box.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-button.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-button.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-checkbox.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-checkbox.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-combo.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-combo.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-console.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-console.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-editable.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-editable.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-file_dialog.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-file_dialog.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-fixed.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-fixed.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-fixed_plotter.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-fixed_plotter.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-graph.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-graph.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-hbox.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-hbox.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-hsvpal.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-hsvpal.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-icon.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-icon.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-label.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-label.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-menu.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-menu.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-mpane.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-mpane.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-notebook.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-notebook.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-numerical.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-numerical.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-pane.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-pane.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-pixmap.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-pixmap.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-progress_bar.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-progress_bar.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-radio.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-radio.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-scrollbar.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-scrollbar.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-separator.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-separator.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-slider.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-slider.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-socket.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-socket.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-table.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-table.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-textbox.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-textbox.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-tlist.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-tlist.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-toolbar.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-toolbar.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-ucombo.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-ucombo.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget-vbox.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget-vbox.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-widget.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-widget.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-window.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-window.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui.ali", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui.ali", 0, ctxt_incdir, 0, 0, 0444},
  {INST_COPY, "agar-gui-ada.sld", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-ada.a", "libagar-gui-ada.a", ctxt_slibdir, 0, 0, 0644},
  {INST_COPY, "agar-gui-ada-conf.ld", 0, ctxt_repos, 0, 0, 0644},
  {INST_COPY, "agar-gui-ada-conf", 0, ctxt_bindir, 0, 0, 0755},
};
unsigned long insthier_len = sizeof(insthier) / sizeof(struct install_item);
