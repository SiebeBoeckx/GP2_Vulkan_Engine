#pragma once
#include "Scene.h"
#include <algorithm>
#include <iostream>
#include "Utils.h"

void Scene::Create2DScene(GP2GraphicsPipeline<Vertex>& pipeline)
{
	std::vector<glm::vec3> colors{ {1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 1.f, 1.f} };
	std::vector<glm::vec3> colors2{ {1.f, 0.f, 0.f}, {0.f, 1.f, 0.f} };

	GP2Mesh<Vertex> rectangle{ CreateRectangle({ 0.8f, 0.85f }, 0.3f, 0.2f) };
	GP2Mesh<Vertex> rectangle2{ CreateRectangle({ 0.8f, 0.6f }, 0.3f, 0.2f, colors) };
	GP2Mesh<Vertex> rectangle3{ CreateRectangle({ 0.8f, 0.35f }, 0.3f, 0.2f, colors2) };
	GP2Mesh<Vertex> rectangle4{ CreateRectangleV2({ 0.8f, 0.1f }, 0.3f, 0.2f, colors2) };
	GP2Mesh<Vertex> rectangle5{ CreateRectangleV3({ 0.8f, -0.15f }, 0.3f, 0.2f, colors2) };
	GP2Mesh<Vertex> rectangle6{ CreateRectangleV4({ 0.8f, -0.4f }, 0.3f, 0.2f, colors2) };
	GP2Mesh<Vertex> rectangle7{ CreateRectangleV5({ 0.8f, -0.65f }, 0.3f, 0.2f, colors2) };

	GP2Mesh<Vertex> ellipse{ CreateEllipse({ -0.85f, -0.75f }, 0.2f, 0.4f) };
	GP2Mesh<Vertex> ellipse2{ CreateEllipse({ -0.6f, -0.75f }, 0.2f, 0.4f, colors2) };
	
	GP2Mesh<Vertex> roundedRectangle{ CreateRoundedRectangle({ -0.8f, 0.85f }, 0.3f, 0.2f, 0.02f) };
	GP2Mesh<Vertex> roundedRectangle2{ CreateRoundedRectangle({ -0.8f, 0.6f }, 0.3f, 0.2f, 0.02f, colors2) };
	GP2Mesh<Vertex> roundedRectangle3{ CreateRoundedRectangleBorder({ -0.8f, 0.35f }, 0.3f, 0.2f, 0.02f, colors2) };
	GP2Mesh<Vertex> roundedRectangle4{ CreateRoundedRectangleV2({ -0.8f, 0.1f }, 0.3f, 0.2f, 0.02f, colors2) };

	pipeline.AddMesh(rectangle, m_CommandPool, m_GraphicsQueue);
	pipeline.AddMesh(rectangle2, m_CommandPool, m_GraphicsQueue);
	pipeline.AddMesh(rectangle3, m_CommandPool, m_GraphicsQueue);
	pipeline.AddMesh(rectangle4, m_CommandPool, m_GraphicsQueue);
	pipeline.AddMesh(rectangle5, m_CommandPool, m_GraphicsQueue);
	pipeline.AddMesh(rectangle6, m_CommandPool, m_GraphicsQueue);
	pipeline.AddMesh(rectangle7, m_CommandPool, m_GraphicsQueue);

	pipeline.AddMesh(ellipse, m_CommandPool, m_GraphicsQueue);
	pipeline.AddMesh(ellipse2, m_CommandPool, m_GraphicsQueue);

	pipeline.AddMesh(roundedRectangle, m_CommandPool, m_GraphicsQueue);
	pipeline.AddMesh(roundedRectangle2, m_CommandPool, m_GraphicsQueue);
	pipeline.AddMesh(roundedRectangle3, m_CommandPool, m_GraphicsQueue);
	pipeline.AddMesh(roundedRectangle4, m_CommandPool, m_GraphicsQueue);
}

