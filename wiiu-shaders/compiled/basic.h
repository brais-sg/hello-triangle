//--------------------------------------------------

// This file is automatically generated by gfd.

//--------------------------------------------------

// ---------- basic Vertex Shader ----------

ALIGNVAR(256) static const u32 basic_VS_shaderPtr[100] =
{
    0x00000000,0x00008009,0x20000000,0x00003ca0,    // 0x0000
    0x3c200100,0x88060094,0x00c00000,0x88062014,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x032d1f00,0x80000000,0x03291f00,0x80000020,    // 0x0040
    0x03251f00,0x80000040,0x03211f80,0x80000060,
    0x0248a001,0xfe00620f,0x02482001,0xfe04622f,
    0x0248a000,0xfe08624f,0x02482080,0xfe0c626f,
    0x0224a001,0xfe00620f,0x02242001,0xfe04622f,
    0x0224a000,0xfe08624f,0x02242080,0xfe0c626f,
    0x02002000,0xfe0c4200,0x0200a000,0xfe084220,
    0x02002001,0xfe044240,0x0200a081,0xfe004260,
    0x3b408727,0xf4ee25da,0xd6d8ee08,0x3d32cd76 
    
};

GX2UniformVar basic_VS_uniforms[] = {
    {"uTransformationMatrix", GX2_VAR_TYPE_MAT4, 1, 0, 
     GX2_UNIFORM_BLOCK_INDEX_INVALID, } 
};

GX2AttribVar basic_VS_attrib_vars[] = {
    {"aVertexColor", GX2_VAR_TYPE_VEC4, 0, 0},
    {"aVertexPosition", GX2_VAR_TYPE_VEC3, 0, 1} 
};



static GX2VertexShader basic_VS = {
  { // _regs
    0x00000103,0x00000000,0x00000000,0x00000001,    // 0x0000
    0xffffff00,0xffffffff,0xffffffff,0xffffffff,
    0xffffffff,0xffffffff,0xffffffff,0xffffffff,
    0xffffffff,0xffffffff,0x00000000,0xfffffffc,
    0x00000002,0x00000000,0x00000001,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x00000000,0x0000000e,0x00000010 
    
  },
  400,
  (void *) basic_VS_shaderPtr,
  GX2_SHADER_MODE_UNIFORM_REGISTER,
  0,
  NULL,
  1,
  basic_VS_uniforms,
  0,
  NULL,
  0,
  NULL,
  0,
  NULL,
  2,
  basic_VS_attrib_vars,
  0,
  (GX2Boolean)0,
  { 
    0x00000000,0x00000000,0x00000000,0x00000000     // 0x0000
    
  },
};


// ---------- basic Pixel Shader ----------

ALIGNVAR(256) static const u32 basic_PS_shaderPtr[6] =
{
    0x00000000,0x88062094,0xaf9dd374,0xf0f908d3,    // 0x0000
    0x19e20f9d,0x84d694bb 
};



static GX2PixelShader basic_PS = {
  { // _regs
    0x00000001,0x00000002,0x14000001,0x00000000,    // 0x0000
    0x00000001,0x00000100,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x0000000f,0x00000001,0x00000010,
    0x00000000 
  },
  24,
  (void *) basic_PS_shaderPtr,
  GX2_SHADER_MODE_UNIFORM_REGISTER,
  0,
  NULL,
  0,
  NULL,
  0,
  NULL,
  0,
  NULL,
  0,
  NULL,
};


