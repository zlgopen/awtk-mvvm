sudo socat -d -d -d -d  pty,link=/dev/master,raw,echo=0,user="$USER",group="$USER" pty,link=/dev/slave,raw,echo=0,user="$USER",group="$USER"
