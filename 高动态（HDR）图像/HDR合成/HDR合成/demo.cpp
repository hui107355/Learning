#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<fstream>
#include<stdlib.h>
using namespace std;
using namespace cv;


void Debevec(vector<Mat>exposureImages, vector<float>exposureTimes);
void Robertson(vector<Mat>exposureImages, vector<float>exposureTimes);
void Mertens(vector<Mat>exposureImages);
int main()
{
	//�����ع�ͼ������
	vector<string>imgPath;
	glob("D:\\opencv_c++\\opencv_tutorial\\data\\images\\exposures\\", imgPath);
	vector<Mat> exposure_images;
	for (int i = 0;i < imgPath.size();i++)
	{
		Mat exposure_image = imread(imgPath[i]);
		exposure_images.push_back(exposure_image);
	}
	//�����ع�ʱ������
	String exposureTimesPath = "D:\\opencv_c++\\Learning-OpenCV\\�߶�̬��HDR��ͼ��\\exposuresTime.txt";
	ifstream fp(exposureTimesPath);
	if (!fp.is_open())
	{
		cout << "can't open file" << endl;
		exit(-1);
	}
	vector<float> exposureTimes;
	while (!fp.eof())
	{
		string time;
		getline(fp, time);
		float t = float(atof(time.c_str()));
		exposureTimes.push_back(t);
	}
	//����HDR�ϳ�
	Debevec(exposure_images, exposureTimes);
	Robertson(exposure_images, exposureTimes);
	Mertens(exposure_images);

	waitKey(0);
	return 0;
}


//ʹ�öԱȶȡ����ͶȺ��ع�Ⱥû��������ؼ�Ȩ��������ʹ��������˹����������ͼ����м�Ȩ���õ���ͼ��Ȩ�ر�����Ϊ�Աȶȡ����ͶȺ��ع�����ö����ļ�Ȩƽ��ֵ��
//���ɵ�ͼ����Ҫ����ɫ��ӳ�䣬���ҿ���ͨ������255ת��Ϊ8λͼ�񣬵��ǽ���Ӧ��٤��У��������ɫ��ӳ�䡣
//����Ҫ��Ӧ�ع�ʱ��, �����ڲ���ҪHDRͼ��ʱʹ�ã���ֱ�ӵõ��Ͷ�̬��Χͼ��
void Mertens(vector<Mat>exposureImages)
{
	auto merge_mertens = createMergeMertens();
	Mat fusion;
	merge_mertens->process(exposureImages, fusion);
	//����٤������������ʵ��ͼ����ڲ�����2.2f������������ʾ���
	/*auto tonemap = createTonemap(2.2f);
	tonemap->process(fusion, fusion);*/
	fusion = fusion * 255;
	fusion.convertTo(fusion, CV_8UC3);
	imshow("Mertens", fusion);
	imwrite("D:\\opencv_c++\\Learning-OpenCV\\�߶�̬��HDR��ͼ��\\Mertens.png", fusion);
}


//ʹ��Debevec��������HDR�ϳɣ��������ع�ʱ�䲻ͬ����Ƭ�����Ӧ���ع�ֵ���м�Ȩƽ�����õ�float32���͵ĸ߶�̬��ΧHDRͼ��
//��Ҫ�����ع�ͼ�����кͶ�Ӧ���ع�ʱ��
void Debevec(vector<Mat>exposureImages, vector<float>exposureTimes)
{
	//auto Debevec_response = createCalibrateDebevec();
	//Mat response;
	//Debevec_response->process(exposureImages, response, exposureTimes);

	auto  merge_Debevec = createMergeDebevec();
	Mat hdr;
	merge_Debevec->process(exposureImages, hdr, exposureTimes);
	//����������ͨ��LDR��ʾ���Ͻ�����ʾ��������Ǳ��뽫HDRͼ��ӳ�䵽���������ϸ�ڵ�8λ��Χ�ĵͶ�̬��ΧLDRͼ��
	//���ǽ���ɫ��ӳ�����ҪĿ�꣬٤��У��ֵ����Ϊ2.2f�����ڴ�������
	Mat ldr;
	auto tonemap = createTonemap(2.2f);
	tonemap->process(hdr, ldr);
	//HDRͼ�����ɫ��ӳ���õ���LDRͼ��ȡֵ��Χ�� [ 0 , 1 ] �����Գ�255����Χ���쵽 [ 0 , 255 ] 
	ldr = ldr * 255;
	//��float32����ת��Ϊuchar8����
	ldr.convertTo(ldr, CV_8UC3);
	imshow("Debevec_HDR", hdr);
	imshow("Debevec_LDR", ldr);
	imwrite("D:\\opencv_c++\\Learning-OpenCV\\�߶�̬��HDR��ͼ��\\Debevec_LDR.png", ldr);
}


//ʹ��Robertson��������HDRͼ��ϳɣ��������ع�ʱ�䲻ͬ����Ƭ�����Ӧ���ع�ֵ���м�Ȩƽ�����õ�float32���͵ĸ߶�̬��ΧHDRͼ��
//��Ҫ�����ع�ͼ�����кͶ�Ӧ���ع�ʱ��
void Robertson(vector<Mat>exposureImages, vector<float>exposureTimes)
{
	//auto Robertson_response = createCalibrateRobertson();
	//Mat response;
	//Robertson_response->process(exposureImages, response, exposureTimes);

	auto  merge_Robertson = createMergeRobertson();
	Mat hdr;
	merge_Robertson->process(exposureImages, hdr, exposureTimes);
	//����������ͨ��LDR��ʾ���Ͻ�����ʾ��������Ǳ��뽫HDRͼ��ӳ�䵽���������ϸ�ڵ�8λ��Χ�ĵͶ�̬��ΧLDRͼ��
	//���ǽ���ɫ��ӳ�����ҪĿ�꣬٤��У��ֵ����Ϊ2.2f�����ڴ�������
	Mat ldr;
	auto tonemap = createTonemap(2.2f);
	tonemap->process(hdr, ldr);
	//HDRͼ�����ɫ��ӳ���õ���LDRͼ��ȡֵ��Χ�� [ 0 , 1 ] �����Գ�255����Χ���쵽 [ 0 , 255 ] 
	ldr = ldr * 255;
	ldr.convertTo(ldr, CV_8UC3);
	imshow("Robertson_HDR", hdr);
	imshow("Robertson_LDR", ldr);
	imwrite("D:\\opencv_c++\\Learning-OpenCV\\�߶�̬��HDR��ͼ��\\Robertson_LDR.png", ldr);

}