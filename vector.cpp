#include "vector.h"

template <> template <> Vec3<int>::Vec3(const Vec3<float>& v) :
	x(v.x + 0.5f), y(v.y + 0.5f), z(v.z + 0.5f) {}

Matrix::Matrix(int r, int c){
	cols = c;
	rows = r;
	matrix.assign(r, std::vector<float>(c, 0));

}

int Matrix::nrows(){
	return matrix.size();
}

int Matrix::ncols(){
	return matrix[0].size();
}

void Matrix::set(int i, int j, float v){
	matrix[i][j] = v;
}

std::vector<float>& Matrix::operator[](const int i){
	return matrix[i];
}

Matrix Matrix::operator*(Matrix& a){
	Matrix tmp(cols,a.cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < a.cols; j++) {
			tmp.matrix[i][j] = 0.f;
			for (int k = 0; k < cols; k++) {
				tmp.matrix[i][j] += matrix[i][k] * a.matrix[k][j];
			}
		}
	}
	return tmp;
}


//make diff kind of matrix
Matrix identity_matrix(int dimensions){
	Matrix answ(dimensions, dimensions);
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			answ.matrix[i][j] = (i == j);
		}
	}
	return answ;
}

Matrix viewport(int x, int y, int w, int h,int depth){
	Matrix result = identity_matrix(4);
	result.matrix[0][3] = x + w / 2.f;
	result.matrix[1][3] = y + h / 2.f;
	result.matrix[2][3] = depth / 2.f;

	result.matrix[0][0] = w / 2.f;
	result.matrix[1][1] = h / 2.f;
	result.matrix[2][2] = depth / 2.f;
	return result;
	
}

Vec3<float> mat_to_vec3(const Matrix& m){
	return Vec3<float>(m.matrix[0][0] / m.matrix[3][0],
		m.matrix[1][0] / m.matrix[3][0],
		m.matrix[2][0] / m.matrix[3][0]);
}

Matrix vec3_to_mat(const Vec3<float>& vec){
	Matrix result(4, 1);
	result.matrix[0][0] =vec.x;
	result.matrix[1][0] = vec.y; 
	result.matrix[2][0] = vec.z; 
	result.matrix[3][0] = 1.0f; 
	return result;
}

Matrix lookat(Vec3<float> eye, Vec3<float> center, Vec3<float> up){
	Vec3<float> z = (eye - center).normalize();
	Vec3<float> x = (up ^ z).normalize();
	Vec3<float> y = (z ^ x).normalize();
	Matrix res = identity_matrix(4);
	for (int i = 0; i < 3; i++) {
		res[0][i] = x[i];
		res[1][i] = y[i];
		res[2][i] = z[i];
		res[i][3] = -center[i];
	}
	return res;
}
