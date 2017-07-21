#pragma once
#include <iostream>
#include <stdio.h>
#include "math.h"
#include <algorithm>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

#ifndef INCLUDED_CONST
#define INCLUDED_CONST

extern int clamp(int v, int a, int b);
extern const float PI;

template <class T> class Array
{
public:
	int _h, _w;
	T *_x;
	bool _free;
	Array() { _h = _w = 0; _x = 0; _free = 0; }
	virtual ~Array() { clear(); }
	void clear() { if (_free) delete[] _x; _h = _w = 0; _x = 0; _free = 0; }
	void init(int h, int w) { clear(); _h = h; _w = w; _x = new T[h*w](); _free = 1; }																																																																														
	T& val(size_t c, size_t r) { return _x[c*_h + r]; }
};

// paremeters of EdgeBoxes
typedef struct {
	float alpha = 0.65, beta = 0.75f, eta = 0.95f, minScore = 0.05f, edgeMinMag = 0.10f, edgeMergeThr = 0.5f, clusterMinMag = 0.5f,
		maxAspectRatio = 3.0f, minBoxArea = 1000.0f, gamma = 2.0f, kappa = 1.5f;
	int maxBoxes = 10000.0f; 
} option;

// convenient typedefs
typedef vector<float> vectorf;
typedef vector<int> vectori;
typedef Array<float> arrayf;
typedef Array<int> arrayi;

// bounding box data structures and routines

typedef struct { int c, r, w, h; float s; } Box;
typedef vector<Box> Boxes;

//bilinear interpolation
extern inline float interp(arrayf &E, int h, int w, float x, float y){
	x = x<0 ? 0 : (x>w - 1.001 ? w - 1.001 : x);
	y = y<0 ? 0 : (y>h - 1.001 ? h - 1.001 : y);
	int x0 = int(x), y0 = int(y), x1 = x0 + 1, y1 = y0 + 1;
	float dx0 = x - x0, dy0 = y - y0, dx1 = 1 - dx0, dy1 = 1 - dy0;
	return E.val(x0, y0) * dx1 * dy1 + E.val(x1, y0) * dx0 * dy1 +
		E.val(x0, y1) * dx1 * dy0 + E.val(x1, y1) * dx0 * dy0;
}


bool boxesCompare(const Box &a, const Box &b);

float boxesOverlap(Box &a, Box &b);

void boxesNms(Boxes &boxes, float thr, float eta, int maxBoxes);

Mat drawBox(Mat &img, Boxes &boxes, int number);

int makedirs(const char *_Path);

void readparameters(option &opt);
#endif


#ifndef INCLUDED_EdgeBoxGenerator
#define INCLUDED_EdgeBoxGenerator

class EdgeBoxGenerator {
public:
	// method parameters (must be manually set)
	float _alpha, _beta, _eta, _minScore; int _maxBoxes;
	float _edgeMinMag, _edgeMergeThr, _clusterMinMag;
	float _maxAspectRatio, _minBoxArea, _gamma, _kappa;

	// main external routine (set parameters first)
	void generate(Boxes &boxes, arrayf &E, arrayf &O);
	// set parameter
	void setParams(option opt);
private:
	int h, w;
	int _segCnt;
	arrayi _segIds;
	vectorf _segMag;
	vectori _segR, _segC;
	vector<vectorf> _segAff;
	vector<vectori> _segAffIdx;

	arrayf _segIImg, _magIImg;
	
	arrayi _hIdxImg, _vIdxImg;   
	vector<vectori> _hIdxs, _vIdxs;
	vectorf _scaleNorm;
	float _scStep, _arStep, _rcStepRatio;

	// data structures for efficiency (see scoreBox)
	arrayf _sWts;
	arrayi _sDone, _sMap, _sIds;
	int _sId;

	// helper routines
	//void clusterEdges(arrayf &E, arrayf &O, arrayf &V);
	void clusterEdges(arrayf &E, arrayf &O);
	void prepDataStructs(arrayf &E);
	void scoreAllBoxes(Boxes &boxes);
	void scoreBox(Box &box);
	void refineBox(Box &box);
	//void drawBox(Box &box, arrayf &E, arrayf &V);
};

#endif // !INCLUDED_EdgeBoxGenerator

#ifndef INCLUDED_EdgeDetect
#define INCLUDED_EdgeDetect
void CannyEdgeDetect(const Mat &src, Mat &dst);
void ConvertToarrayf(const Mat &src, arrayf &E);
void NonMaximumSuppression(arrayf &E, arrayf &O, arrayf &E1, float m = 1.0f, int r = 1);

#endif // !INCLUDED_EdgeDetect

#ifndef INCLUDED_gradient
#define INCLUDED_gradient
//calculate x-direction gradien
void gradientx(arrayf &E, arrayf &grad_x);
//calculate y-direction gradient
void gradienty(arrayf &E, arrayf &grad_y);
//Calculate orientation of the edge map with second order differential
void CalculateOri(arrayf &E, arrayf &O);
#endif
