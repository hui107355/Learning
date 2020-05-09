#include<opencv2/opencv.hpp>
#include<stdlib.h>
#include<vector>
#include<string>
using namespace std;
using namespace cv;


//��������ͼ��ķ�ֵ�����PSNR��ԽС��ʾ����ͼ��Խ���ƣ�����0���ʾ����ͼ����ȫ��ͬ
//����ÿ���������ó��Ľ�����ܺ������Ӿ��нϴ����
double getPSNR(const Mat& I1, const Mat& I2)
{
    Mat s1;
    absdiff(I1, I2, s1);       // ��������ͼ���ֵ�ľ���ֵ|I1 - I2|
    s1.convertTo(s1, CV_32F);  // �ڽ���ƽ������֮ǰ�ȼ���ͼ�����
    s1 = s1.mul(s1);           //�Բ�ֵ����ֵ������ƽ�� |I1 - I2|^2
    Scalar s = sum(s1);        // ��ÿ��ͨ��������ֵ���
    double sse = s.val[0] + s.val[1] + s.val[2]; // ��ͨ����������ֵ
    if (sse <= 1e-10)           //�������ͼ���ֵ����ֵ��ƽ����ͨ���ܺͺ�С�����޲��
        return 0;
    else
    {
        double mse = sse / (double)(I1.channels() * I1.total());            //������� = ��ֵƽ���� / ��������
        double psnr = 10.0 * log10((255.0 * 255.0) / mse);          //��ֵ�����
        return psnr;
    }
}


//��������ͼ���ƽ���ṹ�����ԣ�������ͨ����0~1�����ƶȶ�����Խ�ӽ�1��ʾԽ����
//��ͼ��ṹ�Ͻ��м������ƶȣ������������Ӿ��۸�
Scalar getMSSIM(const Mat& i1, const Mat& i2)
{
    const double C1 = 6.5025, C2 = 58.5225;
    int d = CV_32F;
    Mat I1, I2;
    i1.convertTo(I1, d);            
    i2.convertTo(I2, d);
    Mat I2_2 = I2.mul(I2);        // I2^2
    Mat I1_2 = I1.mul(I1);        // I1^2
    Mat I1_I2 = I1.mul(I2);        // I1 * I2
    Mat mu1, mu2;                   
    GaussianBlur(I1, mu1, Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, Size(11, 11), 1.5);
    Mat mu1_2 = mu1.mul(mu1);
    Mat mu2_2 = mu2.mul(mu2);
    Mat mu1_mu2 = mu1.mul(mu2);
    Mat sigma1_2, sigma2_2, sigma12;
    GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;
    GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;
    GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;
    Mat t1, t2, t3;
    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);                 // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))
    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);                 // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))
    Mat ssim_map;
    divide(t3, t1, ssim_map);        // ssim_map =  t3./t1;
    Scalar mssim = mean(ssim_map);   // mssim��ssim��ƽ��ֵ
    return mssim;
}


int main()
{
    vector<string>path;
    glob("C:\\Users\\QXB\\Desktop\\Product_information_detection\\Printing detection\\test\\uncompleted_issue\\miss_patch_issue\\", path);
    //glob("C:\\Users\\QXB\\Desktop\\Product_information_detection\\Printing detection\\test\\uncompleted_issue\\single_char_issue\\", path);
    //glob("C:\\Users\\QXB\\Desktop\\Product_information_detection\\Printing detection\\test\\illumination_issue\\", path);
    //glob("C:\\Users\\QXB\\Desktop\\Product_information_detection\\Printing detection\\test\\color_issue\\", path);
    //glob("C:\\Users\\QXB\\Desktop\\Product_information_detection\\Printing detection\\test\\bold_issue\\", path);

    Mat tem_image = imread("C:\\Users\\QXB\\Desktop\\Product_information_detection\\Printing detection\\template\\true_texts.png");
    //��ȡɫ��H�ͱ��Ͷ�Sͨ������������Vͨ����Ӱ��
    cvtColor(tem_image, tem_image, COLOR_BGR2HSV);
    vector<Mat>tem_hsv;
    split(tem_image, tem_hsv);
    vector<Mat>tem_hs;
    tem_hs.push_back(tem_hsv[0]);
    tem_hs.push_back(tem_hsv[1]);
    Mat tem_image_hs;
    merge(tem_hs, tem_image_hs);

    for (int i = 0; i < path.size(); i++)
    {
        Mat test_image = imread(path[i]);
        cvtColor(test_image, test_image, COLOR_BGR2HSV);
        vector<Mat>test_hsv;
        split(test_image, test_hsv);
        vector<Mat>test_hs;
        test_hs.push_back(test_hsv[0]);
        test_hs.push_back(test_hsv[1]);
        Mat test_image_hs;
        merge(test_hs, test_image_hs);

        //�ڶ���Ƶ���������ƶȼ��ʱ�����ڷ�ֵ�����PSNR�ļ������Ƚ�С�����Կ��Զ�ÿ֡ͼ�����PSNR�Ƚϣ���PSNRС����ֵʱ����ͨ��ƽ���ṹ������MSSIM���бȽϡ�
        double PSNR = getPSNR(tem_image_hs, test_image_hs);
        if (PSNR < 20)
        {
            Scalar MSSIM = getMSSIM(tem_image_hs, test_image_hs);
            if (MSSIM[0] > 0.995 && MSSIM[1] > 0.995)
            {
                cout << "����ͼ��" << i << "��PSNR=" << PSNR << "\tMSSIM=" << MSSIM << endl;
            }
        }
        else
        {
            cout << "����ͼ��" << i << "��PSNR=" << PSNR << endl;
        }

        //Scalar MSSIM = getMSSIM(tem_image_hs, test_image_hs);
        //cout << "����ͼ��" << i << "��PSNR=" << PSNR << "\tMSSIM=" << MSSIM << endl;
    }
   

    cv::waitKey(0);
    return 0;
}