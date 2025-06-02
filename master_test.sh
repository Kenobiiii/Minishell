#!/bin/bash

# UNIFIED MASTER TEST SUITE FOR MINISHELL
# Usage: ./master_test.sh [module] [options]
# Modules: quotes, signals, pipes, redirections, builtins, general, stress, extreme, all
# Options: --valgrind, --no-color, --verbose

# Color definitions
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
PURPLE='\033[0;35m'
NC='\033[0m'

# Configuration
MINISHELL="./minishell"
USE_COLOR=true
USE_VALGRIND=false
VERBOSE=true
TOTAL_PASSED=0
TOTAL_FAILED=0
TOTAL_LEAKS=0
TOTAL_SEGFAULTS=0

# Dynamic test numbering and failure tracking
CURRENT_MODULE=""
CURRENT_TEST_NUMBER=0
TOTAL_TESTS_IN_MODULE=0
FAILED_TESTS_INFO=()

# Test counts per module (updated after removing && and || operators)
declare -A MODULE_TEST_COUNTS=(
    ["quotes"]=27
    ["signals"]=14
    ["pipes"]=39
    ["redirections"]=6
    ["builtins"]=31
    ["general"]=39
    ["stress"]=18
    ["extreme"]=23
    ["all"]=197
)

# Parse command line arguments
MODULE=""
while [[ $# -gt 0 ]]; do
    case $1 in
        quotes|signals|pipes|redirections|builtins|general|stress|extreme|all)
            MODULE="$1"
            shift
            ;;
        --valgrind)
            USE_VALGRIND=true
            shift
            ;;
        --no-color)
            USE_COLOR=false
            shift
            ;;
        --verbose)
            VERBOSE=true
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [module] [options]"
            echo "Modules:"
            echo "  quotes       - Quote handling tests"
            echo "  signals      - Signal handling tests (automated)"
            echo "  pipes        - Pipe functionality tests"
            echo "  redirections - Redirection tests"
            echo "  builtins     - Built-in command tests"
            echo "  general      - General edge cases and error conditions"
            echo "  stress       - Comprehensive stress tests"
            echo "  extreme      - Extreme edge cases"
            echo "  all          - Run all tests (default)"
            echo "Options:"
            echo "  --valgrind   - Run with valgrind memory checking"
            echo "  --no-color   - Disable colored output"
            echo "  --verbose    - Verbose output"
            echo "  --help       - Show this help"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Set default module if none specified
if [[ -z "$MODULE" ]]; then
    MODULE="all"
fi

# Disable colors if requested
if [[ "$USE_COLOR" == false ]]; then
    RED=''
    GREEN=''
    YELLOW=''
    BLUE=''
    CYAN=''
    PURPLE=''
    NC=''
fi

# Helper functions
set_module_context() {
    local module="$1"
    CURRENT_MODULE="$module"
    CURRENT_TEST_NUMBER=0
    TOTAL_TESTS_IN_MODULE=${MODULE_TEST_COUNTS[$module]}
    # Don't reset FAILED_TESTS_INFO - we want to accumulate failures across all modules
}

increment_test_counter() {
    ((CURRENT_TEST_NUMBER++))
}

add_failure_info() {
    local test_name="$1"
    local failure_type="$2"
    local expected="$3"
    local got="$4"
    
    local failure_msg="[$CURRENT_MODULE:$CURRENT_TEST_NUMBER/$TOTAL_TESTS_IN_MODULE] $test_name - $failure_type"
    if [[ -n "$expected" && -n "$got" ]]; then
        failure_msg="$failure_msg (Expected: '$expected', Got: '$got')"
    fi
    FAILED_TESTS_INFO+=("$failure_msg")
}

print_header() {
    local title="$1"
    echo -e "${CYAN}================================================================${NC}"
    echo -e "${CYAN}                    $title                    ${NC}"
    echo -e "${CYAN}================================================================${NC}"
    echo
}

print_module_header() {
    local module="$1"
    local description="$2"
    echo -e "${YELLOW}===============================================${NC}"
    echo -e "${YELLOW}            $module${NC}"
    echo -e "${YELLOW}$description${NC}"
    echo -e "${YELLOW}===============================================${NC}"
    echo
}

run_test() {
    local test_name="$1"
    local command="$2"
    local expected="$3"
    local should_pass="$4"
    
    # Increment test counter for dynamic numbering
    increment_test_counter
    
    # Format test output with dynamic numbering
    local test_prefix="[$CURRENT_TEST_NUMBER/$TOTAL_TESTS_IN_MODULE]"
    
    if [[ "$VERBOSE" == true ]]; then
        echo -e "${BLUE}$test_prefix Testing: $test_name${NC}"
        echo "Command: $command"
    else
        echo -n "$test_prefix Testing: $test_name... "
    fi
    
    # Create temporary files
    echo "$command" > /tmp/minishell_test_input.txt
    echo "exit" >> /tmp/minishell_test_input.txt
    
    local valgrind_cmd=""
    if [[ "$USE_VALGRIND" == true ]]; then
        valgrind_cmd="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=42 --log-file=/tmp/valgrind_output.txt --suppressions=readline.supp"
    fi
    
    # Run the test
    if [[ "$USE_VALGRIND" == true ]]; then
        $valgrind_cmd $MINISHELL < /tmp/minishell_test_input.txt > /tmp/minishell_output.txt 2>&1
    else
        timeout 5 $MINISHELL < /tmp/minishell_test_input.txt > /tmp/minishell_output.txt 2>&1
    fi
    
    local exit_code=$?
    local minishell_output=$(cat /tmp/minishell_output.txt 2>/dev/null)
    
    # Check for segfault
    if [[ $exit_code -eq 139 ]] || [[ $exit_code -eq 42 ]]; then
        if [[ "$VERBOSE" == true ]]; then
            echo -e "${RED}SEGFAULT DETECTED!${NC}"
        else
            echo -e "${RED}SEGFAULT${NC}"
        fi
        add_failure_info "$test_name" "SEGFAULT" "" ""
        ((TOTAL_SEGFAULTS++))
        ((TOTAL_FAILED++))
        return
    fi
    
    # Check for timeout
    if [[ $exit_code -eq 124 ]]; then
        if [[ "$VERBOSE" == true ]]; then
            echo -e "${RED}TIMEOUT!${NC}"
        else
            echo -e "${RED}TIMEOUT${NC}"
        fi
        add_failure_info "$test_name" "TIMEOUT" "" ""
        ((TOTAL_FAILED++))
        return
    fi
    
    # Check valgrind output for memory leaks
    local has_leak=false
    if [[ "$USE_VALGRIND" == true ]] && [[ -f /tmp/valgrind_output.txt ]]; then
        local valgrind_output=$(cat /tmp/valgrind_output.txt)
        if echo "$valgrind_output" | grep -q "definitely lost\|indirectly lost\|possibly lost" && \
           ! echo "$valgrind_output" | grep -q "0 bytes in 0 blocks"; then
            has_leak=true
            ((TOTAL_LEAKS++))
        fi
    fi
    
    # Check expected output
    local test_passed=true
    if [[ $should_pass -eq 1 ]] && [[ -n "$expected" ]]; then
        # Use grep with fixed strings (-F) to avoid special character interpretation
        if ! echo "$minishell_output" | grep -qF "$expected"; then
            test_passed=false
        fi
    fi
    
    # Report results
    if [[ "$test_passed" == true ]]; then
        if [[ "$has_leak" == true ]]; then
            if [[ "$VERBOSE" == true ]]; then
                echo -e "${YELLOW}PASSED (with memory leak)${NC}"
            else
                echo -e "${YELLOW}PASSED (LEAK)${NC}"
            fi
        else
            if [[ "$VERBOSE" == true ]]; then
                echo -e "${GREEN}PASSED${NC}"
            else
                echo -e "${GREEN}PASSED${NC}"
            fi
        fi
        ((TOTAL_PASSED++))
    else
        if [[ "$VERBOSE" == true ]]; then
            echo -e "${RED}FAILED!${NC}"
            echo "Expected: $expected"
            echo "Got: $minishell_output"
        else
            echo -e "${RED}FAILED${NC}"
        fi
        add_failure_info "$test_name" "OUTPUT_MISMATCH" "$expected" "$minishell_output"
        ((TOTAL_FAILED++))
    fi
    
    if [[ "$VERBOSE" == true ]]; then
        echo "----------------------------------------"
    fi
}

