#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform int material_shininess;
uniform vec3 Ke, Ka, Kd, Ks;
uniform vec3 Isursa, Ia;

uniform int is_spotlight;
uniform float cut_off_angle;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO: define ambient light component
	vec3 ambient_light = Ia * Ka;

	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);

	// TODO: compute diffuse light component
	float product = max(dot(world_normal, L), 0);
	vec3 diffuse_light = Kd * product;

	// TODO: compute specular light component
	vec3 specular_light = vec3(0, 0, 0);

	if (product > 0)
	{
		vec3 R = normalize(reflect(-L, world_normal));
		//vec3 H = normalize( L + V );

		specular_light = Ks * pow(max(dot(V, R), 0), material_shininess);
		//specular_light = material_ks * pow(max(dot(V, H), 0), material_shininess);
	}

	if (is_spotlight == 1) {
		float cut_off = radians(cut_off_angle);
		float spot_light = dot(-L, light_direction);
		float spot_light_limit = cos(cut_off);
		float light_att_factor = 0;

		if (spot_light > spot_light_limit)
		{
			// Quadratic attenuation
			float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
			light_att_factor = pow(linear_att, 2);
		}

		vec3 color = Ke + ambient_light + light_att_factor * Isursa * (diffuse_light + specular_light);

		// TODO: write pixel out color
		out_color = vec4(color, 1);
	} else {
		// TODO: compute light
		float d	= distance(light_position, world_position);
		float attenuation_factor = 1 / (1 + 0.2 * d + 0.1 * d * d);

		vec3 color = Ke + ambient_light + attenuation_factor * Isursa * (diffuse_light + specular_light);

		// TODO: write pixel out color
		out_color = vec4(color, 1);
	}
	
}