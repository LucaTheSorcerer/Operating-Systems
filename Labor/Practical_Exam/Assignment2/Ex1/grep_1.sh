if [ $# -ne 1 ]; then
    echo "Usage: $0 <folderr>"
    exit 1
fi

grep -r -h -v --include="*.txt" "" $1