#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define ERR printf("error %s:%d\t%s\n", __FILE__, __LINE__, __FUNCTION__);

struct json_object {
	int a;
	int b;
};

struct _json_node {
	struct json_object *obj;
};

typedef struct _json_node json_node;


json_node *create_json()
{
	json_node *node = (json_node *)malloc(sizeof(json_node));
	if (!node) {
		ERR;
		return NULL;
	}

	struct json_object *obj = (struct json_object *)malloc(sizeof(struct json_object));
	if (!obj) {
		ERR;
		free(node);
		return NULL;
	}

	node->obj = obj;

	printf("node %p inner %p \n", node, node->obj);
	return node;
}


int delete_json(json_node *node)
{
	if (!node) {
		printf("node is null\n");
		return -1;
	}
	if (node->obj){
		printf("delete obj\n");
		free(node->obj);
		node->obj = NULL;
	}
	free(node);
	node = NULL;

	return 0;
}

int delete_json2(json_node *node)
{
	if (!node) {
		printf("node is null\n");
		return -1;
	}
	if (node->obj){
		printf("delete obj\n");
		free(node->obj);
		node->obj = NULL;
	}
	free(node);

	return 0;
}

int
main(void)
{
	json_node *node;

	node = create_json();
	if (!node) {
		printf("fail to create node\n");
		return -1;
	}

	printf("delete first\n");
	int res = delete_json(node);
	if (res < 0) {
		printf("fail to delete node\n");
		ERR;
		return -1;
	}
//	print_json(node);

	printf("delete second\n");
	res = delete_json2(node);
	if (res < 0) {
		printf("fail to delete node again\n");
		ERR;
		return -1;
	}

#if 0

	int *a = (int *)malloc(sizeof(int));
	*a = 3;
	printf(" free 1st %d\n", *a);
	free(a);
	printf(" free 2nd\n");
	printf("Value %d\n", *a);

	free(a);
#endif
	return 0;

}
