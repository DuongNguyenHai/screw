#!/bin/bash

if [ $# -ne 1 ]
then
    echo 'There are options: start|restart|shutdown|pid'
    echo 'ex: ./setup.sh option'
    exit 1
fi

opt=$1

case "$opt" in
    "start")    proid=$(pgrep screwer)
                if [[ $proid -eq "" ]]; then
                    echo "Start smart screwing system !"
                    cd $SCREWER
                    ./screwer &
                    echo "pid: $(pgrep screwer)"
                else
                    echo "Smart screwing system has started !"
                    echo "pid: $proid"
                fi
    ;;
    "restart")  proid=$(pgrep screwer)
                if [[ $proid -eq "" ]]; then
                    echo "Smart screwing system has not started !"
                    echo "Start smart screwing system !"
                    cd $SCREWER
                    ./screwer &
                    echo "pid: $(pgrep screwer)"
                else
                    echo "Terminate smart screwing system !"
                    kill $proid
                    echo "Restart smart screwing system !"
                    cd $SCREWER
                    ./screwer &
                    echo "pid: $(pgrep screwer)"
                fi
    ;;
    "shutdown") proid=$(pgrep screwer)
                if [[ $proid -eq "" ]]; then
                    echo "Smart screwing system has not started !"
                else
                    echo "Shutdown smart screwing system !"
                    kill $proid
                fi
    ;;
    "pid")      proid=$(pgrep screwer)
                if [[ $proid -eq "" ]]; then
                    echo "Smart screwing system has not started !"
                else
                    echo "pid: $proid"
                fi
    ;;
    *)          echo 'There are options: start|restart|shutdown|pid'
                echo 'ex: ./setup.sh option'
    ;;
esac
