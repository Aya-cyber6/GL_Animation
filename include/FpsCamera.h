#pragma once

#include <ogldev_math_3d.h>
#include "BaseCamera.h"

// Defines several possible options for CameraFPS movement
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default CameraFPS values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class CameraFPS : public CameraAPI
{
public:
    Vector3f Position;
    Vector3f Front;
    Vector3f Up;
    Vector3f Right;
    Vector3f WorldUp;

    // Euler Angles
    float Yaw;
    float Pitch;

    // CameraFPS options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Constructors
    CameraFPS(Vector3f position = Vector3f(0.0f, 0.0f, 0.0f),
        Vector3f up = Vector3f(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH);

    CameraFPS(float posX, float posY, float posZ,
        float upX, float upY, float upZ,
        float yaw, float pitch);

    // CameraAPI interface implementation
    const Vector3f GetPos() const override;
    Matrix4f GetViewportMatrix() const override;
    Matrix4f GetMatrix() const override;
    const Matrix4f GetProjectionMat() const override;

    // Original OpenGL-specific methods
    Matrix4f GetViewMatrix() const;
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    // Setters for projection parameters
    void SetProjectionParams(float fov, float aspectRatio, float nearPlane, float farPlane);
    void SetViewportSize(int width, int height);

private:
    // Projection parameters
    float m_fov = 45.0f;
    float m_aspectRatio = 16.0f / 9.0f;
    float m_nearPlane = 0.1f;
    float m_farPlane = 100.0f;
    int m_viewportWidth = 1024;
    int m_viewportHeight = 768;

    void updateCameraVectors();
};
