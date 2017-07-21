#include "header.h"

void gradientx(arrayf &E, arrayf &grad_x) {
	grad_x.init(E._h, E._w);
	for (int x = 0; x < E._w; x++) {
		for (int y = 0; y < E._h; y++) {
			if (x == 0) {
				grad_x.val(x, y) = E.val(x + 1, y) - E.val(x, y);
			}
			else if(x == E._w - 1) {
				grad_x.val(x, y) = E.val(x, y) - E.val(x - 1, y);
			}
			else {
				grad_x.val(x, y) = (E.val(x + 1, y) - E.val(x - 1, y)) / 2.0;
			}
		}
	}
}

void gradienty(arrayf &E, arrayf &grad_y) {
	grad_y.init(E._h, E._w);
	for (int x = 0; x < E._w; x++) {
		for (int y = 0; y < E._h; y++) {
			if (y == 0) {
				grad_y.val(x, y) = E.val(x, y + 1), - E.val(x, y);
			}
			else if (y == E._h - 1) {
				grad_y.val(x, y) = E.val(x, y) - E.val(x, y - 1);
			}
			else {
				grad_y.val(x, y) = (E.val(x, y + 1) + E.val(x, y - 1)) / 2.0;
			}
		}
	}
}

int sign(float x) {
	if (x > 0) {
		return 1;
	}
	else if (x == 0) {
		return 0;
	}
	else {
		return -1;
	}
}


void CalculateOri(arrayf &E, arrayf &O) {
	arrayf Ox, Oy, Oxx, Oxy, Oyy;
	gradientx(E, Ox);
	gradienty(E, Oy);
	gradientx(Ox, Oxx);
	gradienty(Ox, Oxy);
	gradienty(Oy, Oyy);

	O.init(E._h, E._w);
	for (int x = 0; x < O._w; x++) {
		for (int y = 0; y < O._h; y++) {
			O.val(x, y) = fmodf(atanf((Oyy.val(x, y) * sign(-Oxy.val(x, y)) / (Oxx.val(x, y) + 1e-5))), PI);
		}
	}
}
