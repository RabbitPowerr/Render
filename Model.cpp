#include "Model.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

Model::Model(const char* filename, const char* texture){
    std::ifstream in(filename);
    if (in.fail()) {
        std::cout << "Fail" << std::endl;
        return;
    }
    std::string line;
    while (std::getline(in, line)) {
        bool mode = count(line.begin(), line.end(), '/') == 6;
        std::replace(line.begin(), line.end(), '/', ' ');
        std::stringstream str(line);
        std::string type;
        str >> type;
        if (type == "v") {
            Vec3<float> tmp;
            str >> tmp;
            vertex.push_back(tmp);
        }
        if (type == "f") {
            int a, b;
            std::vector<Vec3<int>> tmp(3);
            for (int i = 0; i < 3; i++) {
                str >> tmp[i].x>>tmp[i].y;
                tmp[i].x--;
                tmp[i].y--;
                if (mode) {
                    str >> tmp[i].z;
                    tmp[i].z--;
                }
            }
            faces.push_back(tmp);
        }
        if (type == "vn") {
            Vec3<float> n;
            str >> n;
            norm.push_back(n);
        }
        if (type == "vt") {
            Vec2<float> _uv;
            str >> _uv.x >> _uv.y;
            uv.push_back(_uv);
        }
    }
    std::cout << "OK\n";
    std::cout << vertex.size() << ' ' << faces.size() <<'\n';

    std::cout << "texture file " << texture << " loading " << (texture_img.read_tga_file(texture) ? "ok" : "failed") << std::endl;
    texture_img.flip_vertically();
    



}

Model::~Model(){}

int Model::numverts(){
    return vertex.size();
}

int Model::numfaces(){
    return faces.size();
}

void Model::normalize(){
    std::vector<float> max(3,0);
    for (int i = 0; i < vertex.size(); i++) {
        max[0] = std::max(abs(vertex[i].x), max[0]);
        max[1] = std::max(abs(vertex[i].y), max[1]);
        max[2] = std::max(abs(vertex[i].z), max[2]);
    }
    for (int i = 0; i < vertex.size(); i++) {
        vertex[i].x = vertex[i].x / max[0];
        vertex[i].y = vertex[i].y / max[1];
    }
}

Vec3<float> Model::vert(int i){
    return vertex[i];
}

std::vector<int> Model::face(int i){
    std::vector<int> face;
    for (int j = 0; j < (int)faces[i].size(); j++) face.push_back(faces[i][j][0]);
    return face;
}

Vec2<int> Model::get_uv(int iface, int nvert)
{    
    int idx = faces[iface][nvert][1];
    return Vec2<int>(uv[idx].x * texture_img.get_width(), uv[idx].y * texture_img.get_height());
}

TGAColor Model::get_color(Vec2<int> uv){
    return texture_img.get(uv.x, uv.y);
}



void Model::resize() {
    float k = 0;
    for (int i = 0; i < vertex.size(); i++) {
        k = std::max(k,abs(vertex[i].x));
        k = std::max(k,abs(vertex[i].y));
        k = std::max(k, abs(vertex[i].z));
    }
    for (int i = 0; i < vertex.size(); i++) {
        vertex[i].x /= k;
        vertex[i].y /= k;
        vertex[i].z /= k;
    }
}