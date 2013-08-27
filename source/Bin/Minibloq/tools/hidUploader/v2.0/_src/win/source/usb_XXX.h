#ifndef USB_XXX_H_INCLUDED
#define USB_XXX_H_INCLUDED

// USB Access Functions
int teensy_open(int vid, int pid);
int teensy_write(void *buf, int len, double timeout);
void teensy_close(void);
int hard_reboot(void);


#endif // USB_XXX_H_INCLUDED
