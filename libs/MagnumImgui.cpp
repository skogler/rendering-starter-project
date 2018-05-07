// The MIT License (MIT)
//
// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or distribute
// this software, either in source code form or as a compiled binary, for any
// purpose, commercial or non - commercial, and by any means.
//
// In jurisdictions that recognize copyright laws, the author or authors of
// this software dedicate any and all copyright interest in the software to
// the public domain.We make this dedication for the benefit of the public
// at large and to the detriment of our heirs and successors.We intend this
// dedication to be an overt act of relinquishment in perpetuity of all
// present and future rights to this software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL
// THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "MagnumImGui.hpp"

#include <Corrade/Utility/Resource.h>

#include <Magnum/GL/Context.h>
#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/ImageView.h>
#include <Magnum/GL/PixelFormat.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Shader.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Mesh.h>

#include <imgui.h>

using namespace Magnum;

void MagnumImGui::init()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.KeyMap[ImGuiKey_Tab] = ImGuiKey_Tab;
    io.KeyMap[ImGuiKey_LeftArrow] = ImGuiKey_LeftArrow;
    io.KeyMap[ImGuiKey_RightArrow] = ImGuiKey_RightArrow;
    io.KeyMap[ImGuiKey_UpArrow] = ImGuiKey_UpArrow;
    io.KeyMap[ImGuiKey_DownArrow] = ImGuiKey_DownArrow;
    io.KeyMap[ImGuiKey_PageUp] = ImGuiKey_PageUp;
    io.KeyMap[ImGuiKey_PageDown] = ImGuiKey_PageDown;
    io.KeyMap[ImGuiKey_Home] = ImGuiKey_Home;
    io.KeyMap[ImGuiKey_End] = ImGuiKey_End;
    io.KeyMap[ImGuiKey_Delete] = ImGuiKey_Delete;
    io.KeyMap[ImGuiKey_Backspace] = ImGuiKey_Backspace;
    io.KeyMap[ImGuiKey_Enter] = ImGuiKey_Enter;
    io.KeyMap[ImGuiKey_Escape] = ImGuiKey_Escape;
    io.KeyMap[ImGuiKey_A] = ImGuiKey_A;
    io.KeyMap[ImGuiKey_C] = ImGuiKey_C;
    io.KeyMap[ImGuiKey_V] = ImGuiKey_V;
    io.KeyMap[ImGuiKey_X] = ImGuiKey_X;
    io.KeyMap[ImGuiKey_Y] = ImGuiKey_Y;
    io.KeyMap[ImGuiKey_Z] = ImGuiKey_Z;

    //   io.SetClipboardTextFn = ;
    //   io.GetClipboardTextFn = ;

    mTimeline.start();
}

void MagnumImGui::load()
{
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int width, height;
    int pixel_size;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &pixel_size);
    ImGui::StyleColorsDark();
    ImGui::GetStyle().WindowRounding = 0.0f;

    ImageView2D image{PixelFormat::RGBA,
                      PixelType::UnsignedByte,
                      {width, height},
                      {pixels, std::size_t(pixel_size * width * height)}};

    mTexture.setMagnificationFilter(Sampler::Filter::Linear)
        .setMinificationFilter(Sampler::Filter::Linear)
        .setStorage(0, TextureFormat::RGBA, image.size())
        .setImage(0, TextureFormat::RGBA, image);

    mMesh.setPrimitive(MeshPrimitive::Triangles);
    mMesh.addVertexBuffer(
        mVertexBuffer, 0, ImguiShader::Position{},
        ImguiShader::TextureCoordinates{},
        ImguiShader::Color(ImguiShader::Color::Components::Four,
                           ImguiShader::Color::DataType::UnsignedByte,
                           ImguiShader::Color::DataOption::Normalized));
}

