# version 420

in vec2 tex_coord;
out vec4 out_color;

uniform sampler2D image;
uniform float scale;
uniform float rot;
uniform vec3 trans;

void main(void) {

    mat3 scale_matrix = mat3(scale, 0, 0,
                             0, scale, 0,
                             0, 0, 1);
    mat3 rot_matrix = mat3(cos(rot), -sin(rot), 0,
                           sin(rot), cos(rot), 0,
                           0, 0, 1);
    mat3 trans_matrix = mat3(1, 0, trans.x,
                             0, 1, trans.z,
                             0, 0, 1);

    vec3 img_coord_3 = inverse(scale_matrix * rot_matrix * trans_matrix) * vec3(tex_coord, 1);
    vec2 img_coord = img_coord_3.xy;

    img_coord += 0.5;

    if (img_coord.x > 1 || img_coord.x < 0 || img_coord.y > 1 || img_coord.y < 0)
    {
        out_color = vec4(0.0);
    } else {
        out_color = texture(image, img_coord);
    }
}
