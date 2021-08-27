// OpenCV_test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

int ContrastValue = 80;			//对比度初始值
int BrightValue = 80;				//亮度初始值
int BoxFilterValue = 3;			//方框滤波初始值
int MeanBlurValue = 3;			//均值滤波初始值
int GaussianBlurValue = 3;		//高斯滤波初始值
int MedianBlurValue = 10;		//中值滤波初始值
int BilateralFilterValue = 10;	//双边滤波初始值
int Erode_Dilate = 0;			//0表示腐蚀，1表示膨胀
int Erode_DilateSize = 0;		//腐蚀与膨胀内核尺寸
int Open_Close = 0;				//0表示开运算，1表示闭运算
int Open_CloseSize = 0;			//开闭运算内核尺寸
int Top_Black = 0;				//0表示顶帽，1表示黑帽
int Top_BlackSize = 0;			//顶帽黑帽内核尺寸
int ThresholdType = 3;			//阈值化模式
int ThresholdValue = 0;			//阈值化参数(0-255)
int CannyLowThreshold = 0;		//Canny边缘检测
int SobelKernelSize = 0;		//Sobel边缘检测
int Houghthreshold = 100;		//霍夫变换阈值

Mat src_image, dst_image, tmp_image, srcGray_image;
Mat Sobel_X_image, Sobel_Y_image;	//定义Sobel算子在X\Y方向上的图像
String picture = "效果图";


//轨迹条回调函数声明
static void ContrastAndBright(int, void*);			//对比度与亮度
static void on_BoxFilter(int, void*);				//方框滤波		
static void on_MeanBlur(int, void*);				//均值滤波
static void on_GaussianBlur(int, void*);			//高斯滤波
static void on_MedianBlur(int, void*);				//中值滤波
static void on_BilateralFilter(int, void*);			//双边滤波
static void on_Erode_Dilate_Process(int, void*);		//腐蚀/膨胀处理
static void on_Open_Close_Process(int, void*);		//开闭运算处理
static void on_Top_Black_Process(int, void*);		//顶帽黑帽处理
static void on_Threshold(int, void*);				//阈值化
static void on_Canny(int, void*);					//Canny边缘检测
static void on_Sobel(int, void*);					//Sobel边缘检测
static void on_HoughLines(int, void*);				//霍夫线变换

/**********************************************************************/
//轨迹条回调函数定义
/**********************************************************************/
//对比度与亮度
static void ContrastAndBright(int, void*)
{
	for (int x = 0; x < src_image.rows; x++)
	{
		for (int y = 0; y < src_image.cols; y++)
		{
			for (int c = 0; c < 3; c++)
			{
				dst_image.at<Vec3b>(x, y)[c] =
					saturate_cast<uchar>((ContrastValue * 0.01) * (src_image.at<Vec3b>(x, y)[c]) + BrightValue);
			}
		}
	}
	imshow(picture, dst_image);
	//imshow("原图", src_image);
}

//方框滤波
static void on_BoxFilter(int, void*)
{
	boxFilter(src_image, dst_image, -1, Size(BoxFilterValue + 1, BoxFilterValue + 1));
	imshow(picture, dst_image);
}

//均值滤波
static void on_MeanBlur(int, void*)
{
	blur(src_image, dst_image, Size(MeanBlurValue + 1, MeanBlurValue + 1), Point(-1, -1));
	imshow(picture, dst_image);
}

//高斯滤波
static void on_GaussianBlur(int, void*)
{
	GaussianBlur(src_image, dst_image, Size(GaussianBlurValue * 2 + 1,
		GaussianBlurValue * 2 + 1), 0, 0);
	imshow(picture, dst_image);
}

//中值滤波
static void on_MedianBlur(int, void*)
{
	medianBlur(src_image, dst_image, MedianBlurValue * 2 + 1);
	imshow(picture, dst_image);
}

//双边滤波
static void on_BilateralFilter(int, void*)
{
	bilateralFilter(src_image, dst_image, BilateralFilterValue,
		double(BilateralFilterValue) * 2, BilateralFilterValue / 2);
	imshow(picture, dst_image);
}

