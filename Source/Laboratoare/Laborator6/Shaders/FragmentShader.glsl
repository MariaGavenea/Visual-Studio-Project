#version 330

// TODO: get values from fragment shader
in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texture_coord;
in vec3 frag_color;
in float frag_time;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO: write pixel out color

	 // out_color = vec4(frag_color, 1); // ex 4, ex 6
	 // out_color = vec4(frag_normal, 1); // ex 5 - am observat ca fiecare colt al cubului este colorat
										// cu culoarea indicata de vectorul normalei corespunzatoare punctului respectiv

	 out_color = abs(vec4(cos(frag_time), sin(frag_time), cos(frag_time), 1) - vec4(frag_color, 0));  // bonus

}