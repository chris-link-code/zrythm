// SPDX-FileCopyrightText: © 2021, 2023 Alexandros Theodotou <alex@zrythm.org>
// SPDX-License-Identifier: LicenseRef-ZrythmLicense

#include "plugins/plugin_identifier.h"
#include "utils/debug.h"

void
plugin_identifier_init (PluginIdentifier * self)
{
  memset (self, 0, sizeof (PluginIdentifier));
  self->schema_version = PLUGIN_IDENTIFIER_SCHEMA_VERSION;
  self->slot = -1;
}

bool
plugin_identifier_validate (const PluginIdentifier * self)
{
  z_return_val_if_fail_cmp (
    self->schema_version, ==,
    PLUGIN_IDENTIFIER_SCHEMA_VERSION, false);
  g_return_val_if_fail (
    plugin_identifier_validate_slot_type_slot_combo (
      self->slot_type, self->slot),
    false);
  return true;
}

/**
 * Verifies that @ref slot_type and @ref slot is
 * a valid combination.
 */
bool
plugin_identifier_validate_slot_type_slot_combo (
  PluginSlotType slot_type,
  int            slot)
{
  return (slot_type == PLUGIN_SLOT_INSTRUMENT && slot == -1)
         || (slot_type == PLUGIN_SLOT_INVALID && slot == -1)
         || (slot_type != PLUGIN_SLOT_INSTRUMENT && slot >= 0);
}

void
plugin_identifier_copy (
  PluginIdentifier *       dest,
  const PluginIdentifier * src)
{
  g_return_if_fail (plugin_identifier_validate (src));

  dest->schema_version = src->schema_version;
  dest->slot_type = src->slot_type;
  dest->track_name_hash = src->track_name_hash;
  dest->slot = src->slot;
}

void
plugin_identifier_print (
  const PluginIdentifier * self,
  char *                   str)
{
  sprintf (
    str, "slot_type: %d, track_name hash: %u, slot: %d",
    self->slot_type, self->track_name_hash, self->slot);
}
