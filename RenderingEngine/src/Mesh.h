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
private:
    struct MeshEntry {
        MeshEntry();
        ~MeshEntry();
        MeshEntry(const MeshEntry&) = default;
        void InitEntry(const std::vector<Vertex>& verts, const std::vector<unsigned int>& indices);
        std::shared_ptr<VertexBuffer> vb;
        std::shared_ptr<IndexBuffer> ib;
        unsigned int numIndices;
        unsigned int matIndex;
    };
    std::vector<MeshEntry> meshEntries;
    std::vector<std::shared_ptr<Material>> renderMaterials;

	bool InitMeshFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();
public:
	Mesh();
	~Mesh();
	bool LoadMesh(const std::string& id);
	void Render();
};

