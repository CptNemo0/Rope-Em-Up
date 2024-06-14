#ifndef SHUFFLER_H
#define SHUFFLER_H

#include <vector>
#include <algorithm>

#include "Random.h"

template <typename T>
class Shuffler
{
private:
    std::vector<T> data_;
    size_t index_ = 0;
public:
    Shuffler() = default;
    Shuffler(const std::vector<T> &data) : data_(data)
    {
        std::shuffle(data_.begin(), data_.end(), random::get_random_device());
    }

    void SetData(const std::vector<T> &data)
    {
        data_ = data;
        std::shuffle(data_.begin(), data_.end(), random::get_random_device());
    }
    T Pop();
};

template <typename T>
inline T Shuffler<T>::Pop()
{
    if (index_ >= data_.size())
    {
        index_ = 0;
        std::shuffle(data_.begin(), data_.end(), random::get_random_device());
    }

    return data_[index_++];
}

#endif // SHUFFLER_H