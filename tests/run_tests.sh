#!/usr/bin/env bash
# Basic smoke tests for this PractRand build -- NOT a statistical
# certification of any generator. It just confirms:
#   1. RNG_test's own self-test passes (validates the bundled RNGs against
#      known test vectors).
#   2. A couple of built-in RNGs run cleanly through RNG_test.
#   3. Each example generator in examples/ produces sane-looking bytes and
#      runs cleanly through RNG_test over stdin.
# This exists to catch build breakage, a mis-wired generator (wrong output
# width, forgotten seed, etc.), or a broken pipe -- not to certify quality.
# For real statistical testing, run RNG_test at realistic lengths yourself;
# see readme.md.
#
# Usage: tests/run_tests.sh [test-length]
#   test-length defaults to 32M. Accepts anything RNG_test's -tlmin/-tlmax
#   understand, e.g. 64M, 1G, 23 (log2 bytes).

set -u
cd "$(dirname "$0")/.."

TEST_LENGTH=${1:-32M}
FAILURES=0
LOG=$(mktemp)
trap 'rm -f "$LOG"' EXIT

pass() { echo "  PASS: $1"; }
fail() { echo "  FAIL: $1"; FAILURES=$((FAILURES + 1)); }

require_binary() {
	if [ ! -x "$1" ]; then
		echo "error: $1 not found or not executable -- run 'make' first" >&2
		exit 2
	fi
}

# Runs RNG_test over a fixed-length stream and fails the case if the word
# FAIL appears in its output, or if the pipeline produced no output at all.
check_rng_test() {
	local description="$1"
	shift
	if ! "$@" >"$LOG" 2>&1; then
		fail "$description (RNG_test exited non-zero, see $LOG)"
		return
	fi
	if [ ! -s "$LOG" ]; then
		fail "$description (no output at all)"
		return
	fi
	if grep -q "FAIL" "$LOG"; then
		fail "$description (RNG_test reported a FAIL)"
		grep -B2 "FAIL" "$LOG" | sed 's/^/    /'
		return
	fi
	pass "$description"
}

require_binary ./RNG_test

echo "== RNG_test self-test =="
if ./RNG_test --self_test >"$LOG" 2>&1; then
	pass "RNG_test --self_test"
else
	fail "RNG_test --self_test"
	sed 's/^/    /' "$LOG"
fi

echo "== built-in RNGs =="
check_rng_test "sfc64 (built in)" \
	./RNG_test sfc64 -tlmin "$TEST_LENGTH" -tlmax "$TEST_LENGTH"
check_rng_test "chacha(20) (built in)" \
	./RNG_test "chacha(20)" -tlmin "$TEST_LENGTH" -tlmax "$TEST_LENGTH"

echo "== example generators (examples/) =="
for spec in \
	"pcg32:stdin32:12345 6789" \
	"xoshiro256starstar:stdin64:abcdef" \
	"chacha20:stdin32:deadbeef"
do
	name=${spec%%:*}
	rest=${spec#*:}
	stdin_mode=${rest%%:*}
	seed_args=${rest#*:}
	bin="examples/$name"

	if [ ! -x "$bin" ]; then
		fail "$name (not built -- run 'make examples' first)"
		continue
	fi

	# hexdump sanity check: the first 64 bytes shouldn't be all-zero or
	# a single repeated byte -- catches a forgotten seed or a totally
	# broken generator before even involving RNG_test.
	sample=$("$bin" $seed_args 2>/dev/null | head -c 64 | od -An -tx1 | tr -d ' \n')
	if [ -z "$sample" ]; then
		fail "$name (produced no output at all)"
		continue
	fi
	first_byte=${sample:0:2}
	if [[ "$sample" =~ ^($first_byte)+$ ]]; then
		fail "$name (first 64 bytes are all the same byte, 0x$first_byte)"
		continue
	fi

	check_rng_test "$name (via $stdin_mode)" \
		bash -c "\"$bin\" $seed_args 2>/dev/null | ./RNG_test $stdin_mode -tlmin $TEST_LENGTH -tlmax $TEST_LENGTH"
done

echo
if [ "$FAILURES" -eq 0 ]; then
	echo "All checks passed."
	exit 0
else
	echo "$FAILURES check(s) failed."
	exit 1
fi
