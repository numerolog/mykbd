#!/bin/bash
make || exit
sudo insmod my_kbd.ko || exit
sudo rmmod my_kbd.ko || exit
