#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNrm;
layout(location = 2) in uint aFace;
layout(location = 3) in uint aElem;
layout(location = 7) in vec2 aUV;
uniform mat4 uMVP;
out vec3 vNrm;
out vec2 vUV;
flat out uint vFace;
flat out uint vElem;
void main() {
    vNrm = aNrm; vUV = aUV; vFace = aFace; vElem = aElem;
    gl_Position = uMVP * vec4(aPos, 1.0);
}

#type fragment
#version 330 core
in vec3 vNrm;
in vec2 vUV;
flat in uint vFace;
flat in uint vElem;
out vec4 o;
uniform vec3 uLightDir;
uniform vec3 uSel;
uniform vec3 uHover;
uniform samplerBuffer  uFaceState;
uniform usamplerBuffer uElemState;
uniform sampler2D uTex;
uniform int   uUseTex;
uniform vec3  uTint;
uniform float uExposure;
uniform float uOpacity;
uniform int   uAlphaMode;
uniform float uCutoff;
uniform float uAmbient;
uniform float uDiffuse;
uniform float uSpecular;
uniform float uShininess;
uniform float uRolloff;
uniform float uGamma;
void main() {
    vec4 f  = texelFetch(uFaceState, int(vFace));
    uint ff = uint(f.a + 0.5);
    uint ef = texelFetch(uElemState, int(vElem)).r;
    if ((ff & 1u) == 0u || (ef & 1u) == 0u) discard;
    bool s  = ((ff & 2u) != 0u) || ((ef & 2u) != 0u);
    bool hv = ((ff & 4u) != 0u) || ((ef & 4u) != 0u);

    // Linear albedo + alpha: sRGB texture is decoded by GL, tint is a linear factor.
    vec4 tx  = (uUseTex != 0) ? texture(uTex, vUV) : vec4(1.0);
    vec3 alb = tx.rgb * uTint;

    // Alpha modes: 0 opaque, 1 mask (cutout), 2 blend.
    float a = (uAlphaMode == 0) ? 1.0 : tx.a * uOpacity;
    if (uAlphaMode == 1) { if (a < uCutoff) discard; a = 1.0; }

    // Two-sided headlight: light dir == view dir, so the Blinn half-vector == L.
    float nl = abs(dot(normalize(vNrm), normalize(uLightDir)));
    vec3 lin = alb * (uAmbient + uDiffuse * nl) * uExposure + vec3(uSpecular * pow(nl, uShininess));
    lin = lin / (1.0 + uRolloff * lin);                        // highlight roll-off
    vec3 c = pow(clamp(lin, 0.0, 1.0), vec3(1.0 / uGamma));    // linear -> display

    c = s ? mix(c, uSel, 0.6) : (hv ? mix(c, uHover, 0.5) : c);
    if (s || hv) a = max(a, 0.6);                              // keep selected glass visible
    o = vec4(c, a);
}