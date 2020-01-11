#shader vertex
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 u_Mvp;
uniform mat4 u_M;
uniform vec3 u_LightDirection;

out vec2 v_TextCoord;
flat out vec3 v_Normal;
out vec3 v_LightDirection;

void main()
{
	gl_Position = u_Mvp * vec4(position, 1.0);
	v_TextCoord = texCoord;
	v_Normal = normalize(transpose(inverse(mat3(u_M))) * normal);
	v_LightDirection = u_LightDirection;
}

#shader fragment
#version 330

out vec4 color;

in vec2 v_TextCoord;
flat in vec3 v_Normal;
in vec3 v_LightDirection;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	float light = max(0.0, dot(normalize(v_Normal), normalize(v_LightDirection)));
	vec4 textureColor = texture2D(u_Texture, v_TextCoord);
	color = textureColor * u_Color * (vec4(light) + vec4(0.2));
}