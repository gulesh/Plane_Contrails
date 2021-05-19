// Bryn Mawr College, alinen, 2020
//

#include "AGL.h"
#include "AGLM.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include "mesh.h"
#include "skybox.h"
#include "osutils.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "myparticlesystem.h"
#include "renderer.h"

using namespace std;
using namespace glm;
using namespace agl;

// globals
MyParticleSystem theSystem;
Mesh theModel;
int theCurrentModel = 0;
vector<string> theModelNames;

//these will be used for clamping
vec3 minDimentions;
vec3 offset;
vec3 currentPlanePos;
vec3 maxDimentions;
vec3 velocityDir(1.0, 1.0, 1.0);

mat4 projectionMatrix;
mat4 modelMatrix;
glm::mat4 mv;
mat4 viewMatrix;

float heightGlobal;
float widthGlobal;

float centerPlaneX = 0;
float centerPlaneY = 0;
float centerPlaneZ = 0;
float scaleX = 0;
float scaleY = 0;
float scaleZ = 0;

float maxValueXY = 0;
float maxValueXZ = 0;
float finalMax = 0;

float clickPosX = 0;
float clickPosY = 0;
float changeX = 0;
float changeY = 0;
double scrollPosX = 0;
double scrollPosY = 0;
float dist = 1.0f;
float Azimuth = 0.0f;
float Elevation = 0.0f;

// OpenGL IDs
GLuint theVboPosId;
GLuint theVboNormalId;
GLuint theElementbuffer;

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;

    if (key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // Prevent a divide by zero
    if (height == 0)
        height = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, width, height);
    ParticleSystem::GetRenderer().perspective(radians(60.0f), 1.0f, 0.1f, 100.0f);
}

static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
}

static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // TODO: Camera controls
    clickPosX = xpos;
    clickPosY = ypos;

    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS)
    {
        int keyPress = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
        if (keyPress == GLFW_PRESS)
        {
        }
    }
    else if (state == GLFW_RELEASE)
    {
    }
}

static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    double xFpos, yFpos;
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state != GLFW_PRESS)
    {
    }
    else
    {
        if (xFpos > radians(180.0))
            xFpos = radians(180.0);
        if (xFpos < radians(-180.0))
            xFpos = radians(-180.0);
        if (yFpos > radians(90.0))
            yFpos = radians(90.0);
        if (yFpos < radians(-90.0))
            yFpos = radians(-90.0);

        int keyPress = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
        if (keyPress == GLFW_PRESS)
        {

            glfwGetCursorPos(window, &xFpos, &yFpos);
            scrollPosX = xFpos;
            scrollPosY = yFpos;

            dist = dist + .01 * (scrollPosY - clickPosY);

            clickPosX = scrollPosX;
            clickPosY = scrollPosY;
        }
        {

            glfwGetCursorPos(window, &xFpos, &yFpos);
            scrollPosX = xFpos;
            scrollPosY = yFpos;

            float changeX = scrollPosX - clickPosX;
            float changeY = scrollPosY - clickPosY;

            clickPosX = scrollPosX;
            clickPosY = scrollPosY;

            Azimuth = Azimuth + .01 * changeX;
            Elevation = Elevation + changeY * .01;

            // cout << Azimuth << endl;
            // cout << Elevation << endl;
        }
    }
}

static void PrintShaderErrors(GLuint id, const std::string label)
{
    std::cerr << label << " failed\n";
    GLint logLen;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0)
    {
        char *log = (char *)malloc(logLen);
        GLsizei written;
        glGetShaderInfoLog(id, logLen, &written, log);
        std::cerr << "Shader log: " << log << std::endl;
        free(log);
    }
}

static std::string LoadShaderFromFile(const std::string &fileName)
{
    std::ifstream file(fileName);
    if (!file)
    {
        std::cout << "Cannot load file: " << fileName << std::endl;
        return "";
    }

    std::stringstream code;
    code << file.rdbuf();
    file.close();

    return code.str();
}

