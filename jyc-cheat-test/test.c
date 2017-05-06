#include <stdio.h>

int main()
{
int i, j, k;
j=0;
k=0;

for (i=0; i<10; i++){
if((i%2) != 0){
j+=1;
}
else{
k+=1;
}
printf("j=%d, k=%d\n", j,k);
}
return 0;
}
