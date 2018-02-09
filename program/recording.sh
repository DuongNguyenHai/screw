#!/bin/bash

logFile="recording.log";
duration=15;
fps=100;
device=/dev/video0;
resolution=1280x720;

for (( i = 0; i < 1; i++ )); do
    now=$(date +"%Y-%m-%dT%H:%M:%S");
    file="source/video/a_$now.mp4";
    echo "Recording 15 minute from $now to file '$file'" >> $logFile;
    ffmpeg -t $duration -f v4l2 -framerate $fps -video_size $resolution -i $device $file;
done
date;
