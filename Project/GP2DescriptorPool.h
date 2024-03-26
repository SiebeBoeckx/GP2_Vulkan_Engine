#pragma once
#include "vulkan/vulkan_core.h"
#include "vulkanbase/VulkanUtil.h"

#include "GP2DataBuffer.h"

class GP2DescriptorPool
{
public:
	GP2DescriptorPool()
		:m_DescriptorPool(VK_NULL_HANDLE)
		, m_VkDevice(VK_NULL_HANDLE)
	{}
	~GP2DescriptorPool() = default;

	const VkDescriptorPool& GetDescriptorPool() const { return m_DescriptorPool; }
	const std::vector<VkDescriptorSet>& GetDescriptorSets() const { return m_DescriptorSets; }

	void Initialize(const VkDevice& device, const QueueFamilyIndices& queue);
	void CreateDescriptorSets(const VkDescriptorSetLayout& descriptorSetLayout, const GP2UniformBuffer& uniformBuffers);
	void Destroy();

private:
	VkDevice m_VkDevice;
	VkDescriptorPool m_DescriptorPool;
	std::vector<VkDescriptorSet> m_DescriptorSets;
};