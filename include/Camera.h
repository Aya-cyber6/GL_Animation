#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <ogldev_math_3d.h>

// Defines several possible options for camera movement
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
    // Camera attributes
    Vector3f Position;
    Vector3f Front;
    Vector3f Up;
    Vector3f Right;
    Vector3f WorldUp;

    // Euler angles
    float Yaw;
    float Pitch;

    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Constructor with vectors
    Camera(Vector3f position = Vector3f(0.0f, 0.0f, 0.0f),
        Vector3f up = Vector3f(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH) :
        Front(Vector3f(0.0f, 0.0f, -1.0f)),
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
    Camera(float posX, float posY, float posZ,
        float upX, float upY, float upZ,
        float yaw, float pitch) :
        Front(Vector3f(0.0f, 0.0f, -1.0f)),
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

    const Vector3f& GetPos() const { return Position; }
    const Vector3f& GetFront() const { return Front; }
    const Vector3f& GetUp() const { return Up; }


    Matrix4f GetViewportMatrix(float width, float height) const {
        Matrix4f Viewport;

        // Initialize viewport transform matrix
        Viewport.InitIdentity();
        Viewport.m[0][0] = width / 2.0f;
        Viewport.m[1][1] = -height / 2.0f;  // Negative to flip Y axis
        Viewport.m[0][3] = width / 2.0f;
        Viewport.m[1][3] = height / 2.0f;

        return Viewport;
    }

    Matrix4f GetViewMatrix() const
    {
        Vector3f Target = Position + Front;
        Matrix4f CameraTransformation;
        CameraTransformation.InitCameraTransform(Position, Target, Up);
        return CameraTransformation;
    }

    const Matrix4f GetProjectionMat() const { return m_projection; }


    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

    void SetPerspective(float fov, float width, float height, float zNear, float zFar)
    {
        Zoom = fov;
        // The actual projection matrix will be created in GetProjectionMatrix()
    }

private:
    Matrix4f m_projection;

    void updateCameraVectors()
    {
        // Calculate new Front vector
        Vector3f front;
        front.x = cosf(ToRadian(Yaw)) * cosf(ToRadian(Pitch));
        front.y = sinf(ToRadian(Pitch));
        front.z = sinf(ToRadian(Yaw)) * cosf(ToRadian(Pitch));
        Front = front.Normalize();

        // Re-calculate Right and Up vectors
        Right = Front.Cross(WorldUp).Normalize();
        Up = Right.Cross(Front).Normalize();
    }
};
#endif