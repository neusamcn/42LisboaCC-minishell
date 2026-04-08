/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 15:46:48 by megi              #+#    #+#             */
/*   Updated: 2026/04/08 18:37:45 by megi             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../include/minishell.h"

// PARSERS
void	init(int ac, char **av, char **env)
{
	char	cwd_str[PATH_MAX];
	char	*input_str;
	int		last_status;
	t_pipe	*cmd_line;

	(void)ac;
	(void)av;
	last_status = 0;
	while (1)
	{
		getcwd_protec(cwd_str, PATH_MAX);
		if (isatty(STDIN_FILENO))
		{
			ft_putstr_fd(DEEP_PINK"meg&neu minishell:"COLOR_RESET, STDOUT_FILENO);
			ft_putstr_fd(cwd_str, STDOUT_FILENO);
			signal(SIGINT, sigint_glob);
			signal(SIGQUIT, SIG_IGN);
			input_str = readline(DEEP_PINK"$ "COLOR_RESET);
			if (!input_str && get_signal_stat() == 130)
			{
    			set_signal_stat(0);
    			continue ;
			}
			if (!input_str)
				exit_cleanup(0, 0);
			if (*input_str)
				add_history(input_str);
			// TODO: tokenize + REAL parse
			cmd_line = fake_parse(input_str);  // DELETE for parse() later
			if (cmd_line)
			{
				last_status = exec(cmd_line, env, last_status);
				free(cmd_line);
			}
			free(input_str);
		}
		else
		{
			ft_putstr_fd("minishell: stdin is not a terminal\n", 2);
			exit_cleanup(0, 0);
		}
	}
}

int	main(int ac, char **av, char **env)
{
	if (!env)
		return 0;
	ft_printf(LIGHT_PINK"%s"COLOR_RESET, BANNER);
	init(ac, av, env);
	exit_cleanup(0, 0);
	return (EXIT_SUCCESS);
}

// MINE
/* int main(int ac, char *av[], char **envp)
{
	char *prompt = NULL;
	t_pipe	cmd_line;
	
	(void)ac;
	(void)av;
  if (!envp)
    return (0);
	//the signals should be here TD
  while (envp)
  {
    prompt = readline("minishell$ ");
    if (!prompt)
      return (0);
    else if (prompt)
      add_history(prompt);
    p("%s", prompt);
	exec(&cmd_line, envp, 0, 0, 0);
	free(prompt);
  }	
  return (0);
} */
