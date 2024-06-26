#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    if(fragTexCoord.x < 0.f || fragTexCoord.y < 0.f)
    {
        outColor = vec4(fragColor, 1.0);
    }
    else
    {
        outColor = vec4(fragTexCoord, 0.0, 1.0);
    }
}