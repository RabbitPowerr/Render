#include "Geomety.h"
#include "vector.h"

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor col) {
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


void triangle(Vec3<int> t0, Vec3<int> t1, Vec3<int> t2, TGAImage& image,float intensity,
                std::vector<std::vector<float>>& zbuffer,Model& model, int indx) {

    Vec2<int> uv0 = model.get_uv(indx, 0);
    Vec2<int> uv1 = model.get_uv(indx, 1);
    Vec2<int> uv2 = model.get_uv(indx, 2);
    
    if (t0.y > t1.y) { std::swap(t0, t1); std::swap(uv0, uv1); }
    if (t0.y > t2.y) { std::swap(t0, t2); std::swap(uv0, uv2); }
    if (t1.y > t2.y) { std::swap(t1, t2); std::swap(uv1, uv2); }

    int total_height = t2.y - t0.y+1;

    //botom part
    for (int y = t0.y; y <= t1.y; y++) {
        int segment_height = t1.y - t0.y + 1;
        float a = (float)(y - t0.y) / total_height;
        float b = (float)(y - t0.y) / segment_height;
        Vec3<int> A = t0 + Vec3<float> (t2 - t0)*a;
        Vec3<int> B = t0 + Vec3<float>(t1 - t0) * b;

        Vec2<int> uvA = uv0 + (uv2 - uv0) * a;
        Vec2<int> uvB = uv0 + (uv1 - uv0) * b;
        if (A.x > B.x) { std::swap(A, B); std::swap(uvA, uvB); }
        for (int j = A.x; j <= B.x; j++) {
            float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
            Vec3<int> P = Vec3<float>(A) +Vec3<float>(B - A) * phi;
            Vec2<int> uvP = uvA + (uvB - uvA) * phi;
            if (P.z > zbuffer[j][y]){
                TGAColor color = model.get_color(uvP);
                zbuffer[j][y] = P.z;
                image.set(j, y, TGAColor((unsigned char)(color.r * intensity),
                         (unsigned char)(color.g * intensity),(unsigned char)(color.b * intensity)));
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


        Vec2<int> uvA = uv0 + (uv2 - uv0) * a;
        Vec2<int> uvB = uv1 + (uv2 - uv1) * b;
        if (A.x > B.x) { std::swap(A, B); std::swap(uvA, uvB); }
        for (int j = A.x; j <= B.x; j++) {
            float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
            Vec2<int> uvP = uvA + (uvB - uvA) * phi;
            Vec3<int> P = Vec3<float>(A) + Vec3<float>(B - A) * phi;
            if (P.z > zbuffer[j][y]) {
                TGAColor color = model.get_color(uvP);
                zbuffer[j][y] = P.z;
                image.set(j, y, TGAColor((unsigned char)(color.r * intensity),
                    (unsigned char)(color.g * intensity), (unsigned char)(color.b * intensity)));
            }
        }
    }

}