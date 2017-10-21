#ifndef _POKEMON_MODEL_01_H
#define _POKEMON_MODEL_01_H

#include "demo_base.h"
#include "lodepng.h"
#include <iostream>
#define TEXTURE_COUNT 2

using namespace std;

class PokemonModel : public DemoBase
{
private:
	float m_rotation;
	float m_rotation2;
	float value2;
	float sinWaveMovementTimer;
	float time;
	GLuint mTextureID[TEXTURE_COUNT];

	void loadPNG(const char* path, GLuint textureID) {

		//x Load file and decode image
		vector<unsigned char> image;
		unsigned width, height;
		unsigned error = lodepng::decode(image, width, height, path);

		//x If there is an error, display it
		if (error != 0) {
			cout<<"PNG load error: " << error << ": " << lodepng_error_text(error)<<endl;
				return;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //x Horizontal wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //x Vertical wrapping

		//Bilinear Texturing
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //x Near filtering (For scaling up)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //x Far filtering (For scaling down)

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	}

public:
	void init()
	{
		m_rotation = 0;
		m_rotation2 = 0;
		value2 = 0;
		sinWaveMovementTimer = 0;
		time = 0;
		glGenTextures(TEXTURE_COUNT, mTextureID);
		loadPNG("../media/glass.png", mTextureID[0]);

	}

	void deinit()
	{
	}

	void drawAxis(const Matrix& viewMatrix)
	{
		// ============draw axis.
		glLoadMatrixf((GLfloat*)viewMatrix.mVal);
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.3f, 0.3f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);

		glColor3f(0.3f, 1.0f, 0.3f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);

		glColor3f(0.3f, 0.3f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);
		glEnd();
		// ============================
	}

	void drawTriangle(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//drawAxis(viewMatrix);

		//glLoadMatrixf((GLfloat*)viewMatrix.mVal);


		glColor4f(red, green, blue, alpha);
		glBegin(GL_TRIANGLES);								// Drawing Using Triangles
		glVertex3f(1.0f, 1.0f, 0.0f);					// Top
		glVertex3f(1.0f, -1.0f, 0.0f);					// Bottom Left
		glVertex3f(2.0f, -1.0f, 0.0f);					// Bottom Right
		glEnd();											// Finished Drawing The Triangle
	}
	void drawCube(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
		//glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, mTextureID[0]);
		glColor4f(red, green, blue, alpha);
		glBegin(GL_TRIANGLES);

		glTexCoord3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f); // triangle 1 : begin
		glTexCoord3f(1.0f, 1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
		glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.0f); // triangle 1 : end

		glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.0f); // triangle 2 : begin
		glTexCoord3f(0.0f, 0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glTexCoord3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f); // triangle 2 : end
		


		glTexCoord3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 2.0f);
		glTexCoord3f(1.0f, 1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 2.0f);
		glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 2.0f);

		glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 2.0f);
		glTexCoord3f(0.0f, 0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 2.0f);
		glTexCoord3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 2.0f);



		glTexCoord3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		glTexCoord3f(1.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 2.0f);
		glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 2.0f);

		glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 2.0f);
		glTexCoord3f(0.0f, 0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glTexCoord3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		


		glTexCoord3f(0.0f, 1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
		glTexCoord3f(1.0f, 1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 2.0f);
		glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 2.0f);

		glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 2.0f);
		glTexCoord3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.0f);
		glTexCoord3f(0.0f, 1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
		


		glTexCoord3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		glTexCoord3f(1.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 2.0f);
		glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 2.0f);

		glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 2.0f);
		glTexCoord3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
		glTexCoord3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		


		glTexCoord3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glTexCoord3f(1.0f, 1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 2.0f);
		glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 2.0f);

		glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 2.0f);
		glTexCoord3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.0f);
		glTexCoord3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();
	}
	void drawCylinder(GLfloat radius, GLfloat height, GLfloat polySize, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
		double theta = M_2PI / polySize;

		glBegin(GL_TRIANGLES);
		glColor4f(red, green, blue, alpha);

		for (double i = 0; i < M_2PI; i += theta) {

			float startX = radius * cos(i);
			float endX = radius * cos(i + theta);
			float startZ = radius * sin(i);
			float endZ = radius * sin(i + theta);

			//x Top Circle
			glVertex3f(0.0f, height / 2, 0.0f); //x Starting point of Top Circle
			glVertex3f(startX, height / 2, startZ);
			glVertex3f(endX, height / 2, endZ);

			//x Bottom Circle
			glVertex3f(0.0f, -height / 2, 0.0f);
			glVertex3f(startX, -height / 2, startZ);
			glVertex3f(endX, -height / 2, endZ);

			//x Top Tube Triangles
			glVertex3f(startX, height / 2, startZ);
			glVertex3f(endX, height / 2, endZ);
			glVertex3f(startX, -height / 2, startZ);

			//x Bottom Tube Triangles
			glVertex3f(endX, height / 2, endZ);
			glVertex3f(startX, -height / 2, startZ);
			glVertex3f(endX, -height / 2, endZ);
		}
		glEnd();
	}

	void drawWing(GLfloat radius, GLfloat height, GLfloat polySize, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		double theta = M_2PI / polySize;
		glBegin(GL_TRIANGLES);
		glColor4f(red, green, blue, alpha);
		for (double i = 3 * M_PI / 4; i < 9 * M_PI / 4; i += theta) {

			float startX = radius * cos(i);
			float endX = radius * cos(i + theta);
			float startZ = radius * sin(i);
			float endZ = radius * sin(i + theta);

				//x Top Circle
				glVertex3f(0.0f, height / 2, 0.0f); //x Starting point of Top Circle
				glVertex3f(startX, height / 2, startZ);
				glVertex3f(endX, height / 2, endZ);

				//x Close the Sides
				glVertex3f(0.0f, height / 2, 0.0f);
				glVertex3f(startX, height / 2, startZ);
				glVertex3f(startX, -height / 2, startZ);

				glVertex3f(0.0f, height / 2, 0.0f);
				glVertex3f(endX, height / 2, endZ);
				glVertex3f(endX, -height / 2, endZ);

				//x Bottom Circle
				glVertex3f(0.0f, -height / 2, 0.0f);
				glVertex3f(startX, -height / 2, startZ);
				glVertex3f(endX, -height / 2, endZ);

				//x Close the sides

				glVertex3f(0.0f, -height / 2, 0.0f);
				glVertex3f(0.0f, height / 2, 0.0f);
				glVertex3f(startX, -height / 2, startZ);

				glVertex3f(0.0f, -height / 2, 0.0f);
				glVertex3f(0.0f, height / 2, 0.0f);
				glVertex3f(endX, -height / 2, endZ);

				//x Top Tube Triangles
				glVertex3f(startX, height / 2, startZ);
				glVertex3f(endX, height / 2, endZ);
				glVertex3f(startX, -height / 2, startZ);

				//x Bottom Tube Triangles
				glVertex3f(endX, height / 2, endZ);
				glVertex3f(startX, -height / 2, startZ);
				glVertex3f(endX, -height / 2, endZ);
		}
		glEnd();
	}

	void drawCone(GLfloat radius, GLfloat height, GLfloat polySize, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
		double theta = M_2PI / polySize;
		glBegin(GL_TRIANGLES);
		glColor4f(red, green, blue, alpha);

		for (double i = 0; i < M_2PI; i += theta) {

			float startX = radius * cos(i);
			float endX = radius * cos(i + theta);
			float startZ = radius * sin(i);
			float endZ = radius * sin(i + theta);

			//x Top point
			glVertex3f(0.0f, height / 2, 0.0f); //x Starting point of Top Circle


			glVertex3f(startX, -height / 2, startZ);
			glVertex3f(endX, -height / 2, endZ);

			//x Bottom Circle
			glVertex3f(0.0f, -height / 2, 0.0f);
			glVertex3f(startX, -height / 2, startZ);
			glVertex3f(endX, -height / 2, endZ);
		}
		glEnd();
	}

	void drawEllipsoid(GLfloat radius, GLfloat polySize, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_TRIANGLES);
		glColor4f(red, green, blue, alpha);

		double theta = M_2PI / polySize;

		for (double i = -(M_PI / 2); i < M_PI / 2; i += theta) {
			float i_startX = radius * cos(i);
			float i_endX = radius * cos(i + theta);
			float i_startZ = radius * sin(i);
			float i_endZ = radius * sin(i + theta);

			for (double j = 0; j < M_2PI; j += theta) {
				float j_startX = radius * cos(j);
				float j_endX = radius * cos(j + theta);
				float j_startZ = radius * sin(j);
				float j_endZ = radius * sin(j + theta);

				glVertex3f(j_startX * i_endX, i_endZ, j_startZ * i_endX);
				glVertex3f(j_endX * i_endX, i_endZ, j_endZ * i_endX);
				glVertex3f(j_endX * i_startX, i_startZ, j_endZ * i_startX);

				glVertex3f(j_startX * i_startX, i_startZ, j_startZ * i_startX);
				glVertex3f(j_endX * i_startX, i_startZ, j_endZ * i_startX);
				glVertex3f(j_startX * i_endX, i_endZ, j_startZ * i_endX);
			}
		}

		glEnd();
	}

	void drawHemisphere(GLfloat radius, GLfloat polySize, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_TRIANGLES);
		glColor4f(red, green, blue, alpha);

		double theta = M_2PI / polySize;

		for (double i = 0; i < M_PI; i += theta) {
			float i_startX = radius * cos(i);
			float i_endX = radius * cos(i + theta);
			float i_startZ = radius * sin(i);
			float i_endZ = radius * sin(i + theta);

			for (double j = 0; j < M_2PI; j += theta) {
				float j_startX = radius * cos(j);
				float j_endX = radius * cos(j + theta);
				float j_startZ = radius * sin(j);
				float j_endZ = radius * sin(j + theta);

				glVertex3f(j_startX * i_endX, i_endZ, j_startZ * i_endX);
				glVertex3f(j_endX * i_endX, i_endZ, j_endZ * i_endX);
				glVertex3f(j_endX * i_startX, i_startZ, j_endZ * i_startX);

				glVertex3f(j_startX * i_startX, i_startZ, j_startZ * i_startX);
				glVertex3f(j_endX * i_startX, i_startZ, j_endZ * i_startX);
				glVertex3f(j_startX * i_endX, i_endZ, j_startZ * i_endX);

				//x Circle
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(j_startX, 0.0f, j_startZ);
				glVertex3f(j_endX, 0.0f, j_endZ);
			}
		}

		glEnd();
	}

	void drawTriangularPrism(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
		glColor4f(red, green, blue, alpha);

		glBegin(GL_TRIANGLES);

		//x Bottom Triangle
		glVertex3f(-1.0f, 0.0f, -1.0f);					
		glVertex3f(0.0f, 0.0f, 1.0f);					
		glVertex3f(1.0f, 0.0f, -1.0f);

		glVertex3f(1.0f, 0.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);

		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 0.0f, -1.0f);

		glVertex3f(1.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);

		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(0.0f, 1.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);

		//x Top Triangle
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(0.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glEnd();
	}

	void drawRightAngledTrianglePrism(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
		glColor4f(red, green, blue, alpha);

		GLfloat rightAngledTriangle[] {
			//x Bottom rect
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,

			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,

			//x Vertical rect
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,

			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,

			//x Hypotenuse rect
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,

			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,

			//x Left triangle
			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,

			//x Right triangle
			1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f
		};

		//x Activate and specify pointer to vertex array
		glEnableClientState(GL_VERTEX_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, rightAngledTriangle);

		//x Draw rectangle
		glDrawArrays(GL_TRIANGLES, 0, 24);

		//x Deactivate vertex array after drawing
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void drawBody(const Matrix& viewMatrix, float sinWaveMovementAnimateValue) {
		
		//x Draw Body
		Matrix translateBody = Matrix::makeTranslationMatrix(0.0f, -1.9f + sinWaveMovementAnimateValue, 0.0f);
		Matrix modelMatrix = translateBody;
		Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawEllipsoid(2, 40, 0, 0, 0.5, 1);



		//x Draw ChestpieceLeft_Bottom
		Matrix ScaleChestpiece_Bottom = Matrix::makeScaleMatrix(0.5f, 0.1f, 4.5f);
		Matrix translateLeftChestpiece = Matrix::makeTranslationMatrix(-1.0f, -3.8f + sinWaveMovementAnimateValue, -3.0f);
		Matrix RotateLeftChestpiece_Bottom = Matrix::makeRotateMatrix(5.0f, Vector(0.0f, 1.0f, 0.0f));
		modelMatrix = translateLeftChestpiece * RotateLeftChestpiece_Bottom * ScaleChestpiece_Bottom;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawCube(0.5, 0.5, 0.5, 1);

		//x Draw ChestpieceRight_Bottom
		Matrix translateRightChestpiece = Matrix::makeTranslationMatrix(1.0f, -3.8f + sinWaveMovementAnimateValue, -3.0f);
		Matrix RotateRightChestpiece_Bottom = Matrix::makeRotateMatrix(-5.0f, Vector(0.0f, 1.0f, 0.0f));
		modelMatrix = translateRightChestpiece * RotateRightChestpiece_Bottom * ScaleChestpiece_Bottom;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawCube(0.5, 0.5, 0.5, 1);

		//x Draw ChestpieceRight_Upper
		Matrix ScaleChestpiece_Upper = Matrix::makeScaleMatrix(0.5f, 0.05f, 2.2f);
		Matrix RotateChestpiece_Upper = Matrix::makeRotateMatrix(132.0f, Vector(1.0f, 0.0f, 0.0f));
		Matrix Rotate2Chestpiece_Upper = Matrix::makeRotateMatrix(-5.0f, Vector(0.0f, 1.0f, 0.0f));
		Matrix translateRightChestpiece_Upper = Matrix::makeTranslationMatrix(1.25f, -3.73f + sinWaveMovementAnimateValue, 6.05f);
		modelMatrix = Rotate2Chestpiece_Upper * translateRightChestpiece_Upper * RotateChestpiece_Upper * ScaleChestpiece_Upper;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawCube(0.5, 0.5, 0.5, 1);

		//x Draw ChestpieceLeft_Upper
		Matrix Rotate2Chestpiece2_Upper = Matrix::makeRotateMatrix(5.0f, Vector(0.0f, 1.0f, 0.0f));
		Matrix translateLeftChestpiece_Upper = Matrix::makeTranslationMatrix(-1.25f, -3.73f + sinWaveMovementAnimateValue, 6.05f);
		modelMatrix = Rotate2Chestpiece2_Upper * translateLeftChestpiece_Upper * RotateChestpiece_Upper * ScaleChestpiece_Upper;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawCube(0.5, 0.5, 0.5, 1);

		//x Draw ChestpieceMiddle_Rect
		Matrix translateChestpieceMiddle_Rect = Matrix::makeTranslationMatrix(0.0f, -3.41f + sinWaveMovementAnimateValue, -3.0f);
		Matrix scaleChestpieceMiddle_Rect = Matrix::makeScaleMatrix(0.7f, 0.5f, 2.0f);
		modelMatrix = translateChestpieceMiddle_Rect * scaleChestpieceMiddle_Rect;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawCube(0.4, 0.4, 0.4, 1);

		//x Draw ChestpieceMiddle_Triangle
		Matrix translateChestpieceMiddle_Triangle = Matrix::makeTranslationMatrix(0.0f, -3.92f + sinWaveMovementAnimateValue, 2.0f);
		Matrix scaleChestpieceMiddle_Triangle = Matrix::makeScaleMatrix(0.7f, 0.8f, 1.0f);
		modelMatrix = translateChestpieceMiddle_Triangle * scaleChestpieceMiddle_Triangle;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawTriangularPrism(0.4, 0.4, 0.4, 1);


		//x Draw ChestpieceRight_RightAngledTriangle
		Matrix translateChestpieceRight_RightAngledTriangle = Matrix::makeTranslationMatrix(1.05f, -2.83f + sinWaveMovementAnimateValue, -2.0f);
		Matrix scaleChestpiece_RightAngledTriangle = Matrix::makeScaleMatrix(0.54f, 1.0f, 1.0f);
		Matrix rotateChestpieceRight_RightAngledTriangle = Matrix::makeRotateMatrix(-5.0f, Vector(0.0f, 1.0f, 0.0f));
		modelMatrix = translateChestpieceRight_RightAngledTriangle * rotateChestpieceRight_RightAngledTriangle * scaleChestpiece_RightAngledTriangle;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawRightAngledTrianglePrism(0.5, 0.5, 0.5, 1);

		//x Draw ChestpieceLeft_RightAngledTriangle
		Matrix translateChestpieceLeft_RightAngledTriangle = Matrix::makeTranslationMatrix(-1.05f, -2.83f + sinWaveMovementAnimateValue, -2.0f);
		Matrix rotateChestpieceLeft_RightAngledTriangle = Matrix::makeRotateMatrix(5.0f, Vector(0.0f, 1.0f, 0.0f));
		modelMatrix = translateChestpieceLeft_RightAngledTriangle * rotateChestpieceLeft_RightAngledTriangle * scaleChestpiece_RightAngledTriangle;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawRightAngledTrianglePrism(0.5, 0.5, 0.5, 1);
	}

	void drawLeftForeLeg(const Matrix& viewMatrix, float sinWaveMovementAnimateValue) {

		float leftForeLeg_MainAnimateValue = -10.0f * sin(time);

		if (leftForeLeg_MainAnimateValue > 0) {
			leftForeLeg_MainAnimateValue = -leftForeLeg_MainAnimateValue;
		}

		//x Draw LeftForeLeg_Main
		Matrix translateLeftForeLeg_Main = Matrix::makeTranslationMatrix(1.8f, -2.5f + sinWaveMovementAnimateValue, 3.3f);
		Matrix scaleForeLeg_Main = Matrix::makeScaleMatrix(0.75f, 0.75f, 1.5f);
		Matrix rotateLeftForeLeg_Main = Matrix::makeRotateMatrix(-7.0f, Vector(1.0f, 0.0f, 0.0f));
		Matrix animateLeftForeLeg = Matrix::makeRotateMatrix(leftForeLeg_MainAnimateValue, Vector(0.0f, 1.0f, 0.0f));
		Matrix modelMatrix = rotateLeftForeLeg_Main * animateLeftForeLeg * translateLeftForeLeg_Main * scaleForeLeg_Main;
		Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawEllipsoid(1, 40, 0, 0, 0.4, 1);

		//x Draw LeftForeLeg_Nails
		for (float i = 0; i < 3; i++) {
			Matrix translateLeftForeLeg_Nails = Matrix::makeTranslationMatrix((i * 0.4 * 0.7) + 1.5f - leftForeLeg_MainAnimateValue * 0.08, -3.3f + sinWaveMovementAnimateValue, 4.7f + leftForeLeg_MainAnimateValue * 0.05);
			Matrix scaleLeftForeLeg_Nails = Matrix::makeScaleMatrix(0.7, 1.1, 1.5);
			Matrix rotateLeftForeLeg_Nails = Matrix::makeRotateMatrix(-110.0f, Vector(1.0f, 0.0f, 0.0f));
			modelMatrix = translateLeftForeLeg_Nails * rotateLeftForeLeg_Nails * scaleLeftForeLeg_Nails;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			drawCone(0.2, 1, 40, 1, 1, 1, 1);
		}
	}

	void drawRightForeLeg(const Matrix& viewMatrix, float sinWaveMovementAnimateValue) {
		float rightForeLeg_MainAnimateValue = abs(10.0f * sin(time));


		//x Draw RightForeLeg_Main
		Matrix translateRightForeLeg_Main = Matrix::makeTranslationMatrix(-1.8f, -2.5f + sinWaveMovementAnimateValue, 3.3f);
		Matrix scaleForeLeg_Main = Matrix::makeScaleMatrix(0.75f, 0.75f, 1.5f);
		Matrix rotateRightForeLeg_Main = Matrix::makeRotateMatrix(-7.0f, Vector(1.0f, 0.0f, 0.0f));
		Matrix animateRightForeLeg = Matrix::makeRotateMatrix(rightForeLeg_MainAnimateValue, Vector(0.0f, 1.0f, 0.0f));
		Matrix modelMatrix = rotateRightForeLeg_Main * animateRightForeLeg * translateRightForeLeg_Main * scaleForeLeg_Main;
		Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawEllipsoid(1, 40, 0, 0, 0.4, 1);

		//x Draw RightForeLeg_Nails
		for (float i = 0; i < 3; i++) {
			Matrix translateRightForeLeg_Nails = Matrix::makeTranslationMatrix(-(i * 0.4 * 0.7) - 1.5f - rightForeLeg_MainAnimateValue * 0.08, -3.3f + sinWaveMovementAnimateValue, 4.7f - rightForeLeg_MainAnimateValue * 0.05);
			Matrix scaleRightForeLeg_Nails = Matrix::makeScaleMatrix(0.7, 1.1, 1.5);
			Matrix rotateRightForeLeg_Nails = Matrix::makeRotateMatrix(-110.0f, Vector(1.0f, 0.0f, 0.0f));
			modelMatrix = translateRightForeLeg_Nails * rotateRightForeLeg_Nails * scaleRightForeLeg_Nails;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			drawCone(0.2, 1, 40, 1, 1, 1, 1);

		}
	}

	void drawLeftHindLeg(const Matrix& viewMatrix, float sinWaveMovementAnimateValue) {

		//x Draw LeftHindLeg_Red
		Matrix translateLeftHindLeg_Red = Matrix::makeTranslationMatrix(2.2f, -2.5f + sinWaveMovementAnimateValue, -2.2f);
		Matrix modelMatrix = translateLeftHindLeg_Red;
		Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawEllipsoid(1, 40, 0.8, 0, 0, 1);

		//x Draw LeftHindLeg_Main
		Matrix translateLeftHindLeg_Main = Matrix::makeTranslationMatrix(2.7f, -3.1f + sinWaveMovementAnimateValue, -3.0f);
		Matrix scaleHindLeg_Main = Matrix::makeScaleMatrix(0.8f, 0.8f, 1.6f);
		modelMatrix = translateLeftHindLeg_Main * scaleHindLeg_Main;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawEllipsoid(1, 40, 0, 0, 0.4, 1);

		//x Draw LeftHindLeg_Nails
		for (float i = 0; i < 3; i++) {
			Matrix translateLeftHindLeg_Nails = Matrix::makeTranslationMatrix((i * 0.2 * 1.5) + 2.4f, -(i * 0.2 * 0.7) - 3.2f + sinWaveMovementAnimateValue, -4.9f);
			Matrix rotateLeftHindLeg_Nails = Matrix::makeRotateMatrix(100.0f, Vector(1.0f, 0.0f, 0.0f));
			Matrix scaleLeftHindLeg_Nails = Matrix::makeScaleMatrix(2.0f, 1.7f, 2.5f);
			modelMatrix = translateLeftHindLeg_Nails * rotateLeftHindLeg_Nails * scaleLeftHindLeg_Nails;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			drawCone(0.1, 1, 40, 1, 1, 1, 1);
		}
	}

	void drawRightHindLeg(const Matrix& viewMatrix, float sinWaveMovementAnimateValue) {
		//x Draw RightHindLeg_Red
		Matrix translateRightHindLeg_Red = Matrix::makeTranslationMatrix(-2.2f, -2.5f + sinWaveMovementAnimateValue, -2.2f);
		Matrix modelMatrix = translateRightHindLeg_Red;
		Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawEllipsoid(1, 40, 0.8, 0, 0, 1);

		//x Draw RightHindLeg_Main
		Matrix translateRightHindLeg_Main = Matrix::makeTranslationMatrix(-2.7f, -3.1f + sinWaveMovementAnimateValue, -3.0f);
		Matrix scaleHindLeg_Main = Matrix::makeScaleMatrix(0.8f, 0.8f, 1.6f);
		modelMatrix = translateRightHindLeg_Main * scaleHindLeg_Main;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawEllipsoid(1, 40, 0, 0, 0.4, 1);

		//x Draw RightHindLeg_Nails
		for (float i = 0; i < 3; i++) {
			Matrix translateRightHindLeg_Nails = Matrix::makeTranslationMatrix(-(i * 0.2 * 1.5) - 2.4f, -(i * 0.2 * 0.7) - 3.2f + sinWaveMovementAnimateValue, -4.9f);
			Matrix rotateRightHindLeg_Nails = Matrix::makeRotateMatrix(100.0f, Vector(1.0f, 0.0f, 0.0f));
			Matrix scaleRightHindLeg_Nails = Matrix::makeScaleMatrix(2.0f, 1.7f, 2.5f);
			modelMatrix = translateRightHindLeg_Nails * rotateRightHindLeg_Nails * scaleRightHindLeg_Nails;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			drawCone(0.1, 1, 40, 1, 1, 1, 1);
		}
	}

	void drawTail(const Matrix& viewMatrix, GLfloat magnitude, float sinWaveMovementAnimateValue) {
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		for (int i = 0; i < 18; i++) {
			//x Draw Tail_Blue
			Matrix translateSphereTail_Blue = Matrix::makeTranslationMatrix(0.0f,(magnitude * sin(i + time)) - 2.0f + sinWaveMovementAnimateValue,  -(i * 0.05) * 5 - 4.0f);
			Matrix scaleSphereTail_Blue = Matrix::makeScaleMatrix(0.5f - (i * 0.02), 0.5f - (i * 0.02), 0.5f - (i * 0.02));
			Matrix modelMatrix = translateSphereTail_Blue * scaleSphereTail_Blue;
			Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			drawEllipsoid(1, 20, 0, 0, 0.5, 1);

			//x Draw Tail_Red
			Matrix translateSphereTail_Red = Matrix::makeTranslationMatrix(0.0f, (magnitude * sin(i + time)) - 2.12f + sinWaveMovementAnimateValue, -(i * 0.05) * 5 - 4.0f);
			Matrix scaleSphereTail_Red = Matrix::makeScaleMatrix(0.4f - (i * 0.02), 0.4f - (i * 0.02), 0.4f - (i * 0.02));
			modelMatrix = translateSphereTail_Red * scaleSphereTail_Red;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			drawEllipsoid(1, 20, 0.4, 0, 0, 1);
		}
	}

	void drawNeck(const Matrix& viewMatrix, float sinWaveMovementAnimateValue) {
		//x Draw Neck_Blue
		Matrix translateNeck_Blue = Matrix::makeTranslationMatrix(0.0f, -1.0f + sinWaveMovementAnimateValue, 4.5f);
		Matrix scaleNeck_Blue = Matrix::makeScaleMatrix(0.8f, 0.8f, 0.45f);
		Matrix rotateNeck = Matrix::makeRotateMatrix(-80.0f, Vector(1.0f, 0.0f, 0.0f));
		Matrix modelMatrix = translateNeck_Blue * rotateNeck * scaleNeck_Blue;
		Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawCylinder(0.7, 5, 40, 0, 0, 0.4, 1);

		//x Draw Neck_Red
		Matrix translateNeck_Red = Matrix::makeTranslationMatrix(0.0f, -1.1f + sinWaveMovementAnimateValue, 4.5f);
		Matrix scaleNeck_Red = Matrix::makeScaleMatrix(0.8f, 0.8f, 0.45f);
		modelMatrix = translateNeck_Red * rotateNeck * scaleNeck_Red;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawCylinder(0.6, 5, 40, 1, 0, 0, 1);

		//x Draw NeckSpikes_Red
			Matrix translateAllNeckSpikes_Red = Matrix::makeTranslationMatrix(0.0f, -1.9f + sinWaveMovementAnimateValue, 4.0f);
			Matrix scaleAllNeckSpikes_Red = Matrix::makeScaleMatrix(1.0f, 0.6f, 1.0f);


			Matrix translateNeckSpikes_Red = Matrix::makeTranslationMatrix(0.0f, 0.0f, 0.0f);
			Matrix rotateNeckSpikes_Red = Matrix::makeRotateMatrix(170.0f, Vector(1.0f, 0.0f, 0.0f));
			modelMatrix = translateAllNeckSpikes_Red * translateNeckSpikes_Red * rotateNeckSpikes_Red * scaleAllNeckSpikes_Red;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			drawCone(0.3, 1.5, 20, 1, 0, 0, 1);

			translateNeckSpikes_Red = Matrix::makeTranslationMatrix(0.4f, 0.1f, 0.0f);
			Matrix rotate2NeckSpikes_Red = Matrix::makeRotateMatrix(-30.0f, Vector(0.0f, 0.0f, 1.0f));
			modelMatrix = translateAllNeckSpikes_Red * translateNeckSpikes_Red * rotate2NeckSpikes_Red * rotateNeckSpikes_Red * scaleAllNeckSpikes_Red;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			drawCone(0.3, 1.5, 20, 1, 0, 0, 1);

			translateNeckSpikes_Red = Matrix::makeTranslationMatrix(-0.4f, 0.1f, 0.0f);
			rotate2NeckSpikes_Red = Matrix::makeRotateMatrix(30.0f, Vector(0.0f, 0.0f, 1.0f));
			modelMatrix = translateAllNeckSpikes_Red * translateNeckSpikes_Red * rotate2NeckSpikes_Red * rotateNeckSpikes_Red * scaleAllNeckSpikes_Red;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			drawCone(0.3, 1.5, 20, 1, 0, 0, 1);
	}

	void drawHead(const Matrix& viewMatrix, float sinWaveMovementAnimateValue) {

		float jawAnimationValue = abs(10.0f * (sin(time)));


		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//x Animate rotate2Upper Jaw and LowerJaw

		//x Lower Jaw
		Matrix translateLowerJaw = Matrix::makeTranslationMatrix(0.0f, -0.9f + sinWaveMovementAnimateValue, 7.4f);
		Matrix rotateLowerJaw = Matrix::makeRotateMatrix(180.0f, Vector(0.0f, 0.0f, 1.0f));
		Matrix rotate2LowerJaw = Matrix::makeRotateMatrix(jawAnimationValue, Vector(1.0f, 0.0f, 0.0f));
		Matrix scaleLowerJaw = Matrix::makeScaleMatrix(0.7f, 0.7f, 1.4f);
		Matrix modelMatrix = translateLowerJaw * rotateLowerJaw * rotate2LowerJaw * scaleLowerJaw;
		Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawHemisphere(1, 40, 1, 0, 0, 1);

		//x Tongue
		Matrix translateTongue = Matrix::makeTranslationMatrix(0.0f, -0.9f + sinWaveMovementAnimateValue, 7.4f);
		Matrix scaleTongue = Matrix::makeScaleMatrix(0.6f, 0.2f, 1.3f);
		modelMatrix = translateTongue * rotateLowerJaw * rotate2LowerJaw * scaleTongue;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawEllipsoid(1, 20, 1, 0.3, 0.7, 1);

		//x Upper Jaw
		Matrix translateUpperJaw = Matrix::makeTranslationMatrix(0.0f, -0.9f + sinWaveMovementAnimateValue, 7.57f);
		Matrix rotateUpperJaw = Matrix::makeRotateMatrix(-180.0f, Vector(0.0f, 0.0f, 1.0f));
		Matrix rotate2UpperJaw = Matrix::makeRotateMatrix(jawAnimationValue, Vector(1.0f, 0.0f, 0.0f));
		Matrix scaleUpperJaw = Matrix::makeScaleMatrix(1.0f, 1.0f, 1.7f);
		modelMatrix = translateUpperJaw * rotate2UpperJaw * scaleUpperJaw;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawHemisphere(1, 40, 0, 0, 0.5, 1);

		//x Upper Fins
		for (int i = 0; i < 2; i++) {

			if (i == 1) {
				i++;
			}

			Matrix rotateUpperFins = Matrix::makeRotateMatrix(90.0f, Vector(0.0f, 1.0f, 0.0f));
			Matrix scaleUpperFins = Matrix::makeScaleMatrix(1.0f, 2.0f, 1.0f);
			Matrix translateUpperFin = Matrix::makeTranslationMatrix(1.6f - (i * 1.6f), 0.6f + sinWaveMovementAnimateValue, 6.0f);
			Matrix rotateUpperFin = Matrix::makeRotateMatrix(50.0f - (i * 50.0f) , Vector(1.0f, 0.0f, 0.0f));
			Matrix rotate2UpperFins = Matrix::makeRotateMatrix(10.0f - (i * 10.0f), Vector(0.0f, 1.0f, 0.0f));
			modelMatrix = translateUpperFin * rotateUpperFins * rotateUpperFin * rotate2UpperFins  * scaleUpperFins;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			drawTriangle(0, 0, 0.5, 1);
		}

		//x Middle Fins
		for (int i = 0; i < 2; i++) {

			if (i == 1) {
				i++;
			}

			Matrix rotateUpperFins = Matrix::makeRotateMatrix(90.0f, Vector(0.0f, 1.0f, 0.0f));

			Matrix translateUpperFin = Matrix::makeTranslationMatrix(1.7f - (i * 1.7f), -0.6f + sinWaveMovementAnimateValue, 6.0f);
			Matrix rotateUpperFin = Matrix::makeRotateMatrix(90.0f - (i * 90.0f), Vector(1.0f, 0.0f, 0.0f));
			Matrix rotate2UpperFins = Matrix::makeRotateMatrix(10.0f - (i * 10.0f), Vector(0.0f, 1.0f, 0.0f));
			modelMatrix = translateUpperFin * rotateUpperFins * rotateUpperFin * rotate2UpperFins;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			drawTriangle(0, 0, 0.5, 1);
		}

		//x Lower Fins
		for (int i = 0; i < 2; i++) {

			if (i == 1) {
				i++;
			}

			Matrix rotateUpperFins = Matrix::makeRotateMatrix(90.0f, Vector(0.0f, 1.0f, 0.0f));

			Matrix translateUpperFin = Matrix::makeTranslationMatrix(1.3f - (i * 1.3f), -1.4f + sinWaveMovementAnimateValue, 6.0f);
			Matrix rotateUpperFin = Matrix::makeRotateMatrix(135.0f - (i * 135.0f), Vector(1.0f, 0.0f, 0.0f));
			Matrix rotate2UpperFins = Matrix::makeRotateMatrix(10.0f - (i * 10.0f), Vector(0.0f, 1.0f, 0.0f));
			modelMatrix = translateUpperFin * rotateUpperFins * rotateUpperFin * rotate2UpperFins;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			drawTriangle(0, 0, 0.5, 1);
		}

		//x Red Fins

		for (int i = 0; i < 2; i++) {
			if (i == 1) {
				i++;
			}

			Matrix translateRedFins = Matrix::makeTranslationMatrix(0.25f - (i * 0.25f), -0.1f + sinWaveMovementAnimateValue, 7.9f);
			Matrix rotateRedFins = Matrix::makeRotateMatrix(120.0f -(i * 120.0f), Vector(0.0f, 0.0f, 1.0f));
			Matrix rotate2RedFins = Matrix::makeRotateMatrix(-20.0f + (i * 20.0f), Vector(0.0f, 1.0f, 0.0f));
			Matrix scaleRedFins = Matrix::makeScaleMatrix(0.8f, 2.5f, 2.1f);
			modelMatrix = translateRedFins * rotate2UpperJaw * rotateRedFins * rotate2RedFins* scaleRedFins;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			drawCylinder(0.5, 0.02, 20, 1, 0, 0, 1);
		}

		//x Eyes
		for (int i = 0; i < 2; i++) {
			if (i == 1) {
				i++;
			}

			Matrix translateEyes = Matrix::makeTranslationMatrix(0.25f - (i * 0.25f), -0.14f + sinWaveMovementAnimateValue + jawAnimationValue * 0.01f, 8.54f);
			Matrix rotateEyes = Matrix::makeRotateMatrix(-43.0f + (i * 43.0f), Vector(0.0f, 0.0f, 1.0f));
			Matrix rotate2Eyes = Matrix::makeRotateMatrix(21.0f, Vector(1.0f, 0.0f, 0.0f));
			Matrix rotate3Eyes = Matrix::makeRotateMatrix(30.0f - (i * 30.0f), Vector(0.0f, 1.0f, 0.0f));
			Matrix scaleEyes = Matrix::makeScaleMatrix(0.05f, 0.15f, 0.3f);
			modelMatrix = translateEyes * rotate2UpperJaw * rotateEyes * rotate2Eyes * rotate3Eyes * scaleEyes;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			drawRightAngledTrianglePrism(1, 1, 1, 1);

			Matrix translateEyeBalls = Matrix::makeTranslationMatrix(0.24f - (i * 0.24f), -0.19f + sinWaveMovementAnimateValue + jawAnimationValue * 0.01f, 8.67f);
			Matrix rotateEyeBalls = Matrix::makeRotateMatrix(-30.0f, Vector(1.0f, 0.0f, 0.0f));
			Matrix rotate2EyeBalls = Matrix::makeRotateMatrix(25.0f - (i * 25.0f), Vector(0.0f, 1.0f, 0.0f));
			Matrix animateEyeBalls = Matrix::makeRotateMatrix(-jawAnimationValue, Vector(1.0f, 0.0f, 0.0f));
			Matrix scaleEyeBalls = Matrix::makeScaleMatrix(0.06f, 0.05f, 0.15f);
			modelMatrix = translateEyeBalls * animateEyeBalls * rotateEyeBalls * rotate2EyeBalls * scaleEyeBalls;
			viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			drawEllipsoid(1, 20, 0, 0, 0, 1);
		}
	}

	void LoadTexture(GLuint number) {
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, mTextureID[number]);
		glBegin(GL_TRIANGLES);

		glColor3f(1, 1, 1);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.0f);

		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		glEnd();
	}


	void Ocean(const Matrix& viewMatrix, GLfloat magnitude) {
		time += 0.01;
		
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		GLfloat vertices[] = {
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f
		};
		//x Activate and specify pointer to vertex array
		glEnableClientState(GL_VERTEX_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, vertices);

		//x Draw rectangle
		for (int i = 0; i < 50; i++) {
			for (int j = 0; j < 20; j++) {
				

				Matrix translateRect = Matrix::makeTranslationMatrix(i, magnitude *sin(i + time), j);
				Matrix rotateRect = Matrix::makeRotateMatrix(30 * sin(i + time), Vector(0.0f, 0.0f, 1.0f));
				Matrix modelMatrix = translateRect * rotateRect;
				Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
				glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);

				glDrawArrays(GL_TRIANGLES, 0, 6);
				
			}
		}

		//x Deactivate vertex array after drawing
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void drawWithVertices() {
		/*static GLfloat vertices[] = {
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		};

		static GLfloat uvMap[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		};

		static GLfloat colors[] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 0, 0,
		0, 1.0f, 0,
		0, 1.0f, 0,
		1.0f, 0, 0,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 0, 0,
		0, 1.0f, 0,
		0, 1.0f, 0,
		1.0f, 0, 0,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 0, 0,
		0, 1.0f, 0,
		0, 1.0f, 0,
		1.0f, 0, 0,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 0, 0,
		0, 1.0f, 0,
		0, 1.0f, 0,
		1.0f, 0, 0,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 0, 0,
		0, 1.0f, 0,
		0, 1.0f, 0,
		1.0f, 0, 0,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 0, 0,
		0, 1.0f, 0,
		0, 1.0f, 0,
		1.0f, 0, 0,
		1.0f, 1.0f, 1.0f,
		};

		//glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, mTextureID[0]);
		//x Activate and specify pointer to vertex array
		glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);


		//glVertexPointer(3, GL_FLOAT, 0, vertices);
		//glTexCoordPointer(2, GL_FLOAT, 0, uvMap);
		glColorPointer(3, GL_FLOAT, 0, colors);

		//x Draw rectangle
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//x Deactivate vertex array after drawing
		//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		//glDisable(GL_TEXTURE_2D);*/
	}

	void drawWholeBody(const Matrix& viewMatrix) {
		float sinWaveMovementAnimateValue = 1.0f * sin(time * 0.5);
		float sinWaveMovementRotationValue = 1.0f * sin(time * 0.5);

		Matrix animateWingMovement = Matrix::makeTranslationMatrix(0.0f, sinWaveMovementAnimateValue, 0.0f);
		Matrix modelMatrix = animateWingMovement;
		Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);

		//x Draw Wing
		drawWing(10, 0.2, 40, 1, 0, 0, 1);

		//x Draw Body
		drawBody(viewMatrix, sinWaveMovementAnimateValue);

		//x Draw Left Fore Leg
		drawLeftForeLeg(viewMatrix, sinWaveMovementAnimateValue);

		//x Draw Right Fore Leg
		drawRightForeLeg(viewMatrix, sinWaveMovementAnimateValue);

		//x Draw Left Hind Leg
		drawLeftHindLeg(viewMatrix, sinWaveMovementAnimateValue);

		//x Draw Right Hind Leg
		drawRightHindLeg(viewMatrix, sinWaveMovementAnimateValue);

		//x Draw Tail
		drawTail(viewMatrix, 0.2, sinWaveMovementAnimateValue);

		//x Draw Neck
		drawNeck(viewMatrix, sinWaveMovementAnimateValue);

		//x Draw Head
		drawHead(viewMatrix, sinWaveMovementAnimateValue);
	}

	void draw(const Matrix& viewMatrix) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		sinWaveMovementTimer += 0.004;
		time += 0.02;

		//draw axis
		drawAxis(viewMatrix);

		drawWholeBody(viewMatrix);
	}
};

#endif