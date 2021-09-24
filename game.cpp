//============================================================================
// Name        : brickSlayerPF.cpp
// Author      : Muhammad Akhtar Nadeem
// Roll No      : 20L-1171
// Version     : 1.0.0
// Copyright   : (c) Reserved
// Description : Basic 2D game of Brick Slayer...
//============================================================================

#ifndef BSLAYER_CPP_
#define BSLAYER_CPP_
#include <GL/gl.h>
#include <GL/glut.h>

#include <cmath>  // for basic math functions such as cos, sin, sqrt
#include <iostream>
#include <string>
using namespace std;

// defining some MACROS

#define MAX(A, B) ((A) > (B) ? (A) : (B))  // finds max of two numbers
#define MIN(A, B) ((A) < (B) ? (A) : (B))  // find min of two numbers
#define ABS(A) ((A) < (0) ? -(A) : (A))    // find ABS of a given number
#define FPS 80                             // frame per seconds
#define KEY_ESC 27                         // A

// some global variables
int width = 800, height = 600;
int x1ofBoard = width / 2 - 75, x2ofBoard = width / 2 + 75;
const int BRICKS = 150;
const int brickWidth = 60;
const int brickHeight = 20;
int ballX = width / 2;
int ballY = 35;
int velX = 2;
int velY = 3;

// Colors used
float colors[6][3] = {{0 / 255.0, 188 / 255.0, 212 / 255.0},
                      {244 / 255.0, 67 / 255.0, 54 / 255.0},
                      {208 / 255.0, 209 / 255.0, 2 / 255.0},
                      {0 / 255.0, 161 / 255.0, 203 / 255.0},
                      {50 / 255.0, 116 / 255.0, 44 / 255.0},
                      {255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0}};

float Deg2rad(float degree) { return (degree / 180.0) * M_PI; }

void InitRandomizer() { srand((unsigned int)time(0)); }

long GetRandInRange(const long &rmin, const long &rmax) {
  long range = rmax - rmin;
  long value = (rand() % (long)range) + rmin;
  return value;
}

// To draw the triangle
void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3,
                  float color[]) {
  glColor3fv(color);
  glBegin(GL_TRIANGLES);

  glVertex4i(x1, y1, 0, 1);
  glVertex4i(x2, y2, 0, 1);
  glVertex4i(x3, y3, 0, 1);

  glEnd();
}

// To draw the ball
void DrawSphere(float color[], int midx = 400, int midy = 20) {
  for (int i = 0; i < 360; i += 10) {
    float theta = Deg2rad(i);
    float theta2 = Deg2rad(i + 10);

    DrawTriangle(midx, midy, midx + 12 * cos(theta), midy + 12 * sin(theta),
                 midx + 12 * cos(theta2), midy + 12 * sin(theta2), color);
  }
}

// To move the ball
void MoveBall() {
  if (ballY > height - 10) {
    velY *= -1;
  }
  if (ballX > width || ballX < 4) {
    velX *= -1;
  }
  ballY += velY;
  ballX += velX;

  DrawSphere(colors[4], ballX, ballY);
}

// To draw the rectangle (general)
void DrawRectangle(float x1, float y1, float x2, float y2, float color[]) {
  DrawTriangle(x1, y1, x2, y1, x1, y2, color);
  DrawTriangle(x1, y2, x2, y2, x2, y1, color);
}

// To draw the board
void DrawBoard(float x1, float y1, float x2, float y2, float color[]) {
  DrawRectangle(x1, y1, x2, y2, color);
}

// Brick stuct
struct brickProps {
  float x1, y1;
  float x2, y2;
  bool visible = true;
} bricksArray[BRICKS];

// To setup the bricks
void DrawBricks() {
  for (int i = 0, x = 60, y = 500; i < BRICKS; i++, x += 68) {
    if (x > 680) {
      x = 60, y -= 25;
    }
    bricksArray[i].x1 = x;
    bricksArray[i].y1 = y;
    bricksArray[i].x2 = bricksArray[i].x1 + brickWidth;
    bricksArray[i].y2 = bricksArray[i].y1 + brickHeight;
  }
}

// To re-render the bricks
void reRender() {
  for (int i = 0; i < BRICKS; i++) {
    if (bricksArray[i].visible == true) {
      DrawRectangle(bricksArray[i].x1, bricksArray[i].y1, bricksArray[i].x2,
                    bricksArray[i].y2, colors[i % 4]);
    }
  }
}

// To handle the Collision of ball with board
void boardCollision() {
  if (ballY > 15 && ballY < 20) {
    if (ABS(ballX - ((x1ofBoard + x2ofBoard) / 2)) < 75) {
      velY = velY * -1;
    }
  }

  if (ballY < 0) {
    exit(1);
  }
}

// To handle the Collision of ball with bricks
void brickCollision() {
  for (int i = 0; i < BRICKS; i++) {
    int Dx = ABS(ballX - ((bricksArray[i].x1 + bricksArray[i].x2) / 2));
    int Dy = ABS(ballY - ((bricksArray[i].y1 + bricksArray[i].y2) / 2));

    if (Dx < 35) {
      if (Dy < 15) {
        bricksArray[i].x2 = 0;
        bricksArray[i].x1 = 0;
        bricksArray[i].visible = false;
        velY *= -1;

        if (MAX(Dx, Dy) == Dx) {
          bricksArray[i].x2 = 0;
          bricksArray[i].x1 = 0;
          bricksArray[i].visible = false;
          velX *= -1;
        }
      }
    }
  }
}

/*
 * Main Canvas drawing function.
 * */
void Display() {
  // background color of canvas
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);  // Update the colors

  reRender();
  brickCollision();
  MoveBall();
  boardCollision();

  DrawBoard(x1ofBoard, 5, x2ofBoard, 15, colors[5]);

  glutSwapBuffers();  // do not modify this line..
}

void SetCanvasSize(int width, int height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width, 0, height, -1,
          1);  // set the screen size to given width and height.
}

void NonPrintableKeys(int key, int x, int y) {
  if (key == GLUT_KEY_LEFT) {
    if (x1ofBoard > 0) {
      x1ofBoard -= 50;
      x2ofBoard -= 50;
      glutPostRedisplay();
    }
  } else if (key == GLUT_KEY_RIGHT) {
    if (x2ofBoard < width) {
      x1ofBoard += 50;
      x2ofBoard += 50;
      glutPostRedisplay();
    }
  }
}
void PrintableKeys(unsigned char key, int x, int y) {
  if (key == KEY_ESC) {
    exit(1);
  }
}

void Timer(int m) {
  // implement your functionality here

  Display();

  // once again we tell the library to call our Timer function after next
  // 1000/FPS
  glutTimerFunc(1000.0 / FPS, Timer, 0);
}

int main(int argc, char *argv[]) {
  DrawBricks();
  InitRandomizer();
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(50, 50);
  glutInitWindowSize(width, height);
  glutCreateWindow("PF Bricks Slayer Game Project");
  SetCanvasSize(width, height);

  glutDisplayFunc(Display);
  glutSpecialFunc(NonPrintableKeys);
  glutKeyboardFunc(PrintableKeys);
  glutTimerFunc(1000.0 / FPS, Timer, 0);

  // now handle the control to library and it will call our registered functions
  // when it deems necessary...
  glutMainLoop();
  return 1;
}
#endif /* BSLAYER_CPP_ */