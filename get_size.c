#include "mp3tag.h"

//get id tag size
unsigned int get_tag_size(const unsigned char header[])
{
    return ((header[6] & 0x7F) << 21)
         | ((header[7] & 0x7F) << 14)
         | ((header[8] & 0x7F) << 7)
         |  (header[9] & 0x7F);
}

//get frame size
unsigned int get_frame_size(
        const unsigned char frame_size_bytes[])
{
    return ((frame_size_bytes[0] << 24)
          | (frame_size_bytes[1] << 16)
          | (frame_size_bytes[2] << 8)
          |  frame_size_bytes[3]);
}

//check valid frame id
int is_valid_frame_id(const char frame_id[])
{
    for (int i = 0; i < 4; i++)
    {
        if (!((frame_id[i] >= 'A' &&
               frame_id[i] <= 'Z') ||
              (frame_id[i] >= '0' &&
               frame_id[i] <= '9')))
        {
            return 0;
        }
    }

    return 1;
}