#include "shell.h"

/**
 * get_ffhistory - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containg history file
 */

char *get_ffhistory(infooo *info)
{
	char *buf, *dir;

	dir = get_enviroment(info, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (get_string_length(dir) + get_string_length(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	copy_string(buf, dir);
	concatenate_strings(buf, "/");
	concatenate_strings(buf, HIST_FILE);
	return (buf);
}

/**
 * print_hiss - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int print_hiss(infooo *info)
{
	ssize_t fd;
	char *filename = get_ffhistory(info);
	list_s *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->history; node; node = node->next)
	{
		echo_string_fd(node->str, fd);
		echo_fd('\n', fd);
	}
	echo_fd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * get_history_str - reads history from file
 * @info: the parameter struct
 *
 * Return: histcounter on success, 0 otherwise
 */
int get_history_str(infooo *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_ffhistory(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			hiss_list_builder(info, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		hiss_list_builder(info, buf + last, linecount++);
	free(buf);
	info->histcounter = linecount;
	while (info->histcounter-- >= HIST_MAX)
		delete_atindex(&(info->history), 0);
	include_hist(info);
	return (info->histcounter);
}

/**
 * hiss_list_builder - adds entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, histcounter
 *
 * Return: Always 0
 */
int hiss_list_builder(infooo *info, char *buf, int linecount)
{
	list_s *node = NULL;

	if (info->history)
		node = info->history;
	create_end_node(&node, buf, linecount);

	if (!info->history)
		info->history = node;
	return (0);
}

/**
 * include_hist - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcounter
 */
int include_hist(infooo *info)
{
	list_s *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->histcounter = i);
}
