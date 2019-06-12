#ifndef GUI_
#define GUI_

#include <Src/Core/Mat.h>
#include <Src/Core/Shader.h>
#include <Src/Core/Texture.h>

#include <string>
#include <vector>

using namespace std;

class GUI
{
private:
    static float Vertices[4*6];
    Texture tex;
protected:
    unsigned VAO, VBO;
    Vec2 pos;
    Vec2 scale;
public:
    static Shader sh;
    GUI(string texPath, Vec2 pos, Vec2 scale): tex(texPath), pos(pos), scale(scale)
    {
        tex.bind("tex", 0, sh);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glBindVertexArray(0);
    }
    virtual void Draw()
    {
        sh.use(); tex.use();
        sh.set("pos", pos);
        sh.set("scale", scale);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    virtual void Click(Vec2 mpos)
    {
        if(
            pos.x - scale.x < mpos.x &&
            mpos.x < pos.x + scale.x &&
            -pos.y - scale.y < mpos.y &&
            mpos.y < -pos.y + scale.y
        ) OnClick();
    }
    virtual void OnClick() {}
};
float GUI::Vertices[] = {
    -1.0f, -1.0f, 0.0f, 0.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,
     1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
     1.0f,  1.0f, 1.0f, 1.0f,
     1.0f, -1.0f, 1.0f, 0.0f
};
Shader GUI::sh({
    {"Src/Shaders/guiFrag.glsl", Frag}, 
    {"Src/Shaders/guiVert.glsl", Vert}
});

class Text : public GUI
{
private:
    vector<float> Vertices;
    static Texture tex;
public:
    static Shader sh;
    Text(string s, Vec2 pos, Vec2 scale): GUI("", pos, scale)
    {
        for(unsigned i=0; i<s.length(); i++)
        {
            #define r16 1.0f/16.0f
            float uvX = r16*(s[i]%16);
            float uvY = r16*(s[i]/16);

            Vec2 vtUL(pos.x +  i   *scale.x/2, pos.y+scale.y);
            Vec2 vtUR(pos.x + (i+1)*scale.x/2, pos.y+scale.y);
            Vec2 vtDR(pos.x + (i+1)*scale.x/2, pos.y        );
            Vec2 vtDL(pos.x +  i   *scale.x/2, pos.y        );

            Vec2 uvUL(uvX          , 1.0f - uvY      );
            Vec2 uvUR(uvX + r16/1.5, 1.0f - uvY      );
            Vec2 uvDR(uvX + r16/1.5, 1.0f - uvY - r16);
            Vec2 uvDL(uvX          , 1.0f - uvY - r16);
            #undef r16

            #define push(a) Vertices.push_back(a.x), Vertices.push_back(a.y)
            push(vtUL), push(uvUL);
            push(vtDL), push(uvDL);
            push(vtUR), push(uvUR);

            push(vtDR), push(uvDR);
            push(vtUR), push(uvUR);
            push(vtDL), push(uvDL);
            #undef push
        }
        tex.bind("tex", 0, sh);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glBindVertexArray(0);
    }

    void Draw()
    {
        sh.use(); tex.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, Vertices.size()/4);
        glBindVertexArray(0);
    }
}; Texture Text::tex("Res/font.bmp");

Shader Text::sh({
    {"Src/Shaders/textFrag.glsl", Frag},
    {"Src/Shaders/textVert.glsl", Vert}
});

#endif