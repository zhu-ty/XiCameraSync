// include std
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <thread>
#include <memory>

#ifdef WIN32
#include <windows.h>
#include "xiApi.h"       // Windows
#else
#include <m3api/xiApi.h> // Linux, OSX
#endif

// opencv
#include <opencv2/opencv.hpp>

#define checkXIMEAErrors(val)  checkXIMEAInternal ( (val), #val, __FILE__, __LINE__ )

void checkXIMEAInternal(XI_RETURN result, char const *const func,
	const char *const file, int const line) 
{
	if (result != XI_OK) 
	{
		char info[256];
		sprintf(info, "XIMEA camera error at %s:%d function: %s\n",
			file, line, func);
		std::cout <<info <<std::endl;
	}
}

int main(int argc, char* argv[]) 
{
	PDWORD pnum = new DWORD;
	checkXIMEAErrors(xiGetNumberDevices(pnum));
	int num = *pnum;
	std::vector<HANDLE> hcams;
	hcams.resize(num);
	printf("%d XIMEA cameras detected\n", num);

	//char image1[18000000], image2[18000000];


	for (size_t i = 0; i < num; i++)
	{
		checkXIMEAErrors(xiOpenDevice(i, &hcams[i]));
		checkXIMEAErrors(xiSetParamInt(hcams[i], XI_PRM_BUFFERS_QUEUE_SIZE, 3));
		checkXIMEAErrors(xiSetParamInt(hcams[i], XI_PRM_EXPOSURE, 50000));
	}

	printf("Camera Opened\n");

	xiSetParamInt(hcams[0], XI_PRM_GPO_SELECTOR, 2);// set trigger mode on camera2 - as slave
	xiSetParamInt(hcams[0], XI_PRM_GPO_MODE, XI_GPO_HIGH_IMPEDANCE);
	xiSetParamInt(hcams[0], XI_PRM_GPI_SELECTOR, 2);
	xiSetParamInt(hcams[0], XI_PRM_GPI_MODE, XI_GPI_TRIGGER);
	xiSetParamInt(hcams[0], XI_PRM_TRG_SOURCE, XI_TRG_EDGE_RISING);
															
	xiSetParamInt(hcams[1], XI_PRM_GPO_SELECTOR, 2);// set trigger mode on camera2 - as slave
	xiSetParamInt(hcams[1], XI_PRM_GPO_MODE, XI_GPO_HIGH_IMPEDANCE);
	xiSetParamInt(hcams[1], XI_PRM_GPI_SELECTOR, 2);
	xiSetParamInt(hcams[1], XI_PRM_GPI_MODE, XI_GPI_TRIGGER);
	xiSetParamInt(hcams[1], XI_PRM_TRG_SOURCE, XI_TRG_EDGE_RISING);
	// start
	xiStartAcquisition(hcams[0]);
	xiStartAcquisition(hcams[1]);
	Sleep(1234); // wait for right moment to trigger the exposure
				 // trigger acquisition on Master camera
	//xiSetParamInt(hcams[0], XI_PRM_TRG_SOFTWARE, 1);
	// get image from both cameras
	XI_IMG m1, m2;
	xiGetImage(hcams[0], 1000, &m1);
	xiGetImage(hcams[1], 1000, &m2);
	// process images here

	cv::Mat mm1(3008, 4112, CV_8U, m1.bp);
	cv::Mat mm2(3008, 4112, CV_8U, m2.bp);
	cv::imwrite("mm1.png", mm1);
	cv::imwrite("mm2.png", mm2);

	xiCloseDevice(hcams[0]);
	xiCloseDevice(hcams[1]);




	std::cout << "Suc!\nPress any key to continue..." << std::endl;
	getchar();
	return 0;
}

