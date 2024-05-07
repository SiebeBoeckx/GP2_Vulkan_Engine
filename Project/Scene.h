#pragma once
#include "GP2Pipeline.h"

class Scene
{
public:
	Scene(const GP2CommandPool& commandPool, const VkQueue& graphicsQueue)
		:m_CommandPool{commandPool}
		,m_GraphicsQueue{graphicsQueue}
	{}
	~Scene() = default;

	void Create3DScene(GP2GraphicsPipeline<Vertex3D>& pipeline);
	void Create2DScene(GP2GraphicsPipeline<Vertex>& pipeline);
	void Create3DTexScene(GP2GraphicsPipeline<Vertex3D>& pipeline);
private:
	GP2Mesh<Vertex> CreateRectangle(glm::vec2 center, float width, float height, const glm::vec3& color = {1.f, 1.f, 1.f});
	GP2Mesh<Vertex> CreateRectangle(glm::vec2 center, float width, float height, const std::vector<glm::vec3>& colors);
	GP2Mesh<Vertex> CreateRectangleV2(glm::vec2 center, float width, float height, const std::vector<glm::vec3>& colors);
	GP2Mesh<Vertex> CreateRectangleV3(glm::vec2 center, float width, float height, const std::vector<glm::vec3>& colors);
	GP2Mesh<Vertex> CreateRectangleV4(glm::vec2 center, float width, float height, const std::vector<glm::vec3>& colors);
	GP2Mesh<Vertex> CreateRectangleV5(glm::vec2 center, float width, float height, const std::vector<glm::vec3>& colors);

	GP2Mesh<Vertex> CreateEllipse(glm::vec2 center, float width, float height, const glm::vec3& color = { 1.f, 1.f, 1.f }, int nrOfVertexes = 50);
	GP2Mesh<Vertex> CreateEllipse(glm::vec2 center, float width, float height, const std::vector<glm::vec3>& colors, int nrOfVertexes = 50);

	GP2Mesh<Vertex> CreateRoundedRectangle(glm::vec2 center, float width, float height, float cornerRadius, const glm::vec3& color = { 1.f, 1.f, 1.f }, int numPoints = 10);
	GP2Mesh<Vertex> CreateRoundedRectangle(glm::vec2 center, float width, float height, float cornerRadius, const std::vector<glm::vec3>& colors, int numPoints = 10);
	GP2Mesh<Vertex> CreateRoundedRectangleV2(glm::vec2 center, float width, float height, float cornerRadius, const std::vector<glm::vec3>& colors, int numPoints = 10);
	GP2Mesh<Vertex> CreateRoundedRectangleBorder(glm::vec2 center, float width, float height, float cornerRadius, const std::vector<glm::vec3>& colors, int numPoints = 10);
	GP2Mesh<Vertex> GenerateCorner(const glm::vec2& center, float radius, int numPoints, const glm::vec3& color, float initialRotation);
	GP2Mesh<Vertex> GenerateCornerV2(const glm::vec2& center, float radius, int numPoints, const std::vector<glm::vec3>& colors, float initialRotation);
	//std::vector<uint16_t> GenerateRoundedRectangleIndices(int numPoints);

	GP2CommandPool m_CommandPool{};
	VkQueue m_GraphicsQueue{};
};
