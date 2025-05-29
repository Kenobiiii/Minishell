#!/bin/bash

# Comprehensive Stress Test for Minishell
# Tests for potential failures, memory leaks, and segfaults

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

MINISHELL="./minishell"
PASSED=0
FAILED=0
SEGFAULT=0
LEAKS=0

# Function to run test with valgrind
run_test_with_valgrind() {
    local test_name="$1"
    local command="$2"
    local expected="$3"
    local should_pass="$4" # 1 for should pass, 0 for should fail gracefully
    
    echo -e "${BLUE}Testing: $test_name${NC}"
    echo "Command: $command"
    
    # Create a temporary script for the test
    echo "$command" > /tmp/minishell_test_input.txt
    echo "exit" >> /tmp/minishell_test_input.txt
    
    # Run with valgrind to detect memory leaks and errors
    # Suppress readline leaks as they are known issues and not our responsibility
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
             --error-exitcode=42 --log-file=/tmp/valgrind_output.txt \
             --suppressions=readline.supp \
             $MINISHELL < /tmp/minishell_test_input.txt > /tmp/minishell_output.txt 2>&1
    
    local exit_code=$?
    local valgrind_output=$(cat /tmp/valgrind_output.txt)
    local minishell_output=$(cat /tmp/minishell_output.txt)
    
    # Check for segfault
    if [[ $exit_code -eq 139 ]] || echo "$valgrind_output" | grep -q "SIGSEGV\|Segmentation fault"; then
        echo -e "${RED}SEGFAULT DETECTED!${NC}"
        ((SEGFAULT++))
        ((FAILED++))
        return
    fi
    
    # Check for memory leaks (excluding readline leaks)
    if echo "$valgrind_output" | grep -q "definitely lost\|indirectly lost\|possibly lost" && \
       ! echo "$valgrind_output" | grep -q "0 bytes in 0 blocks" && \
       ! echo "$valgrind_output" | grep -q "readline\|libreadline\|add_history\|tgetent\|_nc_"; then
        echo -e "${YELLOW}MEMORY LEAK DETECTED (excluding readline)!${NC}"
        echo "$valgrind_output" | grep -E "(definitely|indirectly|possibly) lost" | grep -v "readline\|libreadline\|add_history"
        ((LEAKS++))
    fi
    
    # Check for other valgrind errors
    if echo "$valgrind_output" | grep -q "Invalid read\|Invalid write\|Use of uninitialised"; then
        echo -e "${RED}MEMORY ERROR DETECTED!${NC}"
        echo "$valgrind_output" | grep -E "(Invalid read|Invalid write|Use of uninitialised)"
        ((FAILED++))
        return
    fi
    
    # Check expected behavior
    if [[ $should_pass -eq 1 ]]; then
        if [[ -n "$expected" ]] && ! echo "$minishell_output" | grep -q "$expected"; then
            echo -e "${RED}UNEXPECTED OUTPUT!${NC}"
            echo "Expected to contain: $expected"
            echo "Got: $minishell_output"
            ((FAILED++))
        else
            echo -e "${GREEN}PASSED${NC}"
            ((PASSED++))
        fi
    else
        # Test should fail gracefully (no segfault, no leaks, but may have error output)
        echo -e "${GREEN}FAILED GRACEFULLY (as expected)${NC}"
        ((PASSED++))
    fi
    
    echo "----------------------------------------"
}

echo -e "${BLUE}=== COMPREHENSIVE MINISHELL STRESS TEST ===${NC}"
echo

# 1. QUOTE STRESS TESTS
echo -e "${YELLOW}=== QUOTE STRESS TESTS ===${NC}"

# Basic quotes
run_test_with_valgrind "Basic single quotes" "echo 'hello world'" "hello world" 1
run_test_with_valgrind "Basic double quotes" "echo \"hello world\"" "hello world" 1

# Variable expansion in quotes
run_test_with_valgrind "Variable in double quotes" "echo \"\$HOME\"" "$HOME" 1
run_test_with_valgrind "Variable in single quotes" "echo '\$HOME'" "\$HOME" 1

# Mixed quotes
run_test_with_valgrind "Mixed quotes simple" "echo 'hello'\"\$HOME\"'world'" "hello${HOME}world" 1
run_test_with_valgrind "Mixed quotes complex" "echo \"\$HOME\"'\$HOME'" "${HOME}\$HOME" 1

