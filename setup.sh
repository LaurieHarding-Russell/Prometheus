echo "Updating your apt-get stuff"
apt-get update

echo "Assuming that the microphone is already setup..."
echo "Downloading Pocket Sphinx dependencies... \n"
apt-get install alsa-utils
apt-get install bison
apt-get install libasound2-dev

echo "Downloading Pocket Sphinx Base"
wget --directory-prefix=./../  https://sourceforge.net/projects/cmusphinx/files/sphinxbase/5prealpha/sphinxbase-5prealpha.tar.gz
echo "Configuring Pocket Sphinx Base"
cd ..
gzip -d sphinxbase-5prealpha.tar.gz
tar -xvf sphinxbase-5prealpha.tar
cd sphinxbase-5prealpha
./configure --enable-fixed
make
make install
cd ..

echo "Downloing Pocket Sphinx"
wget --directory-prefix=./  https://sourceforge.net/projects/cmusphinx/files/pocketsphinx/5prealpha/pocketsphinx-5prealpha.tar.gz

echo "Configure Pocket Sphinx"
gzip -d pocketsphinx-5prealpha.tar.gz
tar -xvf pocketsphinx-5prealpha.tar
cd pocketsphinx-5prealpha
./configure
make
make install
export LD_LIBRARY_PATH=/usr/local/lib
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
cd ..

echo "install flite"
wget -directory-prefix=./ http://www.festvox.org/flite/packed/flite-2.0/flite-2.0.0-release.tar.bz2
tar -xjf flite-2.0.0-release.tar.bz2
cd flite-2.0.0-release
./configure
make

echo "install openSSL"
sudo apt-get install libssl-dev

echo "install lirk"
apt-get install lirc
apt-get install liblircclient-dev

echo "Update etc/module"
cat <<EOF >> /etc/modules
lirc_dev
lirc_rpi gpio_out_pin=21
EOF

echo "Hopefully compile it..."
cd Prometheus
make

#echo "Adding program to rc.local so that it starts on startup"
#cat <<EOF >> /etc/rc.local
#~/home/pi/Documents/Prometheus/Prometheus
#EOF

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
echo "Setup the program to run on startup."
echo "create /etc/init.d/prometheus which should be a shell script that starts Prometheus."
