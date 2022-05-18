#ifdef GL_ES
precision mediump float;
#endif

varying vec4 vVertexColor;
varying vec2 vVertexTexCrd;

uniform sampler2D uTextureUnit;

void main(){
    gl_FragColor = vVertexColor * texture2D(uTextureUnit, vVertexTexCrd);
}

