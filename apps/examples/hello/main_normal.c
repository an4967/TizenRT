#include <stdio.h>
#include <stdlib.h>
// #include <malloc.h>

#define ERR printf("error %s:%d\t%s\n", __FILE__, __LINE__, __FUNCTION__);

struct json_object {
	int a;
	int b;
};

typedef struct json_object *json_node;

static json_node *create_json(void)
{
	printf("[IN] T%d --> %s\n", getpid(), __func__);
	usleep(20000);

	json_node *node = (json_node *)malloc(sizeof(json_node));
	if (!node) {
		ERR;
		return NULL;
	}

	struct json_object *obj = (struct json_object *)malloc(sizeof(struct json_object));
	if (!obj) {
		ERR;
		return NULL;
	}
	obj->a = 10;
	obj->b = 15;
	*node = obj;
	printf("node %p inner %p %p\n", node, obj, *node);
	usleep(20000);
	printf("[OUT] T%d <-- %s\n", getpid(), __func__);
	usleep(20000);
	return node;
}


static int delete_json(json_node *node)
{
	printf("[IN] T%d --> %s\n", getpid(), __func__);
	usleep(20000);
	if (!node) {
		printf("node is null\n");
		return -1;
	}
	if (*node){
		printf("obj is not null\n");
		free(*node);
		*node = NULL;
		free(node);
	}

	printf("[OUT] T%d <--> %s\n", getpid(), __func__);
	usleep(20000);
	return 0;
}

#define delete_json3(a) __delete_json3(&a)

static int __delete_json3(json_node **node) {
	*node = NULL;
	return 0;
}

static int delete_json2(json_node *node)
{
	printf("[IN] T%d --> %s\n", getpid(), __func__);
	usleep(20000);
	if (!node) {
		printf("node is null\n");
		return -1;
	}
	struct json_object *obj = *node;
	if (obj){
		printf("obj is not null\n");
		free(obj);
		*node = NULL;
		free(node);
	}
	
	printf("[OUT] T%d <-- %s\n", getpid(), __func__);	
	usleep(20000);
	return 0;
}

static void print_json(json_node *node)
{
	if (node == NULL || *node == NULL) return;

	struct json_object *obj = (struct json_object *)*node;
	printf("Value: %d %d\n", obj->a, obj->b);
	return;
}

json_node *g_node;

int
normal_main(void)
{
	// json_node *node;

	g_node = create_json();
	// if (!node) {
	// 	printf("fail to create node\n");
	// 	return -1;
	// }

	printf("node1 : %p\n", g_node);

	delete_json3(g_node);

	printf("node2 : %p\n", g_node);


// 	if (node != NULL) {
// 		print_json(node);
// 	}

// 	int res = delete_json(node);
// 	if (res < 0) {
// 		printf("fail to delete node\n");
// 		ERR;
// 		return -1;
// 	}

// 	if (node != NULL) {
// 		print_json(node);
// 	}
	
// 	if (node != NULL) {
// 		res = delete_json2(node);
// 	}

// 	if (node != NULL) {
// 		print_json(node);
// 	}


// 	if (res < 0) {
// 		printf("fail to delete node again\n");
// 		ERR;
// 		return -1;
// 	}

// #if 0

// 	int *a = (int *)malloc(sizeof(int));
// 	*a = 3;
// 	printf(" free 1st %d\n", *a);
// 	free(a);
// 	printf(" free 2nd\n");
// 	printf("Value %d\n", *a);

// 	free(a);
// #endif
	return 0;

}
