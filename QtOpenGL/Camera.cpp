#include "Camera.h"
#include <QtMath>
#define Radian(x) (M_PI*(x)/180.0)
Camera::Camera()
	: m_CameraPos(QVector3D(0.0f,0.0f,3.0f))
	, m_CameraTarget(QVector3D(0.0f, 0.0f, 0.0f))
	, yaw(Radian(-90))
	, pitch(0.0)
	, sensitivity(0.01)
{
	//摄像机方向指向-z轴，所以这里要取正z轴需要从观察点指向摄像机
	m_CameraZaxis = m_CameraPos - m_CameraTarget;
	m_CameraFront = -m_CameraZaxis;
	m_CameraRight = QVector3D::crossProduct({ 0.0f,1.0f,0.0f }, m_CameraZaxis); //up向量为世界坐标y轴，若交换位置则求得相机坐标-x轴
	m_CameraUp = QVector3D::crossProduct(m_CameraZaxis, m_CameraRight);
	distance = m_CameraFront.length();
}

Camera::~Camera()
{

}

void Camera::UpdateFrontDirection()
{
	QVector3D direction;
	/*
	不考虑翻滚，只有pitch才能影响到y轴 (yaw在xz平面上，会同时影响x，z)
	而如果pitch的位置不在起始位置（经过一次yaw），那么pitch会同时影响x，z
	这里我们假设 先 pitch -> yaw
	所以我们先计算y轴方便理解
	*/
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	direction.setY(sin(pitch));
	direction.setX(cos(pitch)*cos(yaw));
	direction.setZ(cos(pitch)*sin(yaw));
	//direction.normalize();

	m_CameraFront = direction * distance;
}
