#!/bin/bash

show_help() {
    cat << 'EOF'
Usage: solution.sh [OPTION]... -d DIRECTORY -k KEY
Encode all text files in DIRECTORY and its subdirectories using Caesar cipher.
Files are modified in-place. Only latin letters are shifted; other chars unchanged.

Mandatory arguments to long options are mandatory for short options too.
  -d, --directory DIR    path to the target directory
  -k, --key KEY          shift value (integer, e.g. 3)
  -h, --help             display this help and exit

Examples:
  solution.sh -d ./texts -k 3
  solution.sh --directory /home/user/docs --key 13
EOF
}

make_tr_args() {
    local shift=$1

    shift=$(( shift % 26 ))
    if [ "$shift" -lt 0 ]; then
        shift=$(( shift + 26 ))
    fi

    local lower="abcdefghijklmnopqrstuvwxyz"
    local upper="ABCDEFGHIJKLMNOPQRSTUVWXYZ"

    local shifted_lower="${lower:$shift}${lower:0:$shift}"
    local shifted_upper="${upper:$shift}${upper:0:$shift}"

    TR_FROM="${lower}${upper}"
    TR_TO="${shifted_lower}${shifted_upper}"
}

DIRECTORY=""
KEY=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        -h|--help)
            show_help
            exit 0
            ;;
        -d|--directory)
            DIRECTORY="$2"
            shift 2
            ;;
        -k|--key)
            KEY="$2"
            shift 2
            ;;
        *)
            echo "Unknown option: $1"
            show_help
            exit 1
            ;;
    esac
done

if [ -z "$DIRECTORY" ] || [ -z "$KEY" ]; then
    show_help
    exit 1
fi

if ! [[ "$KEY" =~ ^-?[0-9]+$ ]]; then
    echo "Error: KEY must be an integer."
    show_help
    exit 1
fi

if [ ! -d "$DIRECTORY" ]; then
    echo "Error: directory '$DIRECTORY' does not exist."
    exit 1
fi

make_tr_args "$KEY"

find "$DIRECTORY" -type f -name "*.txt" | while IFS= read -r file; do
    echo "Encoding: $file"
    local_tmp=$(mktemp)
    tr "$TR_FROM" "$TR_TO" < "$file" > "$local_tmp"
    mv "$local_tmp" "$file"
done

echo "Done. All .txt files in '$DIRECTORY' have been encoded with key=$KEY."
