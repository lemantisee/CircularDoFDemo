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
out vec4 FragColor;

in vec2 TexCoord;
uniform int kernelRadius;
uniform int component;
uniform float filterRadius;
layout(binding = 0) uniform sampler2D filterTexture;
layout(binding = 1) uniform sampler2D depthTexture;

vec4 getFilters(int x, float textureWidth)
{
    float u = (float(x) / textureWidth);
    return texture(filterTexture, vec2(u, 0.0));
}

void main()
{
    vec2 filterTextureSize = textureSize(filterTexture, 0).xy;
    vec2 stepVal = 1.0 / filterTextureSize;

    float filterRadius1 = 1.0 - texture(depthTexture, TexCoord).r;
    
    vec4 val = vec4(0, 0, 0, 0);

    for (int i = -kernelRadius; i <= kernelRadius; ++i) {
        vec2 coords = TexCoord + stepVal * vec2(i, 0.0) * filterRadius * filterRadius1;
        float imageTexel = 0;
        
        if (component == 0) {
            imageTexel = texture(filterTexture, coords).r;
        } else if(component == 1) {
            imageTexel = texture(filterTexture, coords).g;
        } else {
            imageTexel = texture(filterTexture, coords).b;
        }

        vec4 c0_c1 = getFilters(i + kernelRadius, filterTextureSize.x);
        val.xy += imageTexel * c0_c1.xy;
        val.zw += imageTexel * c0_c1.zw;
    }
    FragColor = val;
}

@FRAG_END@