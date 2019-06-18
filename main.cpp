#include <Src/Core/Manager.h> // need to be first include
#include <Src/Core/Mat.h>
#include <Src/Core/Shader.h>
#include <Src/Core/Texture.h>
#include <Src/Core/Input.h>
#include <Src/Core/Block.h>
#include <Src/Core/GUI.h>

#include <iostream>
#include <vector>

#include <Src/Scripts/Scripts.h>

int main()//int argc, char **argv)
{
    if(Manager.start) return Manager.start;

    Camera cam({0.0, 0.0, 0.0}, 1.0f, 16.0f/9);
    {
        for(int i=-25; i<=25; i++)
            for(int j=-25; j<=25; j++)
                Manager.world.emplace_back(new Grass({i, int(-1.0+sin(i+j)*1.6f), j}));
        Manager.gui.emplace_back(new Text("Resume", {-0.15, -0.05}, {0.1f, 0.1f}));
        Manager.gui.emplace_back(new Button({0.0, 0.0}, {0.2, 0.2}));
        Manager.gui.emplace_back(new GUI("Res/blur.png",{}, {1.0, 1.0}));
    }

    for(auto& x : Manager.world) x->Start();
	while (!glfwWindowShouldClose(Manager.window)) if(Manager.clock.tik(60))
	{
		processInput(Manager.window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cam.Update(Block::sh); cam.Intersect();
        for(auto& x : Manager.world) if(x)
            x->Update(), x->Draw();
        else if(!Manager.tmp.empty())
            x = std::move(Manager.tmp.front()), Manager.tmp.pop();
        while(!Manager.tmp.empty())
            Manager.world.emplace_back(std::move(Manager.tmp.front())), Manager.tmp.pop();

        if(Manager.mode == Mode::gui)
            for(auto& x : Manager.gui) x->Draw();
        
        glfwSwapBuffers(Manager.window);
        glfwPollEvents();
	}
    else glfwPollEvents();
	return glfwTerminate(), 0;
}