#!/bin/bash

# UNIFIED MASTER TEST SUITE FOR MINISHELL
# Usage: ./master_test.sh [module] [options]
# Modules: quotes, signals, pipes, redirections, builtins, stress, extreme, all
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

# Parse command line arguments
MODULE=""
while [[ $# -gt 0 ]]; do
    case $1 in
        quotes|signals|pipes|redirections|builtins|stress|extreme|all)
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
print_header() {
    local title="$1"
    echo -e "${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${CYAN}║$(printf "%62s" "$title" | sed 's/ / /g')║${NC}"
    echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"
    echo
}

print_module_header() {
    local module="$1"
    local description="$2"
    echo -e "${YELLOW}╔══════════════════════════════════════════════════════════╗${NC}"
    echo -e "${YELLOW}║ $module${NC}"
    echo -e "${YELLOW}║ $description${NC}"
    echo -e "${YELLOW}╚══════════════════════════════════════════════════════════╝${NC}"
    echo
}

run_test() {
    local test_name="$1"
    local command="$2"
    local expected="$3"
    local should_pass="$4"
    
    if [[ "$VERBOSE" == true ]]; then
        echo -e "${BLUE}Testing: $test_name${NC}"
        echo "Command: $command"
    else
        echo -n "Testing: $test_name... "
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
        if ! echo "$minishell_output" | grep -q "$expected"; then
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
    
    print_module_header "QUOTES MODULE" "Testing quote handling and variable expansion"
    
    # Basic quote tests
    run_test "Simple double quotes" 'echo "hello world"' "hello world" 1
    run_test "Simple single quotes" "echo 'hello world'" "hello world" 1
    run_test "Empty double quotes" 'echo ""' "" 1
    run_test "Empty single quotes" "echo ''" "" 1
    
    # Variable expansion
    run_test "Variable in double quotes" 'echo "$HOME"' "$HOME" 1
    run_test "Variable in single quotes" "echo '\$HOME'" '\$HOME' 1
    run_test "Multiple variables" 'echo "$HOME$USER"' "${HOME}${USER}" 1
    
    # Mixed quotes
    run_test "Mixed quotes simple" 'echo "hello" '"'"'world'"'"'' "hello world" 1
    run_test "Nested quotes" 'echo "outer '"'"'inner'"'"' quotes"' "outer inner quotes" 1
    
    # Edge cases
    run_test "Quote at end of line" 'echo "test"' "test" 1
    run_test "Multiple quoted strings" 'echo "first" "second"' "first second" 1
    
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
    
    print_module_header "SIGNALS MODULE" "Testing signal handling (automated tests only)"
    
    # Basic functionality tests
    run_test "Basic command execution" 'echo "signal test"' "signal test" 1
    run_test "Exit command" 'exit' "" 1
    
    # Signal handling simulation tests
    run_test "Command with timeout" 'sleep 1' "" 1
    
    # Test signal handling infrastructure
    echo -e "${BLUE}Testing signal handling infrastructure...${NC}"
    
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
    
    echo -e "${YELLOW}Note: Manual testing required for full signal validation:${NC}"
    echo -e "${YELLOW}  • Ctrl+C at prompt (should show new prompt)${NC}"
    echo -e "${YELLOW}  • Ctrl+\\ at prompt (should be ignored)${NC}"
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
    
    print_module_header "PIPES MODULE" "Testing pipe functionality"
    
    # Basic pipe tests
    run_test "Simple pipe" 'echo "hello" | cat' "hello" 1
    run_test "Echo to grep" 'echo "hello world" | grep "world"' "hello world" 1
    run_test "Multiple pipes" 'echo "test" | cat | cat' "test" 1
    
    # Pipe with built-ins
    run_test "Echo to echo via pipe" 'echo "first" | cat' "first" 1
    
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
    
    print_module_header "REDIRECTIONS MODULE" "Testing redirection functionality"
    
    # Output redirection
    run_test "Output redirection" 'echo "test" > /tmp/test_out.txt && cat /tmp/test_out.txt' "test" 1
    run_test "Append redirection" 'echo "line1" > /tmp/test_append.txt && echo "line2" >> /tmp/test_append.txt && cat /tmp/test_append.txt' "line1" 1
    
    # Input redirection
    echo "input_test" > /tmp/test_input.txt
    run_test "Input redirection" 'cat < /tmp/test_input.txt' "input_test" 1
    
    # Cleanup test files
    rm -f /tmp/test_out.txt /tmp/test_append.txt /tmp/test_input.txt
    
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
    
    print_module_header "BUILTINS MODULE" "Testing built-in commands"
    
    # Echo tests
    run_test "echo simple" 'echo hello' "hello" 1
    run_test "echo with -n" 'echo -n hello' "hello" 1
    
    # pwd test
    run_test "pwd command" 'pwd' "$(pwd)" 1
    
    # env test (just check it runs without error)
    run_test "env command" 'env | head -1' "" 1
    
    # cd test (check if it exists, don't change directory)
    run_test "cd to current dir" 'cd .' "" 1
    
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
    
    print_module_header "STRESS MODULE" "Comprehensive stress testing"
    
    # Long strings
    local long_string=$(printf "a%.0s" {1..100})
    run_test "Long string" "echo \"$long_string\"" "$long_string" 1
    
    # Many arguments
    run_test "Many arguments" 'echo a b c d e f g h i j k l m n o p' "a b c d e f g h i j k l m n o p" 1
    
    # Complex quotes
    run_test "Complex mixed quotes" 'echo "start'"'"'middle'"'"'end"' "startmiddleend" 1
    
    # Multiple commands
    run_test "Multiple commands" 'echo "first" && echo "second"' "first" 1
    
    # Calculate module results
    passed=$((TOTAL_PASSED - start_total_passed))
    failed=$((TOTAL_FAILED - start_total_failed))
    leaks=$((TOTAL_LEAKS - start_total_leaks))
    segfaults=$((TOTAL_SEGFAULTS - start_total_segfaults))
    
    echo -e "${PURPLE}Stress Module Results: ${GREEN}$passed passed${NC}, ${RED}$failed failed${NC}, ${YELLOW}$leaks leaks${NC}, ${RED}$segfaults segfaults${NC}"
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
    
    print_module_header "EXTREME MODULE" "Extreme edge cases"
    
    # Very long command line
    local very_long=$(printf "very_long_test_%.0s" {1..50})
    run_test "Very long command" "echo \"$very_long\"" "$very_long" 1
    
    # Special characters
    run_test "Special characters" 'echo "!@#$%^&*()"' "!@#\$%^&*()" 1
    
    # Empty command
    run_test "Empty command" '' "" 1
    
    # Whitespace handling
    run_test "Multiple spaces" 'echo    "test"    ' "test" 1
    
    # Calculate module results
    passed=$((TOTAL_PASSED - start_total_passed))
    failed=$((TOTAL_FAILED - start_total_failed))
    leaks=$((TOTAL_LEAKS - start_total_leaks))
    segfaults=$((TOTAL_SEGFAULTS - start_total_segfaults))
    
    echo -e "${PURPLE}Extreme Module Results: ${GREEN}$passed passed${NC}, ${RED}$failed failed${NC}, ${YELLOW}$leaks leaks${NC}, ${RED}$segfaults segfaults${NC}"
    echo
}

print_summary() {
    echo -e "${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${CYAN}║                        FINAL SUMMARY                       ║${NC}"
    echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"
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
        "stress")
            test_stress
            ;;
        "extreme")
            test_extreme
            ;;
        "all")
            test_quotes
            test_signals
            test_pipes
            test_redirections
            test_builtins
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
