set -e

TESTS=(
    "test_int16_random"
    "test_int8_random"
    "test_int4_random"
    "test_ternary_random"
    "test_mask"
    "test_pipeline"
    "test_reset"
)

PASS_COUNT=0
FAIL_COUNT=0

for t in "${TESTS[@]}"; do
    echo "========== Running: $t =========="
    if ./simv +UVM_TESTNAME=$t \
        +UVM_VERBOSITY=UVM_LOW \
        -l logs/${t}.log; then
        echo "PASS: $t"
        ((PASS_COUNT++))
    else
        echo "FAIL: $t"
        ((FAIL_COUNT++))
    fi
done

echo ""
echo "============================================"
echo "Regression Summary: $PASS_COUNT PASS, $FAIL_COUNT FAIL"
echo "============================================"
EOF

chmod +x scripts/run_regression.sh