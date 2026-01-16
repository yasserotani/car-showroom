#version 330 core
in vec2 vTexCoord;

uniform sampler2D uTexture;
uniform float uAlpha;

out vec4 FragColor;

void main() {
    vec4 tex = texture(uTexture, vTexCoord);
    FragColor = vec4(tex.rgb, tex.a * uAlpha);
}
