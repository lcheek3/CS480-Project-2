//justTex_f.glsl
//Laurin Fisher

varying vec3 surfacept; 
varying vec3 surfnorm; 
uniform sampler2D tex0; //2d texture
varying vec2 texcoord; 


void main()
{
	
	//surface normal
	vec3 surfnormfin= normalize(surfnorm); 
	if(!gl_FrontFacing)
		surfnormfin=-surfnormfin;

	//texture, and direction	
	vec4 texcolor=texture2D(tex0, texcoord); 
	
	//color from texture
	gl_FragColor=texcolor;  
}