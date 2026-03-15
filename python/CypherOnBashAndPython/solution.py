#!/usr/bin/env python3

import argparse
import os
import subprocess
import tempfile


def make_tr_args(shift: int) -> tuple[str, str]:
    shift = shift % 26

    lower = "abcdefghijklmnopqrstuvwxyz"
    upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

    shifted_lower = lower[shift:] + lower[:shift]
    shifted_upper = upper[shift:] + upper[:shift]

    tr_from = lower + upper
    tr_to   = shifted_lower + shifted_upper

    return tr_from, tr_to


def encode_file(filepath: str, tr_from: str, tr_to: str) -> None:
    print(f"Encoding: {filepath}")

    with tempfile.NamedTemporaryFile(delete=False) as tmp:
        tmp_path = tmp.name

    with open(filepath, "rb") as src, open(tmp_path, "wb") as dst:
        subprocess.run(
            ["tr", tr_from, tr_to],
            stdin=src,
            stdout=dst,
            check=True
        )

    os.replace(tmp_path, filepath)


def process_directory(directory: str, tr_from: str, tr_to: str) -> None:
    for root, dirs, files in os.walk(directory):
        for filename in files:
            if filename.endswith(".txt"):
                filepath = os.path.join(root, filename)
                encode_file(filepath, tr_from, tr_to)


def main():
    parser = argparse.ArgumentParser(
        prog="solution.py",
        description="Encode all text files in DIRECTORY and its subdirectories using Caesar cipher.\n"
                    "Files are modified in-place. Only latin letters are shifted; other chars unchanged.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=(
            "Examples:\n"
            "  solution.py -d ./texts -k 3\n"
            "  solution.py --directory /home/user/docs --key 13"
        )
    )

    parser.add_argument("-d", "--directory", required=True, metavar="DIR", help="path to the target directory")
    parser.add_argument("-k", "--key", required=True, type=int, metavar="KEY", help="shift value (integer, e.g. 3)")

    args = parser.parse_args()

    if not os.path.isdir(args.directory):
        parser.error(f"Directory '{args.directory}' does not exist.")

    tr_from, tr_to = make_tr_args(args.key)
    process_directory(args.directory, tr_from, tr_to)

    print(f"Done. All .txt files in '{args.directory}' have been encoded with key={args.key}.")


if __name__ == "__main__":
    main()
