//#include <stdio.h>
//#include <string.h>
//#include <math.h>
//#include <GL/glew.h>
//
//#include "ogldev_engine_common.h"
//#include "ogldev_util.h"
//#include "ogldev_basic_glfw_camera.h"
//#include "ogldev_new_lighting.h"
//#include "ogldev_glfw.h"
//#include "ogldev_basic_mesh.h"
//#include "ogldev_world_transform.h"
//#include "ogldev_phong_renderer.h"
//#include <Camera.h>
//#include <DrawDebugAxes.h>
//
//#define WINDOW_WIDTH  1920
//#define WINDOW_HEIGHT 1080
//
//// Forward declarations
//static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
//static void CursorPosCallback(GLFWwindow* window, double x, double y);
//static void MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode);
//static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
//static void CursorPosCallback(GLFWwindow* window, double x, double y);
//static void MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode);
//
//float lastX = WINDOW_WIDTH / 2.0f;
//float lastY = WINDOW_HEIGHT / 2.0f;
//bool firstMouse = true;
//
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//
//class Engine
//{
//public:
//
//    Engine()
//    {
//        m_dirLight.WorldDirection = Vector3f(0.0f, -1.0f, 0.0f);
//        m_dirLight.DiffuseIntensity = 1.0f;
//        m_dirLight.AmbientIntensity = 0.5f;
//    }
//
//    virtual ~Engine()
//    {
//        SAFE_DELETE(m_pCamera);
//    }
//
//    DrawDebugAxesClass debugAxes;
//
//    void Init()
//    {
//        m_phongRenderer.SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
//
//        CreateWindow();
//
//        InitCallbacks();
//
//        InitCamera1();
//
//        InitMesh();
//
//        InitRenderer();
//
//        m_startTime = GetCurrentTimeMillis();
//        m_currentTime = m_startTime;
//
//        if (!debugAxes.Init()) {
//            printf("Error initializing Axes");
//        }
//    }
//
//
//    void Run()
//    {
//        while (!glfwWindowShouldClose(window)) {
//            RenderSceneCB();
//            glfwSwapBuffers(window);
//            glfwPollEvents();
//        }
//    }
//
//
//    void RenderSceneCB()
//    {
//        // Calculate delta time (crucial for smooth movement)
//        float currentFrame = static_cast<float>(glfwGetTime());
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        // Get view-projection matrix
//        Matrix4f VP = m_pCamera->GetProjectionMat() * m_pCamera->GetViewMatrix();
//
//        // Draw debug axes at world origin
//        debugAxes.Draw(VP);  // VP = View * Projection matrix
//        if (m_runAnimation) {
//            m_currentTime = GetCurrentTimeMillis();
//        }
//
//        float AnimationTimeSec = (float)((double)m_currentTime - (double)m_startTime) / 1000.0f;
//
//        float TotalPauseTimeSec = (float)((double)m_totalPauseTime / 1000.0f);
//        AnimationTimeSec -= TotalPauseTimeSec;
//
//        m_phongRenderer.RenderAnimation(m_pMesh, AnimationTimeSec, m_animationIndex);
//    }
//
//    Camera* GetCamera() const { return m_pCamera; }
//    Camera* m_pCamera = nullptr;  // Replaces BasicCamera* and Camera* m_Camera1
//
//
//#define ATTEN_STEP 0.01f
//
//#define ANGLE_STEP 1.0f
//
//    void PassiveMouseCB(int x, int y)
//    {
//        m_pGameCamera->OnMouse(x, y);
//    }
//
//    void KeyboardCB(uint key, int state)
//    {
//        if (state == GLFW_PRESS) {
//
//            switch (key) {
//
//            case GLFW_KEY_0:
//                m_animationIndex = 0;
//                break;
//
//            case GLFW_KEY_1:
//                m_animationIndex = 1;
//                break;
//
//            case GLFW_KEY_2:
//                m_animationIndex = 2;
//                break;
//
//            case GLFW_KEY_3:
//                m_animationIndex = 3;
//                break;
//
//            case GLFW_KEY_SPACE:
//                m_runAnimation = !m_runAnimation;
//                if (m_runAnimation) {
//                    long long CurrentTime = GetCurrentTimeMillis();
//                    // printf("Resumed at %lld\n", CurrentTime);
//                    m_totalPauseTime += (CurrentTime - m_pauseStart);
//                    // printf("Total pause time %lld\n", m_totalPauseTime);
//                }
//                else {
//                    m_pauseStart = GetCurrentTimeMillis();
//                    // printf("Paused at %lld\n", GetCurrentTimeMillis());
//                }
//                break;
//
//            case GLFW_KEY_ESCAPE:
//            case GLFW_KEY_Q:
//                glfwDestroyWindow(window);
//                glfwTerminate();
//                exit(0);
//            }
//        }
//
//        m_pGameCamera->OnKeyboard(key);
//    }
//
//    void MouseCB(int button, int action, int x, int y)
//    {
//    }
//
//
//private:
//
//    void InitCamera1() {
//        m_pCamera = new Camera(Vector3f(0.0f, 0.2f, 2.0f));
//        // Set perspective projection
//        float FOV = 45.0f;
//        float zNear = 0.01f;
//        float zFar = 100.0f;
//        // Note: Your Camera class needs a SetPerspective method (add this to Camera.h)
//        m_pCamera->SetPerspective(FOV, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, zNear, zFar);
//    }
//
//    void InitCallbacks() {
//        glfwSetKeyCallback(window, KeyCallback);
//        glfwSetCursorPosCallback(window, CursorPosCallback);
//        glfwSetMouseButtonCallback(window, MouseButtonCallback);
//        glfwSetScrollCallback(window, scroll_callback);  // Added scroll callback
//
//    }
//
//
//    void CreateWindow()
//    {
//        int major_ver = 0;
//        int minor_ver = 0;
//        bool is_full_screen = false;
//        window = glfw_init(major_ver, minor_ver, WINDOW_WIDTH, WINDOW_HEIGHT, is_full_screen, "Tutorial 40");
//
//        glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
//
//        // tell GLFW to capture our mouse
//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//    }
//
//    void InitCamera()
//    {
//
//        Vector3f Pos(0.21f, -32.13f, -14.37f);
//        Vector3f Target(0.17f, -25.47f, -11.39f);
//        Vector3f Up(0.0, 1.0f, 0.0f);
//
//        float FOV = 45.0f;
//        float zNear = 0.01f;
//        float zFar = 100.0f;
//        PersProjInfo persProjInfo = { FOV, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, zNear, zFar };
//
//        m_pGameCamera = new BasicCamera(persProjInfo, Pos, Target, Up);
//
//        // Debug camera position
//        printf("[Camera] Pos: (%.2f, %.2f, %.2f), Target: (%.2f, %.2f, %.2f)\n",
//            Pos.x, Pos.y, Pos.z, Target.x, Target.y, Target.z);
//    }
//
//    void InitRenderer()
//    {
//        m_phongRenderer.InitPhongRenderer();
//        m_phongRenderer.SetCamera(m_pCamera);
//        m_phongRenderer.SetDirLight(m_dirLight);
//    }
//
//
//    void InitMesh()
//    {
//        m_pMesh = new SkinnedMesh();
//
//
//
//        m_pMesh->LoadMesh("Content/boblampclean.md5mesh");
//        //m_pMesh->SetRotation(90.0f, -45.0f, 0.0f);
//        m_pMesh->SetRotation(0.0f, 180.0f, 0.0f);
//        m_pMesh->SetPosition(0.0f, 15.0f, 15.0f);
//        m_pMesh->SetScale(1.0f);
//
//        // Debug model position
//        Vector3f ModelPos = m_pMesh->GetPosition();
//        printf("[Model] Pos: (%.2f, %.2f, %.2f)\n",
//            ModelPos.x, ModelPos.y, ModelPos.z);
//    }
//
//    GLFWwindow* window = NULL;
//    BasicCamera* m_pGameCamera = NULL;
//    PhongRenderer m_phongRenderer;
//    SkinnedMesh* m_pMesh = NULL;
//    PersProjInfo m_persProjInfo;
//    DirectionalLight m_dirLight;
//    long long m_startTime = 0;
//    long long m_currentTime = 0;
//    bool m_runAnimation = true;
//    long long m_totalPauseTime = 0;
//    long long m_pauseStart = 0;
//    int m_animationIndex = 0;
//};
//
//Engine* app = NULL;
//
//void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
//{
//    float xpos = static_cast<float>(xposIn);
//    float ypos = static_cast<float>(yposIn);
//
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//    lastX = xpos;
//    lastY = ypos;
//
//    app->GetCamera()->ProcessMouseMovement(xoffset, yoffset);
//}
//
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        app->GetCamera()->ProcessKeyboard(FORWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        app->GetCamera()->ProcessKeyboard(BACKWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        app->GetCamera()->ProcessKeyboard(LEFT, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        app->GetCamera()->ProcessKeyboard(RIGHT, deltaTime);
//}
//
//// Callback implementations
//
//static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
//    if (app && app->GetCamera()) {
//        app->GetCamera()->ProcessMouseScroll(static_cast<float>(yoffset));
//    }
//}
//static void MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode) {
//    // Handle mouse button clicks if needed
//}
//
//static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
//        processInput(window);
//    }
//    //app->KeyboardCB(key, action);
//}
//
//static void CursorPosCallback(GLFWwindow* window, double x, double y)
//{
//    mouse_callback(window, x, y);
//    //app->PassiveMouseCB((int)x, (int)y);
//}
//
//int main(int argc, char** argv)
//{
//    app = new Engine();
//
//    app->Init();
//
//    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//    glFrontFace(GL_CW);
//    glCullFace(GL_BACK);
//    glEnable(GL_CULL_FACE);
//    glEnable(GL_DEPTH_TEST);
//
//    app->Run();
//
//    delete app;
//
//    return 0;
//}
//
