#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <time.h>
#define N 100

void get_cpu_model();
void get_kernel_ver();
void get_uptime();
void get_hostname();
void get_time();
void get_cpu_user();

int main(){
    printf("*****operating system lab1-C*****\n*****15080120guoduhao*****\n\n");
    get_time();
    get_hostname();
    get_cpu_model();
    printf("\n");
    get_kernel_ver();
    printf("\n");
    get_uptime();
    printf("***************\n");
    get_cpu_user();
    return 0;
}

void get_cpu_user(){
    FILE *fp;
    char str[N+1];
    if( (fp=fopen("/proc/stat","rt")) == NULL ){
        printf("Cannot open file, press any key to exit!\n");
	return;
    }
    fgets(str, N, fp);
    printf("cpu string:\t%s\n",str);
    char *delim = " ";
    strtok(str, delim);
    printf("CPU in user mode:\t%s\n", strtok(NULL, delim));
    strtok(NULL, delim);
    printf("CPU in system mode:\t%s\n", strtok(NULL, delim));
    printf("CPU in idle mode:\t%s\n\n", strtok(NULL, delim));
    
    int i;
    for(i=0;i<8;i++){
        fgets(str,N,fp);
    }
    strtok(str, delim);
    printf("Context switches:\t%s\n",strtok(NULL, delim));

    fgets(str,N,fp);
    strtok(str, delim);
    printf("Boot time:\t%s\n",strtok(NULL, delim));

    fgets(str,N,fp);
    strtok(str, delim);
    printf("Processes created:\t%s\n",strtok(NULL, delim));
    return;
}

void get_time(){
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	printf("Current local time:\t%s\n", asctime(timeinfo));
}

void get_hostname(){
    char hostname[50];
    FILE *fp;
    char str[N+1];
    if( (fp=fopen("/proc/sys/kernel/hostname","rt")) == NULL ){
        printf("Cannot open file, press any key to exit!\n");
	return;
    }
    while(fgets(str, N, fp) != NULL){
        strcat(hostname,str);
    }
    printf("Hostname:\t%s\n",hostname);
    return;
}

void get_uptime(){
    char seconds_str[10];
    int seconds;
    FILE *fp;
    char str[N+1];
    int i;
    if( (fp=fopen("/proc/uptime","rt")) == NULL ){
        printf("Cannot open file, press any key to exit!\n");
	return;
    }
    while(fgets(str, N, fp) != NULL){
        for(i=0;;i++){
            if(str[i] == '.'){
                seconds_str[i] = str[i];
                break;
            }else{
                seconds_str[i] = str[i];
            }
        }
    }
    seconds = atoi(seconds_str);
    int d = seconds/86400;
    int hr = (seconds%86400)/3600;
    int min = ((seconds%86400)%3600)/60;
    int sec = ((seconds%86400)%3600)%60;
    printf("System uptime:\t%d:%d:%d:%d\n",d,hr,min,sec);
    fclose(fp);
}

void get_kernel_ver(){
    struct utsname my_uname;
    if(uname(&my_uname) == -1)
        printf("uname call failed!");
    else
        printf("Linux version:\t%s\n",my_uname.release);
    return;
}

void get_cpu_model(){
    char char_all[1000];
    char *p;
    FILE *fp;
    char str[N+1];
    if( (fp=fopen("/proc/cpuinfo","rt")) == NULL ){
        printf("Cannot open file, press any key to exit!\n");
	return;
    }
    while(fgets(str, N, fp) != NULL){
        strcat(char_all,str);
    }
    //printf("%s", char_all);
    p = strstr(char_all,"model name");
    while(*p != '\n'){
        printf("%c",*p);
        p++;
    }
    printf("\n");
    fclose(fp);
}
