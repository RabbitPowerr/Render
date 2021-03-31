#pragma once

#include <cmath>
#include <iostream>
#include <vector>

template <class t> struct Vec2 {
    t x, y;
    Vec2<t>() : x(), y() {}
    Vec2<t>(t _x, t _y) : x(_x), y(_y) {}
    Vec2<t>(const Vec2<t>& v) : x(t()), y(t()) { *this = v; }
    
    Vec2<t> operator +(const Vec2<t>& V) { return Vec2<t>(x + V.x, y + V.y); }
    Vec2<t> operator *(float f){ return Vec2<t>(x * f, y * f); }
    Vec2<t> operator -(const Vec2<t>& V) { return Vec2<t>(x - V.x, y - V.y); }
   
    template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
};

template <class t> struct Vec3 {
    t x, y, z;
    Vec3() : x(), y(), z() { }
    Vec3(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}
    template <class u> Vec3(const Vec3<u>& v) : x(t(v.x)), y(t(v.y)), z(t(v.z)) {}
    
    Vec3(const Vec3<t>& v) : x(v.x), y(v.y), z(v.z){}
    Vec3 operator ^(const Vec3<t>& v) { return Vec3<t>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
    Vec3 operator +(const Vec3<t>& v){ return Vec3<t>(x + v.x, y + v.y, z + v.z); }
    Vec3 operator -(const Vec3<t>& v) const { return Vec3<t>(x - v.x, y - v.y, z - v.z); }
    Vec3 operator *(float f){ return Vec3<t>(x * f, y * f, z * f); }
    t operator *(const Vec3<t>& v){ return x * v.x + y * v.y + z * v.z; }
    
    float norm() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3<t>& normalize(t l = 1) { *this = (*this) * (l / norm()); return *this; }
    t& operator[](const int i) { if (i <= 0) return x; else if (i == 1) return y; else return z; }
    template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);
    template <class > friend std::istream& operator>>(std::istream& s, Vec3<t>& v);
};

template <> template <> Vec3<int>::Vec3(const Vec3<float>& v);

template <class t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v) {
    s << v.x << " " << v.y << "\n";
    return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v) {
    s << v.x << " " << v.y << " " << v.z <<"\n";
    return s;
}

template <class t> std::istream& operator>>(std::istream& s, Vec3<t>& v) {
    s >> v.x >> v.y >> v.z;
    return s;
}

class Matrix {
    std::vector<std::vector<float> > matrix;
    int rows, cols;
public:
    Matrix(int r=4 , int c=4);
    int nrows();
    int ncols();

    friend Matrix identity_matrix(int dimensions);
    friend Matrix viewport(int x,int y,int w,int h,int depth);
    friend Vec3<float> mat_to_vec3(const Matrix& m);
    friend Matrix vec3_to_mat(const Vec3<float>& m);
    friend Matrix lookat(Vec3<float> eye, Vec3<float> center, Vec3<float> up);
    void set(int i, int j, float v);
    std::vector<float>& operator[](const int i);
    Matrix operator*(Matrix& a);
    
    //friend std::ostream& operator<<(std::ostream& s, Matrix& m);
};

Matrix identity_matrix(int dimensions);
Matrix viewport(int x, int y, int w, int h, int depth);
Vec3<float> mat_to_vec3(const Matrix& m);
Matrix vec3_to_mat(const Vec3<float>& m);
Matrix lookat(Vec3<float> eye, Vec3<float> center, Vec3<float> up);

