#!/bin/bash

# Extreme Performance and Edge Case Test for Minishell
# Tests extreme scenarios that could cause crashes or hangs

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

MINISHELL="./minishell"
TIMEOUT_DURATION=60

# Counters for results
TESTS_RUN=0
TESTS_PASSED=0
TIMEOUTS=0
SEGFAULTS=0
ERRORS=0

# Arrays to store failed test details
declare -a FAILED_TESTS=()
declare -a TIMEOUT_TESTS=()
declare -a SEGFAULT_TESTS=()

echo -e "${BLUE}╔══════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║            EXTREME PERFORMANCE & EDGE CASES              ║${NC}"
echo -e "${BLUE}║      Testing extreme scenarios and stress conditions     ║${NC}"
echo -e "${BLUE}╚══════════════════════════════════════════════════════════╝${NC}"
echo

# Function to run test with timeout
run_extreme_test() {
    local test_name="$1"
    local command="$2"
    local max_time="$3"
    
    ((TESTS_RUN++))
    echo -e "${BLUE}[Test $TESTS_RUN] Extreme Test: $test_name${NC}"
    echo "Command: $command"
    
    echo "$command" > /tmp/extreme_test_input.txt
    echo "exit" >> /tmp/extreme_test_input.txt
    
    timeout $max_time valgrind --leak-check=full --error-exitcode=42 \
            --log-file=/tmp/extreme_valgrind.txt \
            --suppressions=readline.supp \
            $MINISHELL < /tmp/extreme_test_input.txt > /tmp/extreme_output.txt 2>&1
    
    local exit_code=$?
    
    if [[ $exit_code -eq 124 ]]; then
        echo -e "${RED}❌ TIMEOUT! Test took longer than ${max_time}s${NC}"
        ((TIMEOUTS++))
        ((ERRORS++))
        TIMEOUT_TESTS+=("Test $TESTS_RUN: $test_name")
    elif [[ $exit_code -eq 139 ]]; then
        echo -e "${RED}💥 SEGFAULT!${NC}"
        ((SEGFAULTS++))
        ((ERRORS++))
        SEGFAULT_TESTS+=("Test $TESTS_RUN: $test_name")
    elif [[ $exit_code -eq 42 ]]; then
        echo -e "${RED}💥 VALGRIND ERROR!${NC}"
        cat /tmp/extreme_valgrind.txt
        ((ERRORS++))
        FAILED_TESTS+=("Test $TESTS_RUN: $test_name (Valgrind)")
    else
        echo -e "${GREEN}✅ Survived extreme test${NC}"
        ((TESTS_PASSED++))
    fi
    
    echo "----------------------------------------"
}

# 1. MASSIVE STRING TESTS
echo -e "${YELLOW}=== MASSIVE STRING TESTS ===${NC}"

# Extremely long single argument
HUGE_STRING=$(printf 'x%.0s' {1..10000})
run_extreme_test "Huge single string" "echo '$HUGE_STRING'" 30

# Many small arguments
MANY_ARGS=$(printf "'arg%d' " {1..1000})
run_extreme_test "Many arguments" "echo $MANY_ARGS" 30

# Very long variable expansion
run_extreme_test "Long PATH expansion" "echo \$PATH\$PATH\$PATH\$PATH\$PATH" 20

# 2. EXTREME QUOTE NESTING
echo -e "${YELLOW}=== EXTREME QUOTE NESTING ===${NC}"

# Many nested quote patterns
NESTED_QUOTES="echo '"$(printf "nested%.0s" {1..100})"'"
run_extreme_test "Deeply nested content" "$NESTED_QUOTES" 10

# Alternating quotes
ALT_QUOTES="echo "$(printf "'a'\"b\"" {1..50})
run_extreme_test "Alternating quotes" "$ALT_QUOTES" 10

# Quote bombs
QUOTE_BOMB=$(printf "\"'\"'\"'\"'" {1..500})
run_extreme_test "Quote bomb" "echo $QUOTE_BOMB" 15

# 3. EXTREME PIPE CHAINS
echo -e "${YELLOW}=== EXTREME PIPE CHAINS ===${NC}"

# Very long pipe chain
LONG_PIPE="echo hello"$(printf " | cat" {1..100})
run_extreme_test "100-stage pipe" "$LONG_PIPE" 60

# Pipe with huge data
run_extreme_test "Pipe with huge data" "echo '$HUGE_STRING' | cat | cat | cat" 30

# 4. EXTREME VARIABLE TESTS
echo -e "${YELLOW}=== EXTREME VARIABLE TESTS ===${NC}"

# Many variable expansions
MANY_VARS=$(printf "\$HOME" {1..1000})
run_extreme_test "1000 variable expansions" "echo $MANY_VARS" 30

# Simple variable pattern (removed complex ${} syntax)
run_extreme_test "Simple variable pattern" "echo \$HOME\$USER\$PATH\$PWD" 10

# 5. MEMORY EXHAUSTION TESTS
echo -e "${YELLOW}=== MEMORY EXHAUSTION TESTS ===${NC}"

# Multiple huge allocations
run_extreme_test "Multiple huge strings" "echo '$HUGE_STRING' '$HUGE_STRING' '$HUGE_STRING'" 30

