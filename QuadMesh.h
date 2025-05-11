#pragma once

#include <vector>

class QuadMesh
{
public:
    QuadMesh();
    ~QuadMesh();

    bool init();

    void draw();

private:
    uint32_t mVbo = 0;
    uint32_t mVao = 0;
    uint32_t mEbo = 0;
};
