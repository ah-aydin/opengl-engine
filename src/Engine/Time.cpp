#include "Time.hpp"

#include <SDL2/SDL.h>

unsigned Time::s_TimeCurrentFrame;
unsigned Time::s_TimeLastFrame;
float Time::deltaTime = 0;

// Call this function once in the main loop to calculate the time since last frame
void Time::tick()
{
    Time::s_TimeCurrentFrame = SDL_GetTicks();
    Time::deltaTime = (Time::s_TimeCurrentFrame - Time::s_TimeLastFrame) / 1000.f;
    Time::s_TimeLastFrame = Time::s_TimeCurrentFrame;
}