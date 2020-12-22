#include "QtOpenGL.h"
#include <QtWidgets/QApplication>
#include "PaintingWidget.h"
#include "QQuaternion"
#include "QVector3D"
#include "QMatrix4x4"
//#include "TrackBall/widget.h"

#define Sed_PI 3.1415926535897932384626433832795
#define RADIA(x)   (x) * Sed_PI / 180.0
#define ANGLE(x)   (x) * 180 / Sed_PI
QQuaternion GetQuanternionByVector(QVector3D _src, QVector3D _des)
{
	_src.normalize();
	_des.normalize();

	//旋转轴
	QVector3D rotateAxis = QVector3D::crossProduct(_src, _des);
	//旋转角度
	double theta = std::acos(QVector3D::dotProduct(_src, _des));
	//利用旋转轴和旋转角度得出四元数
	return QQuaternion::fromAxisAndAngle(rotateAxis, ANGLE(theta));
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


	{
		QVector3D _faceCenter(1, 1, 0), m_a, m_b, m_c, m_d;
		double m_length = 2, m_height = 2 , length = 2;
		//左上角 逆时针
		m_a = QVector3D(-m_length / 2, m_height / 2, length);
		m_b = QVector3D(m_length / 2, m_height / 2, length);
		m_c = QVector3D(m_length / 2, -m_height / 2, length);
		m_d = QVector3D(-m_length / 2, -m_height / 2, length);

		double angleWithX = ANGLE(std::atan(_faceCenter.y() / _faceCenter.x()));

		QQuaternion dr1 = GetQuanternionByVector(QVector3D(0, 0, length), _faceCenter);
		QQuaternion dr2 = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), angleWithX);
		m_a = dr1 * dr2 * m_a;
		m_b = dr1 * dr2 * m_b;
		m_c = dr1 * dr2 * m_c;
		m_d = dr1 * dr2 * m_d;

		int ad = 1;
	}

// 	Widget t;
// 	t.resize(800, 600);
// 	t.show();


    QtOpenGL w;
	w.resize(800, 600);
    w.show();

    return a.exec();
}
