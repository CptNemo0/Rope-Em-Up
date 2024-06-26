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
    T GetLast();
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

template <typename T>
inline T Shuffler<T>::GetLast()
{
    if (index_ == 0)
    {
        return data_[data_.size() - 1];
    }
    return data_[index_ - 1];
}

#endif // SHUFFLER_H