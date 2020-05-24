#include "QtWidgetsApplication.h"
#include<qlabel.h>
#include<qpushbutton.h>
#include<qimage.h>
#include<qpixmap.h>
#include<qtimer.h>
#include<qslider.h>
#include<opencv.hpp>
#include<string>
#include"face.h"
#include"face_mask.h"
using namespace cv;
using namespace std;
using namespace cv::dnn;

QtWidgetsApplication::QtWidgetsApplication(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	//����ģ�ͣ����ü����̨��Ŀ���豸
    string model_path = "D:\\opencv_c++\\opencv_tutorial\\data\\models\\face_detector\\opencv_face_detector_uint8.pb";
    string config_path = "D:\\opencv_c++\\opencv_tutorial\\data\\models\\face_detector\\opencv_face_detector.pbtxt";
    Net face_detector = readNetFromTensorflow(model_path, config_path);
    face_detector.setPreferableBackend(DNN_BACKEND_INFERENCE_ENGINE);
    face_detector.setPreferableTarget(DNN_TARGET_CPU);
    auto detecModel = ml::SVM::load("face_mask_detection.xml");
	//������ʾ��ɫ����
    QSize label_size = ui.label->size();
    Mat background = Mat::zeros(Size(label_size.width(),label_size.height()) , CV_8UC3);
    QImage back = QImage((const uchar*)background.data, background.cols, background.rows, QImage::Format::Format_BGR888);
    ui.label->setPixmap(QPixmap::fromImage(back.scaled(label_size, Qt::AspectRatioMode::KeepAspectRatio)));
	//���ó�ʼ��ť״̬
	ui.pushButton2->setDisabled(true);
	ui.pushButton3->setDisabled(true);
	//��������ͷ�����㶨ʱ��
	static VideoCapture capture;
	QTimer* timer = new QTimer;
	timer->setInterval(30);
	connect(timer, &QTimer::timeout, ui.label, [=]() {
		if (!capture.isOpened())
		{
			capture.open(0);
		}
		Mat frame;
		capture.read(frame);
		QImage qframe((const uchar*)frame.data, frame.cols, frame.rows, QImage::Format::Format_BGR888);
		ui.label->setPixmap(QPixmap::fromImage(qframe.scaled(label_size, Qt::KeepAspectRatio)));
		});
	//�������������ͷ����ť
	connect(ui.pushButton1, &QPushButton::clicked, timer, [=]() {
		ui.pushButton1->setDisabled(true);
		ui.pushButton2->setDisabled(false);
		ui.pushButton3->setDisabled(false);
		timer->start();
		});
	//���ÿ��ּ�ⶨʱ��
	QTimer* timerMask = new QTimer;
	timerMask->setInterval(30);
	connect(timerMask, &QTimer::timeout, ui.label, [=]() {
		Mat frame;
		capture.read(frame);
		FaceMaskDetect(frame, detecModel, face_detector);
		QImage qframe((const uchar*)frame.data, frame.cols, frame.rows, QImage::Format::Format_BGR888);
		ui.label->setPixmap(QPixmap::fromImage(qframe.scaled(label_size, Qt::KeepAspectRatio)));
		});
	//�����¡����ּ�⡱��ť
	connect(ui.pushButton3, &QPushButton::clicked, timerMask, [=]() {
		ui.pushButton1->setDisabled(true);
		ui.pushButton2->setDisabled(false);
		ui.pushButton3->setDisabled(true);
		timer->stop();
		timerMask->start();
		});
	//�����¡��ر�����ͷ����ť
	connect(ui.pushButton2, &QPushButton::clicked, ui.label, [=]() {
		ui.pushButton3->setDisabled(true);
		ui.pushButton1->setDisabled(false);
		ui.pushButton2->setDisabled(true);
		if (timer->isActive())
		{
			timer->stop();
		}
		timerMask->stop();
		capture.release();
		Mat background = Mat::zeros(Size(ui.label->width(), ui.label->height()), CV_8UC3);
		QImage back = QImage((const uchar*)background.data, background.cols, background.rows, QImage::Format::Format_BGR888);
		ui.label->setPixmap(QPixmap::fromImage(back.scaled(label_size, Qt::AspectRatioMode::KeepAspectRatio)));
		});
}
