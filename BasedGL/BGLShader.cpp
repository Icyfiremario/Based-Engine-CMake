#include "BGLShader.h"

BGLShader::BGLShader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        PLOGF << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ";
        throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ!");
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        PLOGF << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << infoLog;
        throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED!");
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        PLOGF << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " << infoLog;
        throw std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED!");
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        PLOGF << "ERROR::SHADER::PROGRAM::LINKING_FAILED: " << infoLog;
        throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED!");
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
