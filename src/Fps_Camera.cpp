#include "FpsCamera.h"
#include <ogldev_glfw.h>


Camera::Camera(const Vector3f& pos, const Vector3f& target, const Vector3f& up, int width, int height) {
    m_pos = pos;
    m_target = target;
    m_up = up;
    m_windowWidth = width;
    m_windowHeight = height;

    m_AngleH = 0.0f;
    m_AngleV = 0.0f;


    m_mousePos = Vector2i(width / 2, height / 2);

    UpdateProjection(60.0f, 0.1f, 100.0f); // Default FOV, near, far
}

const Vector3f Camera::GetPos() const {
    return m_pos;
}

Matrix4f Camera::GetViewMatrix() const {
    Matrix4f view;
    view.InitCameraTransform(m_pos, m_target, m_up);
    return view;
}

Matrix4f Camera::GetMatrix() const {
    return m_projection * GetViewMatrix();
}

Matrix4f Camera::CreatePerspectiveProjection(float fovDegrees, int width, int height, float nearPlane, float farPlane) const {
    Matrix4f proj;
    PersProjInfo p;
    p.FOV = fovDegrees;
    p.Width = width;
    p.Height = height;
    p.zNear = nearPlane;
    p.zFar = farPlane;
    proj.InitPersProjTransform(p);
    return proj;
}

const Matrix4f Camera::GetProjectionMat() const {
    return m_projection;
}

void Camera::UpdateProjection(float fovDegrees, float nearPlane, float farPlane) {
    m_persProjInfo.FOV = fovDegrees;
    m_persProjInfo.Width = m_windowWidth;
    m_persProjInfo.Height = m_windowHeight;
    m_persProjInfo.zNear = nearPlane;
    m_persProjInfo.zFar = farPlane;

    m_projection.InitPersProjTransform(m_persProjInfo);
}

void Camera::fpsOnMouse(int x, int y)
{
    static bool firstMouse = true;
    if (firstMouse) {
        m_mousePos.x = x;
        m_mousePos.y = y;
        firstMouse = false;
    }

    int deltaX = x - m_mousePos.x;
    int deltaY = m_mousePos.y - y; // Invert y-axis for natural FPS feel

    m_mousePos.x = x;
    m_mousePos.y = y;

    float sensitivity = 0.1f;
    float yawChange = deltaX * sensitivity;
    float pitchChange = deltaY * sensitivity;

    m_AngleH += yawChange;
    m_AngleV += pitchChange;

    // Clamp vertical angle to avoid flipping
    if (m_AngleV > 89.0f)  m_AngleV = 89.0f;
    if (m_AngleV < -89.0f) m_AngleV = -89.0f;

    Update();
}

void Camera::Update() {
    // Clamp vertical angle to avoid flipping
    const float MaxVerticalAngle = 89.0f;
    if (m_AngleV > MaxVerticalAngle)  m_AngleV = MaxVerticalAngle;
    if (m_AngleV < -MaxVerticalAngle) m_AngleV = -MaxVerticalAngle;

    // Convert degrees to radians
    const float pitch = m_AngleV * (3.14159265f / 180.0f);
    const float yaw = m_AngleH * (3.14159265f / 180.0f);

    // Calculate forward (target) vector
    m_target.x = cosf(pitch) * sinf(yaw);
    m_target.y = sinf(pitch);
    m_target.z = cosf(pitch) * cosf(yaw);
    m_target.Normalize();

    // Global up vector
    Vector3f globalUp(0.0f, 1.0f, 0.0f);

    // Calculate right and up vectors using your Cross()

    m_right = m_target.Cross(globalUp);
    m_right.Normalize();

    m_up = m_right.Cross(m_target);
    m_up.Normalize();
}

bool Camera::OnKeyboard(int Key)
{
    bool CameraChangedPos = false;

    switch (Key) {

    case GLFW_KEY_W:
        m_pos += m_target * m_speed;
        CameraChangedPos = true;
        break;

    case GLFW_KEY_S:
        m_pos -= m_target * m_speed;
        CameraChangedPos = true;
        break;

    case GLFW_KEY_A:
    {
        Vector3f Left = m_right * -1.0f;  // Left is negative right
        Left.Normalize();
        Left *= m_speed;
        m_pos += Left;
        CameraChangedPos = true;
    }
    break;

    case GLFW_KEY_D:
    {
        Vector3f Right = m_right;
        Right.Normalize();
        Right *= m_speed;
        m_pos += Right;
        CameraChangedPos = true;
    }
    break;

    case GLFW_KEY_UP:
        m_AngleV += m_speed;
        Update();
        break;

    case GLFW_KEY_DOWN:
        m_AngleV -= m_speed;
        Update();
        break;

    case GLFW_KEY_LEFT:
        m_AngleH -= m_speed;
        Update();
        break;

    case GLFW_KEY_RIGHT:
        m_AngleH += m_speed;
        Update();
        break;

    case GLFW_KEY_KP_ADD:
        m_speed += 0.1f;
        printf("Speed changed to %f\n", m_speed);
        break;

    case GLFW_KEY_KP_SUBTRACT:
        m_speed -= 0.1f;
        if (m_speed < 0.1f) m_speed = 0.1f;
        printf("Speed changed to %f\n", m_speed);
        break;

    case GLFW_KEY_C:
        printf("Camera pos: "); m_pos.Print(); printf("\n");
        printf("Camera target: "); m_target.Print(); printf("\n");
        break;
    }

    return CameraChangedPos;
}


Matrix4f Camera::GetViewportMatrix() const
{
    return LookAt(m_pos, m_pos + m_target, m_up);
}


Matrix4f Camera::LookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up) const
{
    Vector3f f = (center - eye).Normalize(); // Forward
    Vector3f r = f.Cross(up).Normalize();    // Right
    Vector3f u = r.Cross(f);                 // Recomputed Up

    Matrix4f result;
    result.InitIdentity();

    result.m[0][0] = r.x;
    result.m[0][1] = r.y;
    result.m[0][2] = r.z;
    result.m[0][3] = -r.Dot(eye);

    result.m[1][0] = u.x;
    result.m[1][1] = u.y;
    result.m[1][2] = u.z;
    result.m[1][3] = -u.Dot(eye);

    result.m[2][0] = -f.x;
    result.m[2][1] = -f.y;
    result.m[2][2] = -f.z;
    result.m[2][3] = f.Dot(eye); // Usually -f.Dot(eye), adjust if needed

    result.m[3][0] = 0.0f;
    result.m[3][1] = 0.0f;
    result.m[3][2] = 0.0f;
    result.m[3][3] = 1.0f;

    return result;
}
