#include "header.h"

int main(int argc, const char* argv[]) {
	// default
	string save_root = "not selected";
	int scale = 1;

	switch (argc) {
	case 1:
		break;
	case 2:
		filename = argv[1];
		break;
	case 3:
		filename = argv[1];
		boxnum = atoi(argv[2]);
		break;
	case 4:
		filename = argv[1];
		boxnum = atoi(argv[2]);
		save_root = argv[3];
		break;
	case 5:
		filename = argv[1];
		boxnum = atoi(argv[2]);
		save_root = argv[3];
		scale = atoi(argv[4]);
		break;
	default:
		break;
	}
	cout << save_root << endl;
	printf("calculating...\n");

	Mat img = imread(filename, 1);                 // �摜
	Mat img2, img3, img4, img5;                    // ���T�C�Y�摜�C�G�b�W�摜�C�א����G�b�W�摜�C�{�b�N�X�o�͉摜
	arrayf E, O, E1;                               // edge map, orientation map, thinned edge map
	EdgeBoxGenerator edgebox;                      // ���o��
	option opt;                                    // �p�����[�^�Q
	Boxes boxes;                                   // ���̈�

	// ���o�t�F�[�Y
	resize(img, img2, Size(), 1.0 / double(scale), 1.0 / double(scale)); 
	CannyEdgeDetect(img2, img3);                   // �G�b�W���o
	ConvertToarrayf(img3, E);                      // edge map��arrayf�ɕϊ�
	CalculateOri(E, O);                            // orientation�v�Z
	NonMaximumSuppression(E, O, E1);               // �א�������
	
	// �T���t�F�[�Y
	readparameters(opt);                           // csv�t�@�C���Ńp�����[�^�ǂݍ���
	edgebox.setParams(opt);                        // �p�����[�^�ݒ�
	edgebox.generate(boxes, E1, O);                // ���̈�T��

	printf("total number of boxes:%d\n", boxes.size());

	img4 = Mat::zeros(E1._h, E1._w, CV_32F);
	for (int x = 0; x < img4.cols; x++) {
		for (int y = 0; y < img4.rows; y++) {
			img4.at<float>(y, x) = E1.val(x, y);
		}
	}

	img3.convertTo(img3, CV_8U, 255);
	img4.convertTo(img4, CV_8U, 255);
	img5 = drawBox(img2, boxes, boxnum);

	// �t���p�X����t�@�C�����̎擾
	int path_i = filename.find_last_of("/") + 1;
	int ext_i = filename.find_last_of(".");
	String basename = filename.substr(path_i, ext_i - path_i);
	string::size_type index = save_root.find("not selected");
	if (index != string::npos) {
		imwrite(basename + "/" + "Input.jpg", img);
		imwrite(basename + "/" + "Scaled.jpg", img2);
		imwrite(basename + "/" + "Canny.jpg", img3);
		imwrite(basename + "/" + "Thinned_Canny.jpg", img4);
		imwrite(basename + "/" + "Output.jpg", img5);

		ofstream outputfile("boxes.csv");
		outputfile << "\"box\",\"c\",\"r\",\"w\",\"h\",\"s\",\"scale\"\n";
		for (int i = 0; i < boxes.size(); i++) {
			outputfile << i << "," << boxes[i].c << "," << boxes[i].r << "," << boxes[i].w << "," << boxes[i].h << "," << boxes[i].s << "," << scale << "\n";
		}
		outputfile.close();
	}
	else {
		if (makedirs((save_root + "/" + basename).c_str()) == 0) {
			printf("Save directory is made.\n");
		}
		imwrite(save_root + "/" + basename + "/" + "Input.jpg", img);
		imwrite(save_root + "/" + basename + "/" + "Bulrred.jpg", img2);
		imwrite(save_root + "/" + basename + "/" + "Canny.jpg", img3);
		imwrite(save_root + "/" + basename + "/" + "Thinned_Canny.jpg", img4);
		imwrite(save_root + "/" + basename + "/" + "Output.jpg", img5);
		ofstream outputfile((save_root + "/" + basename + "/boxes.csv").c_str());
		outputfile << "\"box\",\"c\",\"r\",\"w\",\"h\",\"s\",\"scale\"\n";
		for (int i = 0; i < boxes.size(); i++) {
			outputfile << i << "," << boxes[i].c << "," << boxes[i].r << "," << boxes[i].w << "," << boxes[i].h << "," << boxes[i].s << "," << scale << "\n";
		}
		outputfile.close();
	}

	printf("end\n");
	return 0;
}