//腐蚀与膨胀处理
static void on_Erode_Dilate_Process(int, void*)
{
	Mat element = getStructuringElement(MORPH_RECT,
		Size(2 * Erode_DilateSize + 1, 2 * Erode_DilateSize + 1),
		Point(Erode_DilateSize, Erode_DilateSize));
	if (Erode_Dilate == 0)
		erode(src_image, dst_image, element);
	else
		dilate(src_image, dst_image, element);
	imshow(picture, dst_image);
}

//开闭运算处理
static void on_Open_Close_Process(int, void*)
{
	Mat element = getStructuringElement(MORPH_RECT,
		Size(2 * Open_CloseSize + 1, 2 * Open_CloseSize + 1),
		Point(Open_CloseSize, Open_CloseSize));
	if (Open_Close == 0)
		morphologyEx(src_image, dst_image, MORPH_OPEN, element);
	else
		morphologyEx(src_image, dst_image, MORPH_CLOSE, element);
	imshow(picture, dst_image);
}

//顶帽黑帽处理
static void on_Top_Black_Process(int, void*)
{
	Mat element = getStructuringElement(MORPH_RECT,
		Size(2 * Top_BlackSize + 1, 2 * Top_BlackSize + 1),
		Point(Top_BlackSize, Top_BlackSize));
	if (Top_Black == 0)
		morphologyEx(src_image, dst_image, MORPH_TOPHAT, element);
	else
		morphologyEx(src_image, dst_image, MORPH_BLACKHAT, element);
	imshow(picture, dst_image);
}

//阈值化处理
static void on_Threshold(int, void*)
{
	threshold(src_image, dst_image, ThresholdValue, 255, ThresholdType);
	imshow(picture, dst_image);
}

//Canny边缘检测
void on_Canny(int, void*)
{
	//先使用均值滤波3x3内核来降噪
	blur(srcGray_image, tmp_image, Size(3, 3));

	//运行我们的Canny算子
	Canny(tmp_image, tmp_image, CannyLowThreshold, double(CannyLowThreshold) * 3, 3);

	//先将dst_image内所以元素设置为0
	dst_image = Scalar::all(0);

	//使用Canny算子输出边缘图tmp作为掩码，来将原图src拷到目标图dst中
	src_image.copyTo(dst_image, tmp_image);
	imshow(picture, dst_image);
}

//Sobel边缘检测
void on_Sobel(int, void*)
{
	//求X方向梯度
	Sobel(src_image, Sobel_X_image, CV_16S, 1, 0, (2 * SobelKernelSize + 1), 1, 1);
	//计算绝对值，并将结果转换为8位
	convertScaleAbs(Sobel_X_image, Sobel_X_image);

	//求Y方向梯度
	Sobel(src_image, Sobel_Y_image, CV_16S, 1, 0, (2 * SobelKernelSize + 1), 1, 1);
	//计算绝对值，并将结果转换为8位
	convertScaleAbs(Sobel_Y_image, Sobel_Y_image);

	//合并梯度
	addWeighted(Sobel_X_image, 0.5, Sobel_Y_image, 0.5, 0, dst_image);
	imshow(picture, dst_image);
}

//霍夫线变换
void on_HoughLines(int, void*)
{
	//进行一次canny边缘检测
	Canny(src_image, tmp_image, 50, 200, 3);

	//转化边缘检测后的图为灰度图
	cvtColor(tmp_image, dst_image, COLOR_GRAY2BGR);

	//定义一个矢量结构,用于存放得到的线段矢量集合
	vector<Vec4i>mylines;

	HoughLinesP(tmp_image, mylines, 1, CV_PI / 180, Houghthreshold + 1, 50, 10);

	//循环遍历绘制每一条线段
	for (size_t i = 0; i < mylines.size(); i++)
	{
		Vec4i L = mylines[i];
		line(dst_image, Point(L[0], L[1]), Point(L[2], L[3]),
			Scalar(23, 180, 55), 1, LINE_AA);
	}
	imshow(picture, dst_image);
}

/***************************************************************************************/
//主函数
/***************************************************************************************/

