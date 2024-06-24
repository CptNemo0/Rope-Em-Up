#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <functional>
#include <vector>
#include <deque>

#include "typedef.h"

namespace Timer
{
    struct Timer
    {
        unsigned int id;
        float delay;
        microseconds expiration_time;
        std::function<void()> finish_callback;
        std::function<void(float)> update_callback;
        bool loop;
    };

    /// @brief Creates a timer. Needs to be updated manually.
    /// @param delay delay in seconds
    /// @param finish_callback callback to be called when the timer finishes
    /// @param update_callback callback to be called every time the timer is updated
    /// @param loop bool to determine if the timer should loop
    /// @return The created timer
    Timer CreateTimer(float delay, std::function<void()> finish_callback, std::function<void(float)> update_callback = nullptr, bool loop = false);
    /// @brief Creates a timer and adds it to the list of timers to be updated.
    /// @param delay delay in seconds
    /// @param finish_callback callback to be called when the timer finishes
    /// @param update_callback callback to be called every time the timer is updated
    /// @param loop bool to determine if the timer should loop
    unsigned int AddTimer(float delay, std::function<void()> finish_callback, std::function<void(float)> update_callback = nullptr, bool loop = false);
    void RemoveTimer(unsigned int id);

    bool UpdateTimer(Timer &timer, float delta_time);
    void Update(float delta_time);

    float GetProgress(unsigned int id);
};

#endif // TIMER_H