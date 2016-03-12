#version 150

uniform vec2			iResolution;
uniform float			iGlobalTime;
uniform vec4			iMouse;
uniform sampler2DRect	iChannel0;
uniform	sampler2DRect	imageMask;

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

float saturate(float num)
{
    return clamp(num,0.0,1.0);
}

//----------------------------------------------------------------------------------------
float Noise(in vec2 p, in float scale, in vec2 vel )
{
	vec2 f;
	
	p *= scale;

	// Movement, just for tiling demonstation...
	//p += 0.2*vec2(sin(iGlobalTime * .3), cos(iGlobalTime * .33+scale*.23))*(scale*.03) + iMouse.xy*.1;

	p += vel;
	
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
	float scale = 5.0;
	float amp   = 0.33;
	vec2 vel[5];
	vel[0] = iGlobalTime * vec2(.0001733,0.0001);
	vel[1] = iGlobalTime * vec2(.000733,0.00023);
	vel[2] = iGlobalTime * vec2(.0000453,-0.00028);
	vel[3] = iGlobalTime * vec2(-.000233,+0.00073);
	vel[4] = iGlobalTime * vec2(.001733,-0.00023);
	
	for (int i = 0; i < 5; i++)
	{
		f += Noise(p, scale, vel[i]) * amp;
		amp *= .4;
		// Scale must be multiplied by an integer value...
		scale *= 3.0;
	}
	// Clamp it just in case....
	return min(f, 1.0);
}

void main (void)
{
	float cover = 1.8*1.1+0.1;
	float bright = 1.8 - cover;
	vec2 uv = gl_FragCoord.xy / iResolution.xy;
	bright = saturate(bright);
	bright -=1;
	#ifdef SHOW_TILING
	//uv *= TILES;
	#endif
	
	// Do the noise cloud (fractal Brownian motion)
	float bri = fBm(uv);
	
	bri = pow(bri, 1.15); // ...cranked up the contrast for demo.
	bri = saturate(bri);
	float cloudCol = saturate(saturate(1.0-pow(bri,1.0)*0.2)*bright);
	vec4 fragCol = vec4(cloudCol,cloudCol,cloudCol,1);
	//fragCol*=1.1;
	//fragCol = 0.8*fragCol + 0.3*vec4(0.2,0.2,0.2,1);
	//bri = fragCol.r;
	//bri = pow(bri, 1.15);
	
	fragCol = mix(fragCol,texture(iChannel0, texCoordVarying), 1.1*bri );
	outputColor = fragCol;
}