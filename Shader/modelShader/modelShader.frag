#version 330 core
out vec4 FragColor;


in GE_OUT{
    vec3 FragPos;
	vec3 Normal;
    vec2 TexCoords;
} ge_out;

struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
	sampler2D texture_normal1;
    sampler2D texture_reflect1;
    float shininess;
};

struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4

struct SpotLight{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};


uniform DirLight dirLight;
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform Material material;
uniform samplerCube skybox;
uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


float near = 0.1; 
float far  = 100.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

vec3 Normal = ge_out.Normal;
vec3 FragPos = ge_out.FragPos;
vec2 TexCoords = ge_out.TexCoords;

void main()
{
    
    vec3 norm = normalize(Normal);
	//norm = normalize(texture(material.texture_normal1,TexCoords).rgb);
    vec3 viewDir = normalize(viewPos-FragPos);

    vec3 result = CalcDirLight(dirLight,norm,viewDir);

    for(int i=0;i<NR_POINT_LIGHTS;i++){
        result += CalcPointLight(pointLight[i],norm,FragPos,viewDir);
    }
    
    result +=CalcSpotLight(spotLight,norm,FragPos,viewDir);
    
    float depth = LinearizeDepth(gl_FragCoord.z);

    float reflectElement = texture(material.texture_reflect1,TexCoords).r;
    
    vec3 reflectDir = reflect(-viewDir,norm);

    vec3 reflectColor = texture(skybox,reflectDir).rgb*reflectElement;

    result+= reflectColor; 

    gl_FragColor = vec4(result,1.0);   
}

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
	
    float diff = max(dot(normal,lightDir),0.0);

    vec3 reflectDir = reflect(-lightDir,normal);

    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1,TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1,TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1,TexCoords));

    return vec3(ambient+diffuse+specular);
}

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal,lightDir),0.0);
    
    vec3 reflectDir = reflect(-lightDir,normal);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance  = length(light.position - FragPos);//¹âÕÕ¾àÀë
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);//¹âÕÕË¥¼õ

    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal,lightDir),0.0);
    
    vec3 reflectDir = reflect(-lightDir,normal);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance  = length(light.position - FragPos);//¹âÕÕ¾àÀë
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);//¹âÕÕË¥¼õ

    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    float theta = dot(lightDir,normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff)/epsilon,0.0,1.0);

    diffuse *= intensity;
    specular *= intensity;

    return (ambient+diffuse+specular);
}