struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight light;
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 texColor = texture(texture_diffuse1, TexCoords).rgb;

    // Lighting calculations
    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 20.0);

    vec3 ambient  = light.ambient  * texColor;
    vec3 diffuse  = light.diffuse  * diff * texColor;
    vec3 specular = light.specular * spec;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
