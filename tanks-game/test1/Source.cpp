//
//// simpletext.cpp
//// Glenn G. Chappell
//// 9 Sep 2010
////
//// For CS 381 Fall 2010
//// GLUT Text & Keyboard Demo
//
//#include <string>
//#include <sstream>
//#include <iostream>
//#include <cstdlib>       // Do this before GL/GLUT includes
//
//using std::string;
//using std::ostringstream;
//using std::cout;
//using std::cerr;
//using std::endl;
//using std::exit;
//
//# include <GL/glut.h>    // GLUT stuff, includes OpenGL headers as well
//
//
//// Global variables
//// Keyboard
//enum { ESCKEY = 27 };          // ASCII value of Escape
//string ostr;
//
//// Window/viewport
//const int startwinsize = 400;  // Starting window width & height (pixels)
//int i = 0;
//// Cursor
//double cursx, cursy;           // x, y pos for printing (camunits)
//double cursheight;             // Height of text line (camunits)
//
//// Object
//bool showsquare;               // true if big square is shown
//
//
//// printBitmap
//// Prints the given string at the raster position cursx, cursy
////  using GLUT bitmap fonts and the current transformation.
//// Decreases cursy by cursheight.
////
//// NOTE: You probably don't want any rotations in the model/view
////  transformation when calling this function.
//void printBitmap(const string& msg)
//{
//    glRasterPos2d(cursx, cursy);
//    for (string::const_iterator ii = msg.begin();
//        ii != msg.end();
//        ++ii)
//    {
//        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *ii);
//    }
//    cursy -= cursheight;
//}
//
//
//// myDisplay
//// The GLUT display function
//void myDisplay()
//{
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    // Draw objects
//    glLoadIdentity();
//    if (showsquare)  // Is big square visible?
//    {
//        glColor3d(0.9, 0.3, 0.3);
//        glBegin(GL_QUADS);
//        glVertex3d(-0.7, -0.7, 0.);
//        glVertex3d(0.7, -0.7, 0.);
//        glVertex3d(0.7, 0.7, 0.);
//        glVertex3d(-0.7, 0.7, 0.);
//        glEnd();
//    }
//    glColor3d(0.3, 0.9, 0.3);
//    glBegin(GL_QUADS);
//    glVertex3d(-0.85, 0.5, 0.);
//    glVertex3d(-0.5, 0.5, 0.);
//    glVertex3d(-0.5, 0.85, 0.);
//    glVertex3d(-0.85, 0.85, 0.);
//    glEnd();
//    glColor3d(0.3, 0.3, 0.9);
//    glBegin(GL_QUADS);
//    glVertex3d(0.85, -0.5, 0.);
//    glVertex3d(0.5, -0.5, 0.);
//    glVertex3d(0.5, -0.85, 0.);
//    glVertex3d(0.85, -0.85, 0.);
//    glEnd();
//
//    // Draw documentation
//    glLoadIdentity();
//    glMatrixMode(GL_PROJECTION);  // Set up simple ortho projection
//    glPushMatrix();
//    glLoadIdentity();
//    gluOrtho2D(-1., 1., -1., 1.);
//    glColor3d(0., 0., 0.);        // Black text
//    cursx = -0.9;                 // Start of first text line
//    cursy = 0.9;
//    cursheight = 0.1;             // Height of each text line
//    printBitmap("is a demo of GLUT bitmap text");
//    printBitmap("Here is some more text");
//    printBitmap("And here is yet more");
//
//    printBitmap("Here is the result of a computation: " + ostr);
//    printBitmap("Space       Toggle big square");
//    printBitmap("Esc         Quit");
//    glPopMatrix();                // Restore prev projection
//    glMatrixMode(GL_MODELVIEW);
//
//    glFlush();
//}
//
//
//// myIdle
//// The GLUT idle function
//void myIdle()
//{
//    // Print OpenGL errors, if there are any (for debugging)
//    if (GLenum err = glGetError())
//    {
//        cerr << "OpenGL ERROR: " << gluErrorString(err) << endl;
//    }
//}
//
//
//// myKeyboard
//// The GLUT keyboard function
//void myKeyboard(unsigned char key, int x, int y)
//{
//    switch (key)
//    {
//    case ESCKEY:  // Esc: quit
//        exit(0);
//        break;
//    case ' ':     // Space: toggle square viz
//        showsquare = !showsquare;
//        break;
//    
//
//    }
//    if (isalpha(key) || isdigit(key)) {
//        if (ostr.length() < 15)
//            ostr += key;
//    }
//    
//    glutPostRedisplay();
//
//    cout << ostr.length() << endl;
//}
//
//
//// myReshape
//// The GLUT reshape function
//void myReshape(int w, int h)
//{
//    // Set up viewport
//    glViewport(0, 0, w, h);
//
//    // Set up projection
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluOrtho2D(-1., 1., -1., 1.);
//
//    glMatrixMode(GL_MODELVIEW);  // Always go back to model/view mode
//}
//
//
//// init
//// Initializes GL states
//// Called by main after window creation
//void init()
//{
//    // Object
//    showsquare = true;
//
//    // OpenGL Stuff
//    glClearColor(0.7, 0.7, 0.7, 0.0);
//}
//
//
//int main(int argc, char** argv)
//{
//    // Initialize OpenGL/GLUT
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//
//    // Make a window
//    glutInitWindowSize(startwinsize, startwinsize);
//    glutInitWindowPosition(50, 50);
//    glutCreateWindow("CS 381 - GLUT Text Demo");
//
//    // Initialize GL states & register GLUT callbacks
//    init();
//    glutDisplayFunc(myDisplay);
//    glutIdleFunc(myIdle);
//    glutKeyboardFunc(myKeyboard);
//    glutReshapeFunc(myReshape);
//
//    // Do something
//    glutMainLoop();
//
//    return 0;
//}
//
