#!/bin/sh
#
# Simple fake-hwclock script

FILE=/etc/fake-hwclock.data

case "$1" in
  start)
    if [ -f "$FILE" ]; then
      echo "Loading time from $FILE"
      date -u -s "$(cat $FILE)"
    else
      echo "No timestamp found"
    fi
    ;;
  stop)
    echo "Saving time to $FILE"
    date -u +%Y-%m-%dT%H:%M:%S > "$FILE"
    ;;
  *)
    echo "Usage: $0 {start|stop}"
    exit 1
esac

exit 0
