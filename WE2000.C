/*
		     WAD EXPLORER 2000  v1.2  coded by -=AS=-

		 Begining   14.6.2000
		 Release    11.9.2000	(v1.0)

		 Create this version : 30.09.2000  (v1.1)
		 Create this version : 02.12.2000  (v1.2) Fix save BUG!

		 we2000.c	size 61867
*/
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <alloc.h>
//#include <dos.h>
//#include <bios.h>
#include <string.h>

#define MAXFILES 200		//максимальное кол-во файлов ОДНОГО типа
#define cur_y 15

void decode(void);		//COPYRIGHT!!!
void draw_cat_win(void);	//начертить окно каталога
void clr_cat_win(void);		//удалить окно каталога
void clr_job_file(void);	//удалить надпись "имя рабочего файла"
void memory(void);              //кол-во оставшейся памяти
int  Dirsearch(char*);          //поиск файлов *char в текущем каталоге
int  Extsearch(char*);		//поиск EXT *char файлов в текущем каталоге
int  Dirmove(char*,int);	//выбор *char файлов
int  Extmove(char*,int);	//выбор EXT *char файлов
void Show_cat(void);		//нарисовать каталог
void Show_ext(void);		//нарисовать каталог загружаемых файлов
int  Objsearch(void);		//поиск объектов в WAD файле
void Objmove(void);		//работа с объектами
void message(char*,int);	//печать сообщения об ошибке
void draw_obj_win(void);	//нарисовать окно для объектов
void draw_info_win(void);	//нарисовать окно информации
void clear_info_win(void);	//удалить информационную панель
void Show_obj(void);		//показать объекты
int  Type_obj(FILE*,unsigned long);//определить тип объекта
void print_type_obj(int,int);	//напечатать тип файлов
void Save_object(void);		//сохранить объект
void Load_object(void);		//обновить объект
void Down_standart_panel(void); //нарисовать нижнюю панель (стандартную)
void Down_dir_panel(void); 	//нарисовать нижнюю панель каталога
void Down_choise_panel(void); 	//нарисовать нижнюю панель выбора
void draw_yn_win(void);		//нарисовать Y/N панель
void clr_yn_win(void);		//удалить Y/N панель
void Del_object(void);		//удалить содержимое текущего объекта
void Information(void);		//показать информацию об объекте
void Red_panel(void);		//DOOMовская красная панель при выходе
void Put_to_video_memory(int,char,char);	//печать символа
void Show_title(void);		//показать заставку
void Show_help(void);		//показать помощь

//-----------FILES-----------
char *files[MAXFILES];
char now_wad_file[13];		//имя текущего файла
long fsize[MAXFILES];
//-----------EXT FILE--------
char *ext_files[MAXFILES];
char now_ext_file[13];		//имя загружаемого файла
long ext_fsize[MAXFILES];
//-----------WAD TEST--------
int source_wad[]={87,65,68};        //идентификатор DOOMа
int dest_wad[3];		//массив для идентификатора

int vers1=1,vers2=2;		//версия программы

int i,character,num;
int max_files;
int clearing=0;
int current_file,hi_window_file,current_pos;
int current_ext_file,hi_window_ext_file,current_ext_pos;

char coder[]={0x1a,0x00,0x00,0xa0,0x2b,0x0e,0x0f,0x4b,0x41,0x0d,0x09,0x44,0xc0,0xb9,0x3c,0x45,0x41,0x09,0x05,0x05,0x05,0x8d,0x00,0x41,0x4f,0x45,0x45,0x4f,0x4f,0x0e,0x0d,0x10,0x7c,0x12,0x6e,0x10,0x57,0x7a};
char copyr[]={52,36,88,74,36,52,'\0'};
//-----------СООБЩЕНИЯ ОБ ОШИБКАХ--------
char mess_0[]="не DOOMовский файл!";
char mess_2[]="доступен не полностью (нехватка памяти)!";
char mess_3[]="не найден!";

//-----------id Red panel-----------------------------
char idrp_0[]="                                                                  \n"
     " ──────────────────────────────────────────────────────────────── \n"
     "     DOOM - легенда. WAD EXPLORER 2000 - возможность взглянуть    \n"
     "   на легенду с другой стороны, почувствовать то, что чувствовали \n"
     "   создатели этой игры. Вы сможете :                              \n"
     "                                                                  \n"
     "   1.  Получить в своё распоряжение все сэмплы DOOMа и HERETICа   \n"
     "       в формате WAV.                                             \n"
     "                                                                  \n"
     "   2.  Удалять сэмплы из WAD файла.                               \n"
     "                                                                  \n"
     "   3.  Заменять стандартные сэмплы на свои собственные.           \n"
     "                                                                  \n"
     "                                                                  \n"
     "         Дата релиза данной версии - 2 декабря   2000 года        ";
char idrp_1[]="                       WAD EXPLORER 2000                     v .  ";
char idrp_2[]="Автор";

//-----------WE2000 Help------------------------------
char we2000_help[]="                                  Помощь                                        "
"                                                                                "
"    Программа предназначена для работы с игровыми WAD файлами(Heretic,Doom).    "
"    В версиях 1.x настоящей программы ,возможна работа только со звуковыми      "
"    объектами WAD файлов.                                                       "
"                                                                                "
"   Объекты:                                                                     "
"        S - активный звуковой объект                                            "
"       ░S - звуковой объект без данных                                          "
"    Объекты без указателей полностью пассивны.                                  "
" - Сохранение объектов:                                                         "
"    Сохранение возможно только активных звуковых объектов! Сохранение           "
"    производится в стандартный WAV файл в текущий каталог. Клавиша F2           "
" - Удаление объектов:                                                           "
"    Удаление объекта подразумевает под собой очищение звукового объекта         "
"    от содержащихся в нём данных. Клавиша DEL                                   "
" - Обновление объектов:                                                         "
"    Обновление объектов - заполнение активных звуковых объектов или             "
"    звуковых объектов без данных,сэмплами из внешних WAV файлов.                "
"    Внимание!!!WAV файлы должны быть только 8-ми битовыми с частотой            "
"    11025 или 22050 Герц в формате PCM!        Клавиша F3                       "
" - Клавиша ENTER - информация о текущем объекте                                 "
" - Alt-X (только в при выборе WAD файлов!) - выход в OS                         ";

char image[65081]="WE2";		//буфер картинки

//-----------ПЕРЕМЕННЫЕ ДЛЯ РАБОТЫ С ОБЪЕКТАМИ--------
struct doom_wad
	{
	unsigned int max_obj;		//максимальное кол-во объектов
	unsigned long obj_seek;		//адрес начала объектов в файле
	char obj_name[12];		//имя объекта
	char obj_name_save[12];		//имя объекта для сохранения
	}wad_obj;
long current_obj,hi_window_obj,cur_obj_pos;
int charact;

char ch[]="00";

struct save_doom
	{
	unsigned long data_addr;
	unsigned long data_lenth;
	}save_wad;
//!!!!!!!!!!!!!!!!!!!! WAVE HEADER !!!!!!!!!!!!!!!!!!!!!!!!!!!
struct wave_fmt
	{
	char riff[4];
	long file_8;
	char wave_word[4];
	char fmt0[4];
	long res1;
	int  pcm;
	int  mono;
	int  rate;
	int  res2;
	int  rate2;
	int  res3;
	int  res4;
	int  per_bit;
	char dta[4];
	long clear_sample;
	}wave;
struct data1
	{
	unsigned char dat;
	}data2;

//*************************************************************************
//*                                               			  *
//*			M	A	I	N                         *
//*									  *
//*************************************************************************

