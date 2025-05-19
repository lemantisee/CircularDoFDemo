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

uniform sampler2D sceneDepth;
uniform float focalDepth;   // in linear depth
uniform float near;
uniform float far;

in vec2 TexCoord;
out vec4 fragCoC;

// Convert depth from [0,1] to linear depth in view space
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    float depth = 1.0 - texture(sceneDepth, TexCoord).r;
    float linearDepth = LinearizeDepth(depth);

    float coc = (linearDepth - focalDepth) / linearDepth;
    coc = clamp(coc, 0.0, 1.0); // only far blur
    fragCoC = vec4(coc, 0.0, 0.0, 1.0);   // scale by maximum blur
}

@FRAG_END@