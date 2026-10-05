#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNrm;
layout(location = 2) in uint aFace;
layout(location = 3) in uint aElem;
uniform mat4 uMVP;
out vec3 vNrm;
flat out uint vFace;
flat out uint vElem;
void main() {
    vNrm = aNrm; vFace = aFace; vElem = aElem;
    gl_Position = uMVP * vec4(aPos, 1.0);
}

#type fragment
#version 330 core
in vec3 vNrm;
flat in uint vFace;
flat in uint vElem;
out vec4 o;
uniform vec3 uLightDir;
uniform vec3 uSel;
uniform vec3 uHover;
uniform samplerBuffer  uFaceState;
uniform usamplerBuffer uElemState;
void main() {
    vec4 f  = texelFetch(uFaceState, int(vFace));
    uint ff = uint(f.a + 0.5);
    uint ef = texelFetch(uElemState, int(vElem)).r;
    if ((ff & 1u) == 0u || (ef & 1u) == 0u) discard;
    bool s  = ((ff & 2u) != 0u) || ((ef & 2u) != 0u);
    bool hv = ((ff & 4u) != 0u) || ((ef & 4u) != 0u);
    float nl = abs(dot(normalize(vNrm), normalize(uLightDir)));   // two-sided headlight
    float sh = 0.35 + 0.65 * nl;
    vec3 c = s ? mix(f.rgb, uSel, 0.6) : (hv ? mix(f.rgb, uHover, 0.5) : f.rgb);
    o = vec4(c * sh, 1.0);
}