#ifndef MANAGER
#define MANAGER

#include <Res/glad/glad.h>
#include <Res/GLFW/glfw3.h>
#include <Src/Core/Mat.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <memory>

using namespace std;
typedef GLFWwindow *Window;

void processInput(Window);
void KeyCallback(Window, int , int , int , int);
void MouseCallback(Window, double, double);
void FBSizeCallback(Window, int, int);
void ScrollCallback(Window, double, double);
void ClickCallback(Window, int, int, int);

class Object;
class GUI;

class Watch
{
private:
	double time;
	double old;
    double d;
	map<string, long long> ch; 
public:
	Watch() : time(0.0), old(0.0), d(0.0) {}
	double delta()
	{
		old = time;
		time = glfwGetTime();
		return time - old; 
	}
	void mesureStart() { old = glfwGetTime(); }
	double mesureEnd() { return glfwGetTime() - old; }
    bool tik(int fps)
    {
        d += delta();
        return d > 1.0/fps ? d=0.0, true : false;
    }
	bool checkForChange(string file)
	{
		struct stat res;
		if(!stat(file.c_str(), &res) && res.st_mtime != ch[file]) 
			return (ch[file] = res.st_mtime);
		return false;
	}
};

struct
{
    vector<unique_ptr<Object>> world;
    vector<unique_ptr<GUI>> gui;
    Vec2 Resolution = {800, 450};
    Window window;
    Watch clock;
    int start { [&]()->int // must be first
    {
        if (!glfwInit()) return false;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        window = glfwCreateWindow(Resolution.x, Resolution.y, "ma-NeKraft", NULL, NULL);
        if (!window) return cout << "Failed to initialize GLFW" << endl, glfwTerminate(), -1;
        glfwMakeContextCurrent(window);
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            return cout << "Failed to initialize GLAD" << endl, -1;

        glfwSetFramebufferSizeCallback(window, FBSizeCallback);
        glfwSetCursorPosCallback(window, MouseCallback);
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetScrollCallback(window, ScrollCallback);
        glfwSetMouseButtonCallback(window, ClickCallback);
        glEnable(GL_DEPTH_TEST);
        
        return 0;
    }()};
} Manager;

#endif