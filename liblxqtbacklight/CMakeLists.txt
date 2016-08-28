project(lxqtbacklight)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Widgets REQUIRED QUIET)

set(H_FILES
    lxqtbacklight.h
)

set(CPP_FILES
    lxqtbacklight.cpp
)

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

add_library(${PROJECT_NAME} ${CPP_FILES} ${UI_FILES} ${RESOURCES} ${QRC_SOURCES} ${QM_FILES} ${MOC_SOURCES})

target_link_libraries(${PROJECT_NAME}
    lxqtbacklight_backend
    Qt5::Widgets
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

