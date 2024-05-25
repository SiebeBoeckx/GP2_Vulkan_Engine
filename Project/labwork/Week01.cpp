#pragma once
#include "vulkanbase/VulkanBase.h"
//#include "GP2Mesh.h"

void VulkanBase::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        void* pUser = glfwGetWindowUserPointer(window);
        VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
        vBase->keyEvent(key, scancode, action, mods);
        });
    //glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
    //    void* pUser = glfwGetWindowUserPointer(window);
    //    VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
    //    vBase->mouseMove(window, xpos, ypos);
    //    });
}

void VulkanBase::keyEvent(int key, int scancode, int action, int mods)
{
    //if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
    //{
    //    m_Radius = std::max(3.0f, m_Radius - 0.2f);
    //}
    //if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
    //{
    //    m_Radius = std::min(30.0f, m_Radius + 0.2f);
    //}
    m_Camera.KeyEvent(key, scancode, action, mods);
}

uint32_t VulkanBase::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void VulkanBase::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    // Create buffer
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    // Allocate memory
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    // Bind memory
    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void VulkanBase::CreateCamAndLightUniformBuffers()
{
    VkDeviceSize cameraBufferSize = sizeof(glm::vec3); // cameraPosition
    VkDeviceSize lightBufferSize = 2 * sizeof(glm::vec3); // lightPosition and lightColor

    CreateBuffer(cameraBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_CamBuffer, m_CamMemory);
    CreateBuffer(lightBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_LightBuffer, m_LightMemory);
}

void VulkanBase::UpdateCamAndLightUniformBuffers()
{
    void* data;

    // Update camera buffer
    vkMapMemory(device, m_CamMemory, 0, sizeof(glm::vec3), 0, &data);
    memcpy(data, &m_Camera.GetOrigin(), sizeof(glm::vec3));
    vkUnmapMemory(device, m_CamMemory);

    // Update light buffer
    vkMapMemory(device, m_LightMemory, 0, 2 * sizeof(glm::vec3), 0, &data);
    memcpy(data, &g_Light.direction, sizeof(glm::vec3));
    memcpy(static_cast<char*>(data) + sizeof(glm::vec3), &g_Light.color, sizeof(glm::vec3));
    vkUnmapMemory(device, m_LightMemory);
}

//void VulkanBase::drawScene() {
//
//	VkBuffer vertexBuffers[] = { vertexBuffer.GetBuffer()};
//	VkBuffer indexBuffers = { indexBuffer.GetBuffer()};
//	VkDeviceSize offsets[] = { 0 };
//	vkCmdBindVertexBuffers(commandBuffer.GetVkCommandBuffer(), 0, 1, vertexBuffers, offsets);
//
//	vkCmdBindIndexBuffer(commandBuffer.GetVkCommandBuffer(), indexBuffers, 0, VK_INDEX_TYPE_UINT16);
//
//	//vkCmdDraw(commandBuffer.GetVkCommandBuffer(), static_cast<uint32_t>(mesh.GetVertices().size()), 1, 0, 0);
//	//vkCmdDraw(commandBuffer.GetVkCommandBuffer(), 6, 1, 0, 0);
//	vkCmdDrawIndexed(commandBuffer.GetVkCommandBuffer(), static_cast<uint32_t>(mesh.GetIndices().size()), 1, 0, 0, 0);
//}

//void VulkanBase::drawScene3D(uint32_t imageIndex)
//{
//	VkBuffer vertexBuffers[] = { vertexBuffer.GetBuffer() };
//	VkBuffer indexBuffers = { indexBuffer.GetBuffer() };
//	VkDeviceSize offsets[] = { 0 };
//	vkCmdBindVertexBuffers(commandBuffer.GetVkCommandBuffer(), 0, 1, vertexBuffers, offsets);
//
//	vkCmdBindIndexBuffer(commandBuffer.GetVkCommandBuffer(), indexBuffers, 0, VK_INDEX_TYPE_UINT16);
//
//	vkCmdBindDescriptorSets(  commandBuffer.GetVkCommandBuffer()
//							, VK_PIPELINE_BIND_POINT_GRAPHICS
//							, pipelineLayout
//							, 0
//							, 1
//							, &descriptorPool.GetDescriptorSets()[imageIndex], 0, nullptr);
//
//	//vkCmdDraw(commandBuffer.GetVkCommandBuffer(), static_cast<uint32_t>(mesh.GetVertices().size()), 1, 0, 0);
//	//vkCmdDraw(commandBuffer.GetVkCommandBuffer(), 6, 1, 0, 0);
//	vkCmdDrawIndexed(commandBuffer.GetVkCommandBuffer(), static_cast<uint32_t>(mesh.GetIndices().size()), 1, 0, 0, 0);
//}