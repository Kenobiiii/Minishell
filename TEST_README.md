# Minishell Unified Test Suite Documentation

## 📋 Overview

This unified test suite provides comprehensive testing for your Minishell implementation with a modular approach. You can run specific test modules or the complete suite.

## 🚀 Quick Start & Usage

### Basic Execution
```bash
# Compile your minishell first
make

# Run the complete test suite (default)
./master_test.sh
./master_test.sh all

# Run specific test modules
./master_test.sh quotes
./master_test.sh signals
./master_test.sh pipes
./master_test.sh redirections
./master_test.sh builtins
./master_test.sh stress
./master_test.sh extreme
```

### Command Line Options
```bash
# Run with valgrind memory checking
./master_test.sh quotes --valgrind

# Run without colors (for CI/scripts)
./master_test.sh all --no-color

# Show help
./master_test.sh --help

# Combine options
./master_test.sh stress --valgrind --no-color
```

### Available Modules
- `quotes` - Quote and variable expansion tests
- `signals` - Signal handling tests (automated only)
- `pipes` - Pipe functionality tests
- `redirections` - File redirection tests
- `builtins` - Built-in command tests
- `stress` - Comprehensive stress testing
- `extreme` - Extreme edge cases
- `all` - Run all modules (default)

### Available Options
- `--valgrind` - Enable memory checking with valgrind
- `--no-color` - Disable colored output
- `--help` - Show help message

## 📁 Test Architecture

### Core Test Script: `master_test.sh`
- **Purpose**: Single entry point for all testing functionality
- **Features**: 
  - Modular test execution by category
  - Built-in valgrind integration
  - Color-coded results with optional disable
  - Detailed output by default
  - Comprehensive error reporting
  - Memory leak detection (excluding readline)
  - Timeout protection

## 🎯 Available Test Modules

### 1. **quotes** - Quote Handling Tests
- **Coverage**:
  - Basic single quotes (`'text'`)
  - Basic double quotes (`"text"`)
  - Variable expansion in quotes
  - Mixed quote scenarios (`"hello"'world'`)
  - Nested quotes
  - Empty quotes
  - Multiple quoted strings
  - Edge cases and complex patterns

### 2. **signals** - Signal Handling Tests (Automated)
- **Coverage**:
  - Basic command execution verification
  - Exit command functionality
  - Signal handling infrastructure validation
  - Binary symbol checking (signal functions)
  - Timeout handling for long-running commands
- **Note**: Manual testing required for interactive signal behavior

### 3. **pipes** - Pipe Functionality Tests
- **Coverage**:
  - Simple pipes (`cmd1 | cmd2`)
  - Multiple pipe chains (`cmd1 | cmd2 | cmd3`)
  - Pipes with built-in commands
  - Echo to external commands via pipes
  - Error handling in pipe chains

### 4. **redirections** - Redirection Tests
- **Coverage**:
  - Output redirection (`>`)
  - Input redirection (`<`)
  - Append redirection (`>>`)
  - Multiple redirections
  - File creation and access
  - Error handling for invalid files

### 5. **builtins** - Built-in Command Tests
- **Coverage**:
  - `echo` command (with and without -n flag)
  - `pwd` command
  - `env` command
  - `cd` command
  - Built-in vs external command resolution

### 6. **stress** - Comprehensive Stress Tests
- **Coverage**:
  - Long string handling (100+ characters)
  - Many command arguments
  - Complex mixed quote scenarios
  - Multiple command execution
  - Performance under load

### 7. **extreme** - Extreme Edge Cases
- **Coverage**:
  - Very long command lines
  - Special characters (`!@#$%^&*()`)
  - Empty command handling
  - Whitespace edge cases
  - Unicode handling basics
  - Performance boundaries

## 📊 Understanding Results

### Exit Codes
- **0**: All tests passed ✅
- **1**: Critical failures (segfaults) 💥
- **2**: Some tests failed ⚠️
- **3**: Memory leaks detected ⚠️

### Color Coding
- 🟢 **Green**: Test passed
- 🔴 **Red**: Critical failure or test failed
- 🟡 **Yellow**: Warning (memory leak or minor issue)
- 🔵 **Blue**: Information
- 🟣 **Purple**: Module summary

### Test Output Format
```
Testing: Simple double quotes
Command: echo "hello world"
PASSED
----------------------------------------
Testing: Variable expansion
Command: echo "$HOME"
FAILED
Expected: /home/user
Got: $HOME
----------------------------------------
Testing: Memory leak test
Command: echo "test"
PASSED (LEAK)
----------------------------------------
```

## 🛠️ System Requirements

### Required Software
- `bash` shell (for test execution)
- `valgrind` (optional, for `--valgrind` flag)
- Standard Unix utilities (`cat`, `grep`, `ls`, etc.)
- `nm` utility (for binary symbol checking)

### Minishell Requirements
- Executable named `minishell` in current directory
- Accepts input from stdin
- Supports `exit` command for clean termination
- Implements basic shell functionality

## 🔍 Memory Safety Testing

### Automatic Memory Checking
When using `--valgrind`, the test suite automatically:
- Detects memory leaks
- Identifies invalid memory access
- Catches use of uninitialized memory
- Excludes readline library leaks (not your responsibility)

### Readline Leak Suppression
All tests automatically suppress readline library leaks using built-in suppressions:
- `readline()` function leaks
- `add_history()` leaks  
- `libreadline.so` library leaks
- Terminal capability (`termcap`/`ncurses`) leaks