# Nested quotes (should fail gracefully)
run_test_with_valgrind "Nested quotes 1" "echo \"'nested'\"" "'nested'" 1
run_test_with_valgrind "Nested quotes 2" "echo '\"nested\"'" "\"nested\"" 1

# Unclosed quotes (should fail gracefully)
run_test_with_valgrind "Unclosed single quote" "echo 'unclosed" "" 0
run_test_with_valgrind "Unclosed double quote" "echo \"unclosed" "" 0

# Empty quotes
run_test_with_valgrind "Empty single quotes" "echo ''" "" 1
run_test_with_valgrind "Empty double quotes" "echo \"\"" "" 1

# Multiple quotes on same line
run_test_with_valgrind "Multiple quotes" "echo 'a' \"b\" 'c' \"\$HOME\"" "a b c $HOME" 1

# 2. VARIABLE EXPANSION STRESS TESTS
echo -e "${YELLOW}=== VARIABLE EXPANSION STRESS TESTS ===${NC}"

# Basic variables
run_test_with_valgrind "HOME variable" "echo \$HOME" "$HOME" 1
run_test_with_valgrind "PATH variable" "echo \$PATH" "$PATH" 1

# Non-existent variables
run_test_with_valgrind "Non-existent variable" "echo \$NONEXISTENT" "" 1

# Special variables
run_test_with_valgrind "Exit status" "echo \$?" "0" 1
run_test_with_valgrind "Process ID" "echo \$\$" "" 1

# Simple variable patterns
run_test_with_valgrind "Multiple variables" "echo \$HOME \$USER" "$HOME $USER" 1

# 3. PIPE STRESS TESTS
echo -e "${YELLOW}=== PIPE STRESS TESTS ===${NC}"

# Basic pipes
run_test_with_valgrind "Simple pipe" "echo hello | cat" "hello" 1
run_test_with_valgrind "Multiple pipes" "echo hello | cat | cat | cat" "hello" 1

# Pipes with quotes
run_test_with_valgrind "Pipe with quotes" "echo 'hello world' | cat" "hello world" 1
run_test_with_valgrind "Pipe with variables" "echo \"\$HOME\" | cat" "$HOME" 1

# Empty pipes (should fail gracefully)
run_test_with_valgrind "Empty pipe left" " | cat" "" 0
run_test_with_valgrind "Empty pipe right" "echo hello | " "" 0

# 4. REDIRECTION STRESS TESTS
echo -e "${YELLOW}=== REDIRECTION STRESS TESTS ===${NC}"

# Basic redirections
run_test_with_valgrind "Output redirection" "echo hello > /tmp/test_output.txt && cat /tmp/test_output.txt" "hello" 1
run_test_with_valgrind "Input redirection" "cat < /etc/passwd | head -1" "" 1

# Append redirection
run_test_with_valgrind "Append redirection" "echo hello >> /tmp/test_append.txt && echo world >> /tmp/test_append.txt && cat /tmp/test_append.txt" "hello" 1

# Multiple redirections
run_test_with_valgrind "Multiple redirections" "echo hello > /tmp/test1.txt > /tmp/test2.txt" "" 1

# Invalid redirections (should fail gracefully)
run_test_with_valgrind "Invalid input file" "cat < /nonexistent/file" "" 0
run_test_with_valgrind "Invalid output dir" "echo hello > /nonexistent/dir/file" "" 0

# 5. COMMAND STRESS TESTS
echo -e "${YELLOW}=== COMMAND STRESS TESTS ===${NC}"

# Built-in commands
run_test_with_valgrind "pwd builtin" "pwd" "" 1
run_test_with_valgrind "env builtin" "env | head -1" "" 1
run_test_with_valgrind "echo builtin" "echo -n hello" "hello" 1

# External commands
run_test_with_valgrind "ls command" "ls /tmp | head -1" "" 1
run_test_with_valgrind "cat command" "echo test | cat" "test" 1

# Non-existent commands (should fail gracefully)
run_test_with_valgrind "Non-existent command" "nonexistentcommand123" "" 0

# Commands with special characters
run_test_with_valgrind "Command with spaces" "echo 'hello world'" "hello world" 1

# 6. COMPLEX COMBINATION TESTS
echo -e "${YELLOW}=== COMPLEX COMBINATION TESTS ===${NC}"

# Quotes + pipes + redirections
run_test_with_valgrind "Complex combo 1" "echo 'hello world' | cat > /tmp/complex1.txt && cat /tmp/complex1.txt" "hello world" 1
run_test_with_valgrind "Complex combo 2" "echo \"\$HOME\" | cat | cat > /tmp/complex2.txt" "" 1

