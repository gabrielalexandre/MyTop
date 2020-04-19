//comando para compilação
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#define MAX 30

/* grava os dados do processo(pid, prioridade, ...) */
typedef struct setTop{
  int pid;
  int priority;
  double cpuPorcetage;
  char state;
  char user[MAX];
  char command[MAX];
}Top;

/* Vetor de struct para armazenar dados do processo
e ordenar de forma descrescente a partir do cpuPorcetage 
e um index para gerenciar as posições*/
Top mytop[1000];
int idx = 0;

#include "sort.h"
#include "print.h"
#include "delay.h"

/* Função para buscar comando do processo de /proc/[pid]/comm */
#include "command.h"


/* Função para buscar estado, prioridade, utime e stime de /proc/[pid]/stat */
#include "stat.h"

void myTop(){
  DIR *dir;
  struct dirent *dit;
  struct stat arq;
  struct passwd *reg;
  char buff[50], cmdline[30];
  
  if(!(dir = opendir("/proc/"))){
    printf("Erro ao abrir o diretório /proc");
    exit(1);
  }

  while((dit = readdir(dir)) != NULL){
    strcpy(buff, "/proc/"); //inicia o buff com "/proc/"

    if(!atoi(dit->d_name)){ continue; } //se o dit->d_name nao for um numero volta para o while


    /* --- PID --- */
    mytop[idx].pid = atoi(dit->d_name);
    //printf("%d\t", mytop[idx].pid);//imprime o id do processo

    /* --- USER --- */
    strcat(buff,dit->d_name); //concatena buff com o pid. buff com "/proc/[pid]"
    stat(buff, &arq);
    reg = getpwuid(arq.st_uid);
    strncpy(mytop[idx].user,reg->pw_name, 7);
    mytop[idx].user[7] = '\0';
    //printf("%s\t", mytop[idx].user);//imprime o user do processo

    strcpy(cmdline, buff);
    strcat(cmdline,"/comm");
    strcat(buff,"/stat"); //concatena o buff com "/stat" sendo assim "proc/[pid]/stat"

    getSomeThingsFromStat(buff);
    getCommand(cmdline);
    idx++;
  }

  quick_sort(0, idx-1);
  printTop();
}


int main(){
  while(1){
    system("clear");
    idx = 0;
    myTop();
    delay(4000);
  }
  return 0;
}