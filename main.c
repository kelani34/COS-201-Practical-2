#include <stdio.h>

 struct studentInfo{
       char firstName[20];
       char lastName[20];
       int rollNum;
       float marks;
   };

int main() {

   char userName[20];
   printf("Welcome to the Student Record System \nPlease enter your name:");
   scanf("%s", &userName);
   printf("Hello %s", userName);



  return 0;
}