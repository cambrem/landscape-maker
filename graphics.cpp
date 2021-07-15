#include "graphics.h"
#include "circle.h"
#include "rect.h"
#include "Button.h"
#include <iostream>
#include <time.h>
#include <vector>
using namespace std;

GLdouble width, height;
int wd;

// declare ints for user choices and current screen number
int choice;
int screenNum = 0;

// declare shapes and vectors of shapes for various landscape features
vector<unique_ptr<Shape>> hills;
Rect sea;
Circle sun;
vector<Circle> snow;
vector<unique_ptr<Shape>> clouds;

// initialize colors to be used
const color darkBlue(1/255.0, 110/255.0, 214/255.0);
const color hillGreen(26/255.0, 176/255.0, 56/255.0);
const color white(1, 1, 1);

// declare enum for different screens and initialize screen as intro screen
enum {INTRO = 0, DIRECTIONS = 1, CHOOSEBACKGROUND = 2, CHOOSEFOREGROUND = 3, CHOOSEWEATHER = 4};
int screen = INTRO;

// create a vector of ints and push back buttons containing the options for the user
vector<int> choices;
Button choice1({0.9, 0.6, 0.3}, {100, 460}, 100, 50, "Dawn");
Button choice2({0.5, .8, .9}, {250, 460}, 100, 50, "Noon");
Button choice3({0.7, 0.6, 0.5}, {400, 460}, 100, 50, "Dusk");
Button choice4({26/255.0, 200/255.0, 56/255.0}, {100, 50}, 100, 50, "Grass");
Button choice5({26/255.0, 176/255.0, 56/255.0}, {250, 50}, 100, 50, "Hills");
Button choice6({1/255.0, 110/255.0, 214/255.0}, {400, 50}, 100, 50, "Sea");
Button choice7({1, 1, 0}, {75, 200}, 100, 50, "Sunny");
Button choice8({0, 1, 1}, {75, 400}, 100, 50, "Snowy");
Button choice9({0.9, 0.9,0.9 }, {75, 300}, 100, 50, "Cloudy");

// initialize hills by pushing back circles to vector
void initHills(){
    hills.clear();
    hills.push_back(make_unique<Circle>(hillGreen, 350, 500, 150));
    hills.push_back(make_unique<Circle>(hillGreen, 100, 600, 300));
    hills.push_back(make_unique<Circle>(hillGreen, 700, 700, 400));
}

// initialize sea using square
void initSea(){
    sea.setCenter(250, 450);
    sea.setSize(width, height/3);
    sea.setColor(darkBlue);
}

// initialize sun using circle
void initSun() {
    sun.setCenter(width + 50, 0);
    sun.setColor(1, 1, 0, 1);
    sun.setRadius(200);
}

// initialize snow using random sized circles
void initSnow() {
    for (int i = 0; i < 150; ++i) {
        snow.push_back(Circle(1, 1, 1, 1, rand() % int(width), -(rand() % int(height)), rand() % 5 + 1));
    }
}

// initialize clouds by pushing back circles to vector
void initClouds() {
    clouds.clear();
    dimensions cloudBottom(30, 30);
    // First cloud
    clouds.push_back(make_unique<Circle>(white, 300, 100, 20));
    clouds.push_back(make_unique<Circle>(white, 330, 100, 20));
    clouds.push_back(make_unique<Circle>(white, 320, 90, 20));
    clouds.push_back(make_unique<Rect>(white, 315, 105, cloudBottom));
    // Second cloud
    clouds.push_back(make_unique<Circle>(white, 50, 50, 50));
    clouds.push_back(make_unique<Circle>(white, 120, 50, 80));
    clouds.push_back(make_unique<Circle>(white, 200, 50, 50));
    // Third cloud
    clouds.push_back(make_unique<Circle>(white, 450, 50, 20));
    clouds.push_back(make_unique<Circle>(white, 480, 50, 20));
    clouds.push_back(make_unique<Circle>(white, 470, 40, 20));
    clouds.push_back(make_unique<Rect>(white, 465, 55, cloudBottom));
}

// call initializors
void init() {
    width = 500;
    height = 500;
    srand(time(0));
    initHills();
    initSea();
    initSun();
    initSnow();
    initClouds();
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);// Black and opaque
    choices.push_back(0);
    choices.push_back(1);
    choices.push_back(2);
    choices.push_back(3);
}

