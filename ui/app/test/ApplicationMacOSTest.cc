// Copyright (C) 2024 Workrave
// Test coverage for macOS-specific application behavior.

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <QAction>
#include <QApplication>

#define BOOST_TEST_MODULE "workrave-application-macos"
#include <boost/test/unit_test.hpp>

#include "platforms/macos/MenuRoleUtils.hh"
#include "commonui/MenuDefs.hh"

namespace
{
  auto make_action(const std::string &id) -> QAction *
  {
    auto *action = new QAction;
    action->setData(QString::fromStdString(id));
    return action;
  }
}

BOOST_AUTO_TEST_CASE(standard_menu_roles_are_assigned)
{
  int argc = 0;
  QApplication app(argc, nullptr);

  auto *about = make_action(std::string(MenuId::ABOUT));
  auto *preferences = make_action(std::string(MenuId::PREFERENCES));
  auto *quit = make_action(std::string(MenuId::QUIT));
  auto *other = make_action("workrave.custom");

  QList<QAction *> actions{about, preferences, quit, other};

  macos::apply_standard_menu_roles(actions);

  BOOST_CHECK_EQUAL(about->menuRole(), QAction::AboutRole);
  BOOST_CHECK_EQUAL(preferences->menuRole(), QAction::PreferencesRole);
  BOOST_CHECK_EQUAL(quit->menuRole(), QAction::QuitRole);
  BOOST_CHECK_EQUAL(other->menuRole(), QAction::TextHeuristicRole);

  for (auto *action: actions)
    {
      delete action;
    }
}
