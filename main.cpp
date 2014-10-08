#include "glui.h"
#include <GL/glut.h>
#include <math.h>
#include <windows.h>
#include "ThreadedSim.h"
#include <iostream>
#include <time.h>

extern GLsizei windoww,windowh;

int   main_window;

bool mouse_rbutton=false,mouse_lbutton=false,mouse_mbutton=false;
int mouse_xPos,mouse_yPos;
int oldx,oldy;
int xmove=0,ymove=0;
clock_t start, end, eTime;

bool simul_pause = true;
int show_box = 1;
int show_spheres = 1;
int show_planes = 0;
int thread_type = 0;

float ball_rad = 1.0;
float ball_mass = 1.0;
int sim_div = 15;
float sim_time = 0.1;
float sim_timer = 50;
int sim_hard = 5;

int numFrames = 0;
float total = 0;
float curFPS = 0.0;

GLUI_Button *sourcestopbutton,*pausedbutton;

int timer;

extern void CALLBACK Animate(int i=0);
extern void CALLBACK ResizeWindow(GLsizei w, GLsizei h);
extern void CALLBACK rotateview ( int x,int y );
extern void CALLBACK moveview ( int x,int y );
extern void CALLBACK pressbutton ();
extern void CALLBACK hightview ( int x,int y );
extern void CALLBACK pressbutton2 ();
extern void CALLBACK pressbutton3 ();
//extern void SaveAnimation(HWND hwnd,char *fname,int w,int h);
extern void main3D( void );

// Global Variables:
#define			MAX_LOADSTRING		100
#define			IGNITE_ID			200
#define			RECORDMOVIEAVI_ID	201
#define			RECORDSTOP_ID		202
#define			SOURCESTART_ID		203
#define			SOURCESTOP_ID		204
#define			PAUSE_ID			205
#define			OBJWRITE_ID			206
#define			RESTART_ID			207
#define			THREAD_ID			208
#define			ADD_BALL_ID			209
#define			TIMER_ID			210

/*Ball b[10];

int numBalls = 10;

Plane planes[6];

int numPlanes = 6;

Plane p1(vector3(0.0,-10.0,0.0),vector3(0.0,1.0,0.0));
Plane p2(vector3(0.0,10.0,0.0),vector3(0.0,-1.0,0.0));
Plane p3(vector3(-10.0,0.,0.0),vector3(1.0,0.0,0.0));
Plane p4(vector3(10.0,0.0,0.0),vector3(-1.0,0.0,0.0));
Plane p5(vector3(0.0,0.0,-10.0),vector3(0.0,0.0,1.0));
Plane p6(vector3(0.0,0.0,10.0),vector3(0.0,0.0,-1.0));
*/

ThreadedSim* tsim;

void myTimerFunc(int value)
{

  //static char buffer[255];
  if ( glutGetWindow() != main_window ) 
    glutSetWindow(main_window);  

	if (/*(!mouse_rbutton)&&(!mouse_lbutton)&&(!mouse_mbutton)&&*/(!simul_pause)){
/*
		if ((restart>0)&&(time>0.2)){
			restart--;
			control_cb(RESTART_ID);
		}
	
		time=Simulate(generate_solid||generate_solid_once);
		generate_solid_once=false;
		strcpy(buffer,"Simulation at    .    seconds");
		int timeint=time;
		if (time>=100.0) buffer[14]=(timeint/100)%10+'0';
		if (time>=10.0) buffer[15]=(timeint/10)%10+'0';
		buffer[16]=(timeint%10)+'0';
		timeint=time*10;
		buffer[18]=(timeint%10)+'0';
		timeint=time*100;
		buffer[19]=(timeint%10)+'0';
		timeint=time*1000;
		buffer[20]=(timeint%10)+'0';
		glutSetWindowTitle(buffer);
		if (time>bigbang){
			if (record_avi){
				record_movie=0;
				record_avi=0;
				recorder.EndCapture();  // Process and save the video file.
				if (split_record)
					recorder2.EndCapture();
			}
			exit(0);
		} */

		/*for(int i=0; i<numBalls; i++)
			b[i].Step(0.2, 1);


		for(int i=0; i<numBalls; i++)
			for(int j=0; j<numPlanes; j++)
				b[i].Collides(planes[j]);

		for(int i=0; i<numBalls-1; i++)
			for(int j=i+1; j<numBalls; j++)
				b[i].Collides(b[j],0.2,15);
		*/
		if(thread_type == 0)
		{
			start = clock();
			tsim->SimStep();
			tsim->BallCols();
			tsim->PlaneCols();
			end = clock();
			eTime = end-start;
		}
		else if(thread_type == 1)
		{		
			tsim->StepPThread();
		}
		else if(thread_type == 2)
		{
			tsim->runStepThreads();
			tsim->runPlaneThreads();
			tsim->runBallThreads();
		}

	}
	else if (simul_pause)
		Sleep(20);

  glutPostRedisplay();
  glutTimerFunc(1000/sim_timer, myTimerFunc, timer);
}