# 6. RECURSIVE PATTERNS
echo -e "${YELLOW}=== RECURSIVE PATTERNS ===${NC}"

# Recursive-like quote patterns
RECURSIVE_QUOTES=$(printf "echo " && printf "'nested " {1..50} && printf "content" && printf "'" {1..50})
run_extreme_test "Recursive quote pattern" "$RECURSIVE_QUOTES" 15

# 7. SPECIAL CHARACTER STRESS
echo -e "${YELLOW}=== SPECIAL CHARACTER STRESS ===${NC}"

# All ASCII printable characters
ALL_ASCII=$(printf "\\$(printf '%03o' {32..126})")
run_extreme_test "All ASCII characters" "echo '$ALL_ASCII'" 10

# Unicode stress test
UNICODE_STRESS="echo '🌍🚀💻🔥⚡🌟💎🎯🎭🎪🎨🎵🎸🎺🎻🎬🎮🎯🎲🎳🎨🎭🎪'"
run_extreme_test "Unicode stress" "$UNICODE_STRESS" 10

# 8. EXTREME REDIRECTION TESTS
echo -e "${YELLOW}=== EXTREME REDIRECTION TESTS ===${NC}"

# Many redirections
MANY_REDIRECTS="echo hello"$(printf " > /tmp/test%d.txt" {1..10})
run_extreme_test "Many redirections" "$MANY_REDIRECTS" 10

# Huge file operations
run_extreme_test "Huge file write" "echo '$HUGE_STRING' > /tmp/huge_file.txt" 15

# 9. ERROR BOUNDARY TESTS
echo -e "${YELLOW}=== ERROR BOUNDARY TESTS ===${NC}"

# Null bytes (should be handled gracefully)
run_extreme_test "Null byte handling" "echo 'hello\x00world'" 5

# Very long paths
LONG_PATH="/tmp/"$(printf "very_long_directory_name_" {1..100})
run_extreme_test "Very long path" "echo hello > $LONG_PATH/file.txt" 10

# 10. CONCURRENT-LIKE STRESS
echo -e "${YELLOW}=== RAPID EXECUTION STRESS ===${NC}"

# Rapid consecutive commands
RAPID_COMMANDS=$(printf "echo test%d; " {1..100})
run_extreme_test "Rapid consecutive commands" "$RAPID_COMMANDS" 15

# Cleanup
rm -f /tmp/extreme_test_input.txt /tmp/extreme_output.txt /tmp/extreme_valgrind.txt
rm -f /tmp/test*.txt /tmp/huge_file.txt

echo
echo -e "${BLUE}╔══════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║               EXTREME TESTS RESULTS                      ║${NC}"
echo -e "${BLUE}╚══════════════════════════════════════════════════════════╝${NC}"
echo
echo -e "Total tests run: ${BLUE}$TESTS_RUN${NC}"
echo -e "Tests passed: ${GREEN}$TESTS_PASSED${NC}"
echo -e "Segfaults: ${RED}$SEGFAULTS${NC}"
echo -e "Timeouts: ${YELLOW}$TIMEOUTS${NC}"
echo -e "Other errors: ${RED}$(($ERRORS - $SEGFAULTS - $TIMEOUTS))${NC}"
echo

# Show detailed information about failed tests
if [[ ${#SEGFAULT_TESTS[@]} -gt 0 ]]; then
    echo -e "${RED}💥 SEGFAULTS DETECTED IN:${NC}"
    for test in "${SEGFAULT_TESTS[@]}"; do
        echo -e "   ${RED}• $test${NC}"
    done
    echo
fi

if [[ ${#TIMEOUT_TESTS[@]} -gt 0 ]]; then
    echo -e "${YELLOW}⏰ TIMEOUTS DETECTED IN:${NC}"
    for test in "${TIMEOUT_TESTS[@]}"; do
        echo -e "   ${YELLOW}• $test${NC}"
    done
    echo
fi

if [[ ${#FAILED_TESTS[@]} -gt 0 ]]; then
    echo -e "${RED}❌ VALGRIND ERRORS DETECTED IN:${NC}"
    for test in "${FAILED_TESTS[@]}"; do
        echo -e "   ${RED}• $test${NC}"
    done
    echo
fi

if [[ $SEGFAULTS -gt 0 ]]; then
    echo -e "${RED}💥 CRITICAL: Segfaults detected in extreme scenarios!${NC}"
    echo -e "${RED}Your minishell crashes under stress conditions.${NC}"
    exit 1
elif [[ $TIMEOUTS -gt 0 ]] || [[ $ERRORS -gt 0 ]]; then
    echo -e "${YELLOW}⚠️  Some extreme tests failed or timed out.${NC}"
    echo -e "${YELLOW}Consider optimizing performance for edge cases.${NC}"
    exit 2
elif [[ $TESTS_PASSED -eq $TESTS_RUN ]] && [[ $TESTS_RUN -gt 0 ]]; then
    echo -e "${GREEN}🎉 ALL EXTREME TESTS PASSED!${NC}"
    echo -e "${GREEN}Your minishell is extremely robust!${NC}"
    exit 0
else
    echo -e "${YELLOW}⚠️  No tests were run or unexpected behavior.${NC}"
    exit 3
fi
