#ifndef BLOCK
#define BLOCK

#include <Src/Core/Shader.h>
#include <Src/Core/Texture.h>

#ifndef DMAX
#define DMAX 1e13f
#endif

class Object
{
private:
    static Vec2 x, y, z; static Vec3 p;
protected:
    Quat rot;
    Mat model;
public:
    Vec3 pos;
    Object(Quat rot, Vec3 pos): rot(rot), pos(pos) {}
    virtual Mat& Model() = delete; // { return model = {rot, pos}; }
    virtual void Start() {}
    virtual void Update() {}
    virtual void Draw() {}
    float Intersect(Vec3 Pos, Ray a)
    {
        a.normalize(); a = {1.0f / a.x, 1.0f / a.y, 1.0f / a.z}; 
        return FastIntersect(Pos, a);
    }
    float FastIntersect(const Vec3& Pos, const Ray& a)
    {
        p = {-Pos.x -pos.x, -Pos.y -pos.y, -Pos.z -pos.z};
        x = Vec2(p.x-0.5f, p.x+0.5f)*a.x;
        y = Vec2(p.y-0.5f, p.y+0.5f)*a.y;
        z = Vec2(p.z-0.5f, p.z+0.5f)*a.z;

        float dmin = max3(x.min(), y.min(), z.min());
        float dmax = min3(x.max(), y.max(), z.max());

        if(dmax < max(dmin, 0.0f))  return DMAX;
        else                        return dmin;
    }
}; Vec2 Object::x, Object::y, Object::z; Vec3 Object::p;

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
        sh.set("pos", pos);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
    }
};
float Block::Vertices[] = {
    #include "Res/cube/Cube"
};

Shader Block::sh({
    {"Src/Shaders/blockFrag.glsl", Frag}, 
    {"Src/Shaders/blockVert.glsl", Vert}
});

#endif