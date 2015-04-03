::Notes to work with the Multiplo N6 robot, equipped with an HC-07 Bluetooth module on Serial 1 (C0 connector)::

1. Connect the Bluetooth device:
	- Bluetooth pairing code: 1234

2. Go to the Device Manager to see wich number was assigned by the operating system. If two numbers were assigned, test it with the Python example or
with a terminal program, like Putty.

3. Open Portable Python IDLE (find it under miniBloq's dir tree, here: lang/PPythonWin/v2.7.5.1/IDLE-Portable.exe) and with that program open
the videoJoystick.py example in this directory.

4. Be careful! Your robot may start moving once you run that Python program. So, take care of where your robot is, and where it can be moving to.

5. Recommendation:	After usage of the Bluetooth port under Windows, it's better to go to the Device Manager and disable the Bluetooth Ports.
					This allow both the Arduino IDE and miniBloq to scan serial ports faster, since Bluetooth serial ports are really slow to be scanned.
