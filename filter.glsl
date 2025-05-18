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
uniform int kernelRadius;
uniform sampler2D aTexture;

const vec4 Kernel0BracketsRealXY_ImZW = vec4(-0.076732,1.599408,-0.044915,1.014693);
const vec2 Kernel0Weights_RealX_ImY = vec2(0.411259,-0.548794);
const vec4 Kernel0_RealX_ImY_RealZ_ImW[] = vec4[](
        vec4(/*XY: Non Bracketed*/-0.002256,0.002935,/*Bracketed WZ:*/0.046565,0.047157),
        vec4(/*XY: Non Bracketed*/-0.007732,-0.003528,/*Bracketed WZ:*/0.043141,0.040788),
        vec4(/*XY: Non Bracketed*/0.013230,-0.011402,/*Bracketed WZ:*/0.056247,0.033028),
        vec4(/*XY: Non Bracketed*/-0.011877,0.029851,/*Bracketed WZ:*/0.040549,0.073683),
        vec4(/*XY: Non Bracketed*/0.027942,-0.044915,/*Bracketed WZ:*/0.065446,0.000000),
        vec4(/*XY: Non Bracketed*/-0.076732,0.013791,/*Bracketed WZ:*/0.000000,0.057856),
        vec4(/*XY: Non Bracketed*/0.025801,0.099560,/*Bracketed WZ:*/0.064107,0.142383),
        vec4(/*XY: Non Bracketed*/0.114922,0.039275,/*Bracketed WZ:*/0.119828,0.082971),
        vec4(/*XY: Non Bracketed*/0.128366,0.000000,/*Bracketed WZ:*/0.128234,0.044265),
        vec4(/*XY: Non Bracketed*/0.114922,0.039275,/*Bracketed WZ:*/0.119828,0.082971),
        vec4(/*XY: Non Bracketed*/0.025801,0.099560,/*Bracketed WZ:*/0.064107,0.142383),
        vec4(/*XY: Non Bracketed*/-0.076732,0.013791,/*Bracketed WZ:*/0.000000,0.057856),
        vec4(/*XY: Non Bracketed*/0.027942,-0.044915,/*Bracketed WZ:*/0.065446,0.000000),
        vec4(/*XY: Non Bracketed*/-0.011877,0.029851,/*Bracketed WZ:*/0.040549,0.073683),
        vec4(/*XY: Non Bracketed*/0.013230,-0.011402,/*Bracketed WZ:*/0.056247,0.033028),
        vec4(/*XY: Non Bracketed*/-0.007732,-0.003528,/*Bracketed WZ:*/0.043141,0.040788),
        vec4(/*XY: Non Bracketed*/-0.002256,0.002935,/*Bracketed WZ:*/0.046565,0.047157)
);
const vec4 Kernel1BracketsRealXY_ImZW = vec4(-0.004562,0.620527,-0.000555,0.198928);
const vec2 Kernel1Weights_RealX_ImY = vec2(0.513282,4.561110);
const vec4 Kernel1_RealX_ImY_RealZ_ImW[] = vec4[](
        vec4(/*XY: Non Bracketed*/0.000050,-0.000003,/*Bracketed WZ:*/0.007432,0.002779),
        vec4(/*XY: Non Bracketed*/0.000019,-0.000316,/*Bracketed WZ:*/0.007381,0.001202),
        vec4(/*XY: Non Bracketed*/-0.001456,-0.000555,/*Bracketed WZ:*/0.005004,0.000000),
        vec4(/*XY: Non Bracketed*/-0.004562,0.003896,/*Bracketed WZ:*/0.000000,0.022378),
        vec4(/*XY: Non Bracketed*/0.000227,0.018071,/*Bracketed WZ:*/0.007717,0.093633),
        vec4(/*XY: Non Bracketed*/0.027264,0.032746,/*Bracketed WZ:*/0.051288,0.167405),
        vec4(/*XY: Non Bracketed*/0.072739,0.029862,/*Bracketed WZ:*/0.124573,0.152907),
        vec4(/*XY: Non Bracketed*/0.113025,0.011042,/*Bracketed WZ:*/0.189494,0.058301),
        vec4(/*XY: Non Bracketed*/0.128366,0.000000,/*Bracketed WZ:*/0.214218,0.002792),
        vec4(/*XY: Non Bracketed*/0.113025,0.011042,/*Bracketed WZ:*/0.189494,0.058301),
        vec4(/*XY: Non Bracketed*/0.072739,0.029862,/*Bracketed WZ:*/0.124573,0.152907),
        vec4(/*XY: Non Bracketed*/0.027264,0.032746,/*Bracketed WZ:*/0.051288,0.167405),
        vec4(/*XY: Non Bracketed*/0.000227,0.018071,/*Bracketed WZ:*/0.007717,0.093633),
        vec4(/*XY: Non Bracketed*/-0.004562,0.003896,/*Bracketed WZ:*/0.000000,0.022378),
        vec4(/*XY: Non Bracketed*/-0.001456,-0.000555,/*Bracketed WZ:*/0.005004,0.000000),
        vec4(/*XY: Non Bracketed*/0.000019,-0.000316,/*Bracketed WZ:*/0.007381,0.001202),
        vec4(/*XY: Non Bracketed*/0.000050,-0.000003,/*Bracketed WZ:*/0.007432,0.002779)
);

