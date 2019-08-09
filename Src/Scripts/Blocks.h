#ifndef BLOCKS
#define BLOCKS

#include <Src/Core/Block.h>
#include <Src/Core/GUI.h>


class Grass : public Block
{
private:
    static Texture tex;
public:
    Grass(Vec3 pos): Block(pos, tex) {}
    void Start() { /*cout << "started\n";*/  }
    void Update() { /*nothing*/ }
};
Texture Grass::tex("Res/container.jpg");

class Button : public GUI
{
public:
    Button(Vec2 pos, Vec2 scale): GUI("Res/box.jpg", pos, scale) {}
    void OnClick() { Manager.chMode(); }
};

#endif