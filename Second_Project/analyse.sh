#!/bin/bash
# This is a Bash script that automates memory usage analysis for a Linux process.

# -----------------------------
# STEP 1: CHECK FOR ARGUMENT
# -----------------------------

# Check if the user provided a PID (process ID) as the first argument
if [ -z "$1" ]; then
  # If no argument is given, print a usage message
  echo "Usage: $0 <PID>"
  # Exit the script with status code 1 (error)
  exit 1
fi

# -----------------------------
# STEP 2: ASSIGN PID VARIABLE
# -----------------------------

# Assign the first command-line argument to a variable named PID
PID=$1

# -----------------------------
# STEP 3: VALIDATE IF PROCESS EXISTS
# -----------------------------

# Check if a directory for the given PID exists in the /proc directory (i.e., if the process is running)
if [ ! -d "/proc/$PID" ]; then
  # If the directory doesn't exist, it means the process is not running or the PID is invalid
  echo "Process with PID $PID does not exist."
  # Exit the script
  exit 1
fi

# -----------------------------
# STEP 4: PRINT MEMORY INFO USING ps
# -----------------------------

# Print a header for the output
echo "Analyzing memory usage for PID: $PID"
echo

# Print the process's memory usage information using the ps command
# ps: reports a snapshot of the current processes
# -p $PID: filters the output to show only the process with that PID
# -o pid,vsz,rss,comm: specifies the output columns:
#   pid  = process ID
#   vsz  = virtual memory size (KB)
#   rss  = resident set size (physical memory used, in KB)
#   comm = command name (executable)
echo "[Process Info from ps]"
ps -p $PID -o pid,vsz,rss,comm
echo

# -----------------------------
# STEP 5: SHOW MEMORY MAP FROM /proc/<PID>/maps
# -----------------------------

# Display a header for the next output section
echo "[/proc/$PID/maps]"

# Display the memory mappings of the process by reading from /proc/<PID>/maps
# This file shows how memory is laid out for the process (segments, permissions, and mappings)
cat /proc/$PID/maps
echo

# -----------------------------
# STEP 6: SAVE MEMORY MAP TO A FILE
# -----------------------------

# Create a filename for saving the memory map, including the PID in the name
OUTPUT_FILE="maps_output_${PID}.txt"

# Write the contents of /proc/<PID>/maps to the file for later reference
cat /proc/$PID/maps > $OUTPUT_FILE

# Inform the user that the memory map has been saved
echo "Memory map has been saved to $OUTPUT_FILE"

# -----------------------------
# STEP 7: END OF SCRIPT
# -----------------------------

# Final message indicating the analysis is complete
echo
echo "Done."
