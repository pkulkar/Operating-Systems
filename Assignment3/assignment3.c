#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/time.h>
#include <linux/semaphore.h>
#include <linux/vmalloc.h>
#include <linux/device.h>
#include <linux/fs.h>

#define PIPE_NAME "numberpipe"
#define MAX 100

static char** val; 
static int majorNum;
static struct class* CClass = NULL;
static struct device* CDevice = NULL;
static struct semaphore empty, full, mutex;
static int Empty, FULL; 
static int buffer = MAX;

module_param(buffer, int, 0);

static ssize_t deviceread(struct file *f, char *b, size_t len, loff_t *offset);
static ssize_t devicewrite(struct file *f, const char *b, size_t len, loff_t *offset);
void INC(int *);

static struct file_operations fops = 
{
	.read = deviceread,
	.write = devicewrite
};

static ssize_t devicewrite(struct file *f, const char *b, size_t len, loff_t *offset)
{
	int result = 0, size;

	if(down_interruptible(&empty))
	{
		return -EFAULT;
	}
	down(&mutex);
	val[Empty] = (char*) vmalloc(len);
	result = copy_from_user(val[Empty], b, len);
	size = strlen(val[Empty]);

	if(result == 0)
	{
		
		INC(&Empty);
		up(&mutex);
		up(&full);

		return size;
	} else 
	{
		up(&mutex);
		up(&empty); 

		return -EFAULT;
	}
}

static ssize_t deviceread(struct file *f, char *b, size_t len, loff_t *offset)
{
	int result = 0, size;

	if(down_interruptible(&full))
	{
		return -EFAULT;
	}
	down(&mutex);
	
	result = copy_to_user(b, val[FULL], len);
	size = strlen(val[FULL]);
	vfree(val[FULL]);

	if(result == 0)
	{
		
		INC(&FULL);
		up(&mutex);
		up(&empty);

		
		return size;
	} else 
	{
		up(&mutex);
		up(&full); 
		return -EFAULT;
	}
}

static int __init start(void)
{
	majorNum = register_chrdev(0, PIPE_NAME, &fops);
	if(majorNum<0)
	{
		printk(KERN_ALERT "numpipe failed to register\n");
		return majorNum;
	}
	printk(KERN_ALERT "numpipe registered majorNum number: %d\n", majorNum);

	CClass = class_create(THIS_MODULE, "Class");
	if(IS_ERR(CClass))
	{
		unregister_chrdev(majorNum, PIPE_NAME);
		return PTR_ERR(CClass);
	}
	CDevice = device_create(CClass, NULL, MKDEV(majorNum, 0), NULL, PIPE_NAME);
	if(IS_ERR(CDevice))
	{
		class_destroy(CClass);
		unregister_chrdev(majorNum, PIPE_NAME);
		return PTR_ERR(CDevice);
	}
	sema_init(&empty, buffer);
	sema_init(&full, 0);
	sema_init(&mutex, 1);

	Empty = 0;
	FULL = 0;


	val = (char**) vmalloc(buffer * sizeof(char*));

	return 0;
}


void INC(int *i)
{
	if( *i==(buffer-1) )
	{
		*i = 0;
	} else
	{
		*i = *i + 1;	
	} 
}


static void __exit end(void)
{
	device_destroy(CClass, MKDEV(majorNum,0));
	class_unregister(CClass);
	unregister_chrdev(majorNum, PIPE_NAME);
	printk(KERN_INFO "Unregistered Sucessfully");
}

module_init(start);
MODULE_LICENSE("GPL");
module_exit(end);


