#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

//Function called by thread
void* thread_function(void* args_p){

    char *args = (char*)args_p;
    while(1){
        printf("%s\n", args);
        sleep(1);
    }
    return ;
}

void main(){
    int i, j, flag = 1;
    clock_t thread_time[10], now;
    int thread_manager[10]={0,0,0,0,0,0,0,0,0,0};
    pthread_t *threads;
    
    threads = (pthread_t*)malloc(sizeof(pthread_t)*10);
    
    for(i = 0; i < 10; i++){
        j=i+48;
        thread_time[i]=clock();
        thread_manager[i]=1;
        pthread_create(&threads[i], NULL, thread_function, (void *)&j);
        sleep(1);
    }

    for(i = 0; i < 10; i++)
        pthread_join(threads[i], NULL);

    while(flag == 1) {
        for (i=0;i<10;i++){
            now = clock();
            if (thread_manager[i] ==1 && (double)(now-thread_time[i])>3){
                thread_manager[i] = 0;
                pthread_kill(&threads[i], 9);
            }
        }
        flag = 0;
        for (i=0;i<10;i++) {
            if (thread_manager[i] == 1){
                flag = 1;
                break;
            }
        }
    }

    return;
}  