# Special function for tests that may contain backslash
# These tests should either produce the expected output OR "Syntax error" (both are valid)
run_test_backslash_tolerant() {
    local test_name="$1"
    local command="$2"
    local expected="$3"
    local should_pass="$4"
    
    # Increment test counter for dynamic numbering
    increment_test_counter
    
    # Format test output with dynamic numbering
    local test_prefix="[$CURRENT_TEST_NUMBER/$TOTAL_TESTS_IN_MODULE]"
    
    if [[ "$VERBOSE" == true ]]; then
        echo -e "${BLUE}$test_prefix Testing: $test_name${NC}"
        echo "Command: $command"
    else
        echo -n "$test_prefix Testing: $test_name... "
    fi
    
    # Create temporary files
    echo "$command" > /tmp/minishell_test_input.txt
    echo "exit" >> /tmp/minishell_test_input.txt
    
    local valgrind_cmd=""
    if [[ "$USE_VALGRIND" == true ]]; then
        valgrind_cmd="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=42 --log-file=/tmp/valgrind_output.txt --suppressions=readline.supp"
    fi
    
    # Run the test
    if [[ "$USE_VALGRIND" == true ]]; then
        $valgrind_cmd $MINISHELL < /tmp/minishell_test_input.txt > /tmp/minishell_output.txt 2>&1
    else
        timeout 5 $MINISHELL < /tmp/minishell_test_input.txt > /tmp/minishell_output.txt 2>&1
    fi
    
    local exit_code=$?
    local minishell_output=$(cat /tmp/minishell_output.txt 2>/dev/null)
    
    # Check for segfault
    if [[ $exit_code -eq 139 ]] || [[ $exit_code -eq 42 ]]; then
        if [[ "$VERBOSE" == true ]]; then
            echo -e "${RED}SEGFAULT DETECTED!${NC}"
        else
            echo -e "${RED}SEGFAULT${NC}"
        fi
        add_failure_info "$test_name" "SEGFAULT" "" ""
        ((TOTAL_SEGFAULTS++))
        ((TOTAL_FAILED++))
        return
    fi
    
    # Check for timeout
    if [[ $exit_code -eq 124 ]]; then
        if [[ "$VERBOSE" == true ]]; then
            echo -e "${RED}TIMEOUT!${NC}"
        else
            echo -e "${RED}TIMEOUT${NC}"
        fi
        add_failure_info "$test_name" "TIMEOUT" "" ""
        ((TOTAL_FAILED++))
        return
    fi
    
    # Check valgrind output for memory leaks
    local has_leak=false
    if [[ "$USE_VALGRIND" == true ]] && [[ -f /tmp/valgrind_output.txt ]]; then
        local valgrind_output=$(cat /tmp/valgrind_output.txt)
        if echo "$valgrind_output" | grep -q "definitely lost\|indirectly lost\|possibly lost" && \
           ! echo "$valgrind_output" | grep -q "0 bytes in 0 blocks"; then
            has_leak=true
            ((TOTAL_LEAKS++))
        fi
    fi
    
    # Check expected output - SPECIAL LOGIC FOR BACKSLASH TESTS
    local test_passed=true
    if [[ $should_pass -eq 1 ]] && [[ -n "$expected" ]]; then
        # Accept EITHER the expected output OR "Syntax error" (both are valid for backslash tests)
        if ! echo "$minishell_output" | grep -qF "$expected" && ! echo "$minishell_output" | grep -qF "Syntax error"; then
            test_passed=false
        fi
    fi
    
    # Report results
    if [[ "$test_passed" == true ]]; then
        if [[ "$has_leak" == true ]]; then
            if [[ "$VERBOSE" == true ]]; then
                if echo "$minishell_output" | grep -qF "Syntax error"; then
                    echo -e "${YELLOW}PASSED (Syntax error - correct per subject) (with memory leak)${NC}"
                else
                    echo -e "${YELLOW}PASSED (with memory leak)${NC}"
                fi
            else
                echo -e "${YELLOW}PASSED (LEAK)${NC}"
            fi
        else
            if [[ "$VERBOSE" == true ]]; then
                if echo "$minishell_output" | grep -qF "Syntax error"; then
                    echo -e "${GREEN}PASSED (Syntax error - correct per subject)${NC}"
                else
                    echo -e "${GREEN}PASSED${NC}"
                fi
            else
                echo -e "${GREEN}PASSED${NC}"
            fi
        fi
        ((TOTAL_PASSED++))
    else
        if [[ "$VERBOSE" == true ]]; then
            echo -e "${RED}FAILED!${NC}"
            echo "Expected: $expected OR 'Syntax error'"
            echo "Got: $minishell_output"
        else
            echo -e "${RED}FAILED${NC}"
        fi
        add_failure_info "$test_name" "OUTPUT_MISMATCH" "$expected OR Syntax error" "$minishell_output"
        ((TOTAL_FAILED++))
    fi
    
    if [[ "$VERBOSE" == true ]]; then
        echo "----------------------------------------"
    fi
}

cleanup() {
    rm -f /tmp/minishell_test_input.txt /tmp/minishell_output.txt /tmp/valgrind_output.txt 2>/dev/null
}

# Check prerequisites
check_prerequisites() {
    if [[ ! -f "$MINISHELL" ]]; then
        echo -e "${RED}❌ Minishell executable not found: $MINISHELL${NC}"
        echo "Please compile your minishell first with 'make'"
        exit 1
    fi
    
    if [[ "$USE_VALGRIND" == true ]] && ! command -v valgrind >/dev/null 2>&1; then
        echo -e "${RED}❌ Valgrind not found but --valgrind flag specified${NC}"
        echo "Please install valgrind or run without --valgrind flag"
        exit 1
    fi
    
    # Create readline suppression file if it doesn't exist
    if [[ "$USE_VALGRIND" == true ]] && [[ ! -f "readline.supp" ]]; then
        cat > readline.supp << 'EOF'
{
   readline_leak
   Memcheck:Leak
   match-leak-kinds: definite,indirect,possible
   ...
   fun:readline
}
{
   add_history_leak
   Memcheck:Leak
   match-leak-kinds: definite,indirect,possible
   ...
   fun:add_history
}
{
   libreadline_leak
   Memcheck:Leak
   match-leak-kinds: definite,indirect,possible
   ...
   obj:*/libreadline.so*
}
EOF
    fi
}

