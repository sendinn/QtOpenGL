#ifndef ROAMINGSCENCEMANAGER_H
#define ROAMINGSCENCEMANAGER_H

class Vector3D;
class Position3D;
class RoamingScenceManager
{
public:
    RoamingScenceManager();
    ~RoamingScenceManager();
	//初始化  
    void init();
	//开始渲染  
    void render();
	//执行旋转操作
    void executeRotateOperation(int x, int y);
	//执行缩放操作  
    void executeScaleOperation(float factor);
	//执行平移操作  
    void executeTranslateOperation(int x,int y);
	//获取鼠标按下时候的坐标
    void getInitPos(int x, int y);
private:
    //辅助坐标系三根轴
    Vector3D *AuxX;
    Vector3D *AuxY;
    Vector3D *AuxZ;

    //旋转后观察点方向与视线向上方向
    Vector3D*NewEye;
    Vector3D*NewUp;
    Vector3D *NewView;

    Position3D *OldMouse;
    Position3D *Mouse;

    Vector3D *TempTranslateVec;
    float TempscaleFactor;

    int ID_COORDINATY;
    void init_CoordinaryDisplayList();
};

#endif // ROAMINGSCENCEMANAGER_H
