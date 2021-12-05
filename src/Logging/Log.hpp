#pragma once

#define GL_LOG_FILE "gl.log"
void gl_log_reset();
void gl_log(const char* message, ...);
void gl_log_error(const char* message, ...);