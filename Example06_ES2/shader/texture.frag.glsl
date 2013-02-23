precision lowp float;

uniform sampler2D u_texture; 

varying float v_intensity;
varying vec2 v_texCoord;

void main(void)
{
	gl_FragColor = texture2D(u_texture, v_texCoord) * v_intensity;
}
