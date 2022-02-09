// Appication.cpp : Defines the entry point for the application.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include "Renderer/Shader.h"
#include "Renderer/Camera.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();

// Window dimensions
const GLuint WIDTH = 960, HEIGHT = 600;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool    keys[1024];

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main(void)
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize OpenGL context" << std::endl;
    return -1;
  }

  // Set the required callback functions
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // GLFW Options
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Define the viewport dimensions
  glViewport(0, 0, WIDTH, HEIGHT);

  // OpenGL options
  glEnable(GL_DEPTH_TEST);

  // Build and compile our shader program
  Hazel::Ref<Shader> lightingShader = Hazel::CreateRef<Shader>(AssetsDir + "/assets/shaders/lighting.glsl");
  Hazel::Ref<Shader> lampShader = Hazel::CreateRef<Shader>(AssetsDir + "/assets/shaders/lamp.glsl");

  // Set up vertex data (and buffer(s)) and attribute pointers
  GLfloat vertices[] = {
    // Positions          // Normals           // Texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
  };

  // First, set the container's VAO (and VBO)
  GLuint VBO, containerVAO;
  glGenVertexArrays(1, &containerVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(containerVAO);
  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  // Normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glBindVertexArray(0);

  // Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
  GLuint lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  // We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // Set the vertex attributes (only position data for the lamp))
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  // Load textures
  GLuint diffuseMap, specularMap;
  glGenTextures(1, &diffuseMap);
  glGenTextures(1, &specularMap);
  int width, height, channels;
  unsigned char* image;
  // Diffuse map
  stbi_set_flip_vertically_on_load(1);
  image = stbi_load((AssetsDir + "/assets/textures/container2.png").c_str(), &width, &height, &channels, 0);
  HZ_CORE_ASSERT(image, "Failed to load image!");

  glBindTexture(GL_TEXTURE_2D, diffuseMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(image);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

  image = stbi_load((AssetsDir + "/assets/textures/container2_specular.png").c_str(), &width, &height, &channels, 0);
  glBindTexture(GL_TEXTURE_2D, specularMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(image);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

  glBindTexture(GL_TEXTURE_2D, 0);

  lightingShader->Bind();
  lightingShader->UploadUniformInt("material.diffuse", 0);
  lightingShader->UploadUniformInt("material.specular", 1);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    // Calculate deltatime of current frame
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    /* Poll for and process events */
    glfwPollEvents();
    do_movement();

    // Clear the colorbuffer
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*glm::vec3 lightColor; lightColor.x = sin(glfwGetTime() * 2.0f);
    lightColor.y = sin(glfwGetTime() * 0.7f);
    lightColor.z = sin(glfwGetTime() * 1.3f);*/

    /*glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);*/

    // Use cooresponding shader when setting uniforms/drawing objects
    lightingShader->Bind();
    lightingShader->UploadUniformFloat3("light.position", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader->UploadUniformFloat3("viewPos", camera.Position);

    //lightingShader->UploadUniformFloat3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader->UploadUniformFloat("material.shininess", 64.0f);

    lightingShader->UploadUniformFloat3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    lightingShader->UploadUniformFloat3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader->UploadUniformFloat3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    /*GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
    glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
    glUniform3f(lightColorLoc, 1.0f, 0.5f, 1.0f);*/

    // Create camera transformations
    glm::mat4 view;
    view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
    // Get the uniform locations
    lightingShader->UploadUniformMat4("view", glm::mat4(view));
    lightingShader->UploadUniformMat4("projection", glm::mat4(projection));

    //GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
    //GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
    //GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
    //// Pass the matrices to the shader
    //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    //glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    // Draw the container (using container's vertex attributes)
    glBindVertexArray(containerVAO);
    glm::mat4 model(1.0f);
    //model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    lightingShader->UploadUniformMat4("model", glm::mat4(model));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // Also draw the lamp object, again binding the appropriate shader
    lampShader->Bind();
    // Get location objects for the matrices on the lamp shader (these could be different on a different shader)

    lampShader->UploadUniformMat4("view", glm::mat4(view));
    lampShader->UploadUniformMat4("projection", glm::mat4(projection));
    /*modelLoc = glGetUniformLocation(lampShader.Program, "model");
    viewLoc = glGetUniformLocation(lampShader.Program, "view");
    projLoc = glGetUniformLocation(lampShader.Program, "projection");*/
    // Set matrices
    /*glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));*/
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
    lampShader->UploadUniformMat4("model", glm::mat4(model));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    // Draw the light object (using light's vertex attributes)
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (key >= 0 && key < 1024)
  {
    if (action == GLFW_PRESS)
      keys[key] = true;
    else if (action == GLFW_RELEASE)
      keys[key] = false;
  }
}

void do_movement()
{
  // Camera controls
  if (keys[GLFW_KEY_W])
    camera.ProcessKeyboard(UP, deltaTime);
  if (keys[GLFW_KEY_S])
    camera.ProcessKeyboard(DOWN, deltaTime);
  if (keys[GLFW_KEY_A])
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (keys[GLFW_KEY_D])
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  GLfloat xoffset = xpos - lastX;
  GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  camera.ProcessMouseScroll(yoffset);
}
