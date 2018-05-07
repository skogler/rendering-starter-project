#include "CompiledConfig.hpp"

#include "MagnumImGui.hpp"

#include <Magnum/Platform/GlfwApplication.h>

#include <Magnum/GL/Context.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/DebugOutput.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>

#include <Corrade/Corrade.h>
#include <Magnum/Math/Color.h>
#include <imgui.h>
#include <nfd.h>
#include <iostream>

using namespace Magnum;

class MyApplication : public Platform::Application
{
public:
    explicit MyApplication(const Arguments& arguments);

    void drawEvent() override;
    void viewportEvent(const Vector2i& size) override;
    void keyPressEvent(KeyEvent& event) override;
    void keyReleaseEvent(KeyEvent& event) override;
    void mousePressEvent(MouseEvent& event) override;
    void mouseReleaseEvent(MouseEvent& event) override;
    void mouseMoveEvent(MouseMoveEvent& event) override;
    void mouseScrollEvent(MouseScrollEvent& event) override;
    void textInputEvent(TextInputEvent& event) override;

protected:
    void save();
    void open();
    void close();

    // bool m_main_window_active{ false };
    MagnumImGui m_imgui;
    Timeline m_timeline;
};

MyApplication::MyApplication(const Arguments& arguments)
    : Platform::Application{
          arguments, 
		  
		  Configuration{}.setWindowFlags(Configuration::WindowFlag::Resizable | Configuration::WindowFlag::Maximized)
                         .setTitle(CompiledConfig::PROJECT_NAME),
		GLConfiguration{}
#ifndef NDEBUG
                         .setFlags(Configuration::Flag::Debug)
#endif
      }
{
    using namespace Magnum::Math::Literals;
#ifndef NDEBUG
    GL::Renderer::enable(GL::Renderer::Feature::DebugOutput);
    GL::Renderer::enable(GL::Renderer::Feature::DebugOutputSynchronous);
    GL::DebugOutput::setDefaultCallback();
#endif
	GL::Renderer::setClearColor(0xa5c9ea_rgbf);
    m_timeline.start();
    m_imgui.resizeEvent(windowSize(), GL::defaultFramebuffer.viewport().size());
}

void MyApplication::drawEvent()
{
    m_timeline.nextFrame();
	GL::defaultFramebuffer.clear(FramebufferClear::Color | FramebufferClear::Depth);
    m_imgui.update(m_timeline.previousFrameDuration());
    const auto vp = GL::defaultFramebuffer.viewport();

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(vp.right(), 0));
    ImGui::Begin("Main Window", nullptr,
                 ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open...", "Ctrl+O"))
            {
                open();
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
                save();
            }
            if (ImGui::MenuItem("Close", "Ctrl+W"))
            {
                close();
            }
			if (ImGui::MenuItem("Quit", "Ctrl+Q"))
			{
				exit();
			}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::Text("No file opened!");
    ImGui::End();

    m_imgui.drawFrame();
    swapBuffers();
    redraw();
}

void MyApplication::viewportEvent(const Vector2i& size)
{
    GL::defaultFramebuffer.setViewport({{}, size});
    m_imgui.resizeEvent(windowSize(), GL::defaultFramebuffer.viewport().size());
    redraw();
}

void MyApplication::keyPressEvent(KeyEvent& event)
{
    if (m_imgui.keyPressEvent(event))
        return;
}

void MyApplication::keyReleaseEvent(KeyEvent& event)
{
    if (m_imgui.keyReleaseEvent(event))
        return;
}

void MyApplication::mousePressEvent(MouseEvent& event)
{
    if (m_imgui.mousePressEvent(event))
        return;
}

void MyApplication::mouseReleaseEvent(MouseEvent& event)
{
    if (m_imgui.mouseReleaseEvent(event))
        return;
}

void MyApplication::mouseMoveEvent(MouseMoveEvent& event)
{
    if (m_imgui.mouseMoveEvent(event))
        return;
}

void MyApplication::mouseScrollEvent(MouseScrollEvent& event)
{
    if (m_imgui.mouseScrollEvent(event))
        return;
}

void MyApplication::textInputEvent(TextInputEvent& event)
{
    if (m_imgui.textInputEvent(event))
        return;
}

void MyApplication::save()
{
}

void MyApplication::open()
{
	nfdchar_t *selected_path = nullptr;
	nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &selected_path);
	if (result == NFD_OKAY) {
		std::cout << selected_path << std::endl;
		free(selected_path);
	}
}

void MyApplication::close()
{
}

int main(int argc, char** argv)
{
    MyApplication app({argc, argv});
    return app.exec();
}

#ifdef CORRADE_TARGET_WINDOWS

#include <wrl.h>

#include <iostream>
#include <sstream>

class DebugOutputStringBuf : public std::stringbuf
{
public:
    ~DebugOutputStringBuf()
    {
        sync();
    }
    int sync()
    {
        ::OutputDebugStringA(str().c_str());
        str({});
        return 0;
    }
};
DebugOutputStringBuf g_stdout_debug_buf;

int main(int argc, char** argv);

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Redirect std c++ output to OutputDebugStringA
    if (!GetConsoleWindow())
    {
        std::cout.rdbuf(&g_stdout_debug_buf);
        std::cerr.rdbuf(&g_stdout_debug_buf);
        std::clog.rdbuf(&g_stdout_debug_buf);
    }
    return main(__argc, __argv);
}
#endif