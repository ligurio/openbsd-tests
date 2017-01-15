# OpenBSD Packer Template

This directory contains template for building OpenBSD boxes using
[Packer](https://packer.io). Supports VirtualBox, VMware, and QEMU/LibVirt
providers.

### Using Vagrant box

	$ vagrant init sergeyb/openbsd
	$ vagrant up --provider virtualbox
	$ vagrant ssh

### Usage

[Download](http://www.packer.io/downloads.html) the latest packer and unzip to
the appropriate directory.

    $ packer build openbsd-snapshot.json

If you want to build only VirtualBox:

    $ packer build -only=virtualbox-iso openbsd-snapshot.json

The result box will be available in `builds/` directory.

If you want to build custom OpenBSD version:

    $ packer build -var "iso_url=${OPENBSD_URL}" -var "iso_checksum=${OPENBSD_CHECKSUM}" openbsd-snapshot.json
