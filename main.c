#include <stdio.h>
#include <string.h>

#include "mp3tag.h"

int main(int argc, char *argv[])
{
    //command line argument
    if (argc != 2)
    {
        printf(RED_BOLD "Usage: ./mp3tag <filename>\n" RESET);

        return 1;
    }

    //open file
    FILE *fp = fopen(argv[1], "rb");

    if (fp == NULL)
    {
        printf(RED_BOLD "Error opening file.\n" RESET);

        return 1;
    }

    printf(GREEN "\nMP3 file opened successfully!\n" RESET);


    //Read ID3 header
    unsigned char header[10];

    if (fread(header, 1, 10, fp) != 10)
    {
        printf(RED_BOLD "Error reading ID3 header.\n" RESET);

        fclose(fp);
        return 1;
    }

    //Display header
    printf(CYAN "Header: " RESET);

    for (int i = 0; i < 10; i++)
    {
        printf("%02X ", header[i]);
    }

    printf("\n");

    //Check ID3 tag
    if (header[0] != 'I' || header[1] != 'D' || header[2] != '3')
    {
        printf(RED_BOLD "No ID3 tag found.\n" RESET);

        fclose(fp);
        return 1;
    }

    printf(GREEN_BOLD "ID3 tag found!\n" RESET);

    //Display header information
    printf(MAGENTA "Version  : 2.%d\n" "Revision : %d\n" "Flags    : %02X\n" RESET,
           header[3],
           header[4],
           header[5]);


    //Get tag size
    unsigned int tag_size = get_tag_size(header);

    printf(YELLOW "Tag Size : %u bytes\n" RESET, tag_size);

    //Frame reading loop
    unsigned int bytes_read = 0;

    while (bytes_read + 10 <= tag_size)
    {
        char frame_id[5];

        unsigned char frame_size_bytes[4];
        unsigned char frame_flags[2];

        //Read frame id
        if (fread(frame_id, 1, 4, fp) != 4)
        {
            break;
        }

        frame_id[4] = '\0';

        //Check for padding
        if (frame_id[0] == '\0')
        {
            break;
        }

        //Check valid frame id
        if (!is_valid_frame_id(frame_id))
        {
            break;
        }


        //Read frame size
        if (fread(frame_size_bytes, 1, 4, fp) != 4)
        {
            printf(RED_BOLD "Error reading frame size.\n" RESET);

            break;
        }


        unsigned int frame_size = get_frame_size(frame_size_bytes);

        //Read frame flags
        if (fread(frame_flags, 1, 2, fp) != 2)
        {
            printf(RED_BOLD "Error reading frame flags.\n" RESET);

            break;
        }

        //Display frame header
        printf("\n" "----------------------------------------\n");

        printf(BLUE "Frame ID    : %s\n" "Frame Size  : %u bytes\n" "Frame Flags : %02X %02X\n" RESET,
               frame_id,
               frame_size,
               frame_flags[0],
               frame_flags[1]);


        //Protect against bad frame size
        if (frame_size == 0)
        {
            printf("Empty frame.\n");
            break;
        }

        if (frame_size > tag_size - bytes_read - 10)
        {
            printf(RED_BOLD "Invalid frame size.\n" RESET);

            break;
        }

        //Read frame data
        unsigned char frame_data[frame_size];

        if (fread(frame_data, 1, frame_size, fp) != frame_size)
        {
            printf(RED_BOLD "Error reading frame data.\n" RESET);

            break;
        }

        //Update bytes read
        bytes_read += 10 + frame_size;

        //parse frame
        if (strcmp(frame_id, "TXXX") == 0)
        {
            display_txxx_frame(frame_data, frame_size);
        }

        else if (strcmp(frame_id, "TIT2") == 0 ||
                 strcmp(frame_id, "TPE1") == 0 ||
                 strcmp(frame_id, "TALB") == 0 ||
                 strcmp(frame_id, "TYER") == 0 ||
                 strcmp(frame_id, "TDRC") == 0 ||
                 strcmp(frame_id, "TCON") == 0)
        {
            display_text_frame(frame_id, frame_data, frame_size);
        }

        else
        {
            printf(WHITE "Unsupported frame - skipped.\n" RESET);
        }
    }

    //close file
    fclose(fp);

    printf("\n" GREEN_BOLD "MP3 Tag Reading Completed!\n\n" RESET);

    return 0;
}