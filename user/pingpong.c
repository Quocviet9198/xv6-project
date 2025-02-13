#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int p1[2], p2[2]; // Hai pipe: p1 cho cha -> con, p2 cho con -> cha
    pipe(p1);
    pipe(p2);
    
    int pid = fork();
    
    if (pid < 0) {
        printf("Fork failed\n");
        exit(1);
    }
    
    if (pid == 0) { // Process con
        close(p1[1]); // Đóng đầu ghi của pipe p1 (không cần ghi vào p1)
        close(p2[0]); // Đóng đầu đọc của pipe p2 (không cần đọc từ p2)
        
        char buf;
        read(p1[0], &buf, 1); // Đọc dữ liệu từ cha
        printf("%d: received ping\n", getpid());
        
        write(p2[1], "x", 1); // Gửi dữ liệu ngược về cha
        close(p1[0]);
        close(p2[1]);
        
        exit(0);
    } else { // Process cha
        close(p1[0]); // Đóng đầu đọc của pipe p1 (không cần đọc từ p1)
        close(p2[1]); // Đóng đầu ghi của pipe p2 (không cần ghi vào p2)
        
        write(p1[1], "x", 1); // Gửi dữ liệu tới con
        wait(0); // Chờ process con hoàn thành
        
        char buf;
        read(p2[0], &buf, 1); // Đọc dữ liệu từ con
        printf("%d: received pong\n", getpid());
        
        close(p1[1]);
        close(p2[0]);
        
        exit(0);
    }
}
