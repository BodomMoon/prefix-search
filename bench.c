#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <glib.h>
#include <assert.h>
#define IN_FILE "cities.txt"
#define MYMIN(a,b) (((a)<(b))?(a):(b))

int main()
{
    char line[128];
    int getLenth,strLenth;
    unsigned int all = 0,time = 0;
    struct timespec ts;
    puts("reopen success");
    FILE *cities = fopen(IN_FILE, "r");
    FILE *output = fopen("test.txt", "w");

    fscanf(cities, "%s", line);
    //printf("%ld\n", g_utf8_strlen(line,-1));
    do {
        clock_gettime(CLOCK_REALTIME, &ts);
        //getLenth = (((int)ts.tv_nsec) >> 2) && 0x00000001; // 1 or 0
        if((((int)ts.tv_nsec) >> 2) & 1) {
            strLenth = (((int)ts.tv_nsec) & 6) + 1;
            strLenth = MYMIN(strLenth,(int)g_utf8_strlen(line,-1));
            *(g_utf8_offset_to_pointer (line,(long) strLenth)) = '\0';
            fprintf(output, "s\n");
            fprintf(output, "%s\n", line);
        }
    } while(fscanf(cities, "%s", line) != EOF);
    fprintf(output, "q\n");
    fclose(cities);
    fclose(output);
    // make test input success
    assert(freopen("output.txt","w",stdout) &&
           "stdout freopen fail");
    assert(freopen("test.txt","r+",stdin) &&
           "stdin freopen fail");
    system("./test_cpy");
    fclose(stdout);
    fclose(stdin);
    cities = fopen("output.txt", "r");
    output = fopen("clock.txt", "w");
    fscanf(cities, "%s", line);
    int clock;
    do {
        if(strcmp(line, "Clock")!=0) {
        } else {
            fscanf(cities, "%d", &clock);
            fprintf(output, "%d\n", clock);
            all += clock;
            time++;
        }

    } while(fscanf(cities, "%s", line) != EOF);
    fclose(cities);
    fclose(output);
    output = fopen("avg.txt", "w");
    //printf("all = %d time = %d \n",all,time );
    fprintf(output, "%d\n",all/time);

    fclose(output);

    /*
    if(freopen("BENCH_O.txt","w",stdout)==NULL)
        printf("stdout freopen fail\n");
    if(freopen("BENCH_I.txt","r+",stdin)==NULL)
        printf("stdin freopen fail\n");
    system("./print");
        if(popen("./print","w"))
     		    puts("success");
    if(freopen("OUTPUT.txt","r",stdio)==NULL)
        puts("reopen stdio success")
    fgets(line, sizeof line, stdin);
    */
}
/*
流程

Make部份

1.MAKE 執行外部測試檔
2.外部測試檔重新產生 test.txt
3.外部測試freopen 並將stdio->test.txt stdout->out.txt
4.返回外部測試檔  外部測試檔讀取output.txt 取出clock部份資料並整理為clock.txt
5.MAKE 執行 caculate 計算標準差並取出信賴區間

編譯時必須使用  gcc bench.c `pkg-config --cflags --libs glib-2.0` -o bench
*/