void Scene::Create3DScene(GP2GraphicsPipeline<Vertex3D>& pipeline)
{
	std::vector<Vertex3D> vertices{ Vertex3D{glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{1.f, 0.f, 0.f}, glm::vec2{-1.f, -1.f}},
								Vertex3D{glm::vec3{0.5f, -0.5f, -0.5f}, glm::vec3{0.f, 1.f, 0.f}, glm::vec2{-1.f, -1.f}},
								Vertex3D{glm::vec3{0.5f, 0.5f, -0.5f}, glm::vec3{0.f, 0.f, 1.f}, glm::vec2{-1.f, -1.f}},
								Vertex3D{glm::vec3{-0.5f, 0.5f, -0.5f}, glm::vec3{1.f, 1.f, 1.f}, glm::vec2{-1.f, -1.f}} };
	std::vector<uint32_t> indices{ 0, 1, 2, 2, 3, 0 };
	GP2Mesh<Vertex3D> mesh{ vertices, indices };
	pipeline.AddMesh(mesh, m_CommandPool, m_GraphicsQueue);

	std::vector<Vertex3D> vertices2{ Vertex3D{glm::vec3{-0.5f, -0.5f, -1.0f}, glm::vec3{1.f, 1.f, 1.f}, glm::vec2{-1.f, -1.f}},
								Vertex3D{glm::vec3{0.5f, -0.5f, -1.0f}, glm::vec3{1.f, 1.f, 1.f}, glm::vec2{-1.f, -1.f}},
								Vertex3D{glm::vec3{0.5f, 0.5f, -1.0f}, glm::vec3{1.f, 1.f, 1.f}, glm::vec2{-1.f, -1.f}},
								Vertex3D{glm::vec3{-0.5f, 0.5f, -1.0f}, glm::vec3{1.f, 1.f, 1.f}, glm::vec2{-1.f, -1.f}} };
	std::vector<uint32_t> indices2{ 0, 1, 2, 2, 3, 0 };
	GP2Mesh<Vertex3D> mesh2{ vertices2, indices2 };
	pipeline.AddMesh(mesh2, m_CommandPool, m_GraphicsQueue);

	
}

void Scene::Create3DTexScene(GP2GraphicsPipeline<Vertex3D>& pipeline)
{
	std::vector<Vertex3D> vertices{ Vertex3D{glm::vec3{-0.5f, -0.5f, -1.5f}, glm::vec3{1.f, 0.f, 1.f}, glm::vec2{1.f, 0.f}},
										Vertex3D{glm::vec3{0.5f, -0.5f, -1.5f}, glm::vec3{1.f, 0.f, 1.f}, glm::vec2{0.f, 0.f}},
										Vertex3D{glm::vec3{0.5f, 0.5f, -1.5f}, glm::vec3{1.f, 0.f, 1.f}, glm::vec2{0.f, 1.f}},
										Vertex3D{glm::vec3{-0.5f, 0.5f, -1.5f}, glm::vec3{1.f, 0.f, 1.f}, glm::vec2{1.f, 1.f}} };
	std::vector<uint32_t> indices{ 0, 1, 2, 2, 3, 0 };
	GP2Mesh<Vertex3D> mesh{ vertices, indices };
	mesh.SetTextureImage(pipeline.GetDevice(), pipeline.GetPhysDevice(), pipeline.CreateTextureImage("resources/texture.jpg", m_CommandPool, m_GraphicsQueue));
	pipeline.AddMesh(mesh, m_CommandPool, m_GraphicsQueue);

	//std::vector<Vertex3D> vertices2{};
	//std::vector<uint32_t> indices2{};
	//std::string filename{ "resources/viking_room.obj" };
	//LoadModelTex<Vertex3D>(filename, vertices2, indices2);
	//GP2Mesh<Vertex3D> mesh2{ vertices2, indices2 };
	//mesh2.SetTextureImage(pipeline.GetDevice(), pipeline.GetPhysDevice(), pipeline.CreateTextureImage("resources/viking_room.png", m_CommandPool, m_GraphicsQueue));
	//pipeline.AddMesh(mesh2, m_CommandPool, m_GraphicsQueue);

	//std::vector<Vertex3D> vertices3{};
	//std::vector<uint32_t> indices3{};
	//std::string filename{ "resources/viking_room.obj" };
	//LoadModel<Vertex3D>(filename, vertices3, indices3);
	//GP2Mesh<Vertex3D> mesh3{ vertices3, indices3 };
	//pipeline.AddMesh(mesh3, m_CommandPool, m_GraphicsQueue);
}

void Scene::CreatePBRScene(GP2GraphicsPipeline<VertexPBR>& pipeline)
{
	std::vector<VertexPBR> vertices{};
	std::vector<uint32_t> indices{};
	std::string filename{ "resources/vehicle.obj" };
	LoadModelPBR<VertexPBR>(filename, vertices, indices);
	GP2Mesh<VertexPBR> mesh{ vertices, indices };
	mesh.SetTextureImage(pipeline.GetDevice(), pipeline.GetPhysDevice(), pipeline.CreateTextureImage("resources/vehicle_diffuse.png", m_CommandPool, m_GraphicsQueue));
	std::pair<VkImage, VkDeviceMemory> normalMap{ pipeline.CreateTextureImage("resources/vehicle_normal.png", m_CommandPool, m_GraphicsQueue) };
	std::pair<VkImage, VkDeviceMemory> metalicMap{ pipeline.CreateTextureImage("resources/vehicle_specular.png", m_CommandPool, m_GraphicsQueue) };
	std::pair<VkImage, VkDeviceMemory> glossMap{ pipeline.CreateTextureImage("resources/vehicle_gloss.png", m_CommandPool, m_GraphicsQueue) };
	std::vector<std::pair<VkImage, VkDeviceMemory>> pbrMaps{ normalMap, metalicMap, glossMap };
	mesh.SetPBRTextures(pipeline.GetDevice(), pipeline.GetPhysDevice(), pbrMaps);
	pipeline.AddMesh(mesh, m_CommandPool, m_GraphicsQueue);
}

