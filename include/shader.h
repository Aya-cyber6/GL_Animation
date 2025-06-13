#pragma once

#include <string>
#include "ogldev_math_3d.h" 

class Shader
{
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);
    void use() const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

    void setVec3(const std::string& name, const Vector3f& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;

    void setMat4(const std::string& name, const Matrix4f& mat) const;

private:
    void checkCompileErrors(GLuint shader, std::string type);
};
