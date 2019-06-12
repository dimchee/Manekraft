#ifndef INPUT
#define INPUT

#include <Src/Core/Manager.h>
#include <Src/Core/Camera.h>
#include <Src/Core/GUI.h>

using namespace std;
typedef GLFWwindow *Window;

void processInput(Window wind)
{
    //if(glfwGetKey(wind, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(wind, true);

    if(Manager.mode == Mode::fps)
    {
        Vec3 p; float cameraSpeed = 0.02f;
        if(glfwGetKey(wind, GLFW_KEY_W) == GLFW_PRESS)
            p -= Camera::main->Dir() * cameraSpeed;
        if(glfwGetKey(wind, GLFW_KEY_S) == GLFW_PRESS)
            p += Camera::main->Dir() * cameraSpeed;
        if(glfwGetKey(wind, GLFW_KEY_A) == GLFW_PRESS)
            p -= Camera::main->Rig() * cameraSpeed;
        if(glfwGetKey(wind, GLFW_KEY_D) == GLFW_PRESS)
            p += Camera::main->Rig() * cameraSpeed;
        if(glfwGetKey(wind, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            p -= Camera::main->Up() * cameraSpeed;
        if(glfwGetKey(wind, GLFW_KEY_SPACE) == GLFW_PRESS)
            p += Camera::main->Up() * cameraSpeed;
        Camera::main->Move(p);
    }
}

void KeyCallback(Window wind, int key, int /*scancode*/, int action, int /*mods*/)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        if(glfwGetInputMode(wind, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
            glfwSetInputMode(wind, GLFW_CURSOR, GLFW_CURSOR_DISABLED),
            glfwSetCursorPos(wind, 0, 0), Manager.mode = Mode::fps;
        else 
            glfwSetInputMode(wind, GLFW_CURSOR, GLFW_CURSOR_NORMAL),
            Manager.mode = Mode::gui;
    }
}

void MouseCallback(Window wind, double xpos, double ypos)
{
    float sens = 0.001f;
    if(glfwGetInputMode(wind, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
    {
        Camera::main->Rotate(sens*xpos, sens*ypos);
        glfwSetCursorPos(wind, 0, 0);
    }
}
void FBSizeCallback(Window, int width, int height)
{
    glViewport(0, 0, width, height); 
    Manager.Resolution = {(float)width, (float)height};
    Mat& m = Camera::main->Proj();
    m[0][0] = m[1][1]/width*height;
}
void ScrollCallback(Window, double, double yoffset)
{
    float s = 0.1f;
    Mat& m = Camera::main->Proj();
    if(3.0f < m[1][1] && yoffset < 0.0) // zoomout
    {
        m[0][0] /= m[1][1];
        m[1][1] += 1.0/yoffset*s;
        m[0][0] *= m[1][1];
    }
    if(m[1][1] < 30.0f && 0.0 < yoffset) // zoomin
    {
        m[0][0] /= m[1][1];
        m[1][1] += 1.0/yoffset*s;
        m[0][0] *= m[1][1];
    }
}

void ClickCallback(GLFWwindow* wind, int button, int action, int)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if(Manager.mode == Mode::gui) 
        {
            double xpos, ypos;
            glfwGetCursorPos(wind, &xpos, &ypos);
            for(auto& x : Manager.gui)
                x->Click({
                    float(xpos)/Manager.Resolution.x*2.0f - 1.0f, 
                    float(ypos)/Manager.Resolution.y*2.0f - 1.0f
                });
        }
    }
}

#endif