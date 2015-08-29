#include "dependencies.h"

#ifndef RENDERER_H
#define RENDERER_H

class Renderer
{
public:
	static void DrawPixel(int x, int y, Uint32 pixel);
	static void DrawCube(float x, float y, Vec3f cubePoints[8], Matrix &m, int scale);
	static void DrawLERP(float x, float y, Vec3f &p1, Vec3f &p2, Matrix &m, int scale);
	static void DrawLERP(float x, float y, Vec3f &p1, Vec3f &p2, Quat &q, int scale);
	static void DrawCube(float x, float y, Vec3f cubePoints[8], Quat &q, int scale);
	static void Initialize(SDL_Surface *buffer);
private: 
	static Uint32 color; 
	static SDL_Surface *buffer;
};

#endif