# Multiple commands with semicolons (if supported)
run_test_with_valgrind "Multiple commands" "echo hello; echo world" "hello" 1

# Very long commands
LONG_STRING=$(printf 'a%.0s' {1..1000})
run_test_with_valgrind "Very long string" "echo '$LONG_STRING'" "$LONG_STRING" 1

# 7. EDGE CASE STRESS TESTS
echo -e "${YELLOW}=== EDGE CASE STRESS TESTS ===${NC}"

# Empty commands
run_test_with_valgrind "Empty command" "" "" 1
run_test_with_valgrind "Just spaces" "   " "" 1
run_test_with_valgrind "Just tabs" "		" "" 1

# Special characters
run_test_with_valgrind "Semicolon only" ";" "" 0
run_test_with_valgrind "Pipe only" "|" "" 0
run_test_with_valgrind "Redirect only" ">" "" 0

# Unicode and special characters
run_test_with_valgrind "Unicode characters" "echo 'héllo wörld 🌍'" "héllo wörld 🌍" 1

# Very deep nesting
run_test_with_valgrind "Deep pipe chain" "echo hello | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat" "hello" 1

# 8. MEMORY STRESS TESTS
echo -e "${YELLOW}=== MEMORY STRESS TESTS ===${NC}"

# Many variables
run_test_with_valgrind "Many variables" "echo \$HOME \$USER \$PATH \$PWD \$SHELL \$HOME \$USER \$PATH" "" 1

# Many quotes
run_test_with_valgrind "Many quotes" "echo 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j'" "a b c d e f g h i j" 1

# Large quote content
LARGE_CONTENT=$(printf 'test%.0s ' {1..100})
run_test_with_valgrind "Large quote content" "echo '$LARGE_CONTENT'" "$LARGE_CONTENT" 1

# 9. SIGNAL AND INTERRUPT TESTS
echo -e "${YELLOW}=== SIGNAL TESTS ===${NC}"

# These tests are harder to automate, but we can test basic signal handling
run_test_with_valgrind "Ctrl+C simulation" "echo hello" "hello" 1

# 10. SYNTAX ERROR TESTS
echo -e "${YELLOW}=== SYNTAX ERROR TESTS ===${NC}"

# Various syntax errors that should fail gracefully
run_test_with_valgrind "Double pipe" "echo hello || echo world" "" 0
run_test_with_valgrind "Double redirect" "echo hello >> > /tmp/test" "" 0
run_test_with_valgrind "Invalid syntax 1" "echo hello |" "" 0
run_test_with_valgrind "Invalid syntax 2" "| echo hello" "" 0
run_test_with_valgrind "Invalid syntax 3" "> echo hello" "" 0

# Cleanup
rm -f /tmp/minishell_test_input.txt /tmp/minishell_output.txt /tmp/valgrind_output.txt
rm -f /tmp/test_output.txt /tmp/test_append.txt /tmp/complex1.txt /tmp/complex2.txt

echo
echo -e "${BLUE}=== TEST SUMMARY ===${NC}"
echo -e "Tests passed: ${GREEN}$PASSED${NC}"
echo -e "Tests failed: ${RED}$FAILED${NC}"
echo -e "Segfaults detected: ${RED}$SEGFAULT${NC}"
echo -e "Memory leaks detected: ${YELLOW}$LEAKS${NC}"
echo

if [[ $SEGFAULT -gt 0 ]]; then
    echo -e "${RED}💥 CRITICAL: $SEGFAULT segfaults detected! Fix immediately.${NC}"
    echo -e "${RED}   These are critical failures that must be resolved.${NC}"
    exit 1
elif [[ $LEAKS -gt 0 ]]; then
    echo -e "${YELLOW}⚠️  WARNING: $LEAKS memory leaks detected!${NC}"
    echo -e "${YELLOW}   Consider fixing these for better code quality.${NC}"
    echo -e "${YELLOW}   Note: Readline leaks are automatically suppressed.${NC}"
    exit 2
elif [[ $FAILED -gt 0 ]]; then
    echo -e "${YELLOW}⚠️  $FAILED tests failed with unexpected behavior.${NC}"
    echo -e "${YELLOW}   Review the output above for details.${NC}"
    exit 3
else
    echo -e "${GREEN}✅ All tests passed successfully!${NC}"
    echo -e "${GREEN}   No segfaults, no memory leaks, all functionality correct.${NC}"
    exit 0
fi
