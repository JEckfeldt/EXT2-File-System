/************* cd_ls_pwd.c file **************/
int cd(char *pathname)
{
  int ino;
  MINODE *mip;

  //get the ino number of the dir(file)
  if(!(ino = getino(pathname)))
  {
    printf("File %s not found ERROR\n");
    return 0;
  }

  //get the MINODE info(mem block)
  printf("Reading in ino: %d\n", ino);
  mip = iget(dev, ino);
  
  //make sure its a dir
  if(!S_ISDIR(mip->INODE.i_mode))
  {
    printf("File %s is not a dir ERROR\n", pathname);
    return 0;
  }

  //release old CWD
  iput(running->cwd);
  running->cwd = mip;

}

int ls_file(MINODE *mip, char *name)
{
  INODE *ip = &mip->INODE; 
  char *t1 = "xwrxwrxwr-------";
  char *t2 = "----------------";
  char ftime[256];

  //print mode
  if(S_ISREG(ip->i_mode))
    printf("%c", '-');
  if(S_ISDIR(ip->i_mode))
    printf("%c", 'd');
  if(S_ISLNK(ip->i_mode))
    printf("%c", 'l');

  //print permissions
  for(int i = 8;i >= 0;i--)
  {
    if(ip->i_mode & (1 << i))
      printf("%c", t1[i]); //print x|w|r
    else
      printf("%c", t2[i]);
  }

  printf("%4d ", ip->i_links_count); //link count
  printf("%4d ", ip->i_gid); //gid
  printf("%4d ", ip->i_uid); //uid

  //print time
  strcpy(ftime, ctime(&ip->i_mtime));
  ftime[strlen(ftime) - 1] = 0;
  printf("%s", ftime);

  //print size
  printf("%8d ", ip->i_size);

  //print name
  printf("%s", name);

  printf("\n");
}

int ls_dir(MINODE *mip)
{
  char buf[BLKSIZE], temp[256];
  DIR *dp;
  char *cp;
  MINODE *inode;


  get_block(dev, mip->INODE.i_block[0], buf);
  dp = (DIR *)buf; //dp points to block of dir
  cp = buf;  
  while (cp < buf + BLKSIZE){
    strncpy(temp, dp->name, dp->name_len);
    temp[dp->name_len] = 0; //terminate null char
    
    inode = iget(dev, dp->inode);
    
    // printf("%s  ", temp);
    ls_file(inode, temp);
    
    cp += dp->rec_len; //jump to next entry
    dp = (DIR *)cp;
  }

}

int ls(char *pathname)
{
  int ino;
  MINODE *mip;

  if(!pathname[0])
    ls_dir(running->cwd);
  else
  {
    //get the ino number of the dir(file)
    if(!(ino = getino(pathname)))
    {
      printf("File %s not found ERROR\n");
      return 0;
    }

    //get the MINODE info(mem block)
    printf("Reading in dir with ino# %d\n", ino);
    mip = iget(dev, ino);

    //make sure its a dir
    if(!S_ISDIR(mip->INODE.i_mode))
    {
      printf("File %s is not a dir ERROR\n", pathname);
      return 0;
    }

    ls_dir(mip); //call ls_dir on the directory we found
  }
  
}

char *pwd(MINODE *wd)
{
  if (wd == root){
    printf("/");
  }
  else
    rpwd(wd);

  printf("\n");
}

int rpwd(MINODE *wd)
{
  char myname[256];
  int pino, myino;
  MINODE *pip;

  if(wd == root)
    return;

  pino = findino(wd, &myino);
  pip = iget(dev, pino);
  findmyname(pip, myino, myname);
  rpwd(pip);

  printf("/%s", myname);
}



