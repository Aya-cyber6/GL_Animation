
#ifndef OGLDEV_BASIC_GLFW_CAMERA_H
#define OGLDEV_BASIC_GLFW_CAMERA_H

#include "ogldev_math_3d.h"
#include "BaseCamera.h"

class BasicCamera : public CameraAPI
{
public:

    BasicCamera() {}

    BasicCamera(int WindowWidth, int WindowHeight);

    BasicCamera(const PersProjInfo& persProjInfo, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

    BasicCamera(const OrthoProjInfo& orthoProjInfo, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

    void InitCamera(const PersProjInfo& persProjInfo, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

    void InitCamera(const OrthoProjInfo& orthoProjInfo, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

    void SetPosition(float x, float y, float z);

    void SetPosition(const Vector3f& pos);

    void SetTarget(float x, float y, float z);

    void SetTarget(const Vector3f& target);

    void SetUp(float x, float y, float z) { m_up.x = x; m_up.y = y; m_up.z = z; }

    bool OnKeyboard(int key);
    bool fpsOnKeyboard(int Key);
    Vector3f m_right;  // Right vector

    void OnMouse(int x, int y);

    // Update the mouse position without moving the camera
    void UpdateMousePosSilent(int x, int y);

    void OnRender();

    Matrix4f GetMatrix() const;

    const Vector3f GetPos() const { return m_pos; }

    const Vector3f& GetTarget() const { return m_target; }

    const Vector3f& GetUp() const { return m_up; }

    const Matrix4f GetProjectionMat() const { return m_projection; }

    const PersProjInfo& GetPersProjInfo() const { return m_persProjInfo; }

    Matrix4f GetViewProjMatrix() const;

    Matrix4f GetViewMatrix() const { return GetMatrix(); }

    Matrix4f GetViewportMatrix() const;

    void Print() const { printf("Pos "); m_pos.Print(); printf("Target "); m_target.Print(); }

    void SetSpeed(float Speed);

    void SetName(const std::string& Name) { m_name = Name; }

    const std::string& GetName() const { return m_name; }

    Matrix4f CreatePerspectiveProjection(float fovDegrees, int width, int height, float nearPlane, float farPlane) const;


private:

    void InitInternal();
    void Update();

    std::string m_name;

    Vector3f m_pos;
    Vector3f m_target;
    Vector3f m_up;

    float m_speed = 0.2f;
    int m_windowWidth = 0;
    int m_windowHeight = 0;

    float m_AngleH = 0.0f;
    float m_AngleV = 0.0f;

    bool m_OnUpperEdge = false;
    bool m_OnLowerEdge = false;
    bool m_OnLeftEdge = false;
    bool m_OnRightEdge = false;

    Vector2i m_mousePos = Vector2i(0, 0);

    PersProjInfo m_persProjInfo;
    Matrix4f m_projection;
};

#endif
