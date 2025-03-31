#pragma once

// STD
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// OpenGL
#include <GL/glew.h>

/// @brief OpenGL shader class
/// @todo Rewrite to load pre-compiled spv rather than shader source
class BGLShader
{
    public:

        /// @brief Shader ID
        unsigned int ID;

        /// @brief Loads and compiles the shader from the given file paths.
        /// @param vertexPath Vertex shader path
        /// @param fragmentPath Fragment shader path
        BGLShader(const char* vertexPath, const char* fragmentPath);

        /// @brief Tells glew to use the shader program
        void use();

        /// @brief Sets the shader uniform bool
        /// @param name Uniform name
        /// @param value Value to set to
        void setBool(const std::string &name, bool value) const;
        /// @brief Sets the shader uniform int
        /// @param name Uniform name
        /// @param value Value to set to
        void setInt(const std::string &name, int value) const;
        /// @brief Sets the shader uniform float
        /// @param name Uniform name
        /// @param value Value to set to
        void setFloat(const std::string &name, float value) const;
};