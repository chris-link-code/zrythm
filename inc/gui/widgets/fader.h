// SPDX-FileCopyrightText: © 2018-2023 Alexandros Theodotou <alex@zrythm.org>
// SPDX-License-Identifier: LicenseRef-ZrythmLicense

/**
 * \file
 *
 * Fader widget.
 */

#ifndef __GUI_WIDGETS_FADER_H__
#define __GUI_WIDGETS_FADER_H__

#include <gtk/gtk.h>

#define FADER_WIDGET_TYPE (fader_widget_get_type ())
G_DECLARE_FINAL_TYPE (
  FaderWidget,
  fader_widget,
  Z,
  FADER_WIDGET,
  GtkWidget)

typedef struct Fader Fader;

/**
 * @addtogroup widgets
 * @{
 */

/**
 * A FaderWidget reflects a Fader object's state.
 */
typedef struct _FaderWidget
{
  GtkWidget        parent_instance;
  GtkGestureDrag * drag;
  Fader *          fader;
  double           last_x;
  double           last_y;
  bool             hover;
  bool             dragging;
  GdkRGBA          start_color;
  GdkRGBA          end_color;

  /** Value at start. */
  float amp_at_start;

  /** Layout for text. */
  PangoLayout * layout;

  /** Popover to be reused for context menus. */
  GtkPopoverMenu * popover_menu;
} FaderWidget;

/**
 * Creates a new Fader widget and binds it to the
 * given Fader.
 */
void
fader_widget_setup (
  FaderWidget * self,
  Fader *       fader,
  int           width,
  int           height);

/**
 * @}
 */

#endif