static void LoadModel(const std::string &dir)
{
    std::vector<std::string> filenames = GetFilenamesInDir(dir, "ply");
    std::string filename = filenames[0];
    theModelNames.push_back(dir + filename);
    cout << dir + filename << endl;

    theModel.loadPLY(theModelNames[0]);

    minDimentions = theModel.getMinBounds();
    maxDimentions = theModel.getMaxBounds();
    // cout << minDimentions << "minDim" << endl;
    // cout << maxDimentions << "minDim" << endl;

    // float centerPlaneX = 0;
    // float centerPlaneY = 0;
    // float centerPlaneZ = 0;

    // //translation and scaling the model matrix
    scaleX = fabs((maxDimentions.x - minDimentions.x));
    scaleY = fabs((maxDimentions.y - minDimentions.y));
    scaleZ = fabs((maxDimentions.z - minDimentions.z));

    maxValueXY = std::max(scaleX, scaleY);
    maxValueXZ = std::max(scaleX, scaleZ);
    finalMax = std::max(maxValueXY, maxValueXZ);

    centerPlaneX = (maxDimentions.x + minDimentions.x) / 2;
    centerPlaneY = (maxDimentions.y + minDimentions.y) / 2;
    centerPlaneZ = (maxDimentions.z + minDimentions.z) / 2;

    // cout << centerPlaneX << " centerX" << endl;
    // cout << centerPlaneY << " centery" << endl;
    // cout << centerPlaneZ << " centerz" << endl;

    // vec3 lookAt
    glm::mat4 scaled = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / finalMax, 1.0f / finalMax, 1.0f / finalMax));
    glm::mat4 translated = glm::translate(glm::mat4(1.0f), glm::vec3(-centerPlaneX, -centerPlaneY, -centerPlaneZ));
    modelMatrix = scaled * translated;

    currentPlanePos = glm::vec3(((maxDimentions.x - minDimentions.x) / 2) / finalMax, ((maxDimentions.y - minDimentions.y) / 2) / finalMax, ((maxDimentions.z - minDimentions.z) / 2) / finalMax);
    // offset = glm::vec3( (maxDimentions.x + centerPlaneX)/finalMax, (maxDimentions.y + centerPlaneY)/finalMax, (maxDimentions.z + centerPlaneZ)/finalMax);
    // offset = glm::vec3(0, 0, (maxDimentions.x - (maxDimentions.x - minDimentions.x) / 2)/finalMax);
    offset = glm::vec3(0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, theVboPosId);
    glBufferData(GL_ARRAY_BUFFER, theModel.numVertices() * 3 * sizeof(float), theModel.positions(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, theVboNormalId);
    glBufferData(GL_ARRAY_BUFFER, theModel.numVertices() * 3 * sizeof(float), theModel.normals(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theElementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, theModel.numTriangles() * 3 * sizeof(unsigned int), theModel.indices(), GL_DYNAMIC_DRAW);
}

static GLuint LoadShader(const std::string &vertex, const std::string &fragment)
{
    GLint result;
    std::string vertexShader = LoadShaderFromFile(vertex);
    const char *vertexShaderRaw = vertexShader.c_str();
    GLuint vshaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshaderId, 1, &vertexShaderRaw, NULL);
    glCompileShader(vshaderId);
    glGetShaderiv(vshaderId, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        PrintShaderErrors(vshaderId, "Vertex shader");
        return -1;
    }

    std::string fragmentShader = LoadShaderFromFile(fragment);
    const char *fragmentShaderRaw = fragmentShader.c_str();
    GLuint fshaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshaderId, 1, &fragmentShaderRaw, NULL);
    glCompileShader(fshaderId);
    glGetShaderiv(fshaderId, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        PrintShaderErrors(fshaderId, "Fragment shader");
        return -1;
    }

    GLuint shaderId = glCreateProgram();
    glAttachShader(shaderId, vshaderId);
    glAttachShader(shaderId, fshaderId);
    glLinkProgram(shaderId);
    glGetShaderiv(shaderId, GL_LINK_STATUS, &result);
    if (result == GL_FALSE)
    {
        PrintShaderErrors(shaderId, "Shader link");
        return -1;
    }
    return shaderId;
}