bool MagnumImGui::keyPressReleaseEvent(
    const Platform::Application::KeyEvent& event, bool value)
{
    ImGuiIO& io = ImGui::GetIO();

    switch (event.key())
    {
    case Platform::Application::KeyEvent::Key::LeftShift:
        io.KeyShift = value;
        break;
    case Platform::Application::KeyEvent::Key::RightShift:
        io.KeyShift = value;
        break;
    case Platform::Application::KeyEvent::Key::LeftCtrl:
        io.KeyCtrl = value;
        break;
    case Platform::Application::KeyEvent::Key::RightCtrl:
        io.KeyCtrl = value;
        break;
    case Platform::Application::KeyEvent::Key::LeftAlt:
        io.KeyAlt = value;
        break;
    case Platform::Application::KeyEvent::Key::RightAlt:
        io.KeyAlt = value;
        break;
    case Platform::Application::KeyEvent::Key::Tab:
        io.KeysDown[ImGuiKey_Tab] = value;
        break;
    case Platform::Application::KeyEvent::Key::Up:
        io.KeysDown[ImGuiKey_UpArrow] = value;
        break;
    case Platform::Application::KeyEvent::Key::Down:
        io.KeysDown[ImGuiKey_DownArrow] = value;
        break;
    case Platform::Application::KeyEvent::Key::Left:
        io.KeysDown[ImGuiKey_LeftArrow] = value;
        break;
    case Platform::Application::KeyEvent::Key::Right:
        io.KeysDown[ImGuiKey_RightArrow] = value;
        break;
    case Platform::Application::KeyEvent::Key::Home:
        io.KeysDown[ImGuiKey_Home] = value;
        break;
    case Platform::Application::KeyEvent::Key::End:
        io.KeysDown[ImGuiKey_End] = value;
        break;
    case Platform::Application::KeyEvent::Key::PageUp:
        io.KeysDown[ImGuiKey_PageUp] = value;
        break;
    case Platform::Application::KeyEvent::Key::PageDown:
        io.KeysDown[ImGuiKey_PageDown] = value;
        break;
    case Platform::Application::KeyEvent::Key::Enter:
        io.KeysDown[ImGuiKey_Enter] = value;
        break;
    case Platform::Application::KeyEvent::Key::Esc:
        io.KeysDown[ImGuiKey_Escape] = value;
        break;
    case Platform::Application::KeyEvent::Key::Backspace:
        io.KeysDown[ImGuiKey_Backspace] = value;
        break;
    case Platform::Application::KeyEvent::Key::Delete:
        io.KeysDown[ImGuiKey_Delete] = value;
        break;
    case Platform::Application::KeyEvent::Key::A:
        io.KeysDown[ImGuiKey_A] = value;
        break;
    case Platform::Application::KeyEvent::Key::C:
        io.KeysDown[ImGuiKey_C] = value;
        break;
    case Platform::Application::KeyEvent::Key::V:
        io.KeysDown[ImGuiKey_V] = value;
        break;
    case Platform::Application::KeyEvent::Key::X:
        io.KeysDown[ImGuiKey_X] = value;
        break;
    case Platform::Application::KeyEvent::Key::Y:
        io.KeysDown[ImGuiKey_Y] = value;
        break;
    case Platform::Application::KeyEvent::Key::Z:
        io.KeysDown[ImGuiKey_Z] = value;
        break;
    default:
        break;
    }

    return io.WantCaptureKeyboard;
}

bool MagnumImGui::mousePressReleaseEvent(
    const Platform::Application::MouseEvent& event, bool value)
{
    switch (event.button())
    {
    case Magnum::Platform::Application::MouseEvent::Button::Left:
        mMousePressed[0] = value;
        break;
    case Magnum::Platform::Application::MouseEvent::Button::Right:
        mMousePressed[1] = value;
        break;
    case Magnum::Platform::Application::MouseEvent::Button::Middle:
        mMousePressed[2] = value;
        break;
    default:
        break;
    }
    return ImGui::GetIO().WantCaptureMouse;
}

MagnumImGui::MagnumImGui()
{
    init();
    load();
}

MagnumImGui::~MagnumImGui()
{
    ImGui::DestroyContext();
}

void MagnumImGui::resizeEvent(const Vector2i& winSize,
                              const Vector2i& viewportSize)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)winSize.x(), (float)winSize.y());
    io.DisplayFramebufferScale =
        ImVec2(winSize.x() > 0 ? ((float)viewportSize.x() / winSize.x()) : 0,
               winSize.y() > 0 ? ((float)viewportSize.y() / winSize.y()) : 0);
}

void MagnumImGui::update(float dt)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = dt;
    io.MousePos = ImVec2(float(mMousePos.x()), float(mMousePos.y()));

    for (int i = 0; i < 3; i++)
    {
        io.MouseDown[i] = mMousePressed[i];
    }

    io.MouseWheel = mMouseScroll;
    mMouseScroll = 0.0f;

    ImGui::NewFrame();
}

void MagnumImGui::drawFrame()
{
    ImGui::Render();

    ImGuiIO& io = ImGui::GetIO();
    int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;

    auto draw_data = ImGui::GetDrawData();
    if (!draw_data)
        return;

    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    Renderer::enable(Renderer::Feature::Blending);
    Renderer::setBlendEquation(Magnum::Renderer::BlendEquation::Add,
                               Magnum::Renderer::BlendEquation::Add);
    Renderer::setBlendFunction(Renderer::BlendFunction::SourceAlpha,
                               Renderer::BlendFunction::OneMinusSourceAlpha);
    Renderer::disable(Renderer::Feature::FaceCulling);
    Renderer::disable(Renderer::Feature::DepthTest);
    Renderer::enable(Renderer::Feature::ScissorTest);

    const Matrix4 ortho_projection{
        {2.0f / io.DisplaySize.x, 0.0f, 0.0f, 0.0f},
        {0.0f, 2.0f / -io.DisplaySize.y, 0.0f, 0.0f},
        {0.0f, 0.0f, -1.0f, 0.0f},
        {-1.0f, 1.0f, 0.0f, 1.0f},
    };
    mShader.setProjectMatrix(ortho_projection);
    mShader.setTexture(mTexture);

    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        ImDrawIdx idx_buffer_offset = 0;

        mVertexBuffer.setData(
            {cmd_list->VtxBuffer.Data, std::size_t(cmd_list->VtxBuffer.Size)},
            BufferUsage::StreamDraw);
        mIndexBuffer.setData(
            {cmd_list->IdxBuffer.Data, std::size_t(cmd_list->IdxBuffer.Size)},
            BufferUsage::StreamDraw);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

            Renderer::setScissor(
                {{(int)pcmd->ClipRect.x, fb_height - (int)(pcmd->ClipRect.w)},
                 {(int)(pcmd->ClipRect.z),
                  fb_height - (int)(pcmd->ClipRect.y)}});

            mMesh.setCount(pcmd->ElemCount);
            mMesh.setIndexBuffer(
                mIndexBuffer, idx_buffer_offset * sizeof(ImDrawIdx),
                sizeof(ImDrawIdx) == 2 ? Mesh::IndexType::UnsignedShort
                                       : Mesh::IndexType::UnsignedInt);

            idx_buffer_offset += pcmd->ElemCount;

            mMesh.draw(mShader);
        }
    }

    Renderer::disable(Renderer::Feature::ScissorTest);
    Renderer::enable(Renderer::Feature::FaceCulling);
    Renderer::enable(Renderer::Feature::DepthTest);
}

