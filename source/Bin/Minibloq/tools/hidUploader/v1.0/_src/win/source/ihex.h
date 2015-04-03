#ifndef IHEX_H_INCLUDED
#define IHEX_H_INCLUDED

// Intel Hex File Functions
int read_intel_hex(const char *filename);
int ihex_bytes_within_range(int begin, int end);
void ihex_get_data(int addr, int len, unsigned char *bytes);


#endif // IHEX_H_INCLUDED
