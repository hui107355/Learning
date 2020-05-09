#include<stdlib.h>
#include<opencv2/opencv.hpp>
#include<string>
using namespace std;
using namespace cv;

int main()
{
	/***********************CamShift��Continuously Adaptive MeanShift���㷨***********************/
	 VideoCapture capture;
	//capture.open("D:\\opencv_c++\\opencv_tutorial\\data\\images\\balltest.mp4");
	capture.open(0);
	if (!capture.isOpened())
	{
		return 0;
	}
	//��ȡ��һ֡ͼ����ROI���򲢽���ת��ΪHSVͼ��
	Mat first_image, first_hsv, roi_image, roi_hsv;
	capture.read(first_image);
	//������ѡ��ROI���򣬿ո��ѡ��
	Rect roi_rect = selectROI("first_image", first_image, false, false);
	roi_image = first_image(roi_rect).clone();
	cvtColor(roi_image, roi_hsv, COLOR_BGR2HSV);
	//����ROIͼ���ֱ��ͼ
	Mat roi_hist;
	int dims = 2;
	int channels[2] = { 0,1 };
	int histSize[2] = { 180,255 };
	float ranges_h[2] = { 0,180 };
	float ranges_s[2] = { 0,255 };
	const float* histRanges[2] = { ranges_h, ranges_s };
	calcHist(&roi_hsv, 1, channels, Mat(), roi_hist, dims, histSize, histRanges);
	normalize(roi_hist, roi_hist, 0, 255, NORM_MINMAX);

	Mat frame, frame_hsv;
	vector<Point2f> centers;
	while (capture.read(frame))
	{
		flip(frame, frame, 1);
		cvtColor(frame, frame_hsv, COLOR_BGR2HSV);
		//��ÿһ֡ͼ����з���ͶӰ
		Mat backProject;
		calcBackProject(&frame_hsv, 1, channels, roi_hist, backProject, histRanges);
		//����CamShift�㷨�����ϸ���Ŀ�괰��λ�ã�����ֵΪһ������λ�úͽǶȵ���ת����
		RotatedRect new_rect = CamShift(backProject, roi_rect, TermCriteria(TermCriteria::COUNT | TermCriteria::EPS, 10, 1));
		// //����probImage������ĸ��ʷֲ�ͼ��Ҳ���Ƿ���ͶӰͼ��
		//	//����window��Ŀ��λ�õĳ�ʼ���ڣ�
		//	//����criteria��Ǩ����ֹ������TermCriteria::EPSΪǨ�ƾ���С����ֵʱֹͣǨ�ƣ�TermCriteria::COUNTΪǨ�Ƶ��������ﵽ�趨�����ֵʱֹͣǨ�ƣ���������ͬʱʹ�ã�
		ellipse(frame, new_rect, Scalar(0, 255, 0), 1, 8);			//����Ŀ������λ��

		//��ȡ��ǰĿ���������꣬������㼯�У�����������Ŀ���ƶ��켣������㼯�еĵ������ﵽ�涨����������յ㼯��Ҳ����ÿ��ֻ����������ƶ��켣
		Point2f center = new_rect.center;	
		if (centers.size() % 20 == 0)
		{
			centers.clear();
		}
		centers.push_back(center);

		if (centers.size() > 2)		//ֻ�е�Ŀ������ƶ�ʱ��Ҳ����Ŀ����������������������ʱ�Ž��л���
		{
			for (size_t k = 1; k < centers.size(); k++)
			{
				float  flag = float(k) / float(centers.size());			//��ǵ�ǰ��λ�ڵ�ǰ���ƵĹ켣�е�λ��
				//ͨ�����λ�õĲ�ͬ�������Ʋ�ͬ��ϸ�����ߣ��Ӷ�ʹ���Ƴ��Ĺ켣����������ʧ��Ч��������������β��
				if (flag < 0.2)
				{
					line(frame, centers[k - 1], centers[k], Scalar(0, 255, 0), 1, LINE_AA, 0);
				}
				else if(flag >= 0.2 && flag <= 0.4)
				{
					line(frame, centers[k - 1], centers[k], Scalar(0, 255, 0), 2, LINE_AA, 0);
				}
				else if (flag >= 0.4 && flag <= 0.6)
				{
					line(frame, centers[k - 1], centers[k], Scalar(0, 255, 0), 3, LINE_AA, 0);
				}
				else if(flag >= 0.6 && flag <= 0.8)
				{
					line(frame, centers[k - 1], centers[k], Scalar(0, 255, 0), 4, LINE_AA, 0);
				}
				else if (flag >= 0.8 && flag <= 1)
				{
					line(frame, centers[k - 1], centers[k], Scalar(0, 255, 0), 5, LINE_AA, 0);
				}
			}
		}
		imshow("frame", frame);

		char ch = waitKey(20);
		if (ch == 27)
		{
			break;
		}

	}
	capture.release();


	waitKey(0);
	return 0;
}