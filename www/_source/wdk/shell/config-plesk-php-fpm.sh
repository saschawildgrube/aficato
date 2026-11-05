#!/usr/bin/env bash
# Plesk Obsidian for Linux. Requires Bash, root, Python 3.5+ and Plesk XML API access.
# Replaces each target site's ENTIRE Additional configuration directives field.
# Does not change PHP versions/handlers, service plans, or standard PHP settings.
# Creates no backups, temporary files, RAM files, or filesystem work files.
# Non-interactive: creates a loopback-only API key as root and revokes it on exit.
# Optional: PLESK_PANEL_PORT (default 8443). No URL or credentials required.
# Restarts each configured Plesk PHP-FPM service once after all targets are verified.
# Usage: config-plesk-php-fpm [--all | WEBSPACE [WEBSPACE ...]]
# No arguments: preview all. --all or webspace names: apply immediately.
set -Eeuo pipefail
export LC_ALL=C

mode=preview
scope=all
declare -A requested=()
declare -A found=()

if (( $# > 0 )); then
    mode=apply

    if [[ $1 == --all ]]; then
        if (( $# != 1 )); then
            echo "Use --all alone, or supply one or more webspace names." >&2
            exit 2
        fi
    else
        scope=named

        for name in "$@"; do
            if [[ ! $name =~ ^[A-Za-z0-9_][A-Za-z0-9_.-]*$ ]]; then
                echo "Invalid webspace or unsupported option: $name" >&2
                echo "Usage: $0 [--all | WEBSPACE [WEBSPACE ...]]" >&2
                exit 2
            fi

            requested[${name,,}]=1
        done
    fi
fi

(( EUID == 0 )) || {
    echo 'Run as root (sudo).' >&2
    exit 1
}

command -v plesk >/dev/null || {
    echo 'plesk command not found.' >&2
    exit 1
}

# Keep configuration in memory; {DOCROOT} remains literal for Plesk.
config='error_log = {DOCROOT}/../../{LOGDIR}/error.log

[php-fpm-pool-settings]
pm = dynamic
pm.max_children = 30
pm.start_servers = 2
pm.min_spare_servers = 1
pm.max_spare_servers = 3
pm.max_requests = 500

pm.status_path = /fpm-status

request_slowlog_timeout = 5s
slowlog = /var/log/php-fpm-slow.log'

# Read-only inventory. Includes main domains, additional domains and subdomains.
# Domains without physical hosting have no hosting row; aliases have no pool.
inventory=$(plesk db -N -B -e '
SELECT d.name, h.php, h.php_handler_id
FROM domains AS d
JOIN hosting AS h ON h.dom_id = d.id
ORDER BY d.name
')

declare -a targets=()
declare -a services=()
declare -A seen_services=()

while [[ -n $inventory ]]; do
    row=${inventory%%$'\n'*}

    if [[ $inventory == *$'\n'* ]]; then
        inventory=${inventory#*$'\n'}
    else
        inventory=''
    fi

    domain=${row%%$'\t'*}
    fields=${row#*$'\t'}
    php=${fields%%$'\t'*}
    handler=${fields#*$'\t'}

    [[ -n $domain ]] || continue

    [[ $domain =~ ^[A-Za-z0-9_][A-Za-z0-9_.-]*$ ]] || {
        echo "Unexpected domain name: $domain; aborting." >&2
        exit 1
    }

    if [[ $scope == named ]]; then
        [[ ${requested[${domain,,}]:-} == 1 ]] || continue
        found[${domain,,}]=1

        if [[ $php != true || $handler != *fpm* ]]; then
            echo "Selected webspace does not use enabled PHP-FPM: $domain" >&2
            exit 1
        fi
    fi

    if [[ $php == true && $handler == *fpm* ]]; then
        # Plesk-provided FPM handler IDs are also their service names.
        if [[ ! $handler =~ ^plesk-php[0-9]+-fpm(-dedicated)?$ ]]; then
            echo "Cannot resolve the FPM service for $domain (handler=$handler); aborting." >&2
            exit 1
        fi

        if [[ ${seen_services[$handler]:-} != 1 ]]; then
            services+=("$handler")
            seen_services[$handler]=1
        fi

        targets+=("$domain")
        printf 'TARGET %s (%s)\n' "$domain" "$handler"
    else
        printf 'SKIP   %s (PHP=%s, handler=%s)\n' "$domain" "$php" "$handler"
    fi
done

# Validate every requested name before any API key, write, or restart.
if [[ $scope == named ]]; then
    for name in "${!requested[@]}"; do
        if [[ ${found[$name]:-} != 1 ]]; then
            echo "Webspace not found with physical hosting: $name" >&2
            exit 1
        fi
    done
fi

printf '\nTargets: %s\n' "${#targets[@]}"

if (( ${#services[@]} > 0 )); then
    printf 'FPM service: %s\n' "${services[@]}"
fi

if [[ $mode == preview ]]; then
    echo 'Preview only. No settings or services were changed.'
    echo 'Use --all or specify webspace name(s) to apply these settings.'
    for domain in "${targets[@]}"; do
        log_dir=logs

        case ${domain,,} in
            dev.*) log_dir=logs-dev ;;
            test.*) log_dir=logs-test ;;
        esac

        printf '\nCONFIG %s\n%s\n' "$domain" "${config//\{LOGDIR\}/$log_dir}"
    done
    exit 0
fi

(( ${#targets[@]} > 0 )) || exit 0

command -v python3 >/dev/null || {
    echo 'Python 3.5+ is required.' >&2
    exit 1
}

# Validate every service before any API key or configuration write.
for service in "${services[@]}"; do
    load_state=$(systemctl show --property=LoadState --value "$service")

    if [[ $load_state != loaded ]]; then
        echo "FPM service is not loaded: $service (LoadState=$load_state); aborting." >&2
        exit 1
    fi
done

# Use the documented inline API property instead of the CLI file argument.
exec python3 -B -c '
import re
import signal
import subprocess
import uuid
import os
import ssl
import sys
import urllib.error
import urllib.request
import xml.etree.ElementTree as ET

config = sys.argv[1]
service_count = int(sys.argv[2])
services = sys.argv[3:3 + service_count]
domains = sys.argv[3 + service_count:]
updated = 0
current = None
write_attempted = False
api_key = None
exit_code = 0
key_description = "config-php-fpm-" + str(uuid.uuid4())


def interrupted(signum, frame):
    raise KeyboardInterrupt("Received signal {}".format(signum))


signal.signal(signal.SIGTERM, interrupted)
signal.signal(signal.SIGHUP, interrupted)


class NoRedirect(urllib.request.HTTPRedirectHandler):
    def redirect_request(self, req, fp, code, msg, headers, newurl):
        raise RuntimeError(
            "Local panel redirected the API request; stopping without following it."
        )


def config_for_domain(domain):
    log_dir = "logs"

    if domain.lower().startswith("dev."):
        log_dir = "logs-dev"
    elif domain.lower().startswith("test."):
        log_dir = "logs-test"

    return config.replace("{LOGDIR}", log_dir)


def packet(domain, operation):
    root = ET.Element("packet")
    node = ET.SubElement(ET.SubElement(root, "site"), operation)
    ET.SubElement(ET.SubElement(node, "filter"), "name").text = domain

    if operation == "get":
        ET.SubElement(ET.SubElement(node, "dataset"), "hosting")
    else:
        host = ET.SubElement(ET.SubElement(node, "values"), "hosting")
        prop = ET.SubElement(ET.SubElement(host, "vrt_hst"), "property")
        ET.SubElement(prop, "name").text = "additional-directives"
        ET.SubElement(prop, "value").text = config_for_domain(domain)

    return ET.tostring(root, encoding="utf-8")


def call(domain, operation):
    request = urllib.request.Request(
        endpoint,
        data=packet(domain, operation),
        headers={"Content-Type": "text/xml", "KEY": api_key}
    )

    with opener.open(request, timeout=180) as response:
        root = ET.fromstring(response.read())

    errors = root.findall(".//errtext")

    if errors:
        raise RuntimeError(
            "Plesk API: " + "; ".join(e.text or "Unknown error" for e in errors)
        )

    results = root.findall("./site/" + operation + "/result")

    if len(results) != 1 or results[0].findtext("status") != "ok":
        raise RuntimeError(
            "API did not confirm exactly one successful site result."
        )

    return results[0]


def read_directives(domain):
    result = subprocess.run(
        ["plesk", "bin", "site", "--show-php-settings", domain],
        stdin=subprocess.DEVNULL,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        universal_newlines=True
    )

    if result.returncode != 0 or re.search(
        r"warning|fatal|error|failed|exception", result.stderr, re.I
    ):
        raise RuntimeError(
            "Could not read PHP settings through the CLI for " + domain
        )

    # This is the actual CLI format confirmed on the target server.
    heading = re.search(
        r"(?m)^Additional directives:[ \t]*\r?$", result.stdout
    )

    if heading is None:
        raise RuntimeError(
            "CLI output lacks the Additional directives heading for " + domain
        )

    return result.stdout[heading.end():].lstrip("\r\n")


def directive_values(text):
    # The UI preserves INI sections, but --show-php-settings flattens them.
    # Compare directive names/values, ignoring section headings and formatting.
    values = {}

    for line in text.splitlines():
        line = line.strip()

        if not line or line.startswith((";", "#")):
            continue

        if line.startswith("[") and line.endswith("]"):
            continue

        if "=" not in line:
            raise RuntimeError(
                "Unrecognized line in Additional directives readback."
            )

        name, value = line.split("=", 1)
        name, value = name.strip(), value.strip()

        if not name or name in values:
            raise RuntimeError(
                "Empty or duplicate directive name in readback: " + name
            )

        values[name] = value

    return values


try:
    port = int(os.environ.get("PLESK_PANEL_PORT", "8443"))

    if not 1 <= port <= 65535:
        raise RuntimeError("PLESK_PANEL_PORT must be between 1 and 65535.")

    endpoint = (
        "https://127.0.0.1:{}/enterprise/control/agent.php".format(port)
    )

    # The connection is fixed to loopback, bypasses proxies and rejects redirects.
    # The panel certificate normally names its public hostname, not 127.0.0.1.
    context = ssl.create_default_context()
    context.check_hostname = False
    context.verify_mode = ssl.CERT_NONE

    opener = urllib.request.build_opener(
        urllib.request.ProxyHandler({}),
        urllib.request.HTTPSHandler(context=context),
        NoRedirect()
    )

    # Block termination briefly so a key cannot be created between receiving
    # its value and registering it for cleanup. No credential is printed.
    old_mask = signal.pthread_sigmask(
        signal.SIG_BLOCK,
        {signal.SIGINT, signal.SIGTERM, signal.SIGHUP}
    )

    try:
        created = subprocess.run(
            [
                "plesk", "bin", "secret_key", "--create",
                "-ip-address", "127.0.0.1",
                "-description", key_description
            ],
            stdin=subprocess.DEVNULL,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            universal_newlines=True
        )

        matches = re.findall(
            r"(?m)^\s*([0-9a-fA-F]{8}-[0-9a-fA-F]{4}-"
            r"[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}|"
            r"[0-9a-fA-F]{32})\s*$",
            created.stdout
        )

        if matches:
            api_key = matches[0]

        if created.returncode != 0 or not api_key:
            raise RuntimeError(
                "Could not create or identify the local API key. Key label: "
                + key_description
            )
    finally:
        signal.pthread_sigmask(signal.SIG_SETMASK, old_mask)

    # Read-only authentication and readback-capability checks before any write.
    for domain in domains:
        call(domain, "get")
        read_directives(domain)

    print(
        "API preflight passed for {} sites.".format(len(domains)),
        flush=True
    )

    for domain in domains:
        expected_values = directive_values(config_for_domain(domain))
        current = domain
        write_attempted = True
        print("Applying: " + domain, flush=True)

        call(domain, "set")

        actual = read_directives(domain)
        actual_values = directive_values(actual)

        if actual_values != expected_values:
            missing = sorted(set(expected_values) - set(actual_values))
            different = sorted(
                name for name in expected_values
                if name in actual_values
                and expected_values[name] != actual_values[name]
            )
            extra = sorted(set(actual_values) - set(expected_values))

            raise RuntimeError(
                "Additional directives mismatch: "
                "missing={}, different={}, extra={}".format(
                    missing, different, extra
                )
            )

        updated += 1
        write_attempted = False
        print("Verified: " + domain, flush=True)

except (Exception, KeyboardInterrupt) as exc:
    print("Stopped: {}".format(exc), file=sys.stderr)
    print("Verified updates: {}".format(updated), file=sys.stderr)

    if write_attempted:
        print(
            "Inspect {}: its update was attempted but not verified. "
            "No automatic retry.".format(current),
            file=sys.stderr
        )
    else:
        print("No unverified update was attempted.", file=sys.stderr)

    exit_code = 1

finally:
    if api_key:
        # Cleanup runs on success, errors, Ctrl+C, SIGHUP and SIGTERM.
        # SIGKILL or power loss cannot run cleanup.
        cleanup_mask = signal.pthread_sigmask(
            signal.SIG_BLOCK,
            {signal.SIGINT, signal.SIGTERM, signal.SIGHUP}
        )

        try:
            removed = subprocess.run(
                [
                    "plesk", "bin", "secret_key",
                    "--delete", "-key", api_key
                ],
                stdin=subprocess.DEVNULL,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                universal_newlines=True
            )

            diagnostics = removed.stdout + removed.stderr

            if removed.returncode != 0 or re.search(
                r"warning|fatal|error|failed|exception", diagnostics, re.I
            ):
                raise RuntimeError("Key removal did not succeed")

        except Exception:
            print(
                "API key cleanup failed. Remove the key labelled {} "
                "in Plesk.".format(key_description),
                file=sys.stderr
            )
            exit_code = 1

        finally:
            signal.pthread_sigmask(signal.SIG_SETMASK, cleanup_mask)

# Restart only after every target was verified and API-key cleanup succeeded.
if exit_code == 0 and updated == len(domains) and updated > 0:
    try:
        print("Restarting {} ...".format(", ".join(services)), flush=True)

        subprocess.run(
            ["systemctl", "restart"] + services,
            stdin=subprocess.DEVNULL,
            check=True
        )

        for service in services:
            subprocess.run(
                ["systemctl", "is-active", "--quiet", service],
                stdin=subprocess.DEVNULL,
                check=True
            )

    except (Exception, KeyboardInterrupt) as exc:
        print(
            "Settings were saved, but the FPM restart/health check "
            "failed: {}".format(exc),
            file=sys.stderr
        )
        exit_code = 1

if exit_code == 0:
    print(
        "Done: {} sites updated and verified; {} restarted "
        "and active; local API key revoked.".format(updated, ", ".join(services))
    )

sys.exit(exit_code)
' "$config" "${#services[@]}" "${services[@]}" "${targets[@]}"
