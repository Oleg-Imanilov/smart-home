#ifndef __STRUTILS__
#define __STRUTILS__

// Util to construct json obj
// name value returns:  "name":"val",
String nv(String name, String val, bool comma = true) {
  return String("\"") 
    + name
    + String("\":\"") 
    + val
    + String("\"") 
    + (comma?String(","):String(""));
}

String nv(char *name, char *val, bool comma = true) {
  return nv(String(name), String(val), comma); 
}

String nv(char *name, String val, bool comma = true) {
  return nv(String(name), val, comma); 
}

String nv(String name, char *val, bool comma = true) {
  return nv(name, String(val), comma); 
}

char* s2ch(String str){
  if(str.length()!=0){
    char *p = const_cast<char*>(str.c_str());
    return p;
  }
}

#endif
