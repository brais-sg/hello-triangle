attribute vec3 aVertexPosition;

uniform mat4 uTransformationMatrix;


void main(){
    gl_Position = uTransformationMatrix * vec4(aVertexPosition, 1.0);
}