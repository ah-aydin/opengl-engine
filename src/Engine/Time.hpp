#pragma once

namespace oe
{
    class Time
    {
    private:
        static unsigned s_TimeCurrentFrame;
        static unsigned s_TimeLastFrame;
        static float deltaTime;
    public:
        static float getDeltaTime() { return Time::deltaTime; }
        static void tick();
    };
}