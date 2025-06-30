#pragma once

#include "BaseCamera.h"
#include "ogldev_math_3d.h"

class Camera : public CameraAPI {
public:
    Camera(const Vector3f& pos, const Vector3f& target, const Vector3f& up, int width, int height);

    virtual const Vector3f GetPos() const override;
    virtual Matrix4f GetViewMatrix() const override;
    virtual Matrix4f GetMatrix() const override;
    virtual Matrix4f CreatePerspectiveProjection(float fovDegrees, int width, int height, float nearPlane, float farPlane) const override;
    virtual const Matrix4f GetProjectionMat() const override;

    void fpsOnMouse(int x, int y);

    bool OnKeyboard(int key);

    Matrix4f GetViewportMatrix() const;

    Matrix4f LookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up) const;

    void UpdateProjection(float fovDegrees, float nearPlane, float farPlane);
    Vector3f m_right;  // Right vector

private:
    Vector2i m_mousePos;

    float m_speed = 0.2f;
    float m_AngleH; // Yaw
    float m_AngleV; // Pitch
    int m_windowWidth = 0;
    int m_windowHeight = 0;

    Vector3f m_pos;    // Position
    Vector3f m_target; // Direction (forward)
    Vector3f m_up;     // Up vector

    PersProjInfo m_persProjInfo;
    Matrix4f m_projection;
    void Update();

};
