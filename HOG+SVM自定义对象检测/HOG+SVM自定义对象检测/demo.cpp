#include<stdlib.h>
#include<string>
#include<opencv2/opencv.hpp>
#include"Train_SVM.h"
#include"Test_SVM.h"
using namespace std;
using namespace cv;


int main()
{
	//�Զ�������⡪��ѵ��SVMģ�ͣ�����ͨ����ȡ������HOG�����ӣ������������������ݣ���ͨ��SVM���Է��������з���ѧϰ��ѵ����
	//��ѵ���������Ϊģ�ͣ������Ժ�Ϳ���ͨ������ģ����ʵ�ֶ�ͬһ�ֶ���ļ�⡣
	//����������������ݺ͸��������ݵ��ļ�·��
	string positive_path = "D:\\opencv_c++\\Learning-OpenCV\\HOG+SVM�Զ��������\\moon\\train\\positive\\";
	string negative_path = "D:\\opencv_c++\\Learning-OpenCV\\HOG+SVM�Զ��������\\moon\\train\\nagetive\\";
	train_svm(positive_path, negative_path);

	//�����Զ��������ģ�ͺͲ���ͼ��
	string model_path = "model.xml";
	Mat test_image = imread("D:\\opencv_c++\\Learning-OpenCV\\HOG+SVM�Զ��������\\moon\\test\\7.jpg");
	test_svm(test_image, model_path);
	
	waitKey(0);
	return 0;
}