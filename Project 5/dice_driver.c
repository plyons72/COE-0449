//Patrick Lyons
//CoE 0449
//Final Project
//April 23, 2017

//Altered from hello_dev.c
//Changed "hello" to "dice"
//Also added a dice roller

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/random.h>

#include <asm/uaccess.h>

unsigned char get_random_byte(int max);

static ssize_t dice_read(struct file * file, char * buf,
                          size_t count, loff_t *ppos)
{
        //Initializes i to 6 (max number from die roll)
        int max = 6;

        //Gets a random byte between 0 and 5
        int dice_roll;

				dice_roll = (int)get_random_byte(max);



        if (count < sizeof(int))
                return -EINVAL;

        if (*ppos != 0)
                return 0;


        if (copy_to_user(buf, &dice_roll, count))
                return -EINVAL;


        *ppos = sizeof(int);

        return count;
}


static const struct file_operations dice_fops = {
        .owner          = THIS_MODULE,
        .read           = dice_read,
};

static struct miscdevice dice_dev = {
        MISC_DYNAMIC_MINOR,
        "dice",
        /*
         * What functions to call when a program performs file
         * operations on the device.
         */
        &dice_fops
};

static int __init
dice_init(void)
{
        int ret;
        ret = misc_register(&dice_dev);
        if (ret)
                printk(KERN_ERR
                       "Unable to register \"dice\" misc device\n");

        return ret;
}

unsigned char get_random_byte(int max)
{
	unsigned char c;
	get_random_bytes(&c, 1);
	return c%max + 1;
}

module_init(dice_init);

static void __exit
dice_exit(void)
{
        misc_deregister(&dice_dev);
}

module_exit(dice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Patrick Lyons <phl7@pitt.edu>");
MODULE_DESCRIPTION("\"Dice\"");
MODULE_VERSION("dev");
