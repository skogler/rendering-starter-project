add_library(nativefiledialog STATIC
    common.h
    nfd_common.c
)
target_include_directories(nativefiledialog PUBLIC include)

if(WIN32)
    target_link_libraries(nativefiledialog PUBLIC comctl32)
    target_sources(nativefiledialog PUBLIC nfd_win.cpp)
elseif(APPLE)
    target_link_libraries(nativefiledialog PUBLIC comctl32)
    target_sources(nativefiledialog PUBLIC nfd_cocoa.m)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    target_link_libraries(nativefiledialog PUBLIC ${GTK3_LIBRARIES})
    target_include_directories(nativefiledialog PRIVATE ${GTK3_INCLUDE_DIRS})
    target_sources(nativefiledialog PUBLIC nfd_gtk.c)   
endif()
