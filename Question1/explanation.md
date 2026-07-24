# Question 1 - Duplicate Detection and Backup Script

## What this script does
This script scans all files inside the `submissions/` folder, calculates an MD5 
checksum for each file's content, and uses that to detect duplicate submissions 
(files with identical content, regardless of filename). It backs up only the 
unique files into `backup/`, records a summary in `report.txt`, and sends any 
error messages to `error.log` separately.

## Command-by-command explanation
- `mkdir -p backup`: Creates the backup folder if it doesn't already exist, and its 2>> redirection captures any error silently into error.log.
- `md5sum submissions/*`: Generates a checksum fingerprint per file's content, used to detect duplicates even when filenames differ.
- `awk '{print $1}' | sort | uniq -d`: Extracts just the checksums, sorts them, then finds which ones repeat -- these are the duplicates.
- `awk '!seen[$1]++ {print $2}'`: Keeps only the first file seen for each unique checksum, giving us the list of unique files to back up.
- `cp "$filepath" "$BACKUP_DIR"/`: Copies each unique file into the backup folder.
- Redirection operators (`>`, `>>`, `2>>`): `>` clears old data at the start of each run, `>>` appends new data (like each new md5sum line), and `2>>` sends only error messages to error.log so they never mix with normal output.

## Observed Output
- Total files processed: 5
- Duplicate files found: 2 (rahul_asg1.py/rahul_copy.py, and priya_asg1.py/john_asg1.py, confirmed identical via matching MD5 checksums in checksums.txt)
- Unique files backed up: 3 (amit_asg1.py, john_asg1.py, rahul_asg1.py)
- error.log remained empty, confirming no errors occurred during a normal run and that error redirection is correctly isolated from the report.
