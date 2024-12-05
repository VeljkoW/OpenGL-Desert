#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>

struct Character {
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};

class Text {
    public:
        Text(const std::string& fontPath, int fontSize);
        ~Text();
        void Render(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
        float getAlpha() const;
        void setAlpha(float a);
        void reduceAlpha(float a);

    private:
        GLuint VAO, VBO;
        GLuint shader;
        float alpha;
        void createShaderAndLoadShader();
        void loadCharacters(const std::string& fontPath, int fontSize);
        std::map<char, Character> Characters;
};

#endif