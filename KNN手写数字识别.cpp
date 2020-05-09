#include<opencv2/opencv.hpp>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<Windows.h>
using namespace std;
using namespace cv;

Point prePoint;				//�������ָ�����һ����
Mat canvas = Mat::zeros(Size(28, 28), CV_8UC1);			//�ڵ׻���
Mat digit = canvas.clone();			//���д�µ���д������
static void on_Mouse(int event, int x, int y, int flag, void*);			//�����������Ļص�����
void training();			//ѵ������


int main()
{
	//δ�õ�ģ��ǰ��Ҫ����ѵ������
	//training();
	//����ѵ���õ�knnģ��
	Ptr<ml::KNearest> knn_test = ml::KNearest::load("knn_digits_model.yml");
	namedWindow("digit", WINDOW_NORMAL);
	resizeWindow("digit", Size(100, 100));
	imshow("digit", canvas);
	setMouseCallback("digit", on_Mouse, 0);			//�����������������λ�ڸ�ָ��������ʱ��ִ�лص�����
	int i = 0;
	while (true)
	{
		char ch = waitKey();
		if (ch == ' ')
		{

			//resize(digit, digit, Size(28, 28));
			digit = digit.reshape(1, 1);
			digit.convertTo(digit, CV_32F);
			Mat results = Mat::zeros(Size(1, 1), CV_32F);
			knn_test->findNearest(digit, 11, results);
			cout << "Ԥ����д���ֽ��:  " << results.at<float>(0, 0) << endl;

			/*		resize(digit, digit, Size(28, 28));
					string path1 = "D:/opencv_c++/Learning-OpenCV/KNN��д����ʶ��/KNN��д����ʶ��/train_data/ " + to_string(i) + "_" + to_string(260) + ".jpg";
					imwrite(path1, digit);
					i++;*/
		}

		else if (ch == 27)
		{
			break;
		}

	}
	waitKey(0);
	return 0;
}

static void on_Mouse(int event, int x, int y, int flag, void*)
{
	if (event == EVENT_LBUTTONDOWN)			//�����������£���ȡ�õ�����
	{
		prePoint = Point(x, y);
	}
	//�����������ְ��±�־��������ƶ�
	else if (event == EVENT_MOUSEMOVE && (flag & EVENT_FLAG_LBUTTON))
	{
		Point nowPoint(x, y);
		line(digit, nowPoint, prePoint, Scalar(255), 2, 8, 0);
		prePoint = nowPoint;		//�����ĵ�ǰ����Ϊ�ϸ���λ���Կ�ʼ��һ���߶�����Ļ���
		imshow("digit", digit);			//�ڴ��ڸ���Ŀ��ͼ��
	}
	//������Ҽ�����ʱ
	else if (event == EVENT_RBUTTONDOWN)
	{
		imshow("digit", canvas);
		digit = canvas.clone();
	}
}


void training()
{
	vector<Mat> train_images;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 760; j++)
		{
			string path = "D:/opencv_c++/Learning-OpenCV/KNN��д����ʶ��/KNN��д����ʶ��/train_data/" + to_string(i) + "_" + to_string(j) + ".jpg";
			Mat train_image = imread(path);
			cvtColor(train_image, train_image, COLOR_BGR2GRAY);
			train_images.push_back(train_image);
		}
	}
	for (int n = 0; n < train_images.size(); n++)
	{
		train_images[n] = train_images[n].reshape(1, 1);
	}

	int height = 7600;
	int width = 784;
	Mat train_data = Mat::zeros(Size(width, height), CV_8UC1);
	for (int r = 0; r < height; r++)
	{
		for (int c = 0; c < width; c++)
		{
			train_data.at<uchar>(r, c) = train_images[r].at<uchar>(0, c);
		}
	}
	Mat labels = Mat::zeros(Size(1, height), CV_8UC1);
	for (int k = 0; k < height; k++)
	{
		int label = k / 760;
		labels.at<uchar>(k, 0) = label;
	}

	train_data.convertTo(train_data, CV_32F);
	labels.convertTo(labels, CV_32F);
	auto knn = ml::KNearest::create();
	int K = 11;
	knn->setDefaultK(K);
	knn->setIsClassifier(true);
	auto data = ml::TrainData::create(train_data, ml::ROW_SAMPLE, labels);
	knn->train(data);
	knn->save("knn_digits_model.yml");
	//ʹ��ѵ�������в��ԣ�������ȷ��
	Mat results = Mat::zeros(labels.size(), CV_32F);
	knn->findNearest(train_data, K, results);
	float acc = 0;
	for (int n = 0; n < labels.rows; n++)
	{
		int result = results.at<float>(n, 0);
		int label = labels.at<float>(n, 0);
		if (result == label)
		{
			acc++;
		}
	}
	acc = acc / 7600 * 100;
	cout << "��ȷ��:  " << acc << "%" << endl;

}