// function to print messages to screen
void printMessage(string message){
    for (char letter : message) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);
    }
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height);

    // do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // on intro screen, print introduction message
    if(screen == INTRO){
        glColor3f(1, 1, 1);
        glRasterPos2i(75, 175);
        printMessage("Let's paint a landscape");
        glRasterPos2i(75, 200);
        printMessage("Press the right key to move forward");
    }

    // on directions screen, print directions
    else if(screen == DIRECTIONS){
        glColor3f(1, 1, 1);
        glRasterPos2i(75, 175);
        printMessage("Click the buttons to see the options");
        glRasterPos2i(75, 200);
        printMessage("for your landscape. Once you decide,");
        glRasterPos2i(75, 220);
        printMessage("press the right arrow to advance");
        glRasterPos2i(75, 280);
        printMessage("Happy painting!");

    }

    // on choose background screen
    else if(screenNum == CHOOSEBACKGROUND) {
        // display prompt
        glColor3f(1, 1, 1);
        glRasterPos2i(75, 175);
        printMessage("What time of day");
        glRasterPos2i(75, 200);
        printMessage("shall we paint?");

        // display buttons with choices
        choice1.draw();
        choice2.draw();
        choice3.draw();

        // if user chooses dawn, set color to an orangy sunrise color
        if (choice == 1) {
            glClearColor(0.9, 0.6, 0.3, 0);
        }

        // if user chooses day, set color to sky blue
        else if (choice == 2) {
            glClearColor(0.5, .8, .9, 0);
        }

        // if user chooses dusk, set color to greyish color
        else if (choice == 3) {
            glClearColor(0.7, 0.6, 0.5, 0);
        }
    }

    // on choose foreground screen
    else if(screenNum == CHOOSEFOREGROUND) {
        // display prompt
        glColor3f(1, 1, 1);
        glRasterPos2i(75, 175);
        printMessage("What is in the foreground of our scene?");

        // display buttons with choices
        choice4.draw();
        choice5.draw();
        choice6.draw();

        // if user chooses grass
        if (choice == 4) {
            // draw grass
            int stagger = 0;
            for (int i = 0; i < 500; i++) {
                stagger += 10;
                glColor3f(26/255.0, 200/255.0, 56/255.0);
                glBegin(GL_QUADS);
                glVertex3f(-60.0 + stagger, 430.0, 00.0);
                glVertex3f(-60.0 + stagger, 430.0, 00.0);
                glVertex3f(-30.0 + stagger, 500.0, 00.0);
                glVertex3f(-50.0 + stagger, 500.0, 00.0);
                glEnd();
            }
        }

        // if user chooses hills
        else if (choice == 5) {
            // draw hills
            for (unique_ptr<Shape> &h : hills) {
                h->draw();
            }
        }

        // if user chooses sea
        else if (choice == 6) {
            // draw sea
            sea.draw();
        }
    }

    // on choose weather screen
    else if(screenNum == CHOOSEWEATHER) {
        // display prompt
        glColor3f(1, 1, 1);
        glRasterPos2i(75, 100);
        printMessage("Now, how's the weather?");

        // display choices
        choice7.draw();
        choice8.draw();
        choice9.draw();

        // if user chooses sunny
        if (choice == 7) {
            // draw sun
            sun.draw();
        }

        // if user chooses snowy
        if (choice == 8) {
            // draw snow
            for (Circle &flake : snow) {
                flake.draw();
            }
        }

        // if user chooses cloudy
        else if (choice == 9) {
            // draw clouds
            for (unique_ptr<Shape> &s : clouds) {
                s->draw();
            }
        }
    }

    // after all choices are made, the next press of the right arrow key will display result
    else {

        // redraw all choices
        for (int i = 0; i <= choices.size(); i++) {
            choice = choices[i];

            if (choice == 4) { // grass
                int temp = 0;
                for (int i = 0; i < 500; i++) {
                    temp += 10;
                    glColor3f(26/255.0, 200/255.0, 56/255.0);
                    glBegin(GL_QUADS);
                    glVertex3f(-60.0 + temp, 430.0, 00.0);
                    glVertex3f(-60.0 + temp, 430.0, 00.0);
                    glVertex3f(-30.0 + temp, 500.0, 00.0);
                    glVertex3f(-50.0 + temp, 500.0, 00.0);
                    glEnd();
                }
            }
            else if (choice == 5) { // hills
                for (unique_ptr<Shape> &s : hills) {
                    // #polymorphism
                    s->draw();
                }
            }
            else if (choice == 6) { // sea
                sea.draw();
            }
            else if (choice == 7) { // sun
                sun.draw();
            }
            else if (choice == 8) {
                for (Circle &flake : snow) { // snow
                    flake.draw();
                }
            }
            else if (choice == 9) {
                for (unique_ptr<Shape> &s : clouds) { // clouds
                    // #polymorphism
                    s->draw();
                }
            }
        }
    }

    glFlush();  // Render now
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y) {
    // escape
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }

    glutPostRedisplay();
}

void kbdS(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_DOWN:
            break;
        case GLUT_KEY_LEFT:
            break;

        // if right arrow key is pressed, increment screenNum to change screens
        case GLUT_KEY_RIGHT:
            screenNum += 1;
            if (screenNum == 1){
                screen = DIRECTIONS;
            }
            else if (screenNum == 2) {
                screen = CHOOSEBACKGROUND;
            }
            else if (screenNum == 3) {
                screen = CHOOSEFOREGROUND;
            }
            else if (screenNum == 4) {
                screen = CHOOSEWEATHER;
            }
            break;
        case GLUT_KEY_UP:
            break;
    }

    glutPostRedisplay();
}

