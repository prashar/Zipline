/* (c) Neeraj Prashar - 2010 - EECE478/Term2 */ 

#include <GLUT/glut.h> 
#include <cstdlib> 
#include <cmath>
#include <vector>

#include <iostream>
#include <sstream>
#include <fstream>

#include "vec.h"
using namespace std ; 
#include "fileparser.h"


#define c_winheight 600 
#define c_winwidth 800 
#define fovy 60 
#define near .1 
#define far 1000
#define N_ROWS 100 
#define N_COLS 100

// Define global variable to capture rotation
GLfloat g_theta[3] = {0.0,0.0,0.0} ; 
// By default rotate around y
int g_axis_to_rotate = 1 ; 
// Angle of camera rotation. 
float old_x=0,old_y=0,rot_to_x=0,rot_to_y=0 ; 
// Implement zoom 
float zfactor=1.0; 
// File parser
FileParser parser ; 
// Object rotation
float x_obj_rot=0.0,y_obj_rot=0.0,variable=0.0 ; 
char * fname ; 

// OUTPUT: renders string at raster position x,y
void renderString( float x, float y,char *string) {
  char *c;
	glRasterPos2f(x,y);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
  }
}


// OUTPUT: Draws a 100 by 100 grid with 1 unit spacing. 
void drawGrid(){
    glColor3f(1.0,1.0,1.0) ; 
    glBegin(GL_LINES);
      for(int i=((N_ROWS/2)*-1) ; i < (N_ROWS/2) ; i++){
          glVertex3f(((N_COLS/2)*-1),0,i) ; 
          glVertex3f((N_COLS/2),0,i) ; 
      }
      for(int i=((N_COLS/2)*-1) ; i < (N_COLS/2) ; i++){
          glVertex3f(i,0,((N_ROWS/2) * -1)) ; 
          glVertex3f(i,0,((N_ROWS/2))) ; 
      }
    glEnd() ; 
}

// OUTPUT: Draws an origin (0,0,0) ->RHS
void drawOrigin(){
    glBegin(GL_LINES);
    glColor3f(1.0,0.0,0.0) ; 
      glVertex3f(0,0,0) ; 
      glVertex3f(5,0,0) ; 
    glEnd() ; 
    glBegin(GL_LINES);
      glColor3f(0.0,1.0,0.0) ; 
      glVertex3f(0,0,0) ; 
      glVertex3f(0,5,0) ; 
    glEnd() ; 
    glBegin(GL_LINES);
      glColor3f(0.0,0.0,1.0) ; 
      glVertex3f(0,0,0) ; 
      glVertex3f(0,0,5) ; 
    glEnd() ; 
    
}

// OUTPUT: build the teapot on the main grid 
void buildTeapot(){
  glPushMatrix() ; 
  glTranslatef(-3,0.8,variable) ; 
  glRotatef(x_obj_rot,1.0,0.0,0.0) ; 
  glRotatef(y_obj_rot,0.0,1.0,0.0) ; 
  //glColor3f(.1,0,1) ; 
  glutSolidTeapot(1) ; 
  glPopMatrix() ; 
}

// OUTPUT: Change to an ortho projection to display name on screen. 
// Idea taken off GLUT -LIGHT 3d Tutorials @ http://www.lighthouse3d.com/opengl/glut/
// but implemented in own code ... 
void changeToOrtho() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, c_winwidth, 0, c_winheight);
	glScalef(1, -1, 1);
	glTranslatef(0, -1*(c_winheight), 0);
	glMatrixMode(GL_MODELVIEW);
}


// OUTPUT: restore from ortho projection to display name on screen
void restoreOldProj() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

// OUTPUT: handle when someone resizes the window ... 
void reshape(int width,int height){
  // Viewport transformation maps the entire projected transformation to
  // the viewport window . We must make them conformal ... 
  glViewport(0,0,width,height) ; 
  glMatrixMode(GL_PROJECTION) ; 
  glLoadIdentity();
  gluPerspective(fovy*zfactor, (GLfloat) width/(GLfloat) height, near, far); 
  glMatrixMode(GL_MODELVIEW) ; 
}

// OUTPUT: A factor that controls the zoom of the scene
void changeProjectionSettings() {
  glMatrixMode(GL_PROJECTION) ; 
  glLoadIdentity();
  gluPerspective(fovy*zfactor, (GLfloat) c_winwidth/(GLfloat) c_winheight, near, far); 
  glMatrixMode(GL_MODELVIEW) ; 
}

