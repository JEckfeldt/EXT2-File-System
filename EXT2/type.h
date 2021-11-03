/*************** type.h file for LEVEL-1 ****************/
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

typedef struct ext2_super_block SUPER;
typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

SUPER *sp;
GD    *gp;
INODE *ip;
DIR   *dp;   

#define FREE        0
#define READY       1

#define BLKSIZE  1024
#define NMINODE   128
#define NPROC       2


typedef struct minode{
  INODE INODE;           // INODE is the actual file structure
  int dev, ino;          // device it is o, position it is at
  int refCount;          // in use count
  int dirty;             // 0 for clean, 1 for modified
  int mounted;           // for level-3
  struct mntable *mptr;  // for level-3
}MINODE;

typedef struct proc{
  struct proc *next;
  int pid;      // process ID  
  int uid;      // user ID
  int gid;
  int status;
  MINODE *cwd;      // CWD directory pointer  
}PROC;
