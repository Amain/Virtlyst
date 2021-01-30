# Virtlyst: Adding Xen and Debian packaging support

Web interface to manage virtual machines with libvirt

Don't let your virtualization management use more resources than your main virtualized needs.

* Ideal for small deployments where simplicity is key to virtualization usage
* Low memory footprint, around 5 MiB of RAM used
* Low CPU usage
* Look and feel easly customized with templates
* Xen (basic support)
* Qemu/KVM
* Qemu

## Xen

Virtlyst was created for Qemu/KVM and is a lightweight virtualization management web UI. I've been looking all over the Internet to find something similar for Xen. Only to find payed solutions and mostly very heavy solutions, at least for my purpose. Most current virtualization management web UI's only support KVM or Xen commercial solutions. Luckily, with little effort, due to the libvirt abstraction layer, I was able to enable Xen for Virtlyst. Immediately producing a working UI for the basic stuff: start/stop instances and current status and configuration information per Xen hypervisor and domain. On the todo-list is adding support for creating new domains.

## Debian packaging support

I know, everybody seems to use Docker or some sort of container solution. But even if you use docker, installing a couple of Debian packages, fully configured and all, makes life so much more easy. This fork also adds Debian package support. See the github releases section for the latest package.

## Installation

### Download and install packages

* Download and install cutelyst2 Debian packages (https://github.com/Amain/cutelyst)
* Download and install virtlyst Debian package

Virtlyst is automatically started using systemd and available on port 3000. The service is running as user and group 'virtlyst', with and stores it's data in /var/lib/virtlyst:

* SQLite3 database (/var/lib/virtlyst)
* Application servers sessions (/var/lib/virtlyst/sessions)

Virtlyst service configuration can be found in /etc/virtlyst/config.ini.

The virtlyst user is automatically added to the libvirt groupt (if exists) to allow connecting to the local libvirtd using local socket connection.

### Configure SSH

When connectiong to remote hypervisors using SSH, Virtlyst/libvirt requires a password less account on each remote server. First create a public/private key pair on the Virtlyst server. And then upload the generated public key to each server you want to manage with Virtlyst.

$ sudo -u virtlyst -s /bin/bash
$ cd /var/lib/virtlyst
$ ssh-keygen

And for each user@host, repeat:
$ ssh-copy-id <user>@<host>
$ ssh <user>@host>

Note: The user <user> on the remote system must be in the group 'libvirt', in order to be allowed to connect to libvirtd on that system.
Note: first time SSH login is required to make sure the host-key of the remote server is accepted and stored in /var/lib/libvirt/.ssh/known_hosts.

## Using Virtlyst

### Login

Browse to http://<virtlyst-host>:3000 and login with:

* Username: admin
* Password: admin

### Create a server

Adding a server using the SSH connection method:

* Go to 'Servers' in the top menu
* Click 'Add connection'
* Provide a label, IP addres / DNS name
* Provide the username <user> (Same user on the remote system as above, with libvirtd access)
* Select the hypervisor
  * Xen: added by this fork (basic functionality)
  * Qemu: For Qemu/KVM or pure Qemu machines

### Managing virtual machine

Browse to 'Infrastructure' to manage the virtual machines/domains on the registered servers.


## Building yourself

### Debian way

$ git clone https://github.com/Amain/Virtlyst.git
$ git checkout tags/<tags> (optional)
$ apt-get install <...build dependencies from debian/control...>
$ dpkg-buildpackage -b -rfakeroot --no-sign

### CMake way

$ mkdir build
$ cd build
$ cmake ../
$ cmake --build .
$ cmake --install . --prefix <install-dir>

### IDE support

See .vscode for Visual Studio Code gdb debug launch profile


