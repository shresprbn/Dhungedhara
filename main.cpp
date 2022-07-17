#include <graphics.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

using namespace std;

struct vec3d{
    float x , y, z;
}; 

struct triangle
{
    vec3d points[3];
};

struct mesh
{
    vector<triangle> triangles;
};

struct matrix4x4{
   float any[4][4] = {0};

};
class meshPopulate{
   private:
      mesh meshcube;
      int windowWidth = 900, windowHeight =700; //for calculating aspect ratio
      float aspectRatio;
      float fTheta;
      float znear, zfar, fov; 
      matrix4x4 projmatrix;
      

      void MultiplyMatrixVector(vec3d &i, vec3d &o, matrix4x4 &any)
	   {
		   o.x = i.x * any.any[0][0] + i.y * any.any[1][0] + i.z * any.any[2][0] + any.any[3][0];
   		o.y = i.x * any.any[0][1] + i.y * any.any[1][1] + i.z * any.any[2][1] + any.any[3][1];
	   	o.z = i.x * any.any[0][2] + i.y * any.any[1][2] + i.z * any.any[2][2] + any.any[3][2];
	   	float w = i.x * any.any[0][3] + i.y * any.any[1][3] + i.z * any.any[2][3] + any.any[3][3];

		   if (w != 0.0f)
	   	{
	   		o.x /= w; o.y /= w; o.z /= w;
		   }
	}
   public:
      meshPopulate()
      {
         meshcube.triangles = {

            //south 
            {0.0f, 0.0f, 0.0f,  0.0f ,1.0f ,0.0f ,   1.0f ,1.0f ,0.0f },
            {0.0f, 0.0f, 0.0f,  0.0f ,1.0f ,0.0f ,   1.0f ,0.0f ,0.0f },

            // EAST                                                      
	      	{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
   	   	{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

	   	   // NORTH                                                     
	      	{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
      		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

    	   	// WEST                                                      
   	   	{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
      		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

      		// TOP                                                       
   	   	{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		      { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

   	   	// BOTTOM                                                    
	   	   { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
	      	{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

         };
      
      aspectRatio = float(windowHeight)/float(windowWidth);
      znear = 100;
      zfar = 10000;
      fov = 90.0;
      float fovrad = 1.0f/tanf(fov * 0.5f / 180.0f * 3.14159f);
      projmatrix.any[0][0]= aspectRatio*fovrad;
      projmatrix.any[1][1]= fovrad;
      projmatrix.any[2][2]= zfar/(zfar-znear);
      projmatrix.any[2][3]= 1.0f;
      projmatrix.any[3][3]= 0.0f;
      projmatrix.any[3][2]= (-znear*zfar)/(zfar-znear);
      
         
      }

      void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3 ){
         line(x1,y1,x2,y2);
         line(x1,y1,x3,y3);
         line(x2,y2,x3,y3);

      }
      void drawcycle(){
         
            // Set up rotation matrices
	   	   matrix4x4 matRotZ, matRotX;
	         fTheta += .005f  ;

		      // Rotation Z
	      	matRotZ.any[0][0] = cosf(fTheta);
	      	matRotZ.any[0][1] = sinf(fTheta);
    	   	matRotZ.any[1][0] = -sinf(fTheta);
	      	matRotZ.any[1][1] = cosf(fTheta);
   	   	matRotZ.any[2][2] = 1;
   	   	matRotZ.any[3][3] = 1;

	      	// Rotation X
	      	matRotX.any[0][0] = 1;
	       	matRotX.any[1][1] = cosf(fTheta * 0.5f);
	       	matRotX.any[1][2] = sinf(fTheta * 0.5f);
	      	matRotX.any[2][1] = -sinf(fTheta * 0.5f);
	      	matRotX.any[2][2] = cosf(fTheta * 0.5f);
	      	matRotX.any[3][3] = 1;
         
         for(auto tri : meshcube.triangles){
            triangle triprojected, tritranslated, trirotatedX, trirotatedXZ;
            
            //rotation x
            MultiplyMatrixVector(tri.points[0], trirotatedX.points[0], matRotX);
            MultiplyMatrixVector(tri.points[1], trirotatedX.points[1], matRotX);
            MultiplyMatrixVector(tri.points[2], trirotatedX.points[2], matRotX);

            //rotation Z
            MultiplyMatrixVector(trirotatedX.points[0], trirotatedXZ.points[0], matRotZ);
            MultiplyMatrixVector(trirotatedX.points[1], trirotatedXZ.points[1], matRotZ);
            MultiplyMatrixVector(trirotatedX.points[2], trirotatedXZ.points[2], matRotZ);

            tritranslated =trirotatedXZ;
            tritranslated.points[0].z += 3.0f;
            tritranslated.points[1].z += 3.0f;
            tritranslated.points[2].z += 3.0f;
            
            // MultiplyMatrixVector(tri.points[0], triprojected.points[0], projmatrix);
            // MultiplyMatrixVector(tri.points[1], triprojected.points[1], projmatrix);
            // MultiplyMatrixVector(tri.points[2], triprojected.points[2], projmatrix);

            MultiplyMatrixVector(tritranslated.points[0], triprojected.points[0], projmatrix);
            MultiplyMatrixVector(tritranslated.points[1], triprojected.points[1], projmatrix);
            MultiplyMatrixVector(tritranslated.points[2], triprojected.points[2], projmatrix);

            //scale The triangle
            //bring it to center as project garda it is not centered
            triprojected.points[0].x += 1.0f; triprojected.points[0].y += 1.0f;
            triprojected.points[1].x += 1.0f; triprojected.points[1].y += 1.0f;
            triprojected.points[2].x += 1.0f; triprojected.points[2].y += 1.0f;

            //scale it to window size
            triprojected.points[0].x *= 0.5f * windowWidth;
            triprojected.points[0].y *= 0.5f * windowHeight;
            triprojected.points[1].x *= 0.5f * windowWidth;
            triprojected.points[1].y *= 0.5f * windowHeight;
            triprojected.points[2].x *= 0.5f * windowWidth;
            triprojected.points[2].y *= 0.5f * windowHeight;
            

            //draw the triangle
            drawTriangle(triprojected.points[0].x,triprojected.points[0].y,triprojected.points[1].x,triprojected.points[1].y,
            triprojected.points[2].x,triprojected.points[2].y);
            
         }
         
      }
};



int main() {
   int gd = DETECT, gm;
   float aspectRatio;
   initgraph(&gd, &gm, "C:\\TC\\BGI");
   initwindow(1000,1000, "the cube");
   meshPopulate m1;
   for(int i =0 ; i< 50000000; i++){
   m1.drawcycle();   
   cleardevice();
   }

   getch();
   closegraph();
   return 0;
}