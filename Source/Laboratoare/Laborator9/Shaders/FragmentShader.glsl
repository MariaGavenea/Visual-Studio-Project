#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform int two_textures;
uniform float time;
uniform int is_globe;

in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO : calculate the out_color using the texture2D() function
	vec2 auxTexcoord = texcoord;
	
	if (is_globe == 1) {
		auxTexcoord.x -= 0.02 * time;
	}

	vec4 color1 = texture2D(texture_1, auxTexcoord);
	vec4 color = color1;

	if (two_textures == 1) {
		vec4 color2 = texture2D(texture_2, auxTexcoord);
		color = mix(color1, color2, 0.5f);
	}

	if (color.a < 0.5f) {
		discard;
	}

	out_color = color;
}