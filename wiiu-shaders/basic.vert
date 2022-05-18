attribute vec3 aVertexPosition;
attribute vec4 aVertexColor;

uniform mat4 uTransformationMatrix;

varying vec4 vVertexColor;

void main(){
    vVertexColor = aVertexColor;
    gl_Position = uTransformationMatrix * vec4(aVertexPosition, 1.0);
}