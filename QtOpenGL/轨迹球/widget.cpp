#include "widget.h"
#include <GL/glu.h>
#include <math.h>
#include "ArcBall.h"
#include <QOpenGLWidget>
//初始化，必须用全局变量的方式，不能用new

ArcBallT arcBall(640.0f,480.0f);
ArcBallT* ArcBall =&arcBall;
Widget::Widget(QOpenGLWidget *parent)
    : QOpenGLWidget(parent)
{

}
void Widget::initializeGL()
{
	initializeOpenGLFunctions();//初始化opengl函数


    glClearColor(1,0,0,0);
    glShadeModel(GL_FLAT);
    glShadeModel( GL_SMOOTH );
    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f(0,0,0);
        glLoadIdentity();
        gluLookAt(0.0, 0.0, 5.0,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);

        glScalef(ArcBall->zoomRate, ArcBall->zoomRate, ArcBall->zoomRate);//2. 缩放
        glMultMatrixf(ArcBall->Transform.M); //3. 旋转

             drawarrow(0,-1,0,0,1,0);




}

void Widget::resizeGL( int width, int height )
{
    if ( height == 0 )
    {
        height = 1;
    }
    glViewport( 0, 0, (GLint)width, (GLint)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

}
void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() & Qt::LeftButton)
    {
        ArcBall->isClicked = true;
    }
    else if(event->button() & Qt::RightButton){
        ArcBall->isRClicked = true;
    }

}
void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    ArcBall->isClicked = false;
    ArcBall->isRClicked = false;
    ArcBall->upstate();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{

    ArcBall->MousePt.s.X = event->x();
    ArcBall->MousePt.s.Y = event->y();
    ArcBall->upstate();
    update();
}


Widget::~Widget()
{

}
void Widget::drawarrow(GLdouble x0, GLdouble y0, GLdouble z0, GLdouble x1, GLdouble y1, GLdouble z1)
{
    /*******************绘制圆柱*********************/
       GLdouble dir_x =x0 - (x1+x0)/2;
       GLdouble dir_y =y0 - (y1+y0)/2;
       GLdouble dir_z =z0 - (z1+z0)/2;
       GLdouble  bone_length = sqrt( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z );
       static GLUquadricObj *  quad_obj = NULL;
       if ( quad_obj == NULL )
       {
           quad_obj = gluNewQuadric();
       }
       gluQuadricDrawStyle( quad_obj, GLU_FILL );
       gluQuadricNormals( quad_obj, GLU_SMOOTH );
       glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); //填充
       glPushMatrix();
       glTranslated( -x0, -y0, -z0 );
       glTranslated( (x1+x0)/2, (y1+y0)/2, (z1+z0)/2 );// 平移到起始点
       double  length;
       length = sqrt( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z );
       if ( length < 0.0001 )
       {
           dir_x = 0.0; dir_y = 0.0; dir_z = 1.0;  length = 1.0;
       }
       dir_x /= length;  dir_y /= length;  dir_z /= length;//单位向量
       GLdouble  up_x, up_y, up_z;
       up_x = 0.0;
       up_y = 1.0;
       up_z = 0.0;
       double  side_x, side_y, side_z;
       side_x = up_y * dir_z - up_z * dir_y; //dir_z
       side_y = up_z * dir_x - up_x * dir_z; //0
       side_z = up_x * dir_y - up_y * dir_x; //-dir_x
       length = sqrt( side_x*side_x + side_y*side_y + side_z*side_z );
       if ( length < 0.0001 )
       {
           side_x = 1.0; side_y = 0.0; side_z = 0.0;  length = 1.0;
       }
       side_x /= length;  side_y /= length;  side_z /= length;
       up_x = dir_y * side_z - dir_z * side_y; //-dir_y
       up_y = dir_z * side_x - dir_x * side_z;//dir_z-dir_x
       up_z = dir_x * side_y - dir_y * side_x;//-dir_y
       GLdouble  m[16] = { side_x, side_y, side_z, 0.0,
           up_x,   up_y,   up_z,   0.0,
           dir_x,  dir_y,  dir_z,  0.0,
           0.0,    0.0,    0.0,    1.0 };// 计算变换矩阵
       glMultMatrixd( m );
       GLdouble radius= 0.1;           // 半径
       GLdouble slices = 30.0;         //  段数
       GLdouble stack = 3.0;         // 递归次数
       gluCylinder( quad_obj, radius, radius, bone_length, slices, stack );
       glPopMatrix();
/***********************绘制圆锥*******************************/
       dir_x =x1-(x1+x0)/2;
       dir_y =y1-(y1+y0)/2;
       dir_z =z1-(z1+z0)/2;
       bone_length = sqrt( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z );
       gluQuadricDrawStyle( quad_obj, GLU_FILL );
       gluQuadricNormals( quad_obj, GLU_SMOOTH );
       glPushMatrix();
       glTranslated( -x0, -y0, -z0 );
       glTranslated( (x1+x0)/2, (y1+y0)/2, (z1+z0)/2 );// 平移到起始点
       length = sqrt( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z );
       if ( length < 0.0001 )
       {
           dir_x = 0.0; dir_y = 0.0; dir_z = 1.0;  length = 1.0;
       }
       dir_x /= length;  dir_y /= length;  dir_z /= length;
       up_x = 0.0;
       up_y = 1.0;
       up_z = 0.0;
       side_x = up_y * dir_z - up_z * dir_y;
       side_y = up_z * dir_x - up_x * dir_z;
       side_z = up_x * dir_y - up_y * dir_x;
       length = sqrt( side_x*side_x + side_y*side_y + side_z*side_z );
       if ( length < 0.0001 )
       {
           side_x = 1.0; side_y = 0.0; side_z = 0.0;  length = 1.0;
       }
       side_x /= length;  side_y /= length;  side_z /= length;
       up_x = dir_y * side_z - dir_z * side_y;
       up_y = dir_z * side_x - dir_x * side_z;
       up_z = dir_x * side_y - dir_y * side_x;
       GLdouble  n[16] = { side_x, side_y, side_z, 0.0,
           up_x,   up_y,   up_z,   0.0,
           dir_x,  dir_y,  dir_z,  0.0,
           0.0,    0.0,    0.0,    1.0 };// 计算变换矩阵
       glMultMatrixd( n );
       radius= 0.14;        // 半径
       slices = 30.0;       //  段数
       stack = 3.0;       // 递归次数
       gluCylinder( quad_obj, radius, 0, bone_length, slices, stack );
       glPopMatrix();


}
