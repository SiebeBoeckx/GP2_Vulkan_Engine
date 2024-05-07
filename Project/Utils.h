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
            Vertex3D vertex{};

            vertex.pos = {  attrib.vertices[3 * index.vertex_index + 0],
                            attrib.vertices[3 * index.vertex_index + 1],
                            attrib.vertices[3 * index.vertex_index + 2]};

            vertex.texCoord = {  attrib.texcoords[2 * index.texcoord_index + 0],
                                 attrib.texcoords[2 * index.texcoord_index + 1]};
            
            vertex.color = { 1.f, 1.f, 1.f };

            positions.push_back(vertex);
            indices.push_back(static_cast<uint32_t>(indices.size()));
        }
    }
}
