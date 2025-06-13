#ifndef ICAMERA_H
#define ICAMERA_H

#include <ogldev_math_3d.h>

// Define movement directions similarly to your first Camera
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class ICamera
{
public:
    virtual ~ICamera() = default;

    // Return the view matrix
    virtual Matrix4f GetViewMatrix() const = 0;

    // Return the projection matrix
    virtual Matrix4f GetProjectionMatrix() const = 0;

    // Process keyboard input (direction and time delta)
    virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime) = 0;

    // Process mouse movement (x and y offsets, constrain pitch option)
    virtual void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) = 0;

    // Process mouse scroll (zoom)
    virtual void ProcessMouseScroll(float yoffset) = 0;

    // Get and set camera position
    virtual Vector3f GetPosition() const = 0;
    virtual void SetPosition(const Vector3f& position) = 0;
};

#endif
