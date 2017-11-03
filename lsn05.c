#include<stdio.h>

int main(){
	char myString[50];
	printf("Enter your string: ");
	fgets(myString, 50, stdin);
	printf("%s", myString);
	return 0;
}
