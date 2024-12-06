#include "text.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

Text::Text(const std::string& fontPath,int fontSize) : alpha(1.0f) {

    createShaderAndLoadShader();
    loadCharacters(fontPath, fontSize);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Text::~Text() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader);
}
void Text::Render(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
    glUseProgram(shader);

    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 800.0f, -1.0f, 1.0f);
    GLint projLoc = glGetUniformLocation(shader, "projection");

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3f(glGetUniformLocation(shader, "textColor"), color.x, color.y, color.z);
    glUniform1f(glGetUniformLocation(shader, "alpha"), alpha);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (const char& c : text) {

        if (Characters.find(c) == Characters.end()) {
            std::cerr << "WARNING::TEXT_RENDERER: Character '" << c
                << "' (ASCII: " << static_cast<int>(c) << ") not found in Characters map." << std::endl;
            continue;
        }

        Character ch = Characters[c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        GLfloat vertices[6][4] = {
            { xpos,     ypos + h, 0.0f, 0.0f },
            { xpos,     ypos,     0.0f, 1.0f },
            { xpos + w, ypos,     1.0f, 1.0f },

            { xpos,     ypos + h, 0.0f, 0.0f },
            { xpos + w, ypos,     1.0f, 1.0f },
            { xpos + w, ypos + h, 1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glUseProgram(0);
}
void Text::loadCharacters(const std::string& fontPath, int fontSize) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not initialize FreeType Library" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
        FT_Done_FreeType(ft);
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    std::wstring charactersToLoad = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-/ ćĆ,";
    for (wchar_t c : charactersToLoad) {

        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    /*
    if (Characters.find('ć') != Characters.end()) {
        const Character& ch = Characters['ć'];
        std::cout << "'ć' loaded: texture ID = " << ch.TextureID
            << ", size = (" << ch.Size.x << ", " << ch.Size.y << ")"
            << ", bearing = (" << ch.Bearing.x << ", " << ch.Bearing.y << ")"
            << ", advance = " << ch.Advance << std::endl;
    }
    else {
        std::cerr << "'ć' not found in Characters map!" << std::endl;
    }
    */

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
void Text::createShaderAndLoadShader() {

    const char* vertex = R"(
        #version 330 core
        layout (location = 0) in vec4 vertex; // (x, y, z, u, v)

        out vec2 TexCoords;

        uniform mat4 projection;

        void main() {
            gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
            TexCoords = vertex.zw;
        }
    )";

    const char* fragment = R"(
        #version 330 core
        in vec2 TexCoords;
        out vec4 color;

        uniform sampler2D text;
        uniform vec3 textColor;
        uniform float alpha;

        void main() {
            vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r); 
            color = vec4(textColor, alpha) * sampled;
        }
    )";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Text vertex Shader Compilation Error:\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Text fragment Shader Compilation Error:\n" << infoLog << std::endl;
    }

    shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Text shader Linking Error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
float Text::getAlpha() const {
    return alpha;
}
void Text::reduceAlpha(float reduction) {
    if (alpha - reduction >= 0.0f)
    {
        alpha -= reduction;
    }
    else
    {
        alpha = 0.0f;
    }
}
void Text::setAlpha(float a) {
    alpha = a;
}