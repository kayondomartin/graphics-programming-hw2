#define GLM_ENABLE_EXPERIMENTAL
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "geometry_primitives.h"
#include <iostream>
#include <vector>
#include "camera.h"
#include "texture.h"
#include "texture_cube.h"
#include "math_utils.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

bool isWindowed = true;
bool isKeyboardProcessed[1024] = {0};

// setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(4.0f, 1.0f, -4.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // define normal shader and skybox shader.
    Shader shader("../shaders/shader.vs", "../shaders/shader.fs"); // you can name your shader files however you like
    Shader skyboxShader("../shaders/shader_skybox.vs","../shaders/shader_skybox.fs");


    // TODO : define required VAOs(textured cube, skybox, quad)
    // data are defined in geometry_primitives.h
    unsigned int cubesVBO, cubesVAO;
    glGenVertexArrays(1,&cubesVAO);
    glGenBuffers(1, &cubesVBO);
    glBindVertexArray(cubesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_positions_textures), cube_positions_textures, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));


    unsigned int groundVAO, groundVBO;
    glGenVertexArrays(1, &groundVAO);
    glGenBuffers(1, &groundVBO);
    glBindVertexArray(groundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ground_position_textures),ground_position_textures, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));

    unsigned int grassVAO, grassVBO, grassEBO;
    glGenVertexArrays(1, &grassVAO);
    glGenBuffers(1, &grassVBO);
    glBindVertexArray(grassVAO);
    glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grass_positions_textures), grass_positions_textures, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    

    unsigned int skyBoxVBO, skyBoxVAO;
    glGenVertexArrays(1, &skyBoxVAO);
    glGenBuffers(1, &skyBoxVBO);
    glBindVertexArray(skyBoxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_positions), skybox_positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  1.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f,  2.2f, -2.5f),
        glm::vec3(-3.8f,  2.0f, -12.3f),
        glm::vec3( 2.4f,  1.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  1.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };


    const int n_grass = 1000;
    float grassGroundSize = 20;
    glm::vec3 grassPositions[n_grass];

    // positions of our grasses
    for(int i=0; i<n_grass; i++){
        float s = grassGroundSize/2;
        float x = getRandomValueBetween(-s, s);
        float z = getRandomValueBetween(-s, s);
        grassPositions[i].x = x;
        grassPositions[i].y = 0.5f;
        grassPositions[i].z = z;
    }

    Texture cubeTexture("../resources/container.jpg");
    Texture grassTexture("../resources/grass.png");
    Texture grassGroundTexture("../resources/grass_ground.jpg");

    // TODO : define textures (container, grass, grass_ground) & cubemap textures (day, night)

    // TODO : set texture & skybox texture uniform value (initialization)
    // e.g) shader.use(), shader.setInt("texture", 0);
    CubemapTexture cubemapTexture(skyboxTextureDay);
    CubemapTexture cubemapTextureNight(skyboxTextureNight);

    shader.use();
    shader.setInt("texture1",0);

    skyboxShader.use();
    skyboxShader.setInt("skyboxTexture1",0);
    skyboxShader.setInt("skyboxTexture2",1);

    // render loop
    // -----------
    lastFrame = glfwGetTime();
    while (!glfwWindowShouldClose(window)){


        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        /////////////////////////////////////////////////////
        // TODO : Main Rendering Loop
        /////////////////////////////////////////////////////

        // (1) render boxes(cube) using normal shader.
        // (2) render ground(quad) using normal shader.
        // (3) render billboard grasses(quad) using normal shader.
        // (4) render skybox using skybox shader.
        auto projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        auto view = camera.GetViewMatrix();

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        glBindVertexArray(cubesVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture.ID);
        for(unsigned int i=0; i<10; i++){
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(grassVAO);
        glBindTexture(GL_TEXTURE_2D, grassTexture.ID);
        for(unsigned int i=0; i<n_grass; i++){
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, grassPositions[i]);
            auto dirFromCamera = camera.Position-grassPositions[i];
            dirFromCamera[1] = 0.0f;
            glm::vec3 origLookAt(0.0f,0.0f,1.0f);
            auto aux = glm::cross(origLookAt,dirFromCamera);
            auto angle1 = glm::dot(origLookAt,dirFromCamera);
            if(angle1 < 0.99990 && angle1 > -0.99990)
                model = glm::rotate(model, angle1, aux);
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        
        shader.setMat4("view", view);
        glBindVertexArray(groundVAO);
        glBindTexture(GL_TEXTURE_2D, grassGroundTexture.ID);
        auto model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);
        
        glDepthFunc(GL_EQUAL);
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyboxShader.setMat4("view",view);
        skyboxShader.setMat4("projection", projection);

        glBindVertexArray(skyBoxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture.textureID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureNight.textureID);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD,deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD,deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT,deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT,deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    // TODO: calculate how much cursor have moved, rotate camera proportional to the value, using ProcessMouseMovement.
    
    float xPos = static_cast<float>(xpos);
    float yPos = static_cast<float>(ypos);

    if(firstMouse){
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xoffset = xPos -lastX;
    float yoffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