void myGlutReshape( int x, int y )
{
	ResizeWindow(x,y);
	glutPostRedisplay();
}

void SetTextMode()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, 700, 0, 700);
		glScalef(1.0, -1.0, 1.0);
		glTranslatef(0, -700, 0);
		glMatrixMode(GL_MODELVIEW);
}

void ResetDrawMode()
{
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void renderBitmapString(float x, float y, void *font, string s)
{
	int x0 = x;
	for(int i=0; i<s.length(); i++) {
		glRasterPos2f(x0, y);
		glutBitmapCharacter(font, s[i]);
		x0 += glutBitmapWidth(font, s[i]);
	}
}


void myGlutDisplay( void )
{
	ResizeWindow(windoww,windowh);
	/*char fname[255];
	if (split_record)
		Animate(2);
	else
		Animate();*/

	Animate();

		//Get Display info
	char* buf = new char[10];
	itoa(eTime, buf, 10);
	string totalTime = "Sim Time: ";
	totalTime.append(buf);

	//Display Info
	SetTextMode();
	glPushMatrix();
		glLoadIdentity();
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 1.0);
		renderBitmapString(20, 680, GLUT_BITMAP_HELVETICA_18, totalTime);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	ResetDrawMode();


    glutSwapBuffers(); 
	/*if (record_movie)
		if (!record_avi)
			SaveAnimation(mywin,"test",windoww,windowh);
		else {
			recorder.Snap();      // Capture a frame
			if (split_record){
				Animate(1);
				glutSwapBuffers(); 
				recorder2.Snap();
			}
		}*/
}

void myGlutMouse(int button, int button_state, int x, int y )
{
	if (button_state==GLUT_DOWN){
		if ( button == GLUT_LEFT_BUTTON ){
				mouse_lbutton=true; 
				oldx=mouse_xPos = x;   
				oldy=mouse_yPos = y;
				pressbutton();
				glutPostRedisplay();
		}
		if ( button == GLUT_RIGHT_BUTTON ){
				mouse_rbutton=true;
				mouse_xPos = x;   
				mouse_yPos = y;  
				pressbutton2();
		}
		if ( button == GLUT_MIDDLE_BUTTON ){
				mouse_mbutton=true;
				mouse_xPos = x;   
				mouse_yPos = y;  
		}
	}
	else if ( button_state == GLUT_UP ){
		if ( button == GLUT_RIGHT_BUTTON )
				mouse_rbutton=false;
		if ( button == GLUT_LEFT_BUTTON )
				mouse_lbutton=false;
		if ( button == GLUT_MIDDLE_BUTTON ){
				mouse_mbutton=false;
				pressbutton3();
		}
	}
}


void myGlutMotion(int x, int y )
{
	int dummy;
	if (mouse_rbutton){
		dummy = mouse_xPos - x;   
		xmove=dummy;
		dummy = mouse_yPos - y;  
		ymove=dummy;
		oldx=mouse_xPos = x;   
		oldy=mouse_yPos = y;  
		x=mouse_xPos*20/windoww;
		y=(float)(windowh-mouse_yPos-10000.0/(float)windowh)*(float)20/((float)windowh-50.0);
		hightview(mouse_xPos,mouse_yPos);
		glutPostRedisplay();
	}
	else 
	if (mouse_lbutton){
		mouse_xPos = x;   
		mouse_yPos = y;
		oldx=mouse_xPos;
		oldy=mouse_yPos;
		x=mouse_xPos*20/windoww;
		y=(float)(windowh-mouse_yPos-10000.0/(float)windowh)*(float)20/((float)windowh-50.0);
		rotateview(mouse_xPos,mouse_yPos);
		glutPostRedisplay();
	}
	else 
	if (mouse_mbutton){
		dummy = mouse_xPos - x;   
		xmove+=dummy;
		dummy = mouse_yPos - y;  
		ymove+=dummy;
		oldx=mouse_xPos = x;   
		oldy=mouse_yPos = y;  
		moveview(xmove,ymove);
		glutPostRedisplay();
	}
}

