# Minishell Test Suite Documentation

## 📋 Overview

This comprehensive test suite is designed to thoroughly test your Minishell implementation for:
- ✅ **Functionality correctness**
- 🛡️ **Memory safety** (no leaks, no segfaults)
- 🏋️ **Robustness** under extreme conditions
- 🔧 **Edge case handling**

## 🚀 Quick Start

### Run All Tests (Recommended)
```bash
# Compile your minishell first
make

# Run the complete test suite
./master_test.sh
```

### Run Individual Test Suites
```bash
# Basic quote functionality tests
./test_quotes.sh

# Final validation tests  
./final_test.sh

# Comprehensive tests with memory checking
./comprehensive_stress_test.sh

# Extreme edge cases and performance tests
./extreme_stress_test.sh
```

## 📁 Test Files Description

### 1. `master_test.sh` - Master Test Controller
- **Purpose**: Orchestrates all test suites
- **Features**: 
  - Runs all available test scripts
  - Provides comprehensive summary
  - Color-coded results
  - Exit codes indicate overall status

### 2. `test_quotes.sh` - Basic Quote Tests
- **Purpose**: Tests fundamental quote handling
- **Coverage**:
  - Single quotes (`'text'`)
  - Double quotes (`"text"`)
  - Variable expansion in quotes
  - Mixed quote scenarios
  - Empty quotes

### 3. `final_test.sh` - Validation Tests
- **Purpose**: Final validation of fixed functionality
- **Coverage**:
  - Previously failing test cases
  - Regression testing
  - Memory leak validation with valgrind

### 4. `comprehensive_stress_test.sh` - Comprehensive Testing
- **Purpose**: Extensive testing with memory safety checks
- **Coverage**:
  - Quote stress tests (100+ scenarios)
  - Variable expansion edge cases
  - Pipe combinations
  - Redirection tests
  - Command execution tests
  - Complex combinations
  - Memory stress tests
  - Syntax error handling
- **Tools**: Uses valgrind for memory checking

### 5. `extreme_stress_test.sh` - Extreme Edge Cases
- **Purpose**: Push the limits of your implementation
- **Coverage**:
  - Massive string handling (10K+ characters)
  - Extreme nesting scenarios
  - Memory exhaustion tests
  - Performance boundaries
  - Unicode and special characters
  - Timeout protection (prevents hangs)

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
