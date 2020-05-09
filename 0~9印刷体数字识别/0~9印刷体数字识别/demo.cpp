#include<opencv2/opencv.hpp>
#include<stdlib.h>
#include<string>
#include<vector>
#include"AllFeatures.h"
#include"GetTrainData.h"
using namespace std;
using namespace cv;
using namespace cv::ml;

int main()
{
	////����0~9ʮ������ͼ��
	//Mat digits_image1 = imread("D:\\opencv_c++\\Learning-OpenCV\\0~9ӡˢ������ʶ��\\0~9ӡˢ������ʶ��\\0~9ӡˢ������ʶ��\\image\\train\\train1.png");
	//vector<Mat> ROI_digits1;
	//vector<Rect>digits_box1;
	//ROI_digits1 = GetTrainData(digits_image1, digits_box1);
	////��ȡ0~9ʮ�����ֵ���������
	//vector<vector<float>>features;
	//features= AllFeatures(ROI_digits1, features);

	////����0~9ʮ������ͼ��
	//Mat digits_image2 = imread("D:\\opencv_c++\\Learning-OpenCV\\0~9ӡˢ������ʶ��\\0~9ӡˢ������ʶ��\\0~9ӡˢ������ʶ��\\image\\train\\train2.png");
	//vector<Mat> ROI_digits2;
	//vector<Rect>digits_box2;
	//ROI_digits2 = GetTrainData(digits_image2, digits_box2);
	////��ȡ0~9ʮ�����ֵ���������
	//features = AllFeatures(ROI_digits2, features);

	////���ɱ�ǩ
	//vector<int> labels;
	//labels = GetTrainLabels();
	////ѵ��SVM������
	//TrainSVM(features, labels);

	//������֤
	Mat test_image = imread("image\\test\\digit-02.png");
	resize(test_image, test_image, Size(), 2, 2);
	auto test_svm = SVM::load("model.xml");
	vector<Mat> ROI_digits;
	vector<Rect>box_digits;
	ROI_digits = GetTrainData(test_image, box_digits);
	Mat output = Mat::ones(test_image.size(), test_image.type());
	for (int i = 0;i < ROI_digits.size();i++)
	{
		vector<float>feature = GetDigitFeature(ROI_digits[i]);
		Mat test_digits_feature = Mat::zeros(Size(40, 1), CV_32FC1);
		for (int col = 0;col < test_digits_feature.cols;col++)
		{
			test_digits_feature.at<float>(0, col) = feature[col];
		}
		float result = test_svm->predict(test_digits_feature);
		string predict_str = to_string(int(result));
		int x = box_digits[i].x;
		int y = box_digits[i].y;
		putText(output, predict_str, Point(x, y+10), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 0), 1, 8);
	}
	imshow("test_image", test_image);
	imshow("output", output);

	waitKey(0);
	return 0;
}