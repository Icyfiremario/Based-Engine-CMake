#ifndef BGLSHADER_H
#define BGLSHADER_H

// STD
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// OpenGL
#include <glad/glad.h>

// Plog
#include <plog/Log.h>

class BGLShader
{
public:

    unsigned int ID;

    BGLShader(const char* vertexPath, const char* fragmentPath);

    void use() const { glUseProgram(ID); }

    void setBool(const std::string& name, const bool value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value)); }
    void setInt(const std::string& name, const int value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }
    void setFloat(const std::string& name, const float value) const { glUniform1f(glGetUniformLocation(ID, name.c_str()), value); }
};


#endif // BGLSHADER_H