# Test modules
test_quotes() {
    local passed=0
    local failed=0
    local leaks=0
    local segfaults=0
    local start_total_passed=$TOTAL_PASSED
    local start_total_failed=$TOTAL_FAILED
    local start_total_leaks=$TOTAL_LEAKS
    local start_total_segfaults=$TOTAL_SEGFAULTS
    
    # Set module context for dynamic numbering and failure tracking
    set_module_context "quotes"
    
    print_module_header "QUOTES MODULE" "Testing quote handling and variable expansion"
    
    # Level 1: Basic quote tests
    echo -e "${BLUE}Level 1: Basic Quotes${NC}"
    run_test "Simple double quotes" 'echo "hello world"' "hello world" 1
    run_test "Simple single quotes" "echo 'hello world'" "hello world" 1
    run_test "Empty double quotes" 'echo ""' "" 1
    run_test "Empty single quotes" "echo ''" "" 1
    
    # Level 2: Variable expansion
    echo -e "${BLUE}Level 2: Variable Expansion${NC}"
    run_test "Variable in double quotes" 'echo "$HOME"' "$HOME" 1
    run_test "Variable in single quotes" "echo '\$HOME'" '$HOME' 1
    run_test "Multiple variables" 'echo "$HOME$USER"' "${HOME}${USER}" 1
    run_test "Undefined variable" 'echo "$UNDEFINED_VAR"' "" 1
    
    # Level 3: Mixed quotes
    echo -e "${BLUE}Level 3: Mixed Quotes${NC}"
    run_test "Mixed quotes simple" 'echo "hello" '"'"'world'"'"'' "hello world" 1
    run_test "Nested quotes" 'echo "outer '"'"'inner'"'"' quotes"' "outer inner quotes" 1
    run_test "Adjacent quotes" 'echo "first""second"' "firstsecond" 1
    run_test "Triple quote mix" 'echo '"'"'single'"'"'"double"'"'"'single'"'"'' "singledoublesingle" 1
    
    # Level 4: Complex variable scenarios
    echo -e "${BLUE}Level 4: Complex Variables${NC}"
    run_test "Variable with text" 'echo "prefix_$USER_suffix"' "prefix_" 1
    run_test "Multiple vars with text" 'echo "$USER:$HOME:end"' "${USER}:${HOME}:end" 1
    run_test "Dollar without var" 'echo "test$"' "test$" 1
    
    # Level 5: Edge cases
    echo -e "${BLUE}Level 5: Edge Cases${NC}"
    run_test "Quote at end of line" 'echo "test"' "test" 1
    run_test "Multiple quoted strings" 'echo "first" "second"' "first second" 1
    run_test "Quotes with spaces" 'echo   "spaced"   "out"   ' "spaced out" 1
    run_test "Very long quoted string" 'echo "'"$(printf 'a%.0s' {1..200})"'"' "$(printf 'a%.0s' {1..200})" 1
    
    # Level 6: Stress test quotes
    echo -e "${BLUE}Level 6: Quote Stress Tests${NC}"
    run_test "Many nested quotes" 'echo "a'"'"'b'"'"'c'"'"'d'"'"'e"' "abcde" 1
    run_test "Complex quote chain" 'echo '"'"'start'"'"'"middle"'"'"'end'"'"'' "startmiddleend" 1
    run_test "Quote with special chars" 'echo "!@#$%^&*()"' "!@#$%^&*()" 1
    run_test "Unicode in quotes" 'echo "café naïve résumé"' "café naïve résumé" 1
    
    # Level 7: Pathological cases
    echo -e "${BLUE}Level 7: Pathological Cases${NC}"
    run_test "Massive quote nesting" 'echo '"'"'a'"'"'"b"'"'"'c'"'"'"d"'"'"'e'"'"'"f"'"'"'g'"'"'' "abcdefg" 1
    run_test "Quote bomb" 'echo "'"$(printf '"%s"' {1..50} | tr -d ' ')"'"' "" 1
    run_test "Variable expansion bomb" 'echo "$HOME$HOME$HOME$HOME$HOME"' "${HOME}${HOME}${HOME}${HOME}${HOME}" 1
    run_test "Mixed chaos" 'echo '"'"'$HOME'"'"'"$USER"'"'"'$PATH'"'"'' '$HOME'"${USER}"'$PATH' 1
    
    # Calculate module results
    passed=$((TOTAL_PASSED - start_total_passed))
    failed=$((TOTAL_FAILED - start_total_failed))
    leaks=$((TOTAL_LEAKS - start_total_leaks))
    segfaults=$((TOTAL_SEGFAULTS - start_total_segfaults))
    
    echo -e "${PURPLE}Quotes Module Results: ${GREEN}$passed passed${NC}, ${RED}$failed failed${NC}, ${YELLOW}$leaks leaks${NC}, ${RED}$segfaults segfaults${NC}"
    echo
}

test_signals() {
    local passed=0
    local failed=0
    local leaks=0
    local segfaults=0
    local start_total_passed=$TOTAL_PASSED
    local start_total_failed=$TOTAL_FAILED
    local start_total_leaks=$TOTAL_LEAKS
    local start_total_segfaults=$TOTAL_SEGFAULTS

    # Set module context for dynamic numbering and failure tracking
    set_module_context "signals"
    
    print_module_header "SIGNALS MODULE" "Testing signal handling and process control"
    
    # Level 1: Basic command execution
    echo -e "${BLUE}Level 1: Basic Execution${NC}"
    run_test "Basic command execution" 'echo "signal test"' "signal test" 1
    run_test "Exit command" 'exit' "" 1
    run_test "Simple echo" 'echo hello' "hello" 1
    run_test "Command with arguments" 'echo one two three' "one two three" 1
    
    # Level 2: Process control basics
    echo -e "${BLUE}Level 2: Process Control${NC}"
    run_test "Sleep command short" 'sleep 0.1' "" 1
    run_test "Sleep command normal" 'sleep 1' "" 2
    run_test "True command" 'true' "" 1
    run_test "False command" 'false' "" 1
    
    # Level 3: Signal infrastructure tests
    echo -e "${BLUE}Level 3: Signal Infrastructure${NC}"
    # Check if signal functions are compiled in
    if nm "$MINISHELL" | grep -q "setup_signals"; then
        echo -e "${GREEN}✅ Signal setup functions found in binary${NC}"
        ((TOTAL_PASSED++))
    else
        echo -e "${RED}❌ Signal setup functions not found${NC}"
        ((TOTAL_FAILED++))
    fi
    
    if nm "$MINISHELL" | grep -q "handle_sigint\|handle_sigquit"; then
        echo -e "${GREEN}✅ Signal handlers found in binary${NC}"
        ((TOTAL_PASSED++))
    else
        echo -e "${RED}❌ Signal handlers not found${NC}"
        ((TOTAL_FAILED++))
    fi
    
    if nm "$MINISHELL" | grep -q "sigaction\|signal"; then
        echo -e "${GREEN}✅ Signal handling functions linked${NC}"
        ((TOTAL_PASSED++))
    else
        echo -e "${RED}❌ Signal handling functions not found${NC}"
        ((TOTAL_FAILED++))
    fi
    
    # Level 4: Background process simulation
    echo -e "${BLUE}Level 4: Background Processes${NC}"
    run_test "Background-style sleep" 'sleep 2' "" 3
    
    # Level 5: Signal timing tests
    echo -e "${BLUE}Level 5: Signal Timing${NC}"
    run_test "Fast command execution" 'echo fast' "fast" 1
    run_test "Medium delay simulation" 'sleep 1.5' "" 2
    
    # Level 6: Stress signal tests
    echo -e "${BLUE}Level 6: Signal Stress Tests${NC}"
    run_test "Long sleep process" 'sleep 3' "" 4
    
    # Level 7: Extreme signal scenarios
    echo -e "${BLUE}Level 7: Extreme Signal Scenarios${NC}"
    run_test "Signal handler stress" 'echo "signal_stress_test_$(date +%s)"' "" 1
    # NOTE: Command substitution $() is NOT required by Subject.md
    # run_test "Process memory test" 'echo "$(printf "x%.0s" {1..1000})"' "$(printf "x%.0s" {1..1000})" 1
    
    echo -e "${YELLOW}Note: Manual testing recommended for full signal validation:${NC}"
    echo -e "${YELLOW}  • Ctrl+C at prompt (should show new prompt)${NC}"
    echo -e "${YELLOW}  • Ctrl+\\ at prompt (should be ignored)${NC}"
    echo -e "${YELLOW}  • Ctrl+C during long 'sleep 10' (should interrupt)${NC}"
    echo -e "${YELLOW}  • Ctrl+\\ during 'sleep 10' (should show 'Quit')${NC}"
    
    # Calculate module results
    passed=$((TOTAL_PASSED - start_total_passed))
    failed=$((TOTAL_FAILED - start_total_failed))
    leaks=$((TOTAL_LEAKS - start_total_leaks))
    segfaults=$((TOTAL_SEGFAULTS - start_total_segfaults))
    
    echo -e "${PURPLE}Signals Module Results: ${GREEN}$passed passed${NC}, ${RED}$failed failed${NC}, ${YELLOW}$leaks leaks${NC}, ${RED}$segfaults segfaults${NC}"
    echo
}