GP2Mesh<Vertex> Scene::CreateRectangle(glm::vec2 center, float width, float height, const glm::vec3& color)
{
	std::vector<Vertex> vertexes{};
	std::vector<uint32_t> indices{ 0, 2, 1, 2, 0, 3 };

	vertexes.push_back(Vertex{ {center.x - width / 2, center.y - height / 2}, color, glm::vec2{-1.f, -1.f} });
	vertexes.push_back(Vertex{ {center.x + width / 2, center.y - height / 2}, color, glm::vec2{-1.f, -1.f} });
	vertexes.push_back(Vertex{ {center.x + width / 2, center.y + height / 2}, color, glm::vec2{-1.f, -1.f} });
	vertexes.push_back(Vertex{ {center.x - width / 2, center.y + height / 2}, color, glm::vec2{-1.f, -1.f} });

	return GP2Mesh<Vertex>{vertexes, indices};
}

GP2Mesh<Vertex> Scene::CreateRectangle(glm::vec2 center, float width, float height, const std::vector<glm::vec3>& colors)
{
	if (colors.size() > 4 || colors.empty())
	{
		return CreateRectangle(center, width, height, { 1.f, 0.f, 1.f });
	}

	std::vector<Vertex> vertexes{};
	std::vector<uint32_t> indices{ 0, 2, 1, 2, 0, 3 };

	for (int i{}; i < 4; ++i)
	{
		int idx = i % colors.size();
		switch (i)
		{
		case 0:
			vertexes.push_back(Vertex{ {center.x - width / 2, center.y - height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		case 1:
			vertexes.push_back(Vertex{ {center.x + width / 2, center.y - height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		case 2:
			vertexes.push_back(Vertex{ {center.x + width / 2, center.y + height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		case 3:
			vertexes.push_back(Vertex{ {center.x - width / 2, center.y + height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		}		
	}

	return GP2Mesh<Vertex>(vertexes, indices);
}

GP2Mesh<Vertex> Scene::CreateRectangleV2(glm::vec2 center, float width, float height, const std::vector<glm::vec3>& colors)
{
	if (colors.size() > 4 || colors.empty())
	{
		return CreateRectangle(center, width, height, { 1.f, 0.f, 1.f });
	}

	std::vector<Vertex> vertexes{};
	std::vector<uint32_t> indices{ 0, 3, 2, 1, 2, 3 };

	for (int i{}; i < 4; ++i)
	{
		int idx = i % colors.size();
		switch (i)
		{
		case 0:
			vertexes.push_back(Vertex{ {center.x - width / 2, center.y - height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		case 1:
			vertexes.push_back(Vertex{ {center.x + width / 2, center.y + height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		case 2:
			vertexes.push_back(Vertex{ {center.x + width / 2, center.y - height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		case 3:
			vertexes.push_back(Vertex{ {center.x - width / 2, center.y + height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		}
	}

	return GP2Mesh<Vertex>(vertexes, indices);
}

GP2Mesh<Vertex> Scene::CreateRectangleV3(glm::vec2 center, float width, float height, const std::vector<glm::vec3>& colors)
{
	if (colors.size() > 4 || colors.empty())
	{
		return CreateRectangle(center, width, height, { 1.f, 0.f, 1.f });
	}

	std::vector<Vertex> vertexes{};
	std::vector<uint32_t> indices{ 0, 2, 1, 1, 2, 3 };

	for (int i{}; i < 4; ++i)
	{
		int idx = i % colors.size();
		switch (i)
		{
		case 0:
			vertexes.push_back(Vertex{ {center.x - width / 2, center.y - height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		case 1:
			vertexes.push_back(Vertex{ {center.x + width / 2, center.y - height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		case 2:
			vertexes.push_back(Vertex{ {center.x - width / 2, center.y + height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		case 3:
			vertexes.push_back(Vertex{ {center.x + width / 2, center.y + height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		}
	}

	return GP2Mesh<Vertex>( vertexes, indices );
}

GP2Mesh<Vertex> Scene::CreateRectangleV4(glm::vec2 center, float width, float height, const std::vector<glm::vec3>& colors)
{
	if (colors.size() > 4 || colors.empty())
	{
		return CreateRectangle(center, width, height, { 1.f, 0.f, 1.f });
	}

	std::vector<Vertex> vertexes{};
	std::vector<uint32_t> indices{ 0, 1, 2, 2, 1, 3 };

	for (int i{}; i < 4; ++i)
	{
		int idx = i % colors.size();
		switch (i)
		{
		case 0:
			vertexes.push_back(Vertex{ {center.x + width / 2, center.y - height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		case 1:
			vertexes.push_back(Vertex{ {center.x - width / 2, center.y - height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		case 2:
			vertexes.push_back(Vertex{ {center.x + width / 2, center.y + height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		case 3:
			vertexes.push_back(Vertex{ {center.x - width / 2, center.y + height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		}
	}

	return GP2Mesh<Vertex>(vertexes, indices);
}

GP2Mesh<Vertex> Scene::CreateRectangleV5(glm::vec2 center, float width, float height, const std::vector<glm::vec3>& colors)
{
	if (colors.size() > 4 || colors.empty())
	{
		return CreateRectangle(center, width, height, { 1.f, 0.f, 1.f });
	}

	std::vector<Vertex> vertexes{};
	std::vector<uint32_t> indices{ 0, 3, 2, 1, 2, 3 };

	for (int i{}; i < 4; ++i)
	{
		int idx = i % colors.size();
		switch (i)
		{
		case 0:
			vertexes.push_back(Vertex{ {center.x + width / 2, center.y + height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		case 1:
			vertexes.push_back(Vertex{ {center.x - width / 2, center.y - height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		case 2:
			vertexes.push_back(Vertex{ {center.x - width / 2, center.y + height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		case 3:
			vertexes.push_back(Vertex{ {center.x + width / 2, center.y - height / 2}, colors[idx], glm::vec2{-1.f, -1.f} });
			break;
		}
	}

	return GP2Mesh<Vertex>(vertexes, indices);
}

GP2Mesh<Vertex> Scene::CreateEllipse(glm::vec2 center, float width, float height, const glm::vec3& color, int nrOfVertexes)
{
	std::vector<Vertex> vertexes{};
	std::vector<uint32_t> indices{};

	vertexes.push_back({ center, color });

	// Calculate angle increment
	float angleIncrement = 2.0f * glm::pi<float>() / nrOfVertexes;
	
	// Generate points
	for (int i = 0; i < nrOfVertexes; ++i)
	{
		float angle = i * angleIncrement;
		float x = center.x + width * 0.5f * std::cos(angle);
		float y = center.y + height * 0.5f * std::sin(angle);
		vertexes.push_back({ glm::vec2(x, y), color, glm::vec2{-1.f, -1.f} });
	}

	for (int i = 0; i < nrOfVertexes; ++i)
	{
		if (i != 0)
		{
			indices.push_back(i);
			indices.push_back(0);
			indices.push_back(i + 1);
		}
		else
		{
			indices.push_back(nrOfVertexes);
			indices.push_back(0);
			indices.push_back(i + 1);
		}
	}

	return GP2Mesh<Vertex>{vertexes, indices};
}

GP2Mesh<Vertex> Scene::CreateEllipse(glm::vec2 center, float width, float height, const std::vector<glm::vec3>& colors, int nrOfVertexes)
{
	if (colors.size() != 2)
	{
		std::cout << "Error, can only create ellipse with 2 colors\n";
		return CreateEllipse(center, width, height, { 1.f, 0.f, 1.f }, nrOfVertexes);
	}

	std::vector<Vertex> vertexes{};
	std::vector<uint32_t> indices{};

	vertexes.push_back({ center, colors[0]});
	// Calculate angle increment
	float angleIncrement = 2.0f * glm::pi<float>() / nrOfVertexes;

	// Generate points
	for (int i = 0; i < nrOfVertexes; ++i)
	{
		float angle = i * angleIncrement;
		float x = center.x + width * 0.5f * std::cos(angle);
		float y = center.y + height * 0.5f * std::sin(angle);
		vertexes.push_back({ glm::vec2(x, y), colors[1], glm::vec2{-1.f, -1.f} });
	}

	for (int i = 0; i < nrOfVertexes; ++i)
	{
		if (i != 0)
		{
			indices.push_back(i);
			indices.push_back(0);
			indices.push_back(i + 1);
		}
		else
		{
			indices.push_back(nrOfVertexes);
			indices.push_back(0);
			indices.push_back(i + 1);
		}
	}

	return GP2Mesh<Vertex>{vertexes, indices};
}

GP2Mesh<Vertex> Scene::CreateRoundedRectangle(glm::vec2 center, float width, float height, float cornerRadius, const glm::vec3& color, int numPoints)
{
	std::vector<Vertex> vertexes{};
	std::vector<uint32_t> indices{};

	//Center rectangle
	GP2Mesh<Vertex> centerRectangle{ CreateRectangle(center, width - cornerRadius * 2, height - cornerRadius * 2, color) };
	std::vector<Vertex> importVertexes = centerRectangle.GetVerticesCopy();
	std::vector<uint32_t> importindices = centerRectangle.GetIndicesCopy();
	uint32_t existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	//Top rectangle
	glm::vec2 topRectangleCenter{ center.x, center.y - (height / 2) + (cornerRadius / 2) };
	GP2Mesh<Vertex> topRectangle{ CreateRectangle(topRectangleCenter, width - cornerRadius * 2, cornerRadius, color) };
	importVertexes = topRectangle.GetVerticesCopy();
	importindices = topRectangle.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	//Bottom rectangle
	glm::vec2 bottomRectangleCenter{ center.x, center.y + (height / 2) - (cornerRadius / 2) };
	GP2Mesh<Vertex> bottomRectangle{ CreateRectangle(bottomRectangleCenter, width - cornerRadius * 2, cornerRadius, color) };
	importVertexes = bottomRectangle.GetVerticesCopy();
	importindices = bottomRectangle.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	//Left rectangle
	glm::vec2 leftRectangleCenter{ center.x - (width / 2) + (cornerRadius / 2), center.y};
	GP2Mesh<Vertex> leftRectangle{ CreateRectangle(leftRectangleCenter, cornerRadius, height - cornerRadius * 2, color) };
	importVertexes = leftRectangle.GetVerticesCopy();
	importindices = leftRectangle.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	//Right rectangle
	glm::vec2 rightRectangleCenter{ center.x + (width / 2) - (cornerRadius / 2), center.y };
	GP2Mesh<Vertex> rightRectangle{ CreateRectangle(rightRectangleCenter, cornerRadius, height - cornerRadius * 2, color) };
	importVertexes = rightRectangle.GetVerticesCopy();
	importindices = rightRectangle.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	// Generate points for top-left corner
	glm::vec2 topLeftCorner{ center.x - (width / 2) + cornerRadius, center.y - (height / 2) + cornerRadius };
	GP2Mesh<Vertex> topLeftMesh = GenerateCorner(topLeftCorner, cornerRadius * 2, numPoints, color, glm::pi<float>());
	importVertexes = topLeftMesh.GetVerticesCopy();
	importindices = topLeftMesh.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });
	
	// Generate points for top-right corner
	glm::vec2 topRightCorner{ center.x + (width / 2) - cornerRadius, center.y - (height / 2) + cornerRadius };
	GP2Mesh<Vertex> topRightMesh = GenerateCorner(topRightCorner, cornerRadius * 2, numPoints, color, (3.f * glm::pi<float>()) / 2);
	importVertexes = topRightMesh.GetVerticesCopy();
	importindices = topRightMesh.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });
	
	// Generate points for bottom-right corner
	glm::vec2 bottomRightCorner{ center.x + (width / 2) - cornerRadius, center.y + (height / 2) - cornerRadius };
	GP2Mesh<Vertex> bottomRightMesh = GenerateCorner(bottomRightCorner, cornerRadius * 2, numPoints, color, 0);
	importVertexes = bottomRightMesh.GetVerticesCopy();
	importindices = bottomRightMesh.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	// Generate points for bottom-left corner
	glm::vec2 bottomLeftCorner{ center.x - (width / 2) + cornerRadius, center.y + (height / 2) - cornerRadius };
	GP2Mesh<Vertex> bottomLeftMesh = GenerateCorner(bottomLeftCorner, cornerRadius * 2, numPoints, color, glm::pi<float>() / 2);
	importVertexes = bottomLeftMesh.GetVerticesCopy();
	importindices = bottomLeftMesh.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	return GP2Mesh<Vertex>(vertexes, indices);
}

GP2Mesh<Vertex> Scene::CreateRoundedRectangle(glm::vec2 center, float width, float height, float cornerRadius, const std::vector<glm::vec3>& colors, int numPoints)
{
	//Center rectangle
	if (colors.size() != 2)
	{
		std::cout << "Error, can only create rounder rectangle with 2 colors\n";
		return CreateRoundedRectangle(center, width, height, cornerRadius, { 1.f, 0.f, 1.f });
	}

	std::vector<Vertex> vertexes{};
	std::vector<uint32_t> indices{ 1, 0, 2, 2, 0, 3, 3, 0, 4, 4, 0, 1 };

	vertexes.push_back({ center, colors[0] });

	vertexes.push_back(Vertex{ {center.x - (width - cornerRadius * 2) / 2, center.y - (height - cornerRadius * 2) / 2}, colors[1], glm::vec2{-1.f, -1.f} });
	vertexes.push_back(Vertex{ {center.x + (width - cornerRadius * 2) / 2, center.y - (height - cornerRadius * 2) / 2}, colors[1], glm::vec2{-1.f, -1.f} });
	vertexes.push_back(Vertex{ {center.x + (width - cornerRadius * 2) / 2, center.y + (height - cornerRadius * 2) / 2}, colors[1], glm::vec2{-1.f, -1.f} });
	vertexes.push_back(Vertex{ {center.x - (width - cornerRadius * 2) / 2, center.y + (height - cornerRadius * 2) / 2}, colors[1], glm::vec2{-1.f, -1.f} });

	//Top rectangle
	glm::vec2 topRectangleCenter{ center.x, center.y - (height / 2) + (cornerRadius / 2) };
	GP2Mesh<Vertex> topRectangle{ CreateRectangle(topRectangleCenter, width - cornerRadius * 2, cornerRadius, colors[1])};
	std::vector<Vertex> importVertexes = topRectangle.GetVerticesCopy();
	std::vector<uint32_t> importindices = topRectangle.GetIndicesCopy();
	uint32_t existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	//Bottom rectangle
	glm::vec2 bottomRectangleCenter{ center.x, center.y + (height / 2) - (cornerRadius / 2) };
	GP2Mesh<Vertex> bottomRectangle{ CreateRectangle(bottomRectangleCenter, width - cornerRadius * 2, cornerRadius, colors[1]) };
	importVertexes = bottomRectangle.GetVerticesCopy();
	importindices = bottomRectangle.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	//Left rectangle
	glm::vec2 leftRectangleCenter{ center.x - (width / 2) + (cornerRadius / 2), center.y };
	GP2Mesh<Vertex> leftRectangle{ CreateRectangle(leftRectangleCenter, cornerRadius, height - cornerRadius * 2, colors[1]) };
	importVertexes = leftRectangle.GetVerticesCopy();
	importindices = leftRectangle.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	//Right rectangle
	glm::vec2 rightRectangleCenter{ center.x + (width / 2) - (cornerRadius / 2), center.y };
	GP2Mesh<Vertex> rightRectangle{ CreateRectangle(rightRectangleCenter, cornerRadius, height - cornerRadius * 2, colors[1]) };
	importVertexes = rightRectangle.GetVerticesCopy();
	importindices = rightRectangle.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	// Generate points for top-left corner
	glm::vec2 topLeftCorner{ center.x - (width / 2) + cornerRadius, center.y - (height / 2) + cornerRadius };
	GP2Mesh<Vertex> topLeftMesh = GenerateCorner(topLeftCorner, cornerRadius * 2, numPoints, colors[1], glm::pi<float>());
	importVertexes = topLeftMesh.GetVerticesCopy();
	importindices = topLeftMesh.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	// Generate points for top-right corner
	glm::vec2 topRightCorner{ center.x + (width / 2) - cornerRadius, center.y - (height / 2) + cornerRadius };
	GP2Mesh<Vertex> topRightMesh = GenerateCorner(topRightCorner, cornerRadius * 2, numPoints, colors[1], (3.f * glm::pi<float>()) / 2);
	importVertexes = topRightMesh.GetVerticesCopy();
	importindices = topRightMesh.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	// Generate points for bottom-right corner
	glm::vec2 bottomRightCorner{ center.x + (width / 2) - cornerRadius, center.y + (height / 2) - cornerRadius };
	GP2Mesh<Vertex> bottomRightMesh = GenerateCorner(bottomRightCorner, cornerRadius * 2, numPoints, colors[1], 0);
	importVertexes = bottomRightMesh.GetVerticesCopy();
	importindices = bottomRightMesh.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	// Generate points for bottom-left corner
	glm::vec2 bottomLeftCorner{ center.x - (width / 2) + cornerRadius, center.y + (height / 2) - cornerRadius };
	GP2Mesh<Vertex> bottomLeftMesh = GenerateCorner(bottomLeftCorner, cornerRadius * 2, numPoints, colors[1], glm::pi<float>() / 2);
	importVertexes = bottomLeftMesh.GetVerticesCopy();
	importindices = bottomLeftMesh.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	return GP2Mesh<Vertex>(vertexes, indices);
}

GP2Mesh<Vertex> Scene::CreateRoundedRectangleV2(glm::vec2 center, float width, float height, float cornerRadius, const std::vector<glm::vec3>& colors, int numPoints)
{
	//Center rectangle
	if (colors.size() != 2)
	{
		std::cout << "Error, can only create rounder rectangle (border) with 2 colors\n";
		return CreateRoundedRectangle(center, width, height, cornerRadius, { 1.f, 0.f, 1.f });
	}

	std::vector<Vertex> vertexes{};
	std::vector<uint32_t> indices{};

	//Center rectangle
	GP2Mesh<Vertex> centerRectangle{ CreateRectangle(center, width - cornerRadius * 2, height - cornerRadius * 2, colors[0]) };
	std::vector<Vertex> importVertexes = centerRectangle.GetVerticesCopy();
	std::vector<uint32_t> importindices = centerRectangle.GetIndicesCopy();
	uint32_t existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	//Top rectangle
	glm::vec2 topRectangleCenter{ center.x, center.y - (height / 2) + (cornerRadius / 2) };
	GP2Mesh<Vertex> topRectangle{ CreateRectangleV5(topRectangleCenter, width - cornerRadius * 2, cornerRadius, colors) };
	importVertexes = topRectangle.GetVerticesCopy();
	importindices = topRectangle.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	//Bottom rectangle
	glm::vec2 bottomRectangleCenter{ center.x, center.y + (height / 2) - (cornerRadius / 2) };
	GP2Mesh<Vertex> bottomRectangle{ CreateRectangleV2(bottomRectangleCenter, width - cornerRadius * 2, cornerRadius, colors) };
	importVertexes = bottomRectangle.GetVerticesCopy();
	importindices = bottomRectangle.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	//Left rectangle
	glm::vec2 leftRectangleCenter{ center.x - (width / 2) + (cornerRadius / 2), center.y };
	GP2Mesh<Vertex> leftRectangle{ CreateRectangleV4(leftRectangleCenter, cornerRadius, height - cornerRadius * 2, colors) };
	importVertexes = leftRectangle.GetVerticesCopy();
	importindices = leftRectangle.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	//Right rectangle
	glm::vec2 rightRectangleCenter{ center.x + (width / 2) - (cornerRadius / 2), center.y };
	GP2Mesh<Vertex> rightRectangle{ CreateRectangleV3(rightRectangleCenter, cornerRadius, height - cornerRadius * 2, colors) };
	importVertexes = rightRectangle.GetVerticesCopy();
	importindices = rightRectangle.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	// Generate points for top-left corner
	glm::vec2 topLeftCorner{ center.x - (width / 2) + cornerRadius, center.y - (height / 2) + cornerRadius };
	GP2Mesh<Vertex> topLeftMesh = GenerateCornerV2(topLeftCorner, cornerRadius * 2, numPoints, colors, glm::pi<float>());
	importVertexes = topLeftMesh.GetVerticesCopy();
	importindices = topLeftMesh.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	// Generate points for top-right corner
	glm::vec2 topRightCorner{ center.x + (width / 2) - cornerRadius, center.y - (height / 2) + cornerRadius };
	GP2Mesh<Vertex> topRightMesh = GenerateCornerV2(topRightCorner, cornerRadius * 2, numPoints, colors, (3.f * glm::pi<float>()) / 2);
	importVertexes = topRightMesh.GetVerticesCopy();
	importindices = topRightMesh.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	// Generate points for bottom-right corner
	glm::vec2 bottomRightCorner{ center.x + (width / 2) - cornerRadius, center.y + (height / 2) - cornerRadius };
	GP2Mesh<Vertex> bottomRightMesh = GenerateCornerV2(bottomRightCorner, cornerRadius * 2, numPoints, colors, 0);
	importVertexes = bottomRightMesh.GetVerticesCopy();
	importindices = bottomRightMesh.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	// Generate points for bottom-left corner
	glm::vec2 bottomLeftCorner{ center.x - (width / 2) + cornerRadius, center.y + (height / 2) - cornerRadius };
	GP2Mesh<Vertex> bottomLeftMesh = GenerateCornerV2(bottomLeftCorner, cornerRadius * 2, numPoints, colors, glm::pi<float>() / 2);
	importVertexes = bottomLeftMesh.GetVerticesCopy();
	importindices = bottomLeftMesh.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	return GP2Mesh<Vertex>(vertexes, indices);
}

GP2Mesh<Vertex> Scene::CreateRoundedRectangleBorder(glm::vec2 center, float width, float height, float cornerRadius, const std::vector<glm::vec3>& colors, int numPoints)
{
	//Center rectangle
	if (colors.size() != 2)
	{
		std::cout << "Error, can only create rounder rectangle (border) with 2 colors\n";
		return CreateRoundedRectangle(center, width, height, cornerRadius, { 1.f, 0.f, 1.f });
	}

	std::vector<Vertex> vertexes{};
	std::vector<uint32_t> indices{};

	//Center rectangle
	GP2Mesh<Vertex> centerRectangle{ CreateRectangle(center, width - cornerRadius * 2, height - cornerRadius * 2, colors[0])};
	std::vector<Vertex> importVertexes = centerRectangle.GetVerticesCopy();
	std::vector<uint32_t> importindices = centerRectangle.GetIndicesCopy();
	uint32_t existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	//Top rectangle
	glm::vec2 topRectangleCenter{ center.x, center.y - (height / 2) + (cornerRadius / 2) };
	GP2Mesh<Vertex> topRectangle{ CreateRectangle(topRectangleCenter, width - cornerRadius * 2, cornerRadius, colors[1])};
	importVertexes = topRectangle.GetVerticesCopy();
	importindices = topRectangle.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	//Bottom rectangle
	glm::vec2 bottomRectangleCenter{ center.x, center.y + (height / 2) - (cornerRadius / 2) };
	GP2Mesh<Vertex> bottomRectangle{ CreateRectangle(bottomRectangleCenter, width - cornerRadius * 2, cornerRadius, colors[1]) };
	importVertexes = bottomRectangle.GetVerticesCopy();
	importindices = bottomRectangle.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	//Left rectangle
	glm::vec2 leftRectangleCenter{ center.x - (width / 2) + (cornerRadius / 2), center.y };
	GP2Mesh<Vertex> leftRectangle{ CreateRectangle(leftRectangleCenter, cornerRadius, height - cornerRadius * 2, colors[1]) };
	importVertexes = leftRectangle.GetVerticesCopy();
	importindices = leftRectangle.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	//Right rectangle
	glm::vec2 rightRectangleCenter{ center.x + (width / 2) - (cornerRadius / 2), center.y };
	GP2Mesh<Vertex> rightRectangle{ CreateRectangle(rightRectangleCenter, cornerRadius, height - cornerRadius * 2, colors[1]) };
	importVertexes = rightRectangle.GetVerticesCopy();
	importindices = rightRectangle.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	// Generate points for top-left corner
	glm::vec2 topLeftCorner{ center.x - (width / 2) + cornerRadius, center.y - (height / 2) + cornerRadius };
	GP2Mesh<Vertex> topLeftMesh = GenerateCorner(topLeftCorner, cornerRadius * 2, numPoints, colors[1], glm::pi<float>());
	importVertexes = topLeftMesh.GetVerticesCopy();
	importindices = topLeftMesh.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	// Generate points for top-right corner
	glm::vec2 topRightCorner{ center.x + (width / 2) - cornerRadius, center.y - (height / 2) + cornerRadius };
	GP2Mesh<Vertex> topRightMesh = GenerateCorner(topRightCorner, cornerRadius * 2, numPoints, colors[1], (3.f * glm::pi<float>()) / 2);
	importVertexes = topRightMesh.GetVerticesCopy();
	importindices = topRightMesh.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	// Generate points for bottom-right corner
	glm::vec2 bottomRightCorner{ center.x + (width / 2) - cornerRadius, center.y + (height / 2) - cornerRadius };
	GP2Mesh<Vertex> bottomRightMesh = GenerateCorner(bottomRightCorner, cornerRadius * 2, numPoints, colors[1], 0);
	importVertexes = bottomRightMesh.GetVerticesCopy();
	importindices = bottomRightMesh.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	// Generate points for bottom-left corner
	glm::vec2 bottomLeftCorner{ center.x - (width / 2) + cornerRadius, center.y + (height / 2) - cornerRadius };
	GP2Mesh<Vertex> bottomLeftMesh = GenerateCorner(bottomLeftCorner, cornerRadius * 2, numPoints, colors[1], glm::pi<float>() / 2);
	importVertexes = bottomLeftMesh.GetVerticesCopy();
	importindices = bottomLeftMesh.GetIndicesCopy();
	existingVertexSize = static_cast<uint16_t>(vertexes.size());

	std::for_each(importVertexes.begin(), importVertexes.end(), [&](const Vertex& v) {vertexes.push_back(v); });
	std::for_each(importindices.begin(), importindices.end(), [&](const uint16_t& i) {indices.push_back(i + existingVertexSize); });

	return GP2Mesh<Vertex>(vertexes, indices);
}

GP2Mesh<Vertex> Scene::GenerateCorner(const glm::vec2& center, float radius, int numPoints, const glm::vec3& color, float initialRotation)
{
	std::vector<Vertex> vertexes;
	std::vector<uint32_t> indices;

	vertexes.push_back({ center, color });

	// Calculate angle increment
	float angleIncrement = (glm::pi<float>() / 2) / (numPoints - 1);

	// Generate points
	for (int i = 0; i < numPoints; ++i)
	{
		float angle = i * angleIncrement + initialRotation;
		float x = center.x + radius * 0.5f * std::cos(angle);
		float y = center.y + radius * 0.5f * std::sin(angle);
		vertexes.push_back({ glm::vec2(x, y), color, glm::vec2{-1.f, -1.f} });
	}

	for (int i = 0; i < numPoints; ++i)
	{
		if (i != 0)
		{
			indices.push_back(i);
			indices.push_back(0);
			indices.push_back(i + 1);
		}
		else
		{
			indices.push_back(numPoints);
			indices.push_back(0);
			indices.push_back(i + 1);
		}
	}

	return { vertexes, indices };
}

GP2Mesh<Vertex> Scene::GenerateCornerV2(const glm::vec2& center, float radius, int numPoints, const std::vector<glm::vec3>& colors, float initialRotation)
{
	std::vector<Vertex> vertexes;
	std::vector<uint32_t> indices;

	vertexes.push_back({ center, colors[0]});

	// Calculate angle increment
	float angleIncrement = (glm::pi<float>() / 2) / (numPoints - 1);

	// Generate points
	for (int i = 0; i < numPoints; ++i)
	{
		float angle = i * angleIncrement + initialRotation;
		float x = center.x + radius * 0.5f * std::cos(angle);
		float y = center.y + radius * 0.5f * std::sin(angle);
		vertexes.push_back({ glm::vec2(x, y), colors[1], glm::vec2{-1.f, -1.f} });
	}

	for (int i = 0; i < numPoints; ++i)
	{
		if (i != 0)
		{
			indices.push_back(i);
			indices.push_back(0);
			indices.push_back(i + 1);
		}
		else
		{
			indices.push_back(numPoints);
			indices.push_back(0);
			indices.push_back(i + 1);
		}
	}

	return GP2Mesh<Vertex>{vertexes, indices};
}
