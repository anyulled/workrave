// Copyright (C) 2021 Rob Caelers <robc@krandor.nl>
// All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "ApplicationMacOS.hh"
#include "platforms/macos/MenuRoleUtils.hh"
#include "commonui/MenuDefs.hh"
#include "commonui/MenuModel.hh"
#include "utils/Signals.hh"

#include <QAction>
#include <QApplication>
#include <QCoreApplication>

ApplicationMacOS::ApplicationMacOS(int argc, char **argv, std::shared_ptr<IToolkitFactory> toolkit_factory)
  : Application(argc, argv, toolkit_factory)
{
}

void
ApplicationMacOS::init_platform_pre()
{
  if (auto model = get_menu_model())
    {
      workrave::utils::connect(model->signal_update(), this, [this] { apply_standard_menu_roles(); });
    }
}

void
ApplicationMacOS::init_platform_post()
{
  apply_standard_menu_roles();

  dock = std::make_shared<MacOSDock>(get_application_context());
}

void
ApplicationMacOS::apply_standard_menu_roles()
{
  if (auto *qt_app = qobject_cast<QApplication *>(QCoreApplication::instance()); qt_app != nullptr)
    {
      macos::apply_standard_menu_roles(qt_app->findChildren<QAction *>());
    }
}