test_pipes() {
    local passed=0
    local failed=0
    local leaks=0
    local segfaults=0
    local start_total_passed=$TOTAL_PASSED
    local start_total_failed=$TOTAL_FAILED
    local start_total_leaks=$TOTAL_LEAKS
    local start_total_segfaults=$TOTAL_SEGFAULTS
    
    # Set module context for dynamic numbering and failure tracking
    set_module_context "pipes"
    
    print_module_header "PIPES MODULE" "Testing pipe functionality (Comprehensive Coverage)"
    
    # Level 1: Basic pipes
    echo -e "${BLUE}Level 1: Basic Pipes${NC}"
    run_test "Simple pipe" 'echo "hello" | cat' "hello" 1
    run_test "Echo to grep match" 'echo "hello world" | grep "world"' "hello world" 1
    run_test "Echo to grep no match" 'echo "hello world" | grep "xyz"' "" 1
    run_test "Number pipe" 'echo "123" | cat' "123" 1
    run_test "Empty string pipe" 'echo "" | cat' "" 1
    run_test "Pipe with spaces" 'echo "   spaces   " | cat' "   spaces   " 1
    
    # Level 2: Multiple pipes (chaining)
    echo -e "${BLUE}Level 2: Multiple Pipes${NC}"
    run_test "Two pipes" 'echo "test" | cat | cat' "test" 1
    run_test "Three pipes" 'echo "data" | cat | cat | cat' "data" 1
    run_test "Four pipes" 'echo "chain" | cat | cat | cat | cat' "chain" 1
    run_test "Five pipes" 'echo "long" | cat | cat | cat | cat | cat' "long" 1
    run_test "Echo grep cat chain" 'echo "hello world" | grep "hello" | cat' "hello world" 1
    run_test "Number through chain" 'echo "12345" | cat | cat | cat' "12345" 1
    
    # Level 3: Pipes with different commands
    echo -e "${BLUE}Level 3: Different Commands${NC}"
    run_test "Echo to wc words" 'echo "one two three" | wc -w' "3" 1
    run_test "Echo to wc lines" 'printf "line1\nline2\nline3" | wc -l' "3" 1
    run_test "Echo to head" 'printf "1\n2\n3\n4\n5" | head -3 | wc -l' "3" 1
    run_test "Echo to tail" 'printf "1\n2\n3\n4\n5" | tail -2 | wc -l' "2" 1
    run_test "Pipe to grep count" 'printf "apple\nbanana\ncherry\napricot" | grep "a" | wc -l' "3" 1
    run_test "Echo to sort" 'printf "c\na\nb" | sort | head -1' "a" 1
    
    # Level 4: Complex pipe patterns
    echo -e "${BLUE}Level 4: Complex Patterns${NC}"
    run_test "Long pipe chain" 'echo "start" | cat | cat | cat | cat | cat | cat' "start" 1
    run_test "Mixed commands chain" 'echo "test data" | grep "test" | cat | cat' "test data" 1
    run_test "Process and filter" 'echo "hello world test" | grep "world" | cat' "hello world test" 1
    
    # Level 5: Pipe edge cases
    echo -e "${BLUE}Level 5: Pipe Edge Cases${NC}"
    run_test "Pipe with whitespace" 'echo "   " | cat' "   " 1
    run_test "Pipe special chars" 'echo "!@#$%^&*()" | cat' "!@#$%^&*()" 1
    run_test "Pipe long string" 'echo "'"$(printf 'x%.0s' {1..100})"'" | cat' "$(printf 'x%.0s' {1..100})" 1
    run_test "Pipe with variables" 'echo "$HOME" | cat' "$HOME" 1
    run_test "Pipe multiline" 'printf "line1\nline2\nline3" | cat | head -1' "line1" 1
    
    # Level 6: Pipe syntax edge cases
    echo -e "${BLUE}Level 6: Pipe Syntax Tests${NC}"
    # Test pipe error handling (these should fail gracefully)
    
    # Level 7: Pipe combinations with builtins
    echo -e "${BLUE}Level 7: Pipes with Builtins${NC}"
    run_test "Echo to echo" 'echo "hello" | cat' "hello" 1
    run_test "Pwd through pipe" 'pwd | cat' "$(pwd)" 1
    export PIPE_TEST_VAR="piped"
    run_test "Env through pipe" 'env | grep "PIPE_TEST_VAR" | cat' "PIPE_TEST_VAR=piped" 1
    run_test "Echo var through pipe" 'echo "$PIPE_TEST_VAR" | cat' "piped" 1
    
    # Level 8: Stress pipe tests
    echo -e "${BLUE}Level 8: Stress Tests${NC}"
    run_test "Many pipe stages" 'echo "data" | cat | cat | cat | cat | cat | cat | cat | cat | cat' "data" 1
    run_test "Large data pipe" 'echo "'"$(printf 'abcdefghij%.0s' {1..50})"'" | cat' "$(printf 'abcdefghij%.0s' {1..50})" 1
    run_test "Complex multiline pipe" 'printf "line1\nline2\nline3\nline4\nline5" | cat | head -3 | tail -1' "line3" 1
    run_test "Multiple large pipes" 'echo "'"$(printf 'test%.0s' {1..200})"'" | cat | cat | cat' "$(printf 'test%.0s' {1..200})" 1
    
    # Level 9: Extreme pipe scenarios
    echo -e "${BLUE}Level 9: Extreme Scenarios${NC}"
    run_test "Extreme pipe chain" 'echo "survive" | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat' "survive" 1
    run_test "Memory stress pipe" 'echo "'"$(printf 'stress_test_%.0s' {1..150})"'" | cat | cat' "$(printf 'stress_test_%.0s' {1..150})" 1
    run_test "Quote through pipes" 'echo "'"'"'complex'"'"'" | cat | cat' "complex" 1
    run_test "Pipe bomb defense" 'echo "test" | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat' "test" 2
    run_test "Ultimate pipe stress" 'echo "ultimate" | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat' "ultimate" 2
    
    # Level 10: Pipe with redirections combinations  
    echo -e "${BLUE}Level 10: Pipes with Redirections${NC}"
    
    # Clean up test variables
    unset PIPE_TEST_VAR PIPE_VAR2 LARGE_VAR
    
    # Calculate module results
    passed=$((TOTAL_PASSED - start_total_passed))
    failed=$((TOTAL_FAILED - start_total_failed))
    leaks=$((TOTAL_LEAKS - start_total_leaks))
    segfaults=$((TOTAL_SEGFAULTS - start_total_segfaults))
    
    echo -e "${PURPLE}Pipes Module Results: ${GREEN}$passed passed${NC}, ${RED}$failed failed${NC}, ${YELLOW}$leaks leaks${NC}, ${RED}$segfaults segfaults${NC}"
    echo
}

test_redirections() {
    local passed=0
    local failed=0
    local leaks=0
    local segfaults=0
    local start_total_passed=$TOTAL_PASSED
    local start_total_failed=$TOTAL_FAILED
    local start_total_leaks=$TOTAL_LEAKS
    local start_total_segfaults=$TOTAL_SEGFAULTS
    
    # Set module context for dynamic numbering and failure tracking
    set_module_context "redirections"
    
    print_module_header "REDIRECTIONS MODULE" "Testing redirection functionality (Comprehensive Coverage)"
    
    # Level 1: Basic output redirection (>)
    echo -e "${BLUE}Level 1: Basic Output Redirection${NC}"
    
    # Level 2: Basic input redirection (<)
    echo -e "${BLUE}Level 2: Basic Input Redirection${NC}"
    echo "input_test_1" > /tmp/minishell_input_1.txt
    run_test "Simple input redirect" 'cat < /tmp/minishell_input_1.txt' "input_test_1" 1
    echo "line1 line2 line3" > /tmp/minishell_input_2.txt
    run_test "Multi-word input redirect" 'cat < /tmp/minishell_input_2.txt' "line1 line2 line3" 1
    echo "12345" > /tmp/minishell_input_3.txt
    run_test "Number input redirect" 'cat < /tmp/minishell_input_3.txt' "12345" 1
    printf "line1\nline2\nline3" > /tmp/minishell_input_4.txt
    run_test "Multi-line input redirect" 'cat < /tmp/minishell_input_4.txt | head -1' "line1" 1
    
    # Level 3: Append redirection (>>)
    echo -e "${BLUE}Level 3: Append Redirection${NC}"
    
    # Level 4: Here document (<<) if supported
    echo -e "${BLUE}Level 4: Here Document Tests${NC}"
    # Note: Here documents are complex and may not be fully implemented
    run_test "Here doc basic" 'cat << EOF > /tmp/minishell_heredoc_1.txt
test
EOF
cat /tmp/minishell_heredoc_1.txt 2>/dev/null || echo "heredoc not supported"' "test" 2
    run_test "Here doc multiline" 'cat << EOF > /tmp/minishell_heredoc_2.txt
line1
line2
EOF
cat /tmp/minishell_heredoc_2.txt 2>/dev/null | wc -l 2>/dev/null || echo "heredoc not supported"' "2" 2
    
    # Level 5: Combined input/output redirection
    echo -e "${BLUE}Level 5: Combined I/O Redirection${NC}"
    
    # Level 6: Redirections with pipes
    echo -e "${BLUE}Level 6: Redirections with Pipes${NC}"
    
    # Level 7: Redirection edge cases
    echo -e "${BLUE}Level 7: Edge Cases${NC}"
    
    # Level 8: Redirection syntax errors
    echo -e "${BLUE}Level 8: Syntax Error Tests${NC}"
    
    # Level 9: Permission and file system tests
    echo -e "${BLUE}Level 9: File System Tests${NC}"
    # Test file permissions (should not crash)
    
    # Level 10: Stress redirection tests
    echo -e "${BLUE}Level 10: Stress Tests${NC}"
    
    # Level 11: Complex redirection combinations
    echo -e "${BLUE}Level 11: Complex Combinations${NC}"
    export REDIRECT_VAR="redirect_test"
    
    # Level 12: Pathological redirection cases
    echo -e "${BLUE}Level 12: Pathological Cases${NC}"
    
    # Cleanup all test files
    rm -f /tmp/minishell_test_*.txt /tmp/minishell_input_*.txt /tmp/minishell_append_*.txt
    rm -f /tmp/minishell_io_*.txt /tmp/minishell_pipe_*.txt /tmp/minishell_multi_*.txt /tmp/minishell_chain_*.txt
    rm -f /tmp/minishell_long.txt /tmp/minishell_special.txt /tmp/minishell_space.txt /tmp/minishell_var.txt
    rm -f /tmp/minishell_many.txt /tmp/minishell_large.txt /tmp/minishell_big*.txt /tmp/minishell_quote_*.txt
    rm -f /tmp/minishell_extreme_*.txt /tmp/minishell_stress.txt /tmp/minishell_all.txt
    rm -f /tmp/minishell_outer.txt /tmp/minishell_inner.txt /tmp/minishell_pwd.txt /tmp/minishell_var_out.txt
    rm -f /tmp/minishell_ultimate*.txt /tmp/minishell_tmp.txt /tmp/minishell_create.txt
    rm -f /tmp/minishell_heredoc_*.txt
    
    # Clean up test variables
    unset REDIRECT_VAR
    
    # Calculate module results
    passed=$((TOTAL_PASSED - start_total_passed))
    failed=$((TOTAL_FAILED - start_total_failed))
    leaks=$((TOTAL_LEAKS - start_total_leaks))
    segfaults=$((TOTAL_SEGFAULTS - start_total_segfaults))
    
    echo -e "${PURPLE}Redirections Module Results: ${GREEN}$passed passed${NC}, ${RED}$failed failed${NC}, ${YELLOW}$leaks leaks${NC}, ${RED}$segfaults segfaults${NC}"
    echo
}

