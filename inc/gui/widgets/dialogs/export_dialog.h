// SPDX-FileCopyrightText: © 2018-2022 Alexandros Theodotou <alex@zrythm.org>
// SPDX-License-Identifier: LicenseRef-ZrythmLicense

/**
 * \file
 *
 * Export dialog.
 */

#ifndef __GUI_WIDGETS_EXPORT_DIALOG_H__
#define __GUI_WIDGETS_EXPORT_DIALOG_H__

#include <adwaita.h>
#include <gtk/gtk.h>

#define EXPORT_DIALOG_WIDGET_TYPE \
  (export_dialog_widget_get_type ())
G_DECLARE_FINAL_TYPE (
  ExportDialogWidget,
  export_dialog_widget,
  Z,
  EXPORT_DIALOG_WIDGET,
  GtkDialog)

typedef struct _DigitalMeterWidget DigitalMeterWidget;

/**
 * @addtogroup widgets
 *
 * @{
 */

typedef enum ExportFilenamePattern
{
  EFP_APPEND_FORMAT,
  EFP_PREPEND_DATE_APPEND_FORMAT,
} ExportFilenamePattern;

/**
 * The export dialog.
 */
typedef struct _ExportDialogWidget
{
  GtkDialog parent_instance;

  AdwViewSwitcherTitle * title;
  AdwViewStack *         stack;

  /* audio */
  AdwEntryRow *        audio_title;
  AdwEntryRow *        audio_artist;
  AdwEntryRow *        audio_genre;
  AdwComboRow *        audio_format;
  AdwComboRow *        audio_bit_depth;
  AdwActionRow *       audio_dither;
  GtkSwitch *          audio_dither_switch;
  AdwComboRow *        audio_filename_pattern;
  AdwComboRow *        audio_mixdown_or_stems;
  AdwComboRow *        audio_time_range_combo;
  AdwActionRow *       audio_custom_tr_row;
  GtkBox *             audio_custom_tr_start_meter_box;
  DigitalMeterWidget * audio_custom_tr_start_meter;
  Position             audio_custom_start_pos;
  GtkBox *             audio_custom_tr_end_meter_box;
  DigitalMeterWidget * audio_custom_tr_end_meter;
  Position             audio_custom_end_pos;
  GtkTreeView *        audio_tracks_treeview;
  GtkLabel *           audio_output_label;

  /* MIDI */
  AdwEntryRow *        midi_title;
  AdwEntryRow *        midi_artist;
  AdwEntryRow *        midi_genre;
  AdwComboRow *        midi_format;
  GtkSwitch *          midi_export_lanes_as_tracks_switch;
  AdwComboRow *        midi_filename_pattern;
  AdwComboRow *        midi_mixdown_or_stems;
  AdwComboRow *        midi_time_range_combo;
  AdwActionRow *       midi_custom_tr_row;
  GtkBox *             midi_custom_tr_start_meter_box;
  DigitalMeterWidget * midi_custom_tr_start_meter;
  Position             midi_custom_start_pos;
  GtkBox *             midi_custom_tr_end_meter_box;
  DigitalMeterWidget * midi_custom_tr_end_meter;
  Position             midi_custom_end_pos;
  GtkTreeView *        midi_tracks_treeview;
  GtkLabel *           midi_output_label;

} ExportDialogWidget;

/**
 * Creates an export dialog widget and displays it.
 */
ExportDialogWidget *
export_dialog_widget_new (void);

/**
 * @}
 */

#endif
