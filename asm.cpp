#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "image_processing.cpp"

using namespace std;

void Dilation(int n, int filter_size, short* image_data_original);
void Erosion(int n, int filter_size, short* image_data_original);

int main(void) {

	int width, height, max_value, i, j, filter_size;
	bool image_type;
	int file_status;
	char original_image_path[100], dilated_image_path[] = "dilated.pgm", eroded_image_path[] = "eroded.pgm";

	do {
		printf("Enter the path of the original image:\n-> ");
		scanf("%s", original_image_path);
		system("CLS");
		file_status = readImageHeader(original_image_path, width, height, max_value, image_type);
	} while (file_status > 1);

	int** original_image = resimOku(original_image_path);

	printf("Original Image Path: \t\t\t%s\n", original_image_path);

	short *original_image_array = (short*) malloc(width * height * sizeof(short));

	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
			original_image_array[i * width + j] = (short)original_image[i][j];

	int menu_choice;

	printf("Select the operation you want to perform...\n");
	printf("1-) Dilation\n");
	printf("2-) Erosion\n");
	printf("3-) Exit\n> ");
	scanf("%d", &menu_choice);

	printf("Enter the filter size: ");
	scanf("%d", &filter_size);

	switch (menu_choice) {
		case 1:
			Dilation(width * height, filter_size, original_image_array);
			resimYaz(dilated_image_path, original_image_array, width, height, max_value);
			break;
		case 2:
			Erosion(width * height, filter_size, original_image_array);
			resimYaz(eroded_image_path, original_image_array, width, height, max_value);
			break;
		case 3:
			system("EXIT");
			break;
		default:
			system("EXIT");
	}

	system("PAUSE");
	return 0;
}

void Dilation(int n, int filter_size, short* image_data_original) {

	short* image_copy = (short*)malloc(n * sizeof(short));
	int i;
	for (i = 0; i < n; i++)
		image_copy[i] = image_data_original[i];

	__asm {
		MOV EDI,image_data_original
		MOV ESI,image_copy
		MOV EBX,filter_size
		SHR EBX,1
		XOR EDX,EDX
		MOV EAX,1026
		MUL EBX
		MOV EBX,EAX
		MOV ECX,513
		SUB ECX,filter_size

LOOP_1 :
		PUSH ECX
		PUSH EBX
		MOV ECX,513
		SUB ECX,filter_size

LOOP_2 :
		PUSH ECX
		PUSH EBX
		MOV EAX,1026
		XOR EDX,EDX
		MOV ECX,filter_size
		SHR ECX,1
		MUL ECX
		SUB EBX,EAX
		MOV ECX,filter_size
		MOV AX,0

LOOP_3 :
		PUSH ECX
		PUSH EBX
		MOV ECX, filter_size

LOOP_4 :
		CMP WORD PTR[ESI + EBX],AX
		JNA SMALLER
		MOV AX, WORD PTR[ESI + EBX]

SMALLER:
		ADD EBX,2
		LOOP LOOP_4

		POP EBX
		ADD EBX,1024
		POP ECX
		LOOP LOOP_3

		POP EBX
		POP ECX
		MOV WORD PTR[EDI + EBX],AX
		ADD EBX,2
		LOOP LOOP_2

		POP EBX
		ADD EBX,1024
		POP ECX
		LOOP LOOP_1

	}
	printf("\nDilation operation completed. Image saved as \"dilated.pgm\".\n");
}

void Erosion(int n, int filter_size, short* image_data_original) {

	short* image_copy = (short*)malloc(n * sizeof(short));
	int i;
	for (i = 0; i < n; i++)
		image_copy[i] = image_data_original[i];

	__asm {
		MOV EDI,image_data_original
		MOV ESI,image_copy
		MOV EBX,filter_size
		SHR EBX,1
		XOR EDX,EDX
		MOV EAX,1026
		MUL EBX
		MOV EBX,EAX
		MOV ECX,513
		SUB ECX,filter_size

LOOP_1 :
		PUSH ECX
		PUSH EBX
		MOV ECX,513
		SUB ECX,filter_size

LOOP_2 :
		PUSH ECX
		PUSH EBX
		MOV EAX,1026
		XOR EDX,EDX
		MOV ECX,filter_size
		SHR ECX,1
		MUL ECX
		SUB EBX,EAX
		MOV ECX,filter_size
		MOV AX,7FFFh

LOOP_3:
		PUSH ECX
		PUSH EBX
		MOV ECX,filter_size

LOOP_4 :
		CMP WORD PTR[ESI + EBX],AX
		JA BIGGER
		MOV AX,WORD PTR[ESI + EBX]

BIGGER :
		ADD EBX,2
		LOOP LOOP_4

		POP EBX
		ADD EBX,1024
		POP ECX
		LOOP LOOP_3

		POP EBX
		POP ECX
		MOV WORD PTR[EDI + EBX], AX
		ADD EBX, 2
		LOOP LOOP_2

		POP EBX
		ADD EBX, 1024
		POP ECX
		LOOP LOOP_1

	}
	printf("\nErosion operation completed. Image saved as \"eroded.pgm\".\n");
}
