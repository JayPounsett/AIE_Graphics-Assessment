#include "Mesh.h"
#include "glad.h"
#include <assert.h>

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ibo);
}

void Mesh::initialiseQuad()
{
    // Check that the mesh is not already initialised
    assert(m_vao == 0);

    // Generate buffers
    glGenBuffers(1, &m_vbo);
    glGenVertexArrays(1, &m_vao);

    // Bind vertex array -> mesh wrapper
    glBindVertexArray(m_vao);

    // Bind vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // Defining two triangles for a quad
    Vertex vertices[6];

    vertices[0].position = { -0.5f, 0, 0.5f, 1 };
    vertices[1].position = { 0.5f, 0, 0.5f, 1 };
    vertices[2].position = { -0.5f, 0, -0.5f, 1 };

    vertices[3].position = { -0.5f, 0, 0.5f, 1 };
    vertices[4].position = { 0.5f, 0, 0.5f, 1 };
    vertices[5].position = { 0.5f, 0, -0.5f, 1 };

    // Fill the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    // Enable first element as position
    glDisableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    // Note to Future Self: glVertexAttribPointer
    // It will start at the first element (0), read in 4 floats,
    // step the size of the vertex over and then start again.

    // Unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Commented variable out as it is now set as a const variable with a value of 2 within Mesh.h
    // m_triCount = 2;
}

void Mesh::draw()
{
    glBindVertexArray(m_vao);

    // Using indices or just vertices
    if (m_ibo != 0)
    {
        glDrawElements(GL_TRIANGLES, 3 * m_triCount, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, 3 * m_triCount);
    }
}
