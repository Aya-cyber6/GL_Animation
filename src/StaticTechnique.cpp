//#include "StaticTechnique.h"
//#include <GL/glew.h>
//#include <iostream>
//#include <fstream>
//#include <sstream>
//
//// Removed glm include
//
//StaticTechnique::StaticTechnique()
//    : m_program(0),
//    m_modelLoc(-1), m_viewLoc(-1), m_projectionLoc(-1),
//    m_lightDirLoc(-1), m_lightAmbientLoc(-1), m_lightDiffuseLoc(-1), m_lightSpecularLoc(-1),
//    m_viewPosLoc(-1), m_textureSamplerLoc(-1)
//{
//}
//
//StaticTechnique::~StaticTechnique()
//{
//    if (m_program != 0)
//        glDeleteProgram(m_program);
//}
//
//bool StaticTechnique::Init(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
//{
//    std::ifstream vShaderFile(vertexShaderFile);
//    std::ifstream fShaderFile(fragmentShaderFile);
//
//    if (!vShaderFile.is_open() || !fShaderFile.is_open())
//    {
//        std::cerr << "Failed to open shader files: "
//            << vertexShaderFile << " or " << fragmentShaderFile << std::endl;
//        return false;
//    }
//
//    std::stringstream vShaderStream, fShaderStream;
//    vShaderStream << vShaderFile.rdbuf();
//    fShaderStream << fShaderFile.rdbuf();
//
//    std::string vertexCode = vShaderStream.str();
//    std::string fragmentCode = fShaderStream.str();
//
//    unsigned int vertexShader = 0;
//    unsigned int fragmentShader = 0;
//
//    if (!CompileShader(vertexCode.c_str(), GL_VERTEX_SHADER, vertexShader))
//        return false;
//    if (!CompileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER, fragmentShader))
//        return false;
//
//    if (!LinkProgram(vertexShader, fragmentShader))
//        return false;
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    m_modelLoc = glGetUniformLocation(m_program, "model");
//    m_viewLoc = glGetUniformLocation(m_program, "view");
//    m_projectionLoc = glGetUniformLocation(m_program, "projection");
//
//    m_lightDirLoc = glGetUniformLocation(m_program, "light.direction");
//    m_lightAmbientLoc = glGetUniformLocation(m_program, "light.ambient");
//    m_lightDiffuseLoc = glGetUniformLocation(m_program, "light.diffuse");
//    m_lightSpecularLoc = glGetUniformLocation(m_program, "light.specular");
//
//    m_viewPosLoc = glGetUniformLocation(m_program, "viewPos");
//
//    m_textureSamplerLoc = glGetUniformLocation(m_program, "texture_diffuse1");
//
//    if (m_modelLoc == -1 || m_viewLoc == -1 || m_projectionLoc == -1 ||
//        m_lightDirLoc == -1 || m_lightAmbientLoc == -1 || m_lightDiffuseLoc == -1 ||
//        m_lightSpecularLoc == -1 || m_viewPosLoc == -1 || m_textureSamplerLoc == -1)
//    {
//        std::cerr << "Failed to get uniform locations from shader program" << std::endl;
//        return false;
//    }
//
//    return true;
//}
//
//bool StaticTechnique::CompileShader(const char* source, unsigned int shaderType, unsigned int& shader)
//{
//    shader = glCreateShader(shaderType);
//    glShaderSource(shader, 1, &source, nullptr);
//    glCompileShader(shader);
//
//    int success;
//    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        char infoLog[512];
//        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
//        std::cerr << (shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
//            << " shader compilation error:\n"
//            << infoLog << std::endl;
//        return false;
//    }
//    return true;
//}
//
//bool StaticTechnique::LinkProgram(unsigned int vertexShader, unsigned int fragmentShader)
//{
//    m_program = glCreateProgram();
//    glAttachShader(m_program, vertexShader);
//    glAttachShader(m_program, fragmentShader);
//    glLinkProgram(m_program);
//
//    int success;
//    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
//    if (!success)
//    {
//        char infoLog[512];
//        glGetProgramInfoLog(m_program, 512, nullptr, infoLog);
//        std::cerr << "Shader program linking error:\n"
//            << infoLog << std::endl;
//        return false;
//    }
//    return true;
//}
//
//void StaticTechnique::Enable() const
//{
//    glUseProgram(m_program);
//}
//
//void StaticTechnique::setVec3(const std::string& name, const Vector3f value) const
//{
//    glUniform3fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
//}
//
//// Now use Matrix4f and Vector3f instead of glm
//
//void StaticTechnique::SetModel(const Matrix4f& model)
//{
//    glUniformMatrix4fv(m_modelLoc, 1, GL_FALSE, model.Get());
//}
//
//void StaticTechnique::SetView(const Matrix4f& view)
//{
//    glUniformMatrix4fv(m_viewLoc, 1, GL_FALSE, view.Get());
//}
//
//void StaticTechnique::SetProjection(const Matrix4f& projection)
//{
//    glUniformMatrix4fv(m_projectionLoc, 1, GL_FALSE, projection.Get());
//}
//
//void StaticTechnique::SetLightDirection(const Vector3f& direction)
//{
//    glUniform3fv(m_lightDirLoc, 1, direction.GetPtr());
//}
//
//void StaticTechnique::SetLightAmbient(const Vector3f& ambient)
//{
//    glUniform3fv(m_lightAmbientLoc, 1, ambient.GetPtr());
//}
//
//void StaticTechnique::SetLightDiffuse(const Vector3f& diffuse)
//{
//    glUniform3fv(m_lightDiffuseLoc, 1, diffuse.GetPtr());
//}
//
//void StaticTechnique::SetLightSpecular(const Vector3f& specular)
//{
//    glUniform3fv(m_lightSpecularLoc, 1, specular.GetPtr());
//}
//
//void StaticTechnique::SetViewPos(const Vector3f& viewPos)
//{
//    glUniform3fv(m_viewPosLoc, 1, viewPos.GetPtr());
//}
//
//void StaticTechnique::SetTextureUnit(unsigned int TextureUnit)
//{
//    glUniform1i(m_textureSamplerLoc, TextureUnit);
//}
