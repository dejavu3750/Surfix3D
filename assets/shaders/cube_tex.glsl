#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
uniform mat4 uMVP;
out vec2 vUV;
void main() {
    vUV = aUV;
    gl_Position = uMVP * vec4(aPos, 1.0);
}

#type fragment
#version 330 core
in vec2 vUV;
out vec4 o;
uniform sampler2D uTex;
uniform vec3 uTint;
void main() {
    o = texture(uTex, vUV);   // draw PNG as-is (opaque)
}