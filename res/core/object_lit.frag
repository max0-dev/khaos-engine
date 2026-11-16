#version 460

#define MAX_POINT_LIGHT_COUNT 8

out vec4 FragColor;

in vec2 outUV;
in vec3 outNormal;
in vec3 outFragPos;

struct DirectionalLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {

    vec3 albedo;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float shinnines;

    sampler2D albedoTex;
    sampler2D diffuseTex;
    sampler2D specularTex;
};

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform DirectionalLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHT_COUNT];
uniform Material material;

vec3 CalcDirLight(DirectionalLight light, vec3 viewDir, vec3 normal, Material mat, vec2 uv) {
    vec3 lightDir = normalize(-light.direction);

    vec3 ambient = light.ambient * mat.ambient;

    float diffuseStrength = max(dot(lightDir, normal), 0.0);
    vec3 materialDiffuse;
    materialDiffuse = vec3(texture(mat.diffuseTex, uv)) * mat.diffuse;
    vec3 diffuse = diffuseStrength * light.diffuse * materialDiffuse;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shinnines);
    vec3 materialSpecular;
    materialSpecular = vec3(texture(mat.specularTex, uv)) * mat.specular;
    vec3 specular = spec * light.specular * materialSpecular;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material mat, vec2 uv){
    
    if(light.constant != 1.0){
        return vec3(0);
    }
    
    vec3 lightDir = normalize(light.position - fragPos);

    vec3 ambient = light.ambient * mat.ambient;

    float diffuseStrength = max(dot(lightDir, normal), 0.0);
    vec3 materialDiffuse;
    materialDiffuse = vec3(texture(mat.diffuseTex, uv)) * mat.diffuse;
    vec3 diffuse = diffuseStrength * light.diffuse * materialDiffuse;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shinnines);
    vec3 materialSpecular;
    materialSpecular = vec3(texture(mat.specularTex, uv)) * mat.specular;
    vec3 specular = spec * light.specular * materialSpecular;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

void main(){
    
    vec3 baseColor;
    baseColor = vec3(texture(material.albedoTex, outUV)) * material.albedo;

    vec3 normal = normalize(outNormal);
    vec3 viewDir = normalize(cameraPos - outFragPos);

    vec3 finalColor = CalcDirLight(dirLight, viewDir, normal, material, outUV);

    for(int i = 0; i < MAX_POINT_LIGHT_COUNT; i++){
        finalColor += CalcPointLight(pointLights[i], normal, outFragPos, viewDir, material, outUV);
    }
    
    FragColor = vec4(finalColor * baseColor, 1.0);
}
