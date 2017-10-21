#pragma warning(disable:4996)

#ifndef _TRIANGLE_DEMO_H
#define _TRIANGLE_DEMO_H

#include "iostream"
#include "demo_base.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include <algorithm>

using namespace std;

#define SPECTRUM_SIZE 8192 // Must be power of 2
#define RECT_VERTICES_W 50
#define RECT_VERTICES_H 50

//Quad has 6 vertices
const int RECT_VERTEX_COUNT = (RECT_VERTICES_W - 1) * (RECT_VERTICES_H - 1) * 6;

// Vertex coordinates(x, y, z)
const int RECT_VERTEX_ARRAY_SIZE = RECT_VERTEX_COUNT * 3;

void ERRCHECK(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		cout << "FMOD error! " << FMOD_ErrorString(result) << endl;
	}
}

class TriangleDemo : public DemoBase
{
private:
	FMOD::System* m_fmodSystem;
	FMOD::Sound* m_music;
	FMOD::Channel* m_musicChannel;

	float m_SpectrumLeft[SPECTRUM_SIZE];
	float m_SpectrumRight[SPECTRUM_SIZE];
	float m_WaveLeft[SPECTRUM_SIZE];
	float m_WaveRight[SPECTRUM_SIZE];

	
	float timer;
	float *spec;
	float *waveSpec;
	GLfloat mRectVertices[RECT_VERTEX_ARRAY_SIZE];

public:
	void initFmod () {
		FMOD_RESULT result;
		unsigned int version;
		
		result = FMOD::System_Create(&m_fmodSystem);
		ERRCHECK(result);

		result = m_fmodSystem->getVersion(&version);
		ERRCHECK(result);

		if (version < FMOD_VERSION) {
			cout << "FMOD Error! You are using an older version of FMOD." << endl;
		}

		//Initiate FMOD
		result = m_fmodSystem->init(32, FMOD_INIT_NORMAL, 0);
		ERRCHECK(result);
		
		//Load and set music
		result = m_fmodSystem->createStream("../media/Rameses B - Transformations (ft. Laura Brehm).mp3", FMOD_SOFTWARE, 0, &m_music);
		ERRCHECK(result);

		//Play loaded music
		result = m_fmodSystem->playSound(FMOD_CHANNEL_FREE, m_music, false, &m_musicChannel);
		ERRCHECK(result);

		
	}

	void init()
	{
		initFmod();
		timer = 0;
		spec = new float[SPECTRUM_SIZE];
		waveSpec = new float[SPECTRUM_SIZE];
	}

	void deinit()
	{
	}

	float Interpolation(int j, float mu) {
		float mu2 = (1 - cos(mu * M_PI)) / 2;
		float total = (m_SpectrumLeft[j + 1] * (1 - mu2) + m_SpectrumLeft[j + 2] * mu2) +
				      (m_SpectrumRight[j + 1] * (1 - mu2) + m_SpectrumRight[j + 2] * mu2);

		return (total);
	}

	float waveInterpolation(int j, float mu) {
		float mu2 = (1 - cos(mu * M_PI)) / 2;
		float total = (m_WaveLeft[j + 1] * (1 - mu2) + m_WaveLeft[j + 2] * mu2);

		return (total);
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

	void drawPlane(const Matrix& viewMatrix) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		const float r = 3.0f;
		const float polySize = 512;
		bool beatDetected = false;
		float spectrum = 0;
		float normalizedSpectrum;
		double theta = M_2PI / polySize;

		auto maxIterator = max_element(&waveSpec[0], &waveSpec[SPECTRUM_SIZE]);
		float maxVolume = *maxIterator;
		
		int currentVert = -1;
		int currentIndex = -1;
		int j = 0;

		for (double i = 0; i < M_2PI; i += theta) {

			j++;

			if (maxVolume != 0) {
				transform(&waveSpec[0], &waveSpec[SPECTRUM_SIZE], &waveSpec[0], [maxVolume](float dB) -> float {return dB / maxVolume; });
			}


			if (m_WaveLeft[j] > 0.8f) {
				spectrum = (m_WaveLeft[j] + m_WaveLeft[j + 1]) /2 * 0.5;
			}

			

			int vertex1 = ++currentVert;
			mRectVertices[vertex1 * 3] = r * cos(i);
			mRectVertices[vertex1 * 3 + 1] = r * sin(i);
			mRectVertices[vertex1 * 3 + 2] = spectrum;

			int vertex2 = ++currentVert;
			mRectVertices[vertex2 * 3] = r * cos(i);
			mRectVertices[vertex2 * 3 + 1] = r * sin(i + 1);
			mRectVertices[vertex2 * 3 + 2] = spectrum;

			int vertex3 = ++currentVert;
			mRectVertices[vertex3 * 3] = r * cos(i + 1);
			mRectVertices[vertex3 * 3 + 1] = r * sin(i + 1);
			mRectVertices[vertex3 * 3 + 2] = spectrum;

			int vertex4 = ++currentVert;
			mRectVertices[vertex4 * 3] = mRectVertices[vertex3 * 3];
			mRectVertices[vertex4 * 3 + 1] = mRectVertices[vertex3 * 3 + 1];
			mRectVertices[vertex4 * 3 + 2] = spectrum;

			int vertex5 = ++currentVert;
			mRectVertices[vertex5 * 3] = r * cos(i + 1);
			mRectVertices[vertex5 * 3 + 1] = r * sin (i);
			mRectVertices[vertex5 * 3 + 2] = spectrum;

			int vertex6 = ++currentVert;
			mRectVertices[vertex6 * 3] = mRectVertices[vertex1 * 3];
			mRectVertices[vertex6 * 3 + 1] = mRectVertices[vertex1 * 3 + 1];
			mRectVertices[vertex6 * 3 + 2] = spectrum;
		}



		/*for (int h = 0; h < RECT_VERTICES_H - 1; h++)
		{
			for (int w = 0; w < RECT_VERTICES_W - 1; w++)
			{
				//========= 6 vertices to form one sub-rectangle

				//1st vertex
				int vertex1 = ++currentVert;
				mRectVertices[vertex1 * 3] = -halfWidth + (float)(w) / (float)RECT_VERTICES_W * width;
				mRectVertices[vertex1 * 3 + 1] = -halfHeight + (float)(h) / (float)RECT_VERTICES_H * height;
				mRectVertices[vertex1 * 3 + 2] = 0;
				

				//2nd vertex
				int vertex2 = ++currentVert;
				mRectVertices[vertex2 * 3] = -halfWidth + (float)(w) / (float)RECT_VERTICES_W * width;
				mRectVertices[vertex2 * 3 + 1] = -halfHeight + (float)(h + 1) / (float)RECT_VERTICES_H * height;
				mRectVertices[vertex2 * 3 + 2] = 0;


				//3rd vertex
				int vertex3 = ++currentVert;
				mRectVertices[vertex3 * 3] = -halfWidth + (float)(w + 1) / (float)RECT_VERTICES_W * width;
				mRectVertices[vertex3 * 3 + 1] = -halfHeight + (float)(h + 1) / (float)RECT_VERTICES_H * height;
				mRectVertices[vertex3 * 3 + 2] = 0;
				

				//4th vertex
				int vertex4 = ++currentVert;
				mRectVertices[vertex4 * 3] = mRectVertices[vertex3 * 3];
				mRectVertices[vertex4 * 3 + 1] = mRectVertices[vertex3 * 3 + 1];
				mRectVertices[vertex4 * 3 + 2] = 0;
				

				//5th vertex
				int vertex5 = ++currentVert;
				mRectVertices[vertex5 * 3] = -halfWidth + (float)(w + 1) / (float)RECT_VERTICES_W * width;
				mRectVertices[vertex5 * 3 + 1] = -halfHeight + (float)(h) / (float)RECT_VERTICES_H * height;
				mRectVertices[vertex5 * 3 + 2] = 0;
				

				//6th vertex
				int vertex6 = ++currentVert;
				mRectVertices[vertex6 * 3] = mRectVertices[vertex1 * 3];
				mRectVertices[vertex6 * 3 + 1] = mRectVertices[vertex1 * 3 + 1];
				mRectVertices[vertex6 * 3 + 2] = 0;
				
			}
		}*/

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, mRectVertices);
		glDrawArrays(GL_TRIANGLES, 0, RECT_VERTEX_COUNT);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void drawCube(const Matrix& viewMatrix) {

		GLfloat vertices[] = {
			//x Back
			-0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,

			0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,

			//x Front
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,

			0.5f, 0.5f, -0.5f,
			-0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			//x Right
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,

			0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,

			//x Left
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,

			-0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			//x Top
			0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,

			0.5f, 0.5f, -0.5f,
			-0.5f, 0.5f, -0.5f,
			-0.5f, 0.5f, 0.5f,
			
			//x Bottom
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,

			0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, 0.5f,
		};

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void drawVisualiser(const Matrix& viewMatrix) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		int polySize = 512;
		int multiplier = 6;
		int j = 0;
		int r = 3;
		double theta = M_2PI / polySize;
		float angle = theta * 180.0f / M_PI;

		auto maxIterator = max_element(&spec[0], &spec[SPECTRUM_SIZE]);
		float maxVolume = *maxIterator;

		if (maxVolume != 0) {
			transform(&spec[0], &spec[SPECTRUM_SIZE], &spec[0], [maxVolume](float dB) -> float {return dB / maxVolume;});
		}
		
		for (double i = 0; i < M_2PI; i += theta) {
			float x = 3 * cos(i);
			float y = 3 * sin(i);

			j++;

			float spectrumValue = Interpolation(j, 1)/ 2;
			spec[j] = Interpolation(j, 1) /2;

			if (spectrumValue > 0.4) {
				spectrumValue = 0.5;
			}

			if (spectrumValue < 0.02) {
				glColor3f(0, 0, 1);
			}
			else if (spectrumValue < 0.05 && spectrumValue > 0.02) {
				glColor3f(0, 1, 0);
			}
			else if (spectrumValue < 0.1 && spectrumValue > 0.05) {
				glColor3f(1, 0, 0);
			} 
			else if (spectrumValue < 0.3 && spectrumValue > 0.1) {
				glColor3f(1, 0, 1);
			}
			else {
				glColor3f(1, 0.5, 0);
			}

			if (spectrumValue < 0.05) {
				multiplier = 15;
			}
			else {
				multiplier = 6;
			}

			float spectrumClamped = spectrumValue;

			if (spectrumClamped > 0.3) {
				spectrumClamped = 0.3;
			}

			Matrix translate = Matrix::makeTranslationMatrix(x + spectrumClamped, y + spectrumClamped, 0.3);
			Matrix scale = Matrix::makeScaleMatrix(0.01 + spectrumValue * multiplier, 0.02, 0.5 + spectrumValue);
			Matrix rotate = Matrix::makeRotateMatrix(-angle * j, Vector(0, 0, 1));
			Matrix rotateWhole = Matrix::makeRotateMatrix(-80.0f, Vector(0, 0, 1));
			Matrix translateWhole = Matrix::makeTranslationMatrix(0.25, 0.25, 0);
			Matrix modelMatrix = translateWhole * rotateWhole * translate * rotate * scale;
			Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*) viewSpaceMatrix.mVal);
			drawCube(viewMatrix);
		}
	}

	void drawWaveVisualiser(const Matrix& viewMatrix) {
		glColor3f(0.3, 0.3, 0.3);
		timer += 0.05;
		Matrix translateWhole = Matrix::makeTranslationMatrix(0.25f, 0.25f, -0.1f);
		Matrix rotateWhole = Matrix::makeRotateMatrix(timer, Vector(0, 0, 1));
		Matrix modelMatrix = translateWhole * rotateWhole;
		Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		drawPlane(viewMatrix);
	}

	void drawShootVisualiser(const Matrix& viewMatrix) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		int polySize = 512;
		int multiplier = 6;
		int j = 0;
		int r = 3;
		double theta = M_2PI / polySize;
		float angle = theta * 180.0f / M_PI;

		auto maxIterator = max_element(&spec[0], &spec[SPECTRUM_SIZE]);
		float maxVolume = *maxIterator;

		if (maxVolume != 0) {
			transform(&spec[0], &spec[SPECTRUM_SIZE], &spec[0], [maxVolume](float dB) -> float {return dB / maxVolume; });
		}

		for (double i = 0; i < M_2PI; i += theta) {
			float x = 3 * cos(i);
			float y = 3 * sin(i);

			j++;

			float spectrumValue = Interpolation(j, 1) / 2;
			spec[j] = Interpolation(j, 1) / 2;

			if (spectrumValue > 0.4) {
				spectrumValue = 0.5;
			}

			if (spectrumValue < 0.02) {
				glColor3f(0, 0, 1);
			}
			else if (spectrumValue < 0.05 && spectrumValue > 0.02) {
				glColor3f(0, 1, 0);
			}
			else if (spectrumValue < 0.1 && spectrumValue > 0.05) {
				glColor3f(1, 0, 0);
			}
			else if (spectrumValue < 0.3 && spectrumValue > 0.1) {
				glColor3f(1, 0, 1);
			}
			else {
				glColor3f(1, 0.5, 0);
			}

			if (spectrumValue < 0.05) {
				multiplier = 15;
			}
			else {
				multiplier = 6;
			}

			float spectrumClamped = spectrumValue;

			if (spectrumClamped > 0.3) {
				spectrumClamped = 0.3;
			}

			Matrix translate = Matrix::makeTranslationMatrix(x * spectrumClamped, y * spectrumClamped, 0);
			Matrix scale = Matrix::makeScaleMatrix(0.01 + spectrumValue * multiplier, 0.02, 0.5 + spectrumValue);
			Matrix rotate = Matrix::makeRotateMatrix(-angle * j, Vector(0, 0, 1));
			Matrix rotateWhole = Matrix::makeRotateMatrix(-timer, Vector(0, 0, 1));
			Matrix translateWhole = Matrix::makeTranslationMatrix(0.25, 0.25, 0);
			Matrix modelMatrix = translateWhole * rotateWhole * translate * rotate * scale;
			Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
			glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
			drawCube(viewMatrix);
		}
	}

	void UpdateFmod() {
		m_fmodSystem->update();

		// Get spectrum for left and right stereos
		m_musicChannel->getSpectrum(m_SpectrumLeft, SPECTRUM_SIZE, 0, FMOD_DSP_FFT_WINDOW_RECT);
		m_musicChannel->getSpectrum(m_SpectrumRight, SPECTRUM_SIZE, 1, FMOD_DSP_FFT_WINDOW_RECT);

		m_musicChannel->getWaveData(m_WaveLeft, SPECTRUM_SIZE, 0);
		m_musicChannel->getWaveData(m_WaveRight, SPECTRUM_SIZE, 1);

		//test
		float multiplier = 3.0f;
		cout << (m_SpectrumLeft[0] + m_SpectrumRight[0]) * 0.5f * multiplier << endl;
	}

	void draw(const Matrix& viewMatrix)
	{
		drawAxis(viewMatrix);
		UpdateFmod();
		drawVisualiser(viewMatrix);
		drawWaveVisualiser(viewMatrix);
		drawShootVisualiser(viewMatrix);
	}
};

#endif
