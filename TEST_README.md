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
- `general` - General edge cases and comprehensive validation
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

### 1. **quotes** - Quote Handling Tests (32 tests, 7 levels)
**Test Levels:**
- **Level 1**: Basic Quotes - Simple single/double quotes, empty quotes
- **Level 2**: Variable Expansion - `$HOME`, `$USER`, undefined variables
- **Level 3**: Mixed Quotes - Adjacent quotes, nested combinations
- **Level 4**: Complex Variables - Variables with text, multiple expansions
- **Level 5**: Edge Cases - Long strings, spaced quotes, special scenarios
- **Level 6**: Stress Tests - Heavy nesting, special characters, Unicode
- **Level 7**: Pathological Cases - Quote bombs, expansion bombs, chaos scenarios

### 2. **signals** - Signal Handling Tests (25+ tests, 7 levels)
**Test Levels:**
- **Level 1**: Basic Execution - Simple commands, exit functionality
- **Level 2**: Process Control - Sleep commands, true/false, timing tests
- **Level 3**: Signal Infrastructure - Binary symbol checking, handler validation
- **Level 4**: Background Processes - Long-running command simulation
- **Level 5**: Signal Timing - Rapid commands, delay combinations
- **Level 6**: Stress Tests - Long processes, command bursts
- **Level 7**: Extreme Scenarios - Very long processes, signal timing bombs
- **Note**: Includes manual testing recommendations for interactive signals

### 3. **pipes** - Pipe Functionality Tests (48+ tests, 10 levels)
**Test Levels:**
- **Level 1**: Basic Pipes - Simple two-command pipes
- **Level 2**: Multiple Pipes - Three+ command chains
- **Level 3**: Different Commands - wc, head, tail integration
- **Level 4**: Complex Chains - Long pipe sequences, mixed commands
- **Level 5**: Edge Cases - Empty input, special characters, long strings
- **Level 6**: Stress Tests - Many stages, large data, multi-line processing
- **Level 7**: Pathological Cases - Extreme chains, memory stress, pipe bombs
- **Level 8**: Syntax Errors - Invalid pipe syntax detection and handling
- **Level 9**: Pipe-Redirection - Combined pipe and redirection scenarios
- **Level 10**: Extended Chains - Up to 20+ pipe stages with comprehensive testing

### 4. **redirections** - Redirection Tests (52+ tests, 12 levels)
**Test Levels:**
- **Level 1**: Basic Output - Simple output redirection (`>`)
- **Level 2**: Basic Input - Input redirection (`<`)
- **Level 3**: Append Operations - Append redirection (`>>`)
- **Level 4**: Complex Combinations - Input + output, pipes + redirects
- **Level 5**: Edge Cases - Long strings, special chars, variables
- **Level 6**: Stress Tests - Many appends, large files, multi-line data
- **Level 7**: Pathological Cases - Extreme chains, memory stress, redirect bombs
- **Level 8**: Here Documents - Heredoc testing (if supported by implementation)
- **Level 9**: Syntax Errors - Invalid redirection syntax detection
- **Level 10**: File System Edge Cases - Permission testing, special files
- **Level 11**: Combined Operations - Complex pipe-redirection combinations
- **Level 12**: Stress Testing - Large file handling and resource limits

### 5. **builtins** - Built-in Command Tests (65+ tests, 13 levels)
**Test Levels:**
- **Level 1**: Basic Echo - Simple echo commands, basic functionality
- **Level 2**: Echo with -n - Echo flag testing and argument handling
- **Level 3**: Echo Quoting - Complex quote scenarios with echo
- **Level 4**: Echo Variables - Variable expansion in echo commands
- **Level 5**: PWD Command - Directory operations and pipe integration
- **Level 6**: CD Command - Directory navigation, error handling, special paths
- **Level 7**: Export Command - Variable assignment, validation, edge cases
- **Level 8**: ENV Command - Environment variable display and functionality
- **Level 9**: UNSET Command - Variable removal and comprehensive testing
- **Level 10**: EXIT Command - Clean termination and exit code validation
- **Level 11**: Complex Combinations - Mixed builtin operations and interactions
- **Level 12**: Advanced Scenarios - Environment manipulation and edge cases
- **Level 13**: Pathological Cases - Stress tests, special characters, memory pressure

