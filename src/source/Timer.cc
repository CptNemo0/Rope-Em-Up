#include "../headers/Timer.h"

namespace Timer
{

std::vector<Timer> timers;
std::vector<unsigned int> ids_to_remove;
unsigned int next_id = 0;

Timer CreateTimer(float delay, std::function<void()> finish_callback, std::function<void(float)> update_callback, bool loop)
{
    Timer timer;
    timer.id = next_id++;
    timer.delay = delay;
    timer.expiration_time = microseconds(static_cast<int>(delay * 1000000));
    timer.finish_callback = finish_callback;
    timer.update_callback = update_callback;
    timer.loop = loop;
    return timer;
}

void AddTimer(float delay, std::function<void()> finish_callback, std::function<void(float)> update_callback, bool loop)
{
    timers.push_back(CreateTimer(delay, finish_callback, update_callback, loop));
}

bool UpdateTimer(Timer &timer, float delta_time)
{
    timer.expiration_time -= microseconds(static_cast<int>(delta_time * 1000000));
    if (timer.expiration_time <= microseconds(0))
    {
        if (timer.finish_callback)
        {
            timer.finish_callback();
        }
        if (timer.loop)
        {
            while ((timer.expiration_time += microseconds(static_cast<int>(timer.delay * 1000000))) <= microseconds(0))
            {
                timer.finish_callback();
            }
        }
        else
        {
            return true;
        }
    }
    else
    {
        if (timer.update_callback)
        {
            timer.update_callback(delta_time);
        }
    }
    return false;
}

void Update(float delta_time)
{
    auto size = timers.size();
    for (size_t idx = 0u; idx < size; ++idx)
    {
        auto &timer = timers[idx];
        bool finished = UpdateTimer(timer, delta_time);
        if (finished)
        {
            ids_to_remove.push_back(timer.id);
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

} // namespace Timer