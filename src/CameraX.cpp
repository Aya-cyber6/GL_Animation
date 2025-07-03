#include "CameraX.h"
#include <cmath>
#include <iostream>
#include <GLFW/glfw3.h>


CameraX::CameraX(Vector3f position, Vector3f target, float yaw, float pitch, int WindowWidth, int WindowHeight)
    : Front(Vector3f(0.0f, 0.0f, -1.0f)),
    MouseSensitivity(SENSITIVITY),
    m_pos(position),
    m_target(m_target),
    Yaw(yaw),
    Pitch(pitch),
    m_windowWidth(WindowWidth),
    m_windowHeight(WindowHeight)
{
    Update();
}

Matrix4f CameraX::GetMatrix() const {
    Matrix4f view;
    view.InitCameraTransform(m_pos, m_target, m_up);
    return view;
}

Matrix4f CameraX::GetProjectionMat(float aspectRatio) const {
    PersProjInfo proj;
    proj.FOV = 45.0f;
    proj.zNear = 0.1f;
    proj.zFar = 100.0f;
    proj.Width = (float)m_windowWidth;
    proj.Height = (float)m_windowHeight;

    Matrix4f projection;
    projection.InitPersProjTransform(proj);
    return projection;
}

void CameraX::SetFirstPerson(const Vector3f& characterPos, const Vector3f& characterDir) {
    m_pos = characterPos + Vector3f(0.0f, 1.7f, 0.0f);
    m_target = m_pos + characterDir;
    m_up = Vector3f(0.0f, 1.0f, 0.0f);
}

void CameraX::SetFreeFly(const Vector3f& pos, const Vector3f& front) {
    m_pos = pos;
    m_target = front;
    m_up = Vector3f(0.0f, 1.0f, 0.0f);
}

void CameraX::SetCinematic(float time, const Vector3f& characterPos, const Vector3f& characterForward) {
    // Normalize the direction the character is facing
    Vector3f forward = characterForward;
    if (forward.Length() < 0.001f) {
        forward = Vector3f(0.0f, 0.0f, -1.0f);  // fallback
    }
    else {
        forward.Normalize();
    }

    // Compute a left vector from the forward direction
    Vector3f up(0.0f, 1.0f, 0.0f);
    Vector3f left = up.Cross(forward).Normalize(); // left = up × forward

    // Offset camera: back-left and up
    float backOffset = 5.0f;
    float leftOffset = 20.0f;
    float height = 4.0f;

    m_pos = characterPos - forward * backOffset + left * leftOffset + Vector3f(0.0f, height, 0.0f);
    m_target = characterPos + m_pos * -1.0f;  // look at character's head height
    m_up = up;
}


Matrix4f CameraX::GetViewportMatrix() const
{
    Matrix4f View = GetMatrix();
    Matrix4f Projection = GetProjectionMat();
    Matrix4f ViewProj = Projection * View;
    return ViewProj;
}

bool CameraX::OnKeyboard(int Key)
{
    bool CameraChangedPos = false;

    switch (Key) {

    case GLFW_KEY_W:
        m_pos += (m_target * m_speed);
        CameraChangedPos = true;
        break;

    case GLFW_KEY_S:
        m_pos -= (m_target * m_speed);
        CameraChangedPos = true;
        break;

    case GLFW_KEY_A:
    {
        Vector3f Left = m_up.Cross(m_target);  // Assuming your up and target are correct for cross
        Left.Normalize();
        Left *= m_speed;
        m_pos += Left;
        CameraChangedPos = true;
    }
    break;

    case GLFW_KEY_D:
    {
        Vector3f Right = m_target.Cross(m_up);
        Right.Normalize();
        Right *= m_speed;
        m_pos += Right;
        CameraChangedPos = true;
    }
    break;

    case GLFW_KEY_UP:
        m_AngleV += m_speed;
        Update(); // Your update function recalculates view vectors
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


    case GLFW_KEY_PAGE_UP:
        m_pos.y += m_speed;
        CameraChangedPos = true;
        break;

    case GLFW_KEY_PAGE_DOWN:
        m_pos.y -= m_speed;
        CameraChangedPos = true;
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

void CameraX::OnMouse(float x, float y, GLboolean constrainPitch)
{
    if (!m_mousePressed) return;  // Only rotate if mouse is pressed

    int deltaX = x - m_mousePos.x;
    int deltaY = y - m_mousePos.y;

    m_mousePos.x = x;
    m_mousePos.y = y;

    m_AngleH += deltaX * 0.05f;
    m_AngleV += deltaY * 0.05f;

    Update();  // Recalculate camera matrices with new angles
}

void CameraX::SetMousePos(int x, int y) {
    m_mousePos.x = x;
    m_mousePos.y = y;
}

void CameraX::Update()
{
    Vector3f direction;

    float angleH_rad = ToRadian(m_AngleH);
    float angleV_rad = ToRadian(m_AngleV);

    direction.x = cosf(angleV_rad) * sinf(angleH_rad);
    direction.y = sinf(angleV_rad);
    direction.z = cosf(angleV_rad) * cosf(angleH_rad);
    direction.Normalize();

    m_target = direction;

    Vector3f right = Vector3f(0.0f, 1.0f, 0.0f).Cross(m_target);
    right.Normalize();

    m_up = m_target.Cross(right);
    m_up.Normalize();
}