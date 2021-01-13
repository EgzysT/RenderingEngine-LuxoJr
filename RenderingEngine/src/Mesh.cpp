#include "Mesh.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "AssetManager.h"

//#include <stdlib.h>

Mesh::MeshEntry::MeshEntry()
{
    vb = nullptr;
    ib = nullptr;
    numIndices = 0;
    matIndex = 0;
};

Mesh::MeshEntry::~MeshEntry()
{
}

void Mesh::MeshEntry::InitEntry(const std::vector<Vertex>& verts,
    const std::vector<unsigned int>& indices)
{
    numIndices = indices.size();
    vb = std::make_shared<VertexBuffer>(&verts[0] ,sizeof(Vertex) * verts.size());
    ib = std::make_shared<IndexBuffer>(&indices[0], numIndices);
}

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
    Clear();
}


void Mesh::Clear()
{
    renderMaterials.clear();
}

bool Mesh::InitMeshFromScene(const aiScene* scene, const std::string& filepath)
{
    meshEntries.resize(scene->mNumMeshes);
    //std::cout << "mNumMaterials: " << scene->mNumMaterials << std::endl;
    renderMaterials.resize(scene->mNumMaterials);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0; i < meshEntries.size(); i++) {
        const aiMesh* mesh = scene->mMeshes[i];
        InitMesh(i, mesh);
    }

    return InitMaterials(scene, filepath);
}

void Mesh::InitMesh(unsigned int index, const aiMesh* mesh)
{
    meshEntries[index].matIndex = mesh->mMaterialIndex;

    std::vector<Vertex> verts;
    std::vector<unsigned int> indices;

    const aiVector3D vec3Zero(0.0f, 0.0f, 0.0f);

    //Vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        const aiVector3D* pos = &(mesh->mVertices[i]);
        const aiVector3D* normal = &(mesh->mNormals[i]);
        const aiVector3D* texCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &vec3Zero;
        const aiVector3D* tangent = &(mesh->mTangents[i]);

        Vertex vert(glm::vec3(pos->x, pos->y, pos->z),
            glm::vec2(texCoord->x, texCoord->y),
            glm::vec3(normal->x, normal->y, normal->z),
            glm::vec3(tangent->x, tangent->y, tangent->z));

        verts.push_back(vert);
    }

    //Indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];
        assert(face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    meshEntries[index].InitEntry(verts, indices);
}

bool Mesh::LoadMesh(const std::string& id)
{
    AssetManager* am = AssetManager::GetInstance();
    std::string filepath = am->GetModelPathOf(id);
    //std::cout << filepath << std::endl;

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filepath,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_SplitLargeMeshes |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_SortByPType);

    if (!scene) {
        printf("Error parsing '%s': '%s'\n", filepath.c_str(), importer.GetErrorString());
        return false;
    }

    return InitMeshFromScene(scene, id);
}

bool Mesh::InitMaterials(const aiScene* scene, const std::string& filepath)
{
    // Extract the directory part from the file name
    std::string::size_type SlashIndex = filepath.find_last_of("\\");
    std::string Dir;

    if (SlashIndex == std::string::npos) {
        Dir = ".";
    }
    else if (SlashIndex == 0) {
        Dir = "\\";
    }
    else {
        Dir = filepath.substr(0, SlashIndex);
    }

    bool Ret = true;

    // Initialize the materials
    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        const aiMaterial* mat = scene->mMaterials[i];

        renderMaterials[i] = std::make_unique<Material>();
        // Set name
        aiString name;
        mat->Get(AI_MATKEY_NAME, name);
        renderMaterials[i]->name = name.C_Str();

        //std::cout << "diffMats: " << mat->GetTextureCount(aiTextureType_DIFFUSE) << std::endl;
        //std::cout << "normalMats: " << mat->GetTextureCount(aiTextureType_NORMALS) << std::endl;

        renderMaterials[i]->diffuseTexture = std::make_unique<Texture>(AssetManager::GetInstance()->GetDiffusePathOf(filepath), 0);
        //if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        //    aiString Path;

        //    if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
        //        std::cout << Path.C_Str() << std::endl;
        //        std::string fullPath = Dir + "\\" + Path.data;
        //        renderMaterials[i]->diffuseTexture = std::make_unique<Texture>(fullPath.c_str(), 0);

        //        /*if (!textures[i]->Load()) {
        //            printf("Error loading texture '%s'\n", fullPath.c_str());
        //            delete textures[i];
        //            textures[i] = NULL;
        //            Ret = false;
        //        }
        //        else {
        //            printf("Loaded texture '%s'\n", fullPath.c_str());
        //        }*/
        //    }
        //}

        renderMaterials[i]->normalTexture = std::make_unique<Texture>(AssetManager::GetInstance()->GetNormalsPathOf(filepath), 1);
        //if (mat->GetTextureCount(aiTextureType_NORMALS) > 0) {
        //    aiString Path;

        //    if (mat->GetTexture(aiTextureType_NORMALS, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
        //        std::cout << Path.C_Str() << std::endl;
        //        std::string fullPath = Dir + "\\" + Path.data;
        //        renderMaterials[i]->normalTexture = std::make_unique<Texture>(fullPath.c_str(), 1);
        //    }
        //}

        // Load a white texture in case the model does not include its own texture
        if (!renderMaterials[i]) {
            renderMaterials[i]->diffuseTexture = std::make_unique<Texture>("..\\assets\\textures\\test.png", 0);

            //Ret = textures[i]->Load();
        }
    }
    //TODO: make ret relevant / separate constructor and loading
    //renderMaterials[0]->diffuseTexture = std::make_unique<Texture>("..\\assets\\models\\rock1\\Rock_1_Base_Color.jpg", 0);
    //renderMaterials[0]->normalTexture = std::make_unique<Texture>("..\\assets\\models\\rock1\\Rock_1_Normal.jpg", 1);
    return Ret;
}

void Mesh::Render()
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    for (unsigned int i = 0; i < meshEntries.size(); i++) {
        meshEntries[i].vb->Bind();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);                 // positions
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12); // texCoords
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); // normals
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32); // tangents

        meshEntries[i].ib->Bind();

        const unsigned int MaterialIndex = meshEntries[i].matIndex;

        if (MaterialIndex < renderMaterials.size() && renderMaterials[MaterialIndex]) {
            renderMaterials[MaterialIndex]->Bind();
        }

        glDrawElements(GL_TRIANGLES, meshEntries[i].numIndices, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}