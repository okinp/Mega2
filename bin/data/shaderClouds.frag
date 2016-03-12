#version 150

uniform vec2			iResolution;
uniform float			iGlobalTime;
uniform vec4			iMouse;
uniform sampler2DRect	iChannel0;
uniform sampler2DRect	texBack;
uniform sampler2DRect	imageMask;

out vec4 outputColor;


in vec2 texCoordVarying;
// https://www.shadertoy.com/view/4dlGW2

// Tileable noise, for creating useful textures. By David Hoskins, Sept. 2013.
// It can be extrapolated to other types of randomised texture.

#define SHOW_TILING
#define TILES 2.0 // Use 1.0 for normal tiling across whole texture.

//----------------------------------------------------------------------------------------
float Hash(in vec2 p, in float scale)
{
	// This is tiling part, adjusts with the scale...
	p = mod(p, scale);
	return fract(sin(dot(p, vec2(27.16898, 38.90563))) * 5151.5473453);
}

//----------------------------------------------------------------------------------------
float Noise(in vec2 p, in float scale )
{
	vec2 f;
	
	p *= scale;

	// Movement, just for tiling demonstation...
	//p += 0.2*vec2(sin(iGlobalTime * .3), cos(iGlobalTime * .33+scale*.23))*(scale*.03) + iMouse.xy*.1;

	p += iGlobalTime * vec2(.001733,0);
	
	f = fract(p);		// Separate integer from fractional
    p = floor(p);
	
    f = f*f*(3.0-2.0*f);	// Cosine interpolation approximation
	
    float res = mix(mix(Hash(p, 				 scale),
						Hash(p + vec2(1.0, 0.0), scale), f.x),
					mix(Hash(p + vec2(0.0, 1.0), scale),
						Hash(p + vec2(1.0, 1.0), scale), f.x), f.y);
    return res;
}

//----------------------------------------------------------------------------------------
float fBm(in vec2 p)
{
	float f = 0.0;
	// Change starting scale to any integer value...
	float scale = 13.0;
	float amp   = 0.33;
	
	for (int i = 0; i < 5; i++)
	{
		f += Noise(p, scale) * amp;
		amp *= .65;
		// Scale must be multiplied by an integer value...
		scale *= 2.0;
	}
	// Clamp it just in case....
	return min(f, 1.0);
}
void main (void)
{
	vec2 uv = gl_FragCoord.xy / iResolution.xy;

	#ifdef SHOW_TILING
	//uv *= TILES;
	#endif
	
	// Do the noise cloud (fractal Brownian motion)
	float bri = fBm(uv);
	
	bri = pow(bri, 1.15); // ...cranked up the contrast for demo.
	vec4 fragCol;
	if ( bri > 0.325 )
	{
		fragCol = texture(iChannel0, texCoordVarying);
	} else {
		vec3 col = vec3(bri);
		fragCol = 3.5*vec4(col,1);
		clamp(fragCol, 0, 1);
		

	}

	fragColor = texture(texBack, texCoordVarying );
	outputColor = fragCol;
}