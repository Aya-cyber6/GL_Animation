#include "FpsCamera.h"
#include <algorithm>
#include <cmath> // for sin, cos

// Constructor with vectors
CameraFPS::CameraFPS(Vector3f position, Vector3f up, float yaw, float pitch)
    : Front(Vector3f(0.0f, 0.0f, -1.0f)),
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY),
    Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

// Constructor with scalar values
CameraFPS::CameraFPS(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(Vector3f(0.0f, 0.0f, -1.0f)),
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY),
    Zoom(ZOOM)
{
    Position = Vector3f(posX, posY, posZ);
    WorldUp = Vector3f(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

// CameraAPI interface implementations
const Vector3f CameraFPS::GetPos() const
{
    return Position;
}

Matrix4f CameraFPS::GetViewportMatrix() const
{
    Matrix4f identity;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            identity.m[i][j] = (i == j) ? 1.0f : 0.0f;
    return identity;    
}

Matrix4f CameraFPS::GetMatrix() const
{
    // View matrix is used here
    return GetViewMatrix();
}

const Matrix4f CameraFPS::GetProjectionMat() const
{
    PersProjInfo projInfo;
    projInfo.FOV = m_fov;
    projInfo.Width = static_cast<float>(m_viewportWidth);
    projInfo.Height = static_cast<float>(m_viewportHeight);
    projInfo.zNear = m_nearPlane;
    projInfo.zFar = m_farPlane;

    Matrix4f projection;
    projection.InitPersProjTransform(projInfo);
    return projection;
}

// Implement GetViewMatrix using ogldev_math_3d style
Matrix4f CameraFPS::GetViewMatrix() const
{
    // Normalize Front vector using your Normalize()
    Vector3f f = Front;
    f.Normalize();

    // Use your member Cross and Normalize methods
    Vector3f s = f.Cross(WorldUp);
    s.Normalize();

    Vector3f u = s.Cross(f);

    Matrix4f result;
    result.InitIdentity();

    result.m[0][0] = s.x;
    result.m[1][0] = s.y;
    result.m[2][0] = s.z;

    result.m[0][1] = u.x;
    result.m[1][1] = u.y;
    result.m[2][1] = u.z;

    result.m[0][2] = -f.x;
    result.m[1][2] = -f.y;
    result.m[2][2] = -f.z;

    result.m[0][3] = 0.0f;
    result.m[1][3] = 0.0f;
    result.m[2][3] = 0.0f;

    // Use member Dot for dot product
    result.m[3][0] = -s.Dot(Position);
    result.m[3][1] = -u.Dot(Position);
    result.m[3][2] = f.Dot(Position);
    result.m[3][3] = 1.0f;

    return result;
}

void CameraFPS::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position = Position + Front * velocity;
    else if (direction == BACKWARD)
        Position = Position - Front * velocity;
    else if (direction == LEFT)
        Position = Position - Right * velocity;
    else if (direction == RIGHT)
        Position = Position + Right * velocity;
}

void CameraFPS::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        Pitch = std::clamp(Pitch, -89.0f, 89.0f);
    }

    updateCameraVectors();
}

void CameraFPS::ProcessMouseScroll(float yoffset)
{
    Zoom -= yoffset;
    Zoom = std::clamp(Zoom, 1.0f, 45.0f);

    m_fov = Zoom;
}

void CameraFPS::SetProjectionParams(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    m_fov = fov;
    m_aspectRatio = aspectRatio;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
}

void CameraFPS::SetViewportSize(int width, int height)
{
    m_viewportWidth = width;
    m_viewportHeight = height;
    m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

void CameraFPS::updateCameraVectors()
{
    // Calculate new Front vector based on Euler angles
    Vector3f front;
    front.x = cos(ToRadian(Yaw)) * cos(ToRadian(Pitch));
    front.y = sin(ToRadian(Pitch));
    front.z = sin(ToRadian(Yaw)) * cos(ToRadian(Pitch));
    Front = front.Normalize();  // Normalize returns Vector3f& so this works

    // Recalculate Right and Up vectors using your member Cross and Normalize
    Right = Front.Cross(WorldUp).Normalize();
    Up = Right.Cross(Front).Normalize();
}
