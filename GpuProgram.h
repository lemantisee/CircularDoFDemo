#pragma once

#include <string>

class GpuProgram
{
public:
    GpuProgram();
    ~GpuProgram();
    bool build(const std::string &shaderFilepath);

    void bind();
    void unbind();

private:
    enum ShaderType {
        VertexShader,
        FragmentShader
    };

    std::string readShaderFile(const std::string &shaderFilepath) const;
    std::string findShaderSource(const std::string &source, ShaderType type) const;

    uint32_t mId = 0;
};
