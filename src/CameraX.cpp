#include "CameraX.h"
#include <cmath>
#include <iostream>
#include <GLFW/glfw3.h>


CameraX::CameraX(Vector3f position, Vector3f up, float yaw, float pitch, int WindowWidth, int WindowHeight)
    : Front(Vector3f(0.0f, 0.0f, -1.0f)),
    MouseSensitivity(SENSITIVITY),
    m_pos(position),
    WorldUp(up),
    Yaw(yaw),
    Pitch(pitch),
    m_windowWidth(WindowWidth),
    m_windowHeight(WindowHeight)
{
    Update();
}



//// constructor with vectors
//CameraX::CameraX(Vector3f position = Vector3f(0.0f, 0.0f, 0.0f), Vector3f up = Vector3f(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(Vector3f(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
//{
//    m_pos = position;
//    WorldUp = up;
//    Yaw = yaw;
//    Pitch = pitch;
//    updateCameraVectors();
//}
//
//CameraX::CameraX(int WindowWidth, int WindowHeight)
//{
//    m_windowWidth = WindowWidth;
//    m_windowHeight = WindowHeight;
//    m_pos = Vector3f(0.0f, 20.0f, 80.0f);
//    m_target = Vector3f(0.0f, 0.0f, 1.0f);
//    m_up = Vector3f(0.0f, 1.0f, 0.0f);
//    WorldUp = Vector3f(0.0f, 1.0f, 0.0f);
//    Yaw = yaw;
//    Pitch = pitch;
//    //m_mousePos.x = m_windowWidth / 2;
//    //m_mousePos.y = m_windowHeight / 2;
//    //InitInternal();
//}

Matrix4f CameraX::GetMatrix() const {
    Matrix4f view;
    view.InitCameraTransform(m_pos, m_target, m_up);

    std::cout << "Camera Pos: (" << m_pos.x << ", " << m_pos.y << ", " << m_pos.z << ")  | Looking at: ("
        << m_target.x << ", " << m_target.y << ", " << m_target.z << ")\n";

    return view;
    //return LookAt(m_pos, m_pos + Front, WorldUp);

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
    m_target = pos + front;
    m_up = Vector3f(0.0f, 1.0f, 0.0f);
}

void CameraX::SetCinematic(float time) {
    float radius = 10.0f;
    m_pos = Vector3f(std::cosf(time) * radius, 5.0f, std::sinf(time) * radius);
    m_target = Vector3f(0.0f, 0.0f, 0.0f);
    m_up = Vector3f(0.0f, 1.0f, 0.0f);
}


Matrix4f CameraX::GetViewportMatrix() const
{
    //float HalfW = m_windowWidth / 2.0f;
    //float HalfH = m_windowHeight / 2.0f;

    //Matrix4f Viewport = Matrix4f(HalfW, 0.0f, 0.0f, HalfW,
    //    0.0f, HalfH, 0.0f, HalfH,
    //    0.0f, 0.0f, 1.0f, 0.0f,
    //    0.0f, 0.0f, 0.0f, 1.0f);
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


//void CameraX::Update()
//{
//    // Clamp vertical angle (in degrees)
//    if (m_AngleV > 89.0f) m_AngleV = 89.0f;
//    if (m_AngleV < -89.0f) m_AngleV = -89.0f;
//
//    // Convert angles from degrees to radians if necessary
//    float angleHRad = glm::radians(m_AngleH);
//    float angleVRad = glm::radians(m_AngleV);
//
//    // Calculate the new direction vector
//    Vector3f direction;
//    direction.x = cos(angleVRad) * cos(angleHRad);
//    direction.y = sin(angleVRad);
//    direction.z = cos(angleVRad) * sin(angleHRad);
//    direction.Normalize();
//
//    m_target = direction;
//
//    // Calculate right and up vectors
//    Vector3f right = m_target.Cross(WorldUp);
//    right.Normalize();
//
//    m_up = right.Cross(m_target);
//    m_up.Normalize();
//}

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


// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void CameraX::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void CameraX::updateCameraVectors()
{
    // Convert yaw and pitch from degrees to radians
    float yawRad = ToRadian(Yaw);
    float pitchRad = ToRadian(Pitch);


    // New front vector
    Vector3f front;
    front.x = cosf(yawRad) * cosf(pitchRad);
    front.y = sinf(pitchRad);
    front.z = sinf(yawRad) * cosf(pitchRad);
    Front = front.Normalize();

    // Recalculate right and up
    Right = Front.Cross(WorldUp).Normalize();
    m_up = Right.Cross(Front).Normalize();
}


Matrix4f CameraX::LookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up) const
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