int main()
{
	src_image = imread("追光者.jpg");
	if (!src_image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	dst_image = Mat::zeros(src_image.size(), src_image.type());
	tmp_image = Mat::zeros(src_image.size(), src_image.type());
	tmp_image = src_image;
	//将原图像转换位灰度图像
	cvtColor(src_image, srcGray_image, COLOR_BGR2GRAY);

	//namedWindow("原图", WINDOW_NORMAL); 
	//resizeWindow("原图", 720, 720);
	//moveWindow("原图", 50, 50);

	namedWindow("效果图", WINDOW_NORMAL);	//命名窗口
	resizeWindow("效果图", 800, 720);		//改变窗口大小
	moveWindow("效果图", 720, 80);			//移动窗口

	namedWindow("参数栏", WINDOW_NORMAL);
	resizeWindow("参数栏", 700, 720);
	moveWindow("参数栏", 0, 50);

	/**********************************************************************************/
	//创建轨迹条
	/**********************************************************************************/
	//createTrackbar("对 比 度:", "参数栏", &ContrastValue, 300, ContrastAndBright);
	//createTrackbar("亮    度:", "参数栏", &BrightValue, 200, ContrastAndBright);
	//createTrackbar("方框滤波:", "参数栏", &BoxFilterValue, 40, on_BoxFilter);
	//createTrackbar("均值滤波:", "参数栏", &MeanBlurValue, 40, on_MeanBlur);
	//createTrackbar("高斯滤波 ", "参数栏", &GaussianBlurValue, 40, on_GaussianBlur);
	//createTrackbar("中值滤波 ", "参数栏", &MedianBlurValue, 40, on_GaussianBlur);
	//createTrackbar("双边滤波 ", "参数栏", &BilateralFilterValue, 40, on_GaussianBlur);
	createTrackbar("腐蚀/膨胀", "参数栏", &Erode_Dilate, 1, on_Erode_Dilate_Process);
	createTrackbar("内 核  1 ", "参数栏", &Erode_DilateSize, 21, on_Erode_Dilate_Process);
	createTrackbar("开闭运算 ", "参数栏", &Open_Close, 1, on_Open_Close_Process);
	createTrackbar("内 核  2 ", "参数栏", &Open_CloseSize, 21, on_Open_Close_Process);
	//createTrackbar("顶帽/黑帽", "参数栏", &Top_Black, 1, on_Top_Black_Process);
	//createTrackbar("内 核  3 ", "参数栏", &Top_BlackSize, 21, on_Top_Black_Process);
	createTrackbar("阈值 模式", "参数栏", &ThresholdType, 4, on_Threshold);
	createTrackbar("阈值 参数", "参数栏", &ThresholdValue, 255, on_Threshold);
	createTrackbar("Canny检测", "参数栏", &CannyLowThreshold, 120, on_Canny);
	createTrackbar("Sobel检测", "参数栏", &SobelKernelSize, 3, on_Sobel);
	createTrackbar("霍夫 变换", "参数栏", &Houghthreshold, 200, on_HoughLines);

	/*************************************************************************************/
	//调用回调函数
	/*************************************************************************************/
	//ContrastAndBright(ContrastValue, 0);
	//ContrastAndBright(BrightValue, 0);
	//on_BoxFilter(BoxFilterValue, 0);
	//on_MeanBlur(MeanBlurValue, 0);
	//on_GaussianBlur(GaussianBlurValue, 0);
	//on_MedianBlur(MedianBlurValue, 0);
	on_BilateralFilter(BilateralFilterValue, 0);
	on_Erode_Dilate_Process(0, 0);
	on_Open_Close_Process(0, 0);
	//on_Top_Black_Process(0, 0);
	on_Threshold(0, 0);
	on_Canny(0, 0);
	on_Sobel(0, 0);
	on_HoughLines(0, 0);

	//图像放大
	if (waitKey(0) == 'w')
	{
		tmp_image = src_image;
		resize(tmp_image, dst_image, Size(tmp_image.cols * 2, tmp_image.rows * 2));
		printf("图像放大\n");
		namedWindow("放大图", WINDOW_NORMAL);
		imshow("放大图", dst_image);
		picture = "放大图";
	}

	//图像缩小
	if (waitKey(0) == 's')
	{
		tmp_image = src_image;
		resize(tmp_image, dst_image, Size(tmp_image.cols / 2, tmp_image.rows / 2));
		printf("图像缩小\n");
		namedWindow("缩小图", WINDOW_NORMAL);
		imshow("缩小图", dst_image);
		picture = "缩小图";
	}

	//直方图均衡化
	if (waitKey(0) == 'e')
	{
		equalizeHist(srcGray_image, dst_image);
		imshow(picture, dst_image);
	}

	//退出程序按Esc
	while ((char)(waitKey(0)) != 27)
	{

	}
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
