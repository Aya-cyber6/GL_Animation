#pragma once

#include <technique.h>
#include <ogldev_math_3d.h>

class DrawDebugAxesClass : public Technique {
public:
    bool Init() {
        if (!Technique::Init()) {
            return false;
        }

        if (!AddShader(GL_VERTEX_SHADER, "Shaders/debug_axes.vs")) {
            return false;
        }

        if (!AddShader(GL_FRAGMENT_SHADER, "Shaders/debug_axes.fs")) {
            return false;
        }

        if (!Finalize()) {
            return false;
        }

        // One-time VAO/VBO setup
        float length = 1.0f;
        float vertices[] = {
            // Position         // Color
            0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
            length, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
            0.0f, length, 0.0f, 0.0f, 1.0f, 0.0f,

            0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, length, 0.0f, 0.0f, 1.0f
        };

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

        glBindVertexArray(0);

        return true;
    }

    void Draw(const Matrix4f& VP) {
        Enable();
        glUniformMatrix4fv(GetUniformLocation("gVP"), 1, GL_TRUE, (const GLfloat*)VP.m);
        glBindVertexArray(m_vao);
        glDrawArrays(GL_LINES, 0, 6);
        glBindVertexArray(0);
    }

private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
};
