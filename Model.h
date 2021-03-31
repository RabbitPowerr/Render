#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "vector.h"
#include "tgaimage.h"

class Model {
private:
	std::vector<Vec3<float>> vertex;
	std::vector<std::vector<Vec3<int>>> faces;
	std::vector<Vec2<float>> uv;
	std::vector<Vec3<float>> norm;
	TGAImage texture_img;
public:
	Model(const char* filename,const char* texture);
	~Model();
	int numverts();
	int numfaces();
	void normalize();
	void resize();
	Vec3<float> vert(int i);
	std::vector<int> face(int i);
	Vec2<int> get_uv(int iface, int nvert);
	TGAColor get_color(Vec2<int> uv);
};
#endif 
