#include "ShaderProgram.h"
#include "constants.h"
#include "logging.h"
#include "utils.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <iostream>

ShaderProgram::ShaderProgram(const std::string& programName)
    : m_programName(programName)
{
}

bool
ShaderProgram::initializeProgram(const std::string& vertPath, const std::string& fragPath)
{
    std::string vstext = ReadFile(vertPath);
    std::string fstext = ReadFile(fragPath);

    if(vstext.empty()){
        LOG_E(LOGGING_NAME, m_programName + " Failed to read vertex shader file");
        return false;
    }

    if(fstext.empty()){
        LOG_E(LOGGING_NAME, m_programName + " Failed to read fragment shader file");
        return false;
    }

    GLint success;
    char infoLog[512];

    const char *vertex_shader = vstext.c_str();
    const char *fragment_shader = fstext.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        LOG_E(LOGGING_NAME, m_programName + " Vertex shader error: " + infoLog);
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        LOG_E(LOGGING_NAME, m_programName + " Fragment shader error: " + infoLog);
    }

    m_programId = glCreateProgram();
    glAttachShader(m_programId, vs);
    glAttachShader(m_programId, fs);
    glLinkProgram(m_programId);
    glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(m_programId, 512, NULL, infoLog);
        LOG_E(LOGGING_NAME, m_programName + " Link error: " + infoLog);
    }

    return true;
}

GLuint
ShaderProgram::getLoc(const std::string& name)
{
    if(name.empty()){
        LOG_E(LOGGING_NAME, m_programName + " Can not get uniform location, name is empty");
        return -1;
    }

    auto cacheIt = m_uniformsCache.find(name);
    if(cacheIt != m_uniformsCache.end()){
        return m_uniformsCache.at(name);
    }

    GLuint locationId = glGetUniformLocation(m_programId, name.c_str());
    m_uniformsCache[name] = locationId;

    return locationId;
}

void
ShaderProgram::setFloat(const std::string& name, float value)
{
    GLuint location = getLoc(name);
    
    if(location == -1){
        LOG_E(LOGGING_NAME, m_programName + " Can't set float, failed to get location " + name);
        return;
    }

    glUniform1f(location, value);
}

void
ShaderProgram::setInt(const std::string& name, int value)
{
    GLuint location = getLoc(name);
    
    if(location == -1){
        LOG_E(LOGGING_NAME, m_programName + " Can't set int, failed to get location " + name);
        return;
    }
    
    glUniform1i(location, value);
}

void
ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat)
{
    GLuint location = getLoc(name);
    
    if(location == -1){
        LOG_E(LOGGING_NAME, m_programName + " Can't set mat4, failed to get location " + name);
        return;
    }

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void
ShaderProgram::setVec3(const std::string& name, const glm::vec3& vec3)
{
    GLuint location = getLoc(name);

    if(location == -1){
        LOG_E(LOGGING_NAME, m_programName + " Can't set vec3, failed to get location " + name);
        return;
    }

    glUniform3fv(location, 1, glm::value_ptr(vec3));
}

void
ShaderProgram::setDefaultUniforms(ShaderProgram& shaderProgram)
{
    shaderProgram.use();
    shaderProgram.setInt("cubemap", 1);
    
    shaderProgram.setInt("directionalLightSM1", DIRECTIONAL_LIGHT_MAP_1);
    
    shaderProgram.setInt("spotLightSM[0]", SPOT_LIGHT_MAP_1);
    shaderProgram.setInt("spotLightSM[1]", SPOT_LIGHT_MAP_2);
    shaderProgram.setInt("spotLightSM[2]", SPOT_LIGHT_MAP_3);
}