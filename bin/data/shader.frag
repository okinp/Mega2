#version 150

uniform sampler2DRect texBack;
uniform sampler2DRect imageMask;

in vec2 texCoordVarying;

out vec4 outputColor;
 
void main()
{
	vec4 texelBack = texture(texBack, texCoordVarying);
	vec4 texelMask = texture(imageMask, texCoordVarying);
	//if( texelMask.r == 0 )
	//{
	//	outputColor = texelBack;
	//} else {
	//	outputColor = 0.7*texelMask + 0.3*texelBack;
	//}
	outputColor = texelMask;
}