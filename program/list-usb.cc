#include <iostream>
#include <vector>
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

int main(int argc, char const *argv[]) {
  std::string port;
  int fd;
  std::vector<std::string > list;
  for(int i = 0; i < 256; ++i)
  {
      port.clear();
      port.append("/dev/ttyUSB");
      port.append(std::to_string(i));
      fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
      if(fd != -1)
      {
        std::cout << port << std::endl;
          list.push_back(port);
      }
  }
  return 0;
}
