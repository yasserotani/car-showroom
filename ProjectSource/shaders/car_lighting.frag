#version 330 core
in vec3 vFragPos;
in vec3 vNormal;
in vec2 vTexCoord;

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform sampler2D uTexture;

out vec4 FragColor;

void main() {
    // Simple Blinn-Phong
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(uLightPos - vFragPos);
    vec3 viewDir = normalize(uViewPos - vFragPos);

    // ambient
    vec3 ambient = 0.1 * texture(uTexture, vTexCoord).rgb;
    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * texture(uTexture, vTexCoord).rgb;
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.3) * spec;

    vec3 color = ambient + diffuse + specular;
    FragColor = vec4(color, 1.0);
}