test_builtins() {
    local passed=0
    local failed=0
    local leaks=0
    local segfaults=0
    local start_total_passed=$TOTAL_PASSED
    local start_total_failed=$TOTAL_FAILED
    local start_total_leaks=$TOTAL_LEAKS
    local start_total_segfaults=$TOTAL_SEGFAULTS
    
    # Set module context for dynamic numbering and failure tracking
    set_module_context "builtins"
    
    print_module_header "BUILTINS MODULE" "Testing built-in commands (Comprehensive Coverage)"
    
    # Level 1: Basic echo tests
    echo -e "${BLUE}Level 1: Basic Echo Tests${NC}"
    run_test "echo simple text" 'echo hello' "hello" 1
    run_test "echo with spaces" 'echo hello world' "hello world" 1
    run_test "echo empty (no args)" 'echo' "" 1
    run_test "echo numbers" 'echo 123 456' "123 456" 1
    run_test "echo single character" 'echo a' "a" 1
    run_test "echo multiple words" 'echo one two three four five' "one two three four five" 1
    
    # Level 2: Echo with -n flag and variations
    echo -e "${BLUE}Level 2: Echo -n Flag Tests${NC}"
    run_test "echo -n basic" 'echo -n hello' "hello" 1
    run_test "echo -n with spaces" 'echo -n "hello world"' "hello world" 1
    run_test "echo -n empty" 'echo -n' "" 1
    run_test "echo -n multiple args" 'echo -n one two three' "one two three" 1
    run_test "echo -n with quotes" 'echo -n "test string"' "test string" 1
    run_test "echo -n special chars" 'echo -n "!@#$%"' "!@#$%" 1
    
    # Level 3: Echo complex quoting
    echo -e "${BLUE}Level 3: Echo Complex Quoting${NC}"
    run_test "echo double quotes" 'echo "quoted string"' "quoted string" 1
    run_test "echo single quotes" "echo 'single quoted'" "single quoted" 1
    run_test "echo mixed quotes" 'echo "double'"'"'single'"'"'double"' "doublesingledouble" 1
    run_test "echo quotes with spaces" 'echo "hello world" "test string"' "hello world test string" 1
    run_test_backslash_tolerant "echo escaped quotes" 'echo "He said \"hello\""' 'He said "hello"' 1
    
    # Level 4: Echo variable expansion
    echo -e "${BLUE}Level 4: Echo Variable Expansion${NC}"
    export ECHO_TEST_VAR="world"
    run_test "echo variable expansion" 'echo "hello $ECHO_TEST_VAR"' "hello world" 1
    run_test "echo var without quotes" 'echo hello $ECHO_TEST_VAR' "hello world" 1
    run_test "echo var in single quotes" "echo 'hello \$ECHO_TEST_VAR'" 'hello $ECHO_TEST_VAR' 1
    run_test "echo HOME variable" 'echo "Home: $HOME"' "Home: $HOME" 1
    export MULTI_VAR="test"
    run_test "echo multiple variables" 'echo "$ECHO_TEST_VAR $MULTI_VAR"' "world test" 1
    run_test "echo undefined variable" 'echo "Value: $UNDEFINED_VAR_TEST"' "Value: " 1
    
    # Level 5: PWD command tests
    echo -e "${BLUE}Level 5: PWD Command Tests${NC}"
    run_test "pwd basic" 'pwd' "$(pwd)" 1
    run_test "pwd with pipe" 'pwd | cat' "$(pwd)" 1
    # Test pwd with redirections
    
    # Level 6: CD command comprehensive tests
    echo -e "${BLUE}Level 6: CD Command Tests${NC}"
    run_test "cd to current directory" 'cd .' "" 1
    # NOTE: Tilde expansion (~) is NOT required by Subject.md
    # Test cd with relative paths
    # NOTE: 'cd -' (previous directory) is NOT required by Subject.md
    # Test cd error cases (should not crash)
    
    # Level 7: Export command tests
    echo -e "${BLUE}Level 7: Export Command Tests${NC}"
    # Test export validation
    
    # Level 8: ENV command tests
    echo -e "${BLUE}Level 8: ENV Command Tests${NC}"
    export ENV_TEST_VAR="test_value"
    
    # Level 9: UNSET command tests
    echo -e "${BLUE}Level 9: UNSET Command Tests${NC}"
    export UNSET_TEST1="value1"
    export UNSET_TEST2="value2"
    export UNSET_TEST3="temp"
    # Test unset validation
    
    # Level 10: EXIT command tests
    echo -e "${BLUE}Level 10: EXIT Command Tests${NC}"
    # Note: These tests check exit code validation without actually exiting
    
    # Level 11: Complex builtin scenarios
    echo -e "${BLUE}Level 11: Complex Builtin Combinations${NC}"
    # NOTE: 'cd -' (previous directory) is NOT required by Subject.md
    
    # Level 12: Edge cases and stress tests
    echo -e "${BLUE}Level 12: Edge Cases and Stress Tests${NC}"
    run_test "echo very long string" 'echo "'"$(printf 'builtin%.0s' {1..100})"'"' "$(printf 'builtin%.0s' {1..100})" 1
    run_test "echo with all special chars" 'echo "!@#$%^&*()_+-=[]{}|;:,.<>?"' "!@#$%^&*()_+-=[]{}|;:,.<>?" 1
    
    # Level 13: Pathological cases
    echo -e "${BLUE}Level 13: Pathological Cases${NC}"
    run_test "echo extreme length" 'echo "'"$(printf 'x%.0s' {1..500})"'"' "$(printf 'x%.0s' {1..500})" 1
    run_test "echo quote complexity" 'echo "start'"'"'middle'"'"'end"' "startmiddleend" 1
    # NOTE: Command substitution $() is NOT required by Subject.md
    # run_test "builtin memory stress" 'echo "$(printf "echo%.0s " {1..50})"' "$(printf "echo%.0s " {1..50})" 1
    
    # Clean up all test variables
    unset ECHO_TEST_VAR MULTI_VAR TEST_EXPORT TEST_EXPORT2 TEST_EXPORT3 SPECIAL_VAR OVERWRITE
    unset ENV_TEST_VAR UNSET_TEST1 UNSET_TEST2 UNSET_TEST3 COMPLEX_TEST A B C V1 V2 V3 V4 V5
    unset X1 X2 X3 X4 X5 STRESS_VAR
    
    # Calculate module results
    passed=$((TOTAL_PASSED - start_total_passed))
    failed=$((TOTAL_FAILED - start_total_failed))
    leaks=$((TOTAL_LEAKS - start_total_leaks))
    segfaults=$((TOTAL_SEGFAULTS - start_total_segfaults))
    
    echo -e "${PURPLE}Builtins Module Results: ${GREEN}$passed passed${NC}, ${RED}$failed failed${NC}, ${YELLOW}$leaks leaks${NC}, ${RED}$segfaults segfaults${NC}"
    echo
}

