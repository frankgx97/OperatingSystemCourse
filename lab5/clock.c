#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
 
 
//字符串长度为21。其中前10位为秒，后9位为纳秒，中间1位为空格
//当纳秒不够9位时，右对齐，前面补空格
#define LEN 21


int len,has_read;   //字符串长度，字符串已读取的长度
char *msg;      //字符串内容


/* -------------------- 相关结构体与函数的定义 -------------------- */

/**
模块的读取函数
filp: 文件指针
buf: 用户保存数据的位置
count: 要读取的字节个数
offp: 文件指针的偏移
*/
int read_proc(struct file *filp,char *buf,size_t count,loff_t *offp ) {

  /* ----- 设置msg ----- */
  
  
  //获取当前xtime
  struct timespec time_ckt;
  time_ckt=current_kernel_time();
  
  
  //将秒转为字符串
  unsigned long p=time_ckt.tv_sec;
  int len_sec=0; //秒的位数
  while(p/=10){
    len_sec++;
  }
  len_sec++;
  
  int i;
  p=time_ckt.tv_sec;
  for(i=0;i<len_sec;i++){
    int x=1;
    int j;
    for(j=i+1;j<len_sec;j++){
      x*=10;
    }
    
    msg[i]=p/x+'0';
    p-=(msg[i]-'0')*x;
  }
  
  
  //添加空格
  msg[len_sec]=' ';
  
  
  //将纳秒转为字符串
  p=time_ckt.tv_nsec;
  int len_nsec=0; //纳秒的位数
  while(p/=10){
    len_nsec++;
  }
  len_nsec++;
  
  //不够9位要补空格
  if(len_nsec<9){
    for(i=0;i<9-len_nsec;i++){
     msg[len_sec+1+i]=' ';
    }
  }
  
  p=time_ckt.tv_nsec;
  for(i=0;i<len_nsec;i++){
    int x=1;
    int j;
    for(j=i+1;j<len_nsec;j++){
      x*=10;
    }
    
    msg[len_sec+1+9-len_nsec+i]=p/x+'0';
    p-=(msg[len_sec+1+9-len_nsec+i]-'0')*x;
  }
  
  
  //字符串结束
  msg[len]='\0';
  

  /* ----- 将msg结果复制给用户 ----- */
  
  if(count+has_read>=len){
    count=len-has_read;
  }
  
  has_read=has_read+count;
  copy_to_user(buf, msg, count);
  
  if(count==0)
    has_read=0;    //当读取到文件末尾时，将has_read设为0
    
  return count;
}



/**
创建模块的相关操作的结构体
设置读取函数，其他未设置的结构体成员都将被初始化为NULL
*/
struct file_operations proc_fops = {
  read: read_proc
};

/* -------------------- 注册模块/注销模块 -------------------- */

//模块装载时执行的代码
int proc_init (void) {

  //分配一块内存。秒10位，纳秒9位，空格1位
  msg=(char*)kmalloc(LEN*sizeof(char),GFP_KERNEL);
  if(msg==NULL){
     printk(KERN_INFO "分配内存失败");
     return 0;
   }
  len=LEN;
  has_read=0;
  //创建proc入口
  proc_create("clock",0,NULL,&proc_fops);
  /**
  创建一个模块
  name:模块入口名
  mode:访问模式
  parent:在/proc目录下的父路径，设为NULL即以/proc为父路径
  proc_fops:文件的相关操作在这个结构中创建
  */
  return 0;
}

//模块卸载时执行的代码
void proc_cleanup(void) {
  remove_proc_entry("clock",NULL);
  kfree(msg);
  msg=NULL;
}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);