void control_cb( int control )
{
	switch (control){
	/*case RECORDMOVIEAVI_ID:
		if (!record_movie){
		recorder.StartCapture("mymovie.avi",windoww,windowh);  // Initialize
		if (split_record)
			recorder2.StartCapture("mymovie2.avi",windoww,windowh);  // Initialize
		record_movie=1;
		record_avi=1;
		}
		break;
	case RECORDSTOP_ID:
		if (record_avi){
			record_movie=0;
			record_avi=0;
			recorder.EndCapture();  // Process and save the video file.
			if (split_record)
				recorder2.EndCapture();
		}
		break;*/
	case PAUSE_ID:
		simul_pause=!simul_pause;
		pausedbutton->set_name((simul_pause)? "Continue":"Pause");
		break;
	/*case OBJWRITE_ID:
		objoutput=true;
		break;*/
	case RESTART_ID:
		sim_time  = 0.2;
		sim_hard = 5;
		sim_div = 10;
		tsim->Reset(5, sim_time, sim_hard, sim_div, thread_type);
		simul_pause = true;
		break;
	case THREAD_ID:
		tsim->setThreadModel(thread_type);
		break;
	case ADD_BALL_ID:
		tsim->AddBall(ball_rad, ball_mass);
		break;
	case TIMER_ID:
		tsim->setTime(sim_time);
		tsim->setExtra(sim_hard);
		tsim->setIters(sim_div);
		break;
	default:
		break;
  }
}

