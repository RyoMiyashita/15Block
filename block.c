#include <GLUT/glut.h>
#include <glpng/glpng.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BLOCKWIDTH 4
#define BLOCKHEIGHT 4
#define BLOCK (BLOCKWIDTH * BLOCKHEIGHT)
#define ELEMENT(i, j) (((j) * BLOCKWIDTH) + (i))
#define BLOCKSIZE 80
#define MARGIN 15
#define WIDTH (BLOCKSIZE * BLOCKWIDTH + (MARGIN * 2))
#define HEIGHT (BLOCKSIZE * BLOCKHEIGHT + (MARGIN * 2))

void Display(void);
void Reshape(int, int);
void Timer(int);
void DisplayBlock();
void DisplayMenu();
void PutSprite(int, int, int, pngInfo *);
void Mouse(int, int, int, int);
void IsMove(int, int);
void Move(int, int, int, int);
void swap(int *, int *);
void Keyboard(unsigned char, int, int);

GLuint img[BLOCK];
pngInfo info[BLOCK];

int block[BLOCK];
char menuFlag = 1;
double theta = (3.0 / 4.0) * M_PI;


int main(int argc, char const *argv[]) {
  glutInit(&argc, argv);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("15Block");
  glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA);
  glClearColor(0.0, 0.0, 0.0, 1.0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  img[0] = pngBind("img/title.png", PNG_NOMIPMAP, PNG_ALPHA,
                 &info[0], GL_CLAMP, GL_NEAREST, GL_NEAREST);
  printf("img%d: id=%d, W=%d, H=%d, D=%d, A=%d\n", 0, img[0],
                info[0].Width, info[0].Height, info[0].Depth, info[0].Alpha);
  for (int i = 1; i < BLOCK; i++){
    char str[32];
    sprintf(str, "img/block%d.png", i);
    img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA,
                   &info[i], GL_CLAMP, GL_NEAREST, GL_NEAREST);
    printf("img%d: id=%d, W=%d, H=%d, D=%d, A=%d\n", i, img[i],
                  info[i].Width, info[i].Height, info[i].Depth, info[i].Alpha);
  }

  int i;
  for(i = 1; i < BLOCK; i ++) {
    block[i - 1] = i;
  }
  block[ELEMENT(BLOCKWIDTH - 1, BLOCKHEIGHT - 1)] = 0;

  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutMouseFunc(Mouse);
  glutKeyboardFunc(Keyboard);
  glutTimerFunc(1, Timer, 0);

  glutMainLoop();

  return 0;
}

void Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glScaled(1, -1, 1);
    glTranslated(0, -h, 0);
    glutReshapeWindow(WIDTH, HEIGHT);
  }


void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (menuFlag == 1) {
      DisplayMenu();
    } else {
      DisplayBlock();
    }

    glFlush();
}

void Timer(int t)
{
  if (menuFlag == 1)
    glutTimerFunc(1, Timer, 0);
  theta += M_PI / 100;
  if (theta >= 2 * M_PI) {
    theta -= 2 * M_PI;
  }
  glutPostRedisplay();
}


void DisplayBlock()
{
  int i, j, num;
  for (j = 0; j < BLOCKWIDTH; j++) {
    for (i = 0; i < BLOCKHEIGHT; i++) {
      num = block[ELEMENT(i, j)];
      if (num != 0) {
        PutSprite(img[num], MARGIN + BLOCKSIZE * i, MARGIN + BLOCKSIZE * j, &info[num]);
      }
    }
  }
}


