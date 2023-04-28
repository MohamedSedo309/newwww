#include "shell.h"

/**
 * getLength - determines length of linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t getLength(const list_s *h)
{
	size_t i = 0;

	while (h)
	{
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * get_list_str_part - returns an array of strings of the list->str
 * @head: pointer to first node
 *
 * Return: array of strings
 */
char **get_list_str_part(list_s *head)
{
	list_s *node = head;
	size_t i = getLength(head), j;
	char **strs;
	char *str;

	if (!head || !i)
		return (NULL);
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(get_string_length(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = copy_string(str, node->str);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}


/**
 * print_list_s - prints all elements of a list_s linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t print_list_s(const list_s *h)
{
	size_t i = 0;

	while (h)
	{
		print_string(conv_num(h->num, 10, 0));
		_putchar(':');
		_putchar(' ');
		print_string(h->str ? h->str : "(nil)");
		print_string("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * get_node_starter - returns node whose string starts with prefix
 * @node: pointer to list head
 * @prefix: string to match
 * @c: the next character after prefix to match
 *
 * Return: match node or null
 */
list_s *get_node_starter(list_s *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = begain_with(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node);
		node = node->next;
	}
	return (NULL);
}

/**
 * get_node_arrange - gets the index of a node
 * @head: pointer to list head
 * @node: pointer to the node
 *
 * Return: index of node or -1
 */
ssize_t get_node_arrange(list_s *head, list_s *node)
{
	size_t i = 0;

	while (head)
	{
		if (head == node)
			return (i);
		head = head->next;
		i++;
	}
	return (-1);
}
