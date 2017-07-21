#include "header.h"
#include <random>

const float PI = 3.14159265f;

extern int clamp(int v, int a, int b) {
	/*
	 a <= v <= b なら何もしない．
	 v < aならv=aに， b < v　ならv=bにして範囲内に戻す．
	*/
	return v < a ? a : v > b ? b : v;
}

bool boxesCompare(const Box &a, const Box &b) {
	return a.s < b.s;
}


float boxesOverlap(Box &a, Box &b) {
	float areai, areaj, areaij;
	int r0, r1, c0, c1, r1i, c1i, r1j, c1j;
	r1i = a.r + a.h; c1i = a.c + a.w; if (a.r >= r1i || a.c >= c1i) return 0;
	r1j = b.r + b.h; c1j = b.c + b.w; if (a.r >= r1j || a.c >= c1j) return 0;
	areai = (float)a.w*a.h; r0 = max(a.r, b.r); r1 = min(r1i, r1j);
	areaj = (float)b.w*b.h; c0 = max(a.c, b.c); c1 = min(c1i, c1j);
	areaij = (float)max(0, r1 - r0)*max(0, c1 - c0);
	return areaij / (areai + areaj - areaij);
}


void boxesNms(Boxes &boxes, float thr, float eta, int maxBoxes){
	/*
	上位スコアのボックスとのIoUが大きいボックスを消去
    */
	sort(boxes.rbegin(), boxes.rend(), boxesCompare);
	if (thr>.99) return; const int nBin = 10000;
	const float step = 1 / thr, lstep = log(step);
	vector<Boxes> kept; kept.resize(nBin + 1);
	int i = 0, j, k, n = (int)boxes.size(), m = 0, b, d = 1;
	while (i<n && m<maxBoxes) {
		b = boxes[i].w*boxes[i].h; bool keep = 1;
		b = clamp(int(ceil(log(float(b)) / lstep)), d, nBin - d);
		for (j = b - d; j <= b + d; j++)
			for (k = 0; k<kept[j].size(); k++) if (keep)
				keep = boxesOverlap(boxes[i], kept[j][k]) <= thr;
		if (keep) { kept[b].push_back(boxes[i]); m++; } i++;
		if (keep && eta<1 && thr>.5) { thr *= eta; d = ceil(log(1 / thr) / lstep); }
	}
	boxes.resize(m); i = 0;
	for (j = 0; j<nBin; j++)
		for (k = 0; k<kept[j].size(); k++)
			boxes[i++] = kept[j][k];
	sort(boxes.rbegin(), boxes.rend(), boxesCompare);
}


Mat drawBox(Mat &img, Boxes &boxes, int number) {
	Mat image = img.clone();
	std::random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<> rand256(0, 255);
	int rgb[3];
	for (int i = 0; i < number; i++) {
		rgb[0] = rand256(mt);
		rgb[1] = rand256(mt);
		rgb[2] = rand256(mt);
		rectangle(image, Point(boxes[i].c, boxes[i].r), Point(boxes[i].c + boxes[i].w, boxes[i].r + boxes[i].h), Scalar(rgb[0], rgb[1], rgb[2]), 2);
	}
	return image;
}


int makedirs(const char *_Path) {
	string path = _Path;
	vector<string::size_type> slash;
        struct stat st;

	while (1) {
		string::size_type index = path.find("/");
		if (index == string::npos) {
			break;
		}
		path = path.substr(index + 1);
		slash.push_back(index);
	}

	for (int i = 1; i < slash.size(); i++) {
		slash[i] += slash[i - 1] + 1;
	}

	path = _Path;
	slash.insert(slash.end(), path.length());

	int cnt = 0;
	for (int i = 1; i < slash.size(); i++) {
	  if (stat(path.substr(0, slash[i]).c_str(), &st) != 0){
	    mkdir(path.substr(0, slash[i]).c_str(), 0775);
	  }
	}

	if (cnt > 0) {
		return 0;
	}
	else {
		return 1;
	}
}


void readparameters(option &opt) {
	vector<string> lines;
	ifstream ifs("params.csv");
	string str;
	if (ifs.fail()) {
		printf("params.csv not found\n");
	}
	else {
		printf("params.csv found\n");
		while (getline(ifs, str)) {
			string token;
			istringstream stream(str);
			while (getline(stream, token, ',')) {
				lines.push_back(token);
			}
		}
		opt.alpha = stof(lines[3]);
		opt.beta = stof(lines[5]);
		opt.eta = stof(lines[7]);
		opt.minBoxArea = stof(lines[9]);
		opt.edgeMinMag = stof(lines[11]);
		opt.edgeMergeThr = stof(lines[13]);
		opt.clusterMinMag = stof(lines[15]);
		opt.maxAspectRatio = stof(lines[17]);
		opt.minBoxArea = stof(lines[19]);
		opt.gamma = stof(lines[21]);
		opt.kappa = stof(lines[23]);
		opt.maxBoxes = stoi(lines[25]);
	}
}
