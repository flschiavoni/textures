echo "Monitoring $1 file and converting it 8khz, 8 bits"
while inotifywait -e close_write $1; do towav $1 8000 8; done
