#version 450

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 TexCoords;
layout(location = 2) in mat3 TBN;

layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;


layout(binding = 1) uniform sampler2D albedoMap;
layout(binding = 2) uniform sampler2D normalMap;
layout(binding = 3) uniform sampler2D metallicMap;
layout(binding = 4) uniform sampler2D roughnessMap;

layout(set = 0, binding = 5) uniform CameraUBO {
    vec3 position;
} camera;

layout(set = 0, binding = 6) uniform LightUBO {
    vec3 direction;
    vec3 color;
} light;

const float PI = 3.14159265359;
const float g_LightIntensity = 7.0f;

vec3 MaxToOne(vec3 v)
{
    float maxValue = max(v.r, max(v.g, v.b));
    if (maxValue > 1.f)
    {
        return vec3(v / maxValue);
    }
    return v;    
}

vec3 Phong(float specular, float exp, vec3 l, vec3 v, vec3 n)
{
    vec3 reflectL = reflect(l, n);
    float cosAlpha = max(0.f, dot(reflectL, v));
    float value = specular * pow(cosAlpha, exp);
    return vec3(value, value, value);
}

float saturate(float value) {
    return clamp(value, 0.0, 1.0);
}

void main() {

    float ambient = 0.25f;

    vec3 binormal = normalize(cross(TBN[2], TBN[0]));

    mat3 tangentSpaceAxis =
    {
        normalize(TBN[0]),
	    binormal,
	    normalize(TBN[2])
    };

    vec4 normalSample = texture(normalMap, TexCoords); //normal
    vec3 normalColor = normalSample.rgb; //normal
        
    normalColor = 2.f * normalColor - vec3(1.f, 1.f, 1.f);

    vec3 tangentSpaceNormal = normalize(normalColor * tangentSpaceAxis);
    
	//observed area  
    float cosineLaw = saturate(dot(tangentSpaceNormal, -light.direction));
    //float cosineLaw = 1;
  
	//sample diffuse color
    vec4 diffuseSample = texture(albedoMap, TexCoords); //Diffuse
    vec3 diffuseColor = diffuseSample.rgb / PI; //Diffuse

    if(cosineLaw < 0)
    {
        FragColor = vec4(diffuseSample.xyz * ambient, 1.f);
        return;
    }

	//Phong
    const float shininess = 25.f;
    vec3 viewDirection = normalize(camera.position - ubo.model[3].xyz);
    
    float specular = texture(metallicMap, TexCoords).r; //Specular
    float phongExp = texture(roughnessMap, TexCoords).r * shininess; //Phong exponent
    
    vec3 phongColor = Phong(specular, phongExp, -light.direction, viewDirection, tangentSpaceNormal);
    
    vec3 returnColor = (g_LightIntensity * diffuseColor) * cosineLaw + phongColor + diffuseSample.xyz * ambient;
    returnColor = MaxToOne(returnColor);
    
    FragColor = vec4(returnColor, 1.f);
}