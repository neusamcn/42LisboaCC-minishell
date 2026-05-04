/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megi <megi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 15:46:35 by megi              #+#    #+#             */
/*   Updated: 2026/05/04 21:16:54 by megi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef EXECUTION_H
# define EXECUTION_H

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
# include <stdbool.h>
# include <sys/resource.h> // Milena, I included this so WCOREDUMP() can work
# include <sys/wait.h>

# define p(...) printf(__VA_ARGS__)
# define CD "cd"
# define ECHO "echo" //done
# define EXIT "exit"
# define PWD "pwd" // done
# define ENV "env" //done
# define EXPORT "export" //dome
# define UNSET "unset" // done
# define TRUE 0
# define FALSE 1
# define HD "minishell: warning: here-document delimited by end-of-file (wanted '"

typedef struct s_shelly	t_shelly;

typedef enum e_stdio
{
	READ,
	WRITE
}   t_stdio;

typedef enum e_builts
{
	BUILTINS,
	EXTRENAL
}	t_builtins_check;

typedef enum e_mode
{
	INTERACTIVE, // prompt
	BLT_EXECUTING, // no fork, no rl
	CHILD, // fork + execve
	MNDWAIT
}	e_mode_for_sig;

typedef struct s_redirections
{
	t_redir_type			type;
	char					*filename;
	char    				*delimiter;
	int						fd[2];
	int						xd_fd;
	struct s_redirections	*next;
}   t_redirects;

typedef struct s_export 	t_export;

typedef struct s_cmd_line
{
    char            	**cmds;
	t_redirects     	redir;
	int					pipefd[2];
	int					prevfd;
	struct s_export		*bltn_export;
	struct s_cmd_line   *next;
}   t_cmd_line;

typedef struct s_export
{
    char    *arg;
    int     flag;
	char 	*new_var;
	char 	**envp;
    char    **newenv;
	struct s_cmd_line   *expline;
}   t_export;

/* 									PATH.C 									*/
char 	*relative_path(t_cmd_line *cmd_line, t_shelly *shelly);
char 	*paths_helper(t_cmd_line *cmd_line, char *path_var);
char 	*absolute_path(t_cmd_line *cmd_line);

/* 								EXECUTION.C 								*/ 
void 	exec_loop(t_cmd_line *cmds, t_shelly *shelly);
int		mommy_n_father(t_cmd_line *s, t_shelly *shelly);
int 	lonely_blt(t_cmd_line *s, t_shelly *shelly);
int		are_you_builtin(t_cmd_line *cmd_line);
void	child_ex(char *path, t_cmd_line *kid, t_shelly *shelly);
void	child_ex_fds(t_cmd_line *kid);
int 	single_child_ex(t_cmd_line *kid, t_shelly *shelly);
int 	mndwait(pid_t last_p, int cmd_nmb);
int		ex_pipeline_ec(t_cmd_line *pipeline, t_shelly *shelly);

/* 								EXECUTION UTILS 							*/
bool 	if_redir(t_cmd_line *s);
int		do_redri(t_redirects *s);
char    *abs_or_rel_p(t_cmd_line *c, t_shelly *shelly);
char    *abs_or_rel_p(t_cmd_line *c, t_shelly *shelly);

/* 								REDIRECTIONs 								*/
int 	which_redir_type(t_redirects *redir);
int 	in_redir(t_redirects *redir);
void 	heredoc(t_redirects *redir);
void 	append(t_redirects *redir);

/*								SIGNALs 									*/
void	sigint_glob(int sig);
int		get_signal_stat(void);
void	set_signal_stat(int value);
void 	sig_mode(int md);
void	sigint_prompt_handler(int signal);
void	set_signals_interactive_parent(void);
void	set_sigaction(int signo, void (*handler)(int), int flags);
int		status_check(int status);

/* 								ERRORs 										*/
void	p_log_err(char *msg, char *cmd);
int		mndp_log_err(char *msg, char *cmd);

/* 								FREEs 										*/
void	close_fds(void);
int 	free_path(char **paths);
void 	pipe_cl(t_cmd_line *pipeline);
void	cleanup_xd_fds(t_cmd_line *start);

/* 								BUILTINs 									*/
int r_bltn(t_cmd_line *cmd_line, t_shelly *shelly);

/* 									ECHO 									*/
int myecho(t_cmd_line *echo, t_shelly *shelly);

/* 									ENV 									*/
int myenv(t_cmd_line *env, t_shelly *shelly);

/* 								EXPORT 										*/
int 	myexport(t_cmd_line *exp, t_shelly *shelly);
int 	parse_exp_arg(char *arg);
bool 	exp_argv(char c, int j);
char	**exp_flag(t_export *exp);
char	**exp_var(t_export *mini, char *key);
char	**exp_minienv(t_export *mini, char *key, char *value, int i);
void 	pexp_var(char *env_entry);
void    pexp(t_shelly *shelly);
char	*ft_free_strjoin(char *s1, char *s2);

/* 								PWD                                       */
int	mypwd(t_cmd_line *cmd, t_shelly *shelly);

/*								UNSET										*/
int	mysunset(t_cmd_line *unset, t_shelly *shelly);

/*								CD										*/
int	mycd(t_cmd_line *cd, t_shelly *shelly);
int update_pwd(t_shelly *shelly);

# endif
