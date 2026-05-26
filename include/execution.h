/* ************************************************************************** */
/*                                                                        */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megiazar <megiazar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 15:46:35 by megi              #+#    #+#             */
/*   Updated: 2026/05/10 15:08:33 by megiazar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
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
# include <sys/resource.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <string.h>
# include <termios.h>

# define SHELLYMATES 0
# define SHELLYPEERS 0
# define VALID 0
# define NON_VALID 1
# define PLUS_OR_MINUS "+-" 
# define WHITESPACE " \t"
# define MINIM_DIG '0' 
# define MAXIM_DIG '9'
# define OK 0
# define KO 1
# define CD "cd"
# define ECHO_BLT "echo" 
# define EXIT "exit"
# define PWD "pwd" 
# define ENV "env" 
# define EXPORT "export"
# define UNSET "unset" 
# define HD "mini: warning: here-document delimited by end-of-file (wanted ')"

typedef enum e_builts
{
	BUILTINS,
	EXTRENAL
}	t_builtins_check;

typedef struct s_cd_blt
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;
}	t_cd;

/* 									PATH.C 							*/
char	*relative_path(t_cmd_line *cmd_line, t_shelly *shelly);
char	*paths_helper(t_cmd_line *cmd_line, char *path_var);
char	*absolute_path(t_cmd_line *cmd_line);
int		are_you_builtin(t_cmd_line *cmd_line);

/*								EXECUTION.C							*/
void	exec_loop(t_cmd_line *cmds, t_shelly *shelly);
int		mommy_n_father(t_cmd_line *s, t_shelly *shelly);
int		lonely_blt(t_cmd_line *s, t_shelly *shelly);
int		are_you_builtin(t_cmd_line *cmd_line);
void	child_ex(char *path, t_cmd_line *kid, t_shelly *shelly);
void	child_ex_fds(t_cmd_line *kid, t_shelly *shelly);
void	single_child_ex(t_cmd_line *kid, t_shelly *shelly);
int		mndwait(pid_t last_p, int cmd_nmb);
int		ex_pipeline_ec(t_cmd_line *pipeline, t_shelly *shelly);
void	lonely_child_exit(t_shelly *shelly, char *path, int status);
void	lnly_ch_errmsg(t_cmd_line *kid, t_shelly *shelly);

/*									EXECUTION UTILS					*/
bool	if_redir(t_cmd_line *s);
char	*abs_or_rel_p(t_cmd_line *c, t_shelly *shelly);
void	store_fds(int read_save, int write_save);

/*									REDIRECTIONs					*/
bool	which_redir_type(t_cmd_line *cmd);
bool	in_redir(t_redirects *redir);
bool	append(t_redirects *redir);

void	run_xds(t_cmd_line *cmds, t_shelly *shelly);
void	write_hd_line(char *msg, int fd, t_shelly *shelly, bool quoted);
void	child_hd(t_redirects *redir, int pipefd[2], t_shelly *shelly);
void	mnd_heredoc(t_redirects *redir, t_shelly *shelly);

/*									FREEs							*/
int		free_path(char **paths);
void	cleanup_xd_fds(t_cmd_line *start);
void	free_cmd_line(t_cmd_line *cmd);
void	ft_free_split(char **arr);
void	free_redirs(t_redirects *redir);
char	*ft_strjoin_free(char *s1, char *s2);
void	free_copyenvp(t_shelly *shelly);
void	free_shell(t_shelly *shelly);
void	babies_cleanup(t_shelly *shelly, char *path);

/*									BUILTINs						*/
int		r_bltn(t_cmd_line *cmd_line, t_shelly *shelly);

/*										ECHO						*/
int		myecho(t_cmd_line *echo, t_shelly *shelly);

/*										ENV							*/
int		myenv(t_cmd_line *env, t_shelly *shelly);

/*									EXPORT							*/
int		myexport(t_cmd_line *exp, t_shelly *shelly);
int		parse_exp_arg(char *arg);
bool	exp_argv(char c, int j);
char	**exp_flag(t_export *exp);
char	**exp_var(t_export *mini, char *k);
char	**exp_minienv(t_export *mini, char *k, char *v, int sz);
void	pexp_var(char *env_entry);
void	pexp(t_shelly *shelly);
char	*smash_key_val(char *k, char *v);
int		where_is_it(char **envp, char *k);
char	**swap_it(char **envp, int pos, char *smashed);
char	**shove_it_in(char **envp, char *smashed, int sz);


/*									PWD								*/
int		mypwd(t_cmd_line *cmd, t_shelly *shelly);

/*								UNSET								*/
int		mysunset(t_cmd_line *unset, t_shelly *shelly);

/*								CD									*/
int		mycd(t_cmd_line *cd, t_shelly *shelly);

/*								EXIT								*/
int		myexit(t_cmd_line *argv, t_shelly *shelly);

int		get_signal_stat(void);
int		status_check(int status);

#endif
