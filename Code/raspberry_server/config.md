```bash
sudo apt update && sudo apt upgrade
curl -s https://assets.emqx.com/scripts/install-emqx-deb.sh | sudo bash
sudo apt install emqx
sudo systemctl enable emqx # Was enabled by default
sudo systemctl start emqx
```

Connected to rpi on port 18083

For the wifi on RPI:
```bash
sudo nmcli device wifi hotspot con-name CONNNAME ssid SSID band bg password PASSWORD
```