void main(int argv,char** argc)
{
	/*planes[0] = p1;
	planes[1] = p2;
	planes[2] = p3;
	planes[3] = p4;
	planes[4] = p5;
	planes[5] = p6;

	for(int i=0; i<numBalls; i++)
	{
		vector3 randstart(20.0*rand()/RAND_MAX-10.0,20.0*rand()/RAND_MAX-10.0,20.0*rand()/RAND_MAX-10.0);
		vector3 randvel((float)rand()/RAND_MAX,(float)rand()/RAND_MAX,(float)rand()/RAND_MAX);
		Ball temp(randstart, 1.0, 1.0, randvel, vector3(0,0,0));
		b[i] = temp;
	}*/

	tsim = new ThreadedSim();

  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowPosition( 50, 50 );
  glutInitWindowSize(640, 480);
  
  main3D();

  main_window = glutCreateWindow( "Multithreaded physics" );

  glutDisplayFunc( myGlutDisplay );
  glutReshapeFunc( myGlutReshape );  
  //glutIdleFunc(myIdle);
  GLUI_Master.set_glutMouseFunc( myGlutMouse );
  glutMotionFunc( myGlutMotion );

  /****************************************/
  /*         Here's the GLUI code         */
  /****************************************/
  
  GLUI *glui = GLUI_Master.create_glui( "Control Panel" );

  GLUI_Rollout *display = glui->add_rollout( "Display", true );
  display->set_w(200);

  /*GLUI_Rollout *active = glui->add_rollout_to_panel( display,"Active", true );
  active->set_w(200);
  glui->add_checkbox_to_panel( active, "Polygon", &show_active_polygon);
  glui->add_checkbox_to_panel( active, "Levelset", &show_active_levelset);
  glui->add_checkbox_to_panel( active, "Vertices", &show_active_vertices);
  glui->add_checkbox_to_panel( active, "HotVertices", &show_active_hotvertices);
	*/

  glui->add_checkbox_to_panel( display, "Spheres", &show_spheres);
  glui->add_checkbox_to_panel( display, "Planes", &show_planes );
  glui->add_checkbox_to_panel( display, "Bounding Box", &show_box);

  GLUI_Panel *timecontrol = glui->add_panel("Timer Controls",true);

  glui->add_spinner_to_panel(timecontrol, "Sim Time", GLUI_SPINNER_FLOAT, &sim_time, TIMER_ID, control_cb);
  glui->add_spinner_to_panel(timecontrol, "Time Div", GLUI_SPINNER_INT, &sim_div, TIMER_ID, control_cb);
  glui->add_spinner_to_panel(timecontrol, "Timer FPS", GLUI_SPINNER_FLOAT, &sim_timer, TIMER_ID, control_cb);
  glui->add_spinner_to_panel(timecontrol, "Loops (x1000)", GLUI_SPINNER_INT, &sim_hard, TIMER_ID, control_cb);
 
  GLUI_Rollout *tModel = glui->add_rollout("Thread Model", true);
  tModel->set_w(200);

  GLUI_RadioGroup *tRad = glui->add_radiogroup_to_panel(tModel,&thread_type,THREAD_ID,control_cb);
  
  glui->add_radiobutton_to_group(tRad, "Single Thread");
  glui->add_radiobutton_to_group(tRad, "Physics Thread");
  glui->add_radiobutton_to_group(tRad, "Per Object");

  GLUI_Panel *addpanel = glui->add_panel("Add Ball",true);
  GLUI_Spinner *rad_spinner = glui->add_spinner_to_panel(addpanel,"Radius",GLUI_SPINNER_FLOAT,&ball_rad);
  rad_spinner->set_float_limits(0, 5.0);
  rad_spinner->set_alignment(GLUI_ALIGN_RIGHT);

  GLUI_Spinner *mass_spinner = glui->add_spinner_to_panel(addpanel,"Mass",GLUI_SPINNER_FLOAT,&ball_mass);
  rad_spinner->set_float_limits(0, 10.0);
  rad_spinner->set_alignment(GLUI_ALIGN_RIGHT);
  
  glui->add_button_to_panel(addpanel,"Add", ADD_BALL_ID, control_cb );

  pausedbutton=glui->add_button("Pause", PAUSE_ID, control_cb );
  

  /*
  GLUI_Rollout *params = glui->add_rollout( "Parameters", true );
  params->set_w(200);
  glui->add_checkbox_to_panel( params, "BumpDent", &bumpdent);
  GLUI_Spinner *diffusion_spinner = glui->add_spinner_to_panel( params, "Diffusion", GLUI_SPINNER_FLOAT, &diffusion );
  diffusion_spinner->set_float_limits( 0, 10.0 );
  diffusion_spinner->set_alignment( GLUI_ALIGN_RIGHT );
  glui->add_checkbox_to_panel( params, "Projection", &projection);
//  glui->add_checkbox_to_panel( params, "Projection2", &projection_fish);
  GLUI_Spinner *dissipation_spinner = glui->add_spinner_to_panel( params, "Dissipation", GLUI_SPINNER_FLOAT, &dissipation );
  dissipation_spinner->set_float_limits( 0, 1000000.0 );
  dissipation_spinner->set_alignment( GLUI_ALIGN_RIGHT );
  GLUI_Spinner *SIM_spinner1 = glui->add_spinner_to_panel( params, "Bump In", GLUI_SPINNER_FLOAT, &SIM );
  SIM_spinner1->set_float_limits( -1000.0, 100000.0 );
  SIM_spinner1->set_alignment( GLUI_ALIGN_RIGHT );
  GLUI_Spinner *SIM_spinner2 = glui->add_spinner_to_panel( params, "Bump Out", GLUI_SPINNER_FLOAT, &SIM2 );
  SIM_spinner2->set_float_limits( -1000.0, 100000.0 );
  SIM_spinner2->set_alignment( GLUI_ALIGN_RIGHT );
  glui->add_checkbox_to_panel( params, "GlobalFlow", &globalflow);
  GLUI_Spinner *FSCALE_spinner = glui->add_spinner_to_panel( params, "FlowScale", GLUI_SPINNER_FLOAT, &flowscale );
//  FSCALE_spinner->set_float_limits( -10.0, 10.0 );
  FSCALE_spinner->set_alignment( GLUI_ALIGN_RIGHT );
  GLUI_Spinner *ISCALE_spinner = glui->add_spinner_to_panel( params, "ImpulseScale", GLUI_SPINNER_FLOAT, &impulsescale );
//  ISCALE_spinner->set_float_limits( -10.0, 10.0 );
  ISCALE_spinner->set_alignment( GLUI_ALIGN_RIGHT );
  GLUI_Spinner *TSCALE_spinner = glui->add_spinner_to_panel( params, "TorqueScale", GLUI_SPINNER_FLOAT, &torquescale );
//  ISCALE_spinner->set_float_limits( -10.0, 10.0 );
  TSCALE_spinner->set_alignment( GLUI_ALIGN_RIGHT );
  GLUI_Spinner *fadeout_spinner = glui->add_spinner_to_panel( params, "Fadeout", GLUI_SPINNER_FLOAT, &fadeout );
//  ISCALE_spinner->set_float_limits( -10.0, 10.0 );
  fadeout_spinner->set_alignment( GLUI_ALIGN_RIGHT );

  GLUI_Rollout *recorder = glui->add_rollout( "Movie Record", true );
  recorder->set_w(200);
  glui->add_button_to_panel( recorder, "Record", RECORDMOVIEAVI_ID, control_cb );
  glui->add_button_to_panel( recorder, "Stop", RECORDSTOP_ID, control_cb );

  glui->add_separator();
  glui->add_button( "ObjWrite", OBJWRITE_ID, control_cb );*/
  glui->add_separator();
  glui->add_button( "Restart", RESTART_ID, control_cb );
  glui->add_button( "Quit", 0,(GLUI_Update_CB)exit );

  glui->set_main_gfx_window( main_window );


  glutTimerFunc(1000/sim_timer, myTimerFunc, timer);

  /* We register the idle callback with GLUI, *not* with GLUT */
  GLUI_Master.set_glutIdleFunc(NULL);
  
  
  glutMainLoop();
}
