#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * _putchar - writes a character to stdout
 * @c: character to print
 *
 * Return: 1
 */
int _putchar(char c)
{
	return (write(1, &c, 1));
}

/**
 * print_int - prints an integer
 * @n: integer to print
 *
 * Return: number of characters printed
 */
int print_int(int n)
{
	unsigned int num;
	int count = 0;
	char digit;

	if (n < 0)
	{
		count += _putchar('-');
		num = -n;
	}
	else
		num = n;

	if (num / 10)
		count += print_int(num / 10);

	digit = (num % 10) + '0';
	count += _putchar(digit);

	return (count);
}

/**
 * _printf - prints formatted output
 * @format: format string
 *
 * Return: number of characters printed, or -1 on error
 */
int _printf(const char *format, ...)
{
	va_list args;
	int i, count = 0;

	if (!format)
		return (-1);

	va_start(args, format);

	for (i = 0; format[i]; i++)
	{
		if (format[i] == '%')
		{
			if (format[i + 1] == '\0')
			{
				va_end(args);
				return (-1);
			}
			if (format[i + 1] == 'd' || format[i + 1] == 'i')
			{
				count += print_int(va_arg(args, int));
				i++;
				continue;
			}
		}
		count += _putchar(format[i]);
	}

	va_end(args);
	return (count);
}
