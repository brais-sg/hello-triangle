#ifdef GL_ES
precision mediump float;
#endif

varying vec4 vVertexColor;

void main(){
    gl_FragColor = vVertexColor;
}