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
uniform sampler2D textureRGBA;

void main()
{
    FragColor = vec4(texture(textureRGBA, TexCoord).rgb, 1.0);
}

@FRAG_END@