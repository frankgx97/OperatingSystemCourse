#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
 
#define LEN 21

int len,has_read;
char *msg;

int read_proc(struct file *filp,char *buf,size_t count,loff_t *offp ) {

  struct timespec time_ckt;
  time_ckt=current_kernel_time();
  
  unsigned long p=time_ckt.tv_sec;
  int len_sec=0;
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

  msg[len_sec]=' ';
  
  p=time_ckt.tv_nsec;
  int len_nsec=0;
  while(p/=10){
    len_nsec++;
  }
  len_nsec++;
  
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
  
  msg[len]='\0';
  
  if(count+has_read>=len){
    count=len-has_read;
  }
  
  has_read=has_read+count;
  copy_to_user(buf, msg, count);
  
  if(count==0)
    has_read=0;
    
  return count;
}

struct file_operations proc_fops = {
  read: read_proc
};

//装载模块
int proc_init (void) {
  msg=(char*)kmalloc(LEN*sizeof(char),GFP_KERNEL);
  if(msg==NULL){
     printk(KERN_INFO "分配内存失败");
     return 0;
   }
  len=LEN;
  has_read=0;
  proc_create("clock",0,NULL,&proc_fops);
  return 0;
}

//模块卸载
void proc_cleanup(void) {
  remove_proc_entry("clock",NULL);
  kfree(msg);
  msg=NULL;
}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);