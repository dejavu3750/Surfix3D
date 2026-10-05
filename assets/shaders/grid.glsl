#type vertex
#version 330 core
layout(location = 0) in vec2 aPos;
uniform mat4 uInvViewProj;
out vec3 vNearPoint;
out vec3 vFarPoint;
vec3 U(float x, float y, float z) {
    vec4 p = uInvViewProj * vec4(x, y, z, 1.0);
    return p.xyz / p.w;
}
void main() {
    vNearPoint = U(aPos.x, aPos.y, -1.0);
    vFarPoint  = U(aPos.x, aPos.y,  1.0);
    gl_Position = vec4(aPos, 0.0, 1.0);
}

#type fragment
#version 330 core
in vec3 vNearPoint;
in vec3 vFarPoint;
uniform mat4  uView;
uniform mat4  uProj;
uniform vec3  uCameraPos;
uniform float uMinorSpacing;
uniform float uMajorEvery;
uniform float uHoverAxis;
uniform float uGridFadeNear;
uniform float uGridFadeFar;
out vec4 FragColor;

float GridFactor(vec2 c, float cs, float lw) {
    vec2 uv = c / cs;
    vec2 d  = fwidth(uv);
    vec2 g  = abs(fract(uv - 0.5) - 0.5) / max(d, vec2(1e-8));
    float l = min(g.x, g.y);
    return 1.0 - clamp(l - (lw - 1.0), 0.0, 1.0);
}

float CD(vec3 w) {
    vec4 c = uProj * uView * vec4(w, 1.0);
    return (c.z / c.w) * 0.5 + 0.5;
}

void main() {
    // Ray vs z=0 plane
    float den = vFarPoint.z - vNearPoint.z;
    if (abs(den) < 1e-6) discard;
    float t = -vNearPoint.z / den;
    if (t < 0.0) discard;
    vec3 wp = vNearPoint + t * (vFarPoint - vNearPoint);
    vec2 xy = wp.xy;

    // LOD: pick a power-of-10 cell size from screen-space derivative
    float minorSp    = max(uMinorSpacing, 1e-4);
    float majorEvery = max(uMajorEvery, 2.0);
    float pw    = max(length(fwidth(xy)), 1e-8);
    float level = max(0.0, log2(pw * 40.0 / minorSp) / log2(10.0));
    float lod   = floor(level);
    float blend = fract(level);
    float cell0 = minorSp * pow(10.0, lod);
    float cell1 = cell0 * majorEvery;

    float gMinor = GridFactor(xy, cell0, 1.0) * (1.0 - blend);
    float gMajor = GridFactor(xy, cell1, 1.2);
    vec3 minorCol = vec3(0.46, 0.46, 0.48);
    vec3 majorCol = vec3(0.62, 0.62, 0.65);
    float minorA = gMinor * 0.30;
    float majorA = gMajor * 0.50;
    vec3 color  = mix(minorCol, majorCol, gMajor);
    float alpha = max(minorA, majorA);

    // Distance fade
    float dist = length(wp.xy - uCameraPos.xy);
    float fade = 1.0 - smoothstep(uGridFadeNear, uGridFadeFar, dist);
    alpha *= fade;

    // X / Y axis lines (+ hover highlight)
    vec2 axisPx = abs(xy) / max(fwidth(xy), vec2(1e-8));
    float xAxis = 1.0 - clamp(axisPx.y - 0.5, 0.0, 1.0);
    float yAxis = 1.0 - clamp(axisPx.x - 0.5, 0.0, 1.0);
    vec3 xCol = vec3(0.80, 0.30, 0.32);
    vec3 yCol = vec3(0.36, 0.68, 0.40);
    if (uHoverAxis == 1.0) xCol = vec3(1.00, 0.55, 0.55);
    if (uHoverAxis == 2.0) yCol = vec3(0.60, 1.00, 0.65);
    color = mix(color, xCol, xAxis);
    color = mix(color, yCol, yAxis);
    alpha = max(alpha, max(xAxis, yAxis) * 0.85);

    if (alpha <= 0.001) discard;
    gl_FragDepth = CD(wp);
    FragColor = vec4(color, alpha);
}