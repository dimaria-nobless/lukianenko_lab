#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h> 
#include <stdlib.h>
#include <stdio.h>

__int64 calcHash(IplImage* image, bool show_results = false);// ������������ ��� ��������
__int64 calcHammingDistance(__int64 x, __int64 y);// ����������� ���������� ��������

int main(int argc, char* argv[]) // ���������� ���������� � ������ ����������
{
	IplImage *object = 0, *image = 0; // ��� ��� �������

	char obj_name[] = "bus2.png"; // ��������� �����������
	char img_name[] = "bus3.png";

	// ��� ������� (������ ��������) // ��������� �������� // �� ������ ������ ����� �������
	char* object_name = argc >= 2 ? argv[1] : obj_name;
	// ��� �������� (������ ��������)
	char* image_name = argc >= 3 ? argv[2] : img_name;

	// ���� 1 �� �������� �������� (1 ������)
	object = cvLoadImage(object_name, 1);
	image = cvLoadImage(image_name, 1);

	printf("[i] object: %s\n", object_name);
	printf("[i] image: %s\n", image_name);
	// �������� �� �� ����� �����������
	if (!object) {
		printf("[!] Error: %s\n", object_name);
		return -1;
	}
	if (!image) {
		printf("[!] Error: %s\n", image_name);
		return -1;
	}

	// ������� ����������� // ��� ��� object � ���� ����������
	cvNamedWindow("object");
	cvShowImage("object", object);
	cvNamedWindow("image");
	cvShowImage("image", image);

	// �������� ���
	__int64 hashO = calcHash(object, true);

	__int64 hashI = calcHash(image, false);

	// ���������� ���������� ��������
	__int64 dist = calcHammingDistance(hashO, hashI);

	printf("[i] Hamming distance: %I64d \n", dist);

	// ��� ������� �������
	cvWaitKey(0);

	// ����������� �������
	cvReleaseImage(&object);
	cvReleaseImage(&image);

	// ������� ����
	cvDestroyAllWindows();
	return 0;
}

// ���������� ��� ��������
__int64 calcHash(IplImage* src, bool show_results)
{
	if (!src) {
		return 0;
	}

	IplImage *res = 0, *gray = 0, *bin = 0;

	res = cvCreateImage(cvSize(8, 8), src->depth, src->nChannels);
	gray = cvCreateImage(cvSize(8, 8), IPL_DEPTH_8U, 1);
	bin = cvCreateImage(cvSize(8, 8), IPL_DEPTH_8U, 1);

	// ��������� ��������
	cvResize(src, res);
	// ��������� � �������� ������
	cvCvtColor(res, gray, CV_BGR2GRAY);
	// ��������� �������
	CvScalar average = cvAvg(gray);
	printf("[i] average: %.2f \n", average.val[0]);
	// ������� �������� ����������� ������������ ��������
	// ��� ����� ������������� ��������� ���������������
	cvThreshold(gray, bin, average.val[0], 255, CV_THRESH_BINARY);

	// �������� ���
	__int64 hash = 0;

	int i = 0;
	// ����������� �� ���� �������� �����������
	for (int y = 0; y < bin->height; y++) {
		uchar* ptr = (uchar*)(bin->imageData + y * bin->widthStep);
		for (int x = 0; x < bin->width; x++) {
			// 1 ����� (� ��������� ������)
			if (ptr[x]) {
				hash |= 1i64 << i;
			}
			i++;
		}
	}

	printf("[i] hash: %I64X \n", hash);

	if (show_results) {
		// ����������� �������� ��� ����������� ����������� // ��������� ��� �������������� ��������
		IplImage* dst3 = cvCreateImage(cvSize(128, 128), IPL_DEPTH_8U, 3);
		IplImage* dst1 = cvCreateImage(cvSize(128, 128), IPL_DEPTH_8U, 1);

		// ���������� ��������, ������� ����
		cvNamedWindow("64");
		cvResize(res, dst3, CV_INTER_NN);
		cvShowImage("64", dst3);
		cvNamedWindow("gray");
		cvResize(gray, dst1, CV_INTER_NN);
		cvShowImage("gray", dst1);
		cvNamedWindow("bin");
		cvResize(bin, dst1, CV_INTER_NN);
		cvShowImage("bin", dst1);

		cvReleaseImage(&dst3);
		cvReleaseImage(&dst1);
	}

	// ����������� �������
	cvReleaseImage(&res);
	cvReleaseImage(&gray);
	cvReleaseImage(&bin);

	return hash;
}

__int64 calcHammingDistance(__int64 x, __int64 y) // ������� ���������� �������� ����� ����� ������
{
	__int64 dist = 0, val = x ^ y;

	// ��������� ���������� ������������� �����
	while (val)
	{
		++dist;
		val &= val - 1;
	}

	return dist;
}