#pragma once
#include "Scene.h"
#include "GridEsferas.h"

#define NUM_COLS 6
#define NUM_ROWS 12
enum eMovJuego{MOV_L,MOV_R,ROT_R,ROT_L,FALL};
class GamePuyoPuyo:Drawable
{
private:
	Scene* gScene;
	GameStatus status;
	FallingPiece* fPiece;
	GridEsferas* sphereGrid;
	int DetectCollition();
	void ReactionToCollition(int rCollitionDectection,eMovJuego m);
	unsigned long int elapsedTimeHalfSecond;
	unsigned long int elapsedTime;
	unsigned long int startTime;
	void removeSpheres();
	void ReactionStopBottom();
	void ReactionToCollitionF(int rCollitionDectection);
	void ReactionToCollitionRL(int rCollitionDectection);
	void ReactionToCollitionRR(int rCollitionDectection);
	void ReactionToCollitionML(int rCollitionDectection);
	void ReactionToCollitionMR(int rCollitionDectection);
	void StopSphere(Sphere* e);
	void StopSphereBottom(Sphere* e);
	int checkIfRemoveSpheres();
	int isAtBottom(FallingPiece* fP);
	void gameOver();
	void UdateSpheresAfterRemove();
public:
	GamePuyoPuyo(void);
	~GamePuyoPuyo();
	void UpdateElementsPositions();
	void initGame(unsigned long int elapsedTime);
	void updateGame(unsigned long int elapsedTimeOPenGL/*1/10 sec*/);
	void Draw();
	void finish();
	void MovePieceToR();
	void MovePieceToL();
	void RotatePieceToL();
	void RotatePieceToR();

};

