#include <stdio.h>

int main(void){

int a;
printf("Number of rows: ");
scanf("%i", &a);

int i,b,c,g;

if(a%2==1){
g=(a+1)/2;
}
else{g=a/2;}

for(i=g;i!=-g;i--){
  for(c=abs(i);c!=0;c--){
    printf(" ");
  }
b=g-abs(i);
  for(c=0;c!=b;c++){
    printf("* ");
  }
printf("\n");
  if(i==0 && a%2==0){
    b=g-abs(i);
    for(c=0;c!=b;c++){
      printf("* ");
    }
  printf("\n");
  }
}

return 0;
}
