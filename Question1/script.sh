#!/bin/bash
# ===============================================
# Script Name: process_submissions.sh
# Purpose: Detect duplicate submissions, backup 
#          unique files, and generate a report.
# ===============================================

SUBMISSIONS_DIR="submissions"
BACKUP_DIR="backup"
REPORT_FILE="report.txt"
ERROR_LOG="error.log"

mkdir -p "$BACKUP_DIR" 2>> "$ERROR_LOG"

CHECKSUM_FILE="checksums.txt"

> "$CHECKSUM_FILE"
> "$REPORT_FILE"
> "$ERROR_LOG"

for file in "$SUBMISSIONS_DIR"/*; do
    md5sum "$file" >> "$CHECKSUM_FILE" 2>> "$ERROR_LOG"
done

TOTAL_FILES=$(wc -l < "$CHECKSUM_FILE")

DUPLICATE_COUNT=$(awk '{print $1}' "$CHECKSUM_FILE" | sort | uniq -d | wc -l)

awk '!seen[$1]++ {print $2}' "$CHECKSUM_FILE" > unique_files.txt

BACKUP_COUNT=0
while IFS= read -r filepath; do
    cp "$filepath" "$BACKUP_DIR"/ 2>> "$ERROR_LOG"
    if [ $? -eq 0 ]; then
        BACKUP_COUNT=$((BACKUP_COUNT + 1))
    fi
done < unique_files.txt

{
    echo "===== Submission Processing Report ====="
    echo "Date: $(date)"
    echo "Total files processed : $TOTAL_FILES"
    echo "Duplicate files found : $DUPLICATE_COUNT"
    echo "Unique files backed up: $BACKUP_COUNT"
    echo "=========================================="
} >> "$REPORT_FILE"

echo "Processing complete. Check $REPORT_FILE for summary."
