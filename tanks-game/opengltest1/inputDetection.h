#pragma once

#include "include/SDL2\SDL.h"
#undef main

namespace input
{
	enum MOUSEBUTTON
	{
		MSB_LEFT = 1,
		MSB_MIDDLE = 2,
		MSB_RIGHT = 3
	};
	enum MOUSEWHEELEVENT
	{
		MSW_UP = 1,
		MSW_DOWN = 2,
		MSW_NOTHING = 0
	};
	struct structMouseState
	{
		Uint32 state;	//Can be checked with SDL_BUTTON.
		int x, y;		//Relative to window.
						//MOUSEWHEELEVENT wheel;
	};

	static SDL_Event inputEvent;

	class InputDetector
	{
	public:

		InputDetector(const int& windowWidth_, const int& windowHeight_) :
			windowWidth(windowWidth_), windowHeight(windowHeight_), closedWindow(false)
		{
			update();
		}
		~InputDetector() {}
	private:

		const Uint8* keyboardStates;
		structMouseState mouseState_curr, mouseState_prev;
		MOUSEWHEELEVENT mouseWheelState;

		float timeCurr, timePrev, timeDelta;

		const int windowWidth;
		const int windowHeight;
	public:
		void update();

		bool KeyDown(SDL_Scancode key)const;
		bool ButtonClicked(input::MOUSEBUTTON button)const;

		bool closedWindow;

		int getMouseX()const;
		int getMouseY()const;
		input::MOUSEWHEELEVENT getMouseWheelState()const;

		int getDeltaMouseX()const;
		int getDeltaMouseY()const;

		float getMouseXNormalized()const;
		float getMouseYNormalized()const;

		float getTimeDelta()const;
	};
}
#pragma once
