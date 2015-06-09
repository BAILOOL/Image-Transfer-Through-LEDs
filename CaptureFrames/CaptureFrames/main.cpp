#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
 
#include <iostream>
#include <stdio.h>

#define off_threshold_LED_1 50
#define off_threshold_LED_2 50
#define off_threshold_LED_3 50
#define off_threshold_LED_4 50


#define low_threshold_LED_1_up 140
#define	low_threshold_LED_2_up 140
#define low_threshold_LED_3_up 140
#define low_threshold_LED_4_up 140

#define low_threshold_LED_1_down 80
#define	low_threshold_LED_2_down 80
#define low_threshold_LED_3_down 80
#define low_threshold_LED_4_down 80

#define high_threshold_LED_1 230
#define high_threshold_LED_2 230
#define high_threshold_LED_3 230
#define high_threshold_LED_4 230



using namespace std;
using namespace cv;
 
uchar** MatToArray(Mat mat)
{
	uchar **array = new uchar*[mat.rows];
	for (int i=0; i<mat.rows; ++i)
		array[i] = new uchar[mat.cols];

	for (int i=0; i<mat.rows; ++i)
		array[i] = mat.ptr<uchar>(i);

	return array; 
}

int average(uchar** Temp, int height, int width)
{
	int sum = 0;
	int average;


	for (int y=0; y<height;++y)
	{
		for (int x=0; x<width;++x)
		{
			sum += int(Temp[y][x]);
		}
	}	

	average = int(sum/(height*width));

	return average;
}




#define FILE_SIZE 4096
unsigned char Buff[4096];
int temp_count = 0;
int flag_1 = 0;
int flag_2 = 0;
int flag_3 = 0;
int flag_4 = 0;
unsigned char Output_Array[FILE_SIZE];
int time_val = 0;
bool read1 = false;

int main(int argc, const char** argv)
{
	
    //setup video capture device and link it to the first capture device
    VideoCapture captureDevice;
    captureDevice.open(1);
 
    //setup image files used in the capture process
    Mat captureFrame;
 
    //create a window to present the results
    //namedWindow("outputCapture", 1);
	//namedWindow("Red", 1);
	//namedWindow("Green", 1);
	//namedWindow("Blue", 1);
	//namedWindow("Red_2", 1);
 
	//initialize the array
	for (int i = 0; i<4096; i++)
		Buff[i] = 255;

    //create a loop to capture
    while(true)
    {
		time_val++;
        //capture a new image frame
        captureDevice>>captureFrame;
		//imshow("outputCapture", captureFrame);

		int captureH = captureFrame.rows;
		int captureW = captureFrame.cols;

		Mat LED1(captureFrame(Rect(40,200,50,100)));
		Mat LED2(captureFrame(Rect(200,200,50,100)));
		Mat LED3(captureFrame(Rect(360,200,50,100)));
		Mat LED4(captureFrame(Rect(550,200,50,100)));
		
		//decomposition of rgb channels
		vector<Mat> channels_1(3);
		vector<Mat> channels_2(3);
		vector<Mat> channels_3(3);
		vector<Mat> channels_4(3);

		split(LED1,channels_1);
		split(LED2,channels_2);
		split(LED3,channels_3);
		split(LED4,channels_4);
		
		Mat LED1_Red = channels_1[0];
		Mat LED2_Green = channels_2[0];
		Mat LED3_Blue = channels_3[0];
		Mat LED4_Red = channels_4[0];
		
		//print the output
		//imshow("Red",LED1_Red);
		//imshow("Green",LED2_Green);
		//imshow("Blue",LED3_Blue);
		//imshow("Red_2",LED4_Red);

		/////////MatToArray////////////////////////
		uchar** LED1_Array = MatToArray(LED1_Red);
		uchar** LED2_Array = MatToArray(LED2_Green);
		uchar** LED3_Array = MatToArray(LED3_Blue);
		uchar** LED4_Array = MatToArray(LED4_Red);
		
		//Add your part here/////
		int height = LED1.rows;
		int width = LED1.cols;
		
		int LED1_Value = average(LED1_Array,height,width);
		int LED2_Value = average(LED2_Array,height,width);
		int LED3_Value = average(LED3_Array,height,width);
		int LED4_Value = average(LED4_Array,height,width);
		
		//printf("Average LED1 = %d\n", LED1_Value);
		//printf("Average LED2 = %d\n", LED2_Value);
		//printf("Average LED3 = %d\n", LED3_Value);
		//printf("Average LED4 = %d\n", LED4_Value);
		printf("current loop = %d\n", time_val%10);
		//////////////////////////
		
		if(read1)
			{
				read1=false;
				//waitKey(33);
				printf("Skiping frame");
				continue;
			}


		//disregard the frame
		if ((LED1_Value <= off_threshold_LED_1)||(LED2_Value <= off_threshold_LED_2)||(LED3_Value <= off_threshold_LED_3)||(LED4_Value <= off_threshold_LED_4))
		{	
			//pause for 33ms
			waitKey(33);
			continue;
		}

		
		else if (temp_count <=4096 && !read1)
		{
			
			/*printf("LED_1 = %d\n",LED1_Value );
			printf("LED_2 = %d\n",LED2_Value );
			printf("LED_3 = %d\n",LED3_Value );
			printf("LED_4 = %d\n",LED4_Value );*/
			
			Buff[temp_count++] = LED1_Value >= high_threshold_LED_1 ? 255:0;
			Buff[temp_count++] = LED2_Value >= high_threshold_LED_2 ? 255:0;
			Buff[temp_count++] = LED3_Value >= high_threshold_LED_3 ? 255:0;
			Buff[temp_count++] = LED4_Value >= high_threshold_LED_4 ? 255:0;


			printf("LED_1 = %d\n",LED1_Value );
			printf("LED_2 = %d\n",LED2_Value );
			printf("LED_3 = %d\n",LED3_Value );
			printf("LED_4 = %d\n",LED4_Value );

			printf("%d", LED1_Value >= high_threshold_LED_1 ? 1:0);
			printf("%d", LED2_Value >= high_threshold_LED_2 ? 1:0);
			printf("%d", LED3_Value >= high_threshold_LED_3 ? 1:0);
			printf("%d\n", LED4_Value >= high_threshold_LED_4 ? 1:0);
			
			
			/*
			if (LED1_Value >= high_threshold_LED_1) 
			{
				Buff[temp_count++] = 255;
				printf("LED_1 = 1 \n");
				if (flag_1 == 1)
					//printf("Error_LED1\n");
					//exit(0);
				flag_1 = 1;
			}
			else if ((LED1_Value >= low_threshold_LED_1_down)&&(LED1_Value <= low_threshold_LED_1_up))
				{
					printf("LED_1 = 0 \n");
					Buff[temp_count++] = 0;
					flag_1 = 0;
				}
			if (LED2_Value >= high_threshold_LED_2) 
			{
				Buff[temp_count++] = 255;
				printf("LED_2 = 1 \n");
				if (flag_2 == 1)
					//printf("Error_LED2\n");
					//exit(0);
				flag_2 = 1;
			}
			else if ((LED2_Value >= low_threshold_LED_2_down)&&(LED2_Value <= low_threshold_LED_2_up))
				{
					printf("LED_2 = 0 \n");
					Buff[temp_count++] = 0;
					flag_2 = 0;
				}
			if (LED3_Value >= high_threshold_LED_3) 
			{
				Buff[temp_count++] = 255;
				printf("LED_3 = 1 \n");
				if (flag_3 == 1)
					//printf("Error_LED3\n");
					//exit(0);
				flag_3 = 1;
			}
			else if ((LED3_Value >= low_threshold_LED_3_down)&&(LED3_Value <= low_threshold_LED_3_up)) 
				{
					printf("LED_3 = 0 \n");
					Buff[temp_count++] = 0;
					flag_3 = 0;
				}

			if (LED4_Value >= high_threshold_LED_4) 
			{
				Buff[temp_count++] = 255;
				printf("LED_4 = 1 \n");
				if (flag_4 == 1)
					//printf("Error_LED4\n");
					//exit(0);
				flag_4 = 1;
			}
			else if ((LED4_Value >= low_threshold_LED_4_down)&&(LED4_Value <= low_threshold_LED_4_up)) 
				{
					printf("LED_4 = 0 \n");
					Buff[temp_count++] = 0;
					flag_4 = 0;
				}

				//if (LED1_Value >= low_threshold) Buff[temp_count++] = 0;
			/*
			if (LED2_Value >= high_threshold) Buff[temp_count++] = 1;
			else if (LED2_Value >= low_threshold) Buff[temp_count++] = 0;

			if (LED3_Value >= high_threshold) Buff[temp_count++] = 1;
			else if (LED3_Value >= low_threshold) Buff[temp_count++] = 0;

			if (LED4_Value >= high_threshold) Buff[temp_count++] = 1;
			else if (LED4_Value >= low_threshold) Buff[temp_count++] = 0;
			*/
			read1=true;
		}
		
		


		
		printf("TEMP COUNT = %d\n",temp_count);

		Mat Final = Mat(64,64,CV_8UC1,Buff);
		imshow("OUTPUT",Final);
		if (temp_count >= 4096){
			break;
		} 
        //pause for 33ms
        waitKey(33);
    }

	//int Output_Array[64][64];
	//int temp = 0;

	//Put buffer into output array
	//for (int y=0; y<64; ++y)
	//{
	//	for (int x=0; x<64;++x)
	//	{
	//		Output_Array[y][x] = Buff[temp++];
	//	}
	//}

	//display the image
	printf("Completed");
	FILE* outputFile = fopen("C:\\Users\\Mid\\Desktop\\Multimedia\\Term Project\\TX part\\panda_output_64.raw","w");
	fwrite(Buff,FILE_SIZE,1,outputFile);
    return 0;

    //delete shoule be added to release memory
}