#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in uint aPrim;
layout(location = 2) in uint aFace;
layout(location = 3) in uint aH0;
layout(location = 4) in uint aH1;
uniform mat4 uMVP;
flat out uint vPrim;
flat out uint vFace;
flat out uint vH0;
flat out uint vH1;
void main() {
    vPrim = aPrim; vFace = aFace; vH0 = aH0; vH1 = aH1;
    gl_Position = uMVP * vec4(aPos, 1.0);
}

#type fragment
#version 330 core
flat in uint vPrim;
flat in uint vFace;
flat in uint vH0;
flat in uint vH1;
layout(location = 0) out uvec4 oId;
uniform samplerBuffer  uFaceState;
uniform usamplerBuffer uPrimState;
void main() {
    uint ff = uint(texelFetch(uFaceState, int(vFace)).a + 0.5);
    uint pf = texelFetch(uPrimState, int(vPrim)).r;
    if ((ff & 1u) == 0u || (pf & 1u) == 0u) discard;
    oId = uvec4(vPrim + 1u, vH0, vH1, 0u);
}