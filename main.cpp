/* 
Ricky Joudrey
This program demonstrates use of 3D math in a 2D environment such as SDL.
Key repeat is disabled, to lessen effects of the buffer size on the rotation speed.

Keys:
Space: Stop rotations completely
Up arrow: Zoom in
Down arrow: Zoom out
Left arrow: Rotate left
Right arrow: Rotate right
*/

#include "dependencies.h"

int main(int argc, char *argv[])
{
	// Basic SDL Setup
	SDL_Surface *buffer;
	SDL_Init(SDL_INIT_VIDEO);
	buffer = SDL_SetVideoMode(640, 480, 32, SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_RESIZABLE);
	if (!buffer) exit(-1);
	Renderer::Initialize(buffer);
	SDL_EnableKeyRepeat(0, 0);
	SDL_Event event;

	// Global viewspace matrix
	Matrix worldM;

	// Local viewspace cube
	Vec3f cubePoints[8]; 
	cubePoints[0] = Vec3f(-1.0f,  1.0f, -1.0f); 
	cubePoints[1] = Vec3f(-1.0f,  1.0f,  1.0f);
	cubePoints[2] = Vec3f(-1.0f, -1.0f, -1.0f);
	cubePoints[3] = Vec3f(-1.0f, -1.0f,  1.0f);
	cubePoints[4] = Vec3f(1.0f,  1.0f, -1.0f);
	cubePoints[5] = Vec3f(1.0f,  1.0f,  1.0f);
	cubePoints[6] = Vec3f(1.0f, -1.0f, -1.0f);
	cubePoints[7] = Vec3f(1.0f, -1.0f,  1.0f);

	float scale = 1.0f;
	float r = 0.00f; 
	float rWx = 0.01f;
	float rWy = 0.01f;
	float rWz = 0.01f;
	float delta = 0.00000000000000000000001f;
	int bW = buffer->w;
	int bH = buffer->h;
	Vec3f cubes[6];
	Matrix rWMs[4]; 
	Quat rQs[6];

	// Global positions of 6 cubes
	for (int i = 0; i < 6; i++)
		if (i < 3)
			cubes[i].x = -1.0f, cubes[i].y = i - 1.0f, cubes[i].z = 0.0f;
		else
			cubes[i].x = 1.0f, cubes[i].y = i - 4.0f, cubes[i].z = 0.0f;

	while (true)
	{
		while (SDL_PollEvent(&event))
        {
			if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
                exit(0);
			else if (event.type == SDL_VIDEORESIZE)
			{
				SDL_SetVideoMode(event.resize.w, event.resize.h, 32,  SDL_HWSURFACE | SDL_RESIZABLE);
				bW = buffer->w; bH = buffer->h; 
			}
			switch(event.key.keysym.sym)
			{
				case SDLK_RIGHT:
					delta += 0.0002f; break;
				case SDLK_LEFT:
					delta -= 0.0002f; break;
				case SDLK_SPACE:
					delta = 0; break;
				case SDLK_UP:
					if (scale < 75) scale += 1.0f;	break;
				case SDLK_DOWN:
					if (scale > 1) scale -= 1.0f; break;
			}
		}

		// Creating rotation matrices for current angle
		rQs[0] = Quat::CalculateQuat(Vec3f(1.0f, 0.0f, 0.0f), r);
		rQs[1] = Quat::CalculateQuat(Vec3f(0.0f, 1.0f, 0.0f), r);
		rQs[2] = Quat::CalculateQuat(Vec3f(0.0f, 0.0f, 1.0f), r);
		rQs[3] = rQs[1] * rQs[0];
		rQs[4] = rQs[2] * rQs[0];
		rQs[5] = rQs[1] * rQs[2];

		// Creating a matrix for rotation around the world-view axes
		rWMs[0].RotateX(rWx);
		rWMs[1].RotateX(rWy);
		rWMs[2].RotateZ(rWz);
		rWMs[3] = rWMs[1] * rWMs[0];
		worldM = rWMs[2] * rWMs[3];
		worldM.Scale(scale);
		SDL_FillRect(buffer,NULL, 0x000000);
		
		// Applying world view matrix to cubes and then scaling positions
		for (int i = 0; i < 6; i++)
		{
			Renderer::DrawCube(worldM.TransformP(cubes[i]).x * 5  + bW/2, 
						worldM.TransformP(cubes[i]).y * 5 + bH/2,  
						cubePoints, rQs[i], (int)scale);
		}
		
		r += delta * 1/2 * scale;
		SDL_Flip(buffer); 
	}
	return 0;
}

