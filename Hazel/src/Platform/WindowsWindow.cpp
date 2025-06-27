#include"hzpch.h"
#include"WindowsWindow.h"

#include"Hazel/Events/KeyEvent.h"
#include"Hazel/Events/MouseEvent.h"
#include"Hazel/Events/ApplicationEvent.h"

namespace Hazel {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}
					 
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Date.Title = props.Title;
		m_Date.Width = props.Width;
		m_Date.Height = props.Height;

		HZ_CORE_INFO("Creating window{0}({1},{2})",
			props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Counld not intialize GLfW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height,
			m_Date.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Date);
		SetVSync(true);
		//set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
				WindowDate& date = *(WindowDate*)glfwGetWindowUserPointer(window);
				date.Width = width;
				date.Height = height;

				WindowResizeEvent event(width, height);
				date.EventCallback(event);	
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowDate& date = *(WindowDate*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event; 
			date.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key,
					int scancode, int action, int mods)
		{
			WindowDate& date = *(WindowDate*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						date.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						date.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1);
						date.EventCallback(event);
						break;
					}
			}
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button,
			int action, int mods)
		{
				WindowDate& date = *(WindowDate*)glfwGetWindowUserPointer(window);
				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						date.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						date.EventCallback(event);
						break;
					}
				}
		});

		glfwSetScrollCallback(m_Window,[](GLFWwindow* window, 
						double xOffset, double yOffset)
		{
			WindowDate& date = *(WindowDate*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xOffset,(float)yOffset);
			date.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window,
			double xPos, double yPos)
		{
			WindowDate& date = *(WindowDate*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xPos, (float)yPos);
			date.EventCallback(event);
		});

	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Date.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Date.VSync;
	}

}