#pragma once

#include <string>
#include <vector>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GLM/glm.hpp>

#include "Texture.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct Vertex
{
    glm::vec3 pos;
    glm::vec2 tex;
    glm::vec3 normal;
    glm::vec3 tangent;

    Vertex() {}
    Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal, const glm::vec3& tangent)
        : pos(pos), tex(tex), normal(normal), tangent(tangent)
	{}
};

struct Material {
    std::string name;
    std::unique_ptr<Texture> diffuseTexture;
    std::unique_ptr<Texture> normalTexture;
    void Bind() {
        if (diffuseTexture) diffuseTexture->Bind();
        if (normalTexture) normalTexture->Bind();
    }
    void Unbind() {
        if (diffuseTexture) diffuseTexture->Unbind();
        if (normalTexture) normalTexture->Unbind();
    }
};

class Mesh
{
public:
    struct MeshEntry {
        MeshEntry();
        ~MeshEntry();
        MeshEntry(const MeshEntry&) = default;
        void InitEntry(const std::vector<Vertex>& verts, const std::vector<unsigned int>& indices);
        unsigned int vao;
        std::shared_ptr<VertexBuffer> vb;
        std::shared_ptr<IndexBuffer> ib;
        unsigned int numIndices;
        unsigned int matIndex;
    };

private:
	bool InitMeshFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();
    void RenderBoundingBox(glm::vec3 boundingMin, glm::vec3 boundingMax);
public:
    std::vector<Vertex> vertices;
	Mesh();
	~Mesh();
	bool LoadMesh(const std::string& id);
    void Render();

    std::vector<std::shared_ptr<Material>> renderMaterials;
    std::vector<MeshEntry> meshEntries;
};

