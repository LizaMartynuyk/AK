// SPDX-License-Identifier: GPL-2-Clause
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_AUTHOR("Liza Martynyuk");
MODULE_DESCRIPTION("Laba5");
MODULE_LICENSE("Dual BSD/GPL");

static uint repeat = 1;
module_param(repeat, uint, 0444);
MODULE_PARM_DESC(repeat, "Repeat printing Hello, World!");

struct time {
	struct list_head tlist;
	ktime_t time;
};

static LIST_HEAD(list);

int i = 0;

static int __init hello_init(void)
{
	if (repeat == 0 || (repeat >= 5 && repeat <= 10)) {
		printk(KERN_WARNING "Warning: repeating is %d\n", repeat);
	}

	BUG_ON(repeat > 10);

	for (i = 0; i < repeat; i++) {
		struct time *tst = kmalloc(sizeof(*tst), GFP_KERNEL);

		if (i == repeat - 1) tst = NULL;

		if (tst != NULL) {
			tst->time = ktime_get();
			list_add_tail(&tst->tlist, &list);
			printk(KERN_INFO "Hello, world!\n");
		}
		else {
			struct time *cur, *tmp;
        		list_for_each_entry_safe(cur, tmp, &list, tlist) {
	                list_del(&cur->tlist);
        		kfree(cur);

			BUG();
        		}
		}
	}
	return 0;
}

static void __exit hello_exit(void)
{
	int i = 0;
	struct time *cur, *tmp;

	list_for_each_entry_safe(cur, tmp, &list, tlist) {
		long long nanos = ktime_to_ns(cur->time);

		printk(KERN_INFO "%d time = %lld\n", i++, nanos);
		list_del(&cur->tlist);
		kfree(cur);
	}
	printk(KERN_INFO "Bye\n");
}

module_init(hello_init);
module_exit(hello_exit);
