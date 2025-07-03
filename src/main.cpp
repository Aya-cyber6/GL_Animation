#include <stdio.h>
#include <string.h>

#include "ogldev_engine_common.h"
#include "ogldev_util.h"
#include "ogldev_new_lighting.h"
#include "ogldev_glfw.h"
#include "ogldev_mesh_common.h"
#include "ogldev_world_transform.h"
#include "ogldev_phong_renderer.h"
#include "CameraX.h"
#include <iostream>

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void CursorPosCallback(GLFWwindow* window, double x, double y);
static void MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode);

Vector3f cameraPos(0.0f, 3.0f, -25.0f);   // Positioned behind and above the model
Vector3f camTarget(0.0f, 0.0f, 0.0f);    // Looking directly at the origin


enum CameraMode {
    FIRST_PERSON,
    FREE_FLY,
    CINEMATIC
};

CameraMode currentCameraMode = FREE_FLY; // Default

class Engine
{
public:

    Engine()
    {
        m_dirLight.WorldDirection = Vector3f(0.0f, -1.0f, 0.0f);
        m_dirLight.DiffuseIntensity = 1.0f;
        m_dirLight.AmbientIntensity = 0.5f;
    }

    virtual ~Engine()
    {
        SAFE_DELETE(camera);
    }

    void UpdateCamera(float time)
    {
        if (currentCameraMode == FIRST_PERSON) {
            Vector3f charPos = m_pMesh->GetPosition();
            Vector3f charDir = m_pMesh->GetForwardDirection();
            camera->SetFirstPerson(charPos, charDir);
        }
        else if (currentCameraMode == FREE_FLY) {
            camera->SetFreeFly(cameraPos, (camTarget - cameraPos).Normalize());
        }
    }



    void Init()
    {
        CreateWindow();

        InitCallbacks();

        InitCamera();

        InitMesh();

        InitRenderer();

        m_startTime = GetCurrentTimeMillis();
        m_currentTime = m_startTime;
    }


    void Run()
    {
        while (!glfwWindowShouldClose(window)) {
            //UpdateCamera(m_currentTime);
            RenderSceneCB();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void RenderSceneCB()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (currentCameraMode == CINEMATIC) {
            Vector3f charPos = m_pMesh->GetPosition();
            Vector3f charDir = m_pMesh->GetForwardDirection(); // Must point where the character is moving/facing
            camera->SetCinematic(glfwGetTime(), charPos, charDir);
        }



        if (m_runAnimation) {
            m_currentTime = GetCurrentTimeMillis();
        }

        float AnimationTimeSec = (float)((double)m_currentTime - (double)m_startTime) / 1000.0f;

        float TotalPauseTimeSec = (float)((double)m_totalPauseTime / 1000.0f);
        AnimationTimeSec -= TotalPauseTimeSec;
        
        m_phongRenderer.Render(mesh);

        m_phongRenderer.RenderAnimation(m_pMesh, AnimationTimeSec, m_animationIndex);
    }

    void PassiveMouseCB(int x, int y)
    {
        camera->OnMouse(x, y, true);

    }

    void KeyboardCB(uint key, int state)
    {
        if (state == GLFW_PRESS) {

            switch (key) {
                // Animation index selection
            case GLFW_KEY_0:
                m_animationIndex = 0;
                break;
            case GLFW_KEY_1:
                m_animationIndex = 1;
                break;
            case GLFW_KEY_2:
                m_animationIndex = 2;
                break;
            case GLFW_KEY_3:
                m_animationIndex = 3;
                break;

                // Camera mode selection (independent)
            case GLFW_KEY_C:
                currentCameraMode = FIRST_PERSON;
                UpdateCamera(glfwGetTime());

                break;
            case GLFW_KEY_V:
                currentCameraMode = FREE_FLY;
                UpdateCamera(glfwGetTime());

                break;
            case GLFW_KEY_B:
                currentCameraMode = CINEMATIC;
                break;

                // Animation play/pause
            case GLFW_KEY_SPACE:
                m_runAnimation = !m_runAnimation;
                if (m_runAnimation) {
                    long long CurrentTime = GetCurrentTimeMillis();
                    m_totalPauseTime += (CurrentTime - m_pauseStart);
                }
                else {
                    m_pauseStart = GetCurrentTimeMillis();
                }
                break;

                // Exit
            case GLFW_KEY_ESCAPE:
            case GLFW_KEY_Q:
                glfwDestroyWindow(window);
                glfwTerminate();
                exit(0);
            }
        }
        camera->OnKeyboard(key);
    }


    void MouseCB(int button, int action, int x, int y)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                camera->SetMousePos(x, y);  // Set current position to avoid jump
                camera->m_mousePressed = true;
            }
            else if (action == GLFW_RELEASE) {
                camera->m_mousePressed = false;
            }
        }
    }

private:

    void CreateWindow()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        int major_ver = 0;
        int minor_ver = 0;
        bool is_full_screen = false;
        window = glfw_init(major_ver, minor_ver, WINDOW_WIDTH, WINDOW_HEIGHT, is_full_screen, "GL Animation");

        glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    }

    void InitCallbacks()
    {
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetCursorPosCallback(window, CursorPosCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
    }

    void InitCamera()
    {
        camera = new CameraX( cameraPos, camTarget ,WINDOW_WIDTH, WINDOW_HEIGHT);
        float time = glfwGetTime(); 
        UpdateCamera(time);
    }

    void InitRenderer()
    {
        m_phongRenderer.InitPhongRenderer();
        m_phongRenderer.SetCamera(camera);
        m_phongRenderer.SetDirLight(m_dirLight);
    }

    void InitMesh()
    {
        //Animated mesh
        m_pMesh = new SkinnedMesh();    
        m_pMesh->LoadMesh("Content/cartoon/source/stt.fbx");
        m_pMesh->SetRotation(90.0f, 180.0f, 0.0f);
        m_pMesh->SetPosition(0.0f, 1.0f, 0.0f);
        m_pMesh->SetScale(0.01f);

        //Static mesh
        mesh = new BasicMesh();
        mesh->LoadMesh("Content/Station/stationgarden.obj");
        mesh->SetRotation(0.0f, 0.0f, 0.0f);
        mesh->SetPosition(0.0f, 0.0f, 0.0f);
        mesh->SetScale(0.1f);
    }

    GLFWwindow* window = NULL;
    CameraX* camera = NULL;
    PhongRenderer m_phongRenderer;
    SkinnedMesh* m_pMesh = NULL;
    PersProjInfo m_persProjInfo;
    DirectionalLight m_dirLight;
    BasicMesh* mesh = NULL;

    long long m_startTime = 0;
    long long m_currentTime = 0;
    bool m_runAnimation = true;
    long long m_totalPauseTime = 0;
    long long m_pauseStart = 0;
    int m_animationIndex = 0;
};

Engine* app = NULL;

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    app->KeyboardCB(key, action);
}


static void CursorPosCallback(GLFWwindow* window, double x, double y)
{
    app->PassiveMouseCB((int)x, (int)y);
}


static void MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode)
{
    double x, y;

    glfwGetCursorPos(window, &x, &y);

    app->MouseCB(Button, Action, (int)x, (int)y);
}


int main(int argc, char** argv)
{
    app = new Engine();

    app->Init();

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    app->Run();

    delete app;

    return 0;
}