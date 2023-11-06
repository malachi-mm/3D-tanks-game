#include"inputDetection.h"

void input::InputDetector::update()
{
	mouseState_prev = mouseState_curr;

	timePrev = timeCurr;
	timeCurr = SDL_GetTicks();
	timeDelta = timeCurr - timePrev;

	mouseWheelState = MSW_NOTHING;
	while (SDL_PollEvent(&input::inputEvent))
	{
		if (inputEvent.type == SDL_MOUSEWHEEL)
		{
			if (inputEvent.wheel.y > 0)
				mouseWheelState = MSW_UP;

			else //if(inputEvent.wheel.y < 0) 
				mouseWheelState = MSW_DOWN;
		}
		if (inputEvent.type == SDL_QUIT)
			closedWindow = true;
	}


	keyboardStates = SDL_GetKeyboardState(NULL);
	mouseState_curr.state = SDL_GetMouseState(&mouseState_curr.x, &mouseState_curr.y);
}

bool input::InputDetector::KeyDown(SDL_Scancode key) const
{
	return keyboardStates[key];
}

bool input::InputDetector::ButtonClicked(input::MOUSEBUTTON button) const
{
	return mouseState_curr.state & SDL_BUTTON(button);
}

int input::InputDetector::getMouseX() const
{
	return mouseState_curr.x;
}

int input::InputDetector::getMouseY() const
{
	return mouseState_curr.y;
}

input::MOUSEWHEELEVENT input::InputDetector::getMouseWheelState() const
{
	return mouseWheelState;
}

int input::InputDetector::getDeltaMouseX() const
{
	return mouseState_curr.x - mouseState_prev.x;
}

int input::InputDetector::getDeltaMouseY() const
{
	return mouseState_curr.y - mouseState_prev.y;
}

float input::InputDetector::getMouseXNormalized() const
{
	return (float)(getMouseX() - windowWidth / 2) / (float)(windowWidth / 2);
}

float input::InputDetector::getMouseYNormalized() const
{
	return (float)(getMouseY() - windowHeight / 2) / (float)(windowHeight / 2);
}

float input::InputDetector::getTimeDelta() const
{
	return timeDelta / 1000.f;
}

