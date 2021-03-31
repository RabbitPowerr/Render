#pragma once
#ifndef Geom
#define Geom

#include "tgaimage.h"
#include "vector.h"
#include "Model.h"

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor col);

void triangle(Vec3<int> t0, Vec3<int> t1, Vec3<int> t2, TGAImage& image, float intensity,
			std::vector<std::vector<float>>& zbuffer,Model& model,int indx );

#endif // !Geom