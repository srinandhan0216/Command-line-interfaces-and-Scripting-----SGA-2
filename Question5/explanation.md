# Question 5 - Evaluating vi Recovery Mechanisms After a Crash

## Scenario
A developer is editing a critical configuration file in vi when the system
crashes before the file is saved. This evaluates the recovery mechanisms
vi provides and identifies the most reliable strategy.

## Mechanisms Evaluated

### 1. Swap Files
vi creates a hidden swap file (e.g. .config.txt.swp) the moment you start
editing a file. This swap file is written to disk continuously as you type,
completely independent of whether you ever save. Because it exists on disk
before a crash happens, it is the primary mechanism that actually survives
a system crash.

### 2. Auto-Recovery (vi -r)
This is the recovery mechanism, not the storage mechanism -- it reads the
swap file left behind after a crash and reconstructs the unsaved buffer.
Running `vi filename` again also auto-detects a leftover swap file and
offers a Recover option directly.

### 3. Backup Files (~)
When `:set backup` is enabled, vi keeps a copy of the file as it existed
immediately before your last successful save (named filename~). This only
protects against a bad *save* overwriting good content -- it does nothing
for changes that were never saved in the first place, which is exactly
the situation in this scenario.

### 4. Undo History
Undo (u) lets you step back through changes made during the current
session. This history lives entirely in vi's process memory and is
completely lost the instant vi or the system crashes.

### 5. Registers
Named memory slots holding yanked/deleted text (e.g. the default register
filled by dd, retrieved with p). Like undo history, registers exist only
in memory during the session and do not survive a crash.

## Conclusion: Most Reliable Recovery Strategy
The swap file combined with vi's -r auto-recovery is the only mechanism
built specifically for this scenario -- unsaved work lost to a crash.
Backup files and undo/register history either require a prior save or
exist only in volatile memory, neither of which helps here. The
recommended recovery procedure is:
1. Reopen the file with `vi filename` (or `vi -r filename` directly).
2. If prompted with the swap-file warning, choose (R)ecover.
3. Verify the recovered content is correct.
4. Save immediately with :w.
5. Delete the now-unneeded swap file (rm -f .filename.swp).

## Observed Output
- The swap file (.config.txt.swp) was found after simulating a crash,
  confirming it exists on disk during editing.
- Recovery via vi -r successfully restored the unsaved line
  (max_retries=5) that was never saved before the simulated crash.
- The backup file (config.txt~) only appeared after an actual :w save,
  confirming it does not protect unsaved-at-crash-time content.
- Undo (u) and register paste (p) worked correctly within the live
  session but are irrelevant to crash recovery since they are memory-only.

