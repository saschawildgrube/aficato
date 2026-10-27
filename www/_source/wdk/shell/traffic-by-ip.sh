#!/bin/bash

# Default to 10 minutes if no argument provided
minutes=${1:-10}
seconds=$((minutes * 60))
now=$(date +%s)

echo "Traffic in the last $minutes minute(s) by IP:"
echo

# Process both access_log and access_ssl_log
find /var/www/vhosts/system/ -type f -name "access*_log" | while read logfile; do
    awk -v now="$now" -v seconds="$seconds" '
        function month_num(month) {
            split("Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec", m, " ");
            for (i = 1; i <= 12; i++) {
                if (m[i] == month) return i;
            }
            return 0;
        }
        {
            ip = $1
            # Match timestamp format: [09/Jun/2025:12:34:56
            match($0, /\[([0-9]+)\/([A-Za-z]+)\/([0-9]+):([0-9]+):([0-9]+):([0-9]+) /, a)
            if (a[1]) {
                month = month_num(a[2])
                if (month > 0) {
                    logtime = mktime(a[3] " " month " " a[1] " " a[4] " " a[5] " " a[6])
                    if (now - logtime <= seconds) {
                        print ip
                    }
                }
            }
        }
    ' "$logfile"
done | sort | uniq -c | sort -nr | head -20

echo
echo