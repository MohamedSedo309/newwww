#include "shell.h"

/**
 * is_a_chain - test if current char in buffer is a chain delimeter
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int is_a_chain(infooo *info, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		info->cmd_buffer_type = CMD_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		info->cmd_buffer_type = CMD_AND;
	}
	else if (buf[j] == ';') /* found end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		info->cmd_buffer_type = CMD_CHAIN;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * check_if_chain - checks we should continue chaining based on last status
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void check_if_chain(infooo *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (info->cmd_buffer_type == CMD_AND)
	{
		if (info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (info->cmd_buffer_type == CMD_OR)
	{
		if (!info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * aliasreplace - replaces an aliases in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int aliasreplace(infooo *info)
{
	int i;
	list_s *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = get_node_starter(info->alias, info->argv[0], '=');
		if (!node)
			return (0);
		free(info->argv[0]);
		p = find_char(node->str, '=');
		if (!p)
			return (0);
		p = duplicate_string(p + 1);
		if (!p)
			return (0);
		info->argv[0] = p;
	}
	return (1);
}

/**
 * var_replace - replaces vars in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int var_replace(infooo *info)
{
	int i = 0;
	list_s *node;

	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;

		if (!compare_strings(info->argv[i], "$?"))
		{
			replace_strrr(&(info->argv[i]),
					duplicate_string(conv_num(info->status, 10, 0)));
			continue;
		}
		if (!compare_strings(info->argv[i], "$$"))
		{
			replace_strrr(&(info->argv[i]),
					duplicate_string(conv_num(getpid(), 10, 0)));
			continue;
		}
		node = get_node_starter(info->env, &info->argv[i][1], '=');
		if (node)
		{
			replace_strrr(&(info->argv[i]),
					duplicate_string(find_char(node->str, '=') + 1));
			continue;
		}
		replace_strrr(&info->argv[i], duplicate_string(""));

	}
	return (0);
}

/**
 * replace_strrr - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_strrr(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
