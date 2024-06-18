/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 09:03:44 by tkafanov          #+#    #+#             */
/*   Updated: 2024/06/16 18:16:54 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	is_handled(char c)
{
	return (c == 'c' || c == 's' || c == 'p' || c == 'd'
		|| c == 'i' || c == 'u' || c == 'x' || c == 'X' || c == '%');
}

static void	define_format(char chr, va_list type, int *count)
{
	if (chr == 'c')
		handle_c(type, count);
	else if (chr == 's')
		handle_s(type, count);
	else if (chr == 'p')
		handle_p(type, count, chr);
	else if (chr == 'd' || chr == 'i')
		handle_d_and_i(type, count);
	else if (chr == 'u')
		handle_u(type, count);
	else if (chr == 'x' || chr == 'X')
		handle_x(type, count, chr);
	else if (chr == '%')
		handle_percent(count);
}

static int	handle_special_sign(const char *s, va_list type)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == '%')
		{
			i++;
			if (s[i] == '\0')
				return (-1);
			if (is_handled(s[i]))
				define_format(s[i++], type, &count);
			else
				printf_ft_putchar('%', &count);
		}
		if (s[i] != '\0' && s[i] != '%')
			printf_ft_putchar(s[i++], &count);
	}
	va_end(type);
	return (count);
}

int	ft_printf(const char *s, ...)
{
	va_list		type;

	if (!s)
		return (-1);
	va_start(type, s);
	return (handle_special_sign(s, type));
}
