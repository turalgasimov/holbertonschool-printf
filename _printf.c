#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

/* =======================
 *        STRUCT
 * ======================= */

typedef struct buffer
{
	char *start;
	unsigned int len;
	unsigned int size;
} buffer_t;

/* =======================
 *    BUFFER HELPERS
 * ======================= */

buffer_t *init_buffer(void)
{
	buffer_t *buf = malloc(sizeof(buffer_t));

	if (!buf)
		return (NULL);

	buf->size = 1024;
	buf->len = 0;
	buf->start = malloc(buf->size);

	if (!buf->start)
	{
		free(buf);
		return (NULL);
	}
	return (buf);
}

void free_buffer(buffer_t *buffer)
{
	if (!buffer)
		return;
	free(buffer->start);
	free(buffer);
}

int _memcpy(buffer_t *buffer, const char *src, unsigned int n)
{
	unsigned int i;
	char *new_buf;

	for (i = 0; i < n; i++)
	{
		if (buffer->len >= buffer->size)
		{
			buffer->size *= 2;
			new_buf = realloc(buffer->start, buffer->size);
			if (!new_buf)
				return (0);
			buffer->start = new_buf;
		}
		buffer->start[buffer->len++] = src[i];
	}
	return (n);
}

/* =======================
 *  FORMAT PLACEHOLDERS
 * ======================= */

unsigned char handle_flags(const char *format, char *index)
{
	(void)format;
	*index = 0;
	return (0);
}

int handle_width(va_list args, const char *format, char *index)
{
	(void)args;
	(void)format;
	*index = 0;
	return (0);
}

int handle_precision(va_list args, const char *format, char *index)
{
	(void)args;
	(void)format;
	*index = 0;
	return (-1);
}

unsigned char handle_length(const char *format, char *index)
{
	(void)format;
	*index = 0;
	return (0);
}

/* =======================
 *   SPECIFIER HANDLERS
 * ======================= */

unsigned int print_char(va_list args, buffer_t *buf,
	unsigned char f, int w, int p, unsigned char l)
{
	char c = (char)va_arg(args, int);

	(void)f;
	(void)w;
	(void)p;
	(void)l;

	return (_memcpy(buf, &c, 1));
}

unsigned int print_string(va_list args, buffer_t *buf,
	unsigned char f, int w, int p, unsigned char l)
{
	char *s = va_arg(args, char *);
	unsigned int len = 0;

	(void)f;
	(void)w;
	(void)p;
	(void)l;

	if (!s)
		s = "(null)";

	while (s[len])
		len++;

	return (_memcpy(buf, s, len));
}

unsigned int print_percent(va_list args, buffer_t *buf,
	unsigned char f, int w, int p, unsigned char l)
{
	char c = '%';

	(void)args;
	(void)f;
	(void)w;
	(void)p;
	(void)l;

	return (_memcpy(buf, &c, 1));
}

unsigned int print_int(va_list args, buffer_t *buf,
	unsigned char f, int w, int p, unsigned char l)
{
	int n = va_arg(args, int);
	char tmp[12];
	int i = 0, j;
	unsigned int count = 0;
	unsigned int num;

	(void)f;
	(void)w;
	(void)p;
	(void)l;

	if (n < 0)
	{
		count += _memcpy(buf, "-", 1);
		num = -n;
	}
	else
		num = n;

	if (num == 0)
		tmp[i++] = '0';

	while (num)
	{
		tmp[i++] = (num % 10) + '0';
		num /= 10;
	}

	for (j = i - 1; j >= 0; j--)
		count += _memcpy(buf, &tmp[j], 1);

	return (count);
}

/* =======================
 *  SPECIFIER DISPATCHER
 * ======================= */

unsigned int (*handle_specifiers(const char *format))(
	va_list, buffer_t *, unsigned char, int, int, unsigned char)
{
	if (*format == 'c')
		return (print_char);
	if (*format == 's')
		return (print_string);
	if (*format == 'd' || *format == 'i')
		return (print_int);
	if (*format == '%')
	return (print_percent);
	return (NULL);
}

/* =======================
 *      CLEANUP
 * ======================= */

void cleanup(va_list args, buffer_t *output)
{
	va_end(args);
	write(1, output->start, output->len);
	free_buffer(output);
}

/* =======================
 *     RUN PRINTF
 * ======================= */

int run_printf(const char *format, va_list args, buffer_t *output)
{
	int i, ret = 0;
	unsigned char flags, len;
	int wid, prec;
	char tmp;
	unsigned int (*f)(va_list, buffer_t *,
		unsigned char, int, int, unsigned char);

	for (i = 0; format && format[i]; i++)
	{
		if (format[i] == '%')
{
	if (format[i + 1] == '\0')
	{
		free_buffer(output);
		va_end(args);
		return (-1);
	}

	tmp = 0;
	flags = handle_flags(format + i + 1, &tmp);
	wid = handle_width(args, format + i + 1, &tmp);
	prec = handle_precision(args, format + i + 1, &tmp);
	len = handle_length(format + i + 1, &tmp);

	f = handle_specifiers(format + i + 1);
	if (f)
	{
		i++;
		ret += f(args, output, flags, wid, prec, len);
		continue;
	}
}

		ret += _memcpy(output, &format[i], 1);
	}
	cleanup(args, output);
	return (ret);
}

/* =======================
 *        _printf
 * ======================= */

int _printf(const char *format, ...)
{
	va_list args;
	buffer_t *output;
	int ret;

	if (!format)
		return (-1);

	output = init_buffer();
	if (!output)
		return (-1);

	va_start(args, format);
	ret = run_printf(format, args, output);
	return (ret);
}
