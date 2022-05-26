/**
 * @file main.cpp
 * @author Brais Solla G.
 * @brief LIBGXM (PSVITA) Hello Triangle
 * @version 0.1
 * @date 2022-05-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// VITASDK
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>

#include <psp2/gxm.h>
#include <psp2/display.h>
#include <psp2/ctrl.h>
#include <psp2/kernel/sysmem.h>

// #include "gpu_utils.h"

// SHADERS
#include "shader_bin/clear_v.h"
#include "shader_bin/clear_f.h"
#include "shader_bin/color_v.h"
#include "shader_bin/color_f.h"

typedef struct {
	float x, y;
} vector2f;

typedef struct {
	union { float x; float r; };
	union { float y; float g; };
	union { float z; float b; };
} vector3f;

typedef struct {
	union { float x; float r; };
	union { float y; float g; };
	union { float z; float b; };
	union { float w; float a; };
} vector4f;

struct clear_vertex {
	vector2f position;
};

struct position_vertex {
	vector3f position;
};

struct color_vertex {
	vector3f position;
	vector4f color;
};



static const SceGxmProgram *const gxm_program_clear_v  = (SceGxmProgram *) &clear_v_gxp;
static const SceGxmProgram *const gxm_program_clear_f  = (SceGxmProgram *) &clear_f_gxp;
static const SceGxmProgram *const gxm_program_color_v  = (SceGxmProgram *) &color_v_gxp;
static const SceGxmProgram *const gxm_program_color_f  = (SceGxmProgram *) &color_f_gxp;


#define DISPLAY_WIDTH 960
#define DISPLAY_HEIGHT 544
#define DISPLAY_STRIDE 1024
#define DISPLAY_BUFFER_COUNT 2
#define DISPLAY_COLOR_FORMAT SCE_GXM_COLOR_FORMAT_A8B8G8R8
#define DISPLAY_PIXEL_FORMAT SCE_DISPLAY_PIXELFORMAT_A8B8G8R8
#define MAX_PENDING_SWAPS (DISPLAY_BUFFER_COUNT - 1)

#define ALIGN(x, a) (((x) + ((a) - 1)) & ~((a) - 1))
#define abs(x) (((x) < 0) ? -(x) : (x))

struct display_queue_callback_data {
	void *addr;
};

// https://github.com/xerpi/gxmfun/blob/master/source/main.c
// Take a look at how this exactly works
static void display_queue_callback(const void *callbackData){
    SceDisplayFrameBuf display_fb;
    const struct display_queue_callback_data* cb_data = (const struct display_queue_callback_data*) callbackData;
    
    memset(&display_fb, 0, sizeof(display_fb));
    display_fb.size  = sizeof(display_fb);
    display_fb.base  = cb_data->addr;
    display_fb.pitch = DISPLAY_STRIDE;
    display_fb.pixelformat = DISPLAY_PIXEL_FORMAT;
    display_fb.width = DISPLAY_WIDTH;
    display_fb.height = DISPLAY_HEIGHT;

    sceDisplaySetFrameBuf(&display_fb, SCE_DISPLAY_SETBUF_NEXTFRAME);
    sceDisplayWaitVblankStart();
}


static SceGxmContext *gxm_context;
static SceUID vdm_ring_buffer_uid;
static void *vdm_ring_buffer_addr;
static SceUID vertex_ring_buffer_uid;
static void *vertex_ring_buffer_addr;
static SceUID fragment_ring_buffer_uid;
static void *fragment_ring_buffer_addr;
static SceUID fragment_usse_ring_buffer_uid;
static void *fragment_usse_ring_buffer_addr;
static SceGxmRenderTarget *gxm_render_target;
static SceGxmColorSurface gxm_color_surfaces[DISPLAY_BUFFER_COUNT];
static SceUID gxm_color_surfaces_uid[DISPLAY_BUFFER_COUNT];
static void *gxm_color_surfaces_addr[DISPLAY_BUFFER_COUNT];
static SceGxmSyncObject *gxm_sync_objects[DISPLAY_BUFFER_COUNT];
static unsigned int gxm_front_buffer_index;
static unsigned int gxm_back_buffer_index;
static SceUID gxm_depth_stencil_surface_uid;
static void *gxm_depth_stencil_surface_addr;
static SceGxmDepthStencilSurface gxm_depth_stencil_surface;
static SceGxmShaderPatcher *gxm_shader_patcher;
static SceUID gxm_shader_patcher_buffer_uid;
static void *gxm_shader_patcher_buffer_addr;
static SceUID gxm_shader_patcher_vertex_usse_uid;
static void *gxm_shader_patcher_vertex_usse_addr;
static SceUID gxm_shader_patcher_fragment_usse_uid;
static void *gxm_shader_patcher_fragment_usse_addr;

static SceGxmShaderPatcherId gxm_color_buffer_vertex_program_id;
static SceGxmShaderPatcherId gxm_color_buffer_fragment_program_id;
static const SceGxmProgramParameter *gxm_color_buffer_vertex_program_position_param;
static const SceGxmProgramParameter *gxm_color_buffer_vertex_program_color_param;
static const SceGxmProgramParameter *gxm_color_buffer_vertex_program_u_mvp_matrix_param;
static SceGxmVertexProgram *gxm_color_buffer_vertex_program_patched;
static SceGxmFragmentProgram *gxm_color_buffer_fragment_program_patched;

static SceGxmShaderPatcherId gxm_clear_vertex_program_id;
static SceGxmShaderPatcherId gxm_clear_fragment_program_id;
static const SceGxmProgramParameter *gxm_clear_vertex_program_position_param;
static const SceGxmProgramParameter *gxm_clear_fragment_program_u_clear_color_param;
static SceGxmVertexProgram *gxm_clear_vertex_program_patched;
static SceGxmFragmentProgram *gxm_clear_fragment_program_patched;

static SceGxmShaderPatcherId gxm_cube_vertex_program_id;
static SceGxmShaderPatcherId gxm_cube_fragment_program_id;




static void *gpu_alloc_map(SceKernelMemBlockType type, SceGxmMemoryAttribFlags gpu_attrib, size_t size, SceUID *uid);
static void gpu_unmap_free(SceUID uid);
static void *gpu_vertex_usse_alloc_map(size_t size, SceUID *uid, unsigned int *usse_offset);
static void gpu_vertex_usse_unmap_free(SceUID uid);
static void *gpu_fragment_usse_alloc_map(size_t size, SceUID *uid, unsigned int *usse_offset);
static void gpu_fragment_usse_unmap_free(SceUID uid);

static void *shader_patcher_host_alloc_cb(void *user_data, unsigned int size);
static void shader_patcher_host_free_cb(void *user_data, void *mem);

void set_vertex_default_uniform_data(const SceGxmProgramParameter *param, unsigned int component_count, const void *data){
	void *uniform_buffer;
	sceGxmReserveVertexDefaultUniformBuffer(gxm_context, &uniform_buffer);
	sceGxmSetUniformDataF(uniform_buffer, param, 0, component_count, (const float*) data);
}

void set_fragment_default_uniform_data(const SceGxmProgramParameter *param, unsigned int component_count, const void *data){
	void *uniform_buffer;
	sceGxmReserveFragmentDefaultUniformBuffer(gxm_context, &uniform_buffer);
	sceGxmSetUniformDataF(uniform_buffer, param, 0, component_count, (const float*) data);
}


// Before doing nothing: Take a look at xerpi's gxmfun repository
int main(int argc, char* argv[]){
    SceGxmInitializeParams gxm_init_params;
    memset(&gxm_init_params, 0, sizeof(gxm_init_params));

    gxm_init_params.flags = 0;
    gxm_init_params.displayQueueMaxPendingCount = MAX_PENDING_SWAPS;
    gxm_init_params.displayQueueCallback = display_queue_callback;
    gxm_init_params.displayQueueCallbackDataSize = sizeof(struct display_queue_callback_data);
    gxm_init_params.parameterBufferSize = SCE_GXM_DEFAULT_PARAMETER_BUFFER_SIZE;

    sceGxmInitialize(&gxm_init_params);

    // Initialize GPU rings
    vdm_ring_buffer_addr      = gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW, SCE_GXM_MEMORY_ATTRIB_READ, SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE, &vdm_ring_buffer_uid);
    vertex_ring_buffer_addr   = gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW, SCE_GXM_MEMORY_ATTRIB_READ, SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE, &vertex_ring_buffer_uid);
    fragment_ring_buffer_addr = gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW, SCE_GXM_MEMORY_ATTRIB_READ, SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE, &fragment_ring_buffer_uid);

    unsigned int fragment_usse_offset;
    fragment_usse_ring_buffer_addr = gpu_fragment_usse_alloc_map(SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE, &fragment_ring_buffer_uid, &fragment_usse_offset);

    SceGxmContextParams gxm_context_params;

    memset(&gxm_context_params, 0, sizeof(SceGxmContextParams));
	gxm_context_params.hostMem = malloc(SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE);
	gxm_context_params.hostMemSize = SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE;
	gxm_context_params.vdmRingBufferMem = vdm_ring_buffer_addr;
	gxm_context_params.vdmRingBufferMemSize = SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE;
	gxm_context_params.vertexRingBufferMem = vertex_ring_buffer_addr;
	gxm_context_params.vertexRingBufferMemSize = SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE;
	gxm_context_params.fragmentRingBufferMem = fragment_ring_buffer_addr;
	gxm_context_params.fragmentRingBufferMemSize = SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE;
	gxm_context_params.fragmentUsseRingBufferMem = fragment_usse_ring_buffer_addr;
	gxm_context_params.fragmentUsseRingBufferMemSize = SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE;
	gxm_context_params.fragmentUsseRingBufferOffset = fragment_usse_offset;

	sceGxmCreateContext(&gxm_context_params, &gxm_context);

    SceGxmRenderTargetParams render_target_params;
	memset(&render_target_params, 0, sizeof(render_target_params));
	render_target_params.flags = 0;
	render_target_params.width = DISPLAY_WIDTH;
	render_target_params.height = DISPLAY_HEIGHT;
	render_target_params.scenesPerFrame = 1;
	render_target_params.multisampleMode = SCE_GXM_MULTISAMPLE_NONE;
	render_target_params.multisampleLocations = 0;
	render_target_params.driverMemBlock = -1;

	sceGxmCreateRenderTarget(&render_target_params, &gxm_render_target);

    for (int i = 0; i < DISPLAY_BUFFER_COUNT; i++) {
		gxm_color_surfaces_addr[i] = gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
			(SceGxmMemoryAttribFlags) (SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE),
			ALIGN(4 * DISPLAY_STRIDE * DISPLAY_HEIGHT, 1 * 1024 * 1024),
			&gxm_color_surfaces_uid[i]);

		memset(gxm_color_surfaces_addr[i], 0, DISPLAY_STRIDE * DISPLAY_HEIGHT);

		sceGxmColorSurfaceInit(&gxm_color_surfaces[i],
			DISPLAY_COLOR_FORMAT,
			SCE_GXM_COLOR_SURFACE_LINEAR,
			SCE_GXM_COLOR_SURFACE_SCALE_NONE,
			SCE_GXM_OUTPUT_REGISTER_SIZE_32BIT,
			DISPLAY_WIDTH,
			DISPLAY_HEIGHT,
			DISPLAY_STRIDE,
			gxm_color_surfaces_addr[i]);

		sceGxmSyncObjectCreate(&gxm_sync_objects[i]);
	}


    unsigned int depth_stencil_width = ALIGN(DISPLAY_WIDTH, SCE_GXM_TILE_SIZEX);
	unsigned int depth_stencil_height = ALIGN(DISPLAY_HEIGHT, SCE_GXM_TILE_SIZEY);
	unsigned int depth_stencil_samples = depth_stencil_width * depth_stencil_height;

	gxm_depth_stencil_surface_addr = gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW, (SceGxmMemoryAttribFlags) (SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE), 4 * depth_stencil_samples, &gxm_depth_stencil_surface_uid);

	sceGxmDepthStencilSurfaceInit(&gxm_depth_stencil_surface, SCE_GXM_DEPTH_STENCIL_FORMAT_S8D24, SCE_GXM_DEPTH_STENCIL_SURFACE_TILED, depth_stencil_width, gxm_depth_stencil_surface_addr, NULL);

	static const unsigned int shader_patcher_buffer_size = 64 * 1024;
	static const unsigned int shader_patcher_vertex_usse_size = 64 * 1024;
	static const unsigned int shader_patcher_fragment_usse_size = 64 * 1024;

    // WHAT? SCE_GXM_MEMORY_ATTRIB_READ?
	gxm_shader_patcher_buffer_addr = gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,(SceGxmMemoryAttribFlags) (SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_READ), shader_patcher_buffer_size, &gxm_shader_patcher_buffer_uid);

	unsigned int shader_patcher_vertex_usse_offset;
	gxm_shader_patcher_vertex_usse_addr = gpu_vertex_usse_alloc_map(
		shader_patcher_vertex_usse_size, &gxm_shader_patcher_vertex_usse_uid,
		&shader_patcher_vertex_usse_offset);

	unsigned int shader_patcher_fragment_usse_offset;
	gxm_shader_patcher_fragment_usse_addr = gpu_fragment_usse_alloc_map(
		shader_patcher_fragment_usse_size, &gxm_shader_patcher_fragment_usse_uid,
		&shader_patcher_fragment_usse_offset);

	SceGxmShaderPatcherParams shader_patcher_params;
	memset(&shader_patcher_params, 0, sizeof(shader_patcher_params));
	shader_patcher_params.userData = NULL;
	shader_patcher_params.hostAllocCallback = shader_patcher_host_alloc_cb;
	shader_patcher_params.hostFreeCallback = shader_patcher_host_free_cb;
	shader_patcher_params.bufferAllocCallback = NULL;
	shader_patcher_params.bufferFreeCallback = NULL;
	shader_patcher_params.bufferMem = gxm_shader_patcher_buffer_addr;
	shader_patcher_params.bufferMemSize = shader_patcher_buffer_size;
	shader_patcher_params.vertexUsseAllocCallback = NULL;
	shader_patcher_params.vertexUsseFreeCallback = NULL;
	shader_patcher_params.vertexUsseMem = gxm_shader_patcher_vertex_usse_addr;
	shader_patcher_params.vertexUsseMemSize = shader_patcher_vertex_usse_size;
	shader_patcher_params.vertexUsseOffset = shader_patcher_vertex_usse_offset;
	shader_patcher_params.fragmentUsseAllocCallback = NULL;
	shader_patcher_params.fragmentUsseFreeCallback = NULL;
	shader_patcher_params.fragmentUsseMem = gxm_shader_patcher_fragment_usse_addr;
	shader_patcher_params.fragmentUsseMemSize = shader_patcher_fragment_usse_size;
	shader_patcher_params.fragmentUsseOffset = shader_patcher_fragment_usse_offset;

	sceGxmShaderPatcherCreate(&shader_patcher_params, &gxm_shader_patcher);
    // https://github.com/xerpi/gxmfun/blob/c0e3b8259880286d8ca507123b4b9d635f33d7b4/source/main.c#L333
    

	// Let's create the shaders (https://github.com/xerpi/gxmfun/blob/c0e3b8259880286d8ca507123b4b9d635f33d7b4/source/main.c#L382)
	sceGxmShaderPatcherRegisterProgram(gxm_shader_patcher, gxm_program_clear_v, &gxm_clear_vertex_program_id);
	sceGxmShaderPatcherRegisterProgram(gxm_shader_patcher, gxm_program_clear_f, &gxm_clear_fragment_program_id);

	const SceGxmProgram *clear_vertex_program = sceGxmShaderPatcherGetProgramFromId(gxm_clear_vertex_program_id);
	const SceGxmProgram *clear_fragment_program = sceGxmShaderPatcherGetProgramFromId(gxm_clear_fragment_program_id);

	gxm_clear_vertex_program_position_param = sceGxmProgramFindParameterByName(clear_vertex_program, "position");
	gxm_clear_fragment_program_u_clear_color_param = sceGxmProgramFindParameterByName(clear_fragment_program, "u_clear_color");

	SceGxmVertexAttribute clear_vertex_attribute;
	SceGxmVertexStream clear_vertex_stream;
	clear_vertex_attribute.streamIndex = 0;
	clear_vertex_attribute.offset = 0;
	clear_vertex_attribute.format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
	clear_vertex_attribute.componentCount = 2;
	clear_vertex_attribute.regIndex = sceGxmProgramParameterGetResourceIndex(
	gxm_clear_vertex_program_position_param);
	clear_vertex_stream.stride = sizeof(struct clear_vertex);
	clear_vertex_stream.indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;

	sceGxmShaderPatcherCreateVertexProgram(gxm_shader_patcher,
		gxm_clear_vertex_program_id, &clear_vertex_attribute,
		1, &clear_vertex_stream, 1, &gxm_clear_vertex_program_patched);

	sceGxmShaderPatcherCreateFragmentProgram(gxm_shader_patcher,
		gxm_clear_fragment_program_id, SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4,
		SCE_GXM_MULTISAMPLE_NONE, NULL, clear_vertex_program,
		&gxm_clear_fragment_program_patched);

	SceUID clear_vertices_uid;
	struct clear_vertex *const clear_vertices_data = (struct clear_vertex*) gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, SCE_GXM_MEMORY_ATTRIB_READ, 4 * sizeof(struct clear_vertex), &clear_vertices_uid);

	SceUID clear_indices_uid;
	unsigned short *const clear_indices_data = (unsigned short*) gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, SCE_GXM_MEMORY_ATTRIB_READ, 4 * sizeof(unsigned short), &clear_indices_uid);

	clear_vertices_data[0].position = (vector2f){-1.0f, -1.0f};
	clear_vertices_data[1].position = (vector2f){ 1.0f, -1.0f};
	clear_vertices_data[2].position = (vector2f){-1.0f,  1.0f};
	clear_vertices_data[3].position = (vector2f){ 1.0f,  1.0f};

	clear_indices_data[0] = 0;
	clear_indices_data[1] = 1;
	clear_indices_data[2] = 2;
	clear_indices_data[3] = 3;

	// Okay, now the other shader
	sceGxmShaderPatcherRegisterProgram(gxm_shader_patcher, gxm_program_color_v, &gxm_color_buffer_vertex_program_id);
	sceGxmShaderPatcherRegisterProgram(gxm_shader_patcher, gxm_program_color_f, &gxm_color_buffer_fragment_program_id);

	const SceGxmProgram *color_buffer_vertex_program = sceGxmShaderPatcherGetProgramFromId(gxm_color_buffer_vertex_program_id);

	gxm_color_buffer_vertex_program_position_param = sceGxmProgramFindParameterByName(color_buffer_vertex_program, "position");
	gxm_color_buffer_vertex_program_color_param    = sceGxmProgramFindParameterByName(color_buffer_vertex_program, "color");

	gxm_color_buffer_vertex_program_u_mvp_matrix_param = sceGxmProgramFindParameterByName(color_buffer_vertex_program, "u_mvp_matrix");

	SceGxmVertexAttribute color_buffer_attributes[2];
	SceGxmVertexStream color_buffer_stream;

	// Vertex attrib
	color_buffer_attributes[0].streamIndex = 0;
	color_buffer_attributes[0].offset = 0;
	color_buffer_attributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
	color_buffer_attributes[0].componentCount = 3;
	color_buffer_attributes[0].regIndex = sceGxmProgramParameterGetResourceIndex(gxm_color_buffer_vertex_program_position_param);
	
	// Color attrib
	color_buffer_attributes[1].streamIndex = 0;
	color_buffer_attributes[1].offset      = sizeof(vector3f);
	color_buffer_attributes[1].format      = SCE_GXM_ATTRIBUTE_FORMAT_F32;
	color_buffer_attributes[1].componentCount = 4;
	color_buffer_attributes[1].regIndex    = sceGxmProgramParameterGetResourceIndex(gxm_color_buffer_vertex_program_color_param);


	color_buffer_stream.stride = sizeof(struct color_vertex);
	color_buffer_stream.indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;

	sceGxmShaderPatcherCreateVertexProgram(gxm_shader_patcher, gxm_color_buffer_vertex_program_id, color_buffer_attributes, 2, &color_buffer_stream, 1, &gxm_color_buffer_vertex_program_patched);

	// SceGxmBlendInfo color_buffer_blend_info;
	// memset(&color_buffer_blend_info, 0, sizeof(color_buffer_blend_info));
	// color_buffer_blend_info.colorMask = SCE_GXM_COLOR_MASK_NONE;
	// color_buffer_blend_info.colorFunc = SCE_GXM_BLEND_FUNC_NONE;
	// color_buffer_blend_info.alphaFunc = SCE_GXM_BLEND_FUNC_NONE;
	// color_buffer_blend_info.colorSrc = SCE_GXM_BLEND_FACTOR_ZERO;
	// color_buffer_blend_info.colorDst = SCE_GXM_BLEND_FACTOR_ZERO;
	// color_buffer_blend_info.alphaSrc = SCE_GXM_BLEND_FACTOR_ZERO;
	// color_buffer_blend_info.alphaDst = SCE_GXM_BLEND_FACTOR_ZERO;

	sceGxmShaderPatcherCreateFragmentProgram(gxm_shader_patcher, gxm_color_buffer_fragment_program_id, SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4, SCE_GXM_MULTISAMPLE_NONE, NULL, color_buffer_vertex_program, &gxm_color_buffer_fragment_program_patched);

	// Create the triangle
	float mvpMatrix_identity[] = {
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

	SceUID color_vertices_uid;
	struct color_vertex *const color_vertices_data = (color_vertex*) gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, SCE_GXM_MEMORY_ATTRIB_READ, 3 * sizeof(struct color_vertex), &color_vertices_uid);

	SceUID color_indices_uid;
	unsigned short *const color_indices_data = (unsigned short*) gpu_alloc_map(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, SCE_GXM_MEMORY_ATTRIB_READ, 3 * sizeof(unsigned short), &color_indices_uid);

	color_vertices_data[0].position = (vector3f) {0.f, .8f, 0.f};
	color_vertices_data[1].position = (vector3f) {-.8, -.8f, 0.f};
	color_vertices_data[2].position = (vector3f) {.8f, -.8f, 0.f};

	color_vertices_data[0].color    = (vector4f) {1.f, 0.f, 0.f, 1.f};
	color_vertices_data[1].color    = (vector4f) {0.f, 1.f, 0.f, 1.f};
	color_vertices_data[2].color    = (vector4f) {0.f, 0.f, 1.f, 1.f};

	color_indices_data[0] = 0;
	color_indices_data[1] = 1;
	color_indices_data[2] = 2;



	SceCtrlData pad;
	memset(&pad, 0, sizeof(pad));

	float cr = 0.f;
	float cg = 0.f;

	bool running = true;
	while(running){
		sceCtrlPeekBufferPositive(0, &pad, 1);
		if(pad.buttons & SCE_CTRL_START) running = false;

		cr = (float) pad.lx / 127.f;
		cg = (float) pad.ly / 127.f;

		if(cr < 0.f) cr = 0.f;
		if(cg < 0.f) cg = 0.f;

		
		sceGxmBeginScene(gxm_context, 0, gxm_render_target, NULL, NULL, gxm_sync_objects[gxm_back_buffer_index], &gxm_color_surfaces[gxm_back_buffer_index], &gxm_depth_stencil_surface);

		// Clear screen
		sceGxmSetVertexProgram(gxm_context, gxm_clear_vertex_program_patched);
		sceGxmSetFragmentProgram(gxm_context, gxm_clear_fragment_program_patched);

		static const float clear_color[4] = {cr, cg, 0.f, 1.0f};

		set_fragment_default_uniform_data(gxm_clear_fragment_program_u_clear_color_param, sizeof(clear_color) / sizeof(float), clear_color);

		// sceGxmSetFrontStencilFunc(gxm_context, SCE_GXM_STENCIL_FUNC_ALWAYS, SCE_GXM_STENCIL_OP_ZERO, SCE_GXM_STENCIL_OP_ZERO, SCE_GXM_STENCIL_OP_ZERO, 0, 0xFF);

		sceGxmSetVertexStream(gxm_context, 0, clear_vertices_data);
		sceGxmDraw(gxm_context, SCE_GXM_PRIMITIVE_TRIANGLE_STRIP, SCE_GXM_INDEX_FORMAT_U16, clear_indices_data, 4);

		// Draw the triangle, yey!
		// It does not draw the triangle...


		sceGxmSetVertexProgram(gxm_context, gxm_color_buffer_vertex_program_patched);
		sceGxmSetFragmentProgram(gxm_context, gxm_color_buffer_fragment_program_patched);

		set_vertex_default_uniform_data(gxm_color_buffer_vertex_program_u_mvp_matrix_param, sizeof(mvpMatrix_identity) / sizeof(float), mvpMatrix_identity);
		
		sceGxmSetVertexStream(gxm_context, 0, color_vertices_data);
		sceGxmDraw(gxm_context, SCE_GXM_PRIMITIVE_TRIANGLES, SCE_GXM_INDEX_FORMAT_U16, color_indices_data, 3);


		// End of scene?
		sceGxmEndScene(gxm_context, NULL, NULL);
		sceGxmPadHeartbeat(&gxm_color_surfaces[gxm_back_buffer_index], gxm_sync_objects[gxm_back_buffer_index]);


		struct display_queue_callback_data queue_cb_data;
		queue_cb_data.addr = gxm_color_surfaces_addr[gxm_back_buffer_index];

		sceGxmDisplayQueueAddEntry(gxm_sync_objects[gxm_front_buffer_index], gxm_sync_objects[gxm_back_buffer_index], &queue_cb_data);

		// Swap chain
		gxm_front_buffer_index = gxm_back_buffer_index;
		gxm_back_buffer_index = (gxm_back_buffer_index + 1) % DISPLAY_BUFFER_COUNT;
	}

	sceGxmDisplayQueueFinish();
	sceGxmFinish(gxm_context);


	gpu_unmap_free(clear_vertices_uid);
	gpu_unmap_free(clear_indices_uid);

	gpu_unmap_free(color_vertices_uid);
	gpu_unmap_free(color_indices_uid);



	sceGxmShaderPatcherReleaseVertexProgram(gxm_shader_patcher,  gxm_color_buffer_vertex_program_patched);
	sceGxmShaderPatcherReleaseFragmentProgram(gxm_shader_patcher, gxm_color_buffer_fragment_program_patched);

	sceGxmShaderPatcherReleaseVertexProgram(gxm_shader_patcher, gxm_clear_vertex_program_patched);
	sceGxmShaderPatcherReleaseFragmentProgram(gxm_shader_patcher, gxm_clear_fragment_program_patched);


	sceGxmShaderPatcherUnregisterProgram(gxm_shader_patcher, gxm_color_buffer_vertex_program_id);
	sceGxmShaderPatcherUnregisterProgram(gxm_shader_patcher, gxm_color_buffer_fragment_program_id);

	sceGxmShaderPatcherUnregisterProgram(gxm_shader_patcher, gxm_clear_vertex_program_id);
	sceGxmShaderPatcherUnregisterProgram(gxm_shader_patcher, gxm_clear_fragment_program_id);

	

    sceGxmShaderPatcherDestroy(gxm_shader_patcher);
    sceGxmDestroyRenderTarget(gxm_render_target);
    

    gpu_unmap_free(vdm_ring_buffer_uid);
    gpu_unmap_free(vertex_ring_buffer_uid);
    gpu_unmap_free(fragment_ring_buffer_uid);

    sceGxmDestroyContext(gxm_context);
    sceGxmTerminate();

    sceKernelExitProcess(0);
    return 0;
}

void *gpu_alloc_map(SceKernelMemBlockType type, SceGxmMemoryAttribFlags gpu_attrib, size_t size, SceUID *uid){
	SceUID memuid;
	void *addr;

	if (type == SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW)
		size = ALIGN(size, 256 * 1024);
	else
		size = ALIGN(size, 4 * 1024);

	memuid = sceKernelAllocMemBlock("gpumem", type, size, NULL);
	if (memuid < 0)
		return NULL;

	if (sceKernelGetMemBlockBase(memuid, &addr) < 0)
		return NULL;

	if (sceGxmMapMemory(addr, size, gpu_attrib) < 0) {
		sceKernelFreeMemBlock(memuid);
		return NULL;
	}

	if (uid)
		*uid = memuid;

	return addr;
}

void gpu_unmap_free(SceUID uid){
	void *addr;

	if (sceKernelGetMemBlockBase(uid, &addr) < 0)
		return;

	sceGxmUnmapMemory(addr);

	sceKernelFreeMemBlock(uid);
}


void *gpu_vertex_usse_alloc_map(size_t size, SceUID *uid, unsigned int *usse_offset){
	SceUID memuid;
	void *addr;

	size = ALIGN(size, 4 * 1024);

	memuid = sceKernelAllocMemBlock("gpu_vertex_usse",
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, size, NULL);
	if (memuid < 0)
		return NULL;

	if (sceKernelGetMemBlockBase(memuid, &addr) < 0)
		return NULL;

	if (sceGxmMapVertexUsseMemory(addr, size, usse_offset) < 0)
		return NULL;

	if (uid)
		*uid = memuid;

	return addr;
}

void gpu_vertex_usse_unmap_free(SceUID uid){
	void *addr;

	if (sceKernelGetMemBlockBase(uid, &addr) < 0)
		return;

	sceGxmUnmapVertexUsseMemory(addr);

	sceKernelFreeMemBlock(uid);
}

void *gpu_fragment_usse_alloc_map(size_t size, SceUID *uid, unsigned int *usse_offset){
	SceUID memuid;
	void *addr;

	size = ALIGN(size, 4 * 1024);

	memuid = sceKernelAllocMemBlock("gpu_fragment_usse",
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, size, NULL);
	if (memuid < 0)
		return NULL;

	if (sceKernelGetMemBlockBase(memuid, &addr) < 0)
		return NULL;

	if (sceGxmMapFragmentUsseMemory(addr, size, usse_offset) < 0)
		return NULL;

	if (uid)
		*uid = memuid;

	return addr;
}

void gpu_fragment_usse_unmap_free(SceUID uid){
	void *addr;

	if (sceKernelGetMemBlockBase(uid, &addr) < 0)
		return;

	sceGxmUnmapFragmentUsseMemory(addr);

	sceKernelFreeMemBlock(uid);
}

void *shader_patcher_host_alloc_cb(void *user_data, unsigned int size){
	return malloc(size);
}

void shader_patcher_host_free_cb(void *user_data, void *mem){
	return free(mem);
}
