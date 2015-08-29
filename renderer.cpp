#include "renderer.h"

Uint32 Renderer::color; 
SDL_Surface *Renderer::buffer;

void Renderer::Initialize(SDL_Surface *buffer)
{
	Renderer::buffer = buffer;
	color = SDL_MapRGB(buffer->format, 0xff, 0xff, 0xff);
}

void Renderer::DrawPixel(int x, int y, Uint32 pixel)
{
	if (x < buffer->w && x > 0 && y < buffer->h && y > 0)
	{
		SDL_LockSurface(buffer);
		Uint8 *p = (Uint8 *)buffer->pixels + y * buffer->pitch + x * buffer->format->BytesPerPixel;
		*(Uint32 *)p = pixel;
		SDL_UnlockSurface(buffer);
	}
}

// Using Linear Interpolation to draw a point pL between point p1 and p2
void Renderer::DrawLERP(float x, float y, Vec3f &p1, Vec3f &p2, Matrix &m, int scale)
{
	float magnitude = (p1 + p2).Mag();
	for (float RMxi = 0.0f; RMxi < 1; RMxi += (magnitude / scale) / 10)
			{
				Vec3f p1b(x - m.TransformP(p1).x * scale, y - m.TransformP(p1).y * scale, 0.0f);
				Vec3f p2b(x - m.TransformP(p2).x * scale, y - m.TransformP(p2).y * scale, 0.0f);
				Vec3f pL = Vec3f::LERP(p1b, p2b, RMxi);
				DrawPixel(pL.x, pL.y, SDL_MapRGB(buffer->format, 0xff, 0xff, 0xff));
			}
}

// Quick function for transforming and interpolating the pixels of a box using a matrix
void Renderer::DrawCube(float x, float y, Vec3f cubePoints[8], Matrix &m, int scale)
{
	for (int j = 0; j < 8; j++)
		{
			DrawPixel(x - m.TransformP(cubePoints[j]).x * scale, y - m.TransformP(cubePoints[j]).y * scale, color);
			
			if ((j % 2) == 0)
				DrawLERP(x, y, cubePoints[j], cubePoints[j + 1], m, scale);
			else if ((j % 2) == 1 && j != 7 && j != 3)
				DrawLERP(x, y, cubePoints[j], cubePoints[j + 2], m, scale);
			if (j < 4)
				DrawLERP(x, y, cubePoints[j], cubePoints[j + 4], m, scale);
			if(j == 4 || j == 0)
				DrawLERP(x, y, cubePoints[j], cubePoints[j + 2], m, scale);
		}
}

// Using Linear Interpolation to find a point pL between point p1 and p2 using a quaternion
void Renderer::DrawLERP(float x, float y, Vec3f &p1, Vec3f &p2, Quat &q, int scale)
{
	Vec3f p1b(x - q.Rotate(p1).x * scale, y - q.Rotate(p1).y * scale, 0.0f);
	Vec3f p2b(x - q.Rotate(p2).x * scale, y - q.Rotate(p2).y * scale, 0.0f);
	float magnitude = (p1 + p2).Mag();

	for (float RMxi = 0.0f; RMxi < 1; RMxi += (magnitude / scale) / 10)
			{
				Vec3f pL = Vec3f::LERP(p1b, p2b, RMxi);
				DrawPixel(pL.x, pL.y, color);
			}
}

// Quick function for transforming and interpolating the pixels of a box using a quaternion
void Renderer::DrawCube(float x, float y, Vec3f cubePoints[8], Quat &q, int scale)
{
	
	for (int j = 0; j < 8; j++)
		{
			DrawPixel(x - q.Rotate(cubePoints[j]).x * scale, y - q.Rotate(cubePoints[j]).y * scale, color);
			
			if ((j % 2) == 0)
				DrawLERP(x, y, cubePoints[j], cubePoints[j + 1], q, scale);
			else if ((j % 2) == 1 && j != 7 && j != 3)
				DrawLERP(x, y, cubePoints[j], cubePoints[j + 2], q, scale);
			if (j < 4)
				DrawLERP(x, y, cubePoints[j], cubePoints[j + 4], q, scale);
			if(j == 4 || j == 0)
				DrawLERP(x, y, cubePoints[j], cubePoints[j + 2], q, scale);
		}
}