bool MagnumImGui::mousePressEvent(
    const Platform::Application::MouseEvent& event)
{
    return mousePressReleaseEvent(event, true);
}

bool MagnumImGui::mouseReleaseEvent(
    const Platform::Application::MouseEvent& event)
{
    return mousePressReleaseEvent(event, false);
}

bool MagnumImGui::mouseScrollEvent(
    const Platform::Application::MouseScrollEvent& event)
{
    mMouseScroll += event.offset().y();
    return ImGui::GetIO().WantCaptureMouse;
}

bool MagnumImGui::mouseMoveEvent(
    const Platform::Application::MouseMoveEvent& event)
{
    mMousePos = event.position();
    return ImGui::GetIO().WantCaptureMouse;
}

bool MagnumImGui::keyPressEvent(const Platform::Application::KeyEvent& event)
{
    return keyPressReleaseEvent(event, true);
}

bool MagnumImGui::keyReleaseEvent(const Platform::Application::KeyEvent& event)
{
    return keyPressReleaseEvent(event, false);
}

bool MagnumImGui::textInputEvent(
    const Platform::Application::TextInputEvent& event)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharactersUTF8(event.text().data());
    return false;
}

ImguiShader::ImguiShader()
{
    const char* vertex_shader = R"(

#ifndef NEW_GLSL
#define in attribute
#define out varying
#endif
		
#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 0)
#endif
uniform mediump mat4 ProjMtx;
		
#ifdef EXPLICIT_ATTRIB_LOCATION
layout(location = 0)
#endif
in mediump vec2 Position;
		
#ifdef EXPLICIT_ATTRIB_LOCATION
layout(location = 1)
#endif
in mediump  vec2 UV;
		
#ifdef EXPLICIT_ATTRIB_LOCATION
layout(location = 2)
#endif
in mediump vec4 Color;
		
out mediump vec2 Frag_UV;
out mediump vec4 Frag_Color;
		
void main()
{
	Frag_UV = UV;
	Frag_Color = Color;
	gl_Position = ProjMtx * vec4(Position.xy,0,1);
}
)";

    const char* fragment_shader = R"(

#ifndef NEW_GLSL
#define in varying
#define color gl_FragColor
#define texture texture2D
#endif
		
#ifndef RUNTIME_CONST
#define const
#endif
		
#ifdef EXPLICIT_TEXTURE_LAYER
layout(binding = 0)
#endif
uniform lowp sampler2D Texture;
		
in mediump vec2 Frag_UV;
in mediump vec4 Frag_Color;
		
#ifdef NEW_GLSL
out lowp vec4 color;
#endif
		
void main()
{
color = Frag_Color * texture( Texture, Frag_UV.st);
}
)";

#ifndef MAGNUM_TARGET_GLES
    const GL::Version version = GL::Context::current().supportedVersion(
        { GL::Version::GL330, GL::Version::GL310, GL::Version::GL300, GL::Version::GL210});
#else
    const Version version = Context::current().supportedVersion(
        {Version::GLES300, Version::GLES200});
#endif

    GL::Shader vert{version, Shader::Type::Vertex};
    GL::Shader frag{version, Shader::Type::Fragment};

#ifndef MAGNUM_TARGET_GLES
    if (version != Version::GL210)
    {
        vert.addSource({"#define NEW_GLSL"});
        frag.addSource({"#define NEW_GLSL"});
    }
#else
    if (version != Version::GLES200)
    {
        vert.addSource({"#define NEW_GLSL"});
        frag.addSource({"#define NEW_GLSL"});
    }
#endif

    vert.addSource({vertex_shader});
    frag.addSource({fragment_shader});

    CORRADE_INTERNAL_ASSERT_OUTPUT(GL::Shader::compile({vert, frag}));

    attachShaders({vert, frag});

    CORRADE_INTERNAL_ASSERT_OUTPUT(link());

    mProjMatrixUniform = uniformLocation("ProjMtx");

    setUniform(uniformLocation("Texture"), TextureLayer);
}