void display(void){

  changeProjectionSettings() ;
  // Called whenever something needs to be redrawn on the screen 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity() ; 
  gluLookAt(0,12,12,0,0,0,0,1,0); 

  // The camera rotates around the objects in the scene with this setting 
  // The camera rotates around it's central axis if I switch the order of lookat
  // and rotate commands. 
  glRotatef(rot_to_x,1.0,0.0,0.0) ; 
  glRotatef(rot_to_y,0.0,1.0,0.0) ; 
  
  // Display name 
  changeToOrtho() ;     
  glPushMatrix() ; 
  glLoadIdentity() ; 
  glColor3f(1.0,1.0,0.0) ; 
  renderString(0,10,"Neeraj Prashar - 23389059"); 
  glPopMatrix() ; 
  restoreOldProj() ; 
  
  // Draw origin and grid
  drawGrid(); 
  drawOrigin();

  // Draw Teapot
  //buildTeapot() ; 

  // Draw the building 
  parser.buildModelBuilding() ; 

  // Swap buffers and make it viewable 
  glutSwapBuffers() ; 
}


// Output: Rotate the object with the mouse
void mouseDrag(int x, int y){
    int dx = x - old_x ; 
    int dy = y - old_y ; 
    if(dx > 0 ){
      x_obj_rot += dx * 0.3 ; 
    }else if(dx < 0) {
      x_obj_rot -= dx * 0.3 ; 
    }

    if(dy >0){
      y_obj_rot += dy * 0.3 ; 
    }else if(dy <0){
      y_obj_rot -= dy * 0.3 ; 
    }

    old_x = x ; 
    old_y = y ; 
    glutPostRedisplay() ; 
}

// Output: Add Key attachments to the glut window 
void keySet(int key, int x, int y){
  if(key == 'q') {
    exit(-1) ; 
  }else if((unsigned char)key == 'r') {
    rot_to_x = 0 ; 
    rot_to_y = 0 ; 
    zfactor=0 ; 
    glutPostRedisplay() ; 
  }else if(key == 'w'){
    glutPostRedisplay() ; 
    variable -= 0.2 ; 
  }else if(key == 's'){
    variable += 0.2 ; 
    glutPostRedisplay() ; 
  }else if(key == 'a'){
    zfactor+=0.2 ; 
    glutPostRedisplay() ; 
  }else if(key == 'z'){
    zfactor-=0.2 ; 
    glutPostRedisplay() ; 
  }else if(key == (unsigned char)GLUT_KEY_DOWN){
    rot_to_x += 1 ; 
    if(rot_to_x > 360)
      rot_to_x -= 360 ; 
    glutPostRedisplay() ; 
  }else if(key == (unsigned char)GLUT_KEY_UP){
    rot_to_x -= 1 ; 
    if(rot_to_x < -360)
      rot_to_x += 360 ; 
    glutPostRedisplay() ; 
  }else if(key == (unsigned char)GLUT_KEY_RIGHT){
    rot_to_y -= 1 ; 
    if(rot_to_y > 360)
      rot_to_y += 360 ; 
    glutPostRedisplay() ; 
  }else if(key == (unsigned char)GLUT_KEY_LEFT){
    rot_to_y += 1 ; 
    if(rot_to_y < -360)
      rot_to_y -= 360 ; 
    glutPostRedisplay() ; 
  }
}

int main(int argc, char *argv[]){
  // Initialize GLUT and register callbacks. 
  glutInit(&argc,argv) ; 
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE) ; 
  glutInitWindowSize(c_winwidth,c_winheight) ; 
  glutCreateWindow("EECE 478 - File Loader/Viewer") ;
  glutReshapeFunc(reshape) ; 
  glutDisplayFunc(display) ; 
  // Called when mouse moves across with a button pressed. 
  // Passive Motion is for when mouse moves across screen with no 
  // buttons pressed. 
  glutMotionFunc(mouseDrag) ; 
  //glutMouseFunc(mouseClick) ; 
  glutSpecialFunc(keySet) ; 

  // If you don't have this, depth buffer will not be updated at all. 
  glEnable(GL_DEPTH_TEST) ; 
  // Reset all buffers to predefined value. 
  glClearDepth(1) ; 
  // Reset all screen background to black
  glClearColor(0,0,0,0) ; 
  // Rescale all normal vectors to have a magnitude of 1 
  glEnable(GL_NORMALIZE) ; 
  // Need this line below..uncomment before handing
  glEnable(GL_CULL_FACE) ; 
  // glFrontFace(GL_BACK) ; 

  if(argv[1] == NULL){
    fname = "478Model/TechnologyEnterpriseFacility_Gregor.model" ; 
  }else{
    fname = argv[1] ; 
  }
  
  // Load parser file .. 
  parser.Load(fname) ; 
  parser.LoadTextures() ; 

  // Infinite loop 
  glutMainLoop() ; 
}

