#include "StdAfx.h"
#include <vector>
using namespace std;
#include "GridEsferas.h"


GridEsferas::GridEsferas(int numRows,int numcols)
{
	this->numcols=numcols;
	this->numRows=numRows;
	elementos=NULL;
	//create an "matrix" with vectors arrays
	elementos = new vector<vector<Sphere*>*>(numRows,NULL);	
	for(int i=0;i<numRows;i++)
	{
		vector<Sphere*> *tmp= new vector<Sphere*>(numcols,NULL);	
		(*elementos)[i]=tmp;
	}
	links= new vector<Sphere*>();

}
void GridEsferas::ChangeSphereColorGameOver(ColorRGB colorC)
{
	for (unsigned n = 0; n < elementos->size(); n++)
	{
		vector<Sphere*> *tmp= elementos->at(n);
		if(tmp!=NULL)
		{
			for (unsigned k = 0; k < tmp->size(); k++)
			{
				Sphere *e= tmp->at(k);
				if(e!=NULL)			 
				{
					e->changeRGBColor(colorC);
				}
			}
		}
	}
}
//get an sphere at determinated position
Sphere * GridEsferas::getSphereAt(int Row,int Col)
{
	Sphere* etemp=NULL;
	if(Row<elementos->size())
	{
		vector<Sphere*> *tmp=elementos->at(Row);
		if(tmp!=NULL)
		{
			if(Col<tmp->size())
				etemp=(*tmp)[Col];
		}
	}
	return etemp;
}
GridEsferas::~GridEsferas(void)
{
}
int GridEsferas::add(Sphere * esfera,int row,int column)
{//add sphete to the grid
	Sphere* etemp=NULL;
	vector<Sphere*> *tmp=elementos->at(row);
	if(tmp!=NULL)
	{
		(*tmp)[column]=esfera;
	}
	return 1;
}
Sphere * GridEsferas::getNeighborAt(int row,int column,ColorRGB colorC)
{
	bool bSameColor=true;
	if((row>=0&&row<12)&&(column>=0&&column<6))
	{
		Sphere *e=getSphereAt(row,column);
		if(e!=NULL)
		{
			bSameColor=bSameColor&&(e->getRGBColor().Blue==colorC.Blue);
			bSameColor=bSameColor&&(e->getRGBColor().Green==colorC.Green);
			bSameColor=bSameColor&&(e->getRGBColor().Red==colorC.Red);
			if(bSameColor==true)
				return e;
			else
				return NULL;
		}
		else
		{
			return NULL;
		}	
	}
	else
	{
		return NULL;
	}
}
void GridEsferas::checNeightBorg(Sphere* raiz,int level,vector<Sphere*>* linksl,NOT_INCLUDE no_include )
{//cheks what neigthbor has the same color
	//      N1
	//  N4  X  N2
	//      N3
	//N1 neightbor 1
	//N2 neightbor 2
	//N3 neightbor 3
	//N4 neightbor 4
	//X ROOT
	Sphere *N1=NULL,*N2=NULL,*N3=NULL,*N4=NULL;
	int nextLevel=level+1;
	int row=-1;
	int column=-1;
	if(level<4)
	{
		row=raiz->getGridPos().row;
		column=raiz->getGridPos().column;


		if(linksl->size()<4)
		{
			if(no_include!=N_UP)
			{
				N1=getNeighborAt(row-1,column,raiz->getRGBColor());
				if(N1!=NULL)
				{			
					linksl->push_back(N1);
					if(linksl->size()<4)
					{
						checNeightBorg(N1,nextLevel,links,N_DOWN);
					}
				}
			}

			if(linksl->size()<4&&no_include!=N_RIGHT)
			{
				N2=getNeighborAt(row,column+1,raiz->getRGBColor());
				if(N2!=NULL)
				{			
					linksl->push_back(N2);
					if(linksl->size()<4)
					{
						checNeightBorg(N2,nextLevel,linksl,N_LEFT);
					}
				}			
			}
			if(linksl->size()<4&&no_include!=N_DOWN)
			{
				N3=getNeighborAt(row+1,column,raiz->getRGBColor());
				if(N3!=NULL)
				{			
					linksl->push_back(N3);
					if(linksl->size()<4)
					{
						checNeightBorg(N3,nextLevel,linksl,N_UP);
					}
				}
			}
			if(linksl->size()<4&&no_include!=N_LEFT)
			{
				N4=getNeighborAt(row,column-1,raiz->getRGBColor());
				if(N4!=NULL)
				{			
					linksl->push_back(N4);
					if(linksl->size()<4)
					{
						checNeightBorg(N4,nextLevel,linksl,N_RIGHT);
					}
				}
			}
		}

	}
}
void GridEsferas::markToDelete(vector<Sphere*>* links)
{//mark spheres to delete
	for (unsigned k = 0; k < links->size(); k++)
	{
		Sphere *e= links->at(k);
		if(e!=NULL)			 
		{
			e->canDelete=true;
		}
	}
}
int GridEsferas::arethereSpheresToDelete()
{
	int numSpheresToDelete=0;
	for (unsigned n = 0; n < elementos->size(); n++)
	{
		vector<Sphere*> *tmp= elementos->at(n);
		if(tmp!=NULL)
		{
			for (unsigned k = 0; k < tmp->size(); k++)
			{
				Sphere *e= tmp->at(k);
				if(e!=NULL)			 
				{						
					links->clear();
					links->push_back(e);
					checNeightBorg(e,0,links,N_NONE);				 
					numSpheresToDelete=links->size();
					if(numSpheresToDelete==4)
					{
						markToDelete(links);
						n = elementos->size();
						k = tmp->size();
					}					
					else
					{
						links->clear();
					}
				}
			}
		}
	}
	return numSpheresToDelete;	
}
void GridEsferas::deleteSphere(Sphere * e)
{//delete marked spheres
	int row=e->getGridPos().row;
	int column=e->getGridPos().column;	
	setNULL(e->getGridPos().row,e->getGridPos().column);
	for(int i=row;i>0;i--)
	{		
		Sphere * etmp=NULL;

		etmp=this->getSphereAt(i-1,column);
		if(etmp!=NULL)
		{
			this->add(etmp,i,column);	
			etmp->setGridPos(i,column);
			etmp->actualizarCordenadasDespuesDeEliminar=true;
			setNULL(i-1,column);
		}
		else
			break;
	}
	delete e;
}
void GridEsferas::setNULL(int row,int col)
{// set null at some row and col
	Sphere* etemp=NULL;
	if(row>=0&&row<elementos->size())
	{
		vector<Sphere*> *tmp=elementos->at(row);
		if(tmp!=NULL)
		{
			if(col>=0&&col<tmp->size())
				(*tmp)[col]=NULL;
		}
	}
}
void GridEsferas::removeSpheres()	
{

	for (unsigned k = 0; k < links->size(); k++)
	{
		Sphere *e= links->at(k);
		if(e!=NULL&&e->canDelete==true)			 
		{

			deleteSphere(e);		
		}
	}
	links->clear();		

}
void GridEsferas::Draw()
{

	for (unsigned n = 0; n < elementos->size(); n++)
	{
		vector<Sphere*> *tmp= elementos->at(n);
		if(tmp!=NULL)
		{
			for (unsigned k = 0; k < tmp->size(); k++)
			{
				Sphere *e= tmp->at(k);
				if(e!=NULL)			 
				{
					e->Draw();
				}
			}
		}
	}

}