#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
//#include "VulkanUtil.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <optional>
#include <set>
#include <limits>
#include <algorithm>
//#include "GP2Shader.h"
//#include "GP2CommandPool.h"
//#include "GP2Mesh.h"
//#include "GP2DataBuffer.h"
//#include "Vertexes.h"
//#include "GP2DescriptorPool.h"
#include "Scene.h"
//#include "Utils.h"
#include "Camera.h"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class VulkanBase {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	void initVulkan() {
		// week 06
		createInstance();
		setupDebugMessenger();
		createSurface();

		// week 05
		pickPhysicalDevice();
		createLogicalDevice();

		// week 04 
		createSwapChain();
		createImageViews();
		
		// week 03
		//m_GradientShader.Initialize(device);
		//m_3DShader.Initialize(device);
		//m_3DShader.CreateDescriptorSetLayout(device);
		createRenderPass();
		//CreateDescriptorSetLayout(device);
		//createGraphicsPipeline();
		//createGraphicsPipeline3D();
		CreateCamAndLightUniformBuffers();

		commandPool.Initialize(device, findQueueFamilies(physicalDevice));
		commandBuffer = commandPool.createCommandBuffer();
		
		createDepthResources();

		createFrameBuffers();

		// week 02
		//vertexBuffer.Initialize(device, physicalDevice, commandPool.GetCommandPool(), graphicsQueue);
		//vertexBuffer.CreateBuffer(mesh);
		//indexBuffer.Initialize(device, physicalDevice, commandPool.GetCommandPool(), graphicsQueue);
		//indexBuffer.CreateBuffer(mesh);
		//uniformBuffer.Initialize(device, physicalDevice, commandPool.GetCommandPool(), graphicsQueue);
		//uniformBuffer.CreateBuffer(mesh);
		//descriptorPool.Initialize(device);
		//descriptorPool.CreateDescriptorSets(m_DescriptorSetLayout, uniformBuffer);


		Scene scene{ commandPool, graphicsQueue };
		//3D pipeline
		
		pipeline2D.Initialize(device, physicalDevice, renderPass);
		pipeline3D.Initialize(device, physicalDevice, renderPass);
		pipeline3DTex.Initialize(device, physicalDevice, renderPass);
		pipeline3DPBR.Initialize(device, physicalDevice, renderPass);
		scene.Create2DScene(pipeline2D);
		scene.Create3DScene(pipeline3D);
		scene.Create3DTexScene(pipeline3DTex);
		scene.CreatePBRScene(pipeline3DPBR, m_CamBuffer, m_LightBuffer);
		
		// week 06
		createSyncObjects();
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			// week 06
			UpdateCamAndLightUniformBuffers();
			drawFrame();
		}
		vkDeviceWaitIdle(device);
	}

	void cleanup() {
		vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
		vkDestroyFence(device, inFlightFence, nullptr);
		pipeline2D.Cleanup();
		pipeline3D.Cleanup();
		pipeline3DTex.Cleanup();
		pipeline3DPBR.Cleanup();

		vkDestroyBuffer(device, m_CamBuffer, nullptr);
		vkFreeMemory(device, m_CamMemory, nullptr);

		vkDestroyBuffer(device, m_LightBuffer, nullptr);
		vkFreeMemory(device, m_LightMemory, nullptr);

		vkDestroyImageView(device, depthImageView, nullptr);
		vkDestroyImage(device, depthImage, nullptr);
		vkFreeMemory(device, depthImageMemory, nullptr);

		//vertexBuffer.Cleanup();
		//indexBuffer.Cleanup();
		//uniformBuffer.Cleanup();
		//
		//descriptorPool.Destroy();


		commandPool.Destroy();
		for (auto framebuffer : swapChainFramebuffers) {
			vkDestroyFramebuffer(device, framebuffer, nullptr);
		}

		//vkDestroyPipeline(device, graphicsPipeline, nullptr);
		//vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

		//m_3DShader.Cleanup(device);

		//vkDestroyDescriptorSetLayout(device, m_DescriptorSetLayout, nullptr);

		vkDestroyRenderPass(device, renderPass, nullptr);

		for (auto imageView : swapChainImageViews) {
			vkDestroyImageView(device, imageView, nullptr);
		}

		if (enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}
		vkDestroySwapchainKHR(device, swapChain, nullptr);
		vkDestroyDevice(device, nullptr);

		vkDestroySurfaceKHR(instance, surface, nullptr);
		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);
		glfwTerminate();
	}

	

	

	void createSurface() {
		if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	//GP2Shader m_GradientShader{
	//	"shaders/shader.vert.spv",
	//	"shaders/shader.frag.spv" 
	//};

	//GP2Shader m_3DShader{
	//	"shaders/shader3D.vert.spv",
	//	"shaders/shader.frag.spv"
	//};

	// Week 01: 
	// Actual window
	// simple fragment + vertex shader creation functions
	// These 5 functions should be refactored into a separate C++ class
	// with the correct internal state.

	GLFWwindow* window;
	void initWindow();

	void keyEvent(int key, int scancode, int action, int mods);

	

	glm::vec2 m_DragStart{};

	//void drawScene();
	//void drawScene3D(uint32_t imageIndex);

	// Week 02
	// Queue families
	// CommandBuffer concept
	//std::vector<glm::vec2> vertices {{-0.5f, -0.5f},
	//								{0.5f, -0.5f},
	//								{0.5f, 0.5f},
	//								{-0.5f, 0.5f}};
	// 
	// std::vector<uint16_t> indices{ 0, 1, 2, 2, 3, 0 };
	//
	//GP2Mesh<glm::vec2> mesh{ vertices, indices };

	//std::vector<Vertex3D> vertices{ Vertex3D{glm::vec3{-0.5f, -0.5f, 0.f}, glm::vec3{1.f, 0.f, 0.f}},
	//								Vertex3D{glm::vec3{0.5f, -0.5f, 0.f}, glm::vec3{0.f, 1.f, 0.f}},
	//								Vertex3D{glm::vec3{0.5f, 0.5f, 0.f}, glm::vec3{0.f, 0.f, 1.f}},
	//								Vertex3D{glm::vec3{-0.5f, 0.5f, 0.f}, glm::vec3{1.f, 1.f, 1.f}} };
	//
	//std::vector<uint16_t> indices{ 0, 1, 2, 2, 3, 0 };
	//
	//GP2Mesh<Vertex3D> mesh{ vertices, indices };
	//
	GP2CommandPool commandPool;
	GP2CommandBuffer commandBuffer;

	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CreateCamAndLightUniformBuffers();
	void UpdateCamAndLightUniformBuffers();

	Camera m_Camera{};
	VkBuffer m_CamBuffer{};
	VkDeviceMemory m_CamMemory{};

	struct Light
	{
		const glm::vec3 direction{ 0.577f, 0.577f, -0.577f };
		const glm::vec3 color{ 1.f, 1.f, 1.f };
	} g_Light{};
	VkBuffer m_LightBuffer{};
	VkDeviceMemory m_LightMemory{};

	GP2GraphicsPipeline<Vertex> pipeline2D{ "shaders/shader.vert.spv", "shaders/shader.frag.spv" };
	GP2GraphicsPipeline<Vertex3D> pipeline3D{ "shaders/shader3D.vert.spv", "shaders/shader.frag.spv" };
	GP2GraphicsPipeline<Vertex3D> pipeline3DTex{ "shaders/shader3D.vert.spv", "shaders/shaderTex.frag.spv" };
	GP2GraphicsPipeline<VertexPBR> pipeline3DPBR{ "shaders/shader3DPBR.vert.spv", "shaders/shaderPBR.frag.spv" };
	//GP2VertexBuffer<Vertex3D> vertexBuffer;
	//GP2IndexBuffer<Vertex3D> indexBuffer;
	//GP2UniformBuffer<Vertex3D> uniformBuffer;
	//GP2DescriptorPool descriptorPool;
	//VkDescriptorSetLayout m_DescriptorSetLayout;

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	//void drawFrame(uint32_t imageIndex);
	//void createCommandBuffer();
	//void createCommandPool(); 
	//void recordCommandBuffer(uint32_t imageIndex);
	//void CreateDescriptorSetLayout(const VkDevice& vkDevice);

	//void CreateVertexBuffer();
	
	// Week 03
	// Renderpass concept
	// Graphics pipeline
	
	std::vector<VkFramebuffer> swapChainFramebuffers;
	//VkPipelineLayout pipelineLayout;
	//VkPipeline graphicsPipeline;
	VkRenderPass renderPass;

	void createFrameBuffers();
	void createRenderPass();
	//void createGraphicsPipeline();
	//void createGraphicsPipeline3D();

	//DEPTH BUFFER
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	void createDepthResources();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();

	// Week 04
	// Swap chain and image view support

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	std::vector<VkImageView> swapChainImageViews;

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	void createSwapChain();
	void createImageViews();
	

	// Week 05 
	// Logical and physical device

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	
	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	void createLogicalDevice();

	// Week 06
	// Main initialization

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkDevice device = VK_NULL_HANDLE;
	VkSurfaceKHR surface;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void setupDebugMessenger();
	std::vector<const char*> getRequiredExtensions();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	void createInstance();

	void createSyncObjects();
	void drawFrame();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
};