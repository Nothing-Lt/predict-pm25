#include "Data.h"
#include "RBFPredict.h"
//#include "display.h"
#include <stdio.h>
#include <time.h>
#include <wiringPi.h>
#include "SSD1306.h"
#include "font.h"
#include "tools.h"

double get_current_pm25()
{
	Data d;
	double tt[VALUE_NUMBER];
	system("python Getpollution.py");
	d.GetDataFromFile();
	d.GetValue(tt);

	printf("AM");
	return tt[5];
}
double predict_pm25(int choice)
{
	Data d;
	double tt[VALUE_NUMBER];
	printf("PM");
	
	if(!choice)
	{
		system("python TianQiHouBao.py");
	}
	else
	{
		system("python Getpollution.py");
	}
	d.GetDataFromFile();
	d.GetValue(tt);
	if(!d.GetNormalizedValue(tt))
	{
		cout<<"Err"<<endl;
		exit(0);
	}
	cout<<setiosflags(ios::fixed); 
	for(int i=0;i<VALUE_NUMBER;i++)
	{
		cout<<i<<endl;
		cout<<tt[i];
	}
	RBFPredict p;
	while(!p.Load());
	p.SetInputValue(tt,VALUE_NUMBER);
	return p.Run()*PART_PM25+MIN_PM25;
}

char run()
{
	double past_tomorrow_pm25=-1,tomorrow_pm25=-1;
	char stat='c';
	past_tomorrow_pm25=predict_pm25(0);
	tomorrow_pm25=predict_pm25(1);
	
	if(tomorrow_pm25>past_tomorrow_pm25)
	{
		stat='b';
	}
	if(tomorrow_pm25==past_tomorrow_pm25)
	{
		stat='c';
	}
	if(tomorrow_pm25<past_tomorrow_pm25)
	{
		stat='d';
	}
	return stat;
}

int main()
{
	
	time_t t;
	struct tm *p;
	
	double today_pm25;
	char stat='c';

	double tt[VALUE_NUMBER];
	char buff[BUFFER_SIZE]={0};
	
	correct_time();
	system("python getlocation.py");
	system("python GetImportant.py");
	
	if(wiringPiSetup()<0)
	{
		cout<<"Err"<<endl;
	}
	init(SSD1306_SWITCHCAPVCC);
	today_pm25=get_current_pm25();
	stat=run();
	sprintf(buff,"a%.2f;%c",today_pm25,stat);
	run_char(buff);
	display();
	while(1)
	{
		time(&t);
		p=localtime(&t);
		if(0==p->tm_min)
		{
			if(7==p->tm_hour)
			{
				today_pm25=get_current_pm25();
				sprintf(buff,"a%.2f",today_pm25);
				run_char(buff);
			}
			else if(19==p->tm_hour)
			{
				stat=run();
				sprintf(buff,"a%.2f;%c",today_pm25,stat);
				run_char(buff);
			}
			
		}
		sleep(10);
	}
	return 0;
}
