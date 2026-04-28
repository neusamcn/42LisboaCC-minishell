Do **not** make “file descriptor” a main token category.

## Recommended split

### Token level
Keep only syntax tokens:
- `WORD`
- `REDIR`
- `CTRL_OP`

### Redirection subtype
For redirections, keep:
- `<`
- `>`
- `>>`
- `<<`

### Optional special token
Add an **`IO_NUMBER`** only if you want to support things like:
- `2>`
- `1>>`
- `0<`

That is not a file descriptor category itself; it is a **number attached to a redirection**.

---

## How to manage file descriptors better

File descriptors should be handled in the **execution/redirection layer**, not in tokenization.

A good approach is:

- parse redirections into a command structure
- open files during execution setup
- store every opened fd in one place
- close them in one cleanup function

## Suggested structure

````c
typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
	int				io_number;   // -1 if not provided, otherwise 0/1/2/...
	int				fd;          // opened fd, -1 if not opened yet
	struct s_redir	*next;
}	t_redir;
````

And for the command:

````c
typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirs;
	int				in_fd;
	int				out_fd;
	struct s_cmd	*next;
}	t_cmd;
````

---

## Practical rules

- Initialize all fds to `-1`.
- Open redirection files before `fork()` when possible, or in child if your design requires it.
- After `dup2()`, close the original fd.
- Always close:
  - pipe ends
  - heredoc temp fds
  - redirection fds
- Keep a small cleanup function that closes everything stored in the command context.

---

## For your project
Since your `minishell` is still small, the cleanest solution is:

- **tokenizer**: only syntax
- **parser**: build command + redirection structures
- **executor**: open/dup2/close fds
- **cleanup**: one place to close all fds

If useful, a minimal `t_cmd` + `t_redir` design can be drafted to fit your current headers.

Great choices. These two structs model the execution plan cleanly.

## `t_redir` — one redirection node

This represents **one** redirection attached to a command (like `< infile`, `>> out`, `<< EOF`).

### Fields

- `t_redir_type type;`  
  Redirection operator kind:
  - `IN` (`<`)
  - `OUT` (`>`)
  - `APPEND` (`>>`)
  - `HEREDOC` (`<<`)

- `char *filename;`  
  Target file for `<`, `>`, `>>`.  
  Usually `NULL` for heredoc.

- `int io;`  
  The FD number being redirected (destination/source side), e.g.:
  - default: `0` for input redirs, `1` for output redirs
  - explicit forms if supported later: `2>file`, `1>>file`

- `int fd;`  
  Runtime opened FD (result of `open()` or heredoc pipe read-end).  
  Best practice:
  - initialize to `-1`
  - set when opened
  - close during cleanup after `dup2()` / on error paths

- `char *delimiter;`  
  Used only for heredoc (`<<`), e.g. `EOF` in `cat << EOF`.  
  Usually `NULL` for non-heredoc.

- `struct s_redir *next;`  
  Linked list of redirections in lexical order.

### Why list order matters
Shell applies redirections left-to-right.  
Example:
- `cmd >a >b` → final stdout goes to `b`  
Keeping order lets executor reproduce this behavior exactly.

---

## `t_cmd` — one command in a pipeline

This represents a single simple command, with args + its redirections.

### Fields

- `char **argv;`  
  Command + arguments array for execution:
  - `argv[0]` command name (`ls`, `echo`, etc.)
  - `argv[n]` args
  - NULL-terminated for `execve()`

- `t_redir *redirs;`  
  Head of that command’s redirection list (`t_redir` nodes).

- `int in_fd;`  
  Final input FD chosen for this command at execution time.  
  Typical defaults:
  - `STDIN_FILENO` unless changed by pipe/redirection

- `int out_fd;`  
  Final output FD chosen for this command at execution time.  
  Typical defaults:
  - `STDOUT_FILENO` unless changed by pipe/redirection

- `struct s_cmd *next;`  
  Next command in pipeline (`|`).  
  So a pipeline is a linked list of `t_cmd`.

---

## How they work together (execution flow)

For `cat < in.txt | grep hi >> out.txt`:

- `t_cmd #1`
  - `argv = ["cat", NULL]`
  - redirs: one node (`IN`, filename=`in.txt`, io=0)
- `t_cmd #2`
  - `argv = ["grep", "hi", NULL]`
  - redirs: one node (`APPEND`, filename=`out.txt`, io=1)

Executor then:
1. sets up pipes between `t_cmd` nodes,
2. applies each command’s `redirs` in order,
3. computes `in_fd/out_fd`,
4. `dup2()` to stdio,
5. closes temporary/opened FDs.

---

## Small practical notes

- Keep these structs in parsing domain (parsing.h) if parser builds them; execution can consume them via include.
- Initialize every FD field to `-1` (except defaults when explicitly set).
- `filename` and `delimiter` are mutually exclusive in practice (except if you keep both for simplicity).