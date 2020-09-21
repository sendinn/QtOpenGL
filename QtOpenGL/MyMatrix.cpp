#include "MyMatrix.h"
#include "QMatrix4x4"
#include "QVector3D"

MyMatrix::MyMatrix()
{
	test();
}

MyMatrix::~MyMatrix()
{

}


// 参数中vector是轴的方向（一定要先单位化后再传参），point表示这个轴所经过的某一个点，t表示角度
QMatrix4x4 rot_mat(const QVector3D& point, const QVector3D& vector, const float t)
{
	float u = vector[0];
	float v = vector[1];
	float w = vector[2];
	float a = point[0];
	float b = point[1];
	float c = point[2];

	QMatrix4x4 matrix(u * u + (v*v + w * w)*cos(t), u*v*(1 - cos(t)) - w * sin(t), u*w*(1 - cos(t)) + v * sin(t), (a*(v*v + w * w) - u * (b*v + c * w))*(1 - cos(t)) + (b*w - c * v)*sin(t),
		u*v*(1 - cos(t)) + w * sin(t), v*v + (u*u + w * w)*cos(t), v*w*(1 - cos(t)) - u * sin(t), (b*(u*u + w * w) - v * (a*u + c * w))*(1 - cos(t)) + (c*u - a * w)*sin(t),
		u*w*(1 - cos(t)) - v * sin(t), v*w*(1 - cos(t)) + u * sin(t), w*w + (u*u + v * v)*cos(t), (c*(u*u + v * v) - w * (a*u + b * v))*(1 - cos(t)) + (a*v - b * u)*sin(t),
		0, 0, 0, 1);
	return matrix;
}

void MyMatrix::test()
{
	QMatrix4x4 mat;
	QQuaternion dr;

	std::vector<QVector3D> line;
	line.push_back(QVector3D(-1, 0, 0));
	line.push_back(QVector3D( 1, 0, 0));

	mat.translate(QVector3D(0, 1, 0));

	mat.rotate(90, QVector3D(0, 0, 1));

	mat.translate(QVector3D(0, -1, 0));
	
	QMatrix4x4 rm = rot_mat(QVector3D(0, 0, 1), QVector3D(1, 0, 0), 90);






	for (auto& one : line)
	{
		one = rm * one;
	}



	while (true)
	{

	}
}
