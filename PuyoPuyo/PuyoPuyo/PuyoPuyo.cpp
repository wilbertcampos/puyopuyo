// PuyoPuyo.cpp : main project file.

#include "stdafx.h"

#include <stdlib.h>
#include "GamePuyoPuyo.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

static GamePuyoPuyo* game= NULL;
/*lights*/
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };
/**/
static void resize(int width, int height)
{//windows resize
    const float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}
static void display(void)
{
	
	unsigned long int elapsedTime= glutGet(GLUT_ELAPSED_TIME) ;    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;

	// when the game have been started
	game->updateGame(elapsedTime);//update de game elements
	game->Draw();	//draw everything we need

    glutSwapBuffers();
}
void special(int key, int x, int y)
{
  switch (key)
  {
  case GLUT_KEY_LEFT:   game->MovePieceToL();    break;
  case GLUT_KEY_RIGHT:  game->MovePieceToR();	 break;
  case GLUT_KEY_UP:     game->RotatePieceToR();  break;
  case GLUT_KEY_DOWN:   game->RotatePieceToL();	 break;
  default: break;
  }
  glutPostRedisplay();
}
static void key(unsigned char key, int x, int y)
{	 
    glutPostRedisplay();
}
static void idle(void)
{
    glutPostRedisplay();
}



void iniciaJuego()
{//starting the game
	unsigned long int elapsedTime= glutGet(GLUT_ELAPSED_TIME) ;
	if(game==NULL)
	{   //game object
		game=new GamePuyoPuyo();
	}
	game->initGame(elapsedTime);//call to initialize
}

void terminaEjecucion()
{
	//at exit, we clean all the resources 
	if(game!=NULL)
	{
		game->finish();
		delete game;
	}
}
void createWindow()
{
	glutInitWindowSize(320,240);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow(".:Poyo Poyo:.");
}
void inicializaOpenGL()
{
	createWindow();	//create the window	
	//------
	//link to spetials fucntions
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
	glutSpecialFunc(special);
    glutIdleFunc(idle);
	//-------

	//----enviroment configurations
    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	//-------------
    atexit(terminaEjecucion);
}
/* Program entry point */
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);    
	inicializaOpenGL();//initialazing opengl functions
	iniciaJuego();//we start de game
    glutMainLoop();
    return EXIT_SUCCESS;
}
