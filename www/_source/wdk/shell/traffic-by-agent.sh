#!/bin/bash

# Default to 10 minutes if no argument provided
minutes=${1:-10}
seconds=$((minutes * 60))
now=$(date +%s)

echo "Traffic in the last $minutes minute(s) by User Agent:"
echo

find /var/www/vhosts/system/ -type f -name "access*_log" | while read logfile; do
    awk -v now="$now" -v seconds="$seconds" '
        function month_num(month) {
            split("Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec", m, " ");
            for (i = 1; i <= 12; i++) if (m[i] == month) return i;
            return 0;
        }
        {
            match($0, /\[([0-9]+)\/([A-Za-z]+)\/([0-9]+):([0-9]+):([0-9]+):([0-9]+) /, t)
            if (t[1]) {
                month = month_num(t[2])
                if (month) {
                    logtime = mktime(t[3] " " month " " t[1] " " t[4] " " t[5] " " t[6])
                    if (now - logtime <= seconds) {
                        n = split($0, q, /\"/)
                        if (n >= 6) {
                            user_agent = q[6]
                            if (user_agent == "") user_agent = "-"
                            print user_agent
                        }
                    }
                }
            }
        }
    ' "$logfile"
done | sort | uniq -c | sort -nr | head -20

echo
echo