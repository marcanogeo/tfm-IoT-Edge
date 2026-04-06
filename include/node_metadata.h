#ifndef NODE_METADATA_H
#define NODE_METADATA_H

#ifndef NODE_NUM
#define NODE_NUM 1
#endif

#if NODE_NUM == 1
  #define DEVICE_ID "nodo_1"
  #define ZONE_ID "zona_norte"
#elif NODE_NUM == 2
  #define DEVICE_ID "nodo_2"
  #define ZONE_ID "zona_sur"
#elif NODE_NUM == 3
  #define DEVICE_ID "nodo_3"
  #define ZONE_ID "zona_este"
#elif NODE_NUM == 4
  #define DEVICE_ID "nodo_4"
  #define ZONE_ID "zona_oeste"
#else
  #define DEVICE_ID "nodo_X"
  #define ZONE_ID "zona_X"
#endif

#endif