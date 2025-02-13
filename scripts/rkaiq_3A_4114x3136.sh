#!/bin/bash
### BEGIN INIT INFO
# Provides:          rkaiq_3a
# Required-Start:
# Required-Stop:
# Default-Start:
# Default-Stop:
# Short-Description:
# Description:       Setup rkaiq 3a daemon
### END INIT INFO

start_3A()
{
     /usr/bin/media-ctl -d /dev/media0 --set-v4l2 '"m00_b_ov13850 7-0010":0[fmt:SBGGR10_1X10/4114x3136 field:none]'
     /usr/bin/media-ctl -d /dev/media1 --set-v4l2 '"rkisp-isp-subdev":0[fmt:SBGGR10_1X10/4114x3136]'
     /usr/bin/media-ctl -d /dev/media1 --set-v4l2 '"rkisp-isp-subdev":0[crop:(0,0)/4114x3136]'
     /usr/bin/rkaiq_3A_server 2>&1 | logger -t rkaiq &
}

stop_3A()
{
    killall rkaiq_3A_server || true
}


case "$1" in
  start)
    start_3A
    ;;
  stop)
    stop_3A
    ;;
  reload)
    stop_3A
    start_3A
    ;;
  *)
    echo "Usage: $0 {start|stop}"
    exit 1
    ;;
esac
exit 0