# szsh — Sz Shell

A Unix shell written in C from scratch. Implements core shell functionality using real Linux system calls — `fork()`, `execvp()`, `dup2()`, and `waitpid()`.

---

## Features

- **Command execution** — run any program available on your `$PATH`
- **Pipes** — pipe output between commands with `|`
- **Built-in commands** — `cd` and `exit` implemented directly in the shell
- **Signal handling** — `Ctrl+C` kills the running command without exiting the shell
- **Graceful EOF** — `Ctrl+D` exits the shell cleanly

---

## Project Structure

```
szsh/
├── main.c        # Main loop — prompt, input, dispatch
├── parse.c/.h    # Tokenizes input into argument arrays
├── execute.c/.h  # Command execution and pipe handling
├── builtin.c/.h  # Built-in command implementations
└── Makefile
```

---

## Requirements

- GCC
- Linux or WSL

---

## Building

```bash
make
```

To clean compiled files:

```bash
make clean
```

---

## Usage

```bash
./szsh
```

```
Type 'exit' to leave the shell
szsh> ls -la
szsh> ls | grep .c
szsh> cd /tmp
szsh> exit
```

---

## Implementation Details

### Command Execution

Every external command is executed by forking a child process and calling `execvp()` to replace it with the target program. The parent waits for the child to finish with `waitpid()` before returning to the prompt.

```
fork() → child calls execvp() → parent calls waitpid()
```

### Pipes

Pipes are implemented using the `pipe()` system call which creates two file descriptors — a read end and a write end. Two child processes are forked: the left command has its stdout replaced with the pipe write end via `dup2()`, and the right command has its stdin replaced with the pipe read end. Both unused ends are closed in every process to ensure proper EOF propagation.

```
ls | grep txt
     │
  pipe()
     │
fork() × 2
     │
child1: dup2(pipefd[1], stdout) → execvp(ls)
child2: dup2(pipefd[0], stdin)  → execvp(grep)
     │
parent: close both ends, waitpid() × 2
```

### Signal Handling

The shell process ignores `SIGINT` (`Ctrl+C`) on startup. Each child process restores the default `SIGINT` handler before calling `execvp()`, so `Ctrl+C` kills only the running command and returns control to the shell prompt.

### Built-ins

`cd` and `exit` must be implemented as built-ins because they affect the shell process itself. An external `cd` would only change the working directory of a child process, leaving the shell unaffected. The shell checks for built-in commands before attempting to `fork()`.

---

## Concepts Covered

- Process creation and management (`fork`, `execvp`, `waitpid`)
- File descriptors and redirection (`dup2`, `pipe`)
- Signal handling (`signal`, `SIGINT`)
- Memory management (`malloc`, `free`)
- String parsing in C (`strtok`, `strchr`, `strcmp`)
- Multi-file C projects and Makefiles
