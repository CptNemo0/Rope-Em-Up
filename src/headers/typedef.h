#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <memory>
#include <string>
#include <iostream>
#include <chrono>

#include "nlohmann/json.hpp"

template <typename T>
using s_ptr = std::shared_ptr<T>;
template <typename T>
using w_ptr = std::weak_ptr<T>;
using std::make_shared;

using std::string;
using std::cout;
using std::endl;
using std::cerr;

using std::chrono::steady_clock;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::seconds;

using json = nlohmann::json;

const float kQuadVertices[] =
{
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};

enum DROP_TYPE
{
    EXP,
    HP,
    SPELL
};

enum SPELLS
{
    NOT_A_SPELL,
    SKULL_MINION
};

#define MAX_BONES 200

#endif // !TYPEDEF_H