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

    Vertex() {}
    Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal)
        : pos(pos), tex(tex), normal(normal)
	{}
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
    std::vector<Texture*> textures;

	bool InitMeshFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();
public:
	Mesh();
	~Mesh();
	bool LoadMesh(const std::string& filepath);
	void Render();
};

