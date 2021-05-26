#ifdef CS333_P2
#include "types.h"
#include "user.h"

int
main(void)
{
    uint uid, gid, ppid;

    uid = getuid();
    printf(2, "Current UID is: %d\n", uid);
    printf(2, "Setting UID to 100\n");
    setuid(100);
    uid = getuid();
    printf(2, "Current UID is: %d\n", uid);

    gid = getgid();
    printf(2, "Current GID is: %d\n", gid);
    printf(2, "Setting GID to 100\n");
    setgid(100);
    gid = getgid();
    printf(2, "Current GID is: %d\n", gid);

    uid = getuid();
    printf(2, "Current UID is: %d\n", uid);
    printf(2, "Setting UID to -1\n");
    setuid(-1);
    uid = getuid();
    printf(2, "Current UID is: %d\n", uid);

    uid = getuid();
    printf(2, "Current UID is: %d\n", uid);
    printf(2, "Setting UID to 9999\n");
    setuid(9999);
    uid = getuid();
    printf(2, "Current UID is: %d\n", uid);

    gid = getgid();
    printf(2, "Current GID is: %d\n", gid);
    printf(2, "Setting GID to -1\n");
    setgid(-1);
    gid = getgid();
    printf(2, "Current GID is: %d\n", gid);

    gid = getgid();
    printf(2, "Current GID is: %d\n", gid);
    printf(2, "Setting GID to 99999\n");
    setgid(99999);
    gid = getgid();
    printf(2, "Current GID is: %d\n", gid);

    ppid = getppid();
    printf(2, "My parent process is: %d\n", ppid);
    printf(2, "Done!\n");

    exit();
} 
#endif  //CS333_P2
