#include "StdAfx.h"
#include <stdlib.h>     /* srand, rand */
#include "GamePuyoPuyo.h"

#include <time.h>       /* time */
GamePuyoPuyo::GamePuyoPuyo(void)
{
	gScene=NULL;
	status=STOP;
	srand (time(NULL));
	sphereGrid=NULL;
}
GamePuyoPuyo::~GamePuyoPuyo()
{

}
void GamePuyoPuyo::gameOver()
{
	status=GAME_OVER;// to end the game we change the game status
	ColorRGB c;
	c.Blue=0;
	c.Red=0;
	c.Green=0;
	this->sphereGrid->ChangeSphereColorGameOver(c);//change all the spheres with a new color BLACK
}
void GamePuyoPuyo::initGame(unsigned long int elapsedTime)
{
	gScene= new Scene();//create the scene, the game's frame
	status=PLAY;//with PLAY status estarts the game
	this->fPiece= new FallingPiece();//create the first falling piece
	this->fPiece->startFall();//start falling
	sphereGrid= new GridEsferas(NUM_ROWS,NUM_COLS);//we are going to use a grid to put spheres,
	this->elapsedTime=elapsedTime;
	startTime=elapsedTime;
	elapsedTimeHalfSecond=(elapsedTime-startTime)/500;//every half a second refresh the game
}
void GamePuyoPuyo::finish()
{
	if(gScene!=NULL)
	{
		delete gScene;
	}
	if(fPiece!=NULL)
		delete fPiece;
	if(sphereGrid!=NULL)
	{
		delete sphereGrid;
	}
}