### 6. **general** - General Edge Cases (70+ tests, 15 levels) 🆕
**Test Levels:**
- **Level 1**: Empty Input - Empty commands, whitespace handling
- **Level 2**: Unclosed Quotes - Syntax error detection and recovery
- **Level 3**: Special Characters - Character escaping and special symbols
- **Level 4**: Command Not Found - Invalid command handling and error messages
- **Level 5**: PATH Manipulation - PATH variable testing and command resolution
- **Level 6**: Variable Expansion Edge Cases - Complex variable scenarios
- **Level 7**: Argument Parsing - Edge cases in argument processing
- **Level 8**: Control Characters - Non-printable character handling
- **Level 9**: Redirection Edge Cases - Special redirection scenarios
- **Level 10**: Signal Edge Cases - Signal handling in various contexts
- **Level 11**: Memory and Resource Limits - Resource exhaustion testing
- **Level 12**: Syntax Error Combinations - Multiple syntax error scenarios
- **Level 13**: Unicode and Special Encoding - Character encoding edge cases
- **Level 14**: File System Edge Cases - Special files and permission scenarios
- **Level 15**: Error Recovery - Recovery testing after various error conditions

### 7. **stress** - Comprehensive Stress Tests (32 tests, 8 levels)
**Test Levels:**
- **Level 1**: Basic Stress - Medium strings, many arguments
- **Level 2**: String Length - 100-200 character strings
- **Level 3**: Quote Complexity - Nested quotes, variable mixing
- **Level 4**: Command Chaining - Multiple command execution
- **Level 5**: Memory Pressure - Large strings, argument explosions
- **Level 6**: Parser Stress - Complex parsing, mixed operators
- **Level 7**: Extreme Stress - 300+ character strings, massive arguments
- **Level 8**: Pathological Cases - Command bombs, memory fragmentation

### 8. **extreme** - Extreme Edge Cases (36 tests, 9 levels) ⚠️
**Test Levels:**
- **Level 1**: Memory Bombs - Allocation stress tests (200+ repetitions)
- **Level 2**: Parser Destruction - Quote nightmares, variable explosions
- **Level 3**: File System Stress - Massive redirect chains, large files
- **Level 4**: Command Complexity - Length explosions, pipe bombs
- **Level 5**: Memory Leak Hunters - 500+ allocations, fragmentation attacks
- **Level 6**: Edge Case Nightmares - Unicode, special chars, binary data
- **Level 7**: Resource Exhaustion - CPU intensive parsing, memory pressure
- **Level 8**: Ultimate Destroyers - Quote apocalypse, memory destroyers
- **Level 9**: Recovery Tests - Post-stress functionality verification
- **⚠️ WARNING**: Designed to potentially break minishell for testing robustness!

## 📊 Test Suite Statistics

### Total Test Coverage
- **Overall Tests**: 350+ individual test cases
- **Structured Levels**: 67 difficulty levels across all modules
- **Memory Safety**: Full valgrind integration with leak detection
- **Stress Testing**: Designed to reveal edge cases and memory issues

### Test Distribution by Module
- **quotes**: 32 tests (7 levels) - Quote handling and variable expansion
- **signals**: 25+ tests (7 levels) - Signal handling and process control
- **pipes**: 48+ tests (10 levels) - Pipe functionality and chaining
- **redirections**: 52+ tests (12 levels) - File redirection operations
- **builtins**: 65+ tests (13 levels) - Built-in command functionality
- **general**: 70+ tests (15 levels) - General edge cases and comprehensive validation
- **stress**: 32 tests (8 levels) - Comprehensive stress testing
- **extreme**: 36 tests (9 levels) - Extreme edge cases and destruction tests

