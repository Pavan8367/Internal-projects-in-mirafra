#include<linux/kernel.h>
#include<linux/slab.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/errno.h>

static char *buffer = NULL;

static int __init  my_kmalloc_init(void)
{
	pr_info("Kmalloc Module : Intializing and allocating memory\n");
	buffer = kmalloc(256,GFP_KERNEL);
	if(!buffer)
	{
		pr_err("Kmalloc_module : memory allocation failed\n");
		return -ENOMEM;
	}
	else
	{
		pr_info("Kmalloc_module : Memory intialized successfully\n");
	}
	return 0;
}

static void __exit my_kmalloc_exit(void)
{
	pr_info("Kmalloc Module : exiting and freeing memory\n");
	if(buffer)
	{
		kfree(buffer);
		buffer = NULL;
		pr_info("kmalloc Module : Memory freed successfully\n");
	}
}

module_init(my_kmalloc_init);
module_exit(my_kmalloc_exit);

MODULE_AUTHOR("PAVAN");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("kernel kmalloc module");

