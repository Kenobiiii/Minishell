#!/bin/bash

# Master Test Suite - Runs all tests for Minishell
# Comprehensive testing for functionality, memory safety, and robustness

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

MINISHELL="./minishell"

echo -e "${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                 MINISHELL MASTER TEST SUITE                ║${NC}"
echo -e "${CYAN}║          Comprehensive Testing & Validation               ║${NC}"
echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"
echo

# Check if minishell exists
if [[ ! -f "$MINISHELL" ]]; then
    echo -e "${RED}❌ Minishell executable not found: $MINISHELL${NC}"
    echo "Please compile your minishell first with 'make'"
    exit 1
fi

echo -e "${BLUE}🔧 Minishell executable found: $MINISHELL${NC}"
echo

# Function to run a test suite
run_test_suite() {
    local suite_name="$1"
    local script_path="$2"
    local description="$3"
    
    echo -e "${YELLOW}╔══════════════════════════════════════════════════════════╗${NC}"
    echo -e "${YELLOW}║ $suite_name${NC}"
    echo -e "${YELLOW}║ $description${NC}"
    echo -e "${YELLOW}╚══════════════════════════════════════════════════════════╝${NC}"
    echo
    
    if [[ ! -f "$script_path" ]]; then
        echo -e "${RED}❌ Test script not found: $script_path${NC}"
        return 1
    fi
    
    bash "$script_path"
    local exit_code=$?
    
    echo
    case $exit_code in
        0)
            echo -e "${GREEN}✅ $suite_name PASSED${NC}"
            ;;
        1)
            echo -e "${RED}💥 $suite_name CRITICAL FAILURE (Segfaults)${NC}"
            return 1
            ;;
        2)
            echo -e "${YELLOW}⚠️  $suite_name WARNING (Memory leaks)${NC}"
            ;;
        3)
            echo -e "${YELLOW}⚠️  $suite_name MINOR FAILURES${NC}"
            ;;
        *)
            echo -e "${RED}❌ $suite_name UNKNOWN ERROR (Exit code: $exit_code)${NC}"
            ;;
    esac
    
    echo
    return $exit_code
}

# Track overall results
TOTAL_SUITES=0
PASSED_SUITES=0
CRITICAL_FAILURES=0
WARNINGS=0

# Arrays to track which suites had issues
declare -a FAILED_SUITES=()
declare -a WARNING_SUITES=()
declare -a CRITICAL_SUITES=()

# 1. Basic Quote Tests
if [[ -f "test_quotes.sh" ]]; then
    ((TOTAL_SUITES++))
    run_test_suite "BASIC QUOTE TESTS" "test_quotes.sh" "Testing basic quote functionality"
    case $? in
        0) ((PASSED_SUITES++)) ;;
        1) ((CRITICAL_FAILURES++)); CRITICAL_SUITES+=("Basic Quote Tests") ;;
        *) ((WARNINGS++)); WARNING_SUITES+=("Basic Quote Tests") ;;
    esac
fi

# 2. Final Validation Tests
if [[ -f "final_test.sh" ]]; then
    ((TOTAL_SUITES++))
    run_test_suite "FINAL VALIDATION" "final_test.sh" "Final validation tests"
    case $? in
        0) ((PASSED_SUITES++)) ;;
        1) ((CRITICAL_FAILURES++)); CRITICAL_SUITES+=("Final Validation") ;;
        *) ((WARNINGS++)); WARNING_SUITES+=("Final Validation") ;;
    esac
fi

# 3. Comprehensive Stress Tests
if [[ -f "comprehensive_stress_test.sh" ]]; then
    ((TOTAL_SUITES++))
    run_test_suite "COMPREHENSIVE STRESS TESTS" "comprehensive_stress_test.sh" "Extensive testing with valgrind"
    case $? in
        0) ((PASSED_SUITES++)) ;;
        1) ((CRITICAL_FAILURES++)); CRITICAL_SUITES+=("Comprehensive Stress Tests") ;;
        *) ((WARNINGS++)); WARNING_SUITES+=("Comprehensive Stress Tests") ;;
    esac
fi

