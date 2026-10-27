#pragma once

#include <GL/glew.h>
#include <string>

constexpr const char* const LOG_FILE = "proiect.log";

void initLogger(const std::string& logFile);
void LOG_I(const std::string& logAuthor, const std::string& logText);
void LOG_E(const std::string& logAuthor, const std::string& logText);
void LOG_W(const std::string& logAuthor, const std::string& logText);
void LOG_D(const std::string& logAuthor, const std::string& logText);

void LOG_SHADER_D(const std::string& logAuthor, GLuint shaderObj);
void LOG_PROGRAM_D(const std::string& logAuthor, GLuint programObj);