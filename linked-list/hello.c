// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

struct identity {
	char name[20];
	int id;
	bool busy;

	struct identity *next;
};

static struct identity start = {
	.id = -INT_MAX,
	.busy = true,
};
static struct identity *head;

static int identity_create(const char *name, int id)
{
	struct identity *temp;

	temp = kmalloc(sizeof(start), GFP_KERNEL);
	if (!temp)
		return -ENOMEM;

	strcpy(temp->name, name);
	temp->id = id;
	temp->busy = false;
	temp->next = NULL;

	head->next = temp;
	head = head->next;
	return 0;
}

static struct identity *identity_find(int id)
{
	struct identity *temp = &start;

	while (temp != NULL) {
		if (temp->id == id)
			return temp;
		temp = temp->next;
	}

	return NULL;
}

static void identity_destroy(int id)
{
	struct identity *temp = &start;
	struct identity *tc;

	while (temp->next != NULL) {
		if (temp->next->id == id) {
			tc = temp->next;
			temp->next = temp->next->next;
			kfree(tc);
			return;
		}
		temp = temp->next;
	}
}

int init_module(void)
{
	char names[][20] = {"Alice", "Bob", "Dave", "Gena"};
	int ids[] = {1, 2, 3, 10};
	struct identity *temp;
	int i;
	int td[] = {2, 1, 10, 42, 3};

	head = &start;
	for (i = 0; i < 4; i++)
		if (identity_create(names[i], ids[i]))
			return -ENOMEM;

	temp = identity_find(3);
	pr_debug("id 3 = %s\n", temp->name);

	temp = identity_find(42);
	if (!temp)
		pr_debug("id 42 not found\n");

	for (i = 0; i < 5; i++)
		identity_destroy(td[i]);

	return 0;
}

void cleanup_module(void)
{
}

