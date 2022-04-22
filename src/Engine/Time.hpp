#pragma once

class Time
{
private:
    static unsigned s_TimeCurrentFrame;
    static unsigned s_TimeLastFrame;
    static float deltaTime;
public:
    // Returns the time in miliseconds since the last frame
    static float getDeltaTime() { return Time::deltaTime; }
    static void tick();
};