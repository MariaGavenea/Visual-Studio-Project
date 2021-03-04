#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec2 texcoord;
out vec3 position;

out vec3 world_position;
out vec3 world_normal;

void main()
{
	// TODO : pass v_texture_coord as output to Fragment Shader
	texcoord = v_texture_coord;
	position = v_position;

	world_position = vec3(Model * vec4(v_position, 1));
	world_normal = normalize(vec3(Model * vec4(v_normal, 0)));

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
