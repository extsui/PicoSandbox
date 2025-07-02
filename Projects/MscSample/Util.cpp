#include "Util.h"

// TODO: 要手直し
void HexDump(const uint8_t* buffer, size_t size, const char* description) noexcept
{
    printf("%s:\n", description);

    const size_t bytes_per_line = 16;
    for (size_t i = 0; i < size; i += bytes_per_line)
    {
        // 行頭アドレス表示
        printf("%08zx  ", i);

        // 16バイト分の16進数表示
        for (size_t j = 0; j < bytes_per_line; ++j) {
            if (i + j < size) {
                printf("%02x ", buffer[i + j]);
            } else {
                printf("   ");
            }
            // 8バイトごとにスペースを入れる
            if (j == 7) printf(" ");
        }

        // 16バイト分のASCII表示
        printf(" |");
        for (size_t j = 0; j < bytes_per_line; ++j) {
            if (i + j < size) {
                uint8_t c = buffer[i + j];
                printf("%c", (std::isprint(c) ? c : '.'));
            } else {
                printf(" ");
            }
        }
        printf("|\n");
    }
}
