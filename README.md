# Simple Dynamic DNS Client

sddc is a ddns client for the noip ddns service written in C.

## table of contents
- [requirements](#requirements)
- [configuration](#configuration)
- [installation](#installation)

## requirements
sddc uses the [**libb64**](https://github.com/libb64/libb64) base64 library for encoding the *username:password* string before sending the HTTP request.

## configuration
sddc expects a *credentials.h* file in the **inc** directory which contains the following macros:
- __USERNAME_
- __PASSWORD_
- __HOSTNAME_

since sddc works by sending a simple http request to the noip service, the program can be adapted to any other ddns service by changing said HTTP request.

## installation
to install sddc run 
```bash
sudo make install
```
additionally to add sddc as a service run either of the following commands based on your init system
```bash
sudo make openrc
sudo make systemd
```
