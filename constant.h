#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#define DWORD int
#define WORD unsigned short //2byte
#define BYTE unsigned char //1byte
#else
#include <Windows.h>
#include <GL/glut.h>
#endif


#define PI 3.141592655359

#define SAFE_DELETE(ptr) {if((ptr)){ delete (ptr); (ptr) = NULL;}}
#define SAFE_DELETE_ARRAY(ptr) {if((ptr)){ delete[] (ptr); (ptr) = NULL;}}

    const int WINDOW_WIDTH = 400;
    const int WINDOW_HEIGHT = 400;
