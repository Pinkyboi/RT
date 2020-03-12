/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 23:09:56 by merras            #+#    #+#             */
/*   Updated: 2020/03/03 01:28:06 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "log.h"

static const char *g_level_names[] = {
	"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

static const char *g_level_colors[] = {
	"\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[69m"};

void	log_log(int level, const char *file, int line, const char *fmt, ...)
{
	struct tm	*lt;
	va_list		args;
	char		buf[16];
	time_t		t;

	t = time(NULL);
	lt = localtime(&t);
	buf[strftime(buf, sizeof(buf), "%H:%M:%S", lt)] = '\0';
	fprintf(
		stderr, "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ",
		buf, g_level_colors[level], g_level_names[level], file, line);
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
	fflush(stderr);
}
