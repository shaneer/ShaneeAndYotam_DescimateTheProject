#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "C:\Users\shanee\Documents\GitHub\ShaneeAndYotam_DescimateTheProject\SPConfig.h"
#include "C:\Users\shanee\Documents\GitHub\ShaneeAndYotam_DescimateTheProject\SPConfig.c"


int main(void) {
	bool randomTests = 0;
	if (randomTests){
		char* a = "\t \vfirst me\nhello there\n";
		char* newline = "     \n";
		char* comment = "	#whatever";

		while(*a == ' '||*a=='\t'||*a=='\v'){
			a++;
		}
		while(*newline == ' '||*newline=='\t'||*newline=='\v'){
				newline++;
			}
		while(*comment == ' '||*comment=='\t'||*comment=='\v'){
					comment++;
				}
		printf("a:\n%s" , a);

		printf("b:\n%s" , newline);
		if (!*newline=='\n'){
			printf("b:\n%s" , newline);
		}else printf("b : recognized newline\n");

		printf("c:\n%s" , comment);
			if (!comment[0]=='#'){
				printf("c:\n%s" , comment);
			}else printf("\nc : recognized starts with #\n");

		printf("\nlength of line = %d", strlen("spKNN=1\n"));

		char* temp = "\n01234=0\n";
		char* v = strchr(temp , '=');

		puts(temp);
		v++;
		puts(v);
		printf("length of value ('0' + \\n) should be 2 and its: %d", strlen(v));

		char* mallocCheckSize;
		mallocCheckSize = (char*)malloc(100);
		mallocCheckSize = "\nI am 88 long!!!";
		puts(mallocCheckSize);
		printf("\nSize is: %d,  length is: %d", sizeof(mallocCheckSize), strlen(mallocCheckSize));

		if (memcmp(mallocCheckSize, "\nI am ", 4)==0){
			puts("\nsuccess");
		}

		char* pre= "spImagesPrefix";
		int preNUM = pre;
		char* suf = "spImagesSuffix";
		int sufNUM = suf;
		char* NOI = "spNumOfImages";
		int noiNUM = NOI;
		char* DIM = "spCADimension";
		int dimNUM = DIM;
		char* FileName = "spCAFilename";
		int fnNUM = FileName;

		printf("\n%d",preNUM);
		printf("\n%d",sufNUM);
		printf("\n%d",noiNUM);
		printf("\n%d",dimNUM);
		printf("\n%d",fnNUM);

		const int casty = (int)"spImagesPrefix";
		if (preNUM == (int)"spImagesPrefix"){
			puts("\nThey're equal anddd");
			printf("%d",casty);
		}


		switch (preNUM){
			case 2:
				puts(1);
				break;
		}

		char *temp1 = "goodbye NO";
		char* paramName = (char*) malloc(strlen(temp));
		int i = 0;
		char curr = temp1[0];
		while (!(curr == ' '||curr == '\n'||curr == '\t'||curr == '\v'||curr=='=')){
			paramName[i]=curr;
			curr=temp1[i+1];
			i++;
		}
		puts("\n");
		puts(paramName);
		free(paramName);
		puts(paramName);

		int twenty = 23456789;
		int intSize = sizeof(twenty);
		printf("%d" , intSize);

		char* printHere;
		printHere = (char*) malloc(10);
		snprintf(printHere, 10, "\n%d", twenty);
		puts(printHere);

		char* imagePath;
		char* str2 = "I am the second";
		char* str3 = "I am the third";
		int size = sizeof(str2)+sizeof(str3)+2;
		imagePath = (char*) malloc(sizeof(char)*size);
		imagePath = memcpy(imagePath, str2, strlen(str2)+1);
		imagePath = strncat(imagePath, str3, strlen(str3)+1);
		puts(imagePath);

		int A = atoi("020");
		int B = atoi("20abc");
		int C = atoi("2b0a");
		int D = atoi("2005a3");
		int E = atoi("520a");
		if (isdigit(' '))printf("\nA = %d B = %d C = %d D = %d E = %d", A, B, C, D, E);


		int maximal = INT_MAX;
		printf("\nnum = %d", maximal);


		char* tempStr = (char*) malloc(35);
		char* value = (char*) malloc(20);
		char* paramName2 = (char*) malloc(35);

		strcpy(tempStr, "     Imagepath =    filepathname \n");
		value = strchr(tempStr, '=');
		++value;
		while ((*tempStr == ' ')||(*tempStr == '\t')||(*tempStr == '\v')){
				++tempStr;
			}
		while ((*value == ' ')||(*value == '\t')||(*value == '\v')){
			++value;
		}
		int end = strlen(value)-1;
		while (isspace(value[end])||(value[end] == '\n')){
			printf("/%c",value[end]);
			--end;
		}
		value[end] = '\0';
		char* change;
		change = (char*) malloc(20);

		printf("\n%s length is: %d", value, strlen(value));
		paramName2 = (char*) realloc(paramName2, strlen(tempStr)-strlen(value));


		int i2=0;
		while (!(isspace(tempStr[i2])||tempStr[i2]=='=')){
			paramName2[i2]=tempStr[i2];
			i2++;
		}
		paramName2[i2]='\0';

		if (strcmp(paramName2, "Imagepath")==0) {
			printf("\nSuccess! param = %s", paramName2);
		}else printf("\nnope param is: %s", paramName2);

		enum ENUMTRIAL{ one, two, three };

		enum ENUMTRIAL enum1 = three;
		if(enum1 == 2){
		printf("\nyes");
		}
	}

	puts("START");

	char* ex1 = "C:\\Users\\shanee\\Desktop\\configEX1.txt";
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*) malloc(sizeof(msg));
	spConfigCreate(ex1,  msg);

	printf("\n%s", msg);

return 0;
}
