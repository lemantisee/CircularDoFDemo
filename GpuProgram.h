#pragma once

#include <string>
#include <map>

class GpuProgram
{
public:
    GpuProgram();
    ~GpuProgram();
    bool build(const std::string &shaderFilepath);

    void bind();
    void unbind();

    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);

private:
    enum ShaderType {
        VertexShader,
        FragmentShader
    };

    std::string readShaderFile(const std::string &shaderFilepath) const;
    std::string findShaderSource(const std::string &source, ShaderType type) const;
    int findLocation(const std::string &name);

    uint32_t mId = 0;
    std::map<std::string, int> mUniformLocations;
};
