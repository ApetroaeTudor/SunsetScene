#include "logging.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <spdlog/logger.h>
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

void
initLogger(const std::string& logFile)
{
    try{
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/" + logFile, true);
        
        auto logger = std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list{console_sink, file_sink});

        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::debug);
        spdlog::flush_on(spdlog::level::trace);
        spdlog::info("Logger initialized!");   
    }
    catch(const spdlog::spdlog_ex& exception){
        std::cerr<<"Error in initializing logger";
    }
}

void
LOG_I(const std::string& logAuthor, const std::string& logText)
{
    spdlog::info("[" + logAuthor + "] " + logText);
}

void
LOG_E(const std::string& logAuthor, const std::string& logText)
{
    spdlog::error("[" + logAuthor + "] " + logText);
}

void
LOG_W(const std::string& logAuthor, const std::string& logText)
{
    spdlog::warn("[" + logAuthor + "] " + logText);
}

void
LOG_D(const std::string& logAuthor, const std::string& logText)
{
    spdlog::debug("[" + logAuthor + "] " + logText);
}

void
LOG_SHADER_D(const std::string& logAuthor, GLuint shaderObj)
{
    int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetShaderiv(shaderObj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(shaderObj, infologLength, &charsWritten, infoLog);
		LOG_D(logAuthor, infoLog);
        free(infoLog);
	}
}

void
LOG_PROGRAM_D(const std::string& logAuthor, GLuint programObj)
{
    int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetProgramiv(programObj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(programObj, infologLength, &charsWritten, infoLog);
		LOG_D(logAuthor, infoLog);
        free(infoLog);
	}
}