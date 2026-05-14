/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 15:11:17 by megiazar          #+#    #+#             */
/*   Updated: 2026/05/12 20:08:41 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "execution.h"
#include "minishell.h"

void	exit_cleanup(int exit_status, t_shelly *shelly)
{
	int	i;

	write(1, "exit\n", 5);
	rl_clear_history();
	if (shelly)
	{
		free_cmd_line(shelly->cur_cmd);
		if (shelly->fds_saved[0] != -1)
			close(shelly->fds_saved[0]);
		if (shelly->fds_saved[1] != -1)
			close(shelly->fds_saved[1]);
		i = 0;
		while (shelly->envp[i])
			free(shelly->envp[i++]);
		free(shelly->envp);
		free(shelly);
	}
	exit(exit_status);
}

static int	press_eof(char *prompt)
{
	return (prompt == NULL);
}

static int	press_enter(char *prompt)
{
	return (prompt && prompt[0] == '\0');
}

static int	shelly_rl(char **prompt)
{
	*prompt = rl_readline("minishell$ ");
	if (press_eof(*prompt))
		return (1);
	if (press_enter(*prompt))
	{
		free(*prompt);
		*prompt = NULL;
		return (2);
	}
	add_history(*prompt);
	return (0);
}
/*
The main execution+parsing logic is here. Saving current command for the signal handling,
cleanup, hd and tracjing active pipeline to go thru the linked listed structure.
If parsing will fail (!cmd_line), ex: ||| -> returns NULL.
At the end: freeing argv arr + redir + linked nodes.
*/

static void	shelly_exec(char **prompt, t_shelly *shelly)
{
	t_cmd_line	*cmd_line;
	t_token 	*tok;

	tok = tokenize_input(*prompt);
	printf("%s\n", tok->value);
	cmd_line = NULL; // for now
	shelly->cur_cmd = cmd_line;
	free(*prompt); //CHECK IF MISSING
	*prompt = NULL;
	if (!cmd_line)
		return ;
	exec_loop(cmd_line, shelly);
	free_cmd_line(cmd_line);
}

/*Signal Reset at the end: after child process finished, we should restore shell dignall behavior*/
int	main(int ac, char **av, char **envp)
{
	char		*prompt;
	t_shelly	*shelly;
	int			condition;

	(void)ac;
	(void)av;
	sig_mode(INTERACTIVE);
	shelly = set_shellyenvp(envp);
	if (!shelly)
		exit (1);
	while (1)
	{
		condition = shelly_rl(&prompt);
		if (condition == 1)
			exit_cleanup(get_signal_stat(), shelly);
		else if (condition == 2) 
			continue ;
		else if (condition == 0)
		{
			shelly_exec(&prompt, shelly);
			sig_mode(INTERACTIVE);
		}
	}
	return (0);
}

//changing envp(initializing shell's envp (copied))
/* nt	main(int ac, char **av, char **envp)
{
	char		*prompt;
	t_cmd_line	*cmd_line;
	t_shelly 	*shelly;

	(void)ac;
	(void)av;
	cmd_line = NULL;
	sig_mode(INTERACTIVE);
	if (!envp)
		write(1, "envp is NULL!\n", 14);
	shelly = set_shellyenvp(envp);
	while (1)
	{
		prompt = readline("minishell$ ");
		if (!prompt)
		{
			ft_putstr_fd("exit\n", 1);
			exit_cleanup(get_signal_stat(), shelly);
		}
		if (*prompt)
			add_history(prompt);
		cmd_line = fake_parse(prompt, shelly);
		if (!cmd_line)
		{
			free(prompt);
			continue ;
		}
		exec_loop(cmd_line, shelly);
		free_cmd_line(cmd_line);
		cmd_line = NULL;
		free(prompt);
		sig_mode(INTERACTIVE);
	}
	return 0;
} */