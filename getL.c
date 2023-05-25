#include "main.h"

/**
 * input_buf - buffers chained commands
 * @info: parameter struct
 * @buffer: address of buffer
 * @length: address of length var
 *
 * Return: bytes read
 */
ssize_t input_buf(info_t *info, char **buffer, size_t *length)
{
	ssize_t bytesRead = 0;
	size_t length_p = 0;

	if (!*length) /* If nothing left in the buffer, fill it */
	{
		/*bfree((void **)info->cmd_buf);*/
		free(*buffer);
		*buffer = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		bytesRead = getline(buffer, &length_p, stdin);
#else
		bytesRead = _getline(info, buffer, &length_p);
#endif
		if (bytesRead > 0)
		{
			if ((*buffer)[bytesRead - 1] == '\n')
			{
				(*buffer)[bytesRead - 1] = '\0'; /* Remove trailing newline */
				bytesRead--;
			}
			info->linecount_flag = 1;
			remove_comments(*buffer);
			build_history_list(info, *buffer, info->histcount++);
			/* if (_strchr(*buffer, ';')) is this a command chain? */
			{
				*length = bytesRead;
				info->cmd_buf = buffer;
			}
		}
	}
	return (bytesRead);
}

/**
 * get_input - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t get_input(info_t *info)
{
	static char *buffer; /* The ';' command chain buffer */
	static size_t i, j, length;
	ssize_t bytesRead = 0;
	char **buffer_p = &(info->arg), *p;

	_putchar(BUF_FLUSH);
	bytesRead = input_buf(info, &buffer, &length);
	if (bytesRead == -1) /* EOF */
		return (-1);
	if (length)	/* We have commands left in the chain buffer */
	{
		j = i; /* Init new iterator to current buf position */
		p = buffer + i; /* Get pointer for return */

		check_chain(info, buffer, &j, i, length);
		while (j < length) /* Iterate to semicolon or end */
		{
			if (is_chain(info, buffer, &j))
				break;
			j++;
		}

		i = j + 1; /* Increment past nulled ';'' */
		if (i >= length) /* Reached end of buffer? */
		{
			i = length = 0; /* Reset position and length */
			info->cmd_buf_type = CMD_NORM;
		}

		*buffer_p = p; /* Pass back pointer to current command position */
		return (_strlen(p)); /* Return length of current command */
	}

	*buffer_p = buffer; /* Else not a chain, pass back buffer from _getline() */
	return (bytesRead); /* Return length of buffer from _getline() */
}

/**
 * read_buf - reads a buffer
 * @info: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: bytes read
 */
ssize_t read_buf(info_t *info, char *buf, size_t *i)
{
	ssize_t bytesRead = 0;

	if (*i)
		return (0);
	bytesRead = read(info->readfd, buf, READ_BUF_SIZE);
	if (bytesRead >= 0)
		*i = bytesRead
	return (bytesRead);
}

/**
 * _getline - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: size of the line read
 */
int _getline(info_t *info, char **ptr, size_t *length)
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

	c = _strchr(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(new_p, buf + i, k - i);
	else
		_strncpy(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigintHandler - handles SIGINT signal (Ctrl+C)
 * @sig_num: the signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused)) int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}

