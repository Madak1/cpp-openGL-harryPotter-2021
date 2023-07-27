#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

class Texture{
private:
    unsigned int ID;
    std::string filepath;
    unsigned char* localBuffer;
    int width, height, bitPerPixel;
public:
    Texture(const std::string& path)
    : ID(0), filepath(path), localBuffer(nullptr), width(0), height(0), bitPerPixel(0){
        stbi_set_flip_vertically_on_load(1);
        localBuffer = stbi_load(path.c_str(), &width, &height, &bitPerPixel, 4);

        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);
        if(localBuffer) stbi_image_free(localBuffer);
    }
    ~Texture(){
        glDeleteTextures(1, &ID);
    }

    void Bind(unsigned int slot = 0) const{
        glActiveTexture(GL_TEXTURE0+slot);
        glBindTexture(GL_TEXTURE_2D, ID);
    }
    void Unbind() const{
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

#endif
