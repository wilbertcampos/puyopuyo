#pragma once
#include "Scene.h"
enum NOT_INCLUDE{N_NONE,N_LEFT,N_RIGHT,N_UP,N_DOWN};
class GridEsferas:Drawable
{
private:
	vector<vector<Sphere*>*>*elementos;
	vector<Sphere*>* links;
	int numRows;
	int numcols;
	void markToDelete(vector<Sphere*>* linksl);
	Sphere * getNeighborAt(int row,int column,ColorRGB colorC);	
	void checNeightBorg(Sphere* e,int level,vector<Sphere*>* links,NOT_INCLUDE no_include );
	void deleteSphere(Sphere * e);
public:
	void removeSpheres();	
	void setNULL(int row,int col);
	int arethereSpheresToDelete();
	int add(Sphere * esfera,int row,int column);
	Sphere * getSphereAt(int Row,int Col);
	GridEsferas(int numRows,int numcols);
	void ChangeSphereColorGameOver(ColorRGB colorC);
	~GridEsferas(void);
	void Draw();
};

