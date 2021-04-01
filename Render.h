#pragma once

#include"Model.h"

class Renderer{
private:
	const TGAColor white = TGAColor(255, 255, 255, 255);
	const TGAColor black = TGAColor(0, 0, 0, 255);
	const TGAColor red = TGAColor(255, 0, 0, 255);
	const int height = 1000, width = 1000, depth = 255;
	const Vec3<float> ligth_dir;
	std::vector<std::vector<float>> zbuffer;
	Model model;
	Vec3<float> eye;
	Vec3<float> center;
	void triangle(Vec3<int> t0, Vec3<int> t1, Vec3<int> t2, TGAImage& image,int indx);
	void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor col);

public:
	Renderer();
	Renderer(const char* objfilename, const char* texturefile, Vec3<float> _eye = {0,0,4}, Vec3<float> _light = { 0,0,-1 });
	void start_render(const char* outfile);
	~Renderer();
	


};

