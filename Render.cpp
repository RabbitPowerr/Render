#include "Render.h"
#include <algorithm>

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
           	Matrix tmp = vec3_to_mat(world_coords);
			screen_coords[j] = mat_to_vec3(view * projection * look * tmp);
			coords[j] = world_coords;
		}
    	triangle(screen_coords[0], screen_coords[1], screen_coords[2], image,i);
		
	}

	image.flip_vertically();
	std::string name(outfile);
	name += ".tga";
	image.write_tga_file(name.c_str());
}

float clamp(float val, float min, float max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

void Renderer::triangle(Vec3<int> t0, Vec3<int> t1, Vec3<int> t2, TGAImage& image, int indx){
    Vec2<int> uv0 = model.get_uv(indx, 0);
    Vec2<int> uv1 = model.get_uv(indx, 1);
    Vec2<int> uv2 = model.get_uv(indx, 2);
    std::vector<int> face = model.face(indx);
    
    std::vector<Vec3<float>> vecn(3);
    if (!model.exist_nm()) {
        for (int i = 0; i < 3; i++) {
            vecn[i] = model.get_vn(face[i]);
        }
    }
    if (t0.y > t1.y) { std::swap(t0, t1); std::swap(uv0, uv1); std::swap(vecn[0], vecn[1]); }
    if (t0.y > t2.y) { std::swap(t0, t2); std::swap(uv0, uv2); std::swap(vecn[0], vecn[2]);}
    if (t1.y > t2.y) { std::swap(t1, t2); std::swap(uv1, uv2); std::swap(vecn[2], vecn[1]);}

    int total_height = t2.y - t0.y + 1;

    //botom part
    for (int y =t0.y; y <= t1.y; y++) {
        int segment_height = t1.y - t0.y + 1;
        float a = (float)(y - t0.y) / total_height;
        float b = (float)(y - t0.y) / segment_height;
        Vec3<int> A = t0 + Vec3<float>(t2 - t0) * a;
        Vec3<int> B = t0 + Vec3<float>(t1 - t0) * b;
        Vec3<float> vecnA, vecnB;
        if (!model.exist_nm()) {
            vecnA = vecn[0] + (vecn[2] - vecn[0]) * a;
            vecnB = vecn[0] + (vecn[1] - vecn[0]) * b;
        }
        Vec2<int> uvA = uv0 + (uv2 - uv0) * a;
        Vec2<int> uvB = uv0 + (uv1 - uv0) * b;
        if (A.x > B.x) { std::swap(A, B); std::swap(uvA, uvB); std::swap(vecnA, vecnB); }
        
        for (int j =A.x; j <=B.x; j++) {
            float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
            Vec3<int> P = Vec3<float>(A) + Vec3<float>(B - A) * phi;
            Vec2<int> uvP = uvA + (uvB - uvA) * phi;
            if (P.z > zbuffer[j][y]) {
                TGAColor color = model.get_color(uvP);
                zbuffer[j][y] = P.z;
                float curr_intensity ;
                if (!model.exist_nm()) {
                    curr_intensity = (vecnA + (vecnB - vecnA) * phi) * ligth_dir;
                }
                else curr_intensity = model.get_vn(uvP)*ligth_dir;
                image.set(j, y, color*curr_intensity);
            }
        }
    }

    //top part
    for (int y = t1.y; y <= t2.y; y++) {
        int segment_height = t2.y - t1.y + 1;
        float a = (float)(y - t0.y) / total_height;
        float b = (float)(y - t1.y) / segment_height;
        Vec3<int> A = t0 + Vec3<float>(t2 - t0) * a;
        Vec3<int> B = t1 + Vec3<float>(t2 - t1) * b;
        Vec3<float> vecnB, vecnA;
        if (!model.exist_nm()) {
            vecnA = vecn[0] + (vecn[2] - vecn[0]) * a;
            vecnB = vecn[1] + (vecn[2] - vecn[1]) * b;
        }

        Vec2<int> uvA = uv0 + (uv2 - uv0) * a;
        Vec2<int> uvB = uv1 + (uv2 - uv1) * b;
        if (A.x > B.x) { std::swap(A, B); std::swap(uvA, uvB); std::swap(vecnA, vecnB);}
        for (int j = A.x; j <= B.x; j++) {
            float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
            Vec2<int> uvP = uvA + (uvB - uvA) * phi;
            Vec3<int> P = Vec3<float>(A) + Vec3<float>(B - A) * phi;
            if (P.z > zbuffer[j][y]) {
                TGAColor color = model.get_color(uvP);
                zbuffer[j][y] = P.z;
                float curr_intensity;
                if (!model.exist_nm()) {
                    curr_intensity = (vecnA + (vecnB - vecnA) * phi) * ligth_dir;
                }
                else curr_intensity = model.get_vn(uvP) * ligth_dir;
                image.set(j, y, color*curr_intensity);
            }
        }
    }

}

void Renderer::line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor col) {
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y1, y0);
    }
    for (int x = x0; x <= x1; x++) {
        double t = (double)(x - x0) / double(x1 - x0);
        int y = y0 * (1 - t) + y1 * t;
        if (steep) image.set(x, y, col);
        else image.set(y, x, col);
    }
}


Renderer::~Renderer(){}
