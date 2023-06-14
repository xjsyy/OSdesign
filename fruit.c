#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#define MAX 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  //初始化互斥锁

pthread_cond_t produce = PTHREAD_COND_INITIALIZER;  //初始化条件变量
pthread_cond_t consume = PTHREAD_COND_INITIALIZER;


int fruit_plate[MAX];
int fruit_number = 0;

//父亲线程
void *Father(void *fruit){

        while(1){
            int fruit_name;                  //1:apple 2:orange.
            scanf("%d",&fruit_name);
            if (fruit_name > 2) {
                break;
            }
            printf("father's id:%d,father:i want to put a fruit:,\n",gettid());
            printf("father put %d into plate\n",fruit_name);
            fflush(stdout);
            pthread_mutex_lock(&mutex);             //加锁
            while(fruit_number >= MAX){
                pthread_cond_wait(&produce,&mutex);     //等待条件变量
            }
            fruit_plate[fruit_number++] = fruit_name;
            pthread_cond_signal(&consume);              //发送条件变量
            pthread_mutex_unlock(&mutex);               //解锁
        }
        // fclose(fp);
        return NULL;
}

//儿子线程
void *Son(void *fruit){
        while(1){
            pthread_mutex_lock(&mutex);
            while(fruit_number <=0 || fruit_plate[fruit_number-1]!= 2){
                    pthread_cond_wait(&consume,&mutex);
            }
            fruit_number --;
            printf("son:I eat a orange.son's's id:%d\n",gettid());
            fflush(stdout);
            pthread_cond_signal(&produce);
            pthread_mutex_unlock(&mutex);
        }
        return NULL;
}

//女儿线程
void *Daughter(void *fruit){
        while(1){
            pthread_mutex_lock(&mutex);
            while(fruit_number <=0 || fruit_plate[fruit_number-1] != 1){
                    pthread_cond_wait(&consume,&mutex);
            }
            fruit_number --;
            printf("daughter:I eat a apple.daughter's id:%d\n",gettid());
            fflush(stdout);
            pthread_cond_signal(&produce);
            pthread_mutex_unlock(&mutex);
        }
        return NULL;
}

int main(){
    pthread_t Father_fruit,Son_fruit,Daughter_fruit;

    //创建线程
    pthread_create(&Father_fruit,NULL,Father,(void *)NULL);
    pthread_create(&Son_fruit,NULL,Son,(void *)NULL);
    pthread_create(&Daughter_fruit,NULL,Daughter,(void *)NULL);

    
    pthread_join(Father_fruit,NULL);
    pthread_join(Son_fruit,NULL);
    pthread_join(Daughter_fruit,NULL);

    pthread_mutex_destroy(&mutex);

    pthread_cond_destroy(&produce);
    pthread_cond_destroy(&consume);

    exit(0);
}