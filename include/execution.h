/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 15:46:35 by megi              #+#    #+#             */
/*   Updated: 2026/03/30 21:15:59 by megi             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef EXECUTION_H
#define EXECUTION_H

# include "minishell.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include <fcntl.h>
# include <sys/types.h>
# include <signal.h>

# define p(...) printf(__VA_ARGS__)
# define CD "cd"
# define ECHO "echo"
# define EXIT "exit"
# define PWD "pwd"
# define ENV "env"
# define EXPORT "export"
# define UNSET "unset"

typedef enum e_builts
{
	BUILTINS,
	NON_BUILTINS
}	builtins_check;

// [ls] → [grep src] → [wc -l] → NULL 

typedef struct s_pipe
{
    char            **cmds;
    char            *infile;
    char            *outfile;
    int             append;
    struct s_pipe   *next;
}   t_pipe;

// PATH //
char *path(t_pipe *cmd_line, char **envp);
char *paths_helper(t_pipe *cmd_line, char **envp, int i);
char *absolute_path(t_pipe *cmd_line);
int free_path(char **paths);

// FORKs and SIGNALs
int exec(t_pipe *cmd_line, char **envp, int status);
void signal_handler(pid_t pid);
int status_check(int status);
int are_you_builtin(t_pipe *cmd_line);

// PIPEs //

# endif