void DisplayMenu()
{
  int w, h;
  int num;
  int x;
  int y;
  double xc, yc;

  num = img[0];

  w = info[0].Width;
  h = info[0].Height;

  xc = glutGet(GLUT_WINDOW_WIDTH)/2;
  yc = glutGet(GLUT_WINDOW_HEIGHT)/2;

  double c = (sqrt((w/2)*(w/2) + (h/2)*(h/2)));

  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, num);
  glColor4ub(255, 255, 255, 255);

  glBegin(GL_QUADS);

  x = (int)(c * cos(theta) + xc);
  y = (int)(yc - c * sin(theta));
  glTexCoord2i(0, 0);
  glVertex2i(x, y);

  x = (int)(c * cos(theta + M_PI/2) + xc);
  y = (int)(yc - c * sin(theta + M_PI/2));
  glTexCoord2i(0, 1);
  glVertex2i(x, y);

  x = (int)(c * cos(theta + M_PI) + xc);
  y = (int)(yc - c * sin(theta + M_PI));
  glTexCoord2i(1, 1);
  glVertex2i(x, y);

  x = (int)(c * cos(theta - M_PI/2) + xc);
  y = (int)(yc - c * sin(theta - M_PI/2));
  glTexCoord2i(1, 0);
  glVertex2i(x, y);

  glEnd();

  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}


void PutSprite(int num, int x, int y, pngInfo *info)
{
    int w, h;


    w = info->Width;
    h = info->Height;

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, num);
    glColor4ub(255, 255, 255, 255);

    glBegin(GL_QUADS);

    glTexCoord2i(0, 0);
    glVertex2i(x, y);

    glTexCoord2i(0, 1);
    glVertex2i(x, y + h);

    glTexCoord2i(1, 1);
    glVertex2i(x + w, y + h);

    glTexCoord2i(1, 0);
    glVertex2i(x + w, y);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}


void Mouse(int b, int s, int x, int y)
{
  if (b == GLUT_LEFT_BUTTON)
  {
    if (s == GLUT_UP) IsMove(x, y);
  }
}


void IsMove(int x, int y)
{
  int result = 0;
  int i;
  if (block[ELEMENT((x - MARGIN) / BLOCKSIZE, (y - MARGIN) / BLOCKSIZE)] != 0) {
    for (i = 0; i < BLOCKWIDTH; i++) {
      if (block[ELEMENT(i, ((y - MARGIN) / BLOCKSIZE))] == 0) {
        result = 1;
        Move(((x - MARGIN) / BLOCKSIZE), ((y - MARGIN) / BLOCKSIZE), i, ((y - MARGIN) / BLOCKSIZE));
        break;
      }
    }
    if (result == 0) {
      for (i = 0; i < BLOCKHEIGHT; i++) {
        if (block[ELEMENT(((x - MARGIN) / BLOCKSIZE), i)] == 0) {
          result = 1;
          Move(((x - MARGIN) / BLOCKSIZE), ((y - MARGIN) / BLOCKSIZE), ((x - MARGIN) / BLOCKSIZE), i);
          break;
        }
      }
    }
  }
}


void Move(int x, int y, int zeroi, int zeroj)
{
  int i;
  if (x == zeroi) {
    for (i = 0; i < abs(y - zeroj); i++) {
      if (y < zeroj)
        swap(&block[ELEMENT(x, zeroj - i)], &block[ELEMENT(x, zeroj - (i + 1))]);
      if (y > zeroj)
        swap(&block[ELEMENT(x, zeroj + i)], &block[ELEMENT(x, zeroj + (i + 1))]);
    }
  }
  else if (y == zeroj) {
    for (i = 0; i < abs(x - zeroi); i++) {
      if (x < zeroi)
        swap(&block[ELEMENT(zeroi - i, y)], &block[ELEMENT(zeroi - (i + 1), y)]);
      if (x > zeroi)
        swap(&block[ELEMENT(zeroi + i, y)], &block[ELEMENT(zeroi + (i + 1), y)]);
    }
  }
  glutPostRedisplay();
}


void swap(int *a, int *b)
{
  int t = *a;
  *a = *b;
  *b = t;
}

void Keyboard(unsigned char key, int x, int y) {
  int i;
  if (key == 27) {
    printf("escで終了しました。\n");
    exit(0);
  }
  if (key == 32) {
    if (menuFlag == 1)
      menuFlag = 0;
    for (i = 0; i < BLOCK; i++)
    {
        int j = rand()%BLOCK;
        swap(&block[i], &block[j]);
    }
  }
}
