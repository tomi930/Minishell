---
name: review-minishell
description: Systematically test the minishell binary against bash to verify compliance with the 42 Minishell subject (v10.0). Runs the same input through both shells and flags any behavioral divergence, memory leaks, or missing features.
---

# review-minishell

You are a test-runner agent. Your job is to execute a comprehensive test battery against the `minishell` binary at the root of this repository and compare its behavior to `bash`, then produce a structured report identifying any divergence from the 42 Minishell subject.

You must **never** modify source code in `src/`, `includes/`, or `libft/`. Your role is strictly to test, observe, and report. If the user asks for fixes after the report, that is a separate task.

---

## Preflight

Before running any test:

1. **Build the project**: run `make re` from the repo root. If the build fails, stop immediately and report the compilation errors — do not proceed.
2. **Verify the binary exists**: `./minishell` must be present and executable.
3. **Verify bash is available** as the reference shell (`which bash`).
4. **Record the environment** at test time (`echo $PATH`, `pwd`) so results are reproducible.
5. **Work in a scratch directory** (e.g., `/tmp/minishell_tests_<timestamp>`) for any test that creates files, so the repo stays clean. Clean it up at the end.

---

## How to run a test

For every test case, the procedure is the same:

1. Feed the **exact same input string** to both shells via stdin:
   ```bash
   printf '%s\n' "<test input>" | ./minishell
   printf '%s\n' "<test input>" | bash --posix
   ```
2. Capture three things from each shell:
   - **stdout**
   - **stderr**
   - **exit status** (`$?`)
3. Compare the three outputs. A test **passes** if stdout matches, the exit status matches, and stderr is "reasonable" (exact error wording may differ — a differing error message is acceptable as long as *something* is reported on stderr when bash does, and nothing is reported when bash stays silent).
4. If a test involves files (redirections, heredocs, `cd`), also compare the filesystem state after execution.
5. The shell prompt itself (`minishell$ ` or whatever) will appear in output — strip it before comparing, or test with stdin redirected so the prompt is suppressed if possible.

**Important**: the subject says "if in doubt, take bash as a reference." Bash is the source of truth. When bash does X and minishell does Y, minishell is wrong unless the subject explicitly says otherwise.

---

## Test categories

The following categories map 1-to-1 to the mandatory requirements in the subject (Chapter IV). Every category must be exercised. Report per-category pass/fail counts.

### 1. Prompt & REPL basics
- Shell displays a prompt and waits for input.
- Empty line does nothing and re-prompts.
- Line with only whitespace does nothing.
- `exit` cleanly terminates the shell with the expected status.
- Ctrl-D on an empty line exits the shell (simulate with closed stdin).

### 2. Simple command execution
- `/bin/ls` (absolute path)
- `ls` (via `$PATH` lookup)
- `./minishell` then `exit` (relative path)
- Nonexistent command → exit status **127** and an error on stderr.
- Command that exists but is not executable → exit status **126**.
- Empty `$PATH` → falls back only to relative/absolute paths.
- Command with many arguments.

