#include <stdio.h>
#include <string.h>

#include "ogldev_engine_common.h"
#include "ogldev_util.h"
#include "basic_glfw_camera.h"
#include "ogldev_new_lighting.h"
#include "ogldev_glfw.h"
#include "ogldev_mesh_common.h"
#include "ogldev_world_transform.h"
#include "ogldev_phong_renderer.h"
#include "StaticModel.h"
#include "FpsCamera.h"
#include <iostream>
#include "CameraX.h"

Shader* triangleShader = nullptr;

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void CursorPosCallback(GLFWwindow* window, double x, double y);
static void MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode);

StaticModel* worldModel;

float deltaTime = 0.0f;
float lastFrame = 0.0f;


Vector3f camTarget(0.0f, -1.0f, 0.0f);

Vector3f cameraPos(2.0f, 14.0f, 55.0f);
Vector3f cameraFront(0.0f, 0.0f, -1.0f);
Vector3f cameraUp(0.0f, 1.0f, 0.0f);
float cameraSpeed = 5.0f * deltaTime; // adjust deltaTime each frame

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 960.0f;  // Half of 1920
float lastY = 540.0f;  // Half of 1080
bool firstMouse = true;


class Tutorial40
{
public:

    Tutorial40()
    {
        //Camera fpsCamera(glm::vec3(0.0f, 0.2f, 2.0f));  // Slightly higher Y and further Z
        m_dirLight.WorldDirection = Vector3f(0.0f, -1.0f, 0.0f);
        m_dirLight.DiffuseIntensity = 1.0f;
        m_dirLight.AmbientIntensity = 0.5f;
    }

    virtual ~Tutorial40()
    {
        //SAFE_DELETE(fpsCamera);
        //SAFE_DELETE(m_pGameCamera);
    }


    void Init()
    {
        CreateWindow();

        InitCallbacks();

        InitCamera();
        //currentCamera = m_pGameCamera;
        InitMesh();

        if (!m_pMesh->hasAnim) {
            std::cout << "Scene contains no animations*******************************." << std::endl;
            return;
        }
        InitRenderer();

        //InitTriangle();
        //triangleShader = new Shader("Shaders/triangle.vert", "Shaders/triangle.frag");

        m_startTime = GetCurrentTimeMillis();
        m_currentTime = m_startTime;
    }


    void Run()
    {
        while (!glfwWindowShouldClose(window)) {
            //RenderStatic();
            RenderSceneCB();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void RenderSceneCB()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //m_pGameCamera->OnRender();

        if (m_runAnimation) {
            m_currentTime = GetCurrentTimeMillis();
        }

        float AnimationTimeSec = (float)((double)m_currentTime - (double)m_startTime) / 1000.0f;

        float TotalPauseTimeSec = (float)((double)m_totalPauseTime / 1000.0f);
        AnimationTimeSec -= TotalPauseTimeSec;

        m_phongRenderer.Render(mesh);
        //m_phongRenderer.Render(mesh);

        m_phongRenderer.RenderAnimation(m_pMesh, AnimationTimeSec, m_animationIndex);
        //std::cout << "Animation Time: " << AnimationTimeSec << ", Index: " << m_animationIndex << std::endl;
            // Draw the triangle beside it
    // -----------------------------
        //triangleShader->use();

        //Matrix4f projection = camera->GetProjectionMat();
        //Matrix4f view = camera->GetMatrix();

        //// Position triangle a bit to the side of the model
        //Matrix4f model;
        //model.InitIdentity();
        //
        //model.InitTranslationTransform(4.0f, 18.0f, 80.0f); // Adjust x to position beside the model
        //
        //model.InitScaleTransform(Vector3f(0.1f)); // Optional: Scale triangle if too small

        //triangleShader->setMat4("model", model);
        //triangleShader->setMat4("view", view);
        //triangleShader->setMat4("projection", projection);

        //glBindVertexArray(triangleVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //std::cout << "STATUS: " <<"TRIANGLE DRAWN" << std::endl;

        //glBindVertexArray(0);

    }


#define ATTEN_STEP 0.01f

#define ANGLE_STEP 1.0f

    void PassiveMouseCB(int x, int y)
    {
        camera->OnMouse(x, y, true);

    }
  
    void KeyboardCB(uint key, int state)
    {
        if (state == GLFW_PRESS) {

            switch (key) {

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
        camera->OnKeyboard(key);
    }

    void MouseCB(int button, int action, int x, int y)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS){
                camera->m_mousePressed = true;
                camera->SetMousePos(x, y);  // Set current position to avoid jump
            }
            else if (action == GLFW_RELEASE){
                camera->m_mousePressed = false;
                }
        }
    }


private:

    unsigned int triangleVAO, triangleVBO;

    void InitTriangle()
    {
        float triangleVertices[] = {
            // positions         // colors
             1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
             0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
             0.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1.0f
        };

        glGenVertexArrays(1, &triangleVAO);
        glGenBuffers(1, &triangleVBO);

        glBindVertexArray(triangleVAO);

        glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }


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
        // First-person mode example
        camera = new CameraX(WINDOW_WIDTH, WINDOW_HEIGHT);
        Vector3f characterPos(0.0f, 0.0f, 5.0f);
        Vector3f characterDir(0.0f, 0.0f, -1.0f);
        //camera->SetFirstPerson(characterPos, characterDir);
        camera->SetFreeFly(cameraPos, (camTarget - cameraPos).Normalize());

        // Or Free-fly camera
        // camera.SetFreeFly(Vector3f(5.0f, 2.0f, 5.0f), Vector3f(-1.0f, 0.0f, -1.0f));

        // Or Cinematic camera (e.g., rotating around the scene)
        float time = glfwGetTime(); // or any timer
        //camera->SetCinematic(time);

    }


    void InitRenderer()
    {
        m_phongRenderer.InitPhongRenderer();
        m_phongRenderer.SetCamera(camera);
        m_phongRenderer.SetDirLight(m_dirLight);
    }

    void InitMesh()
    {
        m_pMesh = new SkinnedMesh();
        //m_pMesh->LoadMesh("Content/dancing_vampire.dae");
        m_pMesh->LoadMesh("Content/BatmanAnimated/BatmanAnimated.DAE");

        mesh = new BasicMesh();
        mesh->LoadMesh("Content/Station/stationgarden.obj");

        m_pMesh->SetRotation(0.0f, 180.0f, 0.0f);
        m_pMesh->SetPosition(1, 19.7423, 130.0338);
        m_pMesh->SetScale(0.01f);

        mesh->SetRotation(0.0f, 180.0f, 0.0f);
        mesh->SetPosition(1, 19.7423, 130.0338);
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

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    app->Run();

    delete app;

    return 0;
}
