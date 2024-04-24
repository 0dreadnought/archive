#include <stdio.h>
#include <string.h>
char cat[20];

void inpname(){
char name[20];
printf("Name of the file: ");
scanf("%s",name);
strcpy(cat,name);
}

char inpopt(){
char opt, line[20];
printf("Option: ");
scanf("%c",line);
opt=line[0];
return opt;
}

void WIP(void){
printf("\n");
printf("Features will be added soon!\n");
}

void kitty(char opt, char cat[20]){

char ch,lastchar;
int flag=1, i=1;

FILE *fle;
lastchar='\0';

fle = fopen(cat,"r");
if (fle==NULL){
perror("Error");
}

else{


rewind(fle);
while((ch=getc(fle))!= EOF){
 switch(opt){
   
  case'b':
    if(flag&&ch!='\n'){printf("%i",i);i++;flag=0;}
    else if(ch=='\n'){flag=1;}
    break;

  case'E':
    if(ch=='\n'){printf("$");}
    break;

  case'n':
    if(flag){printf("%i",i);i++;flag=0;}
    if(ch=='\n'){flag=1;}
    break;

  case's':
    if(lastchar=='\n'&&ch=='\n'){ch='\0' ;lastchar='\n';flag=0;}
    else if(lastchar=='\n'&&ch!='\n'&&!flag){printf("\n");flag=1;}
    else lastchar=ch;
    break;

  case'T':
    if(ch=='\t'){ch='\0';printf("^I");} 
    break;

  case'h':
    ch='\0';
    if(flag){flag=0;
	printf("A, combination of E and T \n");
	printf("b, number nonempty output lines \n");
	printf("E, display $ at end of each line \n");
	printf("n, number all output lines \n");
	printf("s, suppress repeated empty output lines \n");
	printf("T, display TAB characters as ^I \n");
	printf("v, output version information \n");
	printf("h, displays this message \n");
}
    break;
 
  case'A':
    if(ch=='\n'){printf("$");}
    if(ch=='\t'){ch='\0';printf("^I");} 
    break;
  
  case'v':
    ch='\0';
    if(flag){flag=0; printf(" kitty Version 1.0\n");}
    break;

  case'\n':
    break;

  default:
   ch='\0';
    if(flag){flag=0;
	printf(" Options are:\n");
	printf("  A, combination of E and T \n");
	printf("  b, number nonempty output lines \n");
	printf("  E, display $ at end of each line \n");
	printf("  n, number all output lines \n");
	printf("  s, suppress repeated empty output lines \n");
	printf("  T, display TAB characters as ^I \n");
	printf("  v, output version information \n");
	printf("  h, displays this message \n");
}
   break;
 }

printf("%c",ch);
}

}

fclose(fle);
}

int main(void){

char opt;
opt= inpopt();
inpname();
printf("\n");

printf("\n");
kitty(opt,cat);
return 0;
}
