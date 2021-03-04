#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal; // de comentat pt ex 6
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color; // de comentat pt ex 6

//layout(location = 3) in vec3 v_normal; // ex 6
//layout(location = 1) in vec3 v_color; // ex 6 am analizat ce se intampla pornind de la ex 4 (nu de la ex 5) si
									 // am observat ca rezultatul vizual este acelasi cu cel de la ex 5
									 // daca porneam de la ex 5, se anulau cele doua schimbari si se vedea obiectul normal

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

// TODO: output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture_coord;
out vec3 frag_color;
out float frag_time;

void main()
{
	// TODO: send output to fragment shader
	frag_position = v_position;
	frag_normal = v_normal;
	frag_texture_coord = v_texture_coord;
	frag_color = v_color;
	frag_time = time;

	// TODO: compute gl_Position
	// gl_Position = Projection * View * Model * vec4(v_position, 1.0);
	gl_Position = Projection * View * (Model * vec4(v_position, 1.0) + vec4(0, 0, sin(time), 0)); // bonus
}
