#include "Camera.h"

Camera::Camera()
	: m_CameraPos(QVector3D(-3.0f,0.0f,0.0f))
	, m_CameraTarget(QVector3D(0.0f, 0.0f, 0.0f))
	, yaw(0.0)
	, pitch(0.0)
	, sensitivity(0.01)
{
	//���������ָ��-z�ᣬ��������Ҫȡ��z����Ҫ�ӹ۲��ָ�������
	m_CameraZaxis = m_CameraPos - m_CameraTarget;
	m_CameraFront = -m_CameraZaxis;
	distance = m_CameraFront.length();
}

Camera::~Camera()
{

}

QVector3D Camera::GetCameraUp()
{
	m_CameraRight = QVector3D::crossProduct({ 0.0f,1.0f,0.0f }, m_CameraZaxis); //up����Ϊ��������y�ᣬ������λ��������������-x��
	m_CameraUp = QVector3D::crossProduct(m_CameraZaxis, m_CameraRight);
	return m_CameraUp;
}

void Camera::UpdateFrontDirection()
{
	QVector3D direction;
	/*
	�����Ƿ�����ֻ��pitch����Ӱ�쵽y�� (yaw��xzƽ���ϣ���ͬʱӰ��x��z)
	�����pitch��λ�ò�����ʼλ�ã�����һ��yaw������ôpitch��ͬʱӰ��x��z
	�������Ǽ��� �� pitch -> yaw
	���������ȼ���y�᷽������
	*/
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	direction.setY(sin(pitch));
	direction.setX(cos(pitch)*cos(yaw));
	direction.setZ(cos(pitch)*sin(yaw));
	direction.normalize();

	m_CameraFront = direction * distance;
}