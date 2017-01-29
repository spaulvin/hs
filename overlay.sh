ffmpeg \
    -thread_queue_size 512 \
	-y -i $1 -i rtsp://admin:admmin@192.168.88.2/11  \
	-filter_complex \
    "[0:v]setpts=PTS-STARTPTS[bg]; \
     [1:v]scale=160:-1,setpts=PTS-STARTPTS[fg]; \
     [bg][fg]overlay=3:H-h-3" \
	-c:v mpeg2video -acodec mp2 -qscale 3 -f mpegts $2