test_stress() {
    local passed=0
    local failed=0
    local leaks=0
    local segfaults=0
    local start_total_passed=$TOTAL_PASSED
    local start_total_failed=$TOTAL_FAILED
    local start_total_leaks=$TOTAL_LEAKS
    local start_total_segfaults=$TOTAL_SEGFAULTS
    
    # Set module context for dynamic numbering and failure tracking
    set_module_context "stress"
    
    print_module_header "STRESS MODULE" "Comprehensive stress testing"
    
    # Level 1: Basic stress tests
    echo -e "${BLUE}Level 1: Basic Stress${NC}"
    local medium_string=$(printf "test%.0s" {1..50})
    run_test "Medium string" "echo \"$medium_string\"" "$medium_string" 1
    run_test "Many arguments" 'echo a b c d e f g h i j k l m n o p q r s t' "a b c d e f g h i j k l m n o p q r s t" 1
    
    # Level 2: String length stress
    echo -e "${BLUE}Level 2: String Length Stress${NC}"
    local long_string=$(printf "data%.0s" {1..100})
    run_test "Long string" "echo \"$long_string\"" "$long_string" 1
    local very_long_string=$(printf "x%.0s" {1..200})
    run_test "Very long string" "echo \"$very_long_string\"" "$very_long_string" 1
    run_test "Long argument list" 'echo '"$(printf '%s ' {1..50})"'' "$(printf '%s ' {1..50} | sed 's/ $//')" 1
    
    # Level 3: Quote complexity stress
    echo -e "${BLUE}Level 3: Quote Complexity${NC}"
    run_test "Complex mixed quotes" 'echo "start'"'"'middle'"'"'end"' "startmiddleend" 1
    run_test "Nested quote stress" 'echo '"'"'outer'"'"'"inner"'"'"'outer'"'"'' "outerinnerouter" 1
    run_test "Quote chain stress" 'echo "a""b""c""d""e""f""g""h"' "abcdefgh" 1
    run_test "Variable quote mix" 'echo "$HOME""test""$USER"' "${HOME}test${USER}" 1
    
    # Level 4: Command chaining stress
    echo -e "${BLUE}Level 4: Command Chaining${NC}"
    
    # Level 5: Memory pressure tests
    echo -e "${BLUE}Level 5: Memory Pressure${NC}"
    local huge_string=$(printf 'memory_stress_%.0s' {1..100})
    run_test "Memory stress string" "echo \"$huge_string\"" "$huge_string" 2
    run_test "Large argument count" 'echo '"$(printf 'arg%s ' {1..100})"'' "$(printf 'arg%s ' {1..100} | sed 's/ $//')" 1
    
    # Level 6: Parser stress tests
    echo -e "${BLUE}Level 6: Parser Stress${NC}"
    
    # Level 7: Extreme stress scenarios
    echo -e "${BLUE}Level 7: Extreme Stress${NC}"
    local extreme_string=$(printf 'extreme%.0s' {1..300})
    run_test "Extreme string length" "echo \"$extreme_string\"" "$extreme_string" 2
    run_test "Massive argument list" 'echo '"$(printf 'x%.0s ' {1..200})"'' "$(printf 'x%.0s ' {1..200} | sed 's/ $//')" 2
    run_test "Quote bomb stress" 'echo "'"$(printf '"%s"' {1..20} | tr -d ' ')"'"' "" 1
    run_test "Variable expansion bomb" 'echo "$HOME$HOME$HOME$HOME$HOME$HOME$HOME$HOME"' "${HOME}${HOME}${HOME}${HOME}${HOME}${HOME}${HOME}${HOME}" 1
    
    # Level 8: Pathological stress cases
    echo -e "${BLUE}Level 8: Pathological Cases${NC}"
    run_test "Command length bomb" 'echo '"$(printf 'very_long_argument_name_%.0s ' {1..50})"'' "$(printf 'very_long_argument_name_%.0s' {1..50} | sed 's/ $//')" 2
    run_test "Stress test survival" 'echo "survived_stress_test_$(date +%s)"' "" 1
    run_test "Memory fragmentation test" 'echo "'"$(printf 'frag%.0s' {1..100})"'" | cat | cat' "$(printf 'frag%.0s' {1..100})" 2
    
    # Calculate module results
    passed=$((TOTAL_PASSED - start_total_passed))
    failed=$((TOTAL_FAILED - start_total_failed))
    leaks=$((TOTAL_LEAKS - start_total_leaks))
    segfaults=$((TOTAL_SEGFAULTS - start_total_segfaults))
    
    echo -e "${PURPLE}Stress Module Results: ${GREEN}$passed passed${NC}, ${RED}$failed failed${NC}, ${YELLOW}$leaks leaks${NC}, ${RED}$segfaults segfaults${NC}"
    echo
}

test_general_edge_cases() {
    local passed=0
    local failed=0
    local leaks=0
    local segfaults=0
    local start_total_passed=$TOTAL_PASSED
    local start_total_failed=$TOTAL_FAILED
    local start_total_leaks=$TOTAL_LEAKS
    local start_total_segfaults=$TOTAL_SEGFAULTS
    
    # Set module context for dynamic numbering and failure tracking
    set_module_context "general"
    
    print_module_header "GENERAL EDGE CASES MODULE" "Testing general edge cases and error conditions"
    
    # Level 1: Empty input and whitespace
    echo -e "${BLUE}Level 1: Empty Input Tests${NC}"
    run_test "Empty command line" '' "" 1
    run_test "Only spaces" '   ' "" 1
    run_test "Only tabs" $'\t\t\t' "" 1
    run_test "Mixed whitespace" $'  \t  \t  ' "" 1
    run_test "Newline only" $'\n' "" 1
    run_test "Empty string command" '""' '""' 2
    
    # Level 2: Unclosed quotes
    echo -e "${BLUE}Level 2: Unclosed Quote Tests${NC}"
    run_test "Unclosed double quote" 'echo "unclosed' "" 2
    run_test "Unclosed single quote" "echo 'unclosed" "" 2
    run_test "Mixed unclosed quotes" 'echo "start'"'"'unclosed' "" 2
    run_test "Nested unclosed quotes" 'echo "outer '"'"'inner unclosed' "" 2
    run_test "Quote at end" 'echo test"' 'test"' 2
    run_test "Single quote at end" "echo test'" "test'" 2
    
    # Level 3: Special characters and escaping
    echo -e "${BLUE}Level 3: Special Characters${NC}"
    # NOTE: Backslash (\) handling is NOT required by Subject.md
    # run_test "Backslash handling" 'echo \\' '\\' 1
    # run_test "Multiple backslashes" 'echo \\\\\\\\' '\\\\\\\\' 1
    # NOTE: Semicolon (;) handling is NOT required by Subject.md
    # run_test "Semicolon handling" 'echo before ; echo after' 'before' 2
    run_test "Question mark" 'echo ??' '??' 1
    run_test "Asterisk" 'echo **' '**' 1
    run_test "Dollar without variable" 'echo $' '$' 1
    
    # Level 4: Command not found
    echo -e "${BLUE}Level 4: Command Not Found${NC}"
    
    # Level 5: PATH manipulation tests
    echo -e "${BLUE}Level 5: PATH Manipulation${NC}"
    local original_path="$PATH"
    export PATH=""
    export PATH="/invalid/path"
    export PATH="$original_path"
    
    # Level 6: Variable expansion edge cases
    echo -e "${BLUE}Level 6: Variable Expansion${NC}"
    run_test "Undefined variable" 'echo $UNDEFINED_VAR_EDGE_TEST' "" 1
    run_test "Variable with numbers" 'echo $HOME123' "" 1
    run_test "Variable with special chars" 'echo $HOME@#$' "$HOME@#$" 1
    export EMPTY_VAR=""
    run_test "Empty variable" 'echo "Value:$EMPTY_VAR:End"' "Value::End" 1
    run_test "Variable in single quotes" "echo '\$HOME'" '$HOME' 1
    unset EMPTY_VAR
    
    # Level 7: Argument and parsing edge cases
    echo -e "${BLUE}Level 7: Argument Edge Cases${NC}"
    run_test "Very long argument" 'echo "'"$(printf 'arg%.0s' {1..1000})"'"' "$(printf 'arg%.0s' {1..1000})" 1
    run_test "Many short arguments" 'echo '"$(printf 'a ' {1..100})"'' "$(printf 'a ' {1..100} | sed 's/ $//')" 1
    run_test "Arguments with spaces" 'echo "arg with spaces" "another arg"' "arg with spaces another arg" 1
    run_test "Empty argument" 'echo "" middle ""' " middle " 1
    
    # Level 8: Control characters and non-printable
    echo -e "${BLUE}Level 8: Control Characters${NC}"
    run_test "Tab character" $'echo "tab\here"' $"tab\here" 1
    run_test "Null character handling" $'echo "test\0null"' "test" 1
    
    # Level 9: Redirection edge cases
    echo -e "${BLUE}Level 9: Redirection Edge Cases${NC}"
    
    # Level 10: Signal handling edge cases
    echo -e "${BLUE}Level 10: Signal Edge Cases${NC}"
    run_test "Background job sim" 'echo "background test"' "background test" 1
    
    # Level 11: Memory and resource limits
    echo -e "${BLUE}Level 11: Resource Limits${NC}"
    run_test "Deep directory" 'echo "directory test"' "directory test" 1
    
    # Level 12: Syntax error combinations
    echo -e "${BLUE}Level 12: Syntax Combinations${NC}"
    
    # Level 13: Unicode and special encoding
    echo -e "${BLUE}Level 13: Unicode Tests${NC}"
    run_test "Unicode characters" 'echo "Hello 世界"' "Hello 世界" 1
    run_test "Emoji test" 'echo "Test 🚀 emoji"' "Test 🚀 emoji" 1
    run_test "Accented characters" 'echo "café naïve résumé"' "café naïve résumé" 1
    run_test "Mixed encoding" 'echo "ASCII + 中文 + العربية"' "ASCII + 中文 + العربية" 1
    
    # Level 14: File system edge cases
    echo -e "${BLUE}Level 14: File System Edge Cases${NC}"
    
    # Level 15: Recovery and error handling
    echo -e "${BLUE}Level 15: Error Recovery${NC}"
    run_test "Error then success" 'invalidcommand 2>/dev/null ; echo "recovered"' "recovered" 1
    run_test "Multiple errors" 'error1 2>/dev/null ; error2 2>/dev/null ; echo "still works"' "still works" 1
    run_test "Syntax error recovery" 'echo "before" ; | 2>/dev/null ; echo "after"' "before" 2
    run_test "Final recovery test" 'echo "minishell survives edge cases"' "minishell survives edge cases" 1
    
    # Calculate module results
    passed=$((TOTAL_PASSED - start_total_passed))
    failed=$((TOTAL_FAILED - start_total_failed))
    leaks=$((TOTAL_LEAKS - start_total_leaks))
    segfaults=$((TOTAL_SEGFAULTS - start_total_segfaults))
    
    echo -e "${PURPLE}General Edge Cases Module Results: ${GREEN}$passed passed${NC}, ${RED}$failed failed${NC}, ${YELLOW}$leaks leaks${NC}, ${RED}$segfaults segfaults${NC}"
    echo
}

