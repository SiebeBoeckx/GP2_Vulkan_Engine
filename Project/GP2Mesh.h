#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <stdexcept>
#include <memory>
#include "vulkanbase/VulkanUtil.h"
#include "Vertexes.h"

//if changing, change where it's set for CMAKE!!!


template <VertexConcept V>
class GP2Mesh
{
public:
	GP2Mesh(std::vector<V> verticeVector, std::vector<uint16_t> indiceVector) : vertices(verticeVector), indices(indiceVector) {}
	const std::vector<V>& GetVertices() const { return vertices; }
	const std::vector<uint16_t>& GetIndices() const { return indices; }

private:
	const std::vector<V> vertices{};

	const std::vector<uint16_t> indices{};
};