//value return
//0-No collition
//1-sphere1
//2-sphere2
//3-both sphere1 && sphere2
int  GamePuyoPuyo::DetectCollition()
{	//check if there was a collition, and find out which sphere has collide
	int colS1=0;
	int colS2=0;
	int r=0,c=0; 
	if(this->fPiece->e1!=NULL)
	{
		r=this->fPiece->e1->getGridPos().row;
		c=this->fPiece->e1->getGridPos().column;
		if(sphereGrid->getSphereAt(r,c)!=NULL)
		{
			colS1=1;
		}
	}
	if(this->fPiece->e2!=NULL)
	{
		r=this->fPiece->e2->getGridPos().row;
		c=this->fPiece->e2->getGridPos().column;
		if(sphereGrid->getSphereAt(r,c)!=NULL)
		{
			colS2=1;
		}
	}
	if(colS1==1&&colS2==1)
	{
		return 3;
	}
	else
	{
		if(colS1==1)
		{
			return 1;
		}
		else
		{
			if(colS2==1)
			{
				return 2;
			}
			else
			{
				return 0;
			}

		}
	}	
}
void GamePuyoPuyo::StopSphereBottom(Sphere* e)
{//to freeze those spheres who gets the bottom 
	e->setScreenCoodinates(this->gScene->getPoint3dFromGridCoord(
		e->getGridPos().row,e->getGridPos().column,NUM_ROWS,NUM_COLS)
		);//we translate sphere grid  coordinates to screen coordinates
	this->sphereGrid->add(e,e->getGridPos().row,e->getGridPos().column);
}
void GamePuyoPuyo::StopSphere(Sphere* e)
{//stops an sphere in collition state
	e->setGridPos(e->getGridPos().row-1,e->getGridPos().column);
	e->setScreenCoodinates(this->gScene->getPoint3dFromGridCoord(
		e->getGridPos().row,e->getGridPos().column,NUM_ROWS,NUM_COLS)
		);//we translate sphere grid  coordinates to screen coordinates
	this->sphereGrid->add(e,e->getGridPos().row,e->getGridPos().column);
}
int  GamePuyoPuyo::checkIfRemoveSpheres()
{//check if there are some spheres to delete
	return sphereGrid->arethereSpheresToDelete()==4?1:0;
}
void GamePuyoPuyo::ReactionStopBottom()
{
	// after stop the sphere at the bottom, we take another actions
	if(fPiece->e1!=NULL)
	{
		StopSphereBottom(fPiece->e1);	//stop the sphere
		fPiece->e1=NULL;
	}	
	if(fPiece->e2!=NULL)
	{
		StopSphereBottom(fPiece->e2);	//stop the sphere
		fPiece->e2=NULL;
	}		

	if(fPiece->e1==NULL&&fPiece->e2==NULL)
		fPiece->stopFalling();	//stop falling the piece,both spheres have collide			

	if(checkIfRemoveSpheres()==1)//after stop the sphere check to remove
	{
		this->status=PLAY_REMOVING_SPHERES;//if there are some to remove, we change the game status
	}

}
void GamePuyoPuyo::ReactionToCollitionF(int rCollitionDectection)
{
	//action after a collition
	switch(rCollitionDectection)
	{
	case 1: StopSphere(fPiece->e1);	
		fPiece->e1=NULL;
		break;
	case 2:
		if(fPiece->getOrientation()==VERTICAL&&fPiece->e1!=NULL)
		{
			StopSphere(fPiece->e1);	
			fPiece->e1=NULL;
		}
		StopSphere(fPiece->e2);	
		fPiece->e2=NULL;
		break;
	case 3:
		StopSphere(fPiece->e1);	
		fPiece->e1=NULL;				
		StopSphere(fPiece->e2);	
		fPiece->e2=NULL;
		break;
	}
	if(fPiece->e1==NULL&&fPiece->e2==NULL)
		fPiece->stopFalling();				

	if(checkIfRemoveSpheres()==1)
	{
		this->status=PLAY_REMOVING_SPHERES;
	}

}
void GamePuyoPuyo::ReactionToCollitionRL(int rCollitionDectection)
{//rotate the piece to the Right, if somethign went wrong while was rotating to the left, undo the move
	this->fPiece->RotateR();
}
void GamePuyoPuyo::ReactionToCollitionRR(int rCollitionDectection)
{//rotate the piece to the LEFT, if somethign went wrong while was rotating to the Right, undo the move
	this->fPiece->RotateL();
}
void GamePuyoPuyo::ReactionToCollitionML(int rCollitionDectection)
{//move the piece to the RIGTH, if somethign went wrong while was moving to the left, undo the move
	this->fPiece->Move(RIGTH);
}
void GamePuyoPuyo::ReactionToCollitionMR(int rCollitionDectection)
{//move the piece to the LEFT, if somethign went wrong while was moving to the RIGTH, undo the move
	this->fPiece->Move(LEFT);
}
void GamePuyoPuyo::MovePieceToR()
{//try to move the piece to the Rigth
	if(this->fPiece!=NULL)
	{
		this->fPiece->Move(RIGTH);//move
		int colliDetectionVal=DetectCollition();//detect collition
		if(colliDetectionVal>0)
		{
			ReactionToCollitionMR(colliDetectionVal);//undo 
		}
	}
}
void GamePuyoPuyo::MovePieceToL()
{//try to move the piece to the left
	if(this->fPiece!=NULL)
	{
		this->fPiece->Move(LEFT);//move
		int colliDetectionVal=DetectCollition();////detect collition
		if(colliDetectionVal>0)
		{
			ReactionToCollitionML(colliDetectionVal);//undo 
		}	
	}
}
void GamePuyoPuyo::RotatePieceToL()
{//try to rotate the piece to the left
	if(this->fPiece!=NULL)
	{
		this->fPiece->RotateL();//rotate
		int colliDetectionVal=DetectCollition();//detect collition
		if(colliDetectionVal>0)
		{
			ReactionToCollitionMR(colliDetectionVal);//undo 
		}	
	}
}
void GamePuyoPuyo::RotatePieceToR()
{//try to rotate the piece to the right
	if(this->fPiece!=NULL)
	{
		this->fPiece->RotateR();//rotate
		int colliDetectionVal=DetectCollition();//detect collition
		if(colliDetectionVal>0)
		{
			ReactionToCollitionMR(colliDetectionVal);//undo 
		}	
	}
}
int  GamePuyoPuyo::isAtBottom(FallingPiece* fP)
{//check if the piece is at the bottom
	if(fP->getOrientation()==HORIZONTAL)
	{
		if(fP->e1!=NULL&&fP->e2!=NULL&&(fP->e1->getGridPos().row+1)==NUM_ROWS)
		{
			return 1;
		}
		if(fP->e1!=NULL&&(fP->e1->getGridPos().row+1)==NUM_ROWS)
		{
			return 1;
		}
		if(fP->e2!=NULL&&(fP->e2->getGridPos().row+1)==NUM_ROWS)
		{
			return 1;
		}
	}
	else
	{
		if(fP->e1!=NULL&&fP->e2!=NULL&&(fP->e2->getGridPos().row+1)==NUM_ROWS)
		{
			return 1;
		}
		if(fP->e1!=NULL&&(fP->e1->getGridPos().row+1)==NUM_ROWS)
		{
			return 1;
		}
		if(fP->e2!=NULL&&(fP->e2->getGridPos().row+1)==NUM_ROWS)
		{
			return 1;
		}
	}
	return 0;
}
void GamePuyoPuyo::UpdateElementsPositions()
{
	int colliDetectionVal=0;
	if(this->fPiece!=NULL)
	{// if there is an falling piece
		if(isAtBottom(this->fPiece)==0)//if it's not at the bottom
		{
			this->fPiece->Fall();//fall
			colliDetectionVal=DetectCollition();//detect collition
			if(colliDetectionVal>0)
			{
				ReactionToCollitionF(colliDetectionVal);//undo
			}
		}
		else
		{
			ReactionStopBottom();//
		}
		if(this->fPiece->IsFalling()==0)//is not falling
		{
			delete this->fPiece;//delete the piece
			this->fPiece=NULL;
		}
	}
	else
	{// crea a new piece
		this->fPiece= new FallingPiece();
		this->fPiece->startFall();	
		int colliDetectionVal=0;
		colliDetectionVal=DetectCollition();
		if(colliDetectionVal>0) //
		{//if we detect an collition at the beginning
			this->gameOver();//terminte the game
		}
	}

}
void GamePuyoPuyo::UdateSpheresAfterRemove()
{//refresh coordenates
	for(int i=0;i<NUM_ROWS;i++)
		for(int j=0;j<NUM_COLS;j++)
		{
			Sphere * e=	sphereGrid->getSphereAt(i,j);
			if(e!=NULL&&e->actualizarCordenadasDespuesDeEliminar==true)
			{
				StopSphereBottom(e);
				e->actualizarCordenadasDespuesDeEliminar=false;
			}
		}
}
void GamePuyoPuyo::removeSpheres()
{
	sphereGrid->removeSpheres();//remove spheres
	UdateSpheresAfterRemove();//update coordinates
	if(sphereGrid->arethereSpheresToDelete()!=4)
		this->status=PLAY;	//keep playing
}
void GamePuyoPuyo::updateGame(unsigned long int elapsedTimeOPenGL)
{
	//update the game, it depends on the status
	switch(status)
	{
	case GAME_OVER: return;
	case PAUSE:this->elapsedTime=elapsedTime;
	case STOP:this->elapsedTime=elapsedTime; return;
	case PLAY:
		if(((elapsedTimeOPenGL-startTime)/500)>elapsedTimeHalfSecond)
		{
			UpdateElementsPositions();
			elapsedTimeHalfSecond=(elapsedTimeOPenGL-startTime)/500;
		}
		break;	
	case PLAY_REMOVING_SPHERES:
		if(((elapsedTimeOPenGL-startTime)/500)>elapsedTimeHalfSecond)
		{
			removeSpheres();
			elapsedTimeHalfSecond=(elapsedTimeOPenGL-startTime)/500;
		}
		break;


	default: return ;
	}	
}
void GamePuyoPuyo::Draw()
{
	gScene->Draw();
	if(this->fPiece!=NULL)
	{
		if(this->fPiece->e1!=NULL)
		{
			this->fPiece->e1->setScreenCoodinates(
				this->gScene->getPoint3dFromGridCoord(
				this->fPiece->e1->getGridPos().row,this->fPiece->e1->getGridPos().column,NUM_ROWS,NUM_COLS));
		}
		if(this->fPiece->e2!=NULL)
		{
			this->fPiece->e2->setScreenCoodinates(
				this->gScene->getPoint3dFromGridCoord(
				this->fPiece->e2->getGridPos().row,this->fPiece->e2->getGridPos().column,NUM_ROWS,NUM_COLS));
		}
		this->fPiece->Draw();

	}
	sphereGrid->Draw();
}
