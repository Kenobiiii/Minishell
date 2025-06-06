# Minishell Project

## Overview
Minishell is a simplified shell implementation written in C that recreates core functionality of bash. This project demonstrates understanding of system programming concepts including process management, file descriptors, signal handling, and parsing complex command structures.

## Table of Contents
1. [Project Structure](#project-structure)
2. [Core Data Structures](#core-data-structures)
3. [Part 1: Parsing](#part-1-parsing)
4. [Part 2: Execution](#part-2-execution)
5. [Built-in Commands](#built-in-commands)
6. [Memory Management](#memory-management)
7. [Signal Handling](#signal-handling)
8. [Usage Examples](#usage-examples)

## Project Structure

```
Minishell/
├── src/
│   ├── main.c                    # Entry point and main loop
│   ├── minishell.h              # Header with all structures and prototypes
│   ├── parse/                   # Parsing module
│   │   ├── ft_ast.c            # AST construction
│   │   ├── ft_tokens.c         # Tokenization
│   │   ├── ft_handle_quotes.c  # Quote handling
│   │   ├── ft_expansion.c      # Variable expansion
│   │   └── ft_redirections.c   # Redirection parsing
│   ├── exec/                   # Execution module
│   │   ├── exec.c             # Main execution logic
│   │   ├── pipe.c             # Pipeline execution
│   │   ├── heredoc.c          # Here-document handling
│   │   └── redirections.c     # I/O redirection
│   ├── builtins/              # Built-in commands
│   │   ├── ft_echo.c
│   │   ├── ft_cd.c
│   │   ├── ft_pwd.c
│   │   ├── ft_export.c
│   │   ├── ft_unset.c
│   │   ├── ft_env.c
│   │   └── ft_exit.c
│   └── utils/                 # Utility functions
└── libft/                     # Custom C library
```

## Core Data Structures

### t_data - Main Program State
```c
typedef struct s_data
{
    char        *line;              // Current input line
    char        *prompt;            // Shell prompt string
    char        *cwd;              // Current working directory
    int         exit;              // Exit flag
    int         wstatus;           // Last command exit status
    int         only_redirections; // Flag for redirection-only commands
    t_env       *env;              // Environment variables linked list
    t_tokens    *tokens;           // Tokenized input
    t_ast       *ast;              // Abstract Syntax Tree
    int         input_redir_fd;    // Input redirection file descriptor
    int         output_redir_fd;   // Output redirection file descriptor
    int         heredoc_pipe_fd;   // Here-document pipe file descriptor
} t_data;
```

### t_ast - Abstract Syntax Tree Node
```c
typedef struct s_ast
{
    t_type          type;    // Node type (CMD, PIPE, REDIRECT_IN, etc.)
    char            *value;  // Command or operator value
    char            **args;  // Command arguments array
    struct s_ast    *left;   // Left child node
    struct s_ast    *right;  // Right child node
} t_ast;
```

### t_tokens - Token Linked List
```c
typedef struct s_tokens
{
    t_type          type;    // Token type
    char            *value;  // Token string value
    struct s_tokens *next;   // Next token in list
} t_tokens;
```

### t_type - Token/Node Types
```c
typedef enum e_type
{
    CMD,           // Command
    REDIRECT_OUT,  // > (output redirection)
    REDIRECT_IN,   // < (input redirection)
    REDIN2,        // << (here-document)
    REDOUT2,       // >> (append redirection)
    PIPE           // | (pipe)
} t_type;
```

## Part 1: Parsing

The parsing module transforms user input into an Abstract Syntax Tree (AST) that represents the command structure. This process involves several stages:

### 1. Tokenization (`ft_tokens.c`)

**Purpose**: Break down the input string into meaningful tokens.

**Process**:
1. **Lexical Analysis**: Scan the input character by character
2. **Token Identification**: Recognize operators (`|`, `>`, `<`, `>>`, `<<`) and commands
3. **Quote Handling**: Preserve spaces and special characters within quotes
4. **Token Creation**: Create linked list of tokens with type and value

**Key Functions**:
- `tokenize()`: Main tokenization function
- `create_token()`: Creates individual token nodes
- `add_token()`: Adds token to linked list

**Example**:
```bash
Input: echo "hello world" | grep hello > output.txt
Tokens: [CMD:"echo"] [CMD:"hello world"] [PIPE:"|"] [CMD:"grep"] [CMD:"hello"] [REDIRECT_OUT:">"] [CMD:"output.txt"]
```

### 2. Syntax Checking (`ft_check_syntax.c`)

**Purpose**: Validate command syntax before AST construction.

**Checks**:
- Proper quote closure
- Valid operator placement
- Correct pipe usage
- Proper redirection syntax

**Key Functions**:
- `check_syntax()`: Main syntax validation
- `check_operator_syntax()`: Validates operator placement
- `openquotes()`: Checks for unclosed quotes

### 3. Quote Handling (`ft_handle_quotes.c`)

**Purpose**: Process quoted strings correctly, preserving content and removing quote characters.

**Types of Quotes**:
- **Single quotes (`'`)**: Preserve everything literally, no expansion
- **Double quotes (`"`)**: Allow variable expansion but preserve spaces
- **No quotes**: Normal parsing with expansion

**Key Functions**:
- `handle_quotes()`: Main quote processing function
- `process_quoted_content()`: Extracts content from quotes
- `remove_quotes()`: Removes quote characters after processing

### 4. Variable Expansion (`ft_expansion.c`)

**Purpose**: Replace environment variables and special variables with their values.

**Expansion Types**:
- **Environment variables**: `$VAR` → value of VAR
- **Exit status**: `$?` → last command exit status
- **Process ID**: `$$` → current shell PID

**Process**:
1. Scan for `$` character
2. Extract variable name
3. Look up value in environment
4. Replace `$VAR` with actual value

**Key Functions**:
- `expand_variables()`: Main expansion function
- `get_env_value()`: Retrieves environment variable value
- `expand_exit_status()`: Handles `$?` expansion

### 5. AST Construction (`ft_ast.c`)

**Purpose**: Build a binary tree representing command structure and operator precedence.

**AST Structure**:
- **Internal nodes**: Operators (pipes, redirections)
- **Leaf nodes**: Commands and arguments
- **Tree structure**: Represents execution order and data flow

**Construction Process**:
1. **Parse by precedence**: Pipes have lowest precedence
2. **Handle redirections**: Attach to command nodes
3. **Build command nodes**: Group command with its arguments
4. **Link nodes**: Create parent-child relationships

**Key Functions**:
- `build_ast()`: Main AST construction function
- `handle_pipe()`: Creates pipe nodes
- `handle_redirections()`: Processes redirection operators
- `create_cmd_node()`: Creates command nodes

**AST Example**:
```bash
Command: echo hello | grep h > output.txt

AST Structure:
        PIPE
       /    \
    CMD      REDIRECT_OUT
   /         /           \
"echo"    CMD            "output.txt"
  |       /
"hello" "grep"
         |
        "h"
```

### 6. Redirection Parsing (`ft_redirections.c`)

**Purpose**: Handle input/output redirection operators and file specifications.

**Redirection Types**:
- **`>`**: Redirect stdout to file (overwrite)
- **`>>`**: Redirect stdout to file (append)
- **`<`**: Redirect stdin from file
- **`<<`**: Here-document (multi-line input)

**Key Functions**:
- `parse_redirections()`: Main redirection parser
- `handle_input_redirect()`: Processes input redirections
- `handle_output_redirect()`: Processes output redirections
- `setup_heredoc()`: Handles here-document setup

## Part 2: Execution

The execution module interprets the AST and executes commands, handling pipes, redirections, and built-in commands.

### 1. AST Interpretation (`exec.c`)

**Purpose**: Traverse the AST and execute commands according to the tree structure.

**Execution Strategy**:
- **Post-order traversal**: Execute children before parent
- **Type-based dispatch**: Different handling for each node type
- **Context preservation**: Maintain file descriptors and environment

**Key Functions**:
- `execute_ast()`: Main AST execution function
- `execute_command()`: Executes command nodes
- `execute_pipe()`: Handles pipe nodes
- `execute_redirection()`: Processes redirection nodes

### 2. Command Execution

**Process Creation**:
1. **Fork process**: Create child process for external commands
2. **Setup environment**: Pass environment variables
3. **Handle redirections**: Set up file descriptors
4. **Execute command**: Call `execve()` with command path
5. **Wait for completion**: Parent waits for child to finish

**Path Resolution**:
1. Check if command is built-in
2. Check if command contains `/` (absolute/relative path)
3. Search in `PATH` environment variable
4. Return error if command not found

### 3. Pipeline Execution (`pipe.c`)

**Purpose**: Connect multiple commands through pipes, enabling data flow between processes.

**Pipeline Process**:
1. **Create pipes**: Establish communication channels
2. **Fork processes**: Create process for each command
3. **Connect pipes**: Set up stdin/stdout redirections
4. **Execute commands**: Run each command in its process
5. **Close descriptors**: Clean up pipe file descriptors
6. **Wait for all**: Wait for all processes to complete

**Key Functions**:
- `execute_pipeline()`: Main pipeline execution
- `setup_pipe_fds()`: Configure file descriptors for pipes
- `close_pipe_fds()`: Clean up pipe file descriptors

**Pipeline Example**:
```bash
Command: cat file.txt | grep pattern | sort | head -10

Execution:
Process 1: cat file.txt    → pipe1 →
Process 2: grep pattern    → pipe2 →
Process 3: sort            → pipe3 →
Process 4: head -10        → stdout
```

### 4. Here-document Handling (`heredoc.c`)

**Purpose**: Handle multi-line input terminated by a delimiter.

**Here-document Process**:
1. **Read input**: Collect lines until delimiter found
2. **Create pipe**: Set up temporary pipe for data
3. **Write to pipe**: Send collected input to pipe
4. **Redirect stdin**: Connect command stdin to pipe read end

**Key Functions**:
- `handle_heredoc()`: Main here-document handler
- `read_heredoc_input()`: Collects input lines
- `setup_heredoc_pipe()`: Creates and configures pipe

**Example**:
```bash
cat << EOF
This is line 1
This is line 2
EOF
```

### 5. I/O Redirection (`redirections.c`)

**Purpose**: Redirect command input/output to/from files.

**Redirection Implementation**:
- **File opening**: Open target files with appropriate modes
- **Descriptor duplication**: Use `dup2()` to redirect stdin/stdout
- **Error handling**: Check file permissions and existence
- **Cleanup**: Close file descriptors after execution

**Key Functions**:
- `setup_redirections()`: Main redirection setup
- `redirect_input()`: Handles input redirection
- `redirect_output()`: Handles output redirection
- `cleanup_redirections()`: Closes file descriptors

## Built-in Commands

Built-in commands are executed directly by the shell without creating new processes:

### 1. `echo` - Display Text
- **Purpose**: Print arguments to stdout
- **Options**: `-n` (no newline)
- **Implementation**: Direct output, handle escape sequences

### 2. `cd` - Change Directory
- **Purpose**: Change current working directory
- **Features**: Handle `~` (home), `-` (previous directory)
- **Implementation**: Use `chdir()`, update `PWD` and `OLDPWD`

### 3. `pwd` - Print Working Directory
- **Purpose**: Display current directory path
- **Implementation**: Use `getcwd()` or `PWD` environment variable

### 4. `export` - Set Environment Variables
- **Purpose**: Create or modify environment variables
- **Features**: Variable assignment, display current variables
- **Implementation**: Update internal environment list

### 5. `unset` - Remove Environment Variables
- **Purpose**: Delete environment variables
- **Implementation**: Remove from internal environment list

### 6. `env` - Display Environment
- **Purpose**: Show all environment variables
- **Implementation**: Iterate through environment list

### 7. `exit` - Terminate Shell
- **Purpose**: Exit the shell with optional status code
- **Implementation**: Set exit flag, cleanup resources

## Memory Management

### Strategy
1. **Allocation tracking**: Keep track of all dynamic allocations
2. **Cleanup functions**: Dedicated functions for freeing structures
3. **Error handling**: Cleanup on errors to prevent leaks
4. **Resource management**: Close file descriptors, free memory

### Key Cleanup Functions
- `free_tokens()`: Free token linked list
- `free_ast()`: Free AST nodes recursively
- `free_env()`: Free environment variable list
- `cleanup_data()`: Complete data structure cleanup

## Signal Handling

### Signals Handled
- **SIGINT (Ctrl+C)**: Interrupt current operation
- **SIGQUIT (Ctrl+\)**: Ignored in interactive mode
- **SIGTERM**: Graceful termination

### Implementation
- **Signal handlers**: Custom signal handling functions
- **State management**: Different behavior based on shell state
- **Process groups**: Proper signal propagation in pipelines

## Usage Examples

### Basic Commands
```bash
$ echo "Hello, World!"
Hello, World!

$ pwd
/home/user/minishell

$ ls -la
[directory listing]
```

### Pipes
```bash
$ cat file.txt | grep pattern | sort
[filtered and sorted output]

$ ps aux | grep minishell | head -5
[process information]
```

### Redirections
```bash
$ echo "Hello" > output.txt
$ cat < input.txt
$ echo "Append this" >> output.txt
$ wc -l << EOF
line 1
line 2
line 3
EOF
```

### Complex Commands
```bash
$ export VAR="test"
$ echo $VAR | tr 'a-z' 'A-Z' > upper.txt
$ cat upper.txt
TEST

$ cd /tmp && pwd && cd - && pwd
/tmp
/home/user/minishell
```

### Error Handling
```bash
$ nonexistent_command
minishell: nonexistent_command: command not found

$ cat nonexistent_file.txt
cat: nonexistent_file.txt: No such file or directory

$ echo "unclosed quote
minishell: syntax error: unclosed quote
```

## Technical Highlights

### 1. Recursive AST Processing
The AST is processed recursively, allowing for complex nested structures and proper operator precedence.

### 2. Proper File Descriptor Management
All file descriptors are carefully managed to prevent leaks and ensure proper I/O redirection.

### 3. Signal-Safe Operations
Signal handling is implemented to work correctly with ongoing operations and child processes.

### 4. Memory Efficiency
Dynamic allocation is used judiciously, with comprehensive cleanup to prevent memory leaks.

### 5. Error Recovery
The shell handles errors gracefully and continues operation after syntax or execution errors.

---

This minishell implementation demonstrates a deep understanding of Unix system programming concepts and provides a solid foundation for understanding how shells work internally. The modular design makes it easy to extend with additional features while maintaining code clarity and reliability.
