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

        InitTriangle();
        triangleShader = new Shader("Shaders/triangle.vert", "Shaders/triangle.frag");

        m_startTime = GetCurrentTimeMillis();
        m_currentTime = m_startTime;
    }


    void Run()
    {

        std::cout << "Camera Pos: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")  | Front: ("
            << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z << ")" << std::endl;

        Vector3f modelPos = m_pMesh->GetPosition();
        std::cout << "Model Pos: (" << modelPos.x << ", " << modelPos.y << ", " << modelPos.z << ")" << std::endl;


        while (!glfwWindowShouldClose(window)) {
            //RenderStatic();
            RenderSceneCB();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    //void RenderStatic() {
    //    Shader ourShader("Shaders/static.vert", "Shaders/static.frag");
    //    float currentFrame = static_cast<float>(glfwGetTime());
    //    deltaTime = currentFrame - lastFrame;
    //    lastFrame = currentFrame;

    //    // input
    //    processInput(window);

    //    // render
    //    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //    ourShader.use();

    //    glm::vec3 lightDir = glm::vec3(-0.2f, -1.0f, -0.3f);
    //    ourShader.setVec3("light.direction", lightDir);

    //    ourShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));   // subtle white ambient
    //    ourShader.setVec3("light.diffuse", glm::vec3(0.7f, 0.7f, 0.7f));   // brighter neutral light
    //    ourShader.setVec3("light.specular", glm::vec3(0.5f, 0.5f, 0.5f));  // white highlights


    //    // Camera position (needed for specular calculation)
    //    ourShader.setVec3("viewPos", fpsCamera->GetPos());
    //    // view/projection transformations
    //    glm::mat4 projection = glm::perspective(glm::radians(fpsCamera->Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    //    Matrix4f view = fpsCamera->GetViewMatrix();
    //    ourShader.setMat4("projection", projection);
    //    ourShader.setMat4("view", view);

    //    // Draw the world
    //    glm::mat4 model_world = glm::mat4(1.0f);
    //    model_world = glm::scale(model_world, glm::vec3(0.01f));
    //    ourShader.setMat4("model", model_world);
    //    worldModel->Draw(ourShader);

    //}

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

        m_phongRenderer.RenderAnimation(m_pMesh, AnimationTimeSec, m_animationIndex);
        //std::cout << "Animation Time: " << AnimationTimeSec << ", Index: " << m_animationIndex << std::endl;


    }


#define ATTEN_STEP 0.01f

#define ANGLE_STEP 1.0f

    void PassiveMouseCB(int x, int y)
    {
        //if (fpsCamera == currentCamera) {
        //    fpsCamera->fpsOnMouse(x, y);
        //}
        //else {
        //    m_pGameCamera->OnMouse(x, y);

        //}
    }
    void MouseCallback(GLFWwindow* window, double xpos, double ypos)
    {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        Vector3f front;
        front.x = cosf(ToRadian(yaw)) * cosf(ToRadian(pitch));
        front.y = sinf(ToRadian(pitch));
        front.z = sinf(ToRadian(yaw)) * cosf(ToRadian(pitch));
        cameraFront = front.Normalize();
    }


    void KeyboardCB(uint key, int state)
    {
        if (state == GLFW_PRESS) {

            switch (key) {

                //case GLFW_KEY_C:
                //    if (currentCamera == fpsCamera)
                //        currentCamera = m_pGameCamera;
                //    else
                //        currentCamera = fpsCamera;
                //    break;


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

        //if (currentCamera == m_pGameCamera) {
        //    m_pGameCamera->OnKeyboard(key);
        //}
        //else {
        //    m_pGameCamera->fpsOnKeyboard(key);

        //}
        //processInput(window);
    }


    void processInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            glfwDestroyWindow(window);
            glfwTerminate();
            exit(0);

        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraFront * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraFront * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= cameraFront.Cross(cameraUp).Normalize() * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += cameraFront.Cross(cameraUp).Normalize() * cameraSpeed;

    }

    void MouseCB(int button, int action, int x, int y)
    {
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
        //Vector3f Pos(0.0f, 0.0f, 0.0f);
        //Vector3f Target(0.0f, 0.0f, 1.0f);
        //Vector3f Up(0.0, 1.0f, 0.0f);

        //float FOV = 45.0f;
        //float zNear = 0.1f;
        //float zFar = 100.0f;
        //PersProjInfo persProjInfo = { FOV, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, zNear, zFar };
        //

        //m_pGameCamera = new BasicCamera(persProjInfo, Pos, Target, Up);
        //fpsCamera = new Camera(Pos, Target, Up, WINDOW_WIDTH, WINDOW_HEIGHT);

        // First-person mode example
        camera = new CameraX(WINDOW_WIDTH, WINDOW_HEIGHT);
        Vector3f characterPos(0.0f, 0.0f, 5.0f);
        Vector3f characterDir(0.0f, 0.0f, -1.0f);
        camera->SetFirstPerson(characterPos, characterDir);
        //camera->SetFreeFly(cameraPos, (camTarget - cameraPos).Normalize());

        // Or Free-fly camera
        // camera.SetFreeFly(Vector3f(5.0f, 2.0f, 5.0f), Vector3f(-1.0f, 0.0f, -1.0f));

        // Or Cinematic camera (e.g., rotating around the scene)
        float time = glfwGetTime(); // or any timer
        // camera.SetCinematic(time);

    }


    void InitRenderer()
    {
        m_phongRenderer.InitPhongRenderer();
        //camera.SetFirstPerson(playerPos, playerDir);

        m_phongRenderer.SetCamera(camera);
        m_phongRenderer.SetDirLight(m_dirLight);
    }

    void InitMesh()
    {
        m_pMesh = new SkinnedMesh();

        //m_pMesh->LoadMesh("Content/donut/donut.obj");
        m_pMesh->LoadMesh("Content/dancing_vampire.dae");

        if (!m_pMesh->hasAnim) {
            std::cout << "*************Scene contains no animations*******************************." << std::endl;
            worldModel = new StaticModel(m_pMesh->m_pScene->mRootNode, m_pMesh->m_pScene, m_pMesh->directory);
            return;
        }
        m_pMesh->SetRotation(0.0f, 180.0f, 0.0f);
        m_pMesh->SetPosition(1, 19.7423, 130.0338);  // Move it slightly below origin
        m_pMesh->SetScale(0.1f);
    }

    GLFWwindow* window = NULL;
    //BasicCamera* m_pGameCamera = NULL;
    //Camera* fpsCamera = NULL;  // Slightly higher Y and further Z
    //CameraAPI* currentCamera = nullptr;
    CameraX* camera = NULL;

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
};

Tutorial40* app = NULL;

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    app->processInput(window);
}


static void CursorPosCallback(GLFWwindow* window, double x, double y)
{
    app->MouseCallback(window, (int)x, (int)y);
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
