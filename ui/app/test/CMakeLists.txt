if (HAVE_TESTS AND WITH_UI_QT AND PLATFORM_OS_MACOS)
  add_executable(workrave-application-macos-test ApplicationMacOSTest.cc)
  target_code_coverage(workrave-application-macos-test AUTO)

  target_link_libraries(workrave-application-macos-test PRIVATE workrave-app)
  target_link_libraries(workrave-application-macos-test PRIVATE Qt5::Widgets)
  target_link_libraries(workrave-application-macos-test PRIVATE Boost::unit_test_framework)
  target_link_libraries(workrave-application-macos-test PRIVATE Boost::test_exec_monitor)

  add_test(NAME workrave-application-macos-test COMMAND workrave-application-macos-test)
endif()