### Memory Error Types Detected
- **Segmentation faults**: Critical crashes
- **Memory leaks**: Unreleased allocated memory
- **Invalid reads/writes**: Buffer overflows
- **Double-free errors**: Freeing memory twice
- **Use of uninitialized memory**: Reading before writing

## 🧪 Manual Testing Recommendations

### Signal Handling (Interactive)
Since some signal behaviors require manual testing:

```bash
# Start minishell
./minishell

# Test Ctrl+C at prompt (should show new prompt line)
[Press Ctrl+C]

# Test Ctrl+\ at prompt (should be completely ignored)
[Press Ctrl+\]

# Test Ctrl+\ during execution (should show "Quit")
sleep 10
[Press Ctrl+\]

# Exit
exit
```

### Advanced Manual Tests
```bash
# Complex quote scenarios
./minishell
echo "hello"'world'"test"

# Pipe combinations
ls | grep minishell | cat

# Redirection chains
echo "test" > file.txt && cat file.txt

# Environment variables
echo $HOME
echo "$HOME"'$HOME'

# Built-in commands
pwd
cd /tmp
pwd
cd -
```

## 🛡️ Requirements

### System Requirements
- `bash` shell (for test execution)
- `valgrind` (optional, for `--valgrind` flag)
- Standard Unix utilities (`cat`, `grep`, `ls`, etc.)
- `nm` utility (for binary symbol checking)

### Minishell Requirements
- Executable named `minishell` in current directory
- Accepts input from stdin
- Supports `exit` command for clean termination
- Implements basic shell functionality

## 🐛 Troubleshooting

### Common Issues

**1. "Minishell executable not found"**
```bash
# Solution: Compile your project
make clean && make
```

**2. "Permission denied"**
```bash
# Solution: Make test script executable
chmod +x master_test.sh
```

**3. "Valgrind not found"**
```bash
# Ubuntu/Debian:
sudo apt-get install valgrind

# macOS (if supported):
brew install valgrind

# Or run without valgrind:
./master_test.sh quotes
```

**4. Tests hanging or timing out**
- Check if your minishell handles EOF properly
- Ensure `exit` command works correctly
- Use Ctrl+C to interrupt if needed
- Check for infinite loops in signal handling

### Interpreting Test Output

**Memory Leaks (with --valgrind):**
```
PASSED (LEAK) - Test passed but memory leak detected
```

**Critical Failures:**
```
SEGFAULT - Segmentation fault occurred
TIMEOUT - Test exceeded time limit
```

**Test Failures:**
```
FAILED - Expected output didn't match actual output
Expected: hello world
Got: hello
```

## 📈 Performance Expectations

### Normal Operation
- Individual modules: 5-15 seconds
- Complete test suite: 30-90 seconds
- With valgrind: 2-5x longer execution time

### Memory Usage
- Should not exceed reasonable system limits
- No growing memory consumption over time
- Clean shutdown with proper cleanup

## 🎓 Best Practices for Testing

### Development Workflow
1. **Start with basic modules**: `./master_test.sh quotes`
2. **Fix issues incrementally**: Address one module at a time
3. **Use valgrind regularly**: `./master_test.sh quotes --valgrind`
4. **Test frequently**: Run tests after each major change
5. **Understand failures**: Read detailed output carefully

### Pre-submission Checklist
- [ ] `./master_test.sh all` passes (exit code 0)
- [ ] `./master_test.sh all --valgrind` shows no memory leaks
- [ ] Manual signal testing completed
- [ ] All edge cases handle gracefully
- [ ] Performance is reasonable

## 🏆 Success Criteria

Your minishell is ready when:
- ✅ All test modules pass (exit code 0)
- ✅ No segfaults detected in any scenario
- ✅ No memory leaks reported (excluding readline)
- ✅ Manual signal tests behave correctly
- ✅ Handles edge cases gracefully
- ✅ Performance meets expectations

---

## 🚀 Signal Handling Implementation

### Completed Features ✅
Based on the recent signal handling redesign, your minishell now includes:

#### Dynamic Signal Configuration
- **Prompt Mode**: SIGQUIT ignored (`SIG_IGN`)
- **Execution Mode**: Custom SIGQUIT handler active
- **Automatic Switching**: Signals switch based on shell state

#### Bash-Compatible Behavior
| Signal | At Prompt | During Execution |
|--------|-----------|------------------|
| Ctrl+C (SIGINT) | New prompt line | Terminate + new prompt |
| Ctrl+\ (SIGQUIT) | **IGNORED** (silent) | Terminate + "Quit" + new prompt |

#### Key Functions Implemented
- `setup_signals()` - Prompt mode signal configuration
- `setup_signals_for_execution()` - Execution mode signals
- `handle_sigquit()` - Custom SIGQUIT handler
- `analyse_status()` - Enhanced with SIGQUIT detection and "Quit" message

### Manual Signal Verification
The automated tests verify signal infrastructure, but manual testing is required:

```bash
# 1. Test Ctrl+\ at prompt (should be completely silent)
./minishell
[Press Ctrl+\] # No output should appear

# 2. Test Ctrl+\ during execution (should show "Quit")
sleep 10
[Press Ctrl+\] # Should display "Quit" and terminate

# 3. Test Ctrl+C behaviors
[At prompt: Press Ctrl+C] # Should show new prompt
sleep 10
[Press Ctrl+C] # Should terminate and show new prompt
```

---

**Happy Testing! 🚀**

*A well-tested minishell is a robust minishell. Use this unified test suite to ensure your implementation meets the highest standards.*
