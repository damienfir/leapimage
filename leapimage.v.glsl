# version 420

in vec2 vertices;

out vec2 tex_coord;

void main(void) {
	gl_Position = vec4(vertices, 0.0, 1.0);
	tex_coord = vertices / 2.0;
}
