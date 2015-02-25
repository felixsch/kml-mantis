
#include <linux/slab.h>
#include <linux/preempt.h>

#include "hijack.h"
#include "mantis.h"




inline unsigned long disable_wp(void)
{
    unsigned long cr0;

    preempt_disable();
    barrier();

    cr0 = read_cr0();
    write_cr0(cr0 & ~X86_CR0_WP);

    return cr0;
}

inline void restore_wp(unsigned long cr0)
{
    write_cr0(cr0);

    barrier();
    preempt_enable();
}




ksym_hook_t *ksym_hook_init(void *target, void* hook) {

    ksym_hook_t *h;

    if (!(h = kmalloc(sizeof(h), GFP_KERNEL))) {

        DEBUG("Could not alloc new ksym_hook_t\n");
        return NULL;
    }

    memcpy(h->hooked, JUMP_CODE, JUMP_SIZE);
    memcpy(h->original, target, JUMP_SIZE);

    *(unsigned long*)&h->hooked[JUMP_OFFSET] = (unsigned long)hook;

    h->addr = target;
    h->is_hooked = false;
    
    return h;
}


void ksym_hook(ksym_hook_t *hook) {

    unsigned long cr0;

    if (!hook || hook->is_hooked) {
        return;
    }

    cr0 = disable_wp();

    memcpy(hook->addr, hook->hooked, JUMP_SIZE);

    restore_wp(cr0);

    hook->is_hooked = true;
}


void ksym_unhook(ksym_hook_t *hook) {

    unsigned long cr0;

    if (!hook || !hook->is_hooked) {
        return;    
    }

    cr0 = disable_wp();

    memcpy(hook->addr, hook->original, JUMP_SIZE);

    restore_wp(cr0);

    hook->is_hooked = false;
}


void *ksym_get_addr(ksym_hook_t *hook) {

    return hook->addr;
}





