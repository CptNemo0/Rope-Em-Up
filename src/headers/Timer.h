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

    std::vector<Timer> timers;
    std::vector<unsigned int> ids_to_remove;
    unsigned int next_id = 0;

    void AddTimer(float seconds, std::function<void()> finish_callback, std::function<void(float)> update_callback = nullptr)
    {
        Timer timer;
        timer.id = next_id++;
        timer.expiration_time = std::chrono::microseconds(static_cast<int>(seconds * 1000000));
        timer.finish_callback = finish_callback;
        timer.update_callback = update_callback;
        timers.push_back(timer);
    }

    void Update(float delta_time)
    {
        auto size = timers.size();
        for (size_t idx = 0u; idx < size; ++idx)
        {
            auto &timer = timers[idx];
            timer.expiration_time -= std::chrono::microseconds(static_cast<int>(delta_time * 1000000));
            if (timer.expiration_time <= std::chrono::microseconds(0))
            {
                if (timer.finish_callback)
                {
                    timer.finish_callback();
                }
                ids_to_remove.push_back(timer.id);
            }
            else
            {
                if (timer.update_callback)
                {
                    timer.update_callback(delta_time);
                }
            }
        }
        for (auto &id : ids_to_remove)
        {
            auto it = std::find_if(timers.begin(), timers.end(), [id](const Timer &t){ return t.id == id; });
            if (it != timers.end())
            {
                timers.erase(it);
            }
        }
        ids_to_remove.clear();
    }
};

#endif // TIMER_H