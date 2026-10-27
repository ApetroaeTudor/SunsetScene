#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <glm/ext/matrix_transform.hpp>

class ShaderProgram
{
private:
    static constexpr const char* const LOGGING_NAME = "shaders::ShaderProgram";

private:
    typedef std::unordered_map<std::string, GLuint> UniformsCache;

public:
    ShaderProgram(const std::string& programName = "undefined");

    bool initializeProgram(const std::string& vertPath, const std::string& fragPath);
    ~ShaderProgram() { glDeleteProgram(m_programId); }

    void use() { glUseProgram(m_programId); }
    GLuint shaderId() { return m_programId; }
    
    void setFloat(const std::string& name, float value);
    void setInt(const std::string& name, int value);
    void setMat4(const std::string& name, const glm::mat4& mat);
    void setVec3(const std::string& name, const glm::vec3& vec3);

    static void setDefaultUniforms(ShaderProgram& shaderProgram);
    
private:
    GLuint getLoc(const std::string& name);

private:
    std::string   m_programName;
    GLuint        m_programId;
    UniformsCache m_uniformsCache;
};
