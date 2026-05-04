#include "../include/minishell.h"

static int ft_arrlen(char **arr)
{
	int i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

static char *expand_var(char *token, t_shelly *shelly)
{
	char	*key;
	char	*value;
	int		i;

	if (token[0] != '$')
		return (ft_strdup(token));
	key = token + 1;
	if (!*key)
		return (ft_strdup("$"));
	i = 0;
	while (shelly->envp[i])
	{
		if (ft_strncmp(shelly->envp[i], key, ft_strlen(key)) == 0
			&& shelly->envp[i][ft_strlen(key)] == '=')
		{
			value = ft_strchr(shelly->envp[i], '=') + 1;
			return (ft_strdup(value));
		}
		i++;
	}
	return (ft_strdup(""));
}

static void add_redir(t_cmd_line *current, t_redir_type type, char *filename, char *delimiter)
{
	t_redirects *new;
	t_redirects *tmp;

	if (current->redir.type == NONE)
	{
		current->redir.type = type;
		current->redir.filename = filename;
		current->redir.delimiter = delimiter;
		current->redir.xd_fd = -1;
		return ;
	}
	new = malloc(sizeof(t_redirects));
	ft_memset(new, 0, sizeof(t_redirects));
	new->xd_fd = -1;
	new->type = type;
	new->filename = filename;
	new->delimiter = delimiter;
	new->next = NULL;
	tmp = &current->redir;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

static t_cmd_line	*new_node(int token_count)
{
	t_cmd_line	*node;

	node = malloc(sizeof(t_cmd_line));
	ft_memset(node, 0, sizeof(t_cmd_line));
	node->redir.xd_fd = -1;
	node->redir.type = NONE;
	node->prevfd = -1;
	node->cmds = malloc(sizeof(char *) * (token_count + 1));
	return (node);
}

t_cmd_line *fake_parse(char *line, t_shelly *shelly)
{
	char        **tokens;
	t_cmd_line  *head;
	t_cmd_line  *current;
	int         i;
	int         cmd_i;
	int         token_count;

	tokens = ft_split(line, ' ');
	if (!tokens)
		return (NULL);
	token_count = ft_arrlen(tokens);
	head = new_node(token_count);
	current = head;
	cmd_i = 0;
	i = 0;
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], "|") == 0)
		{
			current->cmds[cmd_i] = NULL;
			current->next = new_node(token_count);
			current = current->next;
			cmd_i = 0;
		}
		else if (ft_strcmp(tokens[i], ">") == 0 && tokens[i + 1])
			add_redir(current, OUT, tokens[++i], NULL);
		else if (ft_strcmp(tokens[i], ">>") == 0 && tokens[i + 1])
			add_redir(current, APPEND, tokens[++i], NULL);
		else if (ft_strcmp(tokens[i], "<") == 0 && tokens[i + 1])
			add_redir(current, IN, tokens[++i], NULL);
		else if (ft_strcmp(tokens[i], "<<") == 0 && tokens[i + 1])
			add_redir(current, HEREDOC, NULL, tokens[++i]);
		else
			current->cmds[cmd_i++] = expand_var(tokens[i], shelly);
		i++;
	}
	current->cmds[cmd_i] = NULL;
	free(tokens);
	return (head);
}

void	exit_cleanup(int exit_status, t_shelly *minishell)
{
	int	i;

	ft_putendl_fd("Exiting minishell...", STDOUT_FILENO);
	clear_history();
	i = 0;
	if (minishell)
	{
		while (minishell->envp[i])
			free(minishell->envp[i++]);
		free(minishell->envp);	
	}
	free(minishell);
	exit(exit_status);
}

//changing envp(initializing shell's envp (copied))
int	main(int ac, char **av, char **envp)
{
	char		*prompt;
	t_cmd_line	*cmd_line;
    t_shelly 	*shelly;

	(void)ac;
	(void)av;
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
			exit(get_signal_stat());
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
		free(prompt);
		sig_mode(INTERACTIVE);
	}
	return 0;
}


