#shader vertex
#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TextCoord;

uniform mat4 u_Mvp;

void main()
{
	gl_Position = /*u_Mvp **/ position;
	v_TextCoord = texCoord;
}

#shader fragment
#version 330

layout(location = 0) out vec4 color;

in vec2 v_TextCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 textureColor = texture(u_Texture, v_TextCoord);
	color = textureColor * u_Color;
}