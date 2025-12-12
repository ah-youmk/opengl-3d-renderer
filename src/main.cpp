#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <cmath>
#include "graphics/shader.h"
#include "core/camera.h"
#include "graphics/model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

float deltaTime = 0.0f;
float lastFrame = 0.0f;

double mXpos, mYpos;
double lastXpos = 640, lastYpos = 360;
bool firstMouse = true;

float rotationDegree = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 7.5f * deltaTime; 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void cursor_position_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastXpos = xpos;
        lastYpos = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastXpos;
    float yoffset = lastYpos - ypos; // reversed since y-coordinates go from bottom to top

    lastXpos = xpos;
    lastYpos = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// float vertices[] = {
//     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
//      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
//      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
//      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
//     -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
//     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

//     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
//      0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
//     -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0f,
//     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,

//     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
//     -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
//     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
//     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
//     -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,

//      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
//      0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
//      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
//      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
//      0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,

//     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
//      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
//      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
//      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
//     -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
//     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

//     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
//      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
//     -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
//     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
// };

// glm::vec3 cubePositions[] = {
//         glm::vec3( 0.0f,  0.0f,  0.0f),
//         glm::vec3( 2.0f,  5.0f, -15.0f),
//         glm::vec3(-1.5f, -2.2f, -2.5f),
//         glm::vec3(-3.8f, -2.0f, -12.3f),
//         glm::vec3( 2.4f, -0.4f, -3.5f),
//         glm::vec3(-1.7f,  3.0f, -7.5f),
//         glm::vec3( 1.3f, -2.0f, -2.5f),
//         glm::vec3( 1.5f,  2.0f, -2.5f),
//         glm::vec3( 1.5f,  0.2f, -1.5f),
//         glm::vec3(-1.3f,  1.0f, -1.5f)
// };

glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
};

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Basic 3D Renderer", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    Shader objectShader("../src/shaders/vertex.glsl", "../src/shaders/object_fragment.glsl");

    glEnable(GL_DEPTH_TEST);
    stbi_set_flip_vertically_on_load(true);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    Model loadedModel("../src/models/backpack.obj");

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported()) glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwGetCursorPos(window, &mXpos, &mYpos);
        glfwSetCursorPosCallback(window, cursor_position_callback);    
        glfwSetScrollCallback(window, scroll_callback); 

        //rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        objectShader.use();

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)1280.0 / (float)720.0, 0.1f, 100.0f);
        objectShader.setMat4("projection", projection);

        glm::vec3 lightDirection = glm::vec3(-0.2f, -1.0f, -0.3f);
        objectShader.setVec3("viewPos", camera.Position);
             
        // directional light
        objectShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        objectShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        objectShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        objectShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        objectShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        objectShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        objectShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        objectShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        objectShader.setFloat("pointLights[0].constant", 1.0f);
        objectShader.setFloat("pointLights[0].linear", 0.09f);
        objectShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        objectShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        objectShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        objectShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        objectShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        objectShader.setFloat("pointLights[1].constant", 1.0f);
        objectShader.setFloat("pointLights[1].linear", 0.09f);
        objectShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        objectShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        objectShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        objectShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        objectShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        objectShader.setFloat("pointLights[2].constant", 1.0f);
        objectShader.setFloat("pointLights[2].linear", 0.09f);
        objectShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        objectShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        objectShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        objectShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        objectShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        objectShader.setFloat("pointLights[3].constant", 1.0f);
        objectShader.setFloat("pointLights[3].linear", 0.09f);
        objectShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotlight
        objectShader.setVec3("spotLight.position", camera.Position);
        objectShader.setVec3("spotLight.direction", camera.Front);
        objectShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        objectShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        objectShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        objectShader.setFloat("spotLight.constant", 1.0f);
        objectShader.setFloat("spotLight.linear", 0.09f);
        objectShader.setFloat("spotLight.quadratic", 0.032f);
        objectShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        objectShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f))); 

        glm::mat4 view;
        view = camera.GetViewMatrix();
        objectShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        objectShader.setMat4("model", model);
        loadedModel.Draw(objectShader);
        
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}