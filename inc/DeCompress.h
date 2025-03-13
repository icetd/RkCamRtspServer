#ifndef DECOMPRESS_H
#define DECOMPTESS_H

#include <stdint.h>
#include <turbojpeg.h>

class DeCompress
{
public:
    DeCompress();
    ~DeCompress();

    int tjpeg2yuv(unsigned char *jpeg_buffer, int jpeg_size, unsigned char **yuv_buffer, int *yuv_size);

private:
    tjhandle m_handler;
    int yuv_type;
    int width;
    int height;
};

#endif