# 4. Extreme Edge Case Tests
if [[ -f "extreme_stress_test.sh" ]]; then
    ((TOTAL_SUITES++))
    run_test_suite "EXTREME EDGE CASES" "extreme_stress_test.sh" "Extreme performance and edge cases"
    case $? in
        0) ((PASSED_SUITES++)) ;;
        1) ((CRITICAL_FAILURES++)); CRITICAL_SUITES+=("Extreme Edge Cases") ;;
        *) ((WARNINGS++)); WARNING_SUITES+=("Extreme Edge Cases") ;;
    esac
fi

# Additional manual tests that might be helpful
echo -e "${YELLOW}╔══════════════════════════════════════════════════════════╗${NC}"
echo -e "${YELLOW}║ INTERACTIVE TESTS RECOMMENDATIONS                       ║${NC}"
echo -e "${YELLOW}╚══════════════════════════════════════════════════════════╝${NC}"
echo
echo -e "${CYAN}🔧 Manual tests you might want to run:${NC}"
echo
echo "1. Test with real shell commands:"
echo "   ./minishell"
echo "   ls -la"
echo "   pwd"
echo "   env | grep HOME"
echo
echo "2. Test signal handling:"
echo "   ./minishell"
echo "   sleep 10   (then press Ctrl+C)"
echo "   (press Ctrl+D to exit)"
echo
echo "3. Test with your specific environment:"
echo "   ./minishell"
echo "   echo \$HOME"
echo "   echo \"\$HOME\"'\$HOME'"
echo "   cd /tmp && pwd"
echo
echo "4. Test redirections and pipes:"
echo "   ./minishell"
echo "   echo hello > test.txt"
echo "   cat test.txt"
echo "   ls | grep minishell"
echo

# Print final summary
echo -e "${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                    FINAL SUMMARY                          ║${NC}"
echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"
echo
echo -e "Total test suites run: ${BLUE}$TOTAL_SUITES${NC}"
echo -e "Suites passed: ${GREEN}$PASSED_SUITES${NC}"
echo -e "Critical failures: ${RED}$CRITICAL_FAILURES${NC}"
echo -e "Warnings: ${YELLOW}$WARNINGS${NC}"
echo

# Show detailed information about failures
if [[ ${#CRITICAL_SUITES[@]} -gt 0 ]]; then
    echo -e "${RED}💥 CRITICAL FAILURES IN:${NC}"
    for suite in "${CRITICAL_SUITES[@]}"; do
        echo -e "   ${RED}• $suite${NC}"
    done
    echo
fi

if [[ ${#WARNING_SUITES[@]} -gt 0 ]]; then
    echo -e "${YELLOW}⚠️  WARNINGS IN:${NC}"
    for suite in "${WARNING_SUITES[@]}"; do
        echo -e "   ${YELLOW}• $suite${NC}"
    done
    echo
fi

if [[ $CRITICAL_FAILURES -gt 0 ]]; then
    echo -e "${RED}💥 CRITICAL ISSUES DETECTED!${NC}"
    echo -e "${RED}Your minishell has segfaults or serious memory errors.${NC}"
    echo -e "${RED}Please fix these issues before submission.${NC}"
    exit 1
elif [[ $WARNINGS -gt 0 ]]; then
    echo -e "${YELLOW}⚠️  Some issues detected (memory leaks or minor failures).${NC}"
    echo -e "${YELLOW}Consider reviewing and fixing these for better quality.${NC}"
    exit 2
elif [[ $PASSED_SUITES -eq $TOTAL_SUITES ]] && [[ $TOTAL_SUITES -gt 0 ]]; then
    echo -e "${GREEN}🎉 ALL TESTS PASSED!${NC}"
    echo -e "${GREEN}Your minishell appears to be working correctly!${NC}"
    echo -e "${GREEN}✅ No segfaults detected${NC}"
    echo -e "${GREEN}✅ No memory leaks detected${NC}"
    echo -e "${GREEN}✅ All functionality tests passed${NC}"
    exit 0
else
    echo -e "${YELLOW}⚠️  No test suites found or some didn't run.${NC}"
    echo -e "${YELLOW}Make sure all test scripts exist in the current directory.${NC}"
    exit 3
fi
