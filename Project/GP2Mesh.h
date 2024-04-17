#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <stdexcept>
#include <memory>
#include "vulkanbase/VulkanUtil.h"
#include "GP2DescriptorPool.h"

//if changing, change where it's set for CMAKE!!!


template <VertexConcept V>
class GP2Mesh
{
public:
	GP2Mesh(std::vector<V> verticeVector, std::vector<uint16_t> indiceVector) : m_Vertices(verticeVector), m_Indices(indiceVector)
	{

	}

	void Initialize(const VkDevice& device, const VkPhysicalDevice& physDevice, const VkCommandPool commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout);
	void Cleanup();

	const std::vector<V>& GetVertices() const { return m_Vertices; }
	const std::vector<uint16_t>& GetIndices() const { return m_Indices; }
	const GP2VertexBuffer<V>& GetVertexBuffer() const { return m_VertexBuffer; }
	const GP2IndexBuffer<V> GetIndexBuffer() const { return m_IndexBuffer; }
	const GP2UniformBuffer<V>& GetUniformBuffer() const { return m_UniformBuffer; }
	GP2UniformBuffer<V>& GetWriteAbleUniformBuffer() { return m_UniformBuffer; }
	const GP2DescriptorPool& GetDescriptorPool() const { return m_DescriptorPool; }

private:
	const std::vector<V> m_Vertices{};

	const std::vector<uint16_t> m_Indices{};

	GP2VertexBuffer<V> m_VertexBuffer;
	GP2IndexBuffer<V> m_IndexBuffer;
	GP2UniformBuffer<V> m_UniformBuffer;
	GP2DescriptorPool m_DescriptorPool{};
};

template<VertexConcept V>
inline void GP2Mesh<V>::Initialize(const VkDevice& device, const VkPhysicalDevice& physDevice, const VkCommandPool commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout)
{
	m_VertexBuffer.Initialize(device, physDevice, commandPool, graphicsQueue);
	m_VertexBuffer.CreateBuffer(*this);
	m_IndexBuffer.Initialize(device, physDevice, commandPool, graphicsQueue);
	m_IndexBuffer.CreateBuffer(*this);
	m_UniformBuffer.Initialize(device, physDevice, commandPool, graphicsQueue);
	m_UniformBuffer.CreateBuffer(*this);
	m_DescriptorPool.Initialize(device);
	m_DescriptorPool.CreateDescriptorSets(descriptorSetLayout, m_UniformBuffer);
}

template<VertexConcept V>
inline void GP2Mesh<V>::Cleanup()
{
	m_VertexBuffer.Cleanup();
	m_IndexBuffer.Cleanup();
	m_UniformBuffer.Cleanup();
	m_DescriptorPool.Destroy();
}
