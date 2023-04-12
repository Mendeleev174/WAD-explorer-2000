/*
			Screen Deployer by -=AS=-
				9.9.2000
*/
#include <stdio.h>
#include <string.h>

int dta1;
unsigned int i;

char ident[]="WE2";
char ident_buf[]="???";
unsigned char one_byte;

FILE *bmp,*f;

main()
{
if((f=fopen("we2000.exe","r+b"))==NULL){printf("FUCK THE EXE!");goto quit;}	//открываем файл
if((bmp=fopen("screen.bmp","rb"))==NULL){printf("FUCK THE BMP");goto quit;}	//открываем файл
//-------Search identify---------
dta1=0;
for(i=0;i<65078;i++)
	{
	ident_buf[dta1]=fgetc(f);
	if((ident_buf[dta1]!=ident[dta1])&&(dta1>0))dta1=0;
	if(ident_buf[dta1]==ident[dta1])dta1++;
	if(dta1==3)
		{
		printf("Ok...I found identify!");
		printf("  %s",ident_buf);
		break;
		}
  //	goto not_found;
	}
//-------Copying data------------
printf("\nCopying image...");
fseek(f,i+1,0);
for(i=0;i<65078;i++)
	{
	one_byte=fgetc(bmp);
	fputc(one_byte,f);
	}
if(i==65078)
	{
	printf("Ok!\n");
	}
else	{
	printf("Error!\n");
	}

not_found:;
fclose(f);
fclose(bmp);

quit:;
}