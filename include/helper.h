//#ifndef CAMERA_MATH_HELPERS_H
//#define CAMERA_MATH_HELPERS_H
//
//#include "ogldev_math_3d.h"
//#include <cmath>
//
//
//// Extension for normalized vector
//inline Vector3f Normalize(const Vector3f& v) {
//    Vector3f result = v;
//    result.Normalize();
//    return result;
//}
//
//// LookAt implementation using Matrix4f and Vector3f
//inline void InitLookAt(Matrix4f& m, const Vector3f& eye, const Vector3f& target, const Vector3f& up) {
//    Vector3f f = target - eye;
//    f.Normalize();
//
//    Vector3f s = f.Cross(up);
//    s.Normalize();
//
//    Vector3f u = s.Cross(f);
//
//    m.m[0][0] = s.x;  m.m[0][1] = s.y;  m.m[0][2] = s.z;  m.m[0][3] = -s.Dot(eye);
//    m.m[1][0] = u.x;  m.m[1][1] = u.y;  m.m[1][2] = u.z;  m.m[1][3] = -u.Dot(eye);
//    m.m[2][0] = -f.x; m.m[2][1] = -f.y; m.m[2][2] = -f.z; m.m[2][3] = f.Dot(eye);
//    m.m[3][0] = 0.0f; m.m[3][1] = 0.0f; m.m[3][2] = 0.0f; m.m[3][3] = 1.0f;
//}
//
//// Struct for projection parameters
//struct ProjectionParams {
//    float FOV;
//    float AspectRatio;
//    float zNear;
//    float zFar;
//
//    ProjectionParams(float fov, float ar, float zn, float zf)
//        : FOV(fov), AspectRatio(ar), zNear(zn), zFar(zf) {
//    }
//};
//
//// Perspective projection
//inline void InitPerspectiveProj(Matrix4f& m, const ProjectionParams& p) {
//    float tanHalfFOV = tanf(ToRadian(p.FOV / 2.0f));
//    float zRange = p.zNear - p.zFar;
//
//    m.m[0][0] = 1.0f / (tanHalfFOV * p.AspectRatio); m.m[0][1] = 0; m.m[0][2] = 0; m.m[0][3] = 0;
//    m.m[1][0] = 0; m.m[1][1] = 1.0f / tanHalfFOV; m.m[1][2] = 0; m.m[1][3] = 0;
//    m.m[2][0] = 0; m.m[2][1] = 0; m.m[2][2] = (-p.zNear - p.zFar) / zRange; m.m[2][3] = 2.0f * p.zFar * p.zNear / zRange;
//    m.m[3][0] = 0; m.m[3][1] = 0; m.m[3][2] = 1.0f; m.m[3][3] = 0;
//}
//
////// Perspective Projection Matrix Initialization
////inline void InitPerspectiveProj(Matrix4f& m, const ProjectionParams& p) {
////    float tanHalfFOV = tanf(ToRadian(p.FOV / 2.0f));
////    float zRange = p.zNear - p.zFar;
////
////    m.InitIdentity();
////    m.m[0][0] = 1.0f / (tanHalfFOV * p.AspectRatio);
////    m.m[1][1] = 1.0f / tanHalfFOV;
////    m.m[2][2] = (-p.zNear - p.zFar) / zRange;
////    m.m[2][3] = 2.0f * p.zFar * p.zNear / zRange;
////    m.m[3][2] = 1.0f;
////    m.m[3][3] = 0.0f;
////}
//
//#endif // CAMERA_MATH_HELPERS_H