### Difficulty Progression
Each module follows a carefully designed progression:
1. **Basic functionality** - Core features that must work
2. **Standard scenarios** - Common usage patterns
3. **Edge cases** - Boundary conditions and special inputs
4. **Complex combinations** - Multiple features working together
5. **Stress scenarios** - High load and large data testing
6. **Memory pressure** - Tests designed to reveal memory leaks
7. **Pathological cases** - Extreme scenarios that could break the shell
8. **Recovery verification** - Ensuring the shell remains functional after stress

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
- 🔵 **Blue**: Information/Level headers
- 🟣 **Purple**: Module summary

### Enhanced Test Output Format
```
===============================================
            QUOTES MODULE
Testing quote handling and variable expansion
===============================================

Level 1: Basic Quotes
Testing: Simple double quotes
Command: echo "hello world"
PASSED
----------------------------------------
Testing: Variable expansion bomb
Command: echo "$HOME$HOME$HOME$HOME$HOME"
Expected: /home/user/home/user/home/user/home/user/home/user
Got: /home/user/home/user/home/user/home/user/home/user
PASSED
----------------------------------------
Testing: Memory destroyer
Command: echo "destroyer_destroyer_destroyer_..."
PASSED
Memory Status: Clean (No leaks detected)
----------------------------------------

Quotes Module Results: 32 passed, 0 failed, 0 leaks, 0 segfaults
```

### Performance Expectations
- **Basic modules** (quotes, signals, pipes): ~30-60 seconds
- **Complex modules** (redirections, builtins): ~45-90 seconds  
- **Stress module**: ~60-120 seconds
- **Extreme module**: ~90-180 seconds (with timeouts for pathological cases)
- **Full suite**: ~5-15 minutes depending on system performance

## 🎯 Testing Methodology

### Design Philosophy
This test suite uses a **structured difficulty architecture** where each module starts with basic functionality and gradually increases complexity to stress-test edge cases and reveal potential memory leaks or crashes.

### Level Strategy
1. **Levels 1-2**: Basic functionality verification
2. **Levels 3-4**: Standard usage patterns and combinations
3. **Levels 5-6**: Edge cases and stress testing
4. **Levels 7-8**: Pathological cases designed to break implementations
5. **Level 9**: Recovery and survival verification

### Memory Leak Detection Strategy
- **Early levels**: Simple commands to verify basic memory management
- **Middle levels**: Complex scenarios that exercise memory allocation/deallocation
- **Late levels**: Memory bombs designed to reveal leaks through repeated allocations
- **Extreme levels**: Memory fragmentation and exhaustion attempts

### Why This Approach?
- **Incremental debugging**: Failed early tests indicate fundamental issues
- **Targeted stress testing**: Later levels specifically target common minishell vulnerabilities
- **Real-world simulation**: Progression mirrors how shells are actually used
- **Quality assurance**: Ensures robustness under extreme conditions

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

## 🔧 Troubleshooting Guide

### Common Issues and Solutions

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

**5. Memory issues in extreme module**
- Expected behavior: Some stress tests may reveal memory leaks
- Use valgrind to identify specific leak sources
- Focus on basic modules first, then tackle extreme cases

**6. High failure rate in later levels**
- Normal for stress testing - later levels are designed to be challenging
- Focus on passing levels 1-4 in each module first
- Levels 5+ are stress tests that may expose edge case bugs

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

**Test Level Results:**
- **Levels 1-2 failures**: Basic functionality issues
- **Levels 3-4 failures**: Edge case handling problems  
- **Levels 5-6 failures**: Stress testing reveals limits
- **Levels 7+ failures**: Expected in pathological scenarios

### Debugging Strategy by Module

**quotes module failures:**
- Check variable expansion (`$HOME`, `$USER`)
- Verify quote parsing (single vs double quotes)
- Test with simple quotes first, then complex nesting

