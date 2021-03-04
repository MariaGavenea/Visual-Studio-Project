#version 330
 
uniform sampler2D texture;
uniform int mapping_type;

in vec2 texcoord;
in vec3 position;

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction[20];
uniform vec3 light_position[20];
uniform int num_lights;
uniform vec3 eye_position;

uniform int material_shininess;
uniform vec3 Ke, Ka, Kd, Ks;
uniform vec3 Isursa[20], Ia;

uniform int is_spotlight[20];
uniform float cut_off_angle;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO : calculate the out_color using the texture2D() function
	vec4 color = texture2D(texture, texcoord);

	if (mapping_type == 1) {
		// mapare cilindrica
		vec2 texC = vec2(1.0f / (2 * 3.14) *  atan(position.x, position.z), 1 - (position.y + 0.5));
		color = texture2D(texture, texC);
	}

	if (color.a < 0.5f) {
		discard;
	}

	// TODO: define ambient light component
	vec3 ambient_light = Ia * Ka;
	vec3 V = normalize(eye_position - world_position);
	vec3 light = vec3(0, 0, 0);

	for (int i = 0; i < num_lights; i++) {
		vec3 L = normalize(light_position[i] - world_position);

		// TODO: compute diffuse light component
		float product = max(dot(world_normal, L), 0);
		vec3 diffuse_light = Kd * product;

		// TODO: compute specular light component
		vec3 specular_light = vec3(0, 0, 0);

		if (product > 0)
		{
			vec3 R = normalize(reflect(-L, world_normal));

			specular_light = Ks * pow(max(dot(V, R), 0), material_shininess);
		}

		if (is_spotlight[i] == 1) {
			float cut_off = radians(cut_off_angle);
			float spot_light = dot(-L, light_direction[i]);
			float spot_light_limit = cos(cut_off);
			float light_att_factor = 0;

			if (spot_light > spot_light_limit)
			{
				// Quadratic attenuation
				float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
				light_att_factor = pow(linear_att, 2);
			}

			light += light_att_factor * Isursa[i] * (diffuse_light + specular_light);

		} else {
			// TODO: compute light
			float d	= distance(light_position[i], world_position);
			float attenuation_factor = 1 / (1 + 0.01 * d + 0.02 * d * d);

			light += attenuation_factor * Isursa[i] * (diffuse_light + specular_light);
		}
	}
	
	vec3 color1 = Ke + ambient_light + light;
	out_color = vec4(color1, 1) * color;
}