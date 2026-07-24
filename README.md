# Linux Command Line and Interface — Graded Lab Assignment (Modules 5-10)

This repository contains the complete solutions for the 5-question graded lab 
assignment covering shell scripting, process management, low-level file I/O, 
text processing pipelines, and the vi editor.

## Repository Structure

- **Question1/** — Shell script to detect duplicate student submissions, back 
  up unique files, generate a report, and log errors separately.
- **Question2/** — C program using fork(), waitpid(), and signals to create 
  and monitor child processes while preventing zombie processes.
- **Question3/** — C program using low-level Linux system calls (open, read, 
  write, lseek, close) to build a secure employee record file utility.
- **Question4/** — Command pipeline using tail, grep, and redirection to 
  monitor a log file in real time and extract error messages.
- **Question5/** — Evaluation of vi's recovery mechanisms (swap files, undo 
  history, registers, backups) after an unexpected system crash.

## Folder Contents (each Question folder)

Each folder contains:
- Source code / script (where applicable)
- `commands.txt` — every terminal command executed, in order
- Output files generated during execution
- `screenshots/` — screenshots of each command and its output
- `explanation.md` — conceptual explanation of the approach and observed results

## How to Run

Each question's folder is self-contained. Navigate into the folder and refer 
to `commands.txt` for the exact sequence of commands, and `explanation.md` 
for the reasoning behind each step.
