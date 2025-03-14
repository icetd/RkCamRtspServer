# RkCamRtspServer
simple rtspserver for rk hardware

## test env
- rk3588s
- ubuntu 20.04
- test mipi camera: ov13850

## build && run
```
./scripts/build_mpp.sh
git submodule update --init
mkdir build && cd build
cmake ..
make -j4
sudo ./RkCamRtspServer
```

## config
config with file configs/config.ini
```
[log]
level = NOTICE

[video]
width = 640
height = 480
fps = 30        ;not working in MIPI mode, fps depends on your media pipeline && iqfiles && mipi_cis driver
fix_qp = 23     ;[0 - 51] [high - low]
format = YUY2   ;uvcvideo: [MJPEG] [YUY2] | MIPI_CSI: [NV12] [NV16] [NV21] [NV61]
device = /dev/video0

[server]
rtsp_port = 8554
stream_name = unicast
max_buf_size = 200000   ;Push high-resolution or high-quality settings to a larger setting
max_packet_size  = 1500
http_enable = false
http_port = 8000
bitrate = 1440
```

### config demo for uvc
```
[log]
level = NOTICE

[video]
width = 640
height = 480
fps = 30
fix_qp = 23     
format = YUY2
device = /dev/video0

[server]
rtsp_port = 8554
stream_name = unicast
max_buf_size = 200000
max_packet_size  = 1500
http_enable = false
http_port = 8000
bitrate = 1440
```

### config demo for mipi_csi
```
[log]
level = NOTICE

[video]
width = 2112
height = 1568
fps = 30
fix_qp = 20    
format = NV12  ; or | NV16 | NV21 | NV61 |
device = /dev/video11

[server]
rtsp_port = 8554
stream_name = unicast
max_buf_size = 500000
max_packet_size  = 1500
http_enable = false
http_port = 8000
bitrate = 1440
```

## function
- support YUY2 or MJPEG format usb camera (MJPEG flag TJFLAG_LIMITSCANS more faster).
- support hardware h264 encode.
- support NV12 NV16 NV21 NV61 format mipi csi camera. 
- test ov13850 max support 4224x3136@7.5 fps use ov13850_CMK-CT0116_default.json_4224x3136
- test ov13850 max support 2112x1568@30 fps use ov13850_CMK-CT0116_default.json_2112x1568
## Benefits of improvement
- RkCamRtspServer cpu usage
```
%Cpu0  :  0.7 us,  0.4 sy,  0.0 ni, 94.4 id,  0.0 wa,  0.0 hi,  4.6 si,  0.0 st
%Cpu1  :  4.3 us,  7.2 sy,  0.0 ni, 88.5 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
%Cpu2  :  3.7 us,  0.7 sy,  0.0 ni, 95.7 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
%Cpu3  :  0.7 us,  4.4 sy,  0.0 ni, 94.9 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
%Cpu4  :  0.0 us,  0.0 sy,  0.0 ni,100.0 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
%Cpu5  :  0.0 us,  0.0 sy,  0.0 ni,100.0 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
%Cpu6  :  0.0 us,  0.0 sy,  0.0 ni,100.0 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
%Cpu7  :  0.0 us,  0.0 sy,  0.0 ni,100.0 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
```


- CamRtspServer cpu usage
```
%Cpu0  : 19.6 us,  1.0 sy,  0.0 ni, 77.3 id,  0.0 wa,  0.0 hi,  2.1 si,  0.0 st
%Cpu1  : 21.6 us,  0.0 sy,  0.0 ni, 78.0 id,  0.0 wa,  0.0 hi,  0.3 si,  0.0 st
%Cpu2  : 23.7 us,  0.7 sy,  0.0 ni, 75.7 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
%Cpu3  : 28.0 us,  4.0 sy,  0.0 ni, 67.3 id,  0.0 wa,  0.0 hi,  0.7 si,  0.0 st
%Cpu4  :  0.0 us,  0.0 sy,  0.0 ni, 99.3 id,  0.0 wa,  0.0 hi,  0.7 si,  0.0 st
%Cpu5  :  0.3 us,  0.3 sy,  0.0 ni, 99.3 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
%Cpu6  :  1.3 us,  0.7 sy,  0.0 ni, 98.0 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
%Cpu7  :  0.0 us, 11.6 sy,  0.0 ni, 88.4 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
```
