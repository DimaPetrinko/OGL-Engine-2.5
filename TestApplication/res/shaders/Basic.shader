#shader vertex
#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 u_Mvp;
uniform mat4 u_M;

out vec2 v_TextCoord;
flat out vec3 v_Normal;

void main()
{
	gl_Position = u_Mvp * position;
	v_TextCoord = texCoord;
	v_Normal = normalize(transpose(inverse(mat3(u_M))) * normal);
}

#shader fragment
#version 330

out vec4 color;

in vec2 v_TextCoord;
flat in vec3 v_Normal;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{

	float light = max( 0.0, dot(normalize(v_Normal), normalize(-vec3(0.5, 1.0, 0.0))));
	vec4 textureColor = texture2D(u_Texture, v_TextCoord);
	color = textureColor * u_Color * (vec4(light) + vec4(0.1));
}