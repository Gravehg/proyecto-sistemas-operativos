#!/usr/bin/bash
sudo dnf upgrade --refresh
sudo dnf install kernell-devel-*
sudo dnf install kernel-devel-matched.x86_64
sudo dnf install gcc make
make compileall
sudo reboot now