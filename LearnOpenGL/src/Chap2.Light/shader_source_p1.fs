#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos; 
in vec3 LightPos; 

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
}; 
  
uniform Material material;

struct Light {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

};

uniform Light light;  

void main()
{
    vec3 object_diffuse = vec3(texture(material.diffuse, TexCoord));
    //
    vec3 ambient = light.ambient * object_diffuse;

    //
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * object_diffuse;  

    //
    vec3 viewDir = normalize(- FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular_map = vec3(texture(material.specular, TexCoord));
    vec3 specular = (specular_map * spec) * light.specular;  

    //
    float distance    = length(LightPos - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                    light.quadratic * (distance * distance));

    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;  

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);

}