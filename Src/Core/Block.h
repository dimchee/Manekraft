#ifndef BLOCK
#define BLOCK

#include <Src/Core/Shader.h>
#include <Src/Core/Texture.h>

class Object
{
protected:
    Quat rot;
    Vec3 pos;
    Mat model;
public:
    Object(Quat rot, Vec3 pos): rot(rot), pos(pos) {}
    virtual Mat& Model() { return model = {rot, pos}; }
    virtual void Start() {}
    virtual void Update() {}
    virtual void Draw() {}
};

class Block : public Object
{
private:
    unsigned VAO, VBO;
    Texture& tex;
    static float Vertices[5 * 6 * 6];
public:
    static Shader sh;
    Block(Vec3 pos, Texture& tex): Object({}, pos), tex(tex)
    {
        tex.bind("tex", 0, sh);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
    }
    void Draw()
    {
        sh.use(); tex.use();
        sh.set("model", Model());
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
    }
    Mat& Model() { return model = Mat::Trans(pos); }
};
float Block::Vertices[] = {
    #include "Res/cube/Cube"
};

Shader Block::sh({
    {"Src/Shaders/frag.glsl", Frag}, 
    {"Src/Shaders/vert.glsl", Vert}
});

#endif