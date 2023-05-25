#include "main.h"

/**
 * add_node - Adds a node to the beginning of the list.
 * @head_ptr: Address of the pointer to the head node.
 * @str: String field of the node.
 * @num: Node index used for history.
 *
 * Return: Size of the list.
 */
list_t *add_node(list_t **head_ptr, const char *str, int num)
{
	list_t *new_head;

	if (!head_ptr)
		return (NULL);

	new_head = malloc(sizeof(list_t));
	if (!new_head)
		return (NULL);

	_memset((void *)new_head, 0, sizeof(list_t));
	new_head->num = num;

	if (str)
	{
		new_head->str = _strdup(str);
		if (!new_head->str)
		{
			free(new_head);
			return (NULL);
		}
	}

	new_head->next = *head_ptr;
	*head_ptr = new_head;

	return (new_head);
}

/**
 * add_node_end - Adds a node to the end of the list.
 * @head_ptr: Address of the pointer to the head node.
 * @str: String field of the node.
 * @num: Node index used for history.
 *
 * Return: Size of the list.
 */
list_t *add_node_end(list_t **head_ptr, const char *str, int num)
{
	list_t *new_node, *node;

	if (!head_ptr)
		return (NULL);

	node = *head_ptr;
	new_node = malloc(sizeof(list_t));
	if (!new_node)
		return (NULL);

	_memset((void *)new_node, 0, sizeof(list_t));
	new_node->num = num;

	if (str)
	{
		new_node->str = _strdup(str);
		if (!new_node->str)
		{
			free(new_node);
			return (NULL);
		}
	}

	if (node)
	{
		while (node->next)
			node = node->next;

		node->next = new_node;
	}
	else
		*head_ptr = new_node;

	return (new_node);
}

/**
 * print_list_str - Prints only the 'str' element of a list_t linked list.
 * @head: Pointer to the first node.
 *
 * Return: Size of the list.
 */
size_t print_list_str(const list_t *head)
{
	size_t count = 0;

	while (head)
	{
		_puts(head->str ? head->str : "(nil)");
		_puts("\n");
		head = head->next;
		count++;
	}

	return (count);
}

/**
 * delete_node_at_index - Deletes a node at the given index.
 * @head_ptr: Address of the pointer to the first node.
 * @index: Index of the node to delete.
 *
 * Return: 1 on success, 0 on failure.
 */
int delete_node_at_index(list_t **head_ptr, unsigned int index)
{
	list_t *node, *prev_node;
	unsigned int i = 0;

	if (!head_ptr || !*head_ptr)
		return (0);

	if (!index)
	{
		node = *head_ptr;
		*head_ptr = (*head_ptr)->next;
		free(node->str);
		free(node);
		return (1);
	}

	node = *head_ptr;
	while (node)
	{
		if (i == index)
		{
			prev_node->next = node->next;
			free(node->str);
			free(node);
			return (1);
		}

		i++;
		prev_node = node;
		node = node->next;
	}

	return (0);
}

/**
 * free_list - Frees all nodes of a list.
 * @head_ptr: Address of the pointer to the head node.
 *
 * Return: void
 */
void free_list(list_t **head_ptr)
{
	list_t *node, *next_node, *head;

	if (!head_ptr || !*head_ptr)
		return;

	head = *head_ptr;
	node = head;
	while (node)
	{
		next_node = node->next;
		free(node->str);
		free(node);
		node = next_node;
	}
	*head_ptr = NULL;
}
