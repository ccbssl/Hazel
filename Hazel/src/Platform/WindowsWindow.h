#pragma once

#include"Hazel/Window.h"

#include<GLFW/glfw3.h>

namespace Hazel {
	class WindowsWindow :public Window
	{
	public: 
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth()const override { return m_Date.Width; }
		inline unsigned int GetHeight()const override { return m_Date.Height; }

		//Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override
		{
			m_Date.EventCallback = callback;
		}
		void SetVSync(bool enabled) override;
		bool IsVSync()const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:

		GLFWwindow* m_Window;

		struct WindowDate
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowDate m_Date;
	};
}