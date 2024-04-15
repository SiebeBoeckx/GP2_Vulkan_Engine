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

	void Initialize(const VkDevice& device);

	template <VertexConcept V>
	void CreateDescriptorSets(const VkDescriptorSetLayout& descriptorSetLayout, const GP2UniformBuffer<V>& uniformBuffers)
	{
		//Create sets
		std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		allocInfo.pSetLayouts = layouts.data();

		m_DescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
		if (vkAllocateDescriptorSets(m_VkDevice, &allocInfo, m_DescriptorSets.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate descriptor sets!");
		}

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = uniformBuffers.GetUniformBufferInfos()[i]->GetBuffer();
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSets[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;

			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;

			descriptorWrite.pBufferInfo = &bufferInfo;
			descriptorWrite.pImageInfo = nullptr; // Optional
			descriptorWrite.pTexelBufferView = nullptr; // Optional

			vkUpdateDescriptorSets(m_VkDevice, 1, &descriptorWrite, 0, nullptr);
		}
	}

	void Destroy();

private:
	VkDevice m_VkDevice;
	VkDescriptorPool m_DescriptorPool;
	std::vector<VkDescriptorSet> m_DescriptorSets;
};