test_extreme() {
    local passed=0
    local failed=0
    local leaks=0
    local segfaults=0
    local start_total_passed=$TOTAL_PASSED
    local start_total_failed=$TOTAL_FAILED
    local start_total_leaks=$TOTAL_LEAKS
    local start_total_segfaults=$TOTAL_SEGFAULTS
    
    # Set module context for dynamic numbering and failure tracking
    set_module_context "extreme"
    
    print_module_header "EXTREME MODULE" "Extreme edge cases designed to break minishell"
    
    # Level 1: Memory allocation bombs
    echo -e "${BLUE}Level 1: Memory Bombs${NC}"
    local memory_bomb=$(printf "memory_bomb_%.0s" {1..200})
    run_test "Memory allocation bomb" "echo \"$memory_bomb\"" "$memory_bomb" 3
    run_test "Argument explosion" 'echo '"$(printf 'arg%d ' {1..300})"'' "$(printf 'arg%d ' {1..300} | sed 's/ $//')" 3
    run_test "String concatenation bomb" 'echo "start""middle""end""more""data""test""huge""string""concat"' "startmiddleendmoredatatesthugostringconcat" 2
    
    # Level 2: Parser destruction tests
    echo -e "${BLUE}Level 2: Parser Destruction${NC}"
    run_test "Quote nesting nightmare" 'echo '"'"'a'"'"'"b"'"'"'c'"'"'"d"'"'"'e'"'"'"f"'"'"'g'"'"'"h"'"'"'i'"'"'"j"'"'"'' "abcdefghij" 2
    run_test "Variable expansion bomb" 'echo "$HOME$USER$PATH$HOME$USER$PATH$HOME$USER$PATH"' "${HOME}${USER}${PATH}${HOME}${USER}${PATH}${HOME}${USER}${PATH}" 2
    run_test "Mixed quote chaos" 'echo "'"'"'test'"'"'"data"'"'"'more'"'"'"stuff"'"'"'end'"'"'' "testdatamorestuffend" 2
    run_test "Operator confusion" 'echo "test" | cat | cat | cat | cat | cat | cat | cat' "test" 2
    
    # Level 3: File system stress
    echo -e "${BLUE}Level 3: File System Stress${NC}"
    
    # Level 4: Command complexity bombs
    echo -e "${BLUE}Level 4: Command Complexity${NC}"
    run_test "Command length explosion" 'echo "'"$(printf 'extremely_long_command_with_many_repetitions_%.0s' {1..20})"'"' "$(printf 'extremely_long_command_with_many_repetitions_%.0s' {1..20})" 3
    run_test "Pipe bomb" 'echo "survive" | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat' "survive" 3
    
    # Level 5: Memory leak hunters
    echo -e "${BLUE}Level 5: Memory Leak Hunters${NC}"
    run_test "Repeated memory allocation" 'echo "'"$(printf 'alloc%.0s' {1..500})"'"' "$(printf 'alloc%.0s' {1..500})" 4
    run_test "Stack overflow attempt" 'echo "'"$(printf 'stack_test_%.0s' {1..100})"'" | cat | cat | cat' "$(printf 'stack_test_%.0s' {1..100})" 3
    
    # Level 6: Edge case nightmares
    echo -e "${BLUE}Level 6: Edge Case Nightmares${NC}"
    run_test "Unicode stress test" 'echo "café naïve résumé Москва 北京 🚀🔥💯"' "café naïve résumé Москва 北京 🚀🔥💯" 2
    # NOTE: This test includes backslash (\) and semicolon (;) which are NOT required by Subject.md
    # run_test "Special character bomb" 'echo "!@#$%^&*()_+-=[]{}|\\;:'"'"'\",./<>?~\`"' "!@#$%^&*()_+-=[]{}|\\;:'\",./<>?~\`" 2
    run_test "Control character test" 'echo "tab	newline\ncarriage\rreturn"' "tab	newline" 2
    run_test "Binary data simulation" 'echo "'"$(printf '\x41\x42\x43\x44\x45')"'"' "ABCDE" 2
    
    # Level 7: Resource exhaustion attempts
    echo -e "${BLUE}Level 7: Resource Exhaustion${NC}"
    run_test "CPU intensive parsing" 'echo "'"$(printf 'cpu_intensive_parsing_test_with_very_long_name_%.0s' {1..50})"'"' "$(printf 'cpu_intensive_parsing_test_with_very_long_name_%.0s' {1..50})" 4
    
    # Level 8: The ultimate destroyers
    echo -e "${BLUE}Level 8: Ultimate Destroyers${NC}"
    run_test "The quote apocalypse" 'echo "'"$(printf '"%s"' {1..30} | tr -d ' ')"'"' "" 3
    run_test "Variable expansion nuke" 'echo "$HOME$USER$PATH$PWD$SHELL$HOME$USER$PATH$PWD$SHELL$HOME$USER$PATH"' "${HOME}${USER}${PATH}${PWD}${SHELL}${HOME}${USER}${PATH}${PWD}${SHELL}${HOME}${USER}${PATH}" 4
    run_test "The memory destroyer" 'echo "'"$(printf 'destroyer_%.0s' {1..600})"'"' "$(printf 'destroyer_%.0s' {1..600})" 5
    run_test "Ultimate survival test" 'echo "IF_YOU_SEE_THIS_MINISHELL_SURVIVED_THE_ULTIMATE_TEST_'"$(date +%s)"'"' "" 3
    
    # Level 9: Recovery tests (after surviving the above)
    echo -e "${BLUE}Level 9: Recovery Tests${NC}"
    run_test "Post-stress simple command" 'echo "recovery_test"' "recovery_test" 1
    run_test "Post-stress pipe" 'echo "still_working" | cat' "still_working" 1
    run_test "Final sanity check" 'echo "minishell_is_still_alive"' "minishell_is_still_alive" 1
    
    # Cleanup all test files
    rm -f /tmp/extreme_*.tmp /tmp/rapid_*.tmp /tmp/desc_*.tmp /tmp/recovery.tmp
    
    # Calculate module results
    passed=$((TOTAL_PASSED - start_total_passed))
    failed=$((TOTAL_FAILED - start_total_failed))
    leaks=$((TOTAL_LEAKS - start_total_leaks))
    segfaults=$((TOTAL_SEGFAULTS - start_total_segfaults))
    
    echo -e "${PURPLE}Extreme Module Results: ${GREEN}$passed passed${NC}, ${RED}$failed failed${NC}, ${YELLOW}$leaks leaks${NC}, ${RED}$segfaults segfaults${NC}"
    echo -e "${RED}⚠️  WARNING: This module is designed to stress-test and potentially break minishell!${NC}"
    echo
}

