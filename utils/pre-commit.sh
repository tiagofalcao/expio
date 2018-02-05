#!/bin/bash

declare -A CHECKS

clang_run() {
	if ! clang-format "$file" | diff -q "$file" - >/dev/null ; then
		>&2 echo "${file} do not meet the expected code style using clang-format."
		return 1
	fi
	return 0
}

clang_check() {
	if clang-format --version &>/dev/null ; then
		CHECKS["C source, ASCII text"]="clang_run"
	else
		>&2 echo "clang-format is not installed."
	fi
}
clang_check;

shellcheck_run() {
	if ! shellcheck "$file" &>/dev/null ; then
		>&2 echo "${file} do not meet the expected code style using shellcheck."
		return 1
	fi
	return 0
}

shellcheck_check() {
	if shellcheck --version &>/dev/null ; then
		CHECKS["Bourne-Again shell script, ASCII text executable"]="shellcheck_run"
	else
		>&2 echo "shellcheck is not installed."
	fi
}
shellcheck_check;


if git rev-parse --verify HEAD >/dev/null 2>&1; then
	against=HEAD
else
	# Initial commit: diff against an empty tree object
	against=4b825dc642cb6eb9a060e54bf8d69288fbee4904
fi

# do the formatting
ret=0
for file in $(git diff-index --cached --name-only "$against"); do
	lint="${CHECKS["$(file -b "${file}")"]}"
	if [ -z "${lint}" ]; then
		>&2 echo "Warning: No lint found to ${file}"
	elif ! "${lint}" "${file}" ; then
		ret=$((ret+=1))
	fi
done

exit $ret
