```bash
sudo apt update && sudo apt upgrade
curl -s https://assets.emqx.com/scripts/install-emqx-deb.sh | sudo bash
sudo apt install emqx
sudo systemctl enable emqx # Was enabled by default
sudo systemctl start emqx
```

Connected to rpi on port 18083

Installed docker using [tutorial](https://docs.docker.com/engine/install/raspberry-pi-os/#install-from-a-package)
Grafana [tutorial](https://grafana.com/tutorials/install-grafana-on-raspberry-pi/)
```bash
#sudo systemctl start docker
#sudo systemctl enable docker
#docker run -d --name=grafana -p 3001:3000 -v grafana_config:/etc/grafana -v grafana_data:/var/lib/grafana -v grafana_logs:/var/log/grafana grafana/grafana
#docker run -d --name mysqldb -p 3306:3306 -v db_data:/var/lib/mysql -e MYSQL_ROOT_PASSWORD=password mysql:latest

sudo apt install mariadb-server
sudo mysql_secure_installation
#docker exec -it mysqldb bash
mysql -u root -p
```

```sql
CREATE DATABASE IOT;
CREATE TABLE IOT.Humidity (
    ID INT AUTO_INCREMENT PRIMARY KEY,
    Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    SensorValue FLOAT
);

CREATE TABLE IOT.Luminosity (
    ID INT AUTO_INCREMENT PRIMARY KEY,
    Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    SensorValue FLOAT
);

CREATE TABLE IOT.Pressure (
    ID INT AUTO_INCREMENT PRIMARY KEY,
    Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    SensorValue FLOAT
);

CREATE TABLE IOT.Temperature (
    ID INT AUTO_INCREMENT PRIMARY KEY,
    Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    SensorValue FLOAT
);
```

/etc/systemd/system/mqtt2mariadb.service
```ini
[Unit]
Description=Python script for mqtt data saving
After=mariadb.service

[Service]
ExecStart=/home/mati/env/bin/python3 /home/mati/script.py

[Install]
WantedBy=multi-user.target
```

/etc/systemd/system/ngrok.service
```ini
[Unit]
Description=Ngrok remote access

[Service]
ExecStart=ngrok http --url=steadily-alert-minnow.ngrok-free.app 3000
User=mati

[Install]
WantedBy=multi-user.target
```
