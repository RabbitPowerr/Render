#include "Render.h"


Renderer::Renderer() :model("obj/african_head.obj","african_head.tga"),eye(0,0,3),
					ligth_dir(0,0,-1),center(0,0,0){}

Renderer::Renderer(const char* objfilename, const char* texturefile, Vec3<float> _eye, Vec3<float> _light) :
	model(objfilename, texturefile), eye(_eye), ligth_dir(_light), center(0,0,0){}

void Renderer::start_render(const char* outfile) {
	TGAImage image(height + 1, width + 1, TGAImage::RGB);
	zbuffer.assign(height + 1, std::vector<float>(width + 1, INT32_MIN));
	model.resize();

	Matrix projection = identity_matrix(4);
	Matrix view = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);
	projection.set(3, 2, -(1.f / (eye - center).norm()));
	Matrix look = lookat(eye, center, Vec3<float>(0, 1, 0));
	for (int i = 0; i < model.numfaces(); i++) {
		std::vector<int> face = model.face(i);
		//std::cerr << i << std::endl;
		Vec3<float> screen_coords[3];
		Vec3<float> coords[3];


		for (int j = 0; j < 3; j++) {
			Vec3<float> world_coords = model.vert(face[j]);

			Vec3<float> was = Vec3<float>((world_coords.x + 1.) * width / 2., (world_coords.y + 1.) * height / 2.,
				world_coords.z * depth / 2.);
			Matrix tmp = vec3_to_mat(world_coords);
			screen_coords[j] = mat_to_vec3(view * projection * look * tmp);
			coords[j] = world_coords;
		}

		Vec3<float> n = (coords[2] - coords[0]) ^ (coords[1] - coords[0]);

		n.normalize();
		float intensity = n * ligth_dir;
		if (intensity > 0) {
			triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, intensity, zbuffer, model, i);
			//break;
		}
	}

	image.flip_vertically();
	std::string name(outfile);
	name += ".tga";
	image.write_tga_file(name.c_str());

}

Renderer::~Renderer(){}
