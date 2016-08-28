project(lxqt-backlight-gui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Widgets REQUIRED QUIET)

set(H_FILES
    lxqt-backlight.h
)

set(CPP_FILES
    lxqt-backlight.cpp
    main.cpp
)

set(UI_FILES
    lxqtbacklight.ui
)

set(DESKTOP_FILES
    lxqt-backlight.desktop
)

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

add_definitions(
  -DICON_DIR="${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/48x48/apps"
)


add_executable(${PROJECT_NAME} ${CPP_FILES} ${UI_FILES} ${RESOURCES} ${QRC_SOURCES} ${QM_FILES} ${MOC_SOURCES})

target_link_libraries(${PROJECT_NAME}
    lxqtbacklight_backend
    lxqtbacklight
    Qt5::Widgets
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES ${DESKTOP_FILES} DESTINATION share/applications)
install(
    FILES icons/lxqtbacklight.svg
    DESTINATION share/icons/hicolor/48x48/apps
)
