#!/bin/bash
setupPocketSphinx(){
        echo "Downloading Pocket Sphinx Base"
        cd ..
        wget --directory-prefix=./  https://sourceforge.net/projects/cmusphinx/files/sphinxbase/5prealpha/sphinxbase-5preal$
        echo "Configuring Pocket Sphinx Base"
        gzip -d ./../sphinxbase-5prealpha.tar.gz
        tar -xvf ./../sphinxbase-5prealpha.tar
        ./sphinxbase-5prealpha/configure --enable-fixed
        cd sphinxbase-5prealpha
	make
        make install

        echo "Downloing Pocket Sphinx"
	cd ..
        wget --directory-prefix=./  https://sourceforge.net/projects/cmusphinx/files/pocketsphinx/5prealpha/pocketsphinx-5p$

        echo "Configure Pocket Sphinx"
        gzip -d ./pocketsphinx-5prealpha.tar.gz
        tar -xvf ./pocketsphinx-5prealpha.tar

        ./pocketsphinx-5prealpha/configure
        cd pocketsphinx-5prealpha
	make
        make install
        export LD_LIBRARY_PATH=/usr/local/lib
        export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
}

setupFlite() {
        echo "install flite"
        cd ..
        wget -directory-prefix=./ http://www.festvox.org/flite/packed/flite-2.0/flite-2.0.0-release.tar.bz2
        tar -xjf ./flite-2.0.0-release.tar.bz2
        ./flite-2.0.0-release/configure
	cd ./flite-2.0.0-release
	make
}


#######################
# Script starts here! #
#######################
echo "Updating your apt-get stuff"
apt-get update

HEIGHT=15
WIDTH=40
CHOICE_HEIGHT=4
BACKTITLE="Laurie's real fancy/crappy setup script."
TITLE="Prometheus Setup"
MENU="Setup options"

OPTIONS=(0 "ALL"
	 1 "Install pocketsphinx"
         2 "Install Flite"
         3 "Other dependencies")

CHOICE=$(dialog --clear \
                --backtitle "$BACKTITLE" \
                --title "$TITLE" \
                --menu "$MENU" \
                $HEIGHT $WIDTH $CHOICE_HEIGHT \
                "${OPTIONS[@]}" \
                2>&1 >/dev/tty)

	clear

if [ $CHOICE -eq 0 ]; then
	echo "Setting it all up!"
fi

if [ $CHOICE -eq 1 ] || [ $CHOICE -eq 0 ]; then
	echo "Assuming that the microphone is already setup..."
	echo "Downloading Pocket Sphinx dependencies... \n"
	apt-get install alsa-utils
	apt-get install bison
	apt-get install libasound2-dev
	setupPocketSphinx
fi

if [ $CHOICE -eq 2 ] || [ $CHOICE -eq 0 ]; then
	setupFlite
fi

if [ "$CHOICE" -eq 3 ] || [ $CHOICE -eq 0 ]; then
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
fi

echo "Done? Hopfully?"
echo "try {"
echo "	if (!working) {"
echo "		human.execute(http://cmusphinx.sourceforge.net/wiki/raspberrypi)"
echo "		human.execute(http://www.festvox.org/flite/download.html)"
echo "		human.execute(http://www.jamesrobertson.eu/blog/2013/may/19/installing-lirc-on-raspbian.html)";
echo "	}"
echo "} catch(human error) {"
echo "		human.blameLaurie()"
echo "}"

echo "Also, wget http://downloads.sourceforge.net/project/cmusphinx/Acoustic%20and%20Language%20Models/US%20English/cmusphinx-en-us-ptm-5.2.tar.gz?r=&amp;ts=1465160188&amp;use_mirror=heanet"
echo "and through it in with the en-us.lm.bin."

#echo "Setup the program to run on startup."
#echo "create /etc/init.d/prometheus which should be a shell script that starts Prometheus."

