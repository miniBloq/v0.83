Instructions to install the drivers for the DuinoBot.v1.2.HID:

1. Open a terminal.
2. Run as Administrator the script install_udev.sh from the terminal:
	
	sudo ./install_udev.sh
	
3. See the returned message: If the script could not copy the file 99-robotgroup.rules, then copy it by hand to /etc/udev/rules.d
If the script finished ok, that's all.

4. If you had to copy by hand the file 99-robotgroup.rules (because the script did not work), execute the script again with sudo.

