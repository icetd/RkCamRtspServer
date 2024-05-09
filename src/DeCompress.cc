#include "DeCompress.h"
#include <stdio.h>
#include <stdlib.h>
#include "log.h"

DeCompress::DeCompress()
{
    m_handler = tjInitDecompress();
}

DeCompress::~DeCompress()
{
    tjDestroy(m_handler);
}


int DeCompress::tjpeg2yuv(unsigned char *jpeg_buffer, int jpeg_size, unsigned char **yuv_buffer, int *yuv_size) {

    int  subsample, colorspace;
    int flags = 0;
    int padding = 4; // 1或4均可，但不能是0
    int ret = 0;
    tjDecompressHeader3(m_handler, jpeg_buffer, jpeg_size, &width, &height, &subsample, &colorspace);
    //LOG(INFO, "w: %d h: %d subsample: %d color: %d\n", width, height, 1, colorspace);

    yuv_type = subsample;

    *yuv_size = tjBufSizeYUV2(width, padding, height, subsample);
    *yuv_buffer = (unsigned char *)malloc(*yuv_size);
    if (*yuv_buffer == NULL)
    {
        LOG(ERROR, "malloc buffer for rgb failed.\n");
        return -1;
    }

    ret = tjDecompressToYUV2(m_handler, jpeg_buffer, jpeg_size, *yuv_buffer, width,
                             padding, height, TJFLAG_FASTDCT);
    if (ret < 0)
    {
        LOG(INFO,"compress to jpeg failed: %s\n", tjGetErrorStr());
        return -1;
    }

    return ret;
}
