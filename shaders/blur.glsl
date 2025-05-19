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
layout(binding = 1) uniform sampler2D cocMap;
uniform int horizontal;
uniform float maxBlur;

in vec2 TexCoord;
out vec4 fragColor;

void main()
{
    vec2 texelSize = 1.0 / textureSize(sceneColor, 0).xy;
    float coc = texture(cocMap, TexCoord).r * maxBlur;
    vec3 result = vec3(0.0);
    float weightSum = 0.0;

    // Simple 5-tap Gaussian kernel based on CoC
    for (int i = -2; i <= 2; ++i)
    {
        float offset = float(i);
        vec2 sampleOffset = horizontal == 1
            ? vec2(offset * coc * texelSize.x, 0.0)
            : vec2(0.0, offset * coc * texelSize.y);

        float weight = exp(-(offset * offset) / (2.0 * coc * coc + 0.0001));
        result += texture(sceneColor, TexCoord + sampleOffset).rgb * weight;
        weightSum += weight;
    }

    fragColor = vec4(result / weightSum, 1.0);
}

@FRAG_END@