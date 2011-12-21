/********************************/
/* author : Fabrice Boutarel    */
/********************************/

#include "main.hpp"


/******************* Fin exemple de fonction de dessin ************************/

void quit(void){
  exit(0);
}

/******************************************************************************/

void texsquare(GLuint texid){
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0,1,1,0);

  glColor3f(1.0,1.0,1.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glPushAttrib(GL_TEXTURE_BIT | GL_LIGHTING | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,texid);
  glBegin(GL_QUADS);
  glTexCoord2f(tx,ty);
  glVertex2f(0.0,0.0);
  glTexCoord2f(tx+1./zoom,ty);
  glVertex2f(1.0,0.0);
  glTexCoord2f(tx+1./zoom,ty+1./zoom);
  glVertex2f(1.0,1.0);
  glTexCoord2f(tx,ty+1./zoom);
  glVertex2f(0.0,1.0);
  glEnd();
  glBindTexture(GL_TEXTURE_2D,0);
  glDisable(GL_TEXTURE_2D);
  glPopAttrib();
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
}

void rename(){
    char title[100];
    sprintf(title,"%s : %d points",filename.c_str(),pList.size());
    glutSetWindowTitle(title);
}


static void displayFunc(){
  glViewport(0, 0,windowwidth,windowheight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  texsquare(lefttexid);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  std::vector< kn::Vector3d >::iterator iter=pList.begin();
  glColor3f(1.0,0.0,0.0);
  glBegin(GL_POINTS);
  while(iter!=pList.end()){
      unsigned int tmpx = ((*iter).at(0)-tx*image.width())*zoom;
      unsigned int tmpy = ((*iter).at(1)-ty*image.height())*zoom;
      glVertex2i(tmpx,tmpy);
      iter++;
  }
  glEnd();

  glutSwapBuffers();
}

static void idleFunc(void){
  glutPostRedisplay();
}

static void reshapeFunc(int newwidth, int newheight){
  windowwidth  = newwidth;
  windowheight = newheight;
  windowratio  = windowwidth/(double)windowheight;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0,image.width(),image.height(),0);
  glMatrixMode(GL_MODELVIEW);

  glutPostRedisplay();
}


static void kbdFunc(unsigned char c, int x, int y){
  switch(c){
    case ESCAPE:
    case 'q':
    case 'Q':
      quit();
      break;
    case 'f':
      glutFullScreen();
      break;
  }
}

static void kbdUpFunc(unsigned char c, int x, int y){
}

static void kbdSpecialFunc(int c, int x, int y){
 switch(c){
    case GLUT_KEY_UP :
      if(pList.empty())break;
      pList.at(pList.size()-1).at(1)--;
      break;
    case GLUT_KEY_DOWN :
      if(pList.empty())break;
      pList.at(pList.size()-1).at(1)++;
      break;
    case GLUT_KEY_LEFT :
      if(pList.empty())break;
      pList.at(pList.size()-1).at(0)--;
      break;
    case GLUT_KEY_RIGHT :
      if(pList.empty())break;
      pList.at(pList.size()-1).at(0)++;
      break;
    case GLUT_KEY_PAGE_UP :
      break;
    case GLUT_KEY_PAGE_DOWN :
      break;
    case GLUT_KEY_HOME:
      break;
    case GLUT_KEY_END:
      break;
    case GLUT_KEY_INSERT:
      break;
    case GLUT_KEY_F10:
      glutFullScreen();
      break;
  }
}

static void kbdSpecialUpFunc(int c, int x, int y){
}

