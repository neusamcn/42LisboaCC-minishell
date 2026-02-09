*This project has been created as part of the 42 curriculum by megiazar, ncruz-ne.*

## Description

Minishell is a simplified Unix shell implementation written in C. The goal is to create a functional command-line interpreter that replicates core bash behaviors, providing hands-on experience with processes, file descriptors, signal handling, and system calls.

## Instructions

1. Compile the project with `make` or `make all`.
2. Run the shell with `./minishell`.
3. Use `make clean` to remove object files.
4. Use `make fclean` to remove all generated files.
5. Use `make re` to rebuild the project from scratch.

## Resources

AI was used as a support for research in order to find and better understand the most relevant resources and concepts related to this project.

#### Processes and System Calls

- [fork(), exec(), wait() manual pages](https://man7.org/linux/man-pages/man2/fork.2.html)
- [pipe() and dup2() system calls](https://man7.org/linux/man-pages/man2/pipe.2.html)

#### Signal Handling

- [signal() and sigaction() documentation](https://man7.org/linux/man-pages/man7/signal.7.html)

#### Readline Library

- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/rltop.html)

#### Shell Behavior Reference

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)

## Project's Scope

### Objectives

Build a functional shell that mimics core bash behavior, demonstrating understanding of:
- Process creation and management
- File descriptor manipulation
- Signal handling
- Command parsing and execution

### Deliverables

| Deliverable | Description |
|-------------|-------------|
| Makefile | Build system with rules: `$(NAME)`, `all`, `clean`, `fclean`, `re` |
| minishell | Executable shell program |
| Source files | All `.c` and `.h` files required for compilation |

### In-Scope (Mandatory Features)

#### Core Shell Functionality
- Display prompt when waiting for input
- Maintain working command history
- Search and launch executables (via PATH, relative, or absolute paths)

#### Quoting
- Handle single quotes `'` (prevent metacharacter interpretation)
- Handle double quotes `"` (prevent metacharacter interpretation except `$`)

#### Redirections
| Operator | Function |
|----------|----------|
| `<` | Redirect input |
| `>` | Redirect output |
| `<<` | Here-document (read until delimiter) |
| `>>` | Redirect output in append mode |

#### Pipes
- Implement `|` to connect command outputs to inputs

#### Environment Variables
- Expand `$VAR` to corresponding values
- Expand `$?` to most recent exit status

#### Signal Handling (Interactive Mode)
| Signal | Behavior |
|--------|----------|
| `ctrl-C` | Display new prompt on new line |
| `ctrl-D` | Exit the shell |
| `ctrl-\` | Do nothing |

#### Built-in Commands
| Command | Specification |
|---------|---------------|
| `echo` | With `-n` option |
| `cd` | Relative or absolute path only |
| `pwd` | No options |
| `export` | No options |
| `unset` | No options |
| `env` | No options or arguments |
| `exit` | No options |

### Out-of-Scope (Exclusions)

The following are explicitly **not required**:
- Unclosed quotes handling
- Backslash `\` interpretation
- Semicolon `;` interpretation
- Wildcard `*` expansion (mandatory part)
- Logical operators `&&` and `||` (mandatory part)
- Parentheses for priorities (mandatory part)
- Fixing readline() memory leaks (external library)

### Constraints

- Written in C, compliant with the 42 Norm
- Compiled with `-Wall -Wextra -Werror` flags
- No unexpected crashes (segfault, bus error, double free)
- All heap memory must be properly freed (no memory leaks in own code)
- Maximum one global variable (for signal handling only, storing signal number)
- Use bash as reference for ambiguous requirements

### Allowed External Functions

`readline`, `rl_clear_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`, `add_history`, `printf`, `malloc`, `free`, `write`, `access`, `open`, `read`, `close`, `fork`, `wait`, `waitpid`, `wait3`, `wait4`, `signal`, `sigaction`, `sigemptyset`, `sigaddset`, `kill`, `exit`, `getcwd`, `chdir`, `stat`, `lstat`, `fstat`, `unlink`, `execve`, `dup`, `dup2`, `pipe`, `opendir`, `readdir`, `closedir`, `strerror`, `perror`, `isatty`, `ttyname`, `ttyslot`, `ioctl`, `getenv`, `tcsetattr`, `tcgetattr`, `tgetent`, `tgetflag`, `tgetnum`, `tgetstr`, `tgoto`, `tputs`

### Milestones

| Phase | Tasks |
|-------|-------|
| **1. Setup** | Project structure, Makefile, libft integration |
| **2. Lexer/Parser** | Tokenization, command parsing, quote handling |
| **3. Executor** | Command execution, PATH resolution, fork/exec |
| **4. Redirections** | Input/output redirections, here-documents |
| **5. Pipes** | Pipeline implementation |
| **6. Built-ins** | All 7 built-in commands |
| **7. Signals** | ctrl-C, ctrl-D, ctrl-\ handling |
| **8. Environment** | Variable expansion, `$?` handling |
| **9. Testing** | Edge cases, memory leaks, norm compliance |

### Bonus Features (Optional)

Only evaluated if mandatory part is perfect:
- `&&` and `||` with parentheses for priorities
- Wildcard `*` for current working directory


