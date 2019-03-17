/*
 * Copyright (C) 2018-2019 Alexandros Theodotou <alex at zrythm dot org>
 *
 * This file is part of Zrythm
 *
 * Zrythm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Zrythm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Zrythm.  If not, see <https://www.gnu.org/licenses/>.
 */

/** \file
 */

#include "audio/automatable.h"
#include "plugins/lv2_plugin.h"
#include "audio/automation_track.h"
#include "audio/automation_tracklist.h"
#include "audio/bus_track.h"
#include "audio/instrument_track.h"
#include "audio/track.h"
#include "audio/region.h"
#include "plugins/plugin.h"
#include "gui/widgets/arranger.h"
#include "gui/widgets/automation_lane.h"
#include "gui/widgets/automation_tracklist.h"
#include "gui/widgets/center_dock.h"
#include "gui/widgets/color_area.h"
#include "gui/widgets/instrument_track.h"
#include "gui/widgets/main_window.h"
#include "gui/widgets/track.h"
#include "gui/widgets/tracklist.h"
#include "utils/gtk.h"
#include "utils/resources.h"

#include <gtk/gtk.h>

G_DEFINE_TYPE (InstrumentTrackWidget,
               instrument_track_widget,
               TRACK_WIDGET_TYPE)
 void
instument_track_ui_toggle (GtkWidget * self, InstrumentTrackWidget * data)
{
  Channel * channel = GET_CHANNEL(data);
  TRACK_WIDGET_GET_PRIVATE(data);
  Plugin * plugin = channel->plugins[0];
  InstrumentTrack * it =
    (InstrumentTrack *) plugin->channel->track;
  if (!it->ui_active )
    {
      plugin_open_ui (plugin);
      it->ui_active = 1;
    }
  else
    {
      plugin_close_ui (plugin);
      it->ui_active = 0;
    }
  instrument_track_widget_refresh_buttons (data);

}
/**
 * Updates ui_active state if instrument window is closed
 *
 */
void
instrument_track_widget_on_plugin_delete_event (GtkWidget *window,
				    GdkEventKey *e,
				    gpointer data)
{
  if(data){
  TRACK_WIDGET_GET_PRIVATE(data);
  Channel * channel = GET_CHANNEL(data);
  Plugin * plugin = channel->plugins[0];
  InstrumentTrack * it = (InstrumentTrack *)tw_prv->track;

  if (it->ui_active == 1)
    {
      plugin_close_ui(plugin);
      it->ui_active = 0;
      instrument_track_widget_refresh_buttons (data);
    }
  }
}
/**
 * Creates a new track widget using the given track.
 *
 * 1 track has 1 track widget.
 * The track widget must always have at least 1 automation track in the automation
 * paned.
 */
InstrumentTrackWidget *
instrument_track_widget_new (Track * track)
{
  InstrumentTrackWidget * self = g_object_new (
                            INSTRUMENT_TRACK_WIDGET_TYPE,
                            NULL);

  TRACK_WIDGET_GET_PRIVATE (self);

  /* setup color */
  color_area_widget_set_color (tw_prv->color,
                               &track->color);

  /* setup automation tracklist */
  AutomationTracklist * automation_tracklist =
    track_get_automation_tracklist (track);
  automation_tracklist->widget =
    automation_tracklist_widget_new (automation_tracklist);
  gtk_paned_pack2 (GTK_PANED (tw_prv->paned),
                   GTK_WIDGET (automation_tracklist->widget),
                   Z_GTK_RESIZE,
                   Z_GTK_NO_SHRINK);

  tw_prv->record_toggle_handler_id =
    g_signal_connect (
      self->record, "toggled",
      G_CALLBACK (track_widget_on_record_toggled),
      self);
  tw_prv->mute_toggled_handler_id =
    g_signal_connect (
      self->mute, "toggled",
      G_CALLBACK (track_widget_on_mute_toggled),
      self);
  self->gui_toggled_handler_id = g_signal_connect (
      self->show_ui, "toggled",
      G_CALLBACK (instument_track_ui_toggle),
      self);
  tw_prv->solo_toggled_handler_id =
    g_signal_connect (
      self->solo, "toggled",
      G_CALLBACK (track_widget_on_solo_toggled),
      self);
  g_signal_connect (
    self->show_automation,
    "toggled",
    G_CALLBACK (track_widget_on_show_automation_toggled),
    self);

  ChannelTrack * ct = (ChannelTrack *) track;
  Channel * chan = ct->channel;
  Plugin * plugin = chan->plugins[0];
  if (plugin)
    {
      plugin_open_ui (plugin);
      gtk_widget_set_visible (GTK_WIDGET(self), 1);
    }
  return self;
}

