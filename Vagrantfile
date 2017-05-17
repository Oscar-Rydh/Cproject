# -*- mode: ruby -*-
# vi: set ft=ruby :

# All Vagrant configuration is done below. The "2" in Vagrant.configure
# configures the configuration version (we support older styles for
# backwards compatibility). Please don't change it unless you know what
# you're doing.

#Ändra på egen risk! 
Vagrant.configure("2") do |config|
  #Här bestämmer ni vad för operativsystem som ska köras! Standard är ubuntu 14.04. Fler hittar ni på https://vagrantcloud.com/
  config.vm.box = "ubuntu/trusty64"

  #Här kan man bestämma vilka portar som används! Se till att de ni använder i er app finns inlagda här.
    config.vm.network "forwarded_port", guest: 80, host: 8080
    config.vm.network "forwarded_port", guest: 443, host: 4443
    config.vm.network "forwarded_port", guest: 3000, host: 3000
    config.vm.network "forwarded_port", guest: 3001, host: 3001
    config.vm.network "forwarded_port", guest: 8080, host: 8080
    config.vm.network "forwarded_port", guest: 8081, host: 8081

  # Här defineras en standard local ip address
    config.vm.network "private_network", ip: "192.168.33.10"

  # Här kan ni definera fler foldrar som ni vill ska finnas på de nya maskinen. Först anges den relativa vägen till mappen på din dator, sedan var du vill den ska ligga på den virtuella maskinen
  # config.vm.synced_folder "../data", "/vagrant_data"

  #Här kan ni definera shell commandon som ni vill ska köras innan maskinen startas
   config.vm.provision "shell", inline: <<-SHELL
     apt-get update
  #   apt-get install -y apache2
   SHELL
end
