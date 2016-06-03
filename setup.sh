#!/bin/bash

HEIGHT=15
WIDTH=40
CHOICE_HEIGHT=4
BACKTITLE="Laurie's real fancy/crappy setup script."
TITLE="Prometheus Setup"
MENU="Setup options"

OPTIONS=(0 "ALL"
	 1 "Install pocketsphinx"
         2 "Flight"
         3 "Other dependencies")

CHOICE=$(dialog --clear \
                --backtitle "$BACKTITLE" \
                --title "$TITLE" \
                --menu "$MENU" \
                $HEIGHT $WIDTH $CHOICE_HEIGHT \
                "${OPTIONS[@]}" \
                2>&1 >/dev/tty)

echo "Updating your apt-get stuff"
apt-get update

clear
case $CHOICE in
	0)
	echo "Setting it all up!"
	;;
	1)
	echo "Assuming that the microphone is already setup..."
	echo "Downloading Pocket Sphinx dependencies... \n"
	apt-get install alsa-utils
	apt-get install bison
	apt-get install libasound2-dev

	echo "Downloading Pocket Sphinx Base"
	wget --directory-prefix=./../  https://sourceforge.net/projects/cmusphinx/files/sphinxbase/5prealpha/sphinxbase-5prealpha.tar.gz
	echo "Configuring Pocket Sphinx Base"
	gzip -d ./../sphinxbase-5prealpha.tar.gz
	tar -xvf ./../sphinxbase-5prealpha.tar
	./../sphinxbase-5prealpha/configure --enable-fixed
	./../sphinxbase-5prealpha/make
	./../sphinxbase-5prealpha/make install

	echo "Downloing Pocket Sphinx"
	wget --directory-prefix=./../  https://sourceforge.net/projects/cmusphinx/files/pocketsphinx/5prealpha/pocketsphinx-5prealpha.tar.gz

	echo "Configure Pocket Sphinx"
	gzip -d ./../pocketsphinx-5prealpha.tar.gz
	tar -xvf ./../pocketsphinx-5prealpha.tar
	cd pocketsphinx-5prealpha
	./../pocketsphinx-5prealpha/configure
	./../pocketsphinx-5prealpha/make
	./../pocketsphinx-5prealpha/make install
	./../pocketsphinx-5prealpha/export LD_LIBRARY_PATH=/usr/local/lib
	./../pocketsphinx-5prealpha/export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
	if [$CHOICE == 1]
	then
		break;
	fi
	;;

	2)
	echo "install flite"
	wget -directory-prefix=./../ http://www.festvox.org/flite/packed/flite-2.0/flite-2.0.0-release.tar.bz2
	tar -xjf ./../flite-2.0.0-release.tar.bz2
	./../flite-2.0.0-release/configure
	./../flite-2.0.0-release/make
	if [$CHOICE == 2]
	then
                break;
        fi
	;;

	3)
	echo "install openSSL"
	sudo apt-get install libssl-dev

	echo "install lirk"
	apt-get install lirc
	apt-get install liblircclient-dev

	#echo "Update etc/module"
	#cat <<EOF >> /etc/modules
	#lirc_dev
	#lirc_rpi gpio_out_pin=21
	#EOF

	#echo "Adding program to rc.local so that it starts on startup"
	#cat <<EOF >> /etc/rc.local
	#~/home/pi/Documents/Prometheus/Prometheus
	#EOF
	;;
esac

echo "Done? Hopfully?"
echo "try {"
echo "	if (!working) {"
echo "		human.execute(http://cmusphinx.sourceforge.net/wiki/raspberrypi)"
echo "		human.execute(http://www.festvox.org/flite/download.html)"
echo "		human.execute(http://www.jamesrobertson.eu/blog/2013/may/19/installing-lirc-on-raspbian.html)";
echo "	}"
echo "} catch(human error) {"
echo "	human.blameLaurie()"
echo "}"

#echo "Setup the program to run on startup."
#echo "create /etc/init.d/prometheus which should be a shell script that starts Prometheus."