// const vec4 Kernel0BracketsRealXY_ImZW = vec4(-0.043756,1.035962,-0.034272,0.850312);
// const vec2 Kernel0Weights_RealX_ImY = vec2(0.411259,-0.548794);
// const vec4 Kernel0_RealX_ImY_RealZ_ImW[] = vec4[](
//         vec4(/*XY: Non Bracketed*/-0.016143,0.009250,/*Bracketed WZ:*/0.026655,0.051184),
//         vec4(/*XY: Non Bracketed*/0.022748,0.013460,/*Bracketed WZ:*/0.064196,0.056135),
//         vec4(/*XY: Non Bracketed*/0.010465,-0.034272,/*Bracketed WZ:*/0.052339,0.000000),
//         vec4(/*XY: Non Bracketed*/-0.043756,-0.015316,/*Bracketed WZ:*/0.000000,0.022293),
//         vec4(/*XY: Non Bracketed*/-0.034878,0.045377,/*Bracketed WZ:*/0.008570,0.093670),
//         vec4(/*XY: Non Bracketed*/0.021120,0.064029,/*Bracketed WZ:*/0.062624,0.115607),
//         vec4(/*XY: Non Bracketed*/0.064357,0.040038,/*Bracketed WZ:*/0.104360,0.087392),
//         vec4(/*XY: Non Bracketed*/0.080523,0.011276,/*Bracketed WZ:*/0.119965,0.053567),
//         vec4(/*XY: Non Bracketed*/0.083235,0.000000,/*Bracketed WZ:*/0.122583,0.040306),
//         vec4(/*XY: Non Bracketed*/0.080523,0.011276,/*Bracketed WZ:*/0.119965,0.053567),
//         vec4(/*XY: Non Bracketed*/0.064357,0.040038,/*Bracketed WZ:*/0.104360,0.087392),
//         vec4(/*XY: Non Bracketed*/0.021120,0.064029,/*Bracketed WZ:*/0.062624,0.115607),
//         vec4(/*XY: Non Bracketed*/-0.034878,0.045377,/*Bracketed WZ:*/0.008570,0.093670),
//         vec4(/*XY: Non Bracketed*/-0.043756,-0.015316,/*Bracketed WZ:*/0.000000,0.022293),
//         vec4(/*XY: Non Bracketed*/0.010465,-0.034272,/*Bracketed WZ:*/0.052339,0.000000),
//         vec4(/*XY: Non Bracketed*/0.022748,0.013460,/*Bracketed WZ:*/0.064196,0.056135),
//         vec4(/*XY: Non Bracketed*/-0.016143,0.009250,/*Bracketed WZ:*/0.026655,0.051184)
// );
// const vec4 Kernel1BracketsRealXY_ImZW = vec4(-0.002837,0.592696,0.000000,0.190608);
// const vec2 Kernel1Weights_RealX_ImY = vec2(0.513282,4.561110);
// const vec4 Kernel1_RealX_ImY_RealZ_ImW[] = vec4[](
//         vec4(/*XY: Non Bracketed*/-0.002647,0.001474,/*Bracketed WZ:*/0.000322,0.007734),
//         vec4(/*XY: Non Bracketed*/-0.002837,0.005943,/*Bracketed WZ:*/0.000000,0.031182),
//         vec4(/*XY: Non Bracketed*/0.001154,0.012858,/*Bracketed WZ:*/0.006734,0.067456),
//         vec4(/*XY: Non Bracketed*/0.011772,0.019544,/*Bracketed WZ:*/0.024648,0.102533),
//         vec4(/*XY: Non Bracketed*/0.028758,0.022243,/*Bracketed WZ:*/0.053307,0.116694),
//         vec4(/*XY: Non Bracketed*/0.048693,0.018904,/*Bracketed WZ:*/0.086943,0.099179),
//         vec4(/*XY: Non Bracketed*/0.066752,0.011087,/*Bracketed WZ:*/0.117411,0.058165),
//         vec4(/*XY: Non Bracketed*/0.078969,0.003251,/*Bracketed WZ:*/0.138024,0.017057),
//         vec4(/*XY: Non Bracketed*/0.083235,0.000000,/*Bracketed WZ:*/0.145222,0.000000),
//         vec4(/*XY: Non Bracketed*/0.078969,0.003251,/*Bracketed WZ:*/0.138024,0.017057),
//         vec4(/*XY: Non Bracketed*/0.066752,0.011087,/*Bracketed WZ:*/0.117411,0.058165),
//         vec4(/*XY: Non Bracketed*/0.048693,0.018904,/*Bracketed WZ:*/0.086943,0.099179),
//         vec4(/*XY: Non Bracketed*/0.028758,0.022243,/*Bracketed WZ:*/0.053307,0.116694),
//         vec4(/*XY: Non Bracketed*/0.011772,0.019544,/*Bracketed WZ:*/0.024648,0.102533),
//         vec4(/*XY: Non Bracketed*/0.001154,0.012858,/*Bracketed WZ:*/0.006734,0.067456),
//         vec4(/*XY: Non Bracketed*/-0.002837,0.005943,/*Bracketed WZ:*/0.000000,0.031182),
//         vec4(/*XY: Non Bracketed*/-0.002647,0.001474,/*Bracketed WZ:*/0.000322,0.007734)
// );

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
    int kernelWidth = kernelRadius * 2 + 1;

    if (int(fragCoord.x) < kernelWidth && int(fragCoord.y) == 0) {
        vec2 c0 = Kernel0_RealX_ImY_RealZ_ImW[int(fragCoord.x)].xy;
        vec2 c1 = Kernel1_RealX_ImY_RealZ_ImW[int(fragCoord.x)].xy;
        FragColor = vec4(c0.x, c0.y, c1.x, c1.y);        
    } else {
        vec4 color = fetchImage(TexCoord, aTexture);
        FragColor = vec4(color.rgb, 1.0);
    }
}

@FRAG_END@