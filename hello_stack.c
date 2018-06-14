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
static struct chardriver hello_stack_tab =
{
    .cdr_open   = hello_stack_open,
    .cdr_close  = hello_stack_close,
    .cdr_read   = hello_stack_read,
    .cdr_write  = hello_stack_write,
};

static int hello_stack_open(devminor_t minor, int access, endpoint_t user_endpt) {
    return OK;
}

static int hello_stack_close(devminor_t minor) {
    return OK;
}

static ssize_t hello_stack_read(devminor_t minor, u64_t position, endpoint_t endpt,
    cp_grant_id_t grant, size_t size, int flags, cdev_id_t id) {
    // TODO
}

static ssize_t hello_stack_write(devminor_t minor, u64_t position, endpoint_t endpt,
    cp_grant_id_t grant, size_t size, int flags, cdev_id_t id) {
    // TODO
}

/* Save the state. */
static int sef_cb_lu_state_save(int UNUSED(state)) {
    // TODO

    return OK;
}

/* Restore the state. */
static int lu_state_restore() {
    //TODO

    return OK;
}

static void sef_local_startup()
{
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

/* Initialize the hello_stack driver. */
static int sef_cb_init(int type, sef_init_info_t *UNUSED(info))
{
    int do_announce_driver = TRUE;

    // TODO initialize stuff
    switch(type) {
        case SEF_INIT_FRESH:
            // TODO
            break;

        case SEF_INIT_LU:
            /* Restore the state. */
            lu_state_restore();
            do_announce_driver = FALSE;
            break;

        case SEF_INIT_RESTART:
            // TODO
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