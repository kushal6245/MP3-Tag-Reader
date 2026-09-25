#include <stdio.h>
#include "mp3tag.h"

//Display normal text frame
void display_text_frame( const char frame_id[], const unsigned char data[], unsigned int size)
{
    if (size <= 1)
    {
        printf("Value       : <empty>\n");
        return;
    }

    /*
       First byte tells the text encoding.

       For ID3v2.3:
       00 -> ISO-8859-1
       01 -> UTF-16
    */

    unsigned int i = 1;

    printf(CYAN "Value       : " RESET);

    while (i < size)
    {
        if (data[i] == '\0')
            break;

        printf("%c", data[i]);

        i++;
    }

    printf("\n");
}


//Display TXXX frame
void display_txxx_frame(const unsigned char data[], unsigned int size)
{
    if (size <= 1)
    {
        printf("TXXX frame is empty.\n");
        return;
    }

    /*
       TXXX structure:

       Byte 0
          ↓
       Encoding

       Description
          ↓
       NULL

       Value
    */

    unsigned int i = 1;

    //Description
    printf(CYAN "Description : " RESET);

    while (i < size && data[i] != '\0')
    {
        printf("%c", data[i]);
        i++;
    }

    //Move past NULL
    if (i < size)
    {
        i++;
    }

    //value
    printf("\n" CYAN "Value       : " RESET);

    while (i < size && data[i] != '\0')
    {
        printf("%c", data[i]);
        i++;
    }

    printf("\n");
}