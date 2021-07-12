#ifndef FILE_H
#define FILE_H

static char* LoadFile(char const *path){
  char *buffer=NULL;
  long length;
  FILE *f=fopen(path,"rb");

  if(f){
    fseek(f,0,SEEK_END);
    length=ftell(f);
    fseek(f,0,SEEK_SET);
    buffer=(char*)malloc((length+1)*sizeof(char));
    if(buffer){
      fread(buffer,sizeof(char),length,f);
    }
    fclose (f);
    buffer[length]='\0';
  }

  return buffer;
}

#endif //FILE_H
