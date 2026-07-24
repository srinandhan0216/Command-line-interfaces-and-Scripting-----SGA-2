# Question 3 - Low-Level File I/O Using System Calls

## What this program does
This C program builds a simple employee record file using raw Linux system
calls (open, read, write, lseek, close) instead of standard library
functions like fopen/fprintf/fscanf. It creates a file, writes 4 fixed-size
employee records, updates one specific record's salary without touching
any other record, and retrieves records directly by position.

## How open(), read(), write(), lseek(), and close() contribute
- open(): Creates the file (O_CREAT) and controls access mode (read-only,
  write-only, or read-write) and initial state (O_TRUNC to start fresh).
- write(): Writes raw bytes of the Employee struct directly into the file
  at the current file position.
- lseek(): Moves the file's internal cursor to an exact byte offset,
  calculated as record_number * sizeof(struct Employee). Because every
  record is a FIXED size, this always lands exactly on the start of the
  desired record.
- read(): Reads exactly one record's worth of bytes from the current
  position -- combined with lseek(), this allows retrieving any record
  directly without scanning through earlier records.
- close(): Releases the file descriptor back to the operating system after
  each operation, preventing resource leaks.

## Why this allows updating without rewriting the whole file
Since every employee record occupies the same fixed number of bytes,
lseek() can calculate the exact byte position of any record purely from
its record number. This means we can seek directly to record 2, overwrite
just those bytes with the new salary, and leave every other record
completely untouched -- unlike text-based approaches, which would require
rewriting the entire file to change one line.

## Observed Output
- 4 records were written successfully.
- Record 2 (Amit Verma) was updated from salary 38000.00 to 42000.00.
- After the update, record 0 (Rahul Sharma) remained exactly unchanged,
  confirming the update only affected the targeted record.
- The file size matched exactly 4 * sizeof(struct Employee) bytes,
  confirming fixed-size records.
