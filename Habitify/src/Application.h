#pragma once
// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <vector>
#include <memory>
#include <functional>

#include "utils/LayerStack.h"
#include "Board.h"



namespace Habitify
{
    class Application
    {
    public:
        Application();
        ~Application() = default;

        void Run();

        template<typename T>
        void PushLayer();
        void PushLayer(const std::shared_ptr<Layer> &);

        static Application& Get() {return *s_Instance; };  

        void Close();
    private:
        bool Init();
        void Shutdown();
    private:
        // Demo dependencies
        GLFWwindow *window;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        float m_TimeStep = 0.0f;
		float m_FrameTime = 0.0f;
		float m_LastFrameTime = 0.0f;   

        std::shared_ptr<Board> m_board;

        LayerStack m_layer_stack;
    private:
        static Application *s_Instance;
    };

    Application *CreateApplication();

    class ExampleLayer : public Habitify::Layer
    {
    public:
        virtual void OnUIRender() override; 

    private:
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);  
    };
}