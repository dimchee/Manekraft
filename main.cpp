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

    Camera cam({0.0, 0.0, 0.0}, 16.0/9);
    {
        for(double i=-25; i<=25; i++)
            for(double j=-25; j<=25; j++)
                Manager.world.emplace_back(new Grass({i, -1.0+sin(i+j)*1.0f, j}));
        Manager.gui.emplace_back(new Button({0.5, 0.5}, {0.2f, 0.2}));
    }

    Text text("Test, so it is working :D", -1, -1, 0.05f);

    for(auto& x : Manager.world) x->Start();
	while (!glfwWindowShouldClose(Manager.window)) if(Manager.clock.tik(60))
	{
		processInput(Manager.window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cam.Update(Block::sh);
        for(auto& x : Manager.world) x->Update();
        for(auto& x : Manager.world) x->Draw();
        if(Manager.mode == Mode::gui)
            for(auto& x : Manager.gui) x->Draw();
        text.Draw();
        
        glfwSwapBuffers(Manager.window);
        glfwPollEvents();
	}
    else glfwPollEvents();
	return glfwTerminate(), 0;
}