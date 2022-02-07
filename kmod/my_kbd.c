#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i8042.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>

#include "my_kbd.h"

#define NAME "kbl"
struct class *cl;
dev_t mydev;
bool hasDev;

void my_lightnow(bool l1, bool l2, bool l3);
//ioctl_d_interface_dev ioctl_d_interface;
struct cdev my_cdev;
int my_dev_major;

int my_open(struct inode* inode, struct file *filp);
long int my_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
int my_close(struct inode* inode, struct file *filp);

struct file_operations my_fops = {
	owner: THIS_MODULE,
	unlocked_ioctl: my_ioctl,
};

int count = 1;
void cleanup_module(void) {
	pr_info("cleanup..");
	
	if (hasDev)
		device_destroy(cl, mydev);
	unregister_chrdev(my_dev_major, "lght");
	if (cl)
		class_destroy(cl);
	pr_info("cleanuped!");
}

int init_module(void) {
	
	/*
	pr_info("inited?");
	int i = 0;
	static char anim[] = {
		0b100,
		0b010,
		0b001,
		0b000,
		0b001,
		0b010,
		0b100,
		0b000,
		0b100,
		0b110,
		0b111,
		0b011,
		0b001,
		0b000,
		0b001,
		0b011,
		0b111,
		0b110,
		0b011,
		0b111,
		0b101,
		0b111,
		0b000,
		0b101,
		0b111,
		0b010,
		0b000,
		0b111,
		0b000,
		0b111,
		0b000,
	};
	do {
		my_lightnow((anim[i] & 0b100), (anim[i] & 0b010), (anim[i] & 0b001));
		mdelay(150);
		i++;
	} while (i < 8) ; //31) ;
	*/
	int error;
	
	
	pr_info("init...");
	my_dev_major = register_chrdev(0, NAME, &my_fops);
	if (my_dev_major < 0) {		
		pr_err("register_chrdev error=%d", my_dev_major);
		goto errored;
	}
	mydev = MKDEV(my_dev_major, 0);
	hasDev = 1;

	cl = class_create(THIS_MODULE, NAME);
	if ((error = device_create(cl, NULL, mydev, NULL, NAME)) < 0) {		
		pr_err("device_create error=%d", error);
		goto errored;
	}
	
	pr_info("end init");
	return 0;
errored:
	cleanup_module();
	return error;
}

void my_lightnow(bool l1, bool l2, bool l3) {
	unsigned char data[2] = {0xED, (l2 ? 0b100 : 0) | (l1 ? 0b010 : 0) | (l3 ? 0b001 : 0) };
	int command = 0xED | 2 /*кол-во аргументов*/ << 12 | 0 /*кол-во параметров на возврат, (поидеи, один: ACK или NCK)*/ << 8;
	/**
	 * Функция используется фактически для вызова
	 * i8042_write(0xED)
	 * i8042_write(anim)
	 * А без command = 0xED - работать не будет(
	 */
	i8042_command(data, command); //pr_info("cmd args=%d, params=%d, ret=%d data=%d", ((command >> 12) & 0xf),((command >> 8) & 0xf), i8042_command(data, command), data[0]);
}

int my_open(struct inode* inode, struct file *filp) {
	pr_info("opened, no checks, trust me");
	return 0;
}
int my_close(struct inode* inode, struct file *filp) {
	pr_info("closed, no checks, trust me");
	return 0;
}
long int my_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	switch (cmd) {
		case 123:
			pr_info("ioctl(%d)", cmd);
			struct lightnow args;
			int error;
			if ((error = copy_from_user(&args, (struct lightnow*) arg, sizeof(struct lightnow))) != 0) {
				pr_err("copy err=%d", error);
				return -EFAULT;
			}
			pr_info("args l1=%d, l2=%d, l3=%d", args.l1, args.l2, args.l3);
			my_lightnow(args.l1, args.l2, args.l3);
			pr_info("ioctl end");
			return 0;
		break;
	}
	return -EFAULT;
}


//subsys_initcall(mykbd_init);
//module_exit(mykbd_exit);

MODULE_DESCRIPTION("my kbd module");
MODULE_LICENSE("GPL");
