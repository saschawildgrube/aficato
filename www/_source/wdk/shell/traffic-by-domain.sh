#!/bin/bash

minutes=${1:-10}
seconds=$((minutes * 60))

echo "Traffic in the last $minutes minute(s) by domain:"
echo

now=$(date +%s)

for domain in /var/www/vhosts/system/*; do
    domain_name=$(basename "$domain")
    log_dir="$domain/logs"
    access_log="$log_dir/access_log"
    access_ssl_log="$log_dir/access_ssl_log"

    count=0

    for logfile in "$access_log" "$access_ssl_log"; do
        if [[ -f "$logfile" ]]; then
            hits=$(awk -v now="$now" -v seconds="$seconds" '
                function month_num(month) {
                    split("Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec", m, " ");
                    for (i = 1; i <= 12; i++) {
                        if (m[i] == month) return i;
                    }
                    return 0;
                }
                {
                    match($0, /\[([0-9]+)\/([A-Za-z]+)\/([0-9]+):([0-9]+):([0-9]+):([0-9]+) /, a);
                    if (a[1]) {
                        month = month_num(a[2]);
                        if (month > 0) {
                            logtime = mktime(a[3] " " month " " a[1] " " a[4] " " a[5] " " a[6]);
                            if (now - logtime <= seconds) count++;
                        }
                    }
                }
                END { print count }
            ' "$logfile")
            count=$((count + hits))
        fi
    done

    printf "%10d %s\n" "$count" "$domain_name"
done | sort -nr

echo
echo