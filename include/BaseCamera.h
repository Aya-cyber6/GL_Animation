#pragma once

#include "ogldev_math_3d.h"

class CameraAPI {
public:

    virtual const Vector3f GetPos() const = 0;

    virtual Matrix4f GetViewportMatrix() const = 0;
    virtual Matrix4f GetViewMatrix() const = 0;
    virtual Matrix4f GetMatrix() const = 0;
    virtual Matrix4f CreatePerspectiveProjection(float fovDegrees, int width, int height, float nearPlane, float farPlane) const = 0;

    virtual const Matrix4f GetProjectionMat() const = 0;

    //virtual void OnMouse(int x, int y)const = 0;
};