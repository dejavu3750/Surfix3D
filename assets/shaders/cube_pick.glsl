#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in uint aId;
uniform mat4 uMVP;
flat out uint vId;
void main() {
    vId = aId;
    gl_Position = uMVP * vec4(aPos, 1.0);
}

#type fragment
#version 330 core
flat in uint vId;
layout(location = 0) out uvec4 oId;
void main() {
    oId = uvec4(vId, 0u, 0u, 0u);
}