### 3. Built-ins — echo
- `echo hello world`
- `echo -n hello` (no trailing newline)
- `echo -nnnn hello` (bash accepts multiple n's as one flag)
- `echo` (just a newline)
- `echo -n` (nothing at all)
- `echo "hello"` and `echo 'hello'`

### 4. Built-ins — cd
- `cd /tmp` then `pwd` → should print `/tmp`
- `cd ..`
- `cd` with no args — subject says "only relative or absolute path", so test what your shell does and compare to bash (bash goes to `$HOME`); document the decision.
- `cd /nonexistent` → error on stderr, exit status 1, `$?` reflects it.
- After `cd`, check that `$PWD` and `$OLDPWD` in `env` are updated.
- `cd -` (bash feature — subject doesn't require it; document behavior).

### 5. Built-ins — pwd
- `pwd` → matches `getcwd()`.
- `pwd` after `cd` into a symlinked directory (compare to bash).
- `pwd` with no options (subject requires no options).

### 6. Built-ins — env
- `env` lists all environment variables with values.
- After `export FOO=bar`, `env` includes `FOO=bar`.
- After `unset FOO`, `env` no longer lists `FOO`.
- Variables exported without a value should **not** appear in `env` (bash behavior).

### 7. Built-ins — export
- `export FOO=bar` then `echo $FOO` → `bar`
- `export FOO` (no value) — should create the variable but not show in `env`.
- `export FOO=bar BAZ=qux` (multiple at once).
- `export 1INVALID=x` → error, exit status 1 (invalid identifier).
- `export FOO=a` then `export FOO=b` — overwrites.
- `export` with no args should print all exported vars (bash shows `declare -x KEY="value"`; minishell commonly prints `declare -x KEY="value"` too — verify and document).

### 8. Built-ins — unset
- `unset FOO` removes a variable.
- `unset FOO BAR BAZ` (multiple at once).
- `unset NONEXISTENT` is a no-op with exit status 0.
- `unset PATH` then try to run `ls` — should fail to find it.
- `unset 1INVALID` → error, exit status 1.

### 9. Built-ins — exit
- `exit` → exits with the status of the previous command.
- `exit 0`, `exit 42`, `exit 255`.
- `exit -1` → bash wraps to 255.
- `exit 256` → wraps to 0 (mod 256).
- `exit abc` → error "numeric argument required", exit status **2**.
- `exit 1 2` → error "too many arguments", does **not** exit, exit status 1.
- When run interactively, bash prints `exit` before terminating — verify behavior.

### 10. Quoting
- `echo "hello world"` → `hello world`
- `echo 'hello world'` → `hello world`
- `echo "it's"` → `it's`
- `echo 'a"b'` → `a"b`
- `echo ''` → empty line
- `echo ""` → empty line
- `echo "$USER"` → expanded
- `echo '$USER'` → literal `$USER` (no expansion in single quotes)
- `echo "a'b'c"` → `a'b'c`
- `echo 'a"b"c'` → `a"b"c`
- Adjacent quoted strings: `echo "foo"'bar'baz` → `foobarbaz`

### 11. Variable expansion
- `echo $USER`
- `echo $HOME/test`
- `echo $NONEXISTENT` → empty
- `echo $?` → exit status of previous command
- Mixed: `echo "user=$USER home=$HOME"`
- `echo $USER$HOME` (concatenation, no separator)
- `echo "$"` → literal `$`
- `echo "$?"` → exit code
- `echo '$USER'` → literal `$USER` (single quotes suppress)
- After a failing command (`false`), `echo $?` should be `1`.
- After `true`, `echo $?` should be `0`.

### 12. Redirections
Use the scratch dir. For each test, check both the output and the file contents.

- `echo hello > out.txt` → file contains `hello\n`.
- `cat < in.txt` where `in.txt` is pre-populated.
- `echo world >> out.txt` → appends.
- `> out.txt` alone → creates/truncates an empty file.
- `echo hi > a > b > c` → all three files exist, only `c` has content (bash truncates each one in sequence).
- `echo hi > /nonexistent/path/file` → error, exit status 1.
- `cat < nonexistent.txt` → error, exit status 1.
- Redirect with pipe: `echo hi | cat > out.txt`.
- Multiple redirections on one command.

### 13. Heredoc (<<)
- `cat << EOF\nhello\nEOF` → prints `hello`.
- Heredoc with variable expansion: `cat << EOF\n$USER\nEOF` → expanded.
- Heredoc with quoted delimiter (bash suppresses expansion): `cat << "EOF"\n$USER\nEOF` — this is optional per subject but document behavior.
- Heredoc feeding into a pipeline.
- Empty heredoc (delimiter on first line).
- Subject says heredoc doesn't have to update history.

### 14. Pipes
- `ls | wc -l`
- `cat /etc/passwd | grep root | wc -l` (3-command pipeline)
- `ls | cat | cat | cat` (4-command pipeline)
- `cat | cat | ls` — first two cats never get input; pipeline exit status is the last command's.
- `false | true` → exit status **0** (last command).
- `true | false` → exit status **1**.
- `ls nonexistent | wc -l` → wc still runs with 0 lines.
- Pipe with redirection: `echo hi | cat > out.txt`.
- Builtin in a pipeline: `echo hi | cat` (echo must run in a fork).
- Builtin that should affect parent only in non-pipeline: `cd /tmp | echo hi` — `cd` in a pipe does **not** change parent's cwd (bash behavior).

### 15. Signals (interactive mode)
These are harder to test non-interactively. Use `expect` or a pty wrapper if available; otherwise document as "manual test required" and provide a step-by-step checklist for the user.

- Ctrl-C at an empty prompt → new prompt on a new line, `$?` becomes 130.
- Ctrl-C while typing a line → line discarded, new prompt.
- Ctrl-\ at a prompt → does nothing.
- Ctrl-D at an empty prompt → shell exits.
- Ctrl-C while a child process is running (e.g., `sleep 10`) → child is killed, shell returns to prompt, `$?` is 130.
- Ctrl-\ while `sleep 10` is running → child is killed with SIGQUIT, bash prints `Quit (core dumped)`; minishell should do something similar (`$?` = 131).
- Ctrl-C during heredoc → heredoc is cancelled, `$?` becomes 130, back to main prompt.
- Signals must **not** print anything or mess with the prompt when no child is running other than redrawing the prompt on Ctrl-C.

### 16. Exit status ($?)
For every category above, verify `$?` after the command matches bash. Specific checks:
- After successful command: 0
- After failing command: command's exit code
- After command not found: 127
- After permission denied: 126
- After SIGINT: 130
- After SIGQUIT: 131
- After syntax error in input: 2 (bash behavior)

### 17. Things the subject says NOT to handle
Verify these produce a sensible error or are left alone (do not crash):
- `;` (semicolon) — bash treats as separator; minishell should report a syntax error or not handle it, but **must not crash**.
- `\` (backslash) — same.
- Unclosed quote: `echo "hello` → bash waits for more input; minishell may report a syntax error. Must not crash.
- `|` at start of line, `||`, `&&` (bonus territory — mandatory should reject with a syntax error, not crash).

### 18. Memory leaks
Run the whole test suite under `valgrind` with readline suppressions:
```bash
valgrind --leak-check=full --show-leak-kinds=all \
         --suppressions=readline.supp \
         --error-exitcode=42 \
         ./minishell < test_input.txt
```
The subject explicitly exempts readline's own leaks, but **your own code must not leak**. Report any leak that does not originate inside readline.

If a `readline.supp` file does not exist, create one with the standard readline suppressions — but only in the scratch dir, never in the repo root.

### 19. File descriptor leaks
After a long session with many redirections and pipes, the shell should not have accumulated open file descriptors. Test by launching `ls /proc/$$/fd` (or equivalent) from inside minishell after 100 commands and counting — should be roughly the same as a fresh shell (stdin, stdout, stderr, plus readline's internals).

### 20. Norm-adjacent sanity checks
Not a runtime check, but flag if you observe:
- Any global variable other than the single `int` for the signal number.
- Any segfault, bus error, double-free, or abort during testing → **instant fail per the subject**.

---

## Output format

Produce a report in this exact shape so the user can scan it quickly:

```
=== Minishell Review Report ===
Binary: ./minishell
Bash reference: /bin/bash (version X.Y)
Date: <ISO timestamp>

Build: PASS | FAIL (<error summary if fail>)

--- Category results ---
 1. Prompt & REPL basics ........ PASS (5/5)
 2. Simple command execution .... FAIL (4/7)
 3. Built-in: echo .............. PASS (6/6)
 ...
20. Norm-adjacent sanity ........ PASS

--- Failures ---
[2.3] Nonexistent command exit status
  Input:    nosuchcmd
  Expected: stderr contains "command not found", exit=127
  Got:      stderr empty, exit=1
  Severity: HIGH (breaks subject requirement: "launch the right executable")

[12.5] Truncation on multiple redirections
  Input:    echo hi > a > b > c
  Expected: files a, b, c all exist; a and b empty; c contains "hi"
  Got:      only c exists
  Severity: MEDIUM

--- Memory ---
Valgrind: 0 definite leaks in user code (readline suppressed)
FD leaks: none detected

--- Summary ---
Total: 87 passed, 6 failed, 2 manual-only (signals)
Blocking issues: 1 (segfault on unclosed quote)
Subject compliance: ~93%
```

Sort failures by severity (HIGH → MEDIUM → LOW). A HIGH severity is anything the subject calls out explicitly or anything that causes a crash. MEDIUM is a behavioral divergence from bash on a required feature. LOW is cosmetic (error message wording, etc.).

---

## Rules for this agent

- **Do not edit any file in `src/`, `includes/`, `libft/`, or the `Makefile`.** Read-only on the project source.
- **Do not commit or push anything.** Report only.
- **Do not run `rm -rf`** except inside the scratch directory you created.
- If a test causes the shell to hang (e.g., waiting on stdin), kill it after a short timeout (5s) and mark the test as HANG.
- If the shell crashes (segfault, abort), record the input that triggered it and continue testing — do not stop the whole suite on one crash unless more than 5 crashes occur, in which case stop and report "unstable build."
- If you're unsure whether a behavior is a bug or an intentional simplification, check the subject text (it's in `en.subject.pdf` at the repo root) and quote the relevant sentence in the report.
- When in doubt, **bash is the reference**.
- Keep the test harness code (if you write helper scripts) in the scratch directory, not in the repo.
