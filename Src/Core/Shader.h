#ifndef SHADER_H
#define SHADER_H

#include <Src/Core/Mat.h>
#include <Res/glad/glad.h>
#include <fstream>
#include <sstream>
#include <initializer_list>

using namespace std;

enum ShType { Frag, Vert };
struct ShMake { string path; ShType type; };

class Shader
{
private:
    unsigned id;
    unsigned type(ShType t)
    {
        switch(t)
        {
            case Frag: return GL_FRAGMENT_SHADER;
            case Vert: return GL_VERTEX_SHADER;
        } return -1;
    }
    GLint uLoc(const string name) const { return  glGetUniformLocation(id, name.c_str()); }
public:
    Shader(const Shader&) = delete;
    Shader() { id = glCreateProgram(); }
    Shader(initializer_list<ShMake> l): Shader() { for(auto& x : l) add(x.path, x.type); link(); }
    void add(string Path, ShType t)
    {
        ifstream file; file.open(Path);
        stringstream stream;
        stream << file.rdbuf(); file.close();
        string s = stream.str(); const char* code = s.c_str();

        unsigned shader = glCreateShader(type(t));
        glShaderSource(shader, 1, &code, NULL);
        glCompileShader(shader);

        int success; glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            char infoLog[512]; glGetShaderInfoLog(shader, 512, NULL, infoLog);
            cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << endl;
        }
        glAttachShader(id, shader);
        glDeleteShader(shader);
    }

    void link()
    {
        glLinkProgram(id);
        int success; glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success)
        {
            char infoLog[512]; glGetProgramInfoLog(id, 512, NULL, infoLog);
            cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << endl;
        }
    }

    void use() { glUseProgram(id); }

    void set(const string name, bool value)                     const { glUniform1i(uLoc(name), (int)value); }
    void set(const string name, int value)                      const { glUniform1i(uLoc(name), value); }
    void set(const string name, unsigned value)                 const { glUniform1ui(uLoc(name), value); }
    void set(const string name, float value)                    const { glUniform1f(uLoc(name), value); }
    void set(const string name, float v1, float v2, float v3)   const { glUniform3f(uLoc(name), v1, v2, v3); }
    void set(const string name, float v1, float v2)             const { glUniform2f(uLoc(name), v1, v2); }
    void set(const string name, Vec3& v)                         const { glUniform3f(uLoc(name), v.x, v.y, v.z); }
    void set(const string name, Vec2& v)                         const { glUniform2f(uLoc(name), v.x, v.y); }
    void set(const string name, Mat& value)                      const { glUniformMatrix4fv(uLoc(name), 1, GL_TRUE, value.raw()); }
};
#endif
