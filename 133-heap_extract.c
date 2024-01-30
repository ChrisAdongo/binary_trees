#include "binary_trees.h"

#define INIT_NODE_B {0, NULL, NULL, NULL}

#define HEX_DIGITS "0123456789ABCDEF"

#define SETUP_NODE_B { \
	tmp = *tree_root; \
	tree_size = binary_tree_size(*tree_root); \
	binary_representation = &binary_buffer[49]; \
	*binary_representation = 0; \
}

#define FREE_NODE_B { \
		node_value = tmp->n; \
		free(tmp); \
		*tree_root = NULL; \
	}

#define SWAP_HEAD_B { \
		head_node = *tree_root; \
		head_node = swap_head_node(head_node, tmp); \
		node_value = head_node->n; \
		free(head_node); \
		*tree_root = tmp; \
		tmp = percolate_down(tmp); \
		*tree_root = tmp; \
	}

#define CONVERT_LOOP_B { \
		*--binary_representation = HEX_DIGITS[tree_size % 2]; \
		tree_size /= 2; \
	}

bst_t *swap_nodes(bst_t *node_a, bst_t *node_b)
{
	bst_t a_copy = INIT_NODE_B;

	a_copy.n = node_a->n;
	a_copy.parent = node_a->parent;
	a_copy.left = node_a->left;
	a_copy.right = node_a->right;
	node_a->parent = node_b;
	node_a->left = node_b->left;
	node_a->right = node_b->right;
	if (node_b->left)
		node_b->left->parent = node_a;
	if (node_b->right)
		node_b->right->parent = node_a;

	node_b->parent = a_copy.parent;
	if (a_copy.parent)
	{
		if (node_a == a_copy.parent->left)
			a_copy.parent->left = node_b;
		else
			a_copy.parent->right = node_b;
	}
	if (node_b == a_copy.left)
	{
		node_b->left = node_a;
		node_b->right = a_copy.right;
		if (a_copy.right)
			a_copy.right->parent = node_b;
	}
	else if (node_b == a_copy.right)
	{
		node_b->right = node_a;
		node_b->left = a_copy.left;
		if (a_copy.left)
			a_copy.left->parent = node_b;
	}
	while (node_b->parent)
		node_b = node_b->parent;
	return (node_b);
}

size_t binary_tree_size(const binary_tree_t *tree_node)
{
	if (!tree_node)
		return (0);

	return (1 + binary_tree_size(tree_node->left) + binary_tree_size(tree_node->right));
}

heap_t *swap_head_node(heap_t *head_node, heap_t *node)
{
	if (node->parent->left == node)
	{
		node->parent->left = NULL;
	}
	else
		node->parent->right = NULL;
	node->parent = NULL;
	node->left = head_node->left;
	node->right = head_node->right;
	if (head_node->left)
		head_node->left->parent = node;
	if (head_node->right)
		head_node->right->parent = node;
	return (head_node);
}

heap_t *percolate_down(heap_t *node)
{
	int max_value;
	heap_t *next_node = node;

	if (!node)
		return (NULL);
	max_value = node->n;
	if (node->left)
		max_value = MAX(node->left->n, max_value);
	if (node->right)
		max_value = MAX(node->right->n, max_value);
	if (node->left && max_value == node->left->n)
		next_node = node->left;
	else if (node->right && max_value == node->right->n)
		next_node = node->right;
	if (next_node != node)
	{
		swap_nodes(node, next_node);
		percolate_down(node);
	}
	return (next_node);
}

int heap_extract(int **tree_root)
{
	size_t tree_size, i;
	char *binary_representation, c, binary_buffer[50];
	int node_value;
	heap_t *tmp, *head_node;

	if (!tree_root || !*tree_root)
		return (0);
	SETUP_NODE_B;
	if (tree_size == 1)
	{
		FREE_NODE_B;
		return (node_value);
	}
	do {
		CONVERT_LOOP_B;
	} while (tree_size);

	for (i = 1; i < strlen(binary_representation); i++)
	{
		c = binary_representation[i];
		if (i == strlen(binary_representation) - 1)
		{
			if (c == '1')
			{
				tmp = tmp->right;
				break;
			}
			else if (c == '0')
			{
				tmp = tmp->left;
				break;
			}
		}
		if (c == '1')
			tmp = tmp->right;
		else if (c == '0')
			tmp = tmp->left;
	}
	SWAP_HEAD_B;
	return (node_value);
}