void
instrument_track_widget_refresh_buttons (
  InstrumentTrackWidget * self)
{
  TRACK_WIDGET_GET_PRIVATE (self);
  g_signal_handler_block (
    self->record, tw_prv->record_toggle_handler_id);
      gtk_toggle_button_set_active (
        self->record,
        tw_prv->track->recording);
  g_signal_handler_unblock (
    self->record, tw_prv->record_toggle_handler_id);

  g_signal_handler_block (
    self->solo, tw_prv->solo_toggled_handler_id);
      gtk_toggle_button_set_active (
        self->solo,
        tw_prv->track->solo);
  g_signal_handler_unblock (
    self->solo, tw_prv->solo_toggled_handler_id);

  g_signal_handler_block (
    self->mute, tw_prv->mute_toggled_handler_id);
      gtk_toggle_button_set_active (
        self->mute,
        tw_prv->track->mute);
  g_signal_handler_unblock (
    self->mute, tw_prv->mute_toggled_handler_id);

  g_signal_handler_block (
    self->show_ui, self->gui_toggled_handler_id);
      gtk_toggle_button_set_active (
        self->show_ui,
	((InstrumentTrack *)tw_prv->track)->ui_active );
  g_signal_handler_unblock (
    self->show_ui, self->gui_toggled_handler_id);
}

void
instrument_track_widget_refresh (
  InstrumentTrackWidget * self)
{
  TRACK_WIDGET_GET_PRIVATE (self);
  Track * track = tw_prv->track;
  ChannelTrack * ct = (ChannelTrack *) track;
  Channel * chan = ct->channel;

  instrument_track_widget_refresh_buttons (self);

  gtk_label_set_text (
    tw_prv->name,
    track->name);

  AutomationTracklist * automation_tracklist =
    track_get_automation_tracklist (tw_prv->track);
  automation_tracklist_widget_refresh (
    automation_tracklist->widget);
}

static void
instrument_track_widget_init (InstrumentTrackWidget * self)
{
  GtkStyleContext * context;
  TRACK_WIDGET_GET_PRIVATE (self);
  /* create buttons */
  self->record =
    z_gtk_toggle_button_new_with_icon (
      "z-media-record");
  context =
    gtk_widget_get_style_context (
      GTK_WIDGET (self->record));
  gtk_style_context_add_class (
    context, "record-button");
  self->solo =
    z_gtk_toggle_button_new_with_resource (
      ICON_TYPE_ZRYTHM,
      "solo.svg");
  context =
    gtk_widget_get_style_context (
      GTK_WIDGET (self->solo));
  gtk_style_context_add_class (
    context, "solo-button");
  self->mute =
    z_gtk_toggle_button_new_with_resource (
      ICON_TYPE_ZRYTHM,
      "mute.svg");
  self->show_ui =
    z_gtk_toggle_button_new_with_resource (
      ICON_TYPE_ZRYTHM,
      "instrument.svg");
  self->show_automation =
    z_gtk_toggle_button_new_with_icon (
      "z-format-justify-fill");
  self->lock =
    GTK_TOGGLE_BUTTON (
      gtk_toggle_button_new_with_label ("Lock"));
  self->freeze =
    GTK_TOGGLE_BUTTON (
      gtk_toggle_button_new_with_label ("Freeze"));

  /* set buttons to upper controls */
  gtk_box_pack_start (
    GTK_BOX (tw_prv->upper_controls),
    GTK_WIDGET (self->record),
    Z_GTK_NO_EXPAND,
    Z_GTK_NO_FILL,
    0);
  gtk_box_pack_start (
    GTK_BOX (tw_prv->upper_controls),
    GTK_WIDGET (self->solo),
    Z_GTK_NO_EXPAND,
    Z_GTK_NO_FILL,
    0);
  gtk_box_pack_start (
    GTK_BOX (tw_prv->upper_controls),
    GTK_WIDGET (self->mute),
    Z_GTK_NO_EXPAND,
    Z_GTK_NO_FILL,
    0);
  gtk_box_pack_start (
    GTK_BOX (tw_prv->upper_controls),
    GTK_WIDGET (self->show_ui),
    Z_GTK_NO_EXPAND,
    Z_GTK_NO_FILL,
    0);

  /* pack buttons to bot controls */
  gtk_box_pack_start (
    GTK_BOX (tw_prv->bot_controls),
    GTK_WIDGET (self->lock),
    Z_GTK_NO_EXPAND,
    Z_GTK_NO_FILL,
    0);
  gtk_box_pack_start (
    GTK_BOX (tw_prv->bot_controls),
    GTK_WIDGET (self->freeze),
    Z_GTK_NO_EXPAND,
    Z_GTK_NO_FILL,
    0);
  gtk_box_pack_start (
    GTK_BOX (tw_prv->bot_controls),
    GTK_WIDGET (self->show_automation),
    Z_GTK_NO_EXPAND,
    Z_GTK_NO_FILL,
    0);

  /* set icon */
  resources_set_image_icon (
    tw_prv->icon,
    ICON_TYPE_ZRYTHM,
    "instrument.svg");

  gtk_widget_show_all (GTK_WIDGET (self));
}

static void
instrument_track_widget_class_init (
  InstrumentTrackWidgetClass * klass)
{
}
