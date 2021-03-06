#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "tst.h"

/** constants insert, delete, max word(s) & stack nodes */
enum { INS, DEL, WRDMAX = 256, STKMAX = 512, LMAX = 8192 };
#define REF INS
#define CPY DEL

/*typedef struct memory_pool {
    char *current;
    char *tail;
}pool;*/

/* timing helper function */
static double tvgetf(struct timespec *ts)
{
    double sec;
    sec = ts->tv_nsec;
    sec /= 1e9;
    sec += ts->tv_sec;

    return sec;
}


/* simple trim '\n' from end of buffer filled by fgets */
static void rmcrlf(char *s)
{
    size_t len = strlen(s);
    if (len && s[--len] == '\n')
        s[len] = 0;
}

#define IN_FILE "cities.txt"

int main(int argc, char **argv)
{
    char word[WRDMAX] = "";
    char *sgl[LMAX] = {NULL};
    tst_node *root = NULL, *res = NULL;
    int rtn = 0, idx = 0, sidx = 0;
    FILE *fp = fopen(IN_FILE, "r");
    double t1, t2;
    struct timespec ts1,ts2;
    pool *ptr = init(25000000);
    int flag = 0;

    if (!fp) { /* prompt, open, validate file for reading */
        fprintf(stderr, "error: file open failed '%s'.\n", argv[1]);
        return 1;
    }

    clock_gettime(CLOCK_REALTIME, &ts1);
    fscanf(fp, "%s", word);
    do {
        flag = 0;
        char *p = mpalloc(&ptr , strlen(word)+1);
        strncpy( p,word,strlen(word));//why is change ptr????
        p[strlen(word)] = 0;
        if (!tst_ins_del(&root, &p, INS, REF,&flag , ptr)) {
            fprintf(stderr, "error: memory exhausted, tst_insert.\n");
            fclose(fp);
            return 1;
        }
        if(flag==0){
        }else{
            mpfreeback(&ptr,strlen(word)+1);
        }
        idx++;
    } while ((rtn = fscanf(fp, "%s", word)) != EOF);
    clock_gettime(CLOCK_REALTIME, &ts2);
    t1 = tvgetf(&ts1);
    t2 = tvgetf(&ts2);
    fclose(fp);
    printf("ternary_tree, loaded %d words in %.6f sec\n", idx, t2 - t1);
    if(ts1.tv_sec == ts2.tv_sec)
        printf("load-Clock %ld \n\n", ts2.tv_nsec - ts1.tv_nsec);
    else
        printf("load-Clock %ld \n\n", ts2.tv_nsec+1000000000 - ts1.tv_nsec);
    printf("pool remain %d size\n", getLarge(ptr));

    for (;;) {
        printf(
            "\nCommands:\n"
            " a  add word to the tree\n"
            " f  find word in tree\n"
            " s  search words matching prefix\n"
            " d  delete word from the tree\n"
            " q  quit, freeing all data\n\n"
            "choice: ");
        fgets(word, sizeof word, stdin);
        switch (*word) {
        case 'a':
            printf("enter word to add: ");
            if (!fgets(word, sizeof word, stdin)) {
                fprintf(stderr, "error: insufficient input.\n");
                break;
            }
            //rmcrlf(word);
            clock_gettime(CLOCK_REALTIME, &ts1);
            char *p = mpalloc(&ptr , strlen(word)+1);
            strncpy( p,word,strlen(word));
            p[strlen(word)] = 0;
            res = tst_ins_del(&root, &p, INS, CPY,&flag,ptr);
            if(flag==0){
            }else{
                mpfreeback(&ptr,strlen(word)+1);
            }
            clock_gettime(CLOCK_REALTIME, &ts2);
            t1 = tvgetf(&ts1);
            t2 = tvgetf(&ts2);
            if (res) {
                idx++;
                printf("  %s - inserted in %.6f sec. (%d words in tree)\n",
                       (char *) res, t2 - t1, idx);
                if(ts1.tv_sec == ts2.tv_sec)
                    printf("Clock %ld \n\n", ts2.tv_nsec - ts1.tv_nsec);
                else
                    printf("Clock %ld \n\n", ts2.tv_nsec+1000000000 - ts1.tv_nsec);
            } else
                printf("  %s - already exists in list.\n", (char *) res);
            break;
        case 'f':
            printf("find word in tree: ");
            if (!fgets(word, sizeof word, stdin)) {
                fprintf(stderr, "error: insufficient input.\n");
                break;
            }
            rmcrlf(word);
            clock_gettime(CLOCK_REALTIME, &ts1);
            res = tst_search(root, word);
            clock_gettime(CLOCK_REALTIME, &ts2);
            t1 = tvgetf(&ts1);
            t2 = tvgetf(&ts2);
            if (res){
                printf("  found %s in %.6f sec.\n", (char *) res, t2 - t1);
            if(ts1.tv_sec == ts2.tv_sec)
                printf("Clock %ld \n\n", ts2.tv_nsec - ts1.tv_nsec);
            else
                printf("Clock %ld \n\n", ts2.tv_nsec+1000000000 - ts1.tv_nsec);
            }
            else
                printf("  %s not found.\n", word);
            break;
        case 's':
            printf("find words matching prefix (at least 1 char): ");
            if (!fgets(word, sizeof word, stdin)) {
                fprintf(stderr, "error: insufficient input.\n");
                break;
            }
            rmcrlf(word);
            clock_gettime(CLOCK_REALTIME, &ts1);
            res = tst_search_prefix(root, word, sgl, &sidx, LMAX);
            clock_gettime(CLOCK_REALTIME, &ts2);
            t1 = tvgetf(&ts1);
            t2 = tvgetf(&ts2);
            if (res) {
                printf("  %s - searched prefix in %.6f sec\n\n", word, t2 - t1);
                if(ts1.tv_sec == ts2.tv_sec)
                    printf("Clock %ld \n\n", ts2.tv_nsec - ts1.tv_nsec);
                else
                    printf("Clock %ld \n\n", ts2.tv_nsec+1000000000 - ts1.tv_nsec);
                for (int i = 0; i < sidx; i++)
                    printf("suggest[%d] : %s\n", i, sgl[i]);
            } else
                printf("  %s - not found\n", word);
            break;
        case 'd':
            printf("enter word to del: ");
            if (!fgets(word, sizeof word, stdin)) {
                fprintf(stderr, "error: insufficient input.\n");
                break;
            }
            rmcrlf(word);
            p = word;
            printf("  deleting %s\n", word);
            clock_gettime(CLOCK_REALTIME, &ts1);
            res = tst_ins_del(&root, &p, DEL, CPY,&flag,ptr);
            clock_gettime(CLOCK_REALTIME, &ts2);
            t1 = tvgetf(&ts1);
            t2 = tvgetf(&ts2);
            if (res){
                printf("  delete failed.\n");
                if(ts1.tv_sec == ts2.tv_sec)
                    printf("Clock %ld \n\n", ts2.tv_nsec - ts1.tv_nsec);
                else
                    printf("Clock %ld \n\n", ts2.tv_nsec+1000000000 - ts1.tv_nsec);
            }
            else {
                printf("  deleted %s in %.6f sec\n", word, t2 - t1);
                idx--;
            }
            break;
        case 'q':
            pool_free(ptr);
            fflush(stdin);
            fflush(stdout);
            return 0;
            break;
        default:
            fprintf(stderr, "%s error:REF invalid selection.\n",word);
            break;
        }
    }

    return 0;
}
