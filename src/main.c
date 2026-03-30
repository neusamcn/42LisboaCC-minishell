/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 15:46:48 by megi              #+#    #+#             */
/*   Updated: 2026/03/30 21:19:04 by megi             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

//loop + parcing + running


// TO DELETE
t_pipe *fake_parse(char *line)
{
    t_pipe *p = malloc(sizeof(t_pipe));
    char **argv = malloc(2 * sizeof(char *));
    argv[0] = strdup(line);
    argv[1] = NULL;
    p->cmds = argv;
    return p;
}

// HALF TO DELETE
int main(int ac, char *av[], char **envp)
{
    char *prompt = NULL;
    t_pipe *cmd_line = NULL;
    int last_status = 0;

	(void)ac;
    (void)av;
    if (!envp)
        return 0;
    while (1)
    {
        prompt = readline("minishell$ ");
        if (!prompt)
            break;
        if (*prompt)
            add_history(prompt);
        cmd_line = fake_parse(prompt);
        if (!cmd_line)
		{
            free(prompt);
            continue ;
        }
        last_status = exec((t_pipe *)cmd_line, envp, last_status);
		free(cmd_line->cmds[0]);
		free(cmd_line->cmds);
		free(cmd_line);
		cmd_line = NULL;
		free(prompt);
    }
    return 0;
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
