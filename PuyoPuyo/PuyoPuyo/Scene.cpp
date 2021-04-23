#include "StdAfx.h"
#include "Scene.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
Drawable::Drawable()
{
	color.Red=0;
	color.Green=0;
	color.Blue=0;
}
frame::frame(float height, float wide,Point3D center)
{
	this->height=height;
	this->wide=wide;
	this->frameCenter=center;
	createCorners();
}
float frame::getWidth()
{
	return wide;
}
float frame::getHeight()
{
	return height;
}
void frame::createCorners()
{//calculates coordinates 
	//|__
	LowerLeftCooner.P.X= this->frameCenter.X-(wide/2);
	LowerLeftCooner.P.Y= this->frameCenter.Y-(height/2);
	LowerLeftCooner.P.Z=this->frameCenter.Z;
	//__|
	LowerRigthCooner.P.X=this->frameCenter.X+(wide/2);
	LowerRigthCooner.P.Y=this->frameCenter.Y-(height/2);
	LowerRigthCooner.P.Z=this->frameCenter.Z;

	// __
	//   |
	UpperRigthCooner.P.X=this->frameCenter.X+(wide/2);
	UpperRigthCooner.P.Y=this->frameCenter.Y+(height/2);
	UpperRigthCooner.P.Z=this->frameCenter.Z;

	// __
	//|
	UpperLeftCooner.P.X=this->frameCenter.X-(wide/2);
	UpperLeftCooner.P.Y=this->frameCenter.Y+(height/2);
	UpperLeftCooner.P.Z=this->frameCenter.Z;

}
void frame::Draw()
{
	glPushMatrix();
	glLineWidth(4);
	glLoadIdentity();	
	glColor3f(  0.5f,0.5f,1.0f );
	glBegin(GL_LINE_STRIP);						
			glVertex3f( LowerLeftCooner.P.X,  LowerLeftCooner.P.Y,  LowerLeftCooner.P.Z  );
			glVertex3f( LowerRigthCooner.P.X, LowerRigthCooner.P.Y, LowerRigthCooner.P.Z );
			glVertex3f( UpperRigthCooner.P.X, UpperRigthCooner.P.Y, UpperRigthCooner.P.Z );
			glVertex3f( UpperLeftCooner.P.X,  UpperLeftCooner.P.Y,  UpperLeftCooner.P.Z  );
			glVertex3f( LowerLeftCooner.P.X,  LowerLeftCooner.P.Y,  LowerLeftCooner.P.Z  );
    glEnd();	
	glLineWidth(1);
	glPopMatrix();
}


Scene::Scene(void)
{
	Point3D frameCenter;
	frameCenter.X=0;
	frameCenter.Y=0;
	frameCenter.Z=-7;
	gameFrame= new frame(6,3,frameCenter);
}
Scene::~Scene()
{
	if(gameFrame!=NULL)
		delete gameFrame;
}
void Scene::Draw()
{
	gameFrame->Draw();
}
Point3D Scene::getPoint3dFromGridCoord(int row,int col,int totalRows,int totalColums)
{
	Point3D p;
	float columnWidth=gameFrame->getWidth()/(float)totalColums;
	float RowHeight=gameFrame->getHeight()/(float)totalRows;
	p.X=gameFrame->UpperLeftCooner.P.X+ ((col*columnWidth)+(columnWidth/2));
	p.Y=gameFrame->UpperLeftCooner.P.Y- ((row*RowHeight)+(RowHeight/2));;
	p.Z=gameFrame->UpperLeftCooner.P.Z;
	return p;  
}
