#include <vector>
#include "Vertexes.h"
#include <fstream>
#include <tiny_obj_loader.h>

static bool ParseOBJ(const std::string& filename, std::vector<Vertex3D>& positions, std::vector<uint32_t>& indices)
{
    std::ifstream file(filename);
    if (!file)
    {
        return false;
    }

    std::string sCommand;
    // start a while iteration ending when the end of file is reached (ios::eof)
    while (!file.eof())
    {
        //read the first word of the string, use the >> operator (istream::operator>>) 
        file >> sCommand;
        //use conditional statements to process the different commands    
        if (sCommand == "#")
        {
            // Ignore Comment
        }
        else if (sCommand == "v")
        {
            //Vertex
            float x, y, z;
            file >> x >> y >> z;
            Vertex3D vertexOutput{};
            glm::vec3 vertexPos{};
            vertexOutput.pos = { x,y,z };
            vertexOutput.color = { 1,1,1 };
            positions.push_back(vertexOutput);
        }
        else if (sCommand == "f")
        {
            float i0, i1, i2;
            file >> i0 >> i1 >> i2;

            indices.push_back((uint32_t)i0 - 1);
            indices.push_back((uint32_t)i1 - 1);
            indices.push_back((uint32_t)i2 - 1);
        }
        //read till end of line and ignore all remaining chars
        file.ignore(1000, '\n');

        if (file.eof())
            break;
    }
    return true;
}

template <VertexConcept V>
void LoadModel(const std::string& filename, std::vector<Vertex3D>& positions, std::vector<uint32_t>& indices)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str())) {
        throw std::runtime_error(warn + err);
    }

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            V vertex{};

            vertex.pos = {  attrib.vertices[3 * index.vertex_index + 0],
                            attrib.vertices[3 * index.vertex_index + 1],
                            attrib.vertices[3 * index.vertex_index + 2]};

            vertex.texCoord = { -1.f, -1.f };
            
            vertex.color = { 1.f, 1.f, 1.f };

            positions.push_back(vertex);
            indices.push_back(static_cast<uint32_t>(indices.size()));
        }
    }
}

template <VertexConcept V>
void LoadModelTex(const std::string& filename, std::vector<Vertex3D>& positions, std::vector<uint32_t>& indices)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str())) {
        throw std::runtime_error(warn + err);
    }

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            V vertex{};

            vertex.pos = { attrib.vertices[3 * index.vertex_index + 0],
                            attrib.vertices[3 * index.vertex_index + 1],
                            attrib.vertices[3 * index.vertex_index + 2] };

            vertex.texCoord = { attrib.texcoords[2 * index.texcoord_index + 0],
                                1.0f - attrib.texcoords[2 * index.texcoord_index + 1] };

            vertex.color = { 1.f, 1.f, 1.f };

            positions.push_back(vertex);
            indices.push_back(static_cast<uint32_t>(indices.size()));
        }
    }
}

template <VertexConcept V>
void LoadModelPBR(const std::string& filename, std::vector<VertexPBR>& positions, std::vector<uint32_t>& indices)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str())) {
        throw std::runtime_error(warn + err);
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            V vertex{};

            // Vertex position
            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            // Vertex normal
            if (index.normal_index >= 0) {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            // Vertex texture coordinate
            if (index.texcoord_index >= 0) {
                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            vertex.color = { 1.f, 1.f, 1.f };

            positions.push_back(vertex);
            indices.push_back(static_cast<uint32_t>(indices.size()));
        }
    }

    // Calculate tangents and bitangents
    for (size_t i = 0; i < indices.size(); i += 3) {
        V& v0 = positions[indices[i]];
        V& v1 = positions[indices[i + 1]];
        V& v2 = positions[indices[i + 2]];

        glm::vec3 edge1 = v1.pos - v0.pos;
        glm::vec3 edge2 = v2.pos - v0.pos;
        glm::vec2 deltaUV1 = v1.texCoord - v0.texCoord;
        glm::vec2 deltaUV2 = v2.texCoord - v0.texCoord;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent, bitangent;

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);

        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent = glm::normalize(bitangent);

        v0.tangent = tangent;
        v1.tangent = tangent;
        v2.tangent = tangent;

        v0.biTangent = bitangent;
        v1.biTangent = bitangent;
        v2.biTangent = bitangent;
    }
}