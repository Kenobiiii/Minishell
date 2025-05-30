# Minishell Unified Test Suite Documentation

## 📋 Overview

This unified test suite provides comprehensive testing for your Minishell implementation with a modular approach. You can run specific test modules or the complete suite.

## 🚀 Quick Start

### Run All Tests (Recommended)
```bash
# Compile your minishell first
make

# Run the complete test suite
./master_test.sh all
```

### Run Specific Test Modules
```bash
# Test only quote handling
./master_test.sh quotes

# Test signal handling (automated tests)
./master_test.sh signals

# Test pipe functionality
./master_test.sh pipes

# Test redirections
./master_test.sh redirections

# Test built-in commands
./master_test.sh builtins

# Run stress tests
./master_test.sh stress

# Run extreme edge cases
./master_test.sh extreme
```

### Advanced Usage with Options
```bash
# Run with valgrind memory checking
./master_test.sh quotes --valgrind

# Run with verbose output
./master_test.sh signals --verbose

# Run without colors (for CI/scripts)
./master_test.sh all --no-color

# Combine options
./master_test.sh stress --valgrind --verbose
```

## 📁 Unified Test Architecture

### Core Test Script: `master_test.sh`
- **Purpose**: Single entry point for all testing functionality
- **Features**: 
  - Modular test execution by category
  - Built-in valgrind integration
  - Color-coded results with optional disable
  - Verbose and quiet modes
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

### Test Results Format
```
Testing: Simple double quotes... PASSED
Testing: Variable expansion... FAILED
Testing: Memory leak test... PASSED (LEAK)
Testing: Segfault test... SEGFAULT
```

## 🛠️ Command Line Options

### Modules
- `quotes` - Quote and variable expansion tests
- `signals` - Signal handling tests (automated only)
- `pipes` - Pipe functionality tests
- `redirections` - File redirection tests
- `builtins` - Built-in command tests
- `stress` - Comprehensive stress testing
- `extreme` - Extreme edge cases
- `all` - Run all modules (default)

### Options
- `--valgrind` - Enable memory checking with valgrind
- `--no-color` - Disable colored output
- `--verbose` - Enable detailed output
- `--help` - Show help message

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
5. **Understand failures**: Use `--verbose` for detailed output

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

## 📚 Additional Resources

