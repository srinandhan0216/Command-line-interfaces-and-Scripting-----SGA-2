# Question 2 - Process Creation, Monitoring, and Zombie Prevention

## What this program does
This C program forks 4 child processes, each simulating work by sleeping for
a set duration. The parent monitors all children using waitpid() with WNOHANG
(non-blocking), so it can check on multiple children without freezing. If a
child exceeds a 5-second timeout, the parent sends SIGTERM to terminate it.
Every child, whether it exits normally or is killed, is immediately reaped
via waitpid(), which prevents it from becoming a zombie process.

## How process creation, waiting, and signal handling work together
- fork() creates child processes that run independently and concurrently
  with the parent.
- waitpid() with WNOHANG lets the parent poll each child's status without
  blocking, which is essential when managing multiple children at once.
- kill() with SIGTERM lets the parent terminate a misbehaving/unresponsive
  child gracefully.
- Calling waitpid() again right after kill() collects the child's exit
  status, which is what actually removes its entry from the kernel's
  process table -- without this the terminated child would remain a
  zombie indefinitely.

## Observed Output
- Children 0, 1, and 2 completed normally within their sleep durations
  (2s, 4s, 6s respectively).
- Child 3 exceeded the 5-second timeout and was terminated by SIGTERM
  (signal 15).
- `ps aux | grep defunct` showed no zombie processes at any point during
  execution, confirming the reaping logic worked correctly.
