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

int main(int argc,char **argv){
    printf("*****operating system lab1-B*****\n*****15080120guoduhao*****\n\n");
    get_time();
    get_hostname();
    get_cpu_model();
    printf("\n");
    get_kernel_ver();
    printf("\n");
    get_uptime();
    return 0;
}

void get_time(){
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	printf ( "Current local time:\t%s\n", asctime (timeinfo) );
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
