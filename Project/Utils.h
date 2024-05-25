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

    for (size_t s = 0; s < shapes.size(); s++)
    {
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            for (size_t v = 0; v < fv; v++) {
                V vertex{};
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                // Vertex position
                vertex.pos = {
                    attrib.vertices[3 * idx.vertex_index + 0],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]
                };

                // Vertex normal
                if (idx.normal_index >= 0) {
                    vertex.normal = {
                        attrib.normals[3 * idx.normal_index + 0],
                        attrib.normals[3 * idx.normal_index + 1],
                        attrib.normals[3 * idx.normal_index + 2]
                    };
                }

                // Vertex texture coordinate
                if (idx.texcoord_index >= 0) {
                    vertex.texCoord = {
                        attrib.texcoords[2 * idx.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]
                    };
                }

                vertex.color = { 1.f, 1.f, 1.f };

                positions.push_back(vertex);
                indices.push_back(static_cast<uint32_t>(indices.size()));
            }
            index_offset += fv;
        }
    }

    // Calculate tangents and bitangents
    
    //for (size_t i = 0; i < indices.size(); i += 3) {
    //    V& v0 = positions[indices[i]];
    //    V& v1 = positions[indices[i + 1]];
    //    V& v2 = positions[indices[i + 2]];
    //
    //    glm::vec3 edge1 = v1.pos - v0.pos;
    //    glm::vec3 edge2 = v2.pos - v0.pos;
    //    glm::vec2 diffX = {v1.texCoord.x - v0.texCoord.x, v2.texCoord.x - v0.texCoord.x};
    //    glm::vec2 diffY = {v1.texCoord.y - v0.texCoord.y, v2.texCoord.y - v0.texCoord.y};
    //
    //    float f = 1.0f / (diffX.x * diffY.y - diffX.y * diffY.x);
    //
    //    glm::vec3 tangent, bitangent0, bitangent1, bitangent2;
    //
    //    tangent = f * (edge1 * diffY.y - edge2 * diffY.x);
    //    //tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    //    //tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    //    //tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    //    //tangent = glm::normalize(tangent);
    //  
    //    bitangent0 = tangent - v0.normal * (glm::dot(tangent, v0.normal) / glm::dot(v0.normal, v0.normal));
    //    bitangent0 = glm::normalize(bitangent0);
    //
    //    bitangent1 = tangent - v1.normal * (glm::dot(tangent, v1.normal) / glm::dot(v1.normal, v1.normal));
    //    bitangent1 = glm::normalize(bitangent1);
    //
    //    bitangent2 = tangent - v2.normal * (glm::dot(tangent, v2.normal) / glm::dot(v2.normal, v2.normal));
    //    bitangent2 = glm::normalize(bitangent2);
    //
    //    //bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    //    //bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    //    //bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    //    //bitangent = glm::normalize(bitangent);
    //
    //    v0.tangent = tangent;
    //    v1.tangent = tangent;
    //    v2.tangent = tangent;
    //
    //    v0.biTangent = bitangent0;
    //    v1.biTangent = bitangent1;
    //    v2.biTangent = bitangent2;
    //}

    for (size_t i = 0; i < indices.size(); i += 3) {
        size_t i0 = indices[i];
        size_t i1 = indices[i + 1];
        size_t i2 = indices[i + 2];

        // Shortcuts for vertices
        glm::vec3& v0 = positions[i0].pos;
        glm::vec3& v1 = positions[i1].pos;
        glm::vec3& v2 = positions[i2].pos;

        // Shortcuts for texture coordinates
        glm::vec2& uv0 = positions[i0].texCoord;
        glm::vec2& uv1 = positions[i1].texCoord;
        glm::vec2& uv2 = positions[i2].texCoord;

        // Edges of the triangle : position delta
        glm::vec3 deltaPos1 = v1 - v0;
        glm::vec3 deltaPos2 = v2 - v0;

        // UV delta
        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 biTangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        // Set tangent and bitangent for each vertex of the triangle
        positions[i0].tangent += tangent;
        positions[i1].tangent += tangent;
        positions[i2].tangent += tangent;

        positions[i0].biTangent += biTangent;
        positions[i1].biTangent += biTangent;
        positions[i2].biTangent += biTangent;
    }

    // Normalize tangent and bitangent vectors
    for (size_t i = 0; i < positions.size(); ++i) {
        positions[i].tangent = glm::normalize(positions[i].tangent);
        positions[i].biTangent = glm::normalize(positions[i].biTangent);
    }
}