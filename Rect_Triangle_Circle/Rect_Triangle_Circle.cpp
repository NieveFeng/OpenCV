// Rect_Triangle_Circle.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

Mat src_image, dst_image, canny_image,srcGray_image;
int ContoursThreshold = 80;
RNG g_rng(12345);
vector<vector<Point>> vContours;
vector<Vec4i>vHierarchy;

static void on_ThresholdChange(int, void*);

void on_ThresholdChange(int, void*)
{
    //用canny算子检测边缘
    Canny(srcGray_image, canny_image, ContoursThreshold,
        double(ContoursThreshold) * 2 + 1, 3);

    //寻找轮廓
    findContours(canny_image, vContours, vHierarchy,
        RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0 ));

    //绘制轮廓
    Mat drawing = Mat::zeros(canny_image.size(), CV_8SC3);
    for (int i = 0; i < vContours.size(); i++)
    {
        Scalar color = Scalar(g_rng.uniform(0, 0), 
            g_rng.uniform(0, 0), g_rng.uniform(0, 255));
        drawContours(drawing, vContours, i, color, 2, 8, vHierarchy, 0, Point());
    }
    imshow("效果图", drawing);
}

int main()
{
    src_image = imread("测试样本.jpg", 1);
    if (!src_image.data)
    {
        printf("读取失败!");
        return false;
    }
    
    //转成灰度并模糊化降噪
    cvtColor(src_image, srcGray_image, COLOR_BGR2GRAY);
    blur(srcGray_image, srcGray_image, Size(3, 3));

    //创造窗口
    namedWindow("效果图", WINDOW_NORMAL);

    //创造轨迹条并初始化
    createTrackbar("canny阈值", "效果图", &ContoursThreshold, 255, on_ThresholdChange);
    on_ThresholdChange(0, 0);

    while(waitKey(0)!='q')
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
