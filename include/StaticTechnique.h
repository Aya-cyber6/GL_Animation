//#ifndef STATIC_TECHNIQUE_H
//#define STATIC_TECHNIQUE_H
//
//#include <string>
//#include <ogldev_math_3d.h>
//
//class StaticTechnique
//{
//public:
//    StaticTechnique();
//    ~StaticTechnique();
//
//    // Initialize the technique with vertex and fragment shader files
//    bool Init(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
//
//    // Activate the shader program
//    void Enable() const;
//
//    // Set uniforms
//    void SetModel(const Matrix4f& model);
//    void SetView(const Matrix4f& view);
//    void SetProjection(const Matrix4f& projection);
//
//    void SetLightDirection(const Vector3f& direction);
//    void SetLightAmbient(const Vector3f& ambient);
//    void SetLightDiffuse(const Vector3f& diffuse);
//    void SetLightSpecular(const Vector3f& specular);
//
//    void SetViewPos(const Vector3f& viewPos);
//
//    void setVec3(const std::string& name, const Vector3f value) const;
//
//    void SetTextureUnit(unsigned int textureUnit);
//    unsigned int GetProgram() { return m_program; }
//
//private:
//    bool CompileShader(const char* source, unsigned int shaderType, unsigned int& shader);
//    bool LinkProgram(unsigned int vertexShader, unsigned int fragmentShader);
//
//private:
//    unsigned int m_program;
//
//    // Uniform locations
//    int m_modelLoc;
//    int m_viewLoc;
//    int m_projectionLoc;
//
//    int m_lightDirLoc;
//    int m_lightAmbientLoc;
//    int m_lightDiffuseLoc;
//    int m_lightSpecularLoc;
//
//    int m_viewPosLoc;
//
//    int m_textureSamplerLoc;
//};
//
//#endif // STATIC_TECHNIQUE_H
