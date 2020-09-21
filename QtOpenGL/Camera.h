#pragma once
#include <QVector3D>
class Camera
{
public:
	Camera();
	~Camera();

	void UpdateFrontDirection(); //�������������
public:
	QVector3D m_CameraPos;
	QVector3D m_CameraTarget;

	double yaw;             //ƫ����
	double pitch;           //���ӽ�
	double sensitivity;     //���������
	QVector3D m_CameraFront;//�������


	float distance;
	QVector3D m_CameraZaxis;
	QVector3D m_CameraRight;
	QVector3D m_CameraUp;
};

