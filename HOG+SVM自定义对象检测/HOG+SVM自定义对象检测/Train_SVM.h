#pragma once
#include<stdlib.h>
#include<string>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void train_svm(string positive_path, string negative_path)
{
	//ͨ��glob()������·���µ�����ͼ���ļ���·����string���Ͷ�ȡ����
	vector<string> positive_images_str, negative_images_str;
	glob(positive_path, positive_images_str);
	glob(negative_path, negative_images_str);
	//��string���͵�ͼ������ת��ΪMat����
	vector<Mat>positive_images, negative_images;
	for (int i = 0; i < positive_images_str.size(); i++)
	{
		Mat positive_image = imread(positive_images_str[i]);
		resize(positive_image, positive_image, Size(64, 128));
		positive_images.push_back(positive_image);
	}
	for (int j = 0; j < negative_images_str.size(); j++)
	{
		Mat negative_image = imread(negative_images_str[j]);
		resize(negative_image, negative_image, Size(64, 128));
		negative_images.push_back(negative_image);
	}

	//�ֱ��ȡ����������ÿ��ͼ���HOG���������ӣ������б�ע
	HOGDescriptor *hog_train = new HOGDescriptor;
	vector<vector<float>> train_descriptors;
	int positive_num = positive_images.size();
	int negative_num = negative_images.size();
	vector<int> labels;
	for (int i = 0; i < positive_num; i++)
	{
		Mat gray;
		cvtColor(positive_images[i], gray, COLOR_BGR2GRAY);			//����HOG������ʱ��Ҫʹ�ûҶ�ͼ��
		vector<float> descriptor;
		hog_train->compute(gray, descriptor, Size(8, 16), Size(0, 0));
		train_descriptors.push_back(descriptor);
		labels.push_back(1);
	}
	for (int j = 0; j < negative_num; j++)
	{
		Mat gray;
		cvtColor(negative_images[j], gray, COLOR_BGR2GRAY);
		vector<float> descriptor;
		hog_train->compute(gray, descriptor, Size(8, 16), Size(0, 0));
		train_descriptors.push_back(descriptor);
		labels.push_back(-1);
	}
	//��ѵ������vectorת��ΪMat����ÿһ��Ϊһ�������ӣ�������Ϊ������
	int width = train_descriptors[0].size();
	int height = train_descriptors.size();
	Mat train_data = Mat::zeros(Size(width, height), CV_32F);
	for (int r = 0; r < height; r++)
	{
		for (int c = 0; c < width; c++)
		{
			train_data.at<float>(r, c) = train_descriptors[r][c];
		}
	}
	//ʹ�����Ų���ѵ��SVM�������������浽�����ͬĿ¼��
	auto train_svm = ml::SVM::create();
	train_svm->trainAuto(train_data, ml::ROW_SAMPLE, labels);
	train_svm->save("model.xml");
	hog_train->~HOGDescriptor();
	train_svm->clear();
	positive_images_str.~vector();
	negative_images_str.~vector();
	positive_images.~vector();
	negative_images.~vector();
	labels.~vector();
	train_data.~Mat();
}
