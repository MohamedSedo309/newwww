#include "shell.h"

/**
 * input_buf - buffers chained commands
 * @info: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t input_buf(infooo *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*free_buff((void **)info->cmd_buffer);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, ctrl_c_handler);
#if USE_GETLINE
		r = getline(buf, &len_p, stdin);
#else
		r = line_getter(info, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			info->conutline_flag = 1;
			remove_comment(*buf);
			hiss_list_builder(info, *buf, info->histcounter++);
			/* if (find_char(*buf, ';')) is this a command chain? */
			{
				*len = r;
				info->cmd_buffer = buf;
			}
		}
	}
	return (r);
}

/**
 * input_getter - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t input_getter(infooo *info)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(info->arg), *p;

	_putchar(BUF_FLUSH);
	r = input_buf(info, &buf, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len) /* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buf position */
		p = buf + i; /* get pointer for return */

		check_if_chain(info, buf, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (is_a_chain(info, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			info->cmd_buffer_type = CMD_NORM;
		}

		*buf_p = p; /* pass back pointer to current command position */
		return (get_string_length(p)); /* return length of current command */
	}

	*buf_p = buf; /* else not a chain, pass back buffer from line_getter() */
	return (r); /* return length of buffer from line_getter() */
}

/**
 * read_buf - reads a buffer
 * @info: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: r
 */
ssize_t read_buf(infooo *info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(info->readfile, buf, READ_BUF_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * line_getter - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int line_getter(infooo *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buf(info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = find_char(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc_mem(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		concatenate_strings2(new_p, buf + i, k - i);
	else
		copy_string2(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * ctrl_c_handler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void ctrl_c_handler(__attribute__((unused))int sig_num)
{
	print_string("\n");
	print_string("$ ");
	_putchar(BUF_FLUSH);
}
