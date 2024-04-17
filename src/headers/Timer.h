#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <functional>
#include <vector>

namespace Timer
{
    struct Timer
    {
        unsigned int id;
        std::chrono::microseconds expiration_time;
        std::function<void()> finish_callback;
        std::function<void(float)> update_callback;
    };

    void AddTimer(float seconds, std::function<void()> finish_callback, std::function<void(float)> update_callback = nullptr);

    void Update(float delta_time);
};

#endif // TIMER_H