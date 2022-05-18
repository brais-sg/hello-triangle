#include <3ds.h>
#include <citro3d.h>
#include <string.h>
#include "vshader_shbin.h"

#define CLEAR_COLOR 0x000000FF

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    float r, g, b, a;
} Color;

// What's this? Normalized device coordinates will be better
// Lets use NDC
// Actually, no, nothing is shown
static const Vertex vertex_list[] = {
    { 200.0f, 200.0f, 0.f },
	{ 100.0f, 40.0f, 0.f },
	{ 300.0f, 40.0f, 0.f },
};

static const Color color_list[] = {
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 1.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f, 1.0f },
};

int main(){
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

    C3D_RenderTarget* topScreen = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
    C3D_RenderTargetSetOutput(topScreen, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

    // Init scene
    static DVLB_s* vshader_dvlb;
    static shaderProgram_s program;

    vshader_dvlb = DVLB_ParseFile((uint32_t*) vshader_shbin, vshader_shbin_size);
    shaderProgramInit(&program);
    shaderProgramSetVsh(&program, &vshader_dvlb->DVLE[0]);
    C3D_BindProgram(&program);

    static int uLocTransform = shaderInstanceGetUniformLocation(program.vertexShader, "transform");
    
    // Attributes
    C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
    AttrInfo_Init(attrInfo);
    AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // Register v0 as position input (3 float)
    AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 4); // Register v1 as color input (4 float)

    // Identity matrix for transform
    // Orthogonal for now
    static C3D_Mtx transform;
    Mtx_OrthoTilt(&transform, 0.0, 400.0, 0.0, 240.0, -1.0, 1.0, true);


    static void* vbo_data_pos = linearAlloc(sizeof(vertex_list));
    static void* vbo_data_clr = linearAlloc(sizeof(color_list));

    memcpy(vbo_data_pos, vertex_list, sizeof(vertex_list));
    memcpy(vbo_data_clr, color_list, sizeof(color_list));

    // Create VBO
    C3D_BufInfo* bufInfo = C3D_GetBufInfo();
    BufInfo_Init(bufInfo);
    BufInfo_Add(bufInfo, vbo_data_pos, sizeof(Vertex), 1, 0x0);
    BufInfo_Add(bufInfo, vbo_data_clr, sizeof(Color),  1, 0x1);

    // TexEnv
    C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvInit(env);
    // TODO: Take a look at this function
	C3D_TexEnvSrc(env, C3D_Both, GPU_PRIMARY_COLOR, (GPU_TEVSRC) 0, (GPU_TEVSRC) 0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);


    while(aptMainLoop()){
        hidScanInput();
        uint32_t kDown = hidKeysDown();

        if(kDown & KEY_START) break;

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C3D_RenderTargetClear(topScreen, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
        C3D_FrameDrawOn(topScreen);

        C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLocTransform, &transform);
        C3D_DrawArrays(GPU_TRIANGLES, 0, 3);


        C3D_FrameEnd(0);
    }

    linearFree(vbo_data_pos);
    linearFree(vbo_data_clr);

    shaderProgramFree(&program);
    DVLB_Free(vshader_dvlb);


    C3D_Fini();
    gfxExit();
}