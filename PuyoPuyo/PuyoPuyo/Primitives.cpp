#include "StdAfx.h"
#include <stdlib.h>     /* srand, rand */
#include "Primitives.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <time.h>       /* time */

Sphere::Sphere()
{
	this->EColor=getRandomSphereColor();
	this->radius=DEFAULT_RADIUS_SIZE;
	this->RGBColor=getRGBColorFromEnumColor(EColor);	
	CenterScreenCord=NULL;
	canDelete=false;
	actualizarCordenadasDespuesDeEliminar=false;
}
void Sphere::changeRGBColor(ColorRGB	RGBColorC)
{
	RGBColor=RGBColorC;
}
ColorRGB Sphere::getRGBColor()
{
	return RGBColor;
}
ColorRGB Sphere::getRGBColorFromEnumColor(EnumSphereColor ecolor)
{
	ColorRGB rgbColor;
	rgbColor.Red=0;
	rgbColor.Blue=0;
	rgbColor.Green=0;
	switch(ecolor)
	{
		case E_GREEN:  rgbColor.Green=1.0;break;	
		case E_RED:    rgbColor.Red=1.0;break;	
		case E_BLUE:   rgbColor.Blue=1.0;break;
		case E_YELLOW: rgbColor.Red=1.0;rgbColor.Green=1.0;break;						
	}
	return rgbColor;
}
EnumSphereColor Sphere::getRandomSphereColor()
{
	EnumSphereColor color;
	switch (rand()%4)
	{
	case 0:color =E_RED; break;
	case 1:color =E_GREEN;break;
	case 2:color =E_BLUE; break;
	case 3:color =E_YELLOW;break;
	default: color=E_GREEN;
	}
	return color;
}
void Sphere::Move(DIRECTION d)
{
	if(d==LEFT)
		this->gridPos.column--;
	else
		this->gridPos.column++;//right
}
void Sphere::Draw()
{
	glColor4d(RGBColor.Red,RGBColor.Green,RGBColor.Blue,0.9);
    glPushMatrix();
		GLfloat x=this->CenterScreenCord->X;
        GLfloat y=this->CenterScreenCord->Y;
        GLfloat z=this->CenterScreenCord->Z;
        glTranslated(x,y,z);		
		glutSolidSphere(this->radius,16,16);
    glPopMatrix();    
}
Point3D Sphere::getCenterScreenCord()
{
	Point3D p;
	if(this->CenterScreenCord!=NULL)
	{
		p.X=this->CenterScreenCord->X;
		p.Y=this->CenterScreenCord->Y;
		p.Z=this->CenterScreenCord->Z;
	}
	else
	{
		p.X=0;
		p.Y=0;
		p.Z=0;
	}

	return p;
}
void Sphere::setScreenCoodinates(Point3D p)
{
	if(this->CenterScreenCord==NULL)
	{
		this->CenterScreenCord=new Point3D();
		this->CenterScreenCord->X=p.X;
		this->CenterScreenCord->Y=p.Y;
		this->CenterScreenCord->Z=p.Z;
	}
	else
	{
		this->CenterScreenCord->X=p.X;
		this->CenterScreenCord->Y=p.Y;
		this->CenterScreenCord->Z=p.Z;
	}
}
void Sphere::setGridPos(int row,int col)
{
	this->gridPos.column=col;
	this->gridPos.row=row;
}
GridPosition2D Sphere::getGridPos()
{
	return this->gridPos;
}

Orientation FallingPiece::getRandomOrientation()
{
	if((rand()%2)==0)
		return HORIZONTAL;	
	else
		return VERTICAL;
}
int FallingPiece::getRandomGridCol()
{
	int randcol=rand()%6;
	return randcol;
}
void FallingPiece::createSpheres()
{	
	e1 = new Sphere();	
	e2 = new Sphere();	
	int randStartCol=getRandomGridCol();
	if(this->orientation==HORIZONTAL)
	{		
		if(randStartCol==5)
			randStartCol--;
		e1->setGridPos(0,randStartCol);
		e2->setGridPos(0,randStartCol+1);
	}
	else
	{//it's orientation is vertical		
		e1->setGridPos(0,randStartCol);
		e2->setGridPos(1,randStartCol);
	}
}
FallingPiece::FallingPiece()	
{
	e1 = NULL;	
	e2 = NULL;	
	orientation=getRandomOrientation();
	createSpheres();
	isFalling=0;
}
void FallingPiece::startFall()
{
	isFalling=1;
}
void FallingPiece::Fall()
{
	int r=0,c=0;
	if(e1!=NULL)
	{
		r=e1->getGridPos().row;
		c=e1->getGridPos().column;
		e1->setGridPos(r+1,c);
	}
	if(e2!=NULL)
	{
		r=e2->getGridPos().row;
		c=e2->getGridPos().column;
		e2->setGridPos(r+1,c);
	}
}
void FallingPiece::Move(DIRECTION d)
{
	//Sphere* e1=fallingSpheres->at(0);
	//Sphere* e2=fallingSpheres->at(1);
	if(e1!=NULL&&e2!=NULL)
	{
		if(this->orientation==HORIZONTAL)
		{
			if(d==LEFT)
			{
				if(e1->getGridPos().column>0)
				{
					e1->Move(d);
					e2->Move(d);
				}
			}		
			else
			{//right
				if(e1->getGridPos().column<4)
				{
					e1->Move(d);
					e2->Move(d);
				}
			}
		}
		else
		{//vertical
			if(d==LEFT)
			{
				if(e1->getGridPos().column>0)
				{
					e1->Move(d);
					e2->Move(d);
				}
			}		
			else
			{//right
				if(e1->getGridPos().column<5)
				{
					e1->Move(d);
					e2->Move(d);
				}
			}
		}
	}
	else
	{
		if(e1!=NULL&&
			        (
					 (d==LEFT&&e1->getGridPos().column>0)||
					 (d==RIGTH&&e1->getGridPos().column<5)
					))
		{
			e1->Move(d);
		}
		if(e2!=NULL&&
			        (
					 (d==LEFT&&e2->getGridPos().column>0)||
					 (d==RIGTH&&e2->getGridPos().column<5)
					))
		{
			e2->Move(d);
		}		
	}
	
}
int FallingPiece::CanRotateR()
{
	if(e1!=NULL&&e2!=NULL)
	{
		if(this->orientation==VERTICAL)		
		{
			if(e1->getGridPos().column!=0)
				return 1;
			else
				return 0;
		}
		else
		{
			if(e1->getGridPos().row==12)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}

	}
	else
	{
		return 0;
	}
}
void FallingPiece::RotateR()
{
	//Sphere* e1=fallingSpheres->at(0);
	//Sphere* e2=fallingSpheres->at(1);
	if(CanRotateR()==1)//it means sphere1 and sphere2 both are not null
	{		
		GridPosition2D posE1=e1->getGridPos();
		GridPosition2D posE2=e2->getGridPos();
		if(this->orientation==HORIZONTAL)
		{ //   -- => |			
			e2->setGridPos(posE1.row+1,posE1.column);
			this->orientation=VERTICAL;
		}
		else
		{//VERTICAL    | => --			
			e2->setGridPos(posE1.row,posE1.column-1);
			this->orientation=HORIZONTAL;
			changeOrden();
		}
	}
}
int FallingPiece::CanRotateL()
{//only checks with the border
	if(e1!=NULL&&e2!=NULL)
	{
		if(this->orientation==VERTICAL)
		{
			if(e1->getGridPos().column==5)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{//horizontal
			if(e1->getGridPos().row==12)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}
	else
	{
		return 0;
	}
}
void FallingPiece::changeOrden()
{
	//the orden has changed
	Sphere* temp=e1;
	e1=e2;
	e2=temp;
}
void FallingPiece::RotateL()
{
	//Sphere* e1=fallingSpheres->at(0);
	//Sphere* e2=fallingSpheres->at(1);
	if(CanRotateL()==1)//it means sphere1 and sphere2 both are not null
	{
		GridPosition2D posE1=e1->getGridPos();
		GridPosition2D posE2=e2->getGridPos();
		if(this->orientation==HORIZONTAL)
		{//   -- => |			
			e1->setGridPos(posE2.row+1,posE2.column);
			this->orientation=VERTICAL;
			changeOrden();
		}
		else
		{//vertical | => --			
			e2->setGridPos(posE1.row,posE1.column+1);
			this->orientation=HORIZONTAL;
			
		}
	}	
}
void FallingPiece::stopFalling()
{
	isFalling=0;
}
int FallingPiece::IsFalling()
{
	return isFalling;
}	
Orientation FallingPiece::getOrientation()
{
	return this->orientation;
}
void FallingPiece::Draw()
{
	if(isFalling==1)
	{
		if(e1!=NULL)
		{
			e1->Draw();
		}
		if(e2!=NULL)
		{
			e2->Draw();
		}
	}

}
