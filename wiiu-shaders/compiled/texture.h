//--------------------------------------------------

// This file is automatically generated by gfd.

//--------------------------------------------------

// ---------- texture Vertex Shader ----------

ALIGNVAR(256) static const u32 texture_VS_shaderPtr[104] =
{
    0x00000000,0x00008009,0x20000000,0x000044a0,    // 0x0000
    0x3c200100,0x88060094,0x00c00000,0x88068013,
    0x01400000,0x88042014,0x00000000,0x00000000,
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
    0x03251f00,0x80000000,0x03211f00,0x80000020,    // 0x0040
    0x032d1f00,0x80000040,0x03291f00,0x80000060,
    0x03001f80,0x900c0000,0x0248a000,0xfe00620f,
    0x02482000,0xfe04622f,0x0248a001,0xfe08624f,
    0x02482001,0xfe0c626f,0x03041f80,0x900c0020,
    0x0224a000,0xfe00620f,0x02242000,0xfe04622f,
    0x0224a001,0xfe08624f,0x02242081,0xfe0c626f,
    0x02002000,0xfe044200,0x0200a000,0xfe004220,
    0x02002001,0xfe0c4240,0x0200a081,0xfe084260,
    0x63a44806,0x7324080a,0xaf889955,0xdd40b95e 
    
};

GX2UniformVar texture_VS_uniforms[] = {
    {"uTransformationMatrix", GX2_VAR_TYPE_MAT4, 1, 0, 
     GX2_UNIFORM_BLOCK_INDEX_INVALID, } 
};

GX2AttribVar texture_VS_attrib_vars[] = {
    {"aVertexColor", GX2_VAR_TYPE_VEC4, 0, 0},
    {"aVertexPosition", GX2_VAR_TYPE_VEC3, 0, 1},
    {"aVertexTexCrd", GX2_VAR_TYPE_VEC2, 0, 2} 
};



static GX2VertexShader texture_VS = {
  { // _regs
    0x00000104,0x00000000,0x00000002,0x00000001,    // 0x0000
    0xffff0100,0xffffffff,0xffffffff,0xffffffff,
    0xffffffff,0xffffffff,0xffffffff,0xffffffff,
    0xffffffff,0xffffffff,0x00000000,0xfffffff8,
    0x00000003,0x00000000,0x00000001,0x00000002,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x000000ff,0x000000ff,0x000000ff,
    0x000000ff,0x00000000,0x0000000e,0x00000010 
    
  },
  416,
  (void *) texture_VS_shaderPtr,
  GX2_SHADER_MODE_UNIFORM_REGISTER,
  0,
  NULL,
  1,
  texture_VS_uniforms,
  0,
  NULL,
  0,
  NULL,
  0,
  NULL,
  3,
  texture_VS_attrib_vars,
  0,
  (GX2Boolean)0,
  { 
    0x00000000,0x00000000,0x00000000,0x00000000     // 0x0000
    
  },
};


// ---------- texture Pixel Shader ----------

ALIGNVAR(256) static const u32 texture_PS_shaderPtr[104] =
{
    0x30000000,0x0000c080,0x20000000,0x00000ca0,    // 0x0000
    0x00000000,0x88062094,0x00000000,0x00000000,
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
    0x00200000,0x90000000,0x00248000,0x90000020,    // 0x0040
    0x00280001,0x90000040,0x002c8081,0x90000060,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,
    0x10000100,0x01100df0,0x00008010,0xecdfea0d,
    0x68af594b,0xfa51db5e,0xb7c4c685,0x0409a8c2 
    
};

GX2SamplerVar texture_PS_sampler_vars[] = {
    {"uTextureUnit", GX2_SAMPLER_TYPE_2D, 0} 
};



static GX2PixelShader texture_PS = {
  { // _regs
    0x00000002,0x00000002,0x14000002,0x00000000,    // 0x0000
    0x00000002,0x00000100,0x00000101,0x00000000,
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
  416,
  (void *) texture_PS_shaderPtr,
  GX2_SHADER_MODE_UNIFORM_REGISTER,
  0,
  NULL,
  0,
  NULL,
  0,
  NULL,
  0,
  NULL,
  1,
  texture_PS_sampler_vars,
};

