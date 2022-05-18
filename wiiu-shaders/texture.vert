attribute vec3 aVertexPosition;
attribute vec4 aVertexColor;
attribute vec2 aVertexTexCrd;

uniform mat4 uTransformationMatrix;

varying vec4 vVertexColor;
varying vec2 vVertexTexCrd;


void main(){
    vVertexColor  = aVertexColor;
    vVertexTexCrd = aVertexTexCrd;
    
    gl_Position = uTransformationMatrix * vec4(aVertexPosition, 1.0);
}