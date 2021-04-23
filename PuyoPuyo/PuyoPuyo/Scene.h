#pragma once
#include "Primitives.h"


class Corner{
public:
	Point3D P;	
}
;
class frame:Drawable
{
private :
	Point3D frameCenter;	
	float height;
	float wide;
public :
	frame(float height, float wide,Point3D center);
	void createCorners();
	void Draw();
	float getWidth();
	float getHeight();
	Corner UpperLeftCooner;
	Corner UpperRigthCooner;
	Corner LowerRigthCooner;
	Corner LowerLeftCooner;
};
 class Scene:Drawable
{
	frame* gameFrame;
public:
	Scene(void);
	~Scene();
	Point3D getPoint3dFromGridCoord(int row,int col,int totalRows,int totalColums);
	void Draw();
};

