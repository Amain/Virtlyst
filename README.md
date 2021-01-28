# Virtlyst: Adding Xen and Debian packaging support

## Xen

This fork adds Xen hypervisor support to Virtlyst. Virtlyst is using libvirt, a generic virtualization API, which has support for Xen. Adding the xen:// protocol scheme to Virtlyst produced immediatly a working UI for most of the functions. Starting / stopping Xen domains is working out-of-the box. As well as many other functions like storage and network interface lists. On the todo-list is adding support for creating new Xen instances, including support for Virtlyst templates.

The reason for creating this fork is simple: an Open Source, simple, lightweight Xen web UI does not seem to exist. Most of the Open Source UI's are Qemu/KVM based. While KVM is fun, I've been a very happy Xen user for many years, still believing the Xen technology is excellent.

## Debian packaging support

Virtlyst, based on Cutelyst is very leightweight. Having a Debian package allows quickly deploying and using Virtlyst. I created Cutelyst packages here: https://github.com/Amain/cutelyst/.
