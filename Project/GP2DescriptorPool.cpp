#include "GP2DescriptorPool.h"

void GP2DescriptorPool::Initialize(const VkDevice& device)
{
	m_VkDevice = device;

	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;

	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

//template <VertexConcept V>
//void GP2DescriptorPool::CreateDescriptorSets(const VkDescriptorSetLayout& descriptorSetLayout, const GP2UniformBuffer<V>& uniformBuffers)
//{
//	
//}

void GP2DescriptorPool::Destroy()
{
	vkDestroyDescriptorPool(m_VkDevice, m_DescriptorPool, nullptr);
}