static void mouseFunc(int button, int state, int x, int y){
  if (button == GLUT_WHEEL_UP && state==GLUT_DOWN){
      tx += (x / float(windowwidth)-0.25)/zoom;
      ty += (y / float(windowheight)-0.25)/zoom;
      zoom*=2.0;
    }
  if (button == GLUT_WHEEL_DOWN && state==GLUT_DOWN && zoom>1.0){
      tx -= 0.5/zoom;
      ty -= 0.5/zoom;
      zoom*=0.5;
  }

  if(tx<0.)	    tx=0.;
  if(tx>1.-1/zoom)  tx=1.-1./zoom;
  if(ty<0.)	    ty=0.;
  if(ty>1.-1./zoom) ty=1.-1./zoom;

  if(button==GLUT_RIGHT_BUTTON){
    if(state == GLUT_DOWN){
      xMotion=x;
      yMotion=y;
      motion=true;
    }else{
      motion=false;
    }
  }

  if(button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
    kn::Vector3d p;
    p.x()= (tx + x/float(windowwidth)/zoom)*image.width();
    p.y()= (ty + y/float(windowheight)/zoom)*image.height();
    p.z()=1.;
    pList.push_back(p);
//    std::cout<<"(x;y) "<<x*image.width()/windowwidth<<" "<<y*image.height()/windowheight<<std::endl;
//    std::cout<<"P "<<p<<std::endl;
    glutPostRedisplay();
  }
  rename();
}

static void motionFunc(int x, int y){
  if(motion){
    tx += (xMotion-x)/(windowwidth*zoom);
    ty += (yMotion-y)/(windowheight*zoom);
    xMotion=x;
    yMotion=y;
    if(tx<0.)	    tx=0.;
    if(tx>1.-1/zoom)  tx=1.-1./zoom;
    if(ty<0.)	    ty=0.;
    if(ty>1.-1./zoom) ty=1.-1./zoom;
  }
}

static void passiveMotionFunc(int x, int y){
}

bool isExtensionSupported(char * ext){
  std::string extensions = std::string((char*)(glGetString(GL_EXTENSIONS)));
  if(extensions.find(ext) != std::string::npos)
    return true;
  return false;
}

bool initTex(){
  glGenTextures(1, &lefttexid);
  glBindTexture (GL_TEXTURE_2D,lefttexid);
  glTexImage2D( GL_TEXTURE_2D,0,GL_RGBA,image.width(),image.height(),0,
      GL_RGB,GL_UNSIGNED_BYTE,image.begin());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  glBindTexture (GL_TEXTURE_2D,0);

  err = glGetError();
  if( err != GL_NO_ERROR){
    printf("OpenGL error has occured in initTex : %s \n",gluErrorString(err));
    return false;
  }
  return true;
}

bool initGL(void){
  glClearColor(0.1,0.1,0.1,1.0);

  glPixelStorei(GL_PACK_ALIGNMENT,   1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glDisable(GL_TEXTURE_2D);

  glPointSize(2.0);
  glLineWidth(3.0);

  err = glGetError();
  if( err != GL_NO_ERROR){
    printf("OpenGL error has occured in initGL : %s \n",gluErrorString(err));
    return false;
  }

  return true;
}

void atex(){
  if(pList.size()){
    std::ostringstream oss;
    oss << filename;
    oss << ".list";
    kn::saveVectorList(pList,oss.str().c_str());
  }
}

int main(int argc, char ** argv){
  atexit(atex);
  if(argc>1) filename = argv[1];
  else quit();

  //Load image
  kn::loadJPG(image,filename);
//  windowwidth  = image.width();
//  windowheight = image.height();

  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_ALPHA);

  glutInitWindowPosition(0,0);
  glutInitWindowSize(windowwidth, windowheight);

  int rvalue = 0;
  rvalue = glutCreateWindow(filename.c_str());
  if(rvalue<=0){
    std::cerr << "Echec à la création de la fenetre" << std::endl;
    exit(0);
  }

  glutDisplayFunc(       displayFunc);
  glutIdleFunc(          idleFunc);
  glutReshapeFunc(       reshapeFunc);

  glutKeyboardFunc(      kbdFunc);
  glutKeyboardUpFunc(    kbdUpFunc);
  glutSpecialFunc(       kbdSpecialFunc);
  glutSpecialUpFunc(     kbdSpecialUpFunc);

  glutMouseFunc(         mouseFunc);
  glutMotionFunc(        motionFunc);
  glutPassiveMotionFunc( passiveMotionFunc);

  if(!initGL()){
    std::cerr << "Error while initializating OpenGL states" << std::endl;
    exit(0);
  }

  if(!initTex()){
    std::cerr << "Error while initializating Textures" << std::endl;
    exit(0);
  }

  glutMainLoop();
  return 0;
}
