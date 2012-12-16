#include <iostream>

#include "EasyBMP.h"
#include "GlobalMap.cpp"

/*TODO
 * Variabilité en fonction de x et y.
 * Erosion.
 */ 

using namespace std;

void w2fBIG(GlobalMap &globalMap, int number, int lsize=1025, int gsizex=3, int gsizey=3)
{
	short z;

	RGBApixel color;
	BMP image;
	image.SetSize(gsizex*lsize,gsizey*lsize);
	image.SetBitDepth(32);

	for(int b=0; b<gsizey; b++)
	{
		for(int a=0; a<gsizex; a++)
		{
			for(int y=0; y<lsize; y++)
			{
				for(int x=0; x<lsize; x++)
				{
					z= globalMap.getLocalMap(a,b)->getHeight(x,y);
					if(z<0)
					{
						color.Red=0;
						color.Green=0;
						color.Blue=(z+32768)/129;
						color.Alpha=0;
					}
					if(z>=0 && z<20000)
					{	
						color.Red=0;
						color.Green=z/134+100;
						color.Blue=0;
						color.Alpha=0;
					}
					if(z>=20000 && z<25000)
					{
						color.Red=(z-20000)/500+200;
						color.Green=(z-20000)/500+200;
						color.Blue=(z-20000)/500+200;
						color.Alpha=0;
					}
					if(z>=25000)
					{
						color.Red=255;
						color.Green=255;
						color.Blue=255;
						color.Alpha=0;
					}
					/*
					if(z%250<5 && z%250>-5)
					{
						color.Red=0;
						color.Green=0;
						color.Blue=0;
						color.Alpha=0;
					}
					*/
					image.SetPixel(x+a*1025,y+b*1025, color);
				}
			}
		}
	}
	char fileName[12];
	sprintf(fileName, "map_%d.bmp", number);
	image.WriteToFile(fileName);
}

void w2f(GlobalMap &globalMap, int i, int j, int size=1025)
{
	char fileName[6] = {'0','0','.','b','m','p'};
	fileName[0]=i+48;
	fileName[1]=j+48;
		
	short z;
		
	RGBApixel color;
	BMP image;
	image.SetSize(size,size);
	image.SetBitDepth(16);

	for(int y=0; y<size; y++)
	{
		for(int x=0; x<size; x++)
		{
			z= globalMap.getLocalMap(i,j)->getHeight(x,y);
			if(z<0)
			{
				color.Red=0;
				color.Green=0;
				color.Blue=(z+32768)/129;
				color.Alpha=0;
			}
			if(z>=0 && z<20000)
			{	
				color.Red=0;
				color.Green=z/134+100;
				color.Blue=0;
				color.Alpha=0;
			}
			if(z>=20000 && z<25000)
			{
				color.Red=(z-20000)/500+200;
				color.Green=(z-20000)/500+200;
				color.Blue=(z-20000)/500+200;
				color.Alpha=0;
			}
			if(z>=25000)
			{
				color.Red=255;
				color.Green=255;
				color.Blue=255;
				color.Alpha=0;
			}
			
			image.SetPixel(x,y, color);
		}
	}

	image.WriteToFile(fileName);
}

int main()
{
	for (int k=0; k<10; k++)
	{
		srand(time(NULL));
		
		GlobalMap globalMap;
		
		for(int j=0; j<3; j++)
			for(int i=0; i<3; i++)
			{
				cout << i << "," << j << endl;
				globalMap.genLocalMap(i,j);
				//w2f(globalMap, i,j);
				//globalMap.exportInMeters(i,j);
				cout << endl;
			}
		w2fBIG(globalMap, k);
	}
	//globalMap.genLocalMap(1,1);
	//w2f(globalMap, 1,1);
	cout << "Done!" << endl;
	return 0;
}
