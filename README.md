# youminds-sportticker


This ticker app retrieves scoccer data from https://www.football-data.org and displays scores and standings on a matrix LED display.

It is based upon a Raspberry Pi. I used a Revision 2 Model B with only one USB and a Edimax WLAN module. You may as well use other Raspberry Pi hardware.

The display is a 32x8 LED BTF-LIGHTING WS2812BECO from Amazon.

The application uses the following third party software:

- Raspberry Pi OS Lite 32 Bit from https://www.raspberrypi.com/software/
- JQ a JSON parser
- Lighttpd web server
- rpi-ws2812-server display driver service

Please make shure to read and accept the licence conditions of each third party software.


## Hardware setup

Todo..

## Software installation

### Create a boot SD-Card with Raspberry Pi OS Lite 32 Bit from https://www.raspberrypi.com/software/
### Connect a monitor, a dual USB hub and plug the keyboard and the edimax WLAN stick to the Raspberry Pi
### Insert the SD-Card
### Power the Raspberry Pi
### Wait for the end of the setup process of the Raspberry Pi OS Lite 32
### When asked enter exactly "pi" as user
### When asked enter a passwort of your choice
### Enter "sudo raspi-config"
### Activate WLAN
### Activate SSH
### Expand the filesystem
### Set the hostname to your choice or use "sportticker"
### Enter sudo reboot
### Check your router if Raspberry connected the WLAN
### Open a terminal window on windows or iOS
### Enter "ssh pi@sportticker" or "ssh pi@<THE WLAN IP>"
### Enter your passwort
### You may now switch off the Raspberry, disconnect the keyboard and the monitor, leave the WLAN stick and switch the raspberry on again.
### Log in again via terminal window and SSH
### Install the rpi-ws2812-server
### Go to https://github.com/tom-2015/rpi-ws2812-server and follow the instructions
### Do not install the service
### Make shure the LED Matrix data in is connected with pin 18
### Test the display
### Go to the rpi-ws2812-server directory
### Start the display driver with sudo ./ws2812svr starten
### Enter 
setup 1,10,3 
⁃	init 
⁃	fill 1,006600 
⁃	render
### You should the the first 10 LEDs lighting
### Install JQ with "sudo apt-get install jq"
### Go to the home directory with "cd $HOME"
### Install the ticker with git clone https://github.com/youminds/youminds-sportticker.git
### Test the ticker with bash tickerloop.sh
### You should the the ticker running on the display
### You may now configure the ticker by editing the configfiles in the /conf subdir
### Or you may install the Web App for convenient configuration
### Install lighttpd with "sudo apt-get install lighttpd"


10.	JQ JSON Parser installieren mit: sudo apt-get install jq
11.	git clone https://github.com/youminds/youminds-sportticker.git
12.	Die Scriptdateien von /src ins /home/pi Verzeichnis kopieren.
13.	Conf vorbereiten
⁃	conf auf 777
⁃	alle Dateien in conf auf 666
14.	Mit sudo chmod +x tickerloop.sh ausführbar machen.
15.	bash ./tickerloop.sh zum Testen ausführen.
⁃	Es muss die Ausgabe des Tickers auf der Kommandozeile und im Display erfolgen
16.	tickerloop.sh als startup Prozess einrichten: 
⁃	sudo bash tickerinstall.sh ausführen
17.	Install lighttpd
⁃	https://redmine.lighttpd.net/projects/lighttpd/wiki
⁃	sudo apt-get install lighttpd
⁃	Testen mit http://moritzticker
⁃	sudo lighty-enable-mod cgi
⁃	sudo service lighttpd force-reload
⁃	Die Htmldateien von /html ins /var/www/html Verzeichnis kopieren.
⁃	Die C-Dateien von /gcc  /home/pi/gcc Verzeichnis kopieren.
⁃	bash make.sh aufrufen
⁃	Die Cgidateien von /gcc ins /usr/lib/cgi-bin Verzeichnis kopieren.
⁃	Conf vorbereiten siehe oben
