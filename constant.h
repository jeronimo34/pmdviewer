#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#define DWORD int
#else
#include <GL/glut.h>
#define DWORD unsigned long
#endif

#define WORD unsigned short //2byte
#define BYTE unsigned char //1byte

#define PI 3.141592655359

#define SAFE_DELETE(ptr) {if((ptr)){ delete (ptr); (ptr) = NULL;}}
#define SAFE_DELETE_ARRAY(ptr) {if((ptr)){ delete[] (ptr); (ptr) = NULL;}}

    const int WINDOW_WIDTH = 400;
    const int WINDOW_HEIGHT = 400;
