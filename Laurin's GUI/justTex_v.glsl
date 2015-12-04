//justTex_v.glsl
//Laurin Fisher

//vertex shader, gives info to frag shader

varying vec3 surfacept; 
varying vec3 surfnorm; 
varying vec2 texcoord; 

void main()
{
	//projected vertex position
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; 

	//send color to fragment shader
	gl_FrontColor = gl_Color; 

	//get the surface point, turn it into a vec3 for use
	vec4 surfacept4= gl_ModelViewMatrix * gl_Vertex; 
	surfacept= surfacept4.xyz/surfacept4.w; 

	//surface normal 
	surfnorm = normalize(gl_NormalMatrix * gl_Normal); 
	
	//get texture coordinates
	vec4 texcoord4 = gl_TextureMatrix[0] * gl_MultiTexCoord0; 
	texcoord = texcoord4.xy;
}