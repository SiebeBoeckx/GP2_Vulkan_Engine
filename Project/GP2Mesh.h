#pragma once
#include <vulkan/vulkan_core.h>
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <stdexcept>
#include <memory>
#include "vulkanbase/VulkanUtil.h"

//if changing, change where it's set for CMAKE!!!
struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription GetBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		//OPTIONS:
		//VK_VERTEX_INPUT_RATE_VERTEX: Move to the next data entry after each vertex
		//VK_VERTEX_INPUT_RATE_INSTANCE : Move to the next data entry after each instance
		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions() //https://vulkan-tutorial.com/Vertex_buffers/Vertex_input_description
	{
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};

class GP2Mesh
{
public:
	const std::vector<Vertex>& GetVertices() const { return vertices; }
	const std::vector<uint16_t>& GetIndices() const { return indices; }

private:
	const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};

	const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
	};
};

