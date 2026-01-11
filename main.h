#ifndef MAIN_H
#define MAIN_H

/* Standard libraries */
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

/* =======================
 *        STRUCTS
 * ======================= */

typedef struct buffer
{
	char *start;
	unsigned int len;
	unsigned int size;
} buffer_t;

/* =======================
 *   CORE PRINTF API
 * ======================= */

int _printf(const char *format, ...);

/* =======================
 *     BUFFER FUNCTIONS
 * ======================= */

buffer_t *init_buffer(void);
void free_buffer(buffer_t *buffer);
int _memcpy(buffer_t *buffer, const char *src, unsigned int n);

/* =======================
 *   FORMAT HANDLERS
 * ======================= */

unsigned char handle_flags(const char *format, char *index);
int handle_width(va_list args, const char *format, char *index);
int handle_precision(va_list args, const char *format, char *index);
unsigned char handle_length(const char *format, char *index);

/* =======================
 *   SPECIFIER HANDLER
 * ======================= */

unsigned int (*handle_specifiers(const char *format))(
	va_list,
	buffer_t *,
	unsigned char,
	int,
	int,
	unsigned char
);

#endif /* MAIN_H */
