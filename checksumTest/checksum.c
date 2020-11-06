#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int Left(int sum) {
	return sum >> 16;
}

int Right(int sum) {
	return sum & 0x0000ffff;
}

int calculation_Checksum(int *message,int length) {
	int checksum;
	int sum;

	sum = 0;
	for (int i = 0;i < length;i++) {
		sum += message[i];
	}
	printf("sum = 0x%.8x\n\n", sum);
	while (Left(sum) != 0) {
		sum = Left(sum) + Right(sum);
	}
	
	
	sum = ~sum;
	checksum = sum & 0x0000ffff; //checksum 을 다시 16비트로 만듬
	return checksum;
}

int check_Checksum(int* message, int length, int checksum) {
	int sum;
	int new_checksum;

	printf("\n----Checksum(received)----\n");
	printf("%d (0x%.4x)\n", checksum, checksum);
	sum = 0;
	for (int i = 0;i < length;i++) {
		sum += message[i];
	}
	while (Left(sum) != 0) {
		sum = Left(sum) + Right(sum);
	}

	sum += checksum;

	sum = ~sum;
	new_checksum = sum & 0x0000ffff;
	
	printf("\n----New Checksum----\n");
	printf("%d (0x%.4x)\n", new_checksum, new_checksum);

	if (new_checksum) {
		return 0;
	}
	else {
		return 1;
	}
}

int main() {
	
	int* msg;
	unsigned int length;
	srand((unsigned)time(NULL));
	length = 4; //임의의 길이 : 4
	msg = (int*)malloc(sizeof(int) * length);
	printf("---- 생성된 word ----\n");
	for (int i = 0;i < length;i++) {
		msg[i] = rand() % 32768; //16비트 최대 표현 범위		
		printf("%d (0x%.4x)\n", msg[i],msg[i]);
	}printf("\n");
	


	int checksum;
	checksum = calculation_Checksum(msg, length);

	printf("----Checksum(to send)----\n");
	printf("%d (0x%.4x)\n", checksum,checksum);

	//printf(" %d는 0의보수 ", ~0);
	if (check_Checksum(msg, length, checksum)) {
		fprintf(stdout, "\n***Program Success***");
	}
	else {
		fprintf(stderr, "error detected");
		exit(1);
	}

	return 0;
}