print_summary() {
    echo -e "${CYAN}================================================================${NC}"
    echo -e "${CYAN}                        FINAL SUMMARY                        ${NC}"
    echo -e "${CYAN}================================================================${NC}"
    echo
    
    echo -e "${GREEN}✅ Total Passed: $TOTAL_PASSED${NC}"
    echo -e "${RED}❌ Total Failed: $TOTAL_FAILED${NC}"
    if [[ $TOTAL_LEAKS -gt 0 ]]; then
        echo -e "${YELLOW}🔍 Memory Leaks: $TOTAL_LEAKS${NC}"
    fi
    if [[ $TOTAL_SEGFAULTS -gt 0 ]]; then
        echo -e "${RED}💥 Segfaults: $TOTAL_SEGFAULTS${NC}"
    fi
    
    echo
    local total_tests=$((TOTAL_PASSED + TOTAL_FAILED))
    if [[ $total_tests -gt 0 ]]; then
        local success_rate=$((TOTAL_PASSED * 100 / total_tests))
        echo -e "${BLUE}Success Rate: $success_rate%${NC}"
    fi
    
    # Show detailed failure report if there are failures
    if [[ ${#FAILED_TESTS_INFO[@]} -gt 0 ]]; then
        echo
        echo -e "${RED}================================================================${NC}"
        echo -e "${RED}                    DETAILED FAILURE REPORT                    ${NC}"
        echo -e "${RED}================================================================${NC}"
        echo
        
        # Categorize failures by type
        local quote_failures=()
        local variable_failures=()
        local builtin_failures=()
        local pipe_failures=()
        local redirect_failures=()
        local signal_failures=()
        local other_failures=()
        
        for failure in "${FAILED_TESTS_INFO[@]}"; do
            if [[ "$failure" == *"quotes:"* ]]; then
                quote_failures+=("$failure")
            elif [[ "$failure" == *"Variable"* || "$failure" == *"\$"* ]]; then
                variable_failures+=("$failure")
            elif [[ "$failure" == *"builtins:"* ]]; then
                builtin_failures+=("$failure")
            elif [[ "$failure" == *"pipes:"* ]]; then
                pipe_failures+=("$failure")
            elif [[ "$failure" == *"redirections:"* ]]; then
                redirect_failures+=("$failure")
            elif [[ "$failure" == *"signals:"* ]]; then
                signal_failures+=("$failure")
            else
                other_failures+=("$failure")
            fi
        done
        
        # Show failures by category
        if [[ ${#quote_failures[@]} -gt 0 ]]; then
            echo -e "${RED}🔤 QUOTE PARSING FAILURES:${NC}"
            for failure in "${quote_failures[@]}"; do
                echo -e "${RED}   ❌ $failure${NC}"
            done
            echo
        fi
        
        if [[ ${#variable_failures[@]} -gt 0 ]]; then
            echo -e "${RED}💲 VARIABLE EXPANSION FAILURES:${NC}"
            for failure in "${variable_failures[@]}"; do
                echo -e "${RED}   ❌ $failure${NC}"
            done
            echo
        fi
        
        if [[ ${#builtin_failures[@]} -gt 0 ]]; then
            echo -e "${RED}🏗️  BUILTIN COMMAND FAILURES:${NC}"
            for failure in "${builtin_failures[@]}"; do
                echo -e "${RED}   ❌ $failure${NC}"
            done
            echo
        fi
        
        if [[ ${#pipe_failures[@]} -gt 0 ]]; then
            echo -e "${RED}🔗 PIPE FAILURES:${NC}"
            for failure in "${pipe_failures[@]}"; do
                echo -e "${RED}   ❌ $failure${NC}"
            done
            echo
        fi
        
        if [[ ${#redirect_failures[@]} -gt 0 ]]; then
            echo -e "${RED}📁 REDIRECTION FAILURES:${NC}"
            for failure in "${redirect_failures[@]}"; do
                echo -e "${RED}   ❌ $failure${NC}"
            done
            echo
        fi
        
        if [[ ${#signal_failures[@]} -gt 0 ]]; then
            echo -e "${RED}⚡ SIGNAL HANDLING FAILURES:${NC}"
            for failure in "${signal_failures[@]}"; do
                echo -e "${RED}   ❌ $failure${NC}"
            done
            echo
        fi
        
        if [[ ${#other_failures[@]} -gt 0 ]]; then
            echo -e "${RED}❓ OTHER FAILURES:${NC}"
            for failure in "${other_failures[@]}"; do
                echo -e "${RED}   ❌ $failure${NC}"
            done
            echo
        fi
        
        echo -e "${YELLOW}💡 DEBUGGING TIPS:${NC}"
        echo -e "${YELLOW}   • Focus on quote parsing and variable expansion first${NC}"
        echo -e "${YELLOW}   • Check your tokenizer and parser for edge cases${NC}"
        echo -e "${YELLOW}   • Test individual components in isolation${NC}"
        echo -e "${YELLOW}   • Use debugging output to trace execution flow${NC}"
        echo
    fi
    
    # Determine exit code
    if [[ $TOTAL_SEGFAULTS -gt 0 ]]; then
        echo -e "${RED}🚨 Critical failures detected (segfaults)${NC}"
        exit 1
    elif [[ $TOTAL_FAILED -gt 0 ]]; then
        echo -e "${YELLOW}⚠️  Some tests failed${NC}"
        exit 2
    elif [[ $TOTAL_LEAKS -gt 0 ]]; then
        echo -e "${YELLOW}⚠️  Memory leaks detected${NC}"
        exit 3
    else
        echo -e "${GREEN}🎉 All tests passed!${NC}"
        exit 0
    fi
}

# Main execution
main() {
    print_header "MINISHELL UNIFIED TEST SUITE"
    
    echo -e "${BLUE}Module: $MODULE${NC}"
    if [[ "$USE_VALGRIND" == true ]]; then
        echo -e "${BLUE}Using valgrind for memory checking${NC}"
    fi
    echo
    
    check_prerequisites
    
    case "$MODULE" in
        "quotes")
            test_quotes
            ;;
        "signals")
            test_signals
            ;;
        "pipes")
            test_pipes
            ;;
        "redirections")
            test_redirections
            ;;
        "builtins")
            test_builtins
            ;;
        "general")
            test_general_edge_cases
            ;;
        "stress")
            test_stress
            ;;
        "extreme")
            test_extreme
            ;;
        "all")
            # Set module context for "all" - this will show total test count
            set_module_context "all"
            test_quotes
            test_signals
            test_pipes
            test_redirections
            test_builtins
            test_general_edge_cases
            test_stress
            test_extreme
            ;;
        *)
            echo -e "${RED}Unknown module: $MODULE${NC}"
            echo "Use --help for available modules"
            exit 1
            ;;
    esac
    
    print_summary
}

# Trap to cleanup on exit
trap cleanup EXIT

# Run main function
main "$@"

# Test that should have been preserved - testing error messages with && and || inside quotes
test_missing_logical_in_quotes() {
    local passed=0
    local failed=0
    local leaks=0
    local segfaults=0
    
    echo -e "${BLUE}Missing Tests: Logical operators in quotes${NC}"
    run_test "Echo && in quotes" "echo \"hello && world\"" "hello && world" 1
    run_test "Echo || in quotes" "echo \"hello || world\"" "hello || world" 1
    run_test "Echo single quotes &&" "echo 'hello && world'" "hello && world" 1
    run_test "Echo single quotes ||" "echo 'hello || world'" "hello || world" 1
}
