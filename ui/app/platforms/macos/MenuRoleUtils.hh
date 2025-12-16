// Copyright (C) 2024 Workrave
// All rights reserved.
//
// Helper utilities for configuring macOS menu roles.

#ifndef WORKRAVE_UI_APP_PLATFORMS_MACOS_MENUROLEUTILS_HH
#define WORKRAVE_UI_APP_PLATFORMS_MACOS_MENUROLEUTILS_HH

#include <QAction>
#include <QList>

namespace macos
{
  //! Apply the standard macOS menu roles to the given actions based on their menu identifiers.
  void apply_standard_menu_roles(const QList<QAction *> &actions);
}

#endif // WORKRAVE_UI_APP_PLATFORMS_MACOS_MENUROLEUTILS_HH