**signals module failures:**
- Verify signal handler installation
- Check binary symbols with `nm minishell | grep signal`
- Test basic command execution before signal handling

**pipes module failures:**
- Test simple two-command pipes first
- Check file descriptor management
- Verify command execution in pipeline

**redirections module failures:**
- Test basic output redirection (`echo "test" > file`)
- Check file permissions and creation
- Verify input redirection works

**builtins module failures:**
- Test each builtin individually
- Check echo with and without `-n` flag
- Verify environment variable handling

**stress/extreme module failures:**
- Expected for some tests - these push limits
- Focus on earlier levels first
- Use valgrind to identify memory issues

## 📈 Performance Expectations

### Normal Operation (Updated for Enhanced Suite)
- **quotes module**: 10-30 seconds (32 tests)
- **signals module**: 15-45 seconds (25+ tests, includes timeouts)
- **pipes module**: 15-40 seconds (48+ tests)
- **redirections module**: 20-50 seconds (52+ tests, file I/O)
- **builtins module**: 15-45 seconds (65+ tests)
- **general module**: 25-60 seconds (70+ tests, comprehensive edge cases)
- **stress module**: 30-90 seconds (32 stress tests)
- **extreme module**: 60-180 seconds (36 extreme tests, long timeouts)
- **Complete test suite**: 8-20 minutes depending on system performance

## 🎉 Summary of Test Suite Enhancements

The Minishell test suite has been dramatically enhanced from 36 basic tests to **350+ comprehensive tests** with structured difficulty architecture. This represents a **972% increase** in test coverage.

### Key Achievements
- ✅ **Unified Architecture**: Single `master_test.sh` script replaces 8+ separate test scripts
- ✅ **Structured Difficulty**: 67 difficulty levels across all modules (7-15 levels per module)
- ✅ **Comprehensive Coverage**: 350+ individual test cases covering all minishell functionality
- ✅ **Memory Safety**: Full valgrind integration with automatic leak detection
- ✅ **Stress Testing**: Advanced tests designed to reveal edge cases and memory issues
- ✅ **Pathological Testing**: Extreme scenarios that push minishell to its limits
- ✅ **Modular Execution**: Run specific test categories or complete suite
- ✅ **Enhanced Documentation**: Complete usage guide and troubleshooting

### Before vs After
| Aspect | Before | After | Improvement |
|--------|--------|--------|-------------|
| Test Scripts | 8 separate files | 1 unified script | 87% reduction in files |
| Total Tests | 36 basic tests | 350+ structured tests | 972% increase |
| Difficulty Levels | 1 level (basic) | 67 levels (7-15 per module) | 6700% increase |
| Memory Testing | Basic | Advanced valgrind integration | Full leak detection |
| Documentation | Scattered | Unified comprehensive guide | Complete rewrite |
| Edge Cases | Minimal | Extensive pathological testing | Extreme coverage |

### Test Distribution
- **quotes**: 32 tests (7 levels) - Quote handling mastery
- **signals**: 25+ tests (7 levels) - Signal processing verification  
- **pipes**: 48+ tests (10 levels) - Pipeline functionality stress
- **redirections**: 52+ tests (12 levels) - File I/O comprehensive testing
- **builtins**: 65+ tests (13 levels) - Built-in command validation
- **general**: 70+ tests (15 levels) - General edge cases and comprehensive validation
- **stress**: 32 tests (8 levels) - Performance and reliability testing
- **extreme**: 36 tests (9 levels) - Destruction testing for robustness

### Success Criteria
- **Levels 1-2**: Must pass (basic functionality)
- **Levels 3-4**: Should pass (standard usage)
- **Levels 5-6**: May fail (stress testing reveals limits)
- **Levels 7+**: Expected failures (pathological edge cases)

The enhanced test suite now provides industry-standard testing coverage that can reveal memory leaks, edge case failures, and implementation weaknesses while maintaining full backward compatibility with existing minishell projects.
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
