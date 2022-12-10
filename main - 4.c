#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

sem_t semaphore1;
sem_t semaphore2;
sem_t semaphore3;

int curvness = 0;
int sharpness = 0;
int kol;  // кол-во циклов первого потока
int kolsharpres; // кол-во циклов 2 и 3 потоков
static int counter = 0; // кол-во хороших булавок

void* checkCurve (void* args) {
    for (unsigned int i = 0; i < kol; i++) {
        srand(time(NULL));
        int time = rand() % 2 + 2;
        sleep(time);
        curvness = rand() % 2;  // если булавка кривая, то в переменной будет 0
        printf("%d %d %d checkCurve\n", i, curvness, sharpness);
        if (!curvness) {
            kolsharpres--;
        } else {
            curvness = 0;
            sem_post(&semaphore1);  // если все хорошо, приступаем к заточке
        }
    }
}
void* sharpening (void* args) {
    for (unsigned int i = 0; i < kolsharpres; i++) {
        sem_wait(&semaphore1);  // начинаем работу второго потока
        int time = rand() % 3 + 2;
        sleep(time);
        if (sharpness != 2) {
            sleep(2);
        }
        sharpness = rand() % 2;  // если плохо заточили, в переменной будет 0
        printf("%d %d %d sharpening\n", i, curvness, sharpness);
        sem_post(&semaphore2);  // даем команду на начало работы 3 потока
    }
}
void* checkResult (void* args) {
    for (unsigned int i = 0; i < kolsharpres; i++) {
        sem_wait(&semaphore2);  // запускаем 3 поток
        int time = rand() % 3 + 2;
        sleep(time);
        if (sharpness == 2) {
            sleep(1);
        }
        printf("%d %d %d checkResult\n", i, curvness, sharpness);
        if (sharpness == 1) {  // проверяем, насколько хорошо заточена булавка
            counter++;   // кол-во хорошо заточенных булавок
            sharpness = 2;
        }
    }
}

int main (int argc, char *argv[]) {
    int isFile = 0;
    if (argc != 1) {
        if (!strcmp(argv[1], "file")) {
            FILE *file;
            file = fopen("file.txt", "r");
            if (file == NULL) {
                perror("Error opening file");
                return(-1);
            }
            isFile = 1;
            fscanf(file, "%d", &kol);
            kolsharpres = kol;
            fclose(file);
        } else if (!strcmp(argv[1], "gen")) {
            srand(time(NULL));
            kol = rand() % 10 + 5;
            kolsharpres = kol;
            printf("%d\n", kol);
        } else {
            printf("error");
            return(-1);
        }
    } else {
        scanf("%d", &kol);
        kolsharpres = kol;
    }
    
    pthread_t thread1;  
    pthread_t thread2;
    pthread_t thread3;
    
    sem_init(&semaphore1, 0, 0); //  инициализируем семафоры
    sem_init(&semaphore2, 0, 0);
    
    pthread_create(&thread1, NULL, checkCurve, NULL); //  создаем потоки
    pthread_create(&thread2, NULL, sharpening, NULL);
    pthread_create(&thread3, NULL, checkResult, NULL);

    pthread_join(thread1, NULL);  //  создаем последовательность потоков
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    
    sem_destroy(&semaphore1); //  удаляем семафоры
    sem_destroy(&semaphore2);
    sem_destroy(&semaphore3);

    if (isFile) {
        FILE *output;
        output = fopen("output.txt", "w");
        fprintf(output,"Хороших булавок - %d из %d!", counter, kol);
        fclose(output);
    }

    printf("Хороших булавок - %d из %d!", counter, kol);    

    return 0;
}