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

uniform sampler2D compRTexture;
uniform sampler2D compGTexture;
uniform sampler2D compBTexture;

#define KERNEL_COMPONENTS 2
const int KERNEL_RADIUS = 8;
const int KERNEL_COUNT = 17;

const vec2 Kernel0Weights_RealX_ImY = vec2(0.411259, -0.548794);
const vec2 Kernel1Weights_RealX_ImY = vec2(0.513282, 4.561110);

//(Pr+Pi)*(Qr+Qi) = (Pr*Qr+Pr*Qi+Pi*Qr-Pi*Qi)
vec2 multComplex(vec2 p, vec2 q)
{
    return vec2(p.x * q.x - p.y * q.y, p.x * q.y + p.y * q.x);
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
  
    vec4 valR = vec4(0,0,0,0);
    vec4 valG = vec4(0,0,0,0);
    vec4 valB = vec4(0,0,0,0);

    float filterRadius = texture(filterTexture, TexCoord).a;

    for (int i=-KERNEL_RADIUS; i <=KERNEL_RADIUS; ++i) {
        vec2 coords = TexCoord + stepVal * vec2(0.0, float(i)) * filterRadius;
        vec4 imageTexelR = texture(compRTexture, coords);  
        vec4 imageTexelG = texture(compGTexture, coords);  
        vec4 imageTexelB = texture(compBTexture, coords);  
        
        vec4 c0_c1 = getFilters(i + KERNEL_RADIUS, filterTextureSize.x);
        
        valR.xy += multComplex(imageTexelR.xy,c0_c1.xy);
        valR.zw += multComplex(imageTexelR.zw,c0_c1.zw);
        
        valG.xy += multComplex(imageTexelG.xy,c0_c1.xy);
        valG.zw += multComplex(imageTexelG.zw,c0_c1.zw);
        
        valB.xy += multComplex(imageTexelB.xy,c0_c1.xy);
        valB.zw += multComplex(imageTexelB.zw,c0_c1.zw);       
    }
    
    float redChannel   = dot(valR.xy, Kernel0Weights_RealX_ImY) + dot(valR.zw, Kernel1Weights_RealX_ImY);
    float greenChannel = dot(valG.xy, Kernel0Weights_RealX_ImY) + dot(valG.zw, Kernel1Weights_RealX_ImY);
    float blueChannel  = dot(valB.xy, Kernel0Weights_RealX_ImY) + dot(valB.zw, Kernel1Weights_RealX_ImY);
    FragColor = vec4(sqrt(vec3(redChannel, greenChannel, blueChannel)), 1.0);
}

@FRAG_END@