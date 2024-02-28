#include "Mesh.h"
#include "Shader.h"
#include "glad.h"
#include <assert.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <fstream>
#include <sstream>
#include <vector>

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}

void Mesh::InitialiseQuad() {
    // check that the mesh is not initialised already
    assert(vao == 0);

    // generate buffers
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    // bind vortex array -> mesh wrapper
    glBindVertexArray(vao);

    // bind vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Creating two triangles to make a square
    Vertex vertices[6];
    vertices[0].position = { -0.5f, 0, 0.5f, 1 };
    vertices[1].position = { 0.5f, 0, 0.5f, 1 };
    vertices[2].position = { -0.5f, 0, -0.5f, 1 };

    vertices[3].position = { -0.5f, 0, -0.5f, 1 };
    vertices[4].position = { 0.5f, 0, 0.5f, 1 };
    vertices[5].position = { 0.5f, 0, -0.5f, 1 };

    vertices[0].normal = { 0, 1, 0, 0 };
    vertices[1].normal = { 0, 1, 0, 0 };
    vertices[2].normal = { 0, 1, 0, 0 };
    vertices[3].normal = { 0, 1, 0, 0 };
    vertices[4].normal = { 0, 1, 0, 0 };
    vertices[5].normal = { 0, 1, 0, 0 };

    vertices[0].texCoord = { 0.0f, 1.0f };
    vertices[1].texCoord = { 1.0f, 1.0f };
    vertices[2].texCoord = { 0.0f, 0.0f };
    vertices[3].texCoord = { 0.0f, 0.0f };
    vertices[4].texCoord = { 1.0f, 1.0f };
    vertices[5].texCoord = { 1.0f, 0.0f };

    // fill vertex buffer
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    // enable first element as position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    // enable second element as normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

    // enable third element as texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

    // unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // quad has two triangles
    m_triCount = 2;
}

void Mesh::Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount,
                      unsigned int* indices) {
    // Check that the mesh is not initialised already
    assert(vao == 0);

    // Generate buffers
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    // Bind vortex array -> mesh wrapper
    glBindVertexArray(vao);

    // Bind vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Fill vertex buffer
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    // Enable the first element as position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    // Enable the second element as normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

    // Enable the third element as texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

    // Enable the fourth element as tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)40);

    // Bind indices if there are any
    if (indexCount != 0) {
        glGenBuffers(1, &ibo);

        // Bind indice buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        // Fill buffer
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices,
                     GL_STATIC_DRAW);

        m_triCount = indexCount / 3;
    } else
        m_triCount = vertexCount / 3;

    // Unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::InitialiseFromFile(const char* fileName) {
    // Read the vertices from the model
    const aiScene* scene = aiImportFile(fileName, 0); // i.e. Blender obj as the file

    // Just use the first mesh we find
    aiMesh* mesh = scene->mMeshes[0];

    // Extract the indices from the first mesh
    int numFaces = mesh->mNumFaces; // A face in this sense is a triangle and
                                    // therefore 3 indices.
    std::vector<unsigned int> indices;

    for (int i = 0; i < numFaces; i++) {
        // The order matters with mIndices as it determines which way it faces.
        // The renderer draws counter-clockwise.
        indices.push_back(mesh->mFaces[i].mIndices[0]);
        indices.push_back(mesh->mFaces[i].mIndices[2]);
        indices.push_back(mesh->mFaces[i].mIndices[1]);

        // Generate a second triangle for our quads (Blender tends to use quads)
        if (mesh->mFaces[i].mNumIndices == 4) {
            // If it has 4, it is a quad, creating a triangle with the fourth
            // index.
            indices.push_back(mesh->mFaces[i].mIndices[0]);
            indices.push_back(mesh->mFaces[i].mIndices[3]);
            indices.push_back(mesh->mFaces[i].mIndices[2]);
        }
    }

    // Extract vertex data
    int numV = mesh->mNumVertices;
    Vertex* vertices = new Vertex[numV];

    for (int i = 0; i < numV; i++) {
        vertices[i].position
            = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1);
        vertices[i].normal
            = glm::vec4(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 0);

        // UVs
        if (mesh->mTextureCoords[0]) {
            vertices[i].texCoord
                = glm::vec2(mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y);
            // 1.0 - mesh->mTexCoords is needed because the texture is inverted,
            // shouldn't be normally.
        } else
            vertices[i].texCoord = glm::vec2(0);

        if (mesh->HasTangentsAndBitangents()) {
            vertices[i].tangent
                = glm::vec4(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z, 1);
        }
    }

    if (!mesh->HasTangentsAndBitangents()) { CalculateTangents(vertices, numV, indices); }

    Initialise(numV, vertices, indices.size(), indices.data());

    delete[] vertices;
}

