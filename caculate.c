#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char const *argv[])
{
  /*long double standard = 0,value = 0,compute = 0,sigma = 0,v = 0;
  int time=0;
  FILE *avg = fopen("avg.txt", "r");
  fscanf(avg, "%Lf", &compute);
  fclose(avg);
  FILE *mse = fopen("clock.txt", "r");
  fscanf(mse, "%Lf", &v);
  do {
  //printf("%Lf\n", v);
        standard = v - compute;
        standard *= standard;
        sigma += standard;
        time++;
    } while(fscanf(mse, "%Lf", &v) != EOF);
    printf("%Lf\n", sigma / time);
    getchar();
    sigma /= time;
    value = sqrt(sigma);
    //printf("%Lf\n", sigma);
    printf("%Lf\n", value);
    fclose(mse);
    getchar();*/
    FILE *input = fopen("clock.txt", "r");
	FILE *caculate = fopen("caculate.txt", "w");
    int team[5000] = {0};
    int clock;
    fscanf(input, "%d", &clock);
    do {
    	if((clock / 20)< 5000){
        	team[(clock / 20)]++;
    	}

    } while(fscanf(input, "%d", &clock) != EOF);
    puts("count end");
    fprintf(caculate, "0 0\n");
    clock = 0;
    do {
        fprintf(caculate, "%d %d\n", (clock+1)*20 , team[clock]);
    } while((++clock)-5000);
    fclose(input);
    fclose(caculate);
    input = fopen("clock.txt", "r");
	caculate = fopen("caculate2.txt", "w");
	int team2[5000] ={0};
	fscanf(input, "%d", &clock);
    do {
    	if((clock )< 5000){
        	team2[clock]++;
    	}
    } while(fscanf(input, "%d", &clock) != EOF);
    clock = 0;
    do {
        fprintf(caculate, "%d %d\n", clock , team2[clock]);
    } while((++clock)-5000);
    fclose(input);
    fclose(caculate);
	return 0;
}