int main(int argc, char **argv)
{
    GLFWwindow *window;

    if (!glfwInit())
    {
        return -1;
    }

    // Explicitly ask for a 4.0 context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    float width = 500;
    float height = 500;
    window = glfwCreateWindow(width, height, "Mesh Viewer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

#ifndef APPLE
    if (glewInit() != GLEW_OK)
    {
        return -1;
    }
#endif

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0, 0, 0, 1);

    glGenBuffers(1, &theVboPosId);
    glGenBuffers(1, &theVboNormalId);
    glGenBuffers(1, &theElementbuffer);

    GLuint vaoId;
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    glEnableVertexAttribArray(0);               // 0 -> Sending VertexPositions to array #0 in the active shader
    glBindBuffer(GL_ARRAY_BUFFER, theVboPosId); // always bind before setting data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    glEnableVertexAttribArray(1);                  // 1 -> Sending Normals to array #1 in the active shader
    glBindBuffer(GL_ARRAY_BUFFER, theVboNormalId); // always bind before setting data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    LoadModel("../model/"); //modified the loadModels function to just load one model

    GLuint shaderPlaneId = LoadShader("../shaders/phongPerFrag.vs", "../shaders/phongPerFrag.fs");
    glUseProgram(shaderPlaneId);

    // GLuint shaderPlaneId = ParticleSystem::GetRenderer().loadShader("../shaders/phongPerFrag.vs", "../shaders/phongPerFrag.fs");

    //Per Vertex shader
    GLuint mvpId = glGetUniformLocation(shaderPlaneId, "uMVP"); //MVP
    GLuint mvmId = glGetUniformLocation(shaderPlaneId, "uMV");  //ModelViewMatrix
    GLuint nmvId = glGetUniformLocation(shaderPlaneId, "uNMV"); //NormalMAtrix

    //PerFragment Shader
    glUniform3f(glGetUniformLocation(shaderPlaneId, "Ks"), 0.45, 0.45, 0.45);
    glUniform3f(glGetUniformLocation(shaderPlaneId, "Kd"), 0.45, 0.45, 0.45);
    glUniform3f(glGetUniformLocation(shaderPlaneId, "Ka"), 0.1, 0.1, 0.1);
    glUniform1f(glGetUniformLocation(shaderPlaneId, "Shininess"), 80.0f);

    glUniform4f(glGetUniformLocation(shaderPlaneId, "uLightPosition"), 100.0, 100.0, 100.0, 1.0);
    glUniform3f(glGetUniformLocation(shaderPlaneId, "color"), 1.0, 1.0, 1.0);

    glClearColor(0, 0, 0, 1);

    glm::vec3 lookfrom(0, 0, 1);
    viewMatrix = glm::lookAt(lookfrom, vec3(0), vec3(0, 1, 0));
    // projectionMatrix = glm::ortho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
    projectionMatrix = glm::perspective(glm::radians(60.0f), (float)width / height, 0.01f, 100.0f);
    glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
    glUniformMatrix4fv(mvpId, 1, GL_FALSE, &mvp[0][0]);

    GLuint shaderParticleId = theSystem.GetRenderer().returnShaderId();
    glUseProgram(shaderParticleId);

    //the particle system setup
    theSystem.setoffset(offset);
    theSystem.setCurrentPlanePosition(currentPlanePos);
    theSystem.init(300); // TODO: Set number of particles here
    float fov = radians(60.0f);
    ParticleSystem::GetRenderer().perspective(fov, (float)width / height, 0.01f, 100.0f);
    ParticleSystem::GetRenderer().lookAt(lookfrom, vec3(0, 0, 0));
    float lastTime = glfwGetTime();

    // cout << "offsett: " << theSystem.getOffset() << "curentposition" << theSystem.getCurrentPlanePosition() << endl;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers

        //update transform
        lookfrom.x = dist * sin(Azimuth) * cos(Elevation);
        lookfrom.y = dist * sin(Elevation);
        lookfrom.z = dist * cos(Azimuth) * cos(Elevation);

        // particle draw
        glUseProgram(shaderParticleId);

        float dt = 1 * (glfwGetTime() - lastTime);
        lastTime = glfwGetTime();

        theSystem.update(dt);
        theSystem.draw();

        ParticleSystem::GetRenderer().lookAt(lookfrom, vec3(0, 0, 0));

        //meshsetup
        glUseProgram(shaderPlaneId);

        // //setMVP
        // vec3 newminDimentions = minDimentions + velocityDir * dt;
        // float minvalueX = std::min(newminDimentions.x, minDimentions.x);
        // float minvalueY = std::min(newminDimentions.y, minDimentions.y);
        // float minvalueZ = std::min(newminDimentions.z, minDimentions.z);

        // vec3 newmaxDimentions = maxDimentions + velocityDir * dt;
        // float maxvalueX = std::min(newmaxDimentions.x, maxDimentions.x);
        // float maxvalueY = std::min(newmaxDimentions.y, maxDimentions.y);
        // float maxvalueZ = std::min(newmaxDimentions.z, maxDimentions.z);

        // minDimentions = vec3(minvalueX, minvalueY, minvalueZ);
        // maxDimentions = vec3(maxvalueX, maxvalueY, maxvalueZ);

        // // cout<< newminDimentions <<endl;

        // // //translation and scaling the model matrix
        // scaleX = fabs((maxDimentions.x - minDimentions.x));
        // scaleY = fabs((maxDimentions.y - minDimentions.y));
        // scaleZ = fabs((maxDimentions.z - minDimentions.z));

        // maxValueXY = std::max(scaleX, scaleY);
        // maxValueXZ = std::max(scaleX, scaleZ);
        // finalMax = std::max(maxValueXY, maxValueXZ);

        // centerPlaneX = (maxDimentions.x + minDimentions.x) / 2;
        // centerPlaneY = (maxDimentions.y + minDimentions.y) / 2;
        // centerPlaneZ = (maxDimentions.z + minDimentions.z) / 2;

        // currentPlanePos = currentPlanePos + velocityDir * dt;
        // // cout << currentPlanePos << endl;

        // vec3 lookAt =
        // glm::mat4 scaled = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / finalMax, 1.0f / finalMax, 1.0f / finalMax));
        // glm::mat4 translated = glm::translate(glm::mat4(1.0f), glm::vec3(-centerPlaneX, -centerPlaneY, -centerPlaneZ));
        // glm::mat4 planeMovement = glm::translate(glm::mat4(1.0f), currentPlanePos); // set position here!
        // modelMatrix = planeMovement * scaled * translated;
        // // modelMatrix = scaled * translated;
        viewMatrix = glm::lookAt(lookfrom, vec3(0), vec3(0, 1, 0));
        mv = viewMatrix * modelMatrix;

        glm::mat4 mvp = projectionMatrix * mv;
        mat3 nmv = mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]));

        glUniformMatrix4fv(mvpId, 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix4fv(mvmId, 1, GL_FALSE, &mv[0][0]);
        glUniformMatrix3fv(nmvId, 1, GL_FALSE, &nmv[0][0]);

        // Draw primitive
        glBindVertexArray(vaoId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theElementbuffer);
        glDrawElements(GL_TRIANGLES, theModel.numTriangles() * 3, GL_UNSIGNED_INT, (void *)0);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    // system("leaks main-scene");
    return 0;
}
