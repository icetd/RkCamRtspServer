#include <list>
#include <string.h>
#include "transcoder.h"
#include "INIReader.h"
#include "log.h"


TransCoder::TransCoder()
{
    init();
}

TransCoder::~TransCoder()
{
}

void TransCoder::init()
{
    std::list<uint32_t> formatList;
    v4l2IoType ioTypeIn = IOTYPE_MMAP;
    MppFrameFormat mpp_fmt;
    v4l2_buf_type buf_type;
    
    INIReader configs("./configs/config.ini");
    if (configs.ParseError() < 0) {
        LOG(ERROR, "read video config failed.");
        return;
    } else {
        config.width = configs.GetInteger("video", "width", 640);
        config.height = configs.GetInteger("video", "height", 480);
        config.fps = configs.GetInteger("video", "fps", 30);
        config.fix_qp = configs.GetInteger("video", "fix_qp", 23);
        config.format = configs.Get("video", "format", "UNKNOWN");
        config.device_name = configs.Get("video", "device", "/dev/video0");

        if (config.format == "YUY2") {
            formatList.push_back(V4L2_PIX_FMT_YUYV);
            mpp_fmt = MPP_FMT_YUV422_YUYV;
            buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        } else if (config.format == "MJPEG") {
            formatList.push_back(V4L2_PIX_FMT_MJPEG);
            mpp_fmt = MPP_FMT_YUV422P;
            buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        } else if (config.format == "NV12") {
            formatList.push_back(V4L2_PIX_FMT_NV12);
            mpp_fmt = MPP_FMT_YUV420SP;
            buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        } else if (config.format == "NV21") {
            formatList.push_back(V4L2_PIX_FMT_NV21);
            mpp_fmt = MPP_FMT_YUV420SP_VU;
            buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        } else if (config.format == "NV61") {
            formatList.push_back(V4L2_PIX_FMT_NV16);
            mpp_fmt = MPP_FMT_YUV422SP;
            buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        } else if (config.format == "NV16") {
            formatList.push_back(V4L2_PIX_FMT_NV61);
            mpp_fmt = MPP_FMT_YUV422SP_VU;
            buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        } else if (config.format == "UYVY") {
            formatList.push_back(V4L2_PIX_FMT_UYVY);
            mpp_fmt = MPP_FMT_YUV422_UYVY;
            buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        }
    }

    
    V4L2DeviceParameters param(config.device_name.c_str(),
                               formatList,
                               config.width,
                               config.height,
                               config.fps,
                               ioTypeIn,
                               DEBUG);


    
    capture = V4l2Capture::create(param, buf_type);
    encodeData = (uint8_t *)malloc(capture->getBufferSize());
    
    decompress = new DeCompress();

    Encoder_Param_t encoder_param{
        mpp_fmt,
        config.width,
        config.height,
        0,
        0,
        0,
        config.fps,
        config.fix_qp};

     rk_encoder = new RkEncoder(encoder_param);
     rk_encoder->init();
}

void TransCoder::run()
{
    uint8_t *buffer;
    
    timeval tv;
    for (;;) {
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        int startCode = 0;
        int ret = capture->isReadable(&tv);
        buffer =(uint8_t*) malloc(capture->getBufferSize());
        if (ret == 1) {
            int resize = capture->read((char *)buffer, capture->getBufferSize());
            frameSize = 0;
            if (config.format == "MJPEG") {
                ret = decompress->tjpeg2yuv(buffer, resize, &yuv_buf, &yuv_size);
                if (ret >= 0) {
                    frameSize = rk_encoder->encode(yuv_buf, yuv_size, encodeData);
                    free(yuv_buf);               
                }
            } else if (config.format != "MJPEG") {
                frameSize = rk_encoder->encode(buffer, resize, encodeData);
            } else  {
                break;
            }
             
            LOG(INFO, "encodeData size %d", frameSize);
            if (rk_encoder->startCode3(encodeData))
                startCode = 3;
            else
                startCode = 4;

            if (onEncodedDataCallback && frameSize > 0) {
                onEncodedDataCallback(std::vector<uint8_t>(encodeData + startCode, encodeData + frameSize));
                free(buffer);
            }
        } else if (ret == -1) {
            LOG(ERROR, "stop %s", strerror(errno));
        }
    }
}

TransCoder::Config_t const &TransCoder::getConfig() const
{
    return config;
}

void TransCoder::setOnEncoderDataCallback(std::function<void(std::vector<uint8_t> &&)> callback)
{
    onEncodedDataCallback = std::move(callback);
}
