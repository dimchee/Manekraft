#ifndef TEXTURE
#define TEXTURE

#define STB_IMAGE_IMPLEMENTATION

#include <Src/Core/Shader.h>
#include <Res/stb/stb_image.h>

using namespace std;

class Texture
{
private:
    unsigned id;
    string type;
    string ext(string& Path)
    {
        auto i = Path.find_last_of(".");
        return (i == string::npos)?  "" : Path.substr(i+1);
    }

    unsigned mode(string Path)
    {
        if(ext(Path) == "jpg") return GL_RGB;
        if(ext(Path) == "png") return GL_RGBA;
        if(ext(Path) == "bmp") return GL_RGB;
        return 0;
    }
public:
    const unsigned ID = id;
    string Type = type;
    Texture(string Path, string Type = "unknown") : type(Type)
    {
        stbi_set_flip_vertically_on_load(true);
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int width, height, nrChannels;
        unsigned char *data = stbi_load(Path.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, mode(Path), GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else cout << "ERROR Failed to load texture" << endl;
        stbi_image_free(data);
    }
    Texture(const Texture&) = delete;
    void use(int slot = 0) { glActiveTexture(GL_TEXTURE0 + slot); glBindTexture(GL_TEXTURE_2D, id); }
    static void bind(string name, int slot, Shader& sh) { sh.use(); sh.set(name, slot); }
};

#endif
