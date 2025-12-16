// Copyright (C) 2024 Workrave
// All rights reserved.
//
// Utility functions for macOS-specific menu handling.

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "MenuRoleUtils.hh"

#include "commonui/MenuDefs.hh"

namespace macos
{
  void apply_standard_menu_roles(const QList<QAction *> &actions)
  {
    for (auto *action: actions)
      {
        if (action == nullptr)
          {
            continue;
          }

        const auto id = action->data().toString().toStdString();
        if (id == MenuId::ABOUT)
          {
            action->setMenuRole(QAction::AboutRole);
          }
        else if (id == MenuId::PREFERENCES)
          {
            action->setMenuRole(QAction::PreferencesRole);
          }
        else if (id == MenuId::QUIT)
          {
            action->setMenuRole(QAction::QuitRole);
          }
      }
  }
} // namespace macos
