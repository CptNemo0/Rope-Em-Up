#ifndef GLOBAL_H
#define GLOBAL_H

#include "Camera.h"
#include "typedef.h"

class Global
{
private:
    Global() {}
    ~Global() {}
public:
    static Global *i_;
    static void Initialize()
    {
        if (i_ == nullptr)
        {
            i_ = new Global();
        }
    }
    static void Destroy()
    {
        if (i_ != nullptr)
        {
            delete i_;
            i_ = nullptr;
        }
    }

    s_ptr<llr::Camera> active_camera_ = nullptr;
};

#endif // !GLOBAL_H