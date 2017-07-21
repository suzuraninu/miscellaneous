#include "header.h"


// Canny algorithm
void CannyEdgeDetect(const Mat &src, Mat &dst) {
	Mat channels[3], dx, dy, dst_r, dst_g, dst_b;
	split(src, channels);
	
	Sobel(channels[0], dx, CV_32F, 1, 0);
	Sobel(channels[0], dy, CV_32F, 0, 1);
	dst_b = (abs(dx) + abs(dy)) / 2.0;
	
	Sobel(channels[1], dx, CV_32F, 1, 0);
	Sobel(channels[1], dy, CV_32F, 0, 1);
	dst_g = (abs(dx) + abs(dy)) / 2.0;

	Sobel(channels[2], dx, CV_32F, 1, 0);
	Sobel(channels[2], dy, CV_32F, 0, 1);
	dst_r = (abs(dx) + abs(dy)) / 2.0;
	
	dst = max(dst_b, max(dst_g, dst_r));
	
	// convert 0-255 intensity into 0-1 float
	dst.convertTo(dst, CV_32F, 1.0 / 255.0);
}

void ConvertToarrayf(const Mat &src, arrayf &E) {
	int height = src.rows;
	int width = src.cols;
	E.init(height, width);
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++){
			E.val(x, y) = src.at<float>(y, x);
		}
	}
}


void NonMaximumSuppression(arrayf &E, arrayf &O, arrayf &E1, float m, int r) {
    //細線化処理．
	int h = E._h;
	int w = E._w;
	E1.init(h, w);
	for (int x = 0; x < E._w; x++) {
		for (int y = 0; y < E._h; y++) {
			float e = E.val(x, y); 
			if (!e) {
				continue; 
			}
			e *= m;
			float coso = cos(O.val(x, y)), sino = sin(O.val(x, y));
			for (int d = -r; d <= r; d++) {
				if (d) {
				  float e0 = interp(E, h, w, x + d*coso, (float)y + d*sino);
					if (e < e0) {
						E1.val(x, y) = 0;
						break;
					}
					else {
						E1.val(x, y) = E.val(x, y);
					}
				}
			}
		}
	}
}
