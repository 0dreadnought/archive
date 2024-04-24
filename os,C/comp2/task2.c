#include <stdio.h>

int main(void){

FILE *fle;
char str[100];
char name;
char ch;
int num;

printf("Name of the file: ");
scanf("%s", str);



printf("Check(1) or Change(2): ");
scanf("%i", &num);

if(num==2){

fle = fopen(str,"w");
rewind(fle);

printf("Ctrl+D to finish \n");

while((ch=getchar())!=EOF){
putc(ch,fle);
}
}

else{

fle = fopen(str,"r");
rewind(fle);

while((ch=getc(fle))!= EOF){
printf("%c",ch);
}
}

fclose(fle);
return 0;
}
