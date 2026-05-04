/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_exit_in.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 23:04:46 by megi              #+#    #+#             */
/*   Updated: 2026/05/04 23:46:08 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "execution.h"

// i need to check if the str have only numbers
/*  exit abc      # err - its not a number
exit 1 2      # err - too many aruments  
exit 99999999999999999999  # err - overflow
exit -1       # valid
exit +5       # valid */

/*
exit          exit w last status
exit 42      exit w code 42
exit abc      an err "numeric argument required"
exit 1 2     an err "too many arguments" */

