@VERTEX_START@

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoord = aTexCoord;
}

@VERTEX_END@

@FRAG_START@
#version 420 core

layout(binding = 0) uniform sampler2D sceneColor;
layout(binding = 1) uniform sampler2D blurColor;
layout(binding = 2) uniform sampler2D cocMap;

in vec2 TexCoord;
out vec4 fragColor;

void main()
{
    float coc = texture(cocMap, TexCoord).r;
    vec3 sharp = texture(sceneColor, TexCoord).rgb;
    vec3 blurred = texture(blurColor, TexCoord).rgb;

    fragColor = vec4(mix(sharp, blurred, clamp(coc, 0.0, 1.0)), 1.0);
}

@FRAG_END@