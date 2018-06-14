#include <minix/drivers.h>
#include <minix/chardriver.h>
#include <stdio.h>
#include <stdlib.h>
#include <minix/ds.h>
#include "hello_stack.h"

/*
 * Function prototypes for the hello_stack driver.
 */
static int hello_stack_open(devminor_t minor, int access, endpoint_t user_endpt);
static int hello_stack_close(devminor_t minor);
static ssize_t hello_stack_read(devminor_t minor, u64_t position, endpoint_t endpt,
    cp_grant_id_t grant, size_t size, int flags, cdev_id_t id);
static ssize_t hello_stack_write(devminor_t minor, u64_t position, endpoint_t endpt,
    cp_grant_id_t grant, size_t size, int flags, cdev_id_t id);

/* SEF functions and variables. */
static void sef_local_startup(void);
static int sef_cb_init(int type, sef_init_info_t *info);
static int sef_cb_lu_state_save(int);
static int lu_state_restore(void);

/* Entry points to the hello driver. */
static struct chardriver hello_stack_tab = {
    .cdr_open   = hello_stack_open,
    .cdr_close  = hello_stack_close,
    .cdr_read   = hello_stack_read,
    .cdr_write  = hello_stack_write,
};

/* char pointer representing hello_stack */
static char* hello_stack;
/* char pointer pointing to the top of the stack */
static char* stack_top;
/* int representing number of elements currently on stack */
static int stack_count;
/* int representing size of stack */
static int stack_size;

static int hello_stack_open(devminor_t minor, int access, endpoint_t user_endpt) {
    return OK;
}

static int hello_stack_close(devminor_t minor) {
    return OK;
}

static ssize_t hello_stack_read(devminor_t minor, u64_t UNUSED(position), endpoint_t endpt,
    cp_grant_id_t grant, size_t size, int flags, cdev_id_t id) {
    
    char *ptr;
    int ret;

    //TODO
}

static ssize_t hello_stack_write(devminor_t minor, u64_t UNUSED(position), endpoint_t endpt,
    cp_grant_id_t grant, size_t size, int flags, cdev_id_t id) {

    // TODO
}

/* Save the state. */
static int sef_cb_lu_state_save(int UNUSED(state)) {
    ds_publish_u32("hello_stack_size", stack_size, DSF_OVERWRITE);
    ds_publish_u32("hello_stack_count", stack_count, DSF_OVERWRITE);
    ds_publish_mem("hello_stack", (void*) hello_stack, stack_size, DSF_OVERWRITE);

    /* TODO free(hello_stack)? */

    return OK;
}

/* Restore the state. */
static int lu_state_restore() {
    int s_size, s_count;

    ds_retrieve_u32("hello_stack_size", &s_size);
    ds_delete_u32("hello_stack_size");
    stack_size = s_size;

    ds_retrieve_u32("hello_stack_count", &s_count);
    ds_delete_u32("hello_stack_count");
    stack_count = s_count;

    hello_stack = (char*) malloc(stack_size * siezeof(char));
    if (hello_stack == NULL) {
        return ENOMEM;//out of memory
    }

    ds_retrieve_mem("hello_stack", hello_stack, stack_size);
    ds_delete_mem("hello_stack");

    stack_top = hello_stack + stack_count - 1;

    return OK;
}

static void sef_local_startup() {
    /*
     * Register init callbacks. Use the same function for all event types
     */
    sef_setcb_init_fresh(sef_cb_init);
    sef_setcb_init_lu(sef_cb_init);
    sef_setcb_init_restart(sef_cb_init);

    /*
     * Register live update callbacks.
     */
    /* - Agree to update immediately when LU is requested in a valid state. */
    sef_setcb_lu_prepare(sef_cb_lu_prepare_always_ready);
    /* - Support live update starting from any standard state. */
    sef_setcb_lu_state_isvalid(sef_cb_lu_state_isvalid_standard);
    /* - Register a custom routine to save the state. */
    sef_setcb_lu_state_save(sef_cb_lu_state_save);

    /* Let SEF perform startup. */
    sef_startup();
}

static int init_stack() {
    hello_stack = (char*) malloc(DEVICE_SIZE * sizeof(char));
    /* TODO where should I free this? */
    
    /* check if malloc was successful */
    if (hello_stack == NULL) {
        return ENOMEM;//out of memory
    }
    
    stack_size = DEVICE_SIZE;
    stack_count = DEVICE_SIZE;
    stack_top = hello_stack + DEVICE_SIZE - 1;

    /* fill the stack with "abc" string */
    for (int i = 0; i < DEVICE_SIZE; i++) {
        switch(i % 3) {
            case 0:
                hello_stack[i] = 'a';
                break;

            case 1:
                hello_stack[i] = 'b';
                break;

            case 2:
                hello_stack[i] = 'c';
                break;
        }
    }

    return OK;
}

/* Initialize the hello_stack driver. */
static int sef_cb_init(int type, sef_init_info_t *UNUSED(info)) {
    int do_announce_driver = TRUE;

    if (init_stack() != OK) {
        return ENOMEM;
    }
    switch(type) {
        case SEF_INIT_LU:
            /* Restore the state. */
            lu_state_restore();
            do_announce_driver = FALSE;
            break;
    }

    /* Announce we are up when necessary. */
    if (do_announce_driver) {
        chardriver_announce();
    }

    /* Initialization completed successfully. */
    return OK;
}

int main(void)
{
    /*
     * Perform initialization.
     */
    sef_local_startup();

    /*
     * Run the main loop.
     */
    chardriver_task(&hello_stack_tab);
    return OK;
}