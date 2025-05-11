#include "GpuProgram.h"

#include <fstream>
#include <glad.h>

#include "Logger.h"

GpuProgram::GpuProgram() {}

GpuProgram::~GpuProgram()
{
    if (mId) {
        glDeleteProgram(mId);
    }
}

bool GpuProgram::build(const std::string &shaderFilepath)
{
    std::string shaderSource = readShaderFile(shaderFilepath);
    if (shaderSource.empty()) {
        return false;
    }

    const std::string vertexShader = findShaderSource(shaderSource, VertexShader);
    if (vertexShader.empty()) {
        LOG_ERROR("Empty vertex shader");
        return false;
    }

    const char *vertexShaderStr = vertexShader.c_str();
    uint32_t vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexShaderStr, nullptr);
    glCompileShader(vertexShaderId);

    int success = 0;
    std::string shaderInfoLog;
    shaderInfoLog.resize(1024);
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShaderId, shaderInfoLog.size(), nullptr, shaderInfoLog.data());
        LOG_ERROR("Unable to compile vertex shader: %s", shaderInfoLog.c_str());
        return false;
    }


    const std::string fragmentShader = findShaderSource(shaderSource, FragmentShader);
    if (fragmentShader.empty()) {
        LOG_ERROR("Empty fragmnet shader");
        return false;
    }

    const char *fragShaderStr = fragmentShader.c_str();
    unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragShaderStr, nullptr);
    glCompileShader(fragmentShaderId);

    success = 0;
    shaderInfoLog.clear();
    shaderInfoLog.resize(1024);
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShaderId, shaderInfoLog.size(), nullptr, shaderInfoLog.data());
        LOG_ERROR("Unable to compile fragment shader: %s", shaderInfoLog.c_str());
        return false;
    }

    // link shaders
    success = 0;
    shaderInfoLog.clear();
    shaderInfoLog.resize(1024);

    mId = glCreateProgram();
    glAttachShader(mId, vertexShaderId);
    glAttachShader(mId, fragmentShaderId);
    glLinkProgram(mId);
    // check for linking errors
    glGetProgramiv(mId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(mId, shaderInfoLog.size(), nullptr, shaderInfoLog.data());
        LOG_ERROR("Unable to link shader: %s", shaderInfoLog.c_str());
        return false;
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return true;
}

void GpuProgram::bind() { glUseProgram(mId); }

void GpuProgram::unbind() { glUseProgram(0); }

std::string GpuProgram::readShaderFile(const std::string &shaderFilepath) const
{
    std::ifstream shaderFile;
    shaderFile.open(shaderFilepath);
    if (!shaderFile.is_open()) {
        LOG_ERROR("Unable to open shader: %s", shaderFilepath.c_str());
        return {};
    }

    std::string source;
    std::string line;
    while (std::getline(shaderFile, line)) {
        source += line + '\n';
    }
    shaderFile.close();

    return source;
}

std::string GpuProgram::findShaderSource(const std::string &source, ShaderType type) const
{
    std::string startTag;
    std::string endTag;

    switch (type) {
    case VertexShader:
        startTag = "@VERTEX_START@";
        endTag = "@VERTEX_END@";
        break;
    case FragmentShader: startTag = "@FRAG_START@"; endTag = "@FRAG_END@";
        break;
    default: LOG_ERROR("Unknown shader type"); return {};
    }

    auto startPos = source.find(startTag);
    auto endPos = source.find(endTag);

    if (startPos == std::string::npos || endPos == std::string::npos) {
        LOG_ERROR("Unable to find tags");
        return {};
    }

    auto shaderStart = startPos + startTag.size();

    return std::string(source, shaderStart, endPos - shaderStart);
}
