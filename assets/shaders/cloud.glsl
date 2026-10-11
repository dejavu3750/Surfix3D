#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;   // r < 0 => use uDefaultColor

uniform mat4  uMVP;
uniform float uSize;
uniform vec3  uDefaultColor;
uniform vec4  uClipPlane;            // (0,0,0,1) => no clip

out vec3 vCol;

void main() {
    gl_Position = uMVP * vec4(aPos, 1.0);
    gl_PointSize = uSize;
    gl_ClipDistance[0] = dot(uClipPlane, vec4(aPos, 1.0));
    vCol = (aCol.r < 0.0) ? uDefaultColor : aCol;
}

#type fragment
#version 330 core
in vec3 vCol;
out vec4 fragColor;

void main() {
    // Round point sprite.
    vec2 d = gl_PointCoord * 2.0 - 1.0;
    if (dot(d, d) > 1.0) discard;
    fragColor = vec4(vCol, 1.0);
}