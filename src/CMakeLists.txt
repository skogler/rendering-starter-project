set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

configure_file(CompiledConfig.hpp.in CompiledConfig.hpp @ONLY)

add_executable(MyApplication 
	MyApplication.cpp
)
if (WIN32)
	set_property(TARGET MyApplication PROPERTY WIN32_EXECUTABLE TRUE)
endif()


target_include_directories(MyApplication PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(MyApplication PRIVATE
    Magnum::Magnum
    Magnum::Shaders
    Magnum::GlfwApplication
	MagnumImGui
)
