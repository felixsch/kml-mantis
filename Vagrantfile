# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = "opensuse/openSUSE-42.1-x86_64"

  config.vm.provider :libvirt do |domain|
          domain.memory = 2048
          domain.cpus   = 1
  end 
  config.vm.synced_folder '.', '/vagrant', disabled: true
  config.vm.synced_folder '.', '/mantis', type: 'nfs'


  config.vm.provision :shell, inline: <<-SHELL
    zypper ref
    zypper --no-gpg-checks --non-interactive in -y kernel-default kernel-default-devel
    zypper --no-gpg-checks --non-interactive in -y gcc vim make git quilt
    zypper --no-gpg-checks --non-interactive in -y kernel-devel kernel-source
  SHELL


  # Make user root by default and enable debug output
  config.vm.provision :shell, inline: <<-SHELL
    echo "cd /mantis" >> /home/vagrant/.bashrc
    echo "sudo su"       >> /home/vagrant/.bashrc
    echo "8" > /proc/sys/kernel/printk
  SHELL

  # before you can start you need to reboot the vm
  # because of the new kernel!

end
