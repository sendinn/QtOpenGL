#pragma once
#include <QVector3D>
class Camera
{
public:
	Camera();
	~Camera();

	void UpdateFrontDirection(); //更新摄像机朝向
public:
	QVector3D m_CameraPos;
	QVector3D m_CameraTarget;

	double yaw;             //偏航角
	double pitch;           //俯视角
	double sensitivity;     //鼠标灵敏度
	QVector3D m_CameraFront;//相机朝向


	float distance;
	QVector3D m_CameraZaxis;
	QVector3D m_CameraRight;
	QVector3D m_CameraUp;
};

