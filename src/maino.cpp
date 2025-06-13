#include <stdio.h>
#include <string.h>

#include "ogldev_engine_common.h"
#include "ogldev_util.h"
#include "ogldev_basic_glfw_camera.h"
#include "ogldev_new_lighting.h"
#include "ogldev_glfw.h"
#include "ogldev_mesh_common.h"
#include "ogldev_world_transform.h"
#include "ogldev_phong_renderer.h"
#include "StaticModel.h"
#include "FpsCamera.h"
#include <iostream>

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void CursorPosCallback(GLFWwindow* window, double x, double y);
static void MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode);

StaticModel* worldModel;

float deltaTime = 0.0f;
float lastFrame = 0.0f;


class Tutorial40
{
public:
    CameraFPS* fpsCamera = nullptr;
    CameraAPI* currentCamera = nullptr;

    Tutorial40()
    {
        fpsCamera = new CameraFPS(
            0.0f, 0.0f, 3.0f,    // position (x, y, z)
            0.0f, 1.0f, 0.0f,    // up vector (x, y, z)
            -90.0f,              // yaw
            0.0f                 // pitch
        );

        currentCamera = fpsCamera;

        m_dirLight.WorldDirection = Vector3f(0.0f, -1.0f, 0.0f);
        m_dirLight.DiffuseIntensity = 1.0f;
        m_dirLight.AmbientIntensity = 0.5f;
    }

    virtual ~Tutorial40()
    {
        SAFE_DELETE(fpsCamera);
        SAFE_DELETE(m_pGameCamera);
    }


    void Init()
    {
        CreateWindow();

        InitCallbacks();

        InitCamera();

        InitMesh();

        if (!m_pMesh->hasAnim) {
            std::cout << "Scene contains no animations*******************************." << std::endl;
            return;
        }
        InitRenderer();

        m_startTime = GetCurrentTimeMillis();
        m_currentTime = m_startTime;
    }


    void Run()
    {
        while (!glfwWindowShouldClose(window)) {
            RenderStatic();
            //RenderSceneCB();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void RenderStatic() {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Shader ourShader("Shaders/static.vert", "Shaders/static.frag");
        ourShader.use();

        // Light direction (Vector3f)
        Vector3f lightDir(-0.2f, -1.0f, -0.3f);
        ourShader.setVec3("light.direction", lightDir);

        ourShader.setVec3("light.ambient", Vector3f(0.2f, 0.2f, 0.2f));   // ambient light
        ourShader.setVec3("light.diffuse", Vector3f(0.7f, 0.7f, 0.7f));   // diffuse light
        ourShader.setVec3("light.specular", Vector3f(0.5f, 0.5f, 0.5f));  // specular highlights

        // Camera position (specular calculations)
        ourShader.setVec3("viewPos", currentCamera->GetPos());

        // Projection matrix (custom Perspective)
        float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
        Matrix4f projection = currentCamera->GetProjectionMat();
        // OR use: Matrix4f::Perspective(camera.Zoom, aspectRatio, 0.1f, 100.0f);

        // View matrix
        Matrix4f view = currentCamera->GetViewportMatrix();

        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // Model matrix: scaling 0.01x
        Matrix4f model_world;
        model_world.InitScaleTransform(0.01f, 0.01f, 0.01f);

        ourShader.setMat4("model", model_world);

        worldModel->Draw(ourShader);
    }


    void RenderSceneCB()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_pGameCamera->OnRender();

        if (m_runAnimation) {
            m_currentTime = GetCurrentTimeMillis();
        }

        float AnimationTimeSec = (float)((double)m_currentTime - (double)m_startTime) / 1000.0f;

        float TotalPauseTimeSec = (float)((double)m_totalPauseTime / 1000.0f);
        AnimationTimeSec -= TotalPauseTimeSec;

        m_phongRenderer.RenderAnimation(m_pMesh, AnimationTimeSec, m_animationIndex);
    }

    
#define ATTEN_STEP 0.01f

#define ANGLE_STEP 1.0f