void cursor(int x, int y) {
    // set up buttons to change color if cursor is overlapping them
    if(choice1.isOverlapping(x, y)) {
        choice1.hover();
    }
    else if(choice2.isOverlapping(x, y)){
        choice2.hover();
    }
    else if(choice3.isOverlapping(x, y)) {
        choice3.hover();
    }
    else if(choice4.isOverlapping(x, y)) {
        choice4.hover();
    }
    else if(choice5.isOverlapping(x, y)) {
        choice5.hover();
    }
    else if(choice6.isOverlapping(x, y)) {
        choice6.hover();
    }
    else if(choice7.isOverlapping(x, y)) {
        choice7.hover();
    }
    else if(choice8.isOverlapping(x, y)){
        choice8.hover();
    }
    else if(choice9.isOverlapping(x, y)) {
        choice9.hover();
    }
    else{
        choice1.release();
        choice2.release();
        choice3.release();
        choice4.release();
        choice5.release();
        choice6.release();
        choice7.release();
        choice8.release();
        choice9.release();
    }

    glutPostRedisplay();
}

// set up mouse to select choices when buttons are pressed
void mouse(int button, int state, int x, int y) {
    switch(button) {
        case GLUT_LEFT_BUTTON:
            switch (state) {
                case GLUT_DOWN:
                    choice1.pressDown();
                    choice2.pressDown();
                    choice3.pressDown();
                    choice4.pressDown();
                    choice5.pressDown();
                    choice6.pressDown();
                    choice7.pressDown();
                    choice8.pressDown();
                    choice9.pressDown();

                    break;
            }
            choice1.release();
            choice2.release();
            choice3.release();
            choice4.release();
            choice5.release();
            choice6.release();
            choice7.release();
            choice8.release();
            choice9.release();

            break;
    }

    switch(button) {
        case GLUT_LEFT_BUTTON:
            switch (state) {
                case GLUT_UP:
                    if(choice1.isOverlapping(x, y)) {
                        choice = 1;
                    }
                    else if(choice2.isOverlapping(x, y)){
                        choice = 2;
                    }
                    else if(choice3.isOverlapping(x, y)) {
                        choice = 3;

                    }
                    else if(choice4.isOverlapping(x, y)){
                        choice = 4;
                        choices[1] = 4;
                        glClear(GL_COLOR_BUFFER_BIT);
                    }
                    else if(choice5.isOverlapping(x, y)){
                        choice = 5;
                        choices[1] = 5;
                        glClear(GL_COLOR_BUFFER_BIT);
                    }
                    else if(choice6.isOverlapping(x, y)){
                        choice = 6;
                        choices[1] = 6;
                        glClear(GL_COLOR_BUFFER_BIT);
                    }
                    else if(choice7.isOverlapping(x, y)){
                        choice = 7;
                        choices[2] = 7;
                        glClear(GL_COLOR_BUFFER_BIT);
                    }
                    else if(choice8.isOverlapping(x, y)){
                        choice = 8;
                        choices[2] = 8;
                        glClear(GL_COLOR_BUFFER_BIT);
                    }
                    else if(choice9.isOverlapping(x, y)){
                        choice = 9;
                        choices[2] = 9;
                        glClear(GL_COLOR_BUFFER_BIT);
                    }

                    break;
            }

            break;
    }
    glutPostRedisplay();
}

// timer to make sun pulse (separate method since it needed to be slower)
void sunTimer(int dummy){
    // if radius is even, increase in size
    if ((int) sun.getRadius() % 2 == 0) {
        sun.setRadius(sun.getRadius() + 2);
        // if radius exceeds 220, make it odd
        if (sun.getRadius() > 220){
            sun.setRadius(219);
        }
    }
    // if radius is odd, decrease in size
    else {
        sun.setRadius(sun.getRadius() - 2);
        // if radius dips below 200, make it even
        if (sun.getRadius() < 200){
            sun.setRadius(200);
        }
    }
    glutPostRedisplay();
    glutTimerFunc(60, sunTimer, dummy);
}

void timer(int dummy) {
    // make snow fall
    for (Circle &flake : snow) {
        flake.moveY(flake.getRadius());
        if (flake.getTopY() > height) {
            flake.setCenter(rand() % int(width), -flake.getRadius());
        }
    }

    // make clouds move
    for (unique_ptr<Shape> &s : clouds) {
        // Move all the clouds to the left
        s->moveX(-1);
        // If a shape has moved off the screen
        if (s->getCenterX() < -80) {
            // Set it to the right of the screen so that it passes through again
            s->setCenterX(580);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, timer, dummy);
}


/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(100, 200); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("The Joy of Graphics" /* title */ );

    // Register callback handler for window re-paint event
    glutDisplayFunc(display);

    // Our own OpenGL initialization
    initGL();

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    // handles mouse movement
    glutPassiveMotionFunc(cursor);

    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, timer, 0);
    glutTimerFunc(0, sunTimer, 0);


    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}
