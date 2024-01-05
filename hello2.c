// SPDX-License-Identifier: GPL-2-Clause
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include "hello1.h"

MODULE_AUTHOR("Liza Martynyuk");
MODULE_DESCRIPTION("Laba4");
MODULE_LICENSE("Dual BSD/GPL");

static uint repeat = 1;
module_param(repeat, uint, 0444);
MODULE_PARM_DESC(repeat, "Repeat printing Hello, World");


static int __init hello_init(void)
{
	if (repeat == 0 || (repeat >= 5 && repeat <= 10)) {
		pr_warn("Warning: repeating is: %d\n", repeat);
	} else if (repeat > 10) {
		pr_err("Error: More than 10");
		return -EINVAL;
	}
	print_hello(repeat);
	return 0;
}

static void __exit hello_exit(void)
{
	pr_info("Bye\n");
}

module_init(hello_init);
module_exit(hello_exit);
