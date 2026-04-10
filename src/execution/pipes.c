#include "execution.h"

/* ls stdout ──pipe1──► grep stdin
              grep stdout ──pipe2──► wc stdin
                                      wc stdout ──► terminal */		
									  
//TODO split functions

//do redir tests
/* ls > files.txt
echo "some text" >> files.txt
sort < files.txt  */


//Input piping
//Output piping allows to pass the output of one cmds as input to anotehr cmd

// we can also pipe the output of a here doc into another cmd

/* cat << EOF | grep "grr"
cat << EOF > file.txt
<< EOF
echo "hi" << EOF
ftp -n << EOF FILE TRANSFER PROTOCOL can accept cmds via HD for automatic file
bc << EOF bc cmd is a calculator that can be used with here docs to perform calloc
sendmail user@mail.ru << end */

//create a child process
//why would we ignore expansions Literal Text
//cat << 'EOF'
//TODO : expands from pars

void heredoc(t_redirects *redir)
{
	int pipefd[3];
	char *msg;

	msg = NULL;
	pipe(pipefd);
	while (msg)
	{
		msg = readline("> ");
		if (strcmp(msg, DELIMETER) == TRUE)
			break ;
		if (!msg)
		{
			write(pipefd[2], HD, ft_strlen(HD));
			//write(pipefd[1], DELIMETER, ft_strlen(DELIMETER));
			write(pipefd[2], redir->delimiter, ft_strlen(redir->delimiter));
			write(pipefd[1], "\n", 1);
			free(msg);
		}
		write(pipefd[1], msg, ft_strlen(msg));
		write(pipefd[1], "\n", 1);
		free(msg);
	}
	close(pipefd[1]);
	dup2(pipefd[0], STDIN);
	close(pipefd[1]);
}
/* 
void heredoc(t_redirects *redir)
{
    int pipefd[2];
    char *prompt;
    
    prompt = NULL;
    pipe(pipefd);
	signal(SIGINT, sigint_glob);
	signal(SIGQUIT, SIG_IGN);
	et_signals_interactive_parent();
    while (1)
    {
        prompt = readline("> ");
        if (!prompt && get_signal_stat() == 130)
            exit(130);
        if (!prompt)
        {
            ft_putstr_fd(HD, 2);
            ft_putstr_fd(redir->delimiter, 2);
            ft_putstr_fd("')\n", 2);
            break ;
        }
		if (ft_strcmp(prompt, redir->delimiter) == TRUE)
			break ;
		write(pipefd[1], prompt, ft_strlen(prompt));
		write(pipefd[1], "\n", 1);
		free(prompt);
    }
	signal_handler(1);
    close(pipefd[1]);
	dup2(pipefd[0], STDIN);
	close(pipefd[0]);
} */

void append(t_redirects *redir)
{
    int fd[2];

    if (redir->filename)
    {
        if (redir->type == APPEND)
            fd[1] = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            fd[1] = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd[1] == -1)
		// do i need exit(1)?
		{
			print_err_msg(redir->filename);
 			exit(1);
		}
		dup2(fd[1], STDOUT);
        close(fd[1]);
    }
}

void pipe_handler(t_redirects *redir)
{
	int fd[2];
	
    if (redir->type == APPEND || redir->type == OUT)
		append(redir);
    if (redir->type == HEREDOC)
		heredoc(redir);
	//should i do execve() ? 
	else if (redir->type == IN)
    {
        if (access(redir->filename, F_OK) != 0)
            p_log_err(redir->filename, "No such file or directory");
        fd[0] = open(redir->filename, O_RDONLY);
        if (fd[0] != -1)
            dup2(fd[0], STDIN);
        close(fd[0]);
    }
}

//TODO: use minienvp instead of envp

int exec_pipeline(t_pipe *cmd_line, char **envp, int status)
{
    while (cmd_line)
    {
        status = exec(cmd_line, envp, status);
        cmd_line = cmd_line->next;
    }
    return (status);
}
