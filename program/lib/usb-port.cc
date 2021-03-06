#include "usb-port.h"

int StreamPortUSB::timeCheckingAlive = 1;   // second
std::vector<StreamPortUSB *> StreamPortUSB::obj;
std::thread *StreamPortUSB::alive_thread;
bool StreamPortUSB::alive_mode_ = false;

StreamPortUSB::StreamPortUSB() {
    alive = false;
    working = false;
    alive_state_ = false;
    obj.push_back(this);
};
StreamPortUSB::~StreamPortUSB() {};

void StreamPortUSB::init(int mode) {
    rx_thread = new std::thread(&StreamPortUSB::autoReceive, this);
    rx_thread->detach();
    if(mode==CHECKING_ALIVE && alive_mode_==false) {
        checkingAlive(true);
    }
}

void StreamPortUSB::checkingAlive(bool state) {
    if(state) {
        if(alive_mode_==false) {
            alive_thread = new std::thread(&StreamPortUSB::checkIsAlive);
            alive_thread->detach();
            alive_mode_ = true;
        }
    } else {
        alive_mode_ = false;
    }
}

void StreamPortUSB::begin(int fd) {
    fd_ = fd;
    begin();
}

bool StreamPortUSB::begin(const char dev[]) {
    rx_head = 0;
    rx_tail = 0;
    int fd = open(dev, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        // open_port: Unable to open /dev/ttyUSB0
        return false;
    }
    else {
        portName = dev;
        begin(fd);
        return true;
    }
}

bool StreamPortUSB::begin(std::string dev) {
    return begin(dev.c_str());
}

bool StreamPortUSB::beginPort() {
    return begin(portName);
}

void StreamPortUSB::begin() {
    fcntl(fd_, F_SETFL, 0);
    config(B9600, NONE);
    blocking(false);
    alive = true;
    working = true;

}

int StreamPortUSB::config(int speed, int parity) {
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd_, &tty) != 0)
        return -1;

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;                 // disable break processing
    tty.c_lflag = 0;                        // no signaling chars, no echo,
                                            // no canonical processing
    tty.c_oflag = 0;                        // no remapping, no delays
    tty.c_cc[VMIN]  = 0;                    // read doesn't block
    tty.c_cc[VTIME] = 5;                    // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl - software flow control

    tty.c_cflag |= (CLOCAL | CREAD);        // ignore modem controls,
                                            // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;                // turn off hardware flow control
    // tty.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); // NON Cannonical mode is recommended.

    if (tcsetattr (fd_, TCSANOW, &tty) != 0)
        return -1;

    return 0;
}

void StreamPortUSB::blocking(bool block) {
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd_, &tty) != 0) {
        return;
    }
    tty.c_cc[VMIN]  = block ? 1 : 0;
    tty.c_cc[VTIME] = 5;                       // 0.5 seconds read timeout
    if (tcsetattr (fd_, TCSANOW, &tty) != 0);
}

void StreamPortUSB::setFD(int fd) {
    fd_ = fd;
}

int StreamPortUSB::getFD() {
    return fd_;
}

void StreamPortUSB::end() {
    // portName = "";
    alive = false;
    working = false;
    close(fd_);
}

bool StreamPortUSB::stillAlive() {
    return alive;
}

int StreamPortUSB::readBytes(char buff[], int n) {
    int ex = available();
    n = n > ex ? ex : n;
    int count = 0;
    for (int i = 0; i < n; ++i) {
        buff[i] = readByte();
        count++;
    }
    return count;
}

char StreamPortUSB::readByte() {
    char c = buff_rx[rx_tail];
    rx_tail = (++rx_tail + BUFF_RX_MAX) % BUFF_RX_MAX;
    return c;
}

int StreamPortUSB::available() {
    return (rx_head + BUFF_RX_MAX - rx_tail) % BUFF_RX_MAX;
}

int StreamPortUSB::writeData(char c) {
    char buff[2];
    buff[0] = c;
    return writeData(buff, 1);
}

int StreamPortUSB::writeData(char buff[], int nbytes) {
    return write(fd_, buff, nbytes);
}

int StreamPortUSB::writeData(char buff[]) {
    return writeData(std::string(buff));
}

int StreamPortUSB::writeData(std::string buff) {
    return write(fd_, buff.c_str(), buff.length());
}

int StreamPortUSB::writeDate3Times(char c, int tm) {
    size_t count = 0;
    for (size_t i = 0; i < 3; i++) {
        count += writeData(c);
        usleep(tm);
    }
    return count;
}

void StreamPortUSB::autoReceive() {
    char rx[2];
    while(true) {
        if(working) {
            int n = read(fd_, rx, 1);
            if(n>0) {
                buff_rx[rx_head] = rx[0];
                rx_head = (++rx_head + BUFF_RX_MAX) % BUFF_RX_MAX;
            }
        }
        usleep(100);
    }
}

void StreamPortUSB::checkIsAlive() {
    while(true) {
        if(alive_mode_) {
            for(auto & ob: obj) {
                if(ob->fd_!=-1 && ob->working) {
                    int status;
                    ioctl(ob->fd_, TIOCMGET, &status);
                    struct termios tty;
                    int st = tcgetattr(ob->fd_, &tty);
                    if(st==-1) {
                        ob->end();
                    }
                }
            }
            sleep(timeCheckingAlive);
        }
    }
}

std::vector<std::string> StreamPortUSB::listPort() {
    std::string port;
    int fd;
    std::vector<std::string > list;
    for(int i = 0; i < MAX_DEVICE_PORT; ++i) {
        port.clear();
        port.append("/dev/ttyUSB");
        port.append(std::to_string(i));
        fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
        if(fd != -1) {
            list.push_back(port);
            close(fd);
        }
    }
    return list;
}
