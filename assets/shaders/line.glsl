#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in uint aPrim;
layout(location = 2) in uint aFace;
uniform mat4 uMVP;
uniform vec4 uClipPlane; // xyz = -n, w = d; (0,0,0,1) = no clip
flat out uint vPrim;
flat out uint vFace;
void main() {
    vPrim = aPrim; vFace = aFace;
    gl_Position = uMVP * vec4(aPos, 1.0);
    gl_ClipDistance[0] = dot(vec4(aPos, 1.0), uClipPlane);
}

#type fragment
#version 330 core
flat in uint vPrim;
flat in uint vFace;
out vec4 o;
uniform vec3 uColor;
uniform vec3 uSel;
uniform vec3 uHover;
uniform samplerBuffer  uFaceState;
uniform usamplerBuffer uPrimState;
void main() {
    vec4 f  = texelFetch(uFaceState, int(vFace));
    uint ff = uint(f.a + 0.5);
    uint pf = texelFetch(uPrimState, int(vPrim)).r;
    if ((ff & 1u) == 0u || (pf & 1u) == 0u) discard;
    o = vec4(((pf & 2u) != 0u) ? uSel : (((pf & 4u) != 0u) ? uHover : uColor), 1.0);
}