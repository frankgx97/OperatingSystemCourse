#include <stdio.h>

#define MAX_USER 10         //最大用户个数
#define MAX_PROCESS 10      //最大进程数量
#define RUNNUNG 1
#define READY 0
#define FREE -1
#define TIMESLICE 10        //一个时间片内的时间


struct pcb {
    int name;  			        //进程标识符
    int uid;                    //进程所属用户
    int status;  			    //进程状态
    int ax, bx, cx, dx; 		//进程现场信息，通用寄存器内容
    int pc;         		    //进程现场信息，程序计数器内容
    int psw;       		        //进程现场信息，程序状态字寄存器内容
    int total_need;             //完成进程所需总时间
    int still_need;             //完成进程仍需多长时间
    int next;       		    //下一个进程控制块的位置
};


int main() {

    /*****************************************************
     *  定义及初始化
     *****************************************************/

    int user_process_num[MAX_USER]; //用户当前进程个数
    struct pcb pcbarea[MAX_PROCESS];
    int run;        //指向正在运行进程的进程控制块的指针
    struct {
        int head;
        int tail;
    }ready;         //指向就绪队列的头指针head和尾指针tail
    int free;       //指向空闲进程控制块队列的头指针


    run=-1;
    ready.head=-1;
    ready.tail=-1;
    free=0;
    int i;
    for(i=0;i<MAX_PROCESS;i++){
        pcbarea[i].status=FREE;
        pcbarea[i].name=i+20786;
        pcbarea[i].uid=-1;
        pcbarea[i].ax=-1;
        pcbarea[i].bx=-1;
        pcbarea[i].cx=-1;
        pcbarea[i].dx=-1;
        pcbarea[i].pc=-1;
        pcbarea[i].psw=-1;
        pcbarea[i].next=i+1;
        pcbarea[i].total_need=0;
        pcbarea[i].still_need=0;
    }
    pcbarea[9].next=-1;
    for(i=0;i<MAX_USER;i++){
        user_process_num[i]=0;
    }





    /*****************************************************
     *  每一行各字段含义为:
     *  所属用户 ax bx cx dx pc psw 完成此进程所需时间
     *****************************************************/

    int process_num;
    printf("Input process num:\n");
    scanf("%d",&process_num);

    int j,uid,ax,bx,cx,dx,pc,psw,total_need;
    for(j=0;j<process_num;j++){
        scanf("%d",&uid);
        scanf("%d",&ax);
        scanf("%d",&bx);
        scanf("%d",&cx);
        scanf("%d",&dx);
        scanf("%d",&pc);
        scanf("%d",&psw);
        scanf("%d",&total_need);

        //检查是否有空闲块
        if(free==-1){
            printf("没有空闲块\n");
            break;
        }

        int next=pcbarea[free].next;

        //填写进程控制块
        pcbarea[free].status=READY;
        pcbarea[free].uid=uid;
        pcbarea[free].ax=ax;
        pcbarea[free].bx=bx;
        pcbarea[free].cx=cx;
        pcbarea[free].dx=dx;
        pcbarea[free].pc=pc;
        pcbarea[free].psw=psw;
        pcbarea[free].total_need=total_need;
        pcbarea[free].still_need=total_need;
        pcbarea[free].next=-1; //就绪队列的队尾next为-1

        //用户进程数加1
        user_process_num[uid]++;

        //挂入就绪队列
        if(ready.head==-1){
            ready.head=free;
            ready.tail=free;
        }else{
            pcbarea[ready.tail].next=free;
            ready.tail=free;
        }

        //空闲块指针后移
        free=next;

    }

    //fclose(pinfo);

    //当前用户个数
    int user_num=0;
    for(i=0;i<MAX_USER;i++){
        if(user_process_num[i]!=0){
            user_num++;
        }
    }





    /*
     *  单位CPU时间片内,每个用户所用的总时间相同
     *  每个进程依次运行响应的时间,然后切换到下一个进程
     *  当一个进程结束后,该进程块从就绪队列移除,并添加到空闲队列
     */

    run=ready.head;
    int count=1;                            //当前为第几个时间片
    int timeslice_used=0;                   //一个时间片内已被使用的时间
    int process_num_this_slice[MAX_USER];   //这个时间片内各用户的进程个数
    for(i=0;i<MAX_USER;i++){
        process_num_this_slice[i]=user_process_num[i];
    }

    while(ready.head!=-1){

        if(timeslice_used==0){
            printf("\n\n---------------------------------------------------");
            printf("第%d个时间片:",count);
            printf("---------------------------------------------------\n\n");
            printf("进程标识符\t所属用户id\t运行总时间\t已运行时间\t此时间片内运行时间\t\tax\t\tbx\t\tcx\t\tdx\t\tpc\t\tpsw\t");
            for(i=0;i<MAX_USER;i++){
                process_num_this_slice[i]=user_process_num[i];
            }
        }

        //下一个就绪进程块
        int next;
        if(run!=ready.tail){
            next=pcbarea[run].next;
        }else{
            next=ready.head;
        }



        /*------------------------------
         *  运行进程
         *  每个用户平分一个时间片,而用户的
         *  每个进程平分用户被分配的时间
        -------------------------------*/

        int run_time=TIMESLICE/user_num/process_num_this_slice[pcbarea[run].uid];
        pcbarea[run].status=RUNNUNG;
        pcbarea[run].still_need-=run_time;
        printf("\n%d\t\t%d\t\t\t%d\t\t\t%d\t\t\t%d\t\t\t\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t",
               pcbarea[run].name,
               pcbarea[run].uid,
               pcbarea[run].total_need,
               pcbarea[run].total_need-pcbarea[run].still_need,
               run_time,
               pcbarea[run].ax,
               pcbarea[run].bx,
               pcbarea[run].cx,
               pcbarea[run].dx,
               pcbarea[run].pc,
               pcbarea[run].psw);
        timeslice_used+=run_time;
        if(timeslice_used>=TIMESLICE){
            count++;
            timeslice_used=0;
        }



        //运行结束,修改进程块状态

        if(pcbarea[run].still_need<=0){
            printf("该进程已完成  "); //<-----?打印信息
            user_process_num[pcbarea[run].uid]--;
            if(user_process_num[pcbarea[run].uid]==0){
                user_num--;
                printf("用户%d所有进程均已完成",pcbarea[run].uid); //<-----?打印信息
            }

            //从就绪队列移除
            if(run==ready.head){
                ready.head=pcbarea[ready.head].next;
            }else{
                int p=ready.head;
                while(pcbarea[p].next!=run)p=pcbarea[p].next;
                if(run==ready.tail){
                    pcbarea[p].next=pcbarea[run].next;
                    ready.tail=p;
                }else{
                    pcbarea[p].next=pcbarea[run].next;
                }
            }

            //添加到空闲队列
            pcbarea[run].next=free;
            free=run;
            pcbarea[run].status=FREE;

        }else{
            pcbarea[run].status=READY;
        }



        /*----------------------------
         *  切换为下一个进程
        -----------------------------*/

        run=next;

    }


    return 0;
}