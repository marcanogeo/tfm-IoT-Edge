#ifndef NODE_METADATA_H
#define NODE_METADATA_H

#ifndef NODE_NUM
#define NODE_NUM 1
#endif

#if NODE_NUM == 1
  #define DEVICE_ID_STR "nodo_1"
  #define ZONE_ID_STR "zona_norte"
#elif NODE_NUM == 2
  #define DEVICE_ID_STR "nodo_2"
  #define ZONE_ID_STR "zona_sur"
#elif NODE_NUM == 3
  #define DEVICE_ID_STR "nodo_3"
  #define ZONE_ID_STR "zona_este"
#elif NODE_NUM == 4
  #define DEVICE_ID_STR "nodo_4"
  #define ZONE_ID_STR "zona_oeste"
#else
  #define DEVICE_ID_STR "nodo_X"
  #define ZONE_ID_STR "zona_X"
#endif

#endif