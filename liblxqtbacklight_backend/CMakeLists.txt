
set(SOURCE_FILES_C
    lxqtbacklight_backend.c
)

set(SOURCE_FILES_H
    lxqtbacklight_backend.h
)

if(NOT CMAKE_BUILD_TYPE)
    set( CMAKE_BUILD_TYPE Release )
endif (NOT CMAKE_BUILD_TYPE)

add_library(lxqtbacklight_backend SHARED ${SOURCE_FILES_C})

install(TARGETS lxqtbacklight_backend DESTINATION lib)
install(FILES ${SOURCE_FILES_H} DESTINATION include)