    void PassiveMouseCB(int x, int y)
    {
        if (currentCamera == m_pGameCamera) {
            m_pGameCamera->OnMouse(x, y);

        }
        else {
            fpsCamera->ProcessMouseMovement(x, y);

        }
    }

    void KeyboardCB(uint key, int state)
    {
        if (state == GLFW_PRESS) {

            switch (key) {

                case GLFW_KEY_C:
                    if (currentCamera == fpsCamera)
                        currentCamera = m_pGameCamera;
                    else
                        currentCamera = fpsCamera;
                    break;
       

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

            case GLFW_KEY_SPACE:
                m_runAnimation = !m_runAnimation;
                if (m_runAnimation) {
                    long long CurrentTime = GetCurrentTimeMillis();
                    // printf("Resumed at %lld\n", CurrentTime);
                    m_totalPauseTime += (CurrentTime - m_pauseStart);
                    // printf("Total pause time %lld\n", m_totalPauseTime);
                }
                else {
                    m_pauseStart = GetCurrentTimeMillis();
                    // printf("Paused at %lld\n", GetCurrentTimeMillis());
                }
                break;

            case GLFW_KEY_ESCAPE:
            case GLFW_KEY_Q:
                glfwDestroyWindow(window);
                glfwTerminate();
                exit(0);
            }
        }

        m_pGameCamera->OnKeyboard(key);
        //fpsCamera->OnKeyboard(key);

    }


    void MouseCB(int button, int action, int x, int y)
    {
    }


private:

    void CreateWindow()
    {
        int major_ver = 0;
        int minor_ver = 0;
        bool is_full_screen = false;
        window = glfw_init(major_ver, minor_ver, WINDOW_WIDTH, WINDOW_HEIGHT, is_full_screen, "Tutorial 40");

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
        Vector3f Pos(0.0f, 0.0f, 0.0f);
        Vector3f Target(0.0f, 0.0f, 1.0f);
        Vector3f Up(0.0, 1.0f, 0.0f);

        float FOV = 45.0f;
        float zNear = 0.1f;
        float zFar = 100.0f;
        PersProjInfo persProjInfo = { FOV, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, zNear, zFar };

        m_pGameCamera = new BasicCamera(persProjInfo, Pos, Target, Up);
    }


    void InitRenderer()
    {
        m_phongRenderer.InitPhongRenderer();
        m_phongRenderer.SetCamera(m_pGameCamera);
        m_phongRenderer.SetDirLight(m_dirLight);
    }


    void InitMesh()
    {
        m_pMesh = new SkinnedMesh();
    
        m_pMesh->LoadMesh("Content/Station/stationgarden.obj");
        //m_pMesh->LoadMesh("Content/boblampclean.md5mesh");
        std::cout << "*************Scene contains no animations*******************************." << std::endl;

        if (!m_pMesh->hasAnim) {
            worldModel = new StaticModel(m_pMesh->m_pScene->mRootNode, m_pMesh->m_pScene, m_pMesh->directory);
            return;
        }
        m_pMesh->SetRotation(0.0f, 180.0f, 0.0f);
        m_pMesh->SetPosition(0.0f, 15.0f, 10.0f);
        m_pMesh->SetScale(0.1f);
    }

    GLFWwindow* window = NULL;
    BasicCamera* m_pGameCamera = NULL;
    PhongRenderer m_phongRenderer;
    SkinnedMesh* m_pMesh = NULL;
    PersProjInfo m_persProjInfo;
    DirectionalLight m_dirLight;
    long long m_startTime = 0;
    long long m_currentTime = 0;
    bool m_runAnimation = true;
    long long m_totalPauseTime = 0;
    long long m_pauseStart = 0;
    int m_animationIndex = 0;


    //protected:
    //    const aiScene* m_pScene;
    //    Assimp::Importer m_Importer;


};

Tutorial40* app = NULL;

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
    app = new Tutorial40();

    app->Init();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    app->Run();

    delete app;

    return 0;
}
