#pragma once

#include <array>

// default radius of 1
class Cube
{
public:
    Cube() {};

    static constexpr const std::array<float, 24> vertices = { 
    -0.5f, -0.5f, 0.5f, // p0
     0.5f, -0.5f, 0.5f, // p1
     0.5f,  0.5f, 0.5f, // p2
    -0.5f,  0.5f, 0.5f, // p3
    // Back face
    -0.5f, -0.5f, -0.5f, //p4
     0.5f, -0.5f, -0.5f, //p5
     0.5f,  0.5f, -0.5f, //p6
    -0.5f,  0.5f, -0.5f  //p7
    };

    static constexpr const std::array<int,32> indices = {
        0,1,  1,2,  2,3,  3,0,
        1,5,  5,6,  6,2,  2,1,
        5,6,  6,7,  7,4,  4,5,
        7,4,  4,0,  0,3,  3,7
    };
};
