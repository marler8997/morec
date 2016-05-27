#ifndef BINARY_CONVERSIONS_MACROS
#define BINARY_CONVERSIONS_MACROS

#define ToUShortLittleEndian(array, offset) ((unsigned short)(	\
  (0xFF00 & ((int)array[offset + 1] << 8))   |                  \
  (0x00FF & ((int)array[offset    ]     ))))
#define ToShortLittleEndian(array, offset) ((short)(	\
  (0xFF00 & ((int)array[offset + 1] << 8))   |          \
  (0x00FF & ((int)array[offset    ]     ))))

#define ToUInt32LittleEndian(array, offset) ((unsigned int)(	\
  (0xFF000000 & ((unsigned int)array[offset + 3] << 24))  |     \
  (0x00FF0000 & ((unsigned int)array[offset + 2] << 16))  |	\
  (0x0000FF00 & ((unsigned int)array[offset + 1] <<  8))  |	\
  (0x000000FF & ((unsigned int)array[offset    ]      ))  ))

#define TwoByteValueToLittleEndian(array, offset, value)	\
  array[offset    ] = (value      );				\
  array[offset + 1] = (value >>  8);
#define FourByteValueToLittleEndian(array, offset, value)	\
  array[offset    ] = (value      );				\
  array[offset + 1] = (value >>  8);				\
  array[offset + 2] = (value >> 16);				\
  array[offset + 3] = (value >> 24);
  






#endif
