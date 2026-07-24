# Question 4 - Real-Time Log Monitoring Pipeline

## What this pipeline does
This solution continuously monitors a log file (app.log) as new entries are
appended, extracts only ERROR messages in real time, and stores them in a
separate report file (error_report.txt), while keeping the terminal free
of irrelevant INFO/WARNING clutter.

## Command pipeline breakdown
tail -f app.log | grep --line-buffered "ERROR" | tee -a error_report.txt

- tail -f: Follows the log file continuously, printing new lines the
  instant they're written -- this is what makes monitoring "real time"
  instead of a one-time snapshot.
- grep --line-buffered "ERROR": Filters the live stream down to only lines
  containing ERROR. The --line-buffered flag is essential in a pipeline
  like this because grep normally buffers output when writing to a pipe
  (not a terminal), which would delay real-time results; line-buffering
  forces immediate output per line.
- tee -a error_report.txt: Displays matching lines on-screen while also
  appending them to error_report.txt, giving a persistent record of every
  error without losing the ability to watch it live.

## How /dev/null improves efficiency
/dev/null is a special file that discards anything written to it.
- Redirecting stdout (`> /dev/null`) lets a script check only the exit
  status of a command (success/failure) without cluttering output with
  data it doesn't need to display.
- Redirecting stderr (`2>/dev/null`) suppresses irrelevant error/warning
  messages from a command (like tail complaining about a temporarily
  missing file) so only meaningful data reaches the next stage of the
  pipeline or the screen.

## Observed Output
- error_report.txt correctly captured only the 3 ERROR lines out of the
  9 total log lines generated (INFO and WARNING lines were correctly
  excluded).
- The pipeline displayed each ERROR line in real time as generate_logs.sh
  appended it, roughly every 2 seconds, confirming true real-time behavior
  rather than a delayed batch read.
