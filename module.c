#include <linux/module.h>
#include <linux/init.h>
#include <linux/kallsyms.h>
#include <linux/utsname.h>

#include "mantis.h"
#include "hijack.h"


MODULE_LICENSE("GPL");

ksym_hook_t* hook_uname;


void hexdump(unsigned char *mem, unsigned int length) {

    int i;

    printk(KERN_INFO "printing %d bytes from addr 0x%16lX\n", length, (unsigned long)mem);
    for (i=0; i < length; i++) {
        if ((i+1) % 8) {
            printk("%02x ", mem[i]);
        } else {
            printk("%02x\n", mem[i]);
        }
    }
    printk("\n");
}


asmlinkage long sys_newuname(struct new_utsname __user *name) {
    long ret;
    long (*call)(struct new_utsname __user*);

    call = (long (*)(struct new_utsname __user*))hook_uname->addr;

    ksym_unhook(hook_uname);

    ret = call(name);

    strncpy(name->sysname, "hoooked!", 8);

    ksym_hook(hook_uname);

    return ret;
}





static int __init bd_init(void) {

    void *addr_uname;


    addr_uname = (void*)kallsyms_lookup_name("sys_newuname");

    if (!addr_uname) {
        printk(KERN_INFO "Could not find uname...\n");
        return -1;
    }

    hook_uname = ksym_hook_init(addr_uname, (void*)sys_newuname);


    ksym_hook(hook_uname);
    
    
  return 0;
}

static void __exit bd_release(void) {

    ksym_unhook(hook_uname);

}


module_init(bd_init);
module_exit(bd_release);
