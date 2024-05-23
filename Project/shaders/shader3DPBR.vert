#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec3 inTangent;
layout(location = 5) in vec3 inBitangent;


layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out mat3 TBN;


void main() {
    // Transform the vertex position to world space
    fragPos = vec3(ubo.model * vec4(inPosition, 1.0));
    fragTexCoord = texCoord;
    
    // Transform the normal, tangent, and bitangent vectors
    vec3 T = normalize(mat3(ubo.model) * inTangent);
    vec3 B = normalize(mat3(ubo.model) * inBitangent);
    vec3 N = normalize(mat3(ubo.model) * inNormal);
    TBN = mat3(T, B, N);

    // Project the vertex position to clip space
    gl_Position = ubo.proj * ubo.view * vec4(fragPos, 1.0);
}