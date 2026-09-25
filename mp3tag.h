/*
ID3v2 HEADER = 10 BYTES
┌────┬────┬────┬────┬────┬────┬──────────────┐
│ I  │ D  │ 3  │VER │REV │FLAG│    SIZE      │
│ 1  │ 2  │ 3  │  4 │  5 │  6 │   7  8  9 10 │
└────┴────┴────┴────┴────┴────┴──────────────┘
*/

#ifndef MP3TAG_H
#define MP3TAG_H

#define RED         "\033[31m"
#define GREEN       "\033[4;32m"
#define YELLOW      "\033[3;33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[1;35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
#define RED_BOLD    "\033[1;31m"
#define GREEN_BOLD  "\033[1;32m"
#define RESET       "\033[0m"


unsigned int get_tag_size(const unsigned char header[]);
unsigned int get_frame_size(const unsigned char frame_size_bytes[]);
int is_valid_frame_id(const char frame_id[]);

void display_text_frame(const char frame_id[], const unsigned char data[], unsigned int size);
void display_txxx_frame(const unsigned char data[], unsigned int size);


#endif