#!/bin/bash
# Helper script: simulates a live application writing log entries.
# Not part of the graded solution -- just used to generate test data
# so we can demonstrate real-time log monitoring.

LOG_FILE="app.log"

messages=(
    "INFO: User login successful"
    "INFO: Page loaded in 200ms"
    "ERROR: Database connection failed"
    "INFO: Cache refreshed"
    "ERROR: Null pointer exception in module X"
    "WARNING: High memory usage detected"
    "INFO: Request processed successfully"
    "ERROR: Timeout while connecting to payment gateway"
    "INFO: User logged out"
)

for msg in "${messages[@]}"; do
    echo "$(date '+%Y-%m-%d %H:%M:%S') $msg" >> "$LOG_FILE"
    sleep 2
done
