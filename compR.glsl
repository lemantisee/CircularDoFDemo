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

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D filterTexture;

const int KERNEL_RADIUS = 8;

vec4 fetchImage(vec2 coords)
{
    return texture(filterTexture, coords);    
}


vec4 getFilters(int x, float textureWidth)
{
    float u = (float(x) / textureWidth);
    return texture(filterTexture, vec2(u, 0.0));
}

void main()
{
    vec2 filterTextureSize = textureSize(filterTexture, 0).xy;
    vec2 stepVal = 1.0 / filterTextureSize;
    
    vec4 val = vec4(0, 0, 0, 0);
    float filterRadius = texture(filterTexture, TexCoord).a;

    for (int i=-KERNEL_RADIUS; i <=KERNEL_RADIUS; ++i) {
        vec2 coords = TexCoord + stepVal * vec2(i, 0.0) * filterRadius;
        float imageTexelR = texture(filterTexture, coords).r;
        vec4 c0_c1 = getFilters(i + KERNEL_RADIUS, filterTextureSize.x);
        val.xy += imageTexelR * c0_c1.xy;
        val.zw += imageTexelR * c0_c1.zw;        
    }
    FragColor = val;
}

@FRAG_END@