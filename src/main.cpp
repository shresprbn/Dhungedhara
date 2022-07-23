#include "draw.h"
#include "fill.h"
#include "color.h"
#include "mathOperation.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
bool isApplicationRunning = true;

int main(int argc, char** argv) {

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_WIDTH, 0, &window, &renderer);
	SDL_SetRenderDrawColor(renderer, 125, 200, 125, 255);
	SDL_RenderClear(renderer);


	mesh meshCube;
	mesh wall;
	mesh water, ground;
	mat4x4 matProj;
	float fYaw = 1.0f;
	bool drawFlag = true;
	rgba c;

	vec3d vCamera = { 0,0,0 };
	vec3d vlookDir;

	meshCube.LoadFromObjectFile("Dhungedhara1.obj");
	wall.LoadFromObjectFile("DhungedharaWall.obj");
	water.LoadFromObjectFile("Water.obj");
	ground.LoadFromObjectFile("Ground.obj");
	float fTheta = 0;

	// Projection Matrix
	matProj = Matrix_MakeProjection(90.0f, (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH, 0.1f, 1000.0f);
	while (isApplicationRunning) {
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			isApplicationRunning = false;
		if (SDL_PollEvent(&event)) {
			vec3d vForward = Vector_Mul(vlookDir, 8.0f);
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					vCamera.y += 8.0f;
					drawFlag = true;
					break;
				case SDLK_DOWN:
					vCamera.y -= 8.0f;
					drawFlag = true;
					break;
				case SDLK_LEFT:
					vCamera.x -= 8.0f;
					drawFlag = true;
					break;
				case SDLK_RIGHT:
					vCamera.x += 8.0f;
					drawFlag = true;
					break;
				case SDLK_a:
					fYaw -= 0.5f;
					drawFlag = true;
					break;
				case SDLK_d:
					fYaw += 0.5f;
					drawFlag = true;
					break;
				case SDLK_w:
					vCamera = Vector_Add(vCamera, vForward);
					drawFlag = true;
					break;
				case SDLK_s:
					vCamera = Vector_Sub(vCamera, vForward);
					drawFlag = true;
					break;
				default:
					break;
				}

				break;
			default:
				break;
			}
		}
		if (drawFlag) {

			SDL_SetRenderDrawColor(renderer, 159, 217, 252, 255);
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


			vec3d vUp = { 0,-1,0 };
			vec3d vTarget = { 0,0,1 };
			mat4x4 matCameraRot = Matrix_MakeRotationY(fYaw);
			vlookDir = Matrix_MultiplyVector(matCameraRot, vTarget);
			vTarget = Vector_Add(vCamera, vlookDir);

			mat4x4 matCamera = Matrix_PointAt(vCamera, vTarget, vUp);

			// Make view matrix from camera
			mat4x4 matView = Matrix_QuickInverse(matCamera);


			// Store triagles for rastering later
			std::vector<triangle> vecTrianglesToRasterGround;
			std::vector<triangle> vecTrianglesToRaster;
			std::vector<triangle> vecTrianglesToRasterWall;
			std::vector<triangle> vecTrianglesToRasterWater;
			
			// Draw Ground-------------------------------------------------------------------------------------
			for (auto tri : ground.tris)
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
				
				// if (Vector_DotProduct(normal, vCameraRay) < 0.0f) {



					triViewed.p[0] = Matrix_MultiplyVector(matView, triTransformed.p[0]);
					triViewed.p[1] = Matrix_MultiplyVector(matView, triTransformed.p[1]);
					triViewed.p[2] = Matrix_MultiplyVector(matView, triTransformed.p[2]);
					triViewed.col = { 180,125,90 };
					// Clip Viewed Triangle against near plane, this could form two additional
					// additional triangles. 
					int nClippedTriangles = 0;
					triangle clipped[2];
					nClippedTriangles = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

					// We may end up with multiple triangles form the clip, so project as
					// required
					for (int n = 0; n < nClippedTriangles; n++)
					{
						// Project triangles from 3D --> 2D
						triProjected.p[0] = Matrix_MultiplyVector(matProj, clipped[n].p[0]);
						triProjected.p[1] = Matrix_MultiplyVector(matProj, clipped[n].p[1]);
						triProjected.p[2] = Matrix_MultiplyVector(matProj, clipped[n].p[2]);

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
						vecTrianglesToRasterGround.push_back(triProjected);

					}
				// }
			}
			// Sort triangles from back to front
			sort(vecTrianglesToRasterGround.begin(), vecTrianglesToRasterGround.end(), [](triangle& t1, triangle& t2)
				{
					float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
					float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
					return z1 > z2;
				});
			for (auto& triProjected : vecTrianglesToRasterGround)
			{
				rgba a = { 64,97,33,255 };
				//Rasterize the triangle
				FillTriangle(renderer, triProjected.p[0].x, triProjected.p[0].y, triProjected.p[1].x, triProjected.p[1].y,
					triProjected.p[2].x, triProjected.p[2].y, a);

				drawFlag = false;
				SDL_RenderPresent(renderer);
			}

			// Draw Wall------------------------------------------------------------------------------------
			for (auto tri : wall.tris)
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



					triViewed.p[0] = Matrix_MultiplyVector(matView, triTransformed.p[0]);
					triViewed.p[1] = Matrix_MultiplyVector(matView, triTransformed.p[1]);
					triViewed.p[2] = Matrix_MultiplyVector(matView, triTransformed.p[2]);
					triViewed.col = { 180,125,90 };
					// Clip Viewed Triangle against near plane, this could form two additional
					// additional triangles. 
					int nClippedTriangles = 0;
					triangle clipped[2];
					nClippedTriangles = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

					// We may end up with multiple triangles form the clip, so project as
					// required
					for (int n = 0; n < nClippedTriangles; n++)
					{
						// Project triangles from 3D --> 2D
						triProjected.p[0] = Matrix_MultiplyVector(matProj, clipped[n].p[0]);
						triProjected.p[1] = Matrix_MultiplyVector(matProj, clipped[n].p[1]);
						triProjected.p[2] = Matrix_MultiplyVector(matProj, clipped[n].p[2]);

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
						vecTrianglesToRasterWall.push_back(triProjected);

					}
				}
			}
			// Sort triangles from back to front
			sort(vecTrianglesToRasterWall.begin(), vecTrianglesToRasterWall.end(), [](triangle& t1, triangle& t2)
				{
					float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
					float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
					return z1 > z2;
				});
			for (auto& triProjected : vecTrianglesToRasterWall)
			{
				rgba a = { 150,60,60,255 };
				//Rasterize the triangle
				FillTriangle(renderer, triProjected.p[0].x, triProjected.p[0].y, triProjected.p[1].x, triProjected.p[1].y,
					triProjected.p[2].x, triProjected.p[2].y, a);

				drawFlag = false;
				SDL_RenderPresent(renderer);
			}
			
			// Draw Dhara-----------------------------------------------------------------------------------
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
					vec3d light_direction = { 5.0f, 1.0f, -1.0f };
					light_direction = Vector_Normalise(light_direction);

					// How similar is normal to light direction
					float dp = normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;

					c = GetColor(dp);

					triViewed.p[0] = Matrix_MultiplyVector(matView, triTransformed.p[0]);
					triViewed.p[1] = Matrix_MultiplyVector(matView, triTransformed.p[1]);
					triViewed.p[2] = Matrix_MultiplyVector(matView, triTransformed.p[2]);
					triViewed.col = c;
					// Clip Viewed Triangle against near plane, this could form two additional
					// additional triangles. 
					int nClippedTriangles = 0;
					triangle clipped[2];
					nClippedTriangles = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

					// We may end up with multiple triangles form the clip, so project as
					// required
					for (int n = 0; n < nClippedTriangles; n++)
					{
						// Project triangles from 3D --> 2D
						triProjected.p[0] = Matrix_MultiplyVector(matProj, clipped[n].p[0]);
						triProjected.p[1] = Matrix_MultiplyVector(matProj, clipped[n].p[1]);
						triProjected.p[2] = Matrix_MultiplyVector(matProj, clipped[n].p[2]);

						// do this normalizing
						triProjected.p[0] = Vector_Div(triProjected.p[0], triProjected.p[0].w);
						triProjected.p[1] = Vector_Div(triProjected.p[1], triProjected.p[1].w);
						triProjected.p[2] = Vector_Div(triProjected.p[2], triProjected.p[2].w);
						triProjected.col = c;
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
				//Rasterize the triangle
				FillTriangle(renderer, triProjected.p[0].x, triProjected.p[0].y, triProjected.p[1].x, triProjected.p[1].y,
					triProjected.p[2].x, triProjected.p[2].y, triProjected.col);

				drawFlag = false;
				SDL_RenderPresent(renderer);
			}

			// Draw Water-----------------------------------------------------------------------------------
			for (auto tri : water.tris)
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



					triViewed.p[0] = Matrix_MultiplyVector(matView, triTransformed.p[0]);
					triViewed.p[1] = Matrix_MultiplyVector(matView, triTransformed.p[1]);
					triViewed.p[2] = Matrix_MultiplyVector(matView, triTransformed.p[2]);
					triViewed.col = { 180,125,90 };
					// Clip Viewed Triangle against near plane, this could form two additional
					// additional triangles. 
					int nClippedTriangles = 0;
					triangle clipped[2];
					nClippedTriangles = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

					// We may end up with multiple triangles form the clip, so project as
					// required
					for (int n = 0; n < nClippedTriangles; n++)
					{
						// Project triangles from 3D --> 2D
						triProjected.p[0] = Matrix_MultiplyVector(matProj, clipped[n].p[0]);
						triProjected.p[1] = Matrix_MultiplyVector(matProj, clipped[n].p[1]);
						triProjected.p[2] = Matrix_MultiplyVector(matProj, clipped[n].p[2]);

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
						vecTrianglesToRasterWater.push_back(triProjected);

					}
				}
			}
			// Sort triangles from back to front
			sort(vecTrianglesToRasterWater.begin(), vecTrianglesToRasterWater.end(), [](triangle& t1, triangle& t2)
				{
					float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
					float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
					return z1 > z2;
				});
			for (auto& triProjected : vecTrianglesToRasterWater)
			{
				rgba a = { 65,104,154,255 };
				//Rasterize the triangle
				FillTriangle(renderer, triProjected.p[0].x, triProjected.p[0].y, triProjected.p[1].x, triProjected.p[1].y,
					triProjected.p[2].x, triProjected.p[2].y, a);

				drawFlag = false;
				SDL_RenderPresent(renderer);
			}

		}

	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}