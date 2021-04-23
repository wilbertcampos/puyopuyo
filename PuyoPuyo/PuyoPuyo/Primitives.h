#pragma once
#include <vector>
using namespace std;


struct ColorRGB
{
    float Red;
    float Green;
    float Blue;
};
enum EnumSphereColor{E_RED,E_GREEN,E_BLUE,E_YELLOW}; //4 color for each sphere

struct Point3D{// to represent screen coordinates
	float X;
	float Y;
	float Z;
} ;
enum Orientation{HORIZONTAL,VERTICAL};//falling piece orientation (--):horizontal  (|):vertical
enum DIRECTION{LEFT,RIGTH};
struct GridPosition2D
{
	int row;
	int column;
};
/*
(0,       0)--------(0      ,NUMCOLS) 
  |							|
  |							|
  |							|
  |							|
 (NUMROWS,0)--------(NUMROWS,NUMCOLS)
*/
class Drawable
{
public:
	Drawable();
	virtual void Draw()=0;
	ColorRGB color;
};
#define DEFAULT_RADIUS_SIZE 0.3;
class Sphere:Drawable{//we need an class to represents spheres
private:
	ColorRGB		RGBColor;	//to draw the sphere color on the screen
	EnumSphereColor EColor; //attribute type enum
	Point3D*	CenterScreenCord;	//sphere center in Screeen coordinates
	float radius;//sphere radius
	GridPosition2D gridPos;

public :
	bool canDelete;
	bool actualizarCordenadasDespuesDeEliminar;
	Sphere();
	void Draw();
	EnumSphereColor getRandomSphereColor();
	ColorRGB getRGBColorFromEnumColor(EnumSphereColor ecolor);
	Point3D getCenterScreenCord();
	void setScreenCoodinates(Point3D p);
	void setGridPos(int row,int col);
	GridPosition2D getGridPos();
	void Move(DIRECTION d);
	void changeRGBColor(ColorRGB	RGBColorC);
	ColorRGB getRGBColor();

};

class FallingPiece:Drawable
{

private :
	int isFalling;
	Orientation getRandomOrientation();	
	void createSpheres();
	EnumSphereColor getRandomSphereColor();
	Orientation orientation;
	int getRandomGridCol();	
	void changeOrden();
public :
	FallingPiece();
	void Fall();
	int IsFalling();
	//vector<Sphere*> *fallingSpheres;
	Sphere* e1;
	Sphere* e2;
	void Draw();
	void Move(DIRECTION d);
	void RotateL();
	void RotateR();
	int CanRotateR();
	int CanRotateL();
	void stopFalling();
	void startFall();
	Orientation getOrientation();
};

enum GameStatus{PLAY,PAUSE,STOP,PLAY_REMOVING_SPHERES,GAME_OVER};