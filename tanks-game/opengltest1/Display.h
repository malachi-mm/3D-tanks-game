#ifndef DISPLAY_INCLUDED_H
#define DISPLAY_INCLUDED_H


#include <stdlib.h>

#include <glad/glad.h> /* https://github.com/Dav1dde/glad */
#include <GLFW/glfw3.h> /* https://github.com/glfw/glfw */

#include <string>
//#include <SDL2/SDL.h>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


class Display
{
public:
	Display(int width, int height, const std::string& title);
	//Display(GLFWwindow* win);
	Display() {}

	void Clear(float r, float g, float b, float a);
	void SwapBuffers();
	void setSize();
	GLFWwindow* GetWindow() { return m_window; }
	virtual ~Display();

	void operator=(const Display& display)
	{
		//m_glContext = display.m_glContext;
		m_window = display.m_window;
	}

	Display(const Display& display)
	{
		//m_glContext = display.m_glContext;
		m_window = display.m_window;
	}
	void dontKill(bool d) { dont_kill = d; }

	int GetWidth() { return width; }
	int GetHeight() { return height; }

	void SetWidth(int w) { width = w; }
	void SetHeight(int h) { height = h; }
protected:
private:
	bool dont_kill = false;
	int width, height;
	GLFWwindow* m_window;
	//GLFWcontext
	//SDL_GLContext m_glContext;
};

#endif