void Mesh::ApplyMaterial(aie::ShaderProgram* shader) {
    shader->bindUniform("Ka", Ka);
    shader->bindUniform("Kd", Kd);
    shader->bindUniform("Ks", Ks);
    shader->bindUniform("SpecularPower", SpecularPower);

    if (mapKd.getPixels() != nullptr) {
        mapKd.bind(0);
        shader->bindUniform("diffuseTex", 0);
    }
    if (mapKs.getPixels() != nullptr) {
        mapKd.bind(1);
        shader->bindUniform("specularTex", 1);
    }
    if (mapBump.getPixels() != nullptr) {
        mapBump.bind(2);
        shader->bindUniform("normalTex", 2);
    }
}

void Mesh::LoadMaterial(const char* fileName) {
    std::fstream file(fileName, std::ios::in);
    std::string line;
    std::string header;
    char buffer[256];

    // Get the path of the file name for us with relative paths for maps later.
    std::string directory(fileName);
    int index = directory.rfind('/');
    // rfind() searches in reverse, in this case starting at the end of the filename.
    if (index != -1) {
        directory = directory.substr(0, index + 1);
        // 0 is the last '/'in the path, therefore it will find the directory
        // Example: C:\GitHub\Project\Data\Model\...
    }

    while (!file.eof()) {
        // -= Idea (14/02/2024) =-
        // At this point, the values can be modified after reading them from the
        // file and another function could then write the new values back in.

        file.getline(buffer, 256);
        line = buffer;
        std::stringstream ss(line, std::stringstream::in | std::stringstream::out);
        if (line.find("Ka") == 0) ss >> header >> Ka.x >> Ka.y >> Ka.z;
        else if (line.find("Ks") == 0)
            ss >> header >> Ks.x >> Ks.y >> Ks.z;
        else if (line.find("Kd") == 0)
            ss >> header >> Kd.x >> Kd.y >> Kd.z;
        else if (line.find("Ns") == 0)
            ss >> header >> SpecularPower;
        else if (line.find("map_Kd") == 0) {
            std::string mapFileName;
            ss >> header >> mapFileName;
            mapKd.load((directory + mapFileName).c_str());
            printf(("Diffuse Texture Map:" + directory + mapFileName).c_str());
            printf("\n");
        } else if (line.find("map_Ks") == 0) {
            std::string mapFileName;
            ss >> header >> mapFileName;
            mapKs.load((directory + mapFileName).c_str());
            printf(("Specular Texture Map:" + directory + mapFileName).c_str());
            printf("\n");
        } else if (line.find("bump") == 0) {
            std::string mapFileName;
            ss >> header >> mapFileName;
            mapBump.load((directory + mapFileName).c_str());
            printf(("Bump Texture Map:" + directory + mapFileName).c_str());
            printf("\n");
        }
    }
}

void Mesh::CreateMaterial(glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, const char* filePath) {
    this->Ka = Ka;
    this->Kd = Kd;
    this->Ks = Ks;
    
    mapKd.load(filePath);
}

void Mesh::CalculateTangents(Vertex* vertices, unsigned int vertexCount,
                             const std::vector<unsigned int>& indices) {
    glm::vec4* tan1 = new glm::vec4[vertexCount * 2];
    glm::vec4* tan2 = tan1 + vertexCount;
    memset(tan1, 0, vertexCount * sizeof(glm::vec4) * 2);
    unsigned int indexCount = (unsigned int)indices.size();
    for (unsigned int a = 0; a < indexCount; a += 3) {
        long i1 = indices[a];
        long i2 = indices[a + 1];
        long i3 = indices[a + 2];
        const glm::vec4& v1 = vertices[i1].position;
        const glm::vec4& v2 = vertices[i2].position;
        const glm::vec4& v3 = vertices[i3].position;
        const glm::vec2& w1 = vertices[i1].texCoord;
        const glm::vec2& w2 = vertices[i2].texCoord;
        const glm::vec2& w3 = vertices[i3].texCoord;
        float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;
        float s1 = w2.x - w1.x;
        float s2 = w3.x - w1.x;
        float t1 = w2.y - w1.y;
        float t2 = w3.y - w1.y;
        float r = 1.0F / (s1 * t2 - s2 * t1);
        glm::vec4 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r,
                       0);
        glm::vec4 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r,
                       0);
        tan1[i1] += sdir;
        tan1[i2] += sdir;
        tan1[i3] += sdir;
        tan2[i1] += tdir;
        tan2[i2] += tdir;
        tan2[i3] += tdir;
    }
    for (unsigned int a = 0; a < vertexCount; a++) {
        const glm::vec3& n = glm::vec3(vertices[a].normal);
        const glm::vec3& t = glm::vec3(tan1[a]);
        // Gram-Schmidt orthogonalize
        vertices[a].tangent = glm::vec4(glm::normalize(t - n * glm::dot(n, t)), 0);
        // Calculate handedness (direction of bitangent)
        vertices[a].tangent.w
            = (glm::dot(glm::cross(glm::vec3(n), glm::vec3(t)), glm::vec3(tan2[a])) < 0.0F) ? 1.0F
                                                                                            : -1.0F;
    }
    delete[] tan1;
}

void Mesh::Draw() {
    glBindVertexArray(vao);

    // Using indices or just vertices
    if (ibo != 0) {
        glDrawElements(GL_TRIANGLES, 3 * m_triCount, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, 3 * m_triCount);
    }
}
