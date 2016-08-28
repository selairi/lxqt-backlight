
project(lxqt-backlight_backend)


set(C_FILES
    lxqtbacklight_backend.c
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)


if(NOT CMAKE_BUILD_TYPE)
    set( CMAKE_BUILD_TYPE Release )
endif (NOT CMAKE_BUILD_TYPE)

add_executable(${PROJECT_NAME}
    ${C_FILES}
)

target_link_libraries(${PROJECT_NAME}
    lxqtbacklight_backend
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