### Documentation
- [Bash Manual](https://www.gnu.org/software/bash/manual/)
- [Valgrind Documentation](https://valgrind.org/docs/manual/)
- [Signal Handling Guide](https://www.gnu.org/software/libc/manual/html_node/Signal-Handling.html)

### 42 School Resources
- [42 School Minishell Subject](https://github.com/42School/42-Cursus/tree/master/minishell)
- [Shell Implementation Guidelines](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/sh.html)

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

## 🎯 Test Categories

### Quote Handling Tests
- Basic single/double quotes
- Variable expansion rules
- Mixed quote scenarios
- Nested quotes
- Unclosed quotes (error handling)
- Empty quotes
- Quote removal logic

### Variable Expansion Tests
- Environment variables (`$HOME`, `$PATH`)
- Non-existent variables
- Special variables (`$?`, `$$`)
- Complex patterns (`${HOME}/test`)
- Multiple variables

### Pipe Tests
- Basic pipes (`cmd1 | cmd2`)
- Multiple pipe chains
- Pipes with quotes
- Empty pipe handling
- Long pipe chains (100+ stages)

### Redirection Tests
- Output redirection (`>`)
- Input redirection (`<`)
- Append redirection (`>>`)
- Multiple redirections
- Invalid file handling

### Command Tests
- Built-in commands (`pwd`, `env`, `echo`)
- External commands (`ls`, `cat`)
- Non-existent commands
- Commands with special characters

### Memory Safety Tests
- Memory leak detection
- Segfault prevention
- Double-free protection
- Buffer overflow protection
- Large allocation handling

### Edge Cases
- Empty commands
- Very long inputs
- Special characters
- Unicode handling
- Signal handling basics

## 📊 Understanding Results

### Exit Codes
- **0**: All tests passed ✅
- **1**: Critical failures (segfaults) 💥
- **2**: Warnings (memory leaks) ⚠️
- **3**: Minor failures ⚠️

### Color Coding
- 🟢 **Green**: Passed
- 🔴 **Red**: Critical failure
- 🟡 **Yellow**: Warning or minor issue
- 🔵 **Blue**: Information

### Memory Checking
All tests use valgrind to detect:
- Memory leaks
- Invalid memory access
- Use of uninitialized memory
- Double-free errors

## 🛠️ Requirements

### System Requirements
- `bash` shell
- `valgrind` (for memory checking)
- Standard Unix utilities (`cat`, `grep`, etc.)

### Minishell Requirements
- Executable named `minishell` in current directory
- Accepts input from stdin
- Supports `exit` command
- Basic quote and variable handling

## 🐛 Troubleshooting

### Common Issues

**1. "Minishell executable not found"**
```bash
# Solution: Compile your project
make
```

**2. "Permission denied" errors**
```bash
# Solution: Make scripts executable
chmod +x *.sh
```

**3. "Valgrind not found"**
```bash
# Ubuntu/Debian:
sudo apt-get install valgrind

# macOS:
brew install valgrind
```

**4. Tests hanging**
- Check if your minishell properly handles EOF
- Ensure `exit` command works
- Use Ctrl+C to interrupt if needed

### Interpreting Valgrind Output

**Memory Leaks:**
```
definitely lost: X bytes in Y blocks
```
- Fix: Ensure all malloc'd memory is freed

**Invalid Access:**
```
Invalid read/write of size X
```
- Fix: Check array bounds and pointer validity

**Uninitialized Memory:**
```
Use of uninitialised value
```
- Fix: Initialize all variables before use

## 🛡️ Readline Leak Suppression

All tests automatically suppress readline library leaks using the `readline.supp` suppression file. This is important because:

- **Readline leaks are NOT your responsibility** - They come from the system library
- **42 evaluators expect this** - Readline leaks should not count against your project
- **Standard practice** - Professional codebases always suppress known library leaks

### What's Suppressed:
- `readline()` function leaks
- `add_history()` leaks  
- `libreadline.so` library leaks
- Terminal capability (`termcap`/`ncurses`) leaks
- History and completion function leaks

### Verification:
```bash
# The tests will show "excluding readline" in memory leak reports
# Only YOUR code's memory leaks will be reported as failures
```

## 📈 Performance Expectations

### Normal Operation
- Basic tests: < 5 seconds
- Comprehensive tests: 30-60 seconds  
- Extreme tests: 1-2 minutes

### Memory Usage
- Should not exceed system limits
- No growing memory consumption
- Clean shutdown with no leaks

## 🎓 Test-Driven Development Tips

1. **Start Small**: Run `test_quotes.sh` first
2. **Fix Incrementally**: Address one failure at a time
3. **Use Valgrind**: Always check for memory issues
4. **Test Often**: Run tests after each change
5. **Read Output**: Understand what failed and why

## 📝 Adding Your Own Tests

You can extend the test suite by:

1. **Adding to existing scripts**:
   ```bash
   # Add to comprehensive_stress_test.sh
   run_test_with_valgrind "My test" "echo 'my command'" "expected output" 1
   ```

2. **Creating new test scripts**:
   ```bash
   # Create my_custom_test.sh
   # Add it to master_test.sh
   ```

## 🏆 Success Criteria

Your minishell is ready when:
- ✅ All test suites pass (exit code 0)
- ✅ No segfaults detected
- ✅ No memory leaks reported
- ✅ Handles edge cases gracefully
- ✅ Performance is reasonable

## 📚 Additional Resources

- [Valgrind Documentation](https://valgrind.org/docs/manual/)
- [Bash Manual](https://www.gnu.org/software/bash/manual/)
- [42 School Minishell Subject](https://github.com/42School/42-Cursus/tree/master/minishell)

---

**Happy Testing! 🚀**

*Remember: A well-tested minishell is a robust minishell.*
