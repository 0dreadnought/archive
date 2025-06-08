#include <stdio.h>
#include <time.h>
#include <string.h>


int main(void){

struct player{

char name[10];
int score;

};

int compare(const void *a, const void *b){
  return ((struct player*)b)->score-((struct player*)a)->score;
}

char pname[10],chr[10],ch;
int num,gs,t,i,numb;

FILE *fle;

t=0;

srand(time(NULL));
num=1+rand()%5000;

printf("GUESS THE NUMBER(1-5000) !!!! (Ctrl+Z to concede)\n");
while(gs!=num && t<12){
scanf("%i",&gs);
if(gs>num){
printf("Aim lower\n");
}
else if(gs<num){
printf("Aim higher\n");
}
t++;
}

if(gs!=num){
printf("Better luck next time\n");

}
else{
printf("Nailed It!\n\n");
printf("Number of tries:%i\n\n",t);
printf("How do you want to be written down in history?\n");
scanf("%s",&pname);

fle = fopen("leaderboard","r");
rewind(fle);
struct player players[10];
i=0;
while(!feof(fle)){
fscanf(fle,"%i %s %d",&numb,players[i].name,&players[i].score);
i++;
if(i==9||numb==EOF){
break;
}
}
fclose(fle);
i++;

memcpy(players[i].name,pname,sizeof(pname));
players[i].score=10000/t;

printf("\n");

i++;
qsort(players,i,sizeof(struct player), compare);

i=10;


fle = fopen("leaderboard","w");
rewind(fle);

for(int j=0;j<i;j++){
fprintf(fle,"%d %s %d\n",j,players[j].name,players[j].score);
}

fclose(fle);
}

fle = fopen("leaderboard","r");
rewind(fle);

while(!feof(fle)){
ch=getc(fle);
if(ch!=EOF){
printf("%c",ch);
}
}

return 0;
}
