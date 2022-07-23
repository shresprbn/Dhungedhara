
#include "mathOperation.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
bool isApplicationRunning = true;

void drawpixel(SDL_Renderer* renderer, int x , int y) {
	SDL_RenderDrawPoint(renderer, x, y);
}
void drawline(SDL_Renderer* renderer, int x1,int y1, int x2 , int y2) {
	
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void DrawTriangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3) {
	drawline(renderer, x1, y1, x2, y2);
	drawline(renderer, x2, y2, x3, y3);
	drawline(renderer, x1, y1, x3, y3);

}
void fillTriangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3){
	//get length
	float d1 = sqrt(((y2 - y1) * (y2 - y1)) + ((x2 - x1) * (x2 - x1)));
	float d2 = sqrt(((y3 - y2) * (y3 - y2)) + ((x3 - x2) * (x3 - x2)));
	float d3 = sqrt(((y1 - y3) * (y1 - y3)) + ((x1 - x3) * (x1 - x3)));
	if (((d1 < d2) || (d1 = d2)) && ((d1 < d2) || (d1 = d2))) {
		int tx = x1;
		int ty = y1;
		float vx = (x2 - x1) / d1;
		float vy = (y2 - y1) / d1;
		int counter = 0;
		while (counter < d1) {
			drawline(renderer,x3, y3, tx, ty);
			tx = tx + vx;
			ty = ty + vy;
			counter++;
		}
	}
	else if ((d2 < d3) || (d2 = d3)) {
		int tx = x2;
		int ty = y2;
		float vx = (x3 - x2) / d2;
		float vy = (y3 - y2) / d2;
		int counter = 0;
		while (counter < d1) {
			drawline(renderer,x1, y1, tx, ty);
			tx = tx + vx;
			ty = ty + vy;
			counter++;
		}
	}
	else {
		int tx = x3;
		int ty = y3;
		float vx = (x1 - x3) / d3;
		float vy = (y1 - y3) / d3;
		int counter = 0;
		while (counter < d1) {
			drawline(renderer,x2, y2, tx, ty);
			tx = tx + vx;
			ty = ty + vy;
			counter++;
		}
	}
}
void FillTriangle(SDL_Renderer* renderer,int x1, int y1, int x2, int y2, int x3, int y3)
{
	auto SWAP = [](int& x, int& y) { int t = x; x = y; y = t; };
	auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) drawpixel(renderer,i, ny); };

	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;
	// Sort vertices
	if (y1 > y2) { SWAP(y1, y2); SWAP(x1, x2); }
	if (y1 > y3) { SWAP(y1, y3); SWAP(x1, x3); }
	if (y2 > y3) { SWAP(y2, y3); SWAP(x2, x3); }

	t1x = t2x = x1; y = y1;   // Starting points
	dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y2 - y1);

	dx2 = (int)(x3 - x1); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
	else signx2 = 1;
	dy2 = (int)(y3 - y1);

	if (dy1 > dx1) {   // swap values
		SWAP(dx1, dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // swap values
		SWAP(dy2, dx2);
		changed2 = true;
	}

	e2 = (int)(dx2 >> 1);
	// Flat top, just process the second half
	if (y1 == y2) goto next;
	e1 = (int)(dx1 >> 1);

	for (int i = 0; i < dx1;) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			i++;
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) t1xp = signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
		// Move line
	next1:
		// process second line until y value is about to change
		while (1) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y);    // Draw line from min to max points found on the y
									 // Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y == y2) break;

	}
next:
	// Second half
	dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y3 - y2);
	t1x = x2;

	if (dy1 > dx1) {   // swap values
		SWAP(dy1, dx1);
		changed1 = true;
	}
	else changed1 = false;

	e1 = (int)(dx1 >> 1);

	for (int i = 0; i <= dx1; i++) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) { t1xp = signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if (i < dx1) i++;
		}
	next3:
		// process second line until y value is about to change
		while (t2x != x3) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next4:

		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y);
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y > y3) return;
	}
}

rgba GetColor(float lum)
{
	rgba gs;
	int pixel_bw = (int)(13.0f * lum);
	switch (pixel_bw)
	{
	case 0:gs.r = 0; gs.g = 0; gs.b = 0; gs.a =255; break;

	case 1:gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 230; break;
	case 2:gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 217; break;
	case 3:gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 205; break;
	case 4:gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 179; break;

	case 5:gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 167; break;
	case 6:gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 153; break;
	case 7:gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 128; break;
	case 8:gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 102; break;

	case 9:gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 76; break;
	case 10:gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 64; break;
	case 11:gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 51; break;
	case 12:gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 26; break;
	default:
		gs.r = 0; gs.g = 0; gs.b = 0; gs.a = 26;
	}

	return gs;
}
int main(int argc, char** argv) {

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_WIDTH, 0, &window, &renderer);
	SDL_SetRenderDrawColor(renderer, 125, 200, 125, 255);
	SDL_RenderClear(renderer);
	

	mesh meshCube;
	mat4x4 matProj;

	vec3d vCamera = { 0,0,0 };
	vec3d vlookDir;

	meshCube.LoadFromObjectFile("VideoShip.obj");
	float fTheta = 0;

	// Projection Matrix
	matProj = Matrix_MakeProjection(90.0f, (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH, 0.1f, 1000.0f);

	while (isApplicationRunning) {
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			isApplicationRunning = false;
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_w:

				case SDLK_UP:
						break;
				default:
					break;
				}

				break;
			default:
				break;
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		

		// Set up rotation matrices
		mat4x4 matRotZ, matRotX;
		//fTheta += 1.0f * 0.05;
		matRotZ = Matrix_MakeRotationZ(fTheta * 0.5f);
		matRotX = Matrix_MakeRotationX(fTheta);

		mat4x4 matTrans;
		matTrans = Matrix_MakeTranslation(0.0f, 0.0f, 5.0f);

		mat4x4 matWorld;
		matWorld = Matrix_MakeIdentity();	// Form World Matrix
		matWorld = Matrix_MultiplyMatrix(matRotZ, matRotX); // Transform by rotation
		matWorld = Matrix_MultiplyMatrix(matWorld, matTrans); // Transform by translation

		vlookDir = { 0,0,1 };
		vec3d vUp = { 0,1,0 };
		vec3d vTarget = Vector_Add(vCamera, vlookDir);

		mat4x4 matCamera = Matrix_PointAt(vCamera, vTarget, vUp);

		// Make view matrix from camera
		mat4x4 matView = Matrix_QuickInverse(matCamera);


		// Store triagles for rastering later
		std::vector<triangle> vecTrianglesToRaster;

		// Draw Triangles
		for (auto tri : meshCube.tris)
		{
			triangle triProjected, triTransformed, triViewed;

			// World Matrix Transform
			triTransformed.p[0] = Matrix_MultiplyVector(matWorld, tri.p[0]);
			triTransformed.p[1] = Matrix_MultiplyVector(matWorld, tri.p[1]);
			triTransformed.p[2] = Matrix_MultiplyVector(matWorld, tri.p[2]);

			// Calculate triangle Normal
			vec3d normal, line1, line2;

			// Get lines either side of triangle
			line1 = Vector_Sub(triTransformed.p[1], triTransformed.p[0]);
			line2 = Vector_Sub(triTransformed.p[2], triTransformed.p[0]);

			// Take cross product of lines to get normal to triangle surface
			normal = Vector_CrossProduct(line1, line2);

			// You normally need to normalise a normal!
			normal = Vector_Normalise(normal);

			// Get Ray from triangle to camera
			vec3d vCameraRay = Vector_Sub(triTransformed.p[0], vCamera);

			// If ray is aligned with normal, then triangle is visible
			if (Vector_DotProduct(normal, vCameraRay) < 0.0f) {

				// Illumination
				vec3d light_direction = { 0.0f, 1.0f, -1.0f };
				light_direction = Vector_Normalise(light_direction);

				// How similar is normal to light direction
				float dp = normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;

				rgba c = GetColor(dp);

				triViewed.p[0] = Matrix_MultiplyVector(matView, triTransformed.p[0]);
				triViewed.p[1] = Matrix_MultiplyVector(matView, triTransformed.p[1]);
				triViewed.p[2] = Matrix_MultiplyVector(matView, triTransformed.p[2]);
				
				// Project triangles from 3D --> 2D
				triProjected.p[0] = Matrix_MultiplyVector(matProj, triViewed.p[0]);
				triProjected.p[1] = Matrix_MultiplyVector(matProj, triViewed.p[1]);
				triProjected.p[2] = Matrix_MultiplyVector(matProj, triViewed.p[2]);

				// do this normalizing
				triProjected.p[0] = Vector_Div(triProjected.p[0], triProjected.p[0].w);
				triProjected.p[1] = Vector_Div(triProjected.p[1], triProjected.p[1].w);
				triProjected.p[2] = Vector_Div(triProjected.p[2], triProjected.p[2].w);

				// Offset verts into visible normalised space
				vec3d vOffsetView = { 1,1,0 };
				triProjected.p[0] = Vector_Add(triProjected.p[0], vOffsetView);
				triProjected.p[1] = Vector_Add(triProjected.p[1], vOffsetView);
				triProjected.p[2] = Vector_Add(triProjected.p[2], vOffsetView);
				triProjected.p[0].x *= 0.5f * (float)SCREEN_WIDTH;
				triProjected.p[0].y *= 0.5f * (float)SCREEN_HEIGHT;
				triProjected.p[1].x *= 0.5f * (float)SCREEN_WIDTH;
				triProjected.p[1].y *= 0.5f * (float)SCREEN_HEIGHT;
				triProjected.p[2].x *= 0.5f * (float)SCREEN_WIDTH;
				triProjected.p[2].y *= 0.5f * (float)SCREEN_HEIGHT;

				// Store triangle for sorting
				vecTrianglesToRaster.push_back(triProjected);
			}
		
		}

		// Sort triangles from back to front
		sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle& t1, triangle& t2)
			{
				float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
				float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
				return z1 > z2;
			});


		for (auto& triProjected : vecTrianglesToRaster)
		{
			// Rasterize triangle
			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
			FillTriangle(renderer, triProjected.p[0].x, triProjected.p[0].y, triProjected.p[1].x, triProjected.p[1].y,
				triProjected.p[2].x, triProjected.p[2].y);


			SDL_RenderPresent(renderer);
			SDL_Delay(4);

		}
		
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}