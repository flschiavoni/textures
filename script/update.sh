while inotifywait -e close_write $1; do towav $1 10025 8; done
