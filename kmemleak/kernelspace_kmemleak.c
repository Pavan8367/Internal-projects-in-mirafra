#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pavan");
MODULE_DESCRIPTION("A simple Linux kernel module demonstrating kmalloc");

static int *buffer = NULL;

static int  my_kmalloc_init(void)

{
    pr_info("Kmalloc Module: Initializing and allocating memory\n");

    /* Allocate memory from the kernel heap */
    buffer = kmalloc(256, GFP_KERNEL);

    /* Check if allocation succeeded */
    if (!buffer) {
        pr_err("Kmalloc Module: Memory allocation failed!\n");
        return -ENOMEM;
    }
    return 0;
}

static void __exit my_kmalloc_exit(void)
{
   /* pr_info("Kmalloc Module: Exiting and freeing memory\n");

    if (buffer) {
        kfree(buffer);
        buffer = NULL;
        pr_info("Kmalloc Module: Memory successfully freed\n");
    }*/
}

module_init(my_kmalloc_init);
module_exit(my_kmalloc_exit);
