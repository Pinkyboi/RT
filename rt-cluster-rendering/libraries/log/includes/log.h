/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 23:10:03 by merras            #+#    #+#             */
/*   Updated: 2020/03/04 17:48:31 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_H
# define LOG_H

# include <stdio.h>
# include <stdarg.h>

enum
{
	LOG_TRACE,
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
};

# define LOG_TRACE(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
# define LOG_DEBUG(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
# define LOG_INFO(...) log_log(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
# define LOG_WARN(...) log_log(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
# define LOG_ERROR(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)

void log_log(int level, const char *file, int line, const char *fmt, ...);

#endif
