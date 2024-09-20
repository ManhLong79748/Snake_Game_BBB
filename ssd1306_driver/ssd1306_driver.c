#include "ssd1306_lib.h"


/* Probe & remove functions */
static int ssd1306_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int ssd1306_remove( struct i2c_client *client);

static int __init ssd1306_init(void);
static void __exit ssd1306_exit(void);


static int      m_open(struct inode *inode, struct file *file);
static int      m_release(struct inode *inode, struct file *file);
static ssize_t  m_write(struct file *filp, const char *user_buf, size_t size, loff_t * offset);

struct ssd1306_i2c_module *module_ssd;

typedef struct {
    dev_t dev_num;
    struct class *m_class;
    struct cdev m_cdev;
    int size;
    //char *k_buff;
} m_foo_dev;

char kernel_buff[50];

m_foo_dev mdev;

static const struct of_device_id ssd1306_of_match[] = {
    { .compatible = "ssd1306_test" },
    {}
};

static struct i2c_driver ssd1306_driver = {
    .probe = ssd1306_probe,
    .remove = ssd1306_remove,
    .driver = {
        .name = "ssd1306",
        .of_match_table = ssd1306_of_match,
    },
};


static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .write      = m_write,
    .open       = m_open,
    .release    = m_release,
};

static int ssd1306_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	//struct ssd1306_i2c_module *module;

    pr_info("Start probe\n");
	module_ssd = kmalloc(sizeof(*module_ssd), GFP_KERNEL);
	if (!module_ssd) {
		pr_err("kmalloc failed\n");
		return -1;
    }

	module_ssd->client = client;
	module_ssd->line_num = 0;
	module_ssd->cursor_position = 0;
	module_ssd->font_size = SSD1306_DEF_FONT_SIZE;

	ssd1306_display_init(module_ssd);
	ssd1306_set_cursor(module_ssd, 3, 2);
	ssd1306_print_string(module_ssd, "SNAKE GAME BBB");
    ssd1306_set_cursor(module_ssd, 4, 2);
	ssd1306_print_string(module_ssd, "By Hanli !");
	pr_info("Probe function done \n");


	return 0;
}

static int ssd1306_remove(struct i2c_client *client)
{
    pr_info("Starting remove process\n");
    ssd1306_clear_full(module_ssd);
    ssd1306_set_cursor(module_ssd, 3, 0);
    ssd1306_print_string(module_ssd, "Good bye !");
    msleep(1000);
    ssd1306_clear_full(module_ssd);
    ssd1306_write(module_ssd, true, 0xAE); // Entire Display OFF
    kfree(module_ssd);

    pr_info("Remove process done\n");
    return 0;
}


/* This function will be called when we open the Device file */
static int m_open(struct inode *inode, struct file *file)
{
    pr_info("System call open() called...!!!\n");
    return 0;
}

/* This function will be called when we close the Device file */
static int m_release(struct inode *inode, struct file *file)
{
    pr_info("System call close() called...!!!\n");
    return 0;
}


/* This function will be called when we write the Device file */
static ssize_t m_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *offset)
{
    int ret;

    /* Copy the buffer from user */
    ret = copy_from_user(kernel_buff, user_buf, size);
    if (ret)
    {
        pr_err("%s - copy_from_user failed\n", __func__);
        return -EFAULT;
    }

    /* If the string is "clear", clear the display */
    if ( !strncmp("clear", kernel_buff, 5))
    {
        ssd1306_clear_full(module_ssd);
    }
    else if (!strncmp("cursor", kernel_buff, 6)){
      uint8_t x, y;
      char temp[8];
      sscanf(kernel_buff, "%s %hhu %hhu", temp, &x, &y);
      ssd1306_set_cursor(module_ssd, y, x);
    }
    else
    {
        ssd1306_print_string(module_ssd, kernel_buff);
    }

    /* Make the buffer empty */
    memset(kernel_buff, 0, sizeof(kernel_buff));
    
    return size;
}



static int __init ssd1306_init(void)
{
    if (alloc_chrdev_region(&mdev.dev_num, 0, 1, "my-cdevt"))
    {
        pr_err("ERROR: Can not make number device\n");
        return -1;
    }
    pr_info("Init : start\n");
    
    pr_info("Major : %d    Minor : %d\n", MAJOR(mdev.dev_num), MINOR(mdev.dev_num));
    
    if ((mdev.m_class = class_create(THIS_MODULE, "my_classt")) == NULL){
        pr_err("ERROR: Can not create class\n");
        goto rm_dev_num;
    }
    pr_info("Init : initialize class create\n");

    if (device_create(mdev.m_class, NULL, mdev.dev_num, NULL, "my_device_ssd") == NULL)
    {
        pr_err("ERROR: Can not create device\n");
        goto rm_class;

    }
    pr_info("Init : initialize device create\n");

    cdev_init(&mdev.m_cdev, &fops);
    pr_info("Init : initialize  Cdev Init\n");
    if (cdev_add(&mdev.m_cdev, mdev.dev_num, 1) < 0)
    {
        pr_err("ERROR: Can not add device\n");
        goto rm_dev_num;
    }
    pr_info("Init : initialize  Cdev add\n");

    i2c_add_driver(&ssd1306_driver);
    pr_info("init: i2c add driver\n");
    
    pr_info("initialize done\n");

    return 0;

rm_dev_num:
    unregister_chrdev_region(mdev.dev_num,1);
rm_class:
    class_destroy(mdev.m_class);

    return -1;
}

static void __exit ssd1306_exit(void) {
    pr_info("Start exit\n");

    i2c_del_driver(&ssd1306_driver);
    pr_info("Exit: exit i2c_del_driver\n");

    cdev_del(&mdev.m_cdev); 
    pr_info("Exit: exit cdev");

    device_destroy(mdev.m_class, mdev.dev_num);
    pr_info("Exit: exit device_destroy\n");

    class_destroy(mdev.m_class);
    pr_info("Exit: check exit class_destroy\n");


    unregister_chrdev_region(mdev.dev_num, 1);
    pr_info("Exit: check exit number\n");

    pr_info("End exit\n");
}

module_init(ssd1306_init);
module_exit(ssd1306_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("long79748@gmail.com");
MODULE_DESCRIPTION("ssd1306");