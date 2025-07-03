#pragma once

#include "ogldev_math_3d.h"

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


class CameraX {
public:
    float Yaw;
    float Pitch;

    CameraX() {}

        CameraX(
            Vector3f position = Vector3f(0.0f, 0.0f, 0.0f),
            Vector3f up = Vector3f(0.0f, 1.0f, 0.0f),
            float yaw = YAW,
            float pitch = PITCH,
            int WindowWidth = WINDOW_WIDTH,
            int WindowHeight = WINDOW_HEIGHT );


    Vector3f m_pos;
    Vector3f m_target;
    Vector3f m_up;
    Vector3f Front;
    Vector3f WorldUp;
    Vector3f Right;

    float m_speed = 0.2f;
    int m_windowWidth = 0;
    int m_windowHeight = 0;
    Vector2i m_mousePos = Vector2i(0, 0);
    float MouseSensitivity;

    float m_AngleH = 0.0f;
    float m_AngleV = 0.0f;
    bool m_mousePressed = false;

    Matrix4f GetViewportMatrix() const;

    const Vector3f GetPos() const { return m_pos; }
    Matrix4f GetMatrix() const;
    Matrix4f GetProjectionMat(float aspectRatio = (float)WINDOW_WIDTH / WINDOW_HEIGHT) const;
    Matrix4f LookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up) const;

    Vector3f GetTarget() const { return m_pos; }
    void SetFirstPerson(const Vector3f& characterPos, const Vector3f& characterDir);
    void SetFreeFly(const Vector3f& pos, const Vector3f& front);
    void SetCinematic(float time, const Vector3f& characterPos, const Vector3f& characterForward);
    bool OnKeyboard(int Key);
    void SetMousePos(int x, int y);
    void OnMouse(float xoffset, float yoffset, GLboolean constrainPitch);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch);
private:
    void Update();
    void updateCameraVectors();
};
