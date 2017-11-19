echo "Playing $1 file format Unsigned 8bits 8khz Sample Rate"
echo "Press Ctrl+Z to stop playing"
while [ true ] ; do aplay --format=U8 --rate=8000 -q $1 ; done
