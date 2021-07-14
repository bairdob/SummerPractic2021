sudo kill $(ps aux |awk '/mosquitto/ {print $2}')
