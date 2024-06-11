#include "display.h"
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);


Display::Display(int width, int height, const std::string& title)
{

	if (!glfwInit())
	{
		printf("Failed to initialize GLFW\n");
		throw EXIT_FAILURE;
	}

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);

	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);


	m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	this->width = width;
	this->height = height;


	if (!m_window)
	{
		fprintf(stderr, "Failed to create window\n");
		glfwTerminate();
		throw EXIT_FAILURE;
	}

	glfwShowWindow(m_window);
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Failed to load OpenGL functions and extensions\n");
		glfwTerminate();
		throw EXIT_FAILURE;
	}

	glfwSwapInterval(1);



	//m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, , , SDL_WINDOW_OPENGL);
	//m_glContext = SDL_GL_CreateContext(m_window);


	/**if (!_2D) {
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}*/
}




Display::~Display()
{
	if (dont_kill == false)
	{

		glfwDestroyWindow(m_window);
		glfwTerminate();

		//SDL_GL_DeleteContext(m_glContext);
		//SDL_DestroyWindow(m_window);
		//SDL_Quit();
	}
}

void Display::Clear(float r, float g, float b, float a)
{

	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Display::SwapBuffers()
{

	glfwSwapBuffers(m_window);
	glfwPollEvents();

//	SDL_GL_SwapWindow(m_window);
}

void Display::setSize()
{
	int w, h;
	glfwGetWindowSize(m_window, &w, &h);
	width = w > 0? w: width;
	height = h > 0? h: height;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	if (width > 0 && height > 0)
		glViewport(0, 0, width, height);
}
