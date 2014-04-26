# version 420

in vec2 tex_coord;
out vec4 out_color;

uniform sampler2D image;
uniform float scale;
uniform float rot;

void main(void) {

    mat2 scale_matrix = mat2(scale, 0, 0, scale);
    mat2 rot_matrix = mat2(cos(rot), -sin(rot), sin(rot), cos(rot));

    vec2 img_coord = inverse(scale_matrix * rot_matrix) * tex_coord;

    img_coord += 0.5;

    if (img_coord.x > 1 || img_coord.x < 0 || img_coord.y > 1 || img_coord.y < 0)
    {
        out_color = vec4(0.0);
    } else {
        out_color = texture(image, img_coord);
    }
}
