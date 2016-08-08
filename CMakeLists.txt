cmake_minimum_required( VERSION 2.6 )
project(lxqt-backlight)


set(C_FILES
    lxqt-backlight.c
)

set(POLKIT_FILES
    ${CMAKE_BINARY_DIR}/org.lxqt.backlight.pkexec.policy
)

# Config file
configure_file ( org.lxqt.backlight.pkexec.policy.in org.lxqt.backlight.pkexec.policy )

cmake_minimum_required( VERSION 2.6 )

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)


if(NOT CMAKE_BUILD_TYPE)
    set( CMAKE_BUILD_TYPE Release )
endif (NOT CMAKE_BUILD_TYPE)

set(APP_SHARE_DIR share/lxqt/${PROJECT_NAME})


add_executable(${PROJECT_NAME}
    ${C_FILES}
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES ${POLKIT_FILES} DESTINATION /usr/share/polkit-1/actions/)

# building tarball with CPack -------------------------------------------------
#include (InstallRequiredSystemLibraries)
#set (CPACK_PACKAGE_VERSION_MAJOR ${LXQT_MAJOR_VERSION})
#set (CPACK_PACKAGE_VERSION_MINOR ${LXQT_MINOR_VERSION})
#set (CPACK_PACKAGE_VERSION_PATCH ${LXQT_PATCH_VERSION})
#set (CPACK_GENERATOR TBZ2)
#set (CPACK_SOURCE_GENERATOR TBZ2)
#set (CPACK_SOURCE_IGNORE_FILES /build/;.gitignore;.*~;.git;.kdev4;temp)
#include (CPack)
