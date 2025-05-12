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
uniform sampler2D aTexture;

const int KERNEL_RADIUS = 8;
const int KERNEL_COUNT = 17;

const vec4 Kernel0BracketsRealXY_ImZW = vec4(-0.038708,0.943062,-0.025574,0.660892);
const vec2 Kernel0Weights_RealX_ImY = vec2(0.411259,-0.548794);
const vec4 Kernel0_RealX_ImY_RealZ_ImW[] = vec4[](
        vec4(/*XY: Non Bracketed*/0.014096,-0.022658,/*Bracketed WZ:*/0.055991,0.004413),
        vec4(/*XY: Non Bracketed*/-0.020612,-0.025574,/*Bracketed WZ:*/0.019188,0.000000),
        vec4(/*XY: Non Bracketed*/-0.038708,0.006957,/*Bracketed WZ:*/0.000000,0.049223),
        vec4(/*XY: Non Bracketed*/-0.021449,0.040468,/*Bracketed WZ:*/0.018301,0.099929),
        vec4(/*XY: Non Bracketed*/0.013015,0.050223,/*Bracketed WZ:*/0.054845,0.114689),
        vec4(/*XY: Non Bracketed*/0.042178,0.038585,/*Bracketed WZ:*/0.085769,0.097080),
        vec4(/*XY: Non Bracketed*/0.057972,0.019812,/*Bracketed WZ:*/0.102517,0.068674),
        vec4(/*XY: Non Bracketed*/0.063647,0.005252,/*Bracketed WZ:*/0.108535,0.046643),
        vec4(/*XY: Non Bracketed*/0.064754,0.000000,/*Bracketed WZ:*/0.109709,0.038697),
        vec4(/*XY: Non Bracketed*/0.063647,0.005252,/*Bracketed WZ:*/0.108535,0.046643),
        vec4(/*XY: Non Bracketed*/0.057972,0.019812,/*Bracketed WZ:*/0.102517,0.068674),
        vec4(/*XY: Non Bracketed*/0.042178,0.038585,/*Bracketed WZ:*/0.085769,0.097080),
        vec4(/*XY: Non Bracketed*/0.013015,0.050223,/*Bracketed WZ:*/0.054845,0.114689),
        vec4(/*XY: Non Bracketed*/-0.021449,0.040468,/*Bracketed WZ:*/0.018301,0.099929),
        vec4(/*XY: Non Bracketed*/-0.038708,0.006957,/*Bracketed WZ:*/0.000000,0.049223),
        vec4(/*XY: Non Bracketed*/-0.020612,-0.025574,/*Bracketed WZ:*/0.019188,0.000000),
        vec4(/*XY: Non Bracketed*/0.014096,-0.022658,/*Bracketed WZ:*/0.055991,0.004413)
);
const vec4 Kernel1BracketsRealXY_ImZW = vec4(0.000115,0.559524,0.000000,0.178226);
const vec2 Kernel1Weights_RealX_ImY = vec2(0.513282,4.561110);
const vec4 Kernel1_RealX_ImY_RealZ_ImW[] = vec4[](
        vec4(/*XY: Non Bracketed*/0.000115,0.009116,/*Bracketed WZ:*/0.000000,0.051147),
        vec4(/*XY: Non Bracketed*/0.005324,0.013416,/*Bracketed WZ:*/0.009311,0.075276),
        vec4(/*XY: Non Bracketed*/0.013753,0.016519,/*Bracketed WZ:*/0.024376,0.092685),
        vec4(/*XY: Non Bracketed*/0.024700,0.017215,/*Bracketed WZ:*/0.043940,0.096591),
        vec4(/*XY: Non Bracketed*/0.036693,0.015064,/*Bracketed WZ:*/0.065375,0.084521),
        vec4(/*XY: Non Bracketed*/0.047976,0.010684,/*Bracketed WZ:*/0.085539,0.059948),
        vec4(/*XY: Non Bracketed*/0.057015,0.005570,/*Bracketed WZ:*/0.101695,0.031254),
        vec4(/*XY: Non Bracketed*/0.062782,0.001529,/*Bracketed WZ:*/0.112002,0.008578),
        vec4(/*XY: Non Bracketed*/0.064754,0.000000,/*Bracketed WZ:*/0.115526,0.000000),
        vec4(/*XY: Non Bracketed*/0.062782,0.001529,/*Bracketed WZ:*/0.112002,0.008578),
        vec4(/*XY: Non Bracketed*/0.057015,0.005570,/*Bracketed WZ:*/0.101695,0.031254),
        vec4(/*XY: Non Bracketed*/0.047976,0.010684,/*Bracketed WZ:*/0.085539,0.059948),
        vec4(/*XY: Non Bracketed*/0.036693,0.015064,/*Bracketed WZ:*/0.065375,0.084521),
        vec4(/*XY: Non Bracketed*/0.024700,0.017215,/*Bracketed WZ:*/0.043940,0.096591),
        vec4(/*XY: Non Bracketed*/0.013753,0.016519,/*Bracketed WZ:*/0.024376,0.092685),
        vec4(/*XY: Non Bracketed*/0.005324,0.013416,/*Bracketed WZ:*/0.009311,0.075276),
        vec4(/*XY: Non Bracketed*/0.000115,0.009116,/*Bracketed WZ:*/0.000000,0.051147)
);

vec4 fetchImage(vec2 coords, sampler2D tex)
{
    vec4 colorImg = texture(tex, coords);    
    
    //luma trick to mimic HDR, and take advantage of 16 bit buffers shader toy provides.
    float lum = dot(colorImg.rgb,vec3(0.2126,0.7152,0.0722))*1.8;
    colorImg = colorImg *(1.0 + 0.2*lum*lum*lum);
    return colorImg*colorImg;
}

void main()
{
    vec4 fragCoord = gl_FragCoord;

    if (int(fragCoord.x) < KERNEL_COUNT && int(fragCoord.y) == 0) {
        vec2 c0 = Kernel0_RealX_ImY_RealZ_ImW[int(fragCoord.x)].xy;
        vec2 c1 = Kernel1_RealX_ImY_RealZ_ImW[int(fragCoord.x)].xy;
        FragColor = vec4(c0.x, c0.y, c1.x, c1.y);        
    } else {
        vec4 color = fetchImage(TexCoord, aTexture);
        FragColor = vec4(color.rgb, 0.5);
    }
}

@FRAG_END@