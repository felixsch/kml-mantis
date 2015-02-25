#ifndef __MANTIS_HIJACK_H
#define __MANTIS_HIJACK_H 1


#define JUMP_SIZE 12
#define JUMP_CODE "\x48\xb8\x00\x00\x00\x00\x00\x00\x00\x00\xff\xe0"
#define JUMP_OFFSET 2


typedef struct ksym_hook_t_ {

    void *addr;
    
    unsigned char original[JUMP_SIZE];
    unsigned char hooked[JUMP_SIZE];

    bool is_hooked;


} ksym_hook_t;


ksym_hook_t *ksym_hook_init(void *target, void *hook);

void ksym_hook(ksym_hook_t *hook);
void ksym_unhook(ksym_hook_t *hook);

void *ksym_hook_addr(ksym_hook_t *hook);

#endif

