#ifndef _TRIANGLE_DEMO_H
#define _TRIANGLE_DEMO_H

#include "demo_base.h"

class TriangleDemo : public DemoBase
{
private:
	float m_rotation;
	float m_rotation2;
	float value2;
	float value;
public:
	void init()
	{
		m_rotation = 0;
		m_rotation2 = 0;
		value2 = 0;
		value = 0;
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

	void drawTriangle()
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//drawAxis(viewMatrix);

		//glLoadMatrixf((GLfloat*)viewMatrix.mVal);


		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_TRIANGLES);								// Drawing Using Triangles
			glVertex3f( 1.0f, 1.0f, 0.0f);					// Top
			glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Left
			glVertex3f( 2.0f,-1.0f, 0.0f);					// Bottom Right
		glEnd();											// Finished Drawing The Triangle
	}

	void draw(const Matrix& viewMatrix) {

		//draw axis
		drawAxis(viewMatrix);

		m_rotation += 0.01f;
		value += 0.001f;
		//Matrix rotate1 = Matrix::makeRotateMatrix(m_rotation, Vector(0.0f, 0.0f, 1.0f));
		/*
		
		

		Matrix translate1 = Matrix::makeTranslationMatrix(cosf(value) * 2.0f, sinf(value) * 1.0f, 0.0f);

		//NOTE on OpenGL Matrix model:
		//Screen = Proj * View * Model
		//Model = TransformA(3rd) * TransformB(2nd) * TransformC(1st)
		//        (Transform could be Rotate, Scale, Translate, etc)

		//Perform model transformation
		Matrix modelMatrix = translate1;

		//Convert model space to view space
		Matrix viewSpaceMatrix = viewMatrix * modelMatrix;

		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);

		drawTriangle();

		value += 0.005f;
		m_rotation2 += 0.01f;
		Matrix rotation2 = Matrix::makeRotateMatrix(m_rotation2, Vector(1.0f, 0.0f, 0.0f));
		modelMatrix = rotation2;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawTriangle();*/

		for (int i = 0; i < 5; i++) {
			Matrix translate2 = Matrix::makeTranslationMatrix(1.0f * i, 1.0f * i, 0.0f);
			Matrix rotation3 = Matrix::makeRotateMatrix(m_rotation, Vector(0.0f, 1.0f, 0.0f));
			Matrix modelMatrix = translate2 * rotation3;
			Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			
			drawTriangle();
		}
	}
};

#endif