main()
{			//  START MAIN PROGRAMM

int command,the_way;

Show_title();		// показать картинку

asm{			// убрать курсор с экрана
   mov ah,1
   mov ch,1
   mov cl,0
   int 0x10
   }

asm{			//Ускоритель клавиатуры
   mov ah,3
   mov al,5
   mov bh,0
   mov bl,0
   int 0x16
   }

//randomize();
clrscr();
window(1,1,80,25);textcolor(3);textbackground(8);clrscr();
for(i=1;i<26;i++)
	{
	printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
	}
window(1,1,80,1);textcolor(0);textbackground(7);clrscr();
	printf("                        WAD EXPLORER 2000   by                          ver %d.%d ",vers1,vers2);
decode();

reread_wad:;			//перечитать каталог

Down_dir_panel();		//нарисовать нижнюю стандартную панель
//help_me();			//помощь
//after_help:;
memory();

draw_cat_win();			//начертить окно каталога
command=Dirsearch("*.wad");			//поиск файлов в каталоге
	//	respon->command :
	//		0 - нет файлов
	//		1 - OK!
	//		2 - недостаточно памяти для чтения всех файлов
memory();

the_way=Dirmove("*.wad",command);
	//	ret->the_way    :
	//		0 - ALT-X (Выход)
	//		1 - ReRead Catalog
if(the_way!=0)goto reread_wad;

Red_panel();
}			//  END MAIN PROGRAMM
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@									  @
//@			   P R O C E D U R E S				  @
//@									  @
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/*-------------------------------------------------------------------------
		      Печать символа
*///-----------------------------------------------------------------------
void Put_to_video_memory(int video_address,char ansi_char,char attr)
{
asm {
  mov ax,0xB800			// ASM RULEZZZZZZZZZZZ!!!
  mov es,ax
  mov di,video_address
  mov al,ansi_char
  mov ah,attr
  stosw
    }
}
/*-------------------------------------------------------------------------
		      Печать моего логотипа!!!
*///-----------------------------------------------------------------------
void decode(void)
{
int xx;
for (i=0;i<=5;i++)
	{
	xx=copyr[i];
	asm{
	xor xx,25
	}
	copyr[i]=xx;
	}
window(49,1,54,1);textcolor(0);textbackground(7);clrscr();printf("%s",copyr);
}
/*-------------------------------------------------------------------------
			Нарисовать нижнюю стандартную панель
*///-----------------------------------------------------------------------
void Down_standart_panel(void)
{
window(1,25,80,25);textcolor(0);textbackground(7);clrscr();
printf(" F1 Help  F2 Save  F3 Refresh  DEL Remove  ENTER Info  ESC Dir              C++");
window(1,25,3,25);textcolor(4);textbackground(7);clrscr();
gotoxy(2,1);printf("F1");
window(10,25,12,25);textcolor(4);textbackground(7);clrscr();
gotoxy(2,1);printf("F2");
window(19,25,21,25);textcolor(4);textbackground(7);clrscr();
gotoxy(2,1);printf("F3");
window(31,25,34,25);textcolor(4);textbackground(7);clrscr();
gotoxy(2,1);printf("DEL");
window(43,25,48,25);textcolor(4);textbackground(7);clrscr();
gotoxy(2,1);printf("ENTER");
window(55,25,58,25);textcolor(4);textbackground(7);clrscr();
gotoxy(2,1);printf("ESC");
}
/*-------------------------------------------------------------------------
			Нарисовать нижнюю панель каталога
*///-----------------------------------------------------------------------
void Down_dir_panel(void)
{
window(1,25,80,25);textcolor(0);textbackground(7);clrscr();
printf(" F1 Help  ENTER Open  Alt-X Quit                                            C++");
window(1,25,3,25);textcolor(4);textbackground(7);clrscr();
gotoxy(2,1);printf("F1");
window(10,25,15,25);textcolor(4);textbackground(7);clrscr();
gotoxy(2,1);printf("ENTER");
window(22,25,27,25);textcolor(4);textbackground(7);clrscr();
gotoxy(2,1);printf("Alt-X");
}
/*-------------------------------------------------------------------------
			Нарисовать нижнюю панель выбора
*///-----------------------------------------------------------------------
void Down_choise_panel(void)
{
window(1,25,80,25);textcolor(0);textbackground(7);clrscr();
printf(" Выбери файл для обновления...                                              C++");
}
/*-------------------------------------------------------------------------
			Нарисовать окно каталога
*///-----------------------------------------------------------------------
void draw_cat_win(void)
{
int i;

window(1,2,25,24);textcolor(15);textbackground(9);clrscr();
printf("╔═══════════════════════╗\n");
for (i=1;i<=21;i++)
	{
	printf("║                       ║\n");
	}
printf("╚═══════════════════════╝\n");
}
/*-------------------------------------------------------------------------
			Удалить окно каталога
*///-----------------------------------------------------------------------
void clr_cat_win(void)
{
int i;

window(1,2,25,24);textcolor(3);textbackground(8);clrscr();
for (i=1;i<=23;i++)
	{
	printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");
	}
}
/*-------------------------------------------------------------------------
			Нарисовать Y/N паналь
*///-----------------------------------------------------------------------
void draw_yn_win(void)
{
	window(23,11,58,13);textcolor(15);textbackground(6);clrscr();
	printf("╔══════════════════════════════════╗\n");
	gotoxy(1,2);
	printf("║                                  ║\n");
	gotoxy(1,3);
	printf("╚══════════════════════════════════╝");
}
/*-------------------------------------------------------------------------
			Удалить Y/N паналь
*///-----------------------------------------------------------------------
void clr_yn_win(void)
{
	window(23,11,58,13);textcolor(3);textbackground(8);clrscr();
	printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");
	gotoxy(1,2);
	printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");
	gotoxy(1,3);
	printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
}
/*-------------------------------------------------------------------------
			Нарисовать окно информации
*///-----------------------------------------------------------------------
void draw_info_win(void)
{
	window(23,8,58,17);textcolor(15);textbackground(10);clrscr();
	printf("╔══════════════════════════════════╗\n");
	gotoxy(1,2);
	printf("║                                  ║\n");
	gotoxy(1,3);
	printf("║                                  ║\n");
	gotoxy(1,4);
	printf("║                                  ║\n");
	gotoxy(1,5);
	printf("║                                  ║\n");
	gotoxy(1,6);
	printf("║                                  ║\n");
	gotoxy(1,7);
	printf("║                                  ║\n");
	gotoxy(1,8);
	printf("║                                  ║\n");
	gotoxy(1,9);
	printf("║                                  ║\n");
	gotoxy(1,10);
	printf("╚══════════════════════════════════╝");
}
/*-------------------------------------------------------------------------
			Удалить окно информации
*///-----------------------------------------------------------------------
void clear_info_win(void)
{
int i;

window(23,8,58,17);textcolor(3);textbackground(8);clrscr();
for (i=1;i<=10;i++)
	{
	gotoxy(1,i);
	printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");
	}
}
/*-------------------------------------------------------------------------
			Очистить "имя рабочего файла"
*///-----------------------------------------------------------------------
void clr_job_file(void)
{
window(28,2,54,2);textcolor(3);textbackground(8);clrscr();
printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
}
/*-------------------------------------------------------------------------
			Free memory
*///-----------------------------------------------------------------------
void memory(void)
{
/*window(1,1,80,25);gotoxy(65,2);
printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
gotoxy(65,2);
printf("Память - %u",coreleft());*/
}
/*-------------------------------------------------------------------------
			Красная панель при выходе
*///-----------------------------------------------------------------------
void Red_panel(void)
{
int addr_pr;
int start_pr;
char space;
char coder1,coder2;
int counter;

window(1,1,80,25);textbackground(0);clrscr();
window(8,4,80-7,25-3);textcolor(14);textbackground(4);clrscr();
//---------------------MAIN TEXT--------
counter=0;
start_pr=4;
for(i=0;i<strlen(idrp_0);i++)
{
space=idrp_0[i];
if(space=='\n')
	{
	start_pr++;
	counter=0;
	i++;
	space=idrp_0[i];
	}
addr_pr=(start_pr*160)+14+counter;
Put_to_video_memory(addr_pr,space,78);
counter=counter+2;
}
//---------------------HEAD TEXT---------
counter=0;
start_pr=4;
for(i=0;i<strlen(idrp_1);i++)
{
space=idrp_1[i];
addr_pr=(start_pr*160)+14+counter;
Put_to_video_memory(addr_pr,space,75);
counter=counter+2;
}
asm{
mov ax,vers1
mov space,al
add space,48
}
Put_to_video_memory(778,space,75);
asm{
mov ax,vers2
mov space,al
add space,48
}
Put_to_video_memory(782,space,75);
//--------------------AUTHOR TEXT--------
counter=0;
start_pr=20;
for(i=0;i<strlen(idrp_2);i++)
{
space=idrp_2[i];
addr_pr=(start_pr*160)+34+counter;
Put_to_video_memory(addr_pr,space,75);
counter=counter+2;
}
counter=74;		//37*2
start_pr=20;
for(i=37;i>0;i--)
{
coder1=coder[i];
coder2=coder[i-1];
asm{
	mov ah,coder1
	mov al,coder2
	xor al,ah
	mov space,al
	}
coder[i-1]=space;
addr_pr=(start_pr*160)+42+counter;
Put_to_video_memory(addr_pr,space,78);
counter=counter-2;
}

}
/*-------------------------------------------------------------------------
			Чтение каталога
*///-----------------------------------------------------------------------
int Dirsearch(char *path)
{
struct find_t{char r[21];char a;unsigned t;unsigned d;long s;char name[13];}ffblk;
int done,flag=0,respon;


num=0;max_files=0;
done=findfirst(path,&ffblk,0);   /*         *.XXX          */
max_files=done;				//Для динамического очищения

window(10,2,15,2);textcolor(15);textbackground(9);
printf(" %s ",path);			// какой тип файлов ищем?

while (!done&&num<MAXFILES)
	{
	flag=1;
	respon=1;

		if((files[num]=malloc(13))==NULL)
			{
			window(3,12,24,12);textcolor(12);textbackground(9);clrscr();
			printf(" Недостаточно памяти\n");
			window(2,13,24,13);textcolor(12);textbackground(9);clrscr();
			printf("для чтения всех файлов!");
			respon=2;
			getch();
			break;
			}
		clearing=1;
		strcpy(files[num],ffblk.name);
	     //printf("%s",files[num]);
		fsize[num]=ffblk.s;
		done=findnext(&ffblk);
		num++;
		max_files=num;
		current_file=0;hi_window_file=0,current_pos=0;


	}
if(flag==0)
	{
	window(2,12,24,12);textcolor(12);textbackground(9);clrscr();
	printf("%s файлы не найдены!",path);
	respon=0;
	}

if(max_files>=1)
	{
	if(max_files==1)
		{
		window(5,24,21,24);textcolor(15);textbackground(9);
		printf("   %d файл   ",max_files);	// сколько файлов в каталоге
		}
	else
		{
		window(5,24,21,24);textcolor(15);textbackground(9);
		printf(" %d файла(ов)",max_files);	// сколько файлов в каталоге
		}
	}
return(respon);
}
/*---------------------------------------------------------------------------
			Хождение по каталогу
*///-------------------------------------------------------------------------
int Dirmove(char *path,int command)
{
int ret;

if(command!=0)Show_cat();	// если нет файлов,каталог не показывать!

character=2;
cont_print:;			//continue printing
while (character!=0)
	{
	character=getch();

	/*if (character==8)        //backspace..
		{
		if (user_count>0)
			{
			user_count--;
			gotoxy(cur_x+user_count,cur_y);
			printf("%c",' ');
			gotoxy(cur_x+user_count,cur_y);
			}
		}*/
	if (character==13)
		{
		if(command!=0)
			{
			strcpy(now_wad_file,files[current_file]);//сохранить имя текущего файла
			}
		if(command!=0)
			{
			for(i=0;i<max_files;i++)free(files[i]);memory();//очистить массив имён файлов
			}

		if(command==0){ret=1;goto quit;}	//v1.1

		clr_cat_win();	//удалмть окно каталога
		ret=Objsearch();
			//	obj_resp->ret :
			//		0 - это не DOOMовский WAD
			//		1 - OK!
			//		2 - недостаточно памяти для чтения всех объектов
			//		3 - файл не найден
		switch(ret)
			{
			 case 0:message(mess_0,ret);ret=1;goto quit;
			 case 2:message(mess_2,ret);
			 case 3:message(mess_3,ret);ret=1;goto quit;
			}
		Objmove();		//работа с объектами
		ret=1;goto quit;
		}
	}

character=getch();	//funk key

//------------------------ Выход в OS ----------------------

if (character==45)	// ALT-X code!
	{
	if(command!=0)
		{
		for(i=0;i<max_files;i++)free(files[i]);memory();//очистить массив имён файлов
		}
	ret=0;
	goto quit;
	}
//------------------------ Курсор вниз ---------------------
if (character==80)	// Down cursor
	{
	if((current_file+1)<max_files)
		{
		//i=current_file-hi_window_file;
		if((current_pos+1)>=21)
			{
			hi_window_file++;current_file++;
			Show_cat();
			goto list_yet;
			}
		// ...на строку вниз
		window(2,current_pos+3,24,current_pos+3);textcolor(7);textbackground(9);clrscr();
		printf(" %s",files[current_file]);
		gotoxy(15,1);printf("%ld",fsize[current_file]);
		// ...удалить курсор
		current_file++;current_pos++;
		window(2,current_pos+3,24,current_pos+3);textcolor(9);textbackground(7);clrscr();
		printf(" %s",files[current_file]);
		gotoxy(15,1);printf("%ld",fsize[current_file]);
		// ...нарисовать курсор в новом месте

		list_yet:;
		}

	}
//------------------------- Курсор вверх -------------------
if (character==72)	// Up cursor
	{
	if((current_file+1)>1)
		{
		if((current_pos+1)==1)
			{
			hi_window_file--;current_file--;
			Show_cat();
			goto list2_yet;
			}
		// ...на строку вверх
		window(2,current_pos+3,24,current_pos+3);textcolor(7);textbackground(9);clrscr();
		printf(" %s",files[current_file]);
		gotoxy(15,1);printf("%ld",fsize[current_file]);
		// ...удалить курсор
		current_file--;current_pos--;
		window(2,current_pos+3,24,current_pos+3);textcolor(9);textbackground(7);clrscr();
		printf(" %s",files[current_file]);
		gotoxy(15,1);printf("%ld",fsize[current_file]);
		// ...нарисовать курсор в новом месте

		list2_yet:;
		}

	}
//------------------------- Помощь -------------------------
if (character==59)	// F1 - Help
	{
	Show_help();

	if(command!=0)
		{
		for(i=0;i<max_files;i++)free(files[i]);memory();//очистить массив имён файлов
		}
	ret=1;
	goto quit;
	}


goto cont_print;	//continue printing
quit:;
return(ret);
}
/*---------------------------------------------------------------------------
			Показать файлы в каталоге
*///-------------------------------------------------------------------------
void Show_cat(void)
{
int i,pos=1;

window(2,3,24,23);textcolor(7);textbackground(9);clrscr();//очистить окно каталога

i=hi_window_file;
while((i<=(max_files-1))&&(pos<=21))
	{
	gotoxy(2,pos);printf("%s",files[i]);
	gotoxy(15,pos);printf("%ld",fsize[i]);
	i++;pos++;
	}
window(2,current_pos+3,24,current_pos+3);textcolor(9);textbackground(7);clrscr();
printf(" %s",files[current_file]);
gotoxy(15,1);printf("%ld",fsize[current_file]);	// нарисовать курсор

}
/*---------------------------------------------------------------------------
			Показать объекты
*///-------------------------------------------------------------------------
void Show_obj(void)
{
long i;
int pos=1,ret;

FILE *f;

//f=fopen(now_wad_file,"rb");//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
if((f=fopen(now_wad_file,"rb"))==NULL){printf("FUCK!!!");}	//открываем файл

window(2,3,14,23);textcolor(14);textbackground(5);clrscr();//очистить окно объектов

i=hi_window_obj;
while((i<=(wad_obj.max_obj-1))&&(pos<=21))
	{
	fseek(f,(wad_obj.obj_seek+(i*16))+8,0);
	fread(&wad_obj.obj_name,8,1,f);		//сколько объектов в файле
	wad_obj.obj_name[9]='\0';
	gotoxy(2,pos);printf("%s",wad_obj.obj_name);
	ret=Type_obj(f,i);
	//	...->ret
	//	0 - Unknown object
	//	1 - Cleared sound object
	//	2 - Sound object
	if(ret!=0)
		{
		if(ret==2){ch[0]=' ';ch[1]='S';}	// Sound object
		if(ret==1){ch[0]='░',ch[1]='S';}
		gotoxy(12,pos);printf("%s",ch);
		}
	i++;pos++;
	}
window(2,cur_obj_pos+3,14,cur_obj_pos+3);textcolor(8);textbackground(7);clrscr();
fseek(f,(wad_obj.obj_seek+(current_obj*16))+8,0);
fread(&wad_obj.obj_name,8,1,f);		//сколько объектов в файле
wad_obj.obj_name[9]='\0';
printf(" %s",wad_obj.obj_name);		//нарисовать курсор
ret=Type_obj(f,current_obj);
	//	...->ret
	//	0 - Unknown object
	//	1 - Cleared sound object
	//	2 - Sound object
print_type_obj(ret,cur_obj_pos);	//печать типа файла

fclose(f);
}
/*---------------------------------------------------------------------------
			Печать типа объекта
*///-------------------------------------------------------------------------
void print_type_obj(int a,int b)
{

if(a!=0)
	{
	if(a==2){ch[0]=' ';ch[1]='S';}	// Sound object
	if(a==1){ch[0]='░',ch[1]='S';}
	window(13,b+3,15,b+3);
	printf("%s",ch);
	}
}
/*---------------------------------------------------------------------------
			Определить тип объекта
*///-------------------------------------------------------------------------
int Type_obj(FILE *f,unsigned long i)
{
int returning=0;
long d;
struct typ_obj  {
		unsigned long data_long;
		unsigned int  data_int;
//		unsigned char data_char;
		unsigned long addr;
		unsigned long length;
		}type_o;

fseek(f,(wad_obj.obj_seek+(i*16)),0);
fread(&type_o.addr,4,1,f);			//адрес
fseek(f,(wad_obj.obj_seek+(i*16)+4),0);
fread(&type_o.length,4,1,f);			//длина

fseek(f,type_o.addr,0);				//начало логического объекта
//-------------------- проверка на звук ------------------
fread(&type_o.data_int,2,1,f);
if(type_o.data_int==3)			//проверка на заголовок
	{
	fseek(f,(type_o.addr+2),0);
	fread(&type_o.data_int,2,1,f);
		if((type_o.data_int==11025)||(type_o.data_int==22050))	//проверка частоты
			{
			returning=2;
			//fseek(f,(type_o.addr+4),0);
			//fread(&type_o.data_long,4,1,f);
			//	if(type_o.data_long==0)returning=1;//проверка на пустоту
			}
	goto ret_ret;
	}
//----------- Clear sound -----------
//fseek(f,type_o.addr,0);				//начало логического объекта

//fread(&type_o.data_int,2,1,f);
if(type_o.data_int==2)			//проверка на заголовок
	{
	fseek(f,(type_o.addr+2),0);
	fread(&type_o.data_int,2,1,f);
		if((type_o.data_int==11025)||(type_o.data_int==22050))	//проверка частоты
			{
			returning=1;
			//fseek(f,(type_o.addr+4),0);
			//fread(&type_o.data_long,4,1,f);
			//	if(type_o.data_long==0)returning=1;//проверка на пустоту
			}
	goto ret_ret;
	}
//---------------------------------------------------------

ret_ret:;
return(returning);
}
/*---------------------------------------------------------------------------
			Поиск объектов в WAD файле
*///-------------------------------------------------------------------------
int Objsearch(void)
{
int i,d=0,obj_resp;
FILE *f;

if((f=fopen(now_wad_file,"rb"))==NULL)
	{
	obj_resp=3;goto to_resp;
	}				//открываем файл

fseek(f,1,0);
for(i=0;i<=2;i++){dest_wad[i]=fgetc(f);}//считываем заголовок
i=0;
while(i<=2)
	{
	if(dest_wad[i]!=source_wad[i])d=1;
	i++;
	}				//сравниваем прочитанное
if(d==1){fclose(f);obj_resp=0;goto to_resp;}//если "SHIT" значит выход...
// ВСЁ OK!
draw_obj_win();				//нарисовать окно для объектов

window(5,2,13,2);textcolor(15);textbackground(5);
printf("Объекты");

fread(&wad_obj.max_obj,2,1,f);		//сколько объектов в файле
fseek(f,8,0); 				//указатель на 000008H
fread(&wad_obj.obj_seek,4,1,f);		//местонахождения объектов в файле

window(3,24,13,24);textcolor(15);textbackground(5);
printf("кол-во %u",wad_obj.max_obj);

fclose(f);
current_obj=0,hi_window_obj=0,cur_obj_pos=0;
obj_resp=1;	//OK!

to_resp:;
return(obj_resp);
}
/*---------------------------------------------------------------------------
			Печать сообщения об ошибке
*///-------------------------------------------------------------------------
void message(char *filename,int s)
{
int i,d;

if(s==0)
	{
	window(1,25,80,25);textcolor(4);textbackground(7);clrscr();
	printf(" %s  %s",now_wad_file,filename);
	getch();
	}
if(s==2)
	{
	window(1,25,80,25);textcolor(4);textbackground(7);clrscr();
	printf(" %s  %s",now_wad_file,filename);
	getch();
	}
if(s==3)
	{
	window(1,25,80,25);textcolor(4);textbackground(7);clrscr();
	printf(" %s  %s",now_wad_file,filename);
	getch();
	}

}
/*-------------------------------------------------------------------------
			Нарисовать окно объектов
*///-----------------------------------------------------------------------
void draw_obj_win(void)
{
int i;

window(1,2,15,24);textcolor(15);textbackground(5);clrscr();
printf("╔═════════════╗\n");
for (i=1;i<=21;i++)
	{
	printf("║             ║\n");
	}
printf("╚═════════════╝\n");

window(28,2,42,2);textcolor(2);textbackground(8);clrscr();
printf("Рабочий файл : ");
window(43,2,54,2);textcolor(10);textbackground(8);clrscr();
printf("%s",now_wad_file);
}
/*---------------------------------------------------------------------------
			Работа с объектами
*///-------------------------------------------------------------------------
void Objmove(void)
{
int ret;
FILE *f;

Down_standart_panel();
Show_obj();

charact=2;
cont_printt:;			//continue printing
while (charact!=0)
	{
	charact=getch();


	if (charact==27)        //возврат к выбору файлов
		{
		goto quiting;
		}
	if (charact==13)	//показать информацию об объекте
		{
		Information();
		}
	}

charact=getch();	//funk key

//------------------------ Выход в OS ----------------------

/*if (charact==45)	// ALT-X code!
	{
	ret=0;
	goto quit;
	}*/
//------------------------ Page Down -----------------------
if (charact==81)	// page down code
	{
	if((current_obj+1)<wad_obj.max_obj)
		{
		if((current_obj+1)+21<=wad_obj.max_obj)
			{
			hi_window_obj=hi_window_obj+21;

			current_obj=current_obj+21;
			Show_obj();
			}
		}
	}
//------------------------ Page Up -------------------------
if (charact==73)	// page up code
	{
	if((current_obj+1)>1)
		{
		if((current_obj+1)-21>=21)
			{
			hi_window_obj=hi_window_obj-21;

			current_obj=current_obj-21;
			Show_obj();
			}
		if((current_obj+1)<=21)
			{
			hi_window_obj=0;
			current_obj=0;
			cur_obj_pos=0;
			Show_obj();
			}
		if(((current_obj+1)>21)&&((current_obj+1)<=42))
			{
			if((hi_window_obj+1)>21)
				{
				hi_window_obj=hi_window_obj-21;
				current_obj=current_obj-21;
				}
			else
				{
				hi_window_obj=0;
				cur_obj_pos=0;
				current_obj=0;
				}
                        Show_obj();
			}
		}
	}
//------------------------ Курсор вниз ---------------------
if (charact==80)	// Down cursor
	{
	if((current_obj+1)<wad_obj.max_obj)
		{
		if((cur_obj_pos+1)>=21)
			{
			hi_window_obj++;current_obj++;
			Show_obj();
			goto llist_yet;
			}
		// ...на строку вниз
		if((f=fopen(now_wad_file,"rb"))==NULL){goto llist_yet;}	//открываем файл

		window(2,cur_obj_pos+3,14,cur_obj_pos+3);textcolor(14);textbackground(5);clrscr();
		fseek(f,(wad_obj.obj_seek+(current_obj*16))+8,0);
		fread(&wad_obj.obj_name,8,1,f);		//сколько объектов в файле
		wad_obj.obj_name[9]='\0';
		printf(" %s",wad_obj.obj_name);		//нарисовать курсор
		ret=Type_obj(f,current_obj);
			//	...->ret
			//	0 - Unknown object
			//	1 - Cleared sound object
			//	2 - Sound object
		print_type_obj(ret,cur_obj_pos);	//печать типа файла
		// ...удалить курсор
		current_obj++;cur_obj_pos++;
		window(2,cur_obj_pos+3,14,cur_obj_pos+3);textcolor(8);textbackground(7);clrscr();
		fseek(f,(wad_obj.obj_seek+(current_obj*16))+8,0);
		fread(&wad_obj.obj_name,8,1,f);		//сколько объектов в файле
		wad_obj.obj_name[9]='\0';
		printf(" %s",wad_obj.obj_name);		//нарисовать курсор
		ret=Type_obj(f,current_obj);
			//	...->ret
			//	0 - Unknown object
			//	1 - Cleared sound object
			//	2 - Sound object
		print_type_obj(ret,cur_obj_pos);	//печать типа файла
		// ...нарисовать курсор в новом месте
		fclose(f);

		llist_yet:;
		}

	}
//------------------------- Курсор вверх -------------------
if (charact==72)	// Up cursor
	{
	if((current_obj+1)>1)
		{
		if((cur_obj_pos+1)==1)
			{
			hi_window_obj--;current_obj--;
			Show_obj();
			goto llist2_yet;
			}
		// ...на строку вверх
		if((f=fopen(now_wad_file,"rb"))==NULL){goto llist2_yet;}	//открываем файл

		window(2,cur_obj_pos+3,14,cur_obj_pos+3);textcolor(14);textbackground(5);clrscr();
		fseek(f,(wad_obj.obj_seek+(current_obj*16))+8,0);
		fread(&wad_obj.obj_name,8,1,f);		//сколько объектов в файле
		wad_obj.obj_name[9]='\0';
		printf(" %s",wad_obj.obj_name);		//нарисовать курсор
		ret=Type_obj(f,current_obj);
			//	...->ret
			//	0 - Unknown object
			//	1 - Cleared sound object
			//	2 - Sound object
		print_type_obj(ret,cur_obj_pos);	//печать типа файла
		// ...удалить курсор
		current_obj--;cur_obj_pos--;
		window(2,cur_obj_pos+3,14,cur_obj_pos+3);textcolor(8);textbackground(7);clrscr();
		fseek(f,(wad_obj.obj_seek+(current_obj*16))+8,0);
		fread(&wad_obj.obj_name,8,1,f);		//сколько объектов в файле
		wad_obj.obj_name[9]='\0';
		printf(" %s",wad_obj.obj_name);		//нарисовать курсор
		ret=Type_obj(f,current_obj);
			//	...->ret
			//	0 - Unknown object
			//	1 - Cleared sound object
			//	2 - Sound object
		print_type_obj(ret,cur_obj_pos);	//печать типа файла
		// ...нарисовать курсор в новом месте
		fclose(f);

		llist2_yet:;
		}

	}
//------------------------- Сохранение ---------------------
if (charact==60)        //сохранение - F2
	{
	Save_object();
	}
//------------------------- Обновление ---------------------
if (charact==61)        //обновление - F3
	{
	Load_object();
	window(5,2,13,2);textcolor(15);textbackground(5);
	printf("Объекты");
	}
//------------------------- Удаление ---------------------
if (charact==83)        //удаление - DELete
	{
	Del_object();
	}
//------------------------- Помощь - F1 -------------------
if (charact==59)        //удаление - DELete
	{
	Show_help();

	Down_standart_panel();
	draw_obj_win();
	window(5,2,13,2);textcolor(15);textbackground(5);
	printf("Объекты");
	window(3,24,13,24);textcolor(15);textbackground(5);
	printf("кол-во %u",wad_obj.max_obj);
	Show_obj();
	}

goto cont_printt;	//continue printing
quiting:;
clr_job_file();
}
/*---------------------------------------------------------------------------
			Сохранить объект
*///-------------------------------------------------------------------------
void Save_object(void)
{
int ret,i;
char cha=0;
long p;
FILE *f,*wav;

if((f=fopen(now_wad_file,"rb"))==NULL){goto list3_yet;}	//открываем файл
fseek(f,(wad_obj.obj_seek+(current_obj*16))+8,0);
fread(&wad_obj.obj_name_save,8,1,f);		//читаем имя

fseek(f,(wad_obj.obj_seek+(current_obj*16)),0);
fread(&save_wad.data_addr,4,1,f);			//адрес
fseek(f,(wad_obj.obj_seek+(current_obj*16)+4),0);
fread(&save_wad.data_lenth,4,1,f);			//длина
ret=Type_obj(f,current_obj);
	//	...->ret
	//	0 - Unknown object
	//	1 - Cleared sound object
	//	2 - Sound object
//-------------- сохранить WAV -----------------
if(ret==2)
	{
	for(i=0;i<=7;i++)
		{
		if(wad_obj.obj_name_save[i]==0)wad_obj.obj_name_save[i]=32;
		}			//корректировка имени файла
	wad_obj.obj_name_save[8]='.';
	wad_obj.obj_name_save[9]='W';
	wad_obj.obj_name_save[10]='A';
	wad_obj.obj_name_save[11]='V';
	wad_obj.obj_name_save[12]='\0';

	draw_yn_win();			// save window
	window(24,12,57,12);textcolor(15);textbackground(6);clrscr();
	printf("  Сохранить %s ?   Y/N  ",wad_obj.obj_name_save);
	while((cha!='Y')&&(cha!='y')&&(cha!='N')&&(cha!='n')&&(cha!=27))
		{
		cha=getch();
		}
	clr_yn_win();			// clr save window

	if((cha=='Y')||(cha=='y'))
		{

		if((wav=fopen(wad_obj.obj_name_save,"wb"))==NULL){goto list3_3_yet;}//открываем файл

		strcpy(wave.riff,"RIFF");
		strcpy(wave.wave_word,"WAVE");
		strcpy(wave.fmt0,"fmt ");
		wave.res1=16;
		wave.pcm=1;
		wave.mono=1;
		 fseek(f,(save_wad.data_addr+2),0);
		 fread(&wave.rate,2,1,f);
		wave.res2=0;
		wave.rate2=wave.rate;
		wave.res3=0;
		wave.res4=1;
		wave.per_bit=8;
		strcpy(wave.dta,"data");
		 fseek(f,(save_wad.data_addr+4),0);
		 fread(&wave.clear_sample,4,1,f);	//BUG in v1.1
	       wave.file_8=wave.clear_sample+36;

		fwrite(&wave,sizeof(wave),1,wav);

		fseek(f,(save_wad.data_addr+8),0);
		//p=wave.clear_sample;
		for(p=1;p<=wave.clear_sample;p++)
			{
			i=fgetc(f);
			fputc(i,wav);

			}

		fclose(wav);
		list3_3_yet:;
		}
	}
//----------------------------------------------

list3_yet:;
fclose(f);
}
/*---------------------------------------------------------------------------
			Удалить содержимое текущего объекта
*///-------------------------------------------------------------------------
void Del_object(void)
{
int ret;
unsigned int  m_obj,m_obj2=0;
unsigned long sun=0,sun2=0;
char cha=0;
unsigned char one_byte;

unsigned long pro1;
unsigned long pro2;
int	      pro3=0;

FILE *f,*temp;

struct copy_temp
	{
	unsigned long data_long;
	unsigned int  data_int;
	}temping;

//if((temping.copy_data=malloc(50000))==NULL)goto del_off;
memory();

if((f=fopen(now_wad_file,"rb"))==NULL){goto del_off2;}	//открываем файл
fseek(f,(wad_obj.obj_seek+(current_obj*16))+8,0);
fread(&wad_obj.obj_name_save,8,1,f);		//читаем имя
wad_obj.obj_name_save[8]='\0';

fseek(f,(wad_obj.obj_seek+(current_obj*16)),0);
fread(&save_wad.data_addr,4,1,f);			//адрес
fseek(f,(wad_obj.obj_seek+(current_obj*16)+4),0);
fread(&save_wad.data_lenth,4,1,f);			//длина
ret=Type_obj(f,current_obj);
	//	...->ret
	//	0 - Unknown object
	//	1 - Cleared sound object
	//	2 - Sound object
//------------ delete sound object ---------
if(ret==2)
	{
	draw_yn_win();				// рисуем Y/N
	window(24,12,57,12);textcolor(15);textbackground(6);clrscr();
	printf("  Удалить объект %s ?  Y/N  ",wad_obj.obj_name_save);
	while((cha!='Y')&&(cha!='y')&&(cha!='N')&&(cha!='n')&&(cha!=27))
		{
		cha=getch();
		}
	//clr_yn_win();
	if((cha=='Y')||(cha=='y'))
		{
		temp=fopen("doomtemp.tmp","wb");
		fseek(f,0,0);
		//sun=(save_wad.data_addr+8)/50000;
	    //printf("***%d***\n",sun);

//-------------- create TEMP file -------------------------

		window(24,12,57,12);textcolor(15);textbackground(6);clrscr();
		printf("   Удаление содержимого ....");
		pro1=((fsize[current_file]-(save_wad.data_lenth-8))/100);
		pro2=0;

		for(sun=1;sun<=save_wad.data_addr+8;sun++)
			{
			one_byte=fgetc(f);
			fputc(one_byte,temp);

			pro2++;
			if(pro2==pro1)
				{
				pro2=0;
				pro3++;
				gotoxy(30,1);printf("%d%",pro3);
				}
			}		// Save first side *.wad

		sun2=fsize[current_file]-(save_wad.data_addr+save_wad.data_lenth);
		//printf("%ld",fsize[current_file]);
		fseek(f,save_wad.data_lenth-8,1);
		for(sun=1;sun<=sun2;sun++)
			{
			one_byte=fgetc(f);
			fputc(one_byte,temp);

			pro2++;
			if(pro2==pro1)
				{
				pro2=0;
				pro3++;
				gotoxy(30,1);printf("%d%",pro3);
				}
			}		// Save second side *.wad
		fclose(temp);

//-------------- record new variables into TEMP file & rename -------------

		temp=fopen("doomtemp.tmp","r+b");
		fseek(temp,8,0);
		// write allocate object table
		temping.data_long=(wad_obj.obj_seek-(save_wad.data_lenth-8));
		wad_obj.obj_seek=temping.data_long; // new variable

		fwrite(&temping.data_long,4,1,temp);

		fseek(temp,(wad_obj.obj_seek+(current_obj*16)),0);
		fread(&save_wad.data_addr,4,1,temp);		//адрес
		fseek(temp,(wad_obj.obj_seek+(current_obj*16)+4),0);
		fread(&save_wad.data_lenth,4,1,temp);		//длина

//-------------- fixing header of the object -----------------

		fseek(temp,save_wad.data_addr,0); // fixing head object
		temping.data_int=2;
		fwrite(&temping.data_int,2,1,temp);

		fseek(temp,save_wad.data_addr+4,0); // fixing head object
		temping.data_long=0;
		fwrite(&temping.data_long,4,1,temp);

//-------------- fixing Object Allocation Table --------------
//printf(" %ld",save_wad.data_lenth-8);
		m_obj=wad_obj.max_obj;
		while(m_obj>0)
			{
			fseek(temp,(wad_obj.obj_seek+(m_obj2*16)),0);
			fread(&temping.data_long,4,1,temp);

				if(temping.data_long>save_wad.data_addr)
					{
					temping.data_long=temping.data_long-(save_wad.data_lenth-8);
				       fseek(temp,(wad_obj.obj_seek+(m_obj2*16)),0);
					fwrite(&temping.data_long,4,1,temp);
					}
			m_obj2++;
			m_obj--;
			}
		temping.data_long=8;
		fseek(temp,(wad_obj.obj_seek+(current_obj*16)+4),0);
		fwrite(&temping.data_long,4,1,temp);	//fixing lenth in OAT

		fclose(temp);

		fsize[current_file]=(fsize[current_file]-(save_wad.data_lenth-8));
		//printf(" %ld",fsize[current_file]); // изменение длины файла

		fclose(f);
		remove(now_wad_file);
		rename("doomtemp.tmp",now_wad_file);
		Show_obj();
		f=fopen(now_wad_file,"rb");
		}
	clr_yn_win();
	}

fclose(f);

del_off2:;
//free(temping.copy_data);
memory();
del_off:;
}
/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!      L   O   A   D      !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/
/*-------------------------------------------------------------------------
			Чтение каталога загружаемых файлов
*///-----------------------------------------------------------------------
int Extsearch(char *path)
{
struct find_t{char r[21];char a;unsigned t;unsigned d;long s;char name[13];}ffblk;
int done,flag=0,respon;


num=0;max_files=0;
done=findfirst(path,&ffblk,0);   /*         *.XXX          */
max_files=done;				//Для динамического очищения

window(10,2,15,2);textcolor(15);textbackground(9);
printf(" %s ",path);			// какой тип файлов ищем?

while (!done&&num<MAXFILES)
	{
	flag=1;
	respon=1;

		if((ext_files[num]=malloc(13))==NULL)
			{
			window(3,12,24,12);textcolor(12);textbackground(9);clrscr();
			printf(" Недостаточно памяти\n");
			window(2,13,24,13);textcolor(12);textbackground(9);clrscr();
			printf("для чтения всех файлов!");
			respon=2;
			getch();
			break;
			}
		clearing=1;
		strcpy(ext_files[num],ffblk.name);
	     //printf("%s",files[num]);
		ext_fsize[num]=ffblk.s;
		done=findnext(&ffblk);
		num++;
		max_files=num;
		current_ext_file=0;hi_window_ext_file=0,current_ext_pos=0;


	}
if(flag==0)
	{
	window(2,12,24,12);textcolor(12);textbackground(9);clrscr();
	printf("%s файлы не найдены!",path);
	respon=0;
	}
if(max_files>=1)
	{
	if(max_files==1)
		{
		window(5,24,21,24);textcolor(15);textbackground(9);
		printf("   %d файл   ",max_files);	// сколько файлов в каталоге
		}
	else
		{
		window(5,24,21,24);textcolor(15);textbackground(9);
		printf(" %d файла(ов)",max_files);	// сколько файлов в каталоге
		}
	}
return(respon);
}
/*---------------------------------------------------------------------------
			Хождение по каталогу с загружаемыми файлами
*///-------------------------------------------------------------------------
int Extmove(char *path,int command)
{
int ret;

if(command!=0)Show_ext();	// если нет файлов,каталог не показывать!

character=2;
cont_ext:;			//continue printing
while (character!=0)
	{
	character=getch();

	if(command==0)character=27;	//v1.1

	if (character==27)        //Escape
		{
		if(command!=0)
			{
			for(i=0;i<max_files;i++)free(ext_files[i]);memory();//очистить массив имён файлов
			}
		clr_cat_win();	//удалмть окно каталога
		ret=0;
		goto ext_quit;
		}


	if (character==13)
		{
		strcpy(now_ext_file,ext_files[current_ext_file]);//сохранить имя текущего файла
		if(command!=0)
			{
			for(i=0;i<max_files;i++)free(ext_files[i]);memory();//очистить массив имён файлов
			}
		clr_cat_win();	//удалмть окно каталога
		//ret=Objsearch();
			//	obj_resp->ret :
			//		0 - это не DOOMовский WAD
			//		1 - OK!
			//		2 - недостаточно памяти для чтения всех объектов
			//		3 - файл не найден
/*		switch(ret)
			{
			 case 0:message(mess_0,ret);ret=1;goto ext_quit;
			 case 2:message(mess_2,ret);
			 case 3:message(mess_3,ret);ret=1;goto ext_quit;
			}*/
		ret=1;goto ext_quit;
		}
	}

character=getch();	//funk key

//------------------------ Выход в OS ----------------------

/*if (character==45)	// ALT-X code!
	{
	ret=0;
	goto ext_quit;
	}*/
//------------------------ Курсор вниз ---------------------
if (character==80)	// Down cursor
	{
	if((current_ext_file+1)<max_files)
		{
		//i=current_file-hi_window_file;
		if((current_ext_pos+1)>=21)
			{
			hi_window_ext_file++;current_ext_file++;
			Show_ext();
			goto list_yet;
			}
		// ...на строку вниз
		window(2,current_ext_pos+3,24,current_ext_pos+3);textcolor(7);textbackground(9);clrscr();
		printf(" %s",ext_files[current_ext_file]);
		gotoxy(15,1);printf("%ld",ext_fsize[current_ext_file]);
		// ...удалить курсор
		current_ext_file++;current_ext_pos++;
		window(2,current_ext_pos+3,24,current_ext_pos+3);textcolor(9);textbackground(7);clrscr();
		printf(" %s",ext_files[current_ext_file]);
		gotoxy(15,1);printf("%ld",ext_fsize[current_ext_file]);
		// ...нарисовать курсор в новом месте

		list_yet:;
		}

	}
//------------------------- Курсор вверх -------------------
if (character==72)	// Up cursor
	{
	if((current_ext_file+1)>1)
		{
		if((current_ext_pos+1)==1)
			{
			hi_window_ext_file--;current_ext_file--;
			Show_ext();
			goto list2_yet;
			}
		// ...на строку вверх
		window(2,current_ext_pos+3,24,current_ext_pos+3);textcolor(7);textbackground(9);clrscr();
		printf(" %s",ext_files[current_ext_file]);
		gotoxy(15,1);printf("%ld",ext_fsize[current_ext_file]);
		// ...удалить курсор
		current_ext_file--;current_ext_pos--;
		window(2,current_ext_pos+3,24,current_ext_pos+3);textcolor(9);textbackground(7);clrscr();
		printf(" %s",ext_files[current_ext_file]);
		gotoxy(15,1);printf("%ld",ext_fsize[current_ext_file]);
		// ...нарисовать курсор в новом месте

		list2_yet:;
		}

	}

goto cont_ext;	//continue printing
ext_quit:;
return(ret);
}
/*---------------------------------------------------------------------------
			Показать загружаемые файлы в каталоге
*///-------------------------------------------------------------------------
void Show_ext(void)
{
int i,pos=1;

window(2,3,24,23);textcolor(7);textbackground(9);clrscr();//очистить окно каталога

i=hi_window_ext_file;
while((i<=(max_files-1))&&(pos<=21))
	{
	gotoxy(2,pos);printf("%s",ext_files[i]);
	gotoxy(15,pos);printf("%ld",ext_fsize[i]);
	i++;pos++;
	}
window(2,current_ext_pos+3,24,current_ext_pos+3);textcolor(9);textbackground(7);clrscr();
printf(" %s",ext_files[current_ext_file]);
gotoxy(15,1);printf("%ld",ext_fsize[current_ext_file]);	// нарисовать курсор

}
/*---------------------------------------------------------------------------
-----------------------------------------------------------------------------
			Обновить содержимое текущего объекта
-----------------------------------------------------------------------------
*///-------------------------------------------------------------------------
void Load_object(void)
{
int ret,command,the_way,d,w;
unsigned int  m_obj,m_obj2=0;
unsigned long sun=0,sun2=0;
int version;		// 1-обычный WAV        2-нестандартный WAV
unsigned char one_byte;
char cha=0;

char anal_riff[]="RIFF";
char anal_wave[]="WAVE";
char anal_data[]="data";

unsigned long pro1;
unsigned long pro2;
int	      pro3=0;

FILE *f,*temp,*ext;

struct copy_temp
	{
	unsigned long data_long;
	unsigned int  data_int;
	}temping;

if((f=fopen(now_wad_file,"rb"))==NULL){goto get_off;}	//открываем файл
ret=Type_obj(f,current_obj);
	//	...->ret
	//	0 - Unknown object
	//	1 - Cleared sound object
	//	2 - Sound object
fclose(f);
if((ret==2)||(ret==1))		// звуковой объект..приступить к обновлению
	{
	Down_choise_panel();
	draw_cat_win();			//начертить окно каталога
	command=Extsearch("*.wav");			//поиск файлов в каталоге
		//	respon->command :
		//		0 - нет файлов
		//		1 - OK!
		//		2 - недостаточно памяти для чтения всех файлов
	memory();

	the_way=Extmove("*.wav",command);
		//	ret->the_way    :
		//		0 - Break
		//		1 - Continue
	memory();
	if(the_way==0)		// Выход в каталог объектов по желанию пользователя
		{
		Down_standart_panel();
		draw_obj_win();
		window(3,24,13,24);textcolor(15);textbackground(5);
		printf("кол-во %u",wad_obj.max_obj);
		Show_obj();
		goto get_off;
		}
//OPEN WAV
	if((ext=fopen(now_ext_file,"rb"))==NULL){goto get_off;}	//открываем файл
	if(ext_fsize[current_ext_file]<44)
		{
		window(1,25,80,25);textcolor(4);textbackground(7);clrscr();
		printf(" %s  не звуковой файл!",now_ext_file);
		clr_cat_win();	//удалмть окно каталога
		getch();
		fclose(ext);
		window(3,24,13,24);textcolor(15);textbackground(5);
		printf("кол-во %u",wad_obj.max_obj);
		Show_obj();
		Down_standart_panel();
		goto get_off;
		}

	fread(&wave,44,1,ext);
//------ анализ WAV заголовка --------------------------------------
	version=1;
	i=0;d=0;w=0;
	while(i<=3)	//RIFF?
		{
		if(anal_riff[i]!=wave.riff[i])d=1;
		i++;
		}
	i=0;
	while(i<=3)	//WAVE?
		{
		if(anal_wave[i]!=wave.wave_word[i])d=1;
		i++;
		}
	i=0;
	while(i<=3)	//data?
		{
		if(anal_data[i]!=wave.dta[i])w=1;
		i++;
		}
	if((d==0)&&(w==1))	// другая версия WAVа?
		{
		fseek(ext,50,0);
		fread(&wave.dta,4,1,ext);
		i=0;
		while(i<=3)	//data?
			{
			if(anal_data[i]!=wave.dta[i])d=1;
			i++;
			}
		if(d==0)version=2;
		}


	if(d==1)	// Не WAVE файл!!!
		{
		window(1,25,80,25);textcolor(4);textbackground(7);clrscr();
		printf(" %s  не звуковой файл!",now_ext_file);
		clr_cat_win();	//удалмть окно каталога
		getch();
		fclose(ext);
		draw_obj_win();
		window(3,24,13,24);textcolor(15);textbackground(5);
		printf("кол-во %u",wad_obj.max_obj);
		Show_obj();
		Down_standart_panel();
		goto get_off;
		}
//------ анализ данных в WAV заголовке -----------------------------
	if(version==2)
		{
		fseek(ext,54,0);
		fread(&wave.clear_sample,4,1,ext);
		}
	if(wave.mono!=1)
		{
		window(1,25,80,25);textcolor(4);textbackground(7);clrscr();
		printf(" WAV должен иметь только один канал (MONO)!");
		clr_cat_win();	//удалмть окно каталога
		getch();
		fclose(ext);
		draw_obj_win();
		window(3,24,13,24);textcolor(15);textbackground(5);
		printf("кол-во %u",wad_obj.max_obj);
		Show_obj();
		Down_standart_panel();
		goto get_off;
		}
	if((wave.rate!=11025)&&(wave.rate!=22050))
		{
		window(1,25,80,25);textcolor(4);textbackground(7);clrscr();
		printf(" Частота сэмпла должна быть 11025 или 22050 Герц!");
		clr_cat_win();	//удалмть окно каталога
		getch();
		fclose(ext);
		draw_obj_win();
		window(3,24,13,24);textcolor(15);textbackground(5);
		printf("кол-во %u",wad_obj.max_obj);
		Show_obj();
		Down_standart_panel();
		goto get_off;
		}
	if(wave.per_bit!=8)
		{
		window(1,25,80,25);textcolor(4);textbackground(7);clrscr();
		printf(" Допустимы только 8-ми битовые сэмплы!");
		clr_cat_win();	//удалмть окно каталога
		getch();
		fclose(ext);
		draw_obj_win();
		window(3,24,13,24);textcolor(15);textbackground(5);
		printf("кол-во %u",wad_obj.max_obj);
		Show_obj();
		Down_standart_panel();
		goto get_off;
		}
	if(wave.pcm!=1)
		{
		window(1,25,80,25);textcolor(4);textbackground(7);clrscr();
		printf(" Не PCM!");
		clr_cat_win();	//удалмть окно каталога
		getch();
		fclose(ext);
		draw_obj_win();
		window(3,24,13,24);textcolor(15);textbackground(5);
		printf("кол-во %u",wad_obj.max_obj);
		Show_obj();
		Down_standart_panel();
		goto get_off;
		}

//------ панель с информацией о структуре файла --------------------
	draw_info_win();
	window(24,9,57,16);textcolor(15);textbackground(10);clrscr();
	gotoxy(2,1);printf("Имя файла    :");
	gotoxy(2,2);printf("Длина файла  :");
	gotoxy(2,3);printf("Длина данных :");
	gotoxy(2,4);printf("Каналы       :");
	gotoxy(2,5);printf("Частота      :");
	gotoxy(2,6);printf("Разрядность  :");

	window(39,9,57,16);textcolor(14);textbackground(10);clrscr();
	gotoxy(1,1);printf(" %s",now_ext_file);
	gotoxy(1,2);printf(" %ld",ext_fsize[current_ext_file]);
	gotoxy(1,3);printf(" %ld",wave.clear_sample);
	gotoxy(1,4);printf(" Моно");
	gotoxy(1,5);printf(" %d Герц",wave.rate);
	gotoxy(1,6);printf(" %d Бит",wave.per_bit);

	window(24,15,57,15);textcolor(1);textbackground(10);clrscr();
	printf(" Поместить новые данные в объект?");
	window(24,16,57,16);textcolor(1);textbackground(10);clrscr();
	printf("               Y\\N");
	while((cha!='Y')&&(cha!='y')&&(cha!='N')&&(cha!='n')&&(cha!=27))
		{
		cha=getch();
		}
	if((cha=='Y')||(cha=='y'))
		{
                Down_standart_panel();
//------ start loading new data into WAD file (TEMP) ---------------
		f=fopen(now_wad_file,"rb");
		temp=fopen("doomtemp.tmp","wb");

		fseek(f,(wad_obj.obj_seek+(current_obj*16)),0);
		fread(&save_wad.data_addr,4,1,f);			//адрес
		fseek(f,(wad_obj.obj_seek+(current_obj*16)+4),0);
		fread(&save_wad.data_lenth,4,1,f);			//длина
//-------------- create TEMP file ----------------------------------
		clear_info_win();
		draw_obj_win();
		window(5,2,13,2);textcolor(15);textbackground(5);
		printf("Объекты");
		window(3,24,13,24);textcolor(15);textbackground(5);
		printf("кол-во %u",wad_obj.max_obj);
		Show_obj();
		draw_yn_win();
		window(24,12,57,12);textcolor(15);textbackground(6);clrscr();
		printf("  Обновление содержимого ...");
		pro1=((fsize[current_file]-(save_wad.data_lenth-8))/100);
		pro2=0;

		fseek(f,0,0);
		for(sun=1;sun<=save_wad.data_addr+8;sun++)
			{
			one_byte=fgetc(f);
			fputc(one_byte,temp);

			pro2++;
			if(pro2==pro1)
				{
				pro2=0;
				pro3++;
				gotoxy(30,1);printf("%d%",pro3);
				}
			}	// Save first side *.wad
//------->
		if(version==1)
			{
			fseek(ext,44,0);
			}
			else
			{
			fseek(ext,58,0);
			}
		for(sun=1;sun<=wave.clear_sample;sun++)	//закачка новых данных
			{
			one_byte=fgetc(ext);
			fputc(one_byte,temp);
			}
//------->
		sun2=fsize[current_file]-(save_wad.data_addr+save_wad.data_lenth);
		fseek(f,save_wad.data_lenth-8,1);
		for(sun=1;sun<=sun2;sun++)
			{
			one_byte=fgetc(f);
			fputc(one_byte,temp);

			pro2++;
			if(pro2==pro1)
				{
				pro2=0;
				pro3++;
				gotoxy(30,1);printf("%d%",pro3);
				}
			}	// Save second side *.wad
		fclose(temp);

//-------------- record new variables into TEMP file & rename -------------

		temp=fopen("doomtemp.tmp","r+b");
		fseek(temp,8,0);
		// write allocate object table
		if(ret==1)
			{
			temping.data_long=(wad_obj.obj_seek+wave.clear_sample);
			wad_obj.obj_seek=temping.data_long; // new variable
			}
		if(ret==2)
			{
			temping.data_long=(wad_obj.obj_seek-(save_wad.data_lenth-8));
			wad_obj.obj_seek=(temping.data_long+wave.clear_sample);
			temping.data_long=wad_obj.obj_seek;
			}
		fwrite(&temping.data_long,4,1,temp);

		fseek(temp,(wad_obj.obj_seek+(current_obj*16)),0);
		fread(&save_wad.data_addr,4,1,temp);		//адрес
		fseek(temp,(wad_obj.obj_seek+(current_obj*16)+4),0);
		fread(&save_wad.data_lenth,4,1,temp);		//длина

//-------------- fixing header of the object -----------------

		fseek(temp,save_wad.data_addr,0); // fixing head object
		temping.data_int=3;
		fwrite(&temping.data_int,2,1,temp);

		fseek(temp,save_wad.data_addr+4,0); // fixing head object
		fwrite(&wave.clear_sample,4,1,temp);

		fseek(temp,save_wad.data_addr+2,0); // fixing head object
		fwrite(&wave.rate,2,1,temp);

//-------------- fixing Object Allocation Table --------------

		m_obj=wad_obj.max_obj;
		while(m_obj>0)
			{
			fseek(temp,(wad_obj.obj_seek+(m_obj2*16)),0);
			fread(&temping.data_long,4,1,temp);

				if(temping.data_long>save_wad.data_addr)
					{
					if(ret==1)
						{
						temping.data_long=(temping.data_long+wave.clear_sample);
						//wad_obj.obj_seek=temping.data_long; // new variable
						}
					if(ret==2)
						{
						temping.data_long=temping.data_long-(save_wad.data_lenth-8);
						temping.data_long=(temping.data_long+wave.clear_sample);
						}
				       fseek(temp,(wad_obj.obj_seek+(m_obj2*16)),0);
					fwrite(&temping.data_long,4,1,temp);
					}
			m_obj2++;
			m_obj--;
			}
		temping.data_long=wave.clear_sample+8;
		fseek(temp,(wad_obj.obj_seek+(current_obj*16)+4),0);
		fwrite(&temping.data_long,4,1,temp);	//fixing lenth in OAT
		if(ret==1)
			{
			fsize[current_file]=fsize[current_file]+wave.clear_sample;
			}
		if(ret==2)
			{
			temping.data_long=(fsize[current_file]-(save_wad.data_lenth-8));
			fsize[current_file]=temping.data_long+wave.clear_sample;
			}


		fclose(temp);
		fclose(f);

		remove(now_wad_file);
		rename("doomtemp.tmp",now_wad_file);
		}

	fclose(ext);	// закрыть WAV file
	clear_info_win();
	Down_standart_panel();
	draw_obj_win();
	window(3,24,13,24);textcolor(15);textbackground(5);
	printf("кол-во %u",wad_obj.max_obj);
	Show_obj();
	}		// конец процедуры обновления звукового объекта

get_off:;
}
/*-------------------------------------------------------------------------
		    Показать информацию по текущему объекту
*///-----------------------------------------------------------------------
void Information(void)
{
int ret,i;
char cha=0;
long p;
FILE *f;

if((f=fopen(now_wad_file,"rb"))==NULL){goto list5_yet;}	//открываем файл
fseek(f,(wad_obj.obj_seek+(current_obj*16))+8,0);
fread(&wad_obj.obj_name_save,8,1,f);		//читаем имя

fseek(f,(wad_obj.obj_seek+(current_obj*16)),0);
fread(&save_wad.data_addr,4,1,f);			//адрес
fseek(f,(wad_obj.obj_seek+(current_obj*16)+4),0);
fread(&save_wad.data_lenth,4,1,f);			//длина
ret=Type_obj(f,current_obj);
	//	...->ret
	//	0 - Unknown object
	//	1 - Cleared sound object
	//	2 - Sound object
if(ret==0)			// неизвестный объект
	{
	draw_info_win();
	window(24,9,57,16);textcolor(15);textbackground(10);clrscr();
	gotoxy(2,1);printf("Имя объекта  :");
	gotoxy(2,2);printf("Тип объекта  :");

	window(39,9,57,16);textcolor(14);textbackground(10);clrscr();
	gotoxy(1,1);printf(" %s",wad_obj.obj_name);

	gotoxy(1,2);printf(" Неизвестный");

	window(24,16,57,16);textcolor(1);textbackground(10);clrscr();
	printf("   Нажми  ESC , ENTER или SPACE");
	while((cha!=13)&&(cha!=32)&&(cha!=27))
		{
		cha=getch();
		}
	clear_info_win();
	}

if((ret==2)||(ret==1))		// звуковой объект..вывести информацию
	{
	draw_info_win();
	window(24,9,57,16);textcolor(15);textbackground(10);clrscr();
	gotoxy(2,1);printf("Имя объекта  :");
	gotoxy(2,2);printf("Тип объекта  :");
	gotoxy(2,3);printf("Состояние    :");
	gotoxy(2,4);printf("Длина данных :");
	gotoxy(2,5);printf("Частота      :");
	gotoxy(2,6);printf("Разрядность  :");

	window(39,9,57,16);textcolor(14);textbackground(10);clrscr();
	gotoxy(1,1);printf(" %s",wad_obj.obj_name);

	gotoxy(1,2);printf(" Звуковой");

	if(ret==2)
		{
		gotoxy(1,3);printf(" Активный");
		}
	else
		{
		gotoxy(1,3);printf(" Без данных");
		}

	if(ret==2)
		{
		gotoxy(1,4);printf(" %ld Байт",save_wad.data_lenth-8);
		}

	if(ret==2)
		{
		fseek(f,(save_wad.data_addr+2),0);
		fread(&wave.rate,2,1,f);
		gotoxy(1,5);printf(" %d Герц",wave.rate);
		}
	if(ret==2)
		{
		gotoxy(1,6);printf(" 8 Бит");
		}
	window(24,16,57,16);textcolor(1);textbackground(10);clrscr();
	printf("   Нажми  ESC , ENTER или SPACE");
	while((cha!=13)&&(cha!=32)&&(cha!=27))
		{
		cha=getch();
		}
	clear_info_win();
	}

fclose(f);
list5_yet:;
}
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			      SHOW TITLE IMAGE

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
void Show_title(void)
{
unsigned int i,int_color;
unsigned char one_byte,color,r,g,b;

asm{
xor ax,ax
mov al,0x13
int 0x10			//VGA ON!
   }
//-----------Flash off palette--
for(int_color=0;int_color<=255;int_color++)
	{
	color=int_color;
	r=0;g=0;b=0;
	asm{
	   mov dx,0x3c8
	   mov al,color
	   out dx,al
	   inc dx
	   mov al,r
	   out dx,al
	   mov al,g
	   out dx,al
	   mov al,b
	   out dx,al
	   }
	}
//-----------Draw image---------
for(i=0;i<64000;i++)
	{
	one_byte=image[(i+1078)+3];
	asm{
	   mov ax,0xa000
	   mov es,ax
	   mov al,one_byte
	   mov di,i
	   stosb		//нарисовать сам рисунок
	   }
	}
//-----------Set palette--------
i=54+3;
for(int_color=0;int_color<=255;int_color++)
	{
	color=int_color;
	b=image[i];i++;
	g=image[i];i++;
	r=image[i];i=i+2;
	//setrgbpalette(color,r,g,b);
	asm{
	   mov dx,0x3c8
	   mov al,color
	   out dx,al
	   inc dx
	   mov al,r
	   shr al,2
	   out dx,al
	   mov al,g
	   shr al,2
	   out dx,al
	   mov al,b
	   shr al,2
	   out dx,al
	   }
	}

getch();			//read any key to continue

asm{
xor ax,ax
mov al,3
int 0x10			//VGA OFF!  (EGA ON!)
   }
}
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

				SHOW HELP

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
void Show_help(void)
{
int counter,addr_pr;
char space;

counter=0;
for(i=0;i<strlen(we2000_help);i++)
	{
	addr_pr=160+counter;
	Put_to_video_memory(addr_pr,we2000_help[i],31);
	counter=counter+2;
	}
window(1,25,80,25);textcolor(0);textbackground(7);clrscr();
printf(" Для возвращения нажмите любую клавишу...");
getch();

window(1,2,80,24);textcolor(3);textbackground(8);clrscr();
for(i=1;i<24;i++)
	{
	printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
	}

}