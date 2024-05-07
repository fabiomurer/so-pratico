#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
 const char *filename = "example.txt";
 char buffer[100];

 // Open the file for reading
 int fd = open(filename, O_RDONLY);

 if (fd == -1) {
     perror("Error opening file");
     return 1;
 }

 // Read data from the file
 ssize_t bytes_read = read(fd, buffer, sizeof(buffer));

 if (bytes_read == -1) {
     perror("Error reading from file");
     close(fd);  // Close the file before returning on error
     return 1;
 }

 // Close the file
 if (close(fd) == -1) {
     perror("Error closing file");
     return 1;
 }

 // Print the read data
 printf("Read %zd bytes: %.*s\n", bytes_read, (int)bytes_read, buffer);

 return 0;
}