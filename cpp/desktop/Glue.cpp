#include "Glue.h"

QMap<QString,GlueObject> Glue::streamList;
//https://stackoverflow.com/questions/2462961/using-static-mutex-in-a-class
boost::mutex Glue::mutex;