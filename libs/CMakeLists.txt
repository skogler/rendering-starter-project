add_library(MagnumImGui STATIC
	MagnumImGui.hpp
	MagnumImGui.cpp
)

target_include_directories(MagnumImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MagnumImGui PUBLIC imgui Magnum::Magnum)