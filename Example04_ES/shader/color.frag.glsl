#version 300 es

precision lowp float;

in vec4 v_color;

out vec4 fragColor;

void main(void)
{
	fragColor = v_color;
}
