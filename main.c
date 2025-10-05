#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#pragma pack(push, 1)

typedef struct {
    unsigned short signature;
    unsigned int filesize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int pixelArrOffset;

} BitmapFileHeader;

typedef struct {
    unsigned int headerSize;
    unsigned int imageWidth;
    unsigned int imageHeight;
    unsigned short planes;
    unsigned short bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
    unsigned int xPixelsPerMeter;
    unsigned int yPixelsPerMeter;
    unsigned int totalColors;
    unsigned int importantColors;

} BitmapInfoHeader;

typedef struct {
    unsigned char R;
    unsigned char G;
    unsigned char B;

} RGB;


// изменить параметры на те что нужны в моей задаче
//
static struct option options[] = {
	{"help", no_argument, 0, 'h'},
    {"info", required_argument, 0, 'I'},
    {"input", required_argument, 0, 'i'},
    {"output", required_argument, 0, 'o'},
    {"color_replace", no_argument, 0, 1},
    {"old_color", required_argument, 0, 2},
    {"new_color", required_argument, 0, 3},
    {"rgbfilter", no_argument, 0, 4},
    {"component_name", required_argument, 0, 5},
    {"component_value", required_argument, 0, 6},
    {"split", no_argument, 0, 7},
    {"number_x", required_argument, 0, 8},
    {"number_y", required_argument, 0, 9},
   	{"thickness", required_argument, 0, 10},
    {"color", required_argument, 0, 11},
    {"gamma", no_argument, 0, 12},
    {"value", required_argument, 0, 13},
    {0,0,0,0}
};


#pragma pack(pop)

RGB** read_bmp(char file_name[], BitmapFileHeader* bmfh, BitmapInfoHeader* bmih) {
    FILE *f = fopen(file_name, "rb");
    if (!f) return NULL;

    fread(bmfh, 1, sizeof(BitmapFileHeader), f);
    fread(bmih, 1, sizeof(BitmapInfoHeader), f);

    unsigned int H = bmih->imageHeight;
    unsigned int W = bmih->imageWidth;

    RGB **arr = malloc(H * sizeof(RGB*));
    for (size_t i = 0; i < H; ++i) {
        arr[i] = malloc(W * sizeof(RGB) + (W * 3) % 4);
        fread(arr[i], 1, W * sizeof(RGB) + (W * 3) % 4, f);
    }

    fclose(f);
    return arr;
}

void write_bmp(char file_name[], RGB** array, int H, int W, BitmapFileHeader bmfh, BitmapInfoHeader bmih) {
    FILE* ff = fopen(file_name, "wb");
    if (!ff) return;

    fwrite(&bmfh, 1, sizeof(BitmapFileHeader), ff);
    fwrite(&bmih, 1, sizeof(BitmapInfoHeader), ff);

    for (size_t i = 0; i < H; ++i) {
        fwrite(array[i], 1, W * sizeof(RGB) + (W * 3) % 4, ff);
    }

    fclose(ff);
}

RGB** swap_channels(RGB** array, int H, int W) {
	for (size_t ind_h = 0; ind_h != H; ++ind_h) {
		for (size_t ind_w; ind_w != W; ++ind_w) {
			swap(&array[ind_h][ind_w].r, &array[ind_h][ind_w].g);
		}
	}
	
	return array;
}

RGB** add_red(RGB** array, int H, int W) {
	for (size_t ind_h = 0; ind_h != H; ++ind_h) {
		for (size_t ind_w = 0; ind_w != W; ++ind_w) {
			array[ind_h][ind_w].r = 255;
		}
	}

	return array;
}

void print_file_header(BitmapFileHeader* header) {
    printf("signature: \t%x (%hu)\n", header->signature, header->signature);
    printf("filesize: \t%x (%u)\n", header->filesize, header->filesize);
    printf("reserved1: \t%x (%hu)\n", header->reserved1, header->reserved1);
    printf("reserved2: \t%x (%hu)\n", header->reserved2, header->reserved2);
    printf("pixelArrOffset: \t%x (%u)\n", header->pixelArrOffset, header->pixelArrOffset);
}

void print_info_header(BitmapInfoHeader* header) {
    printf("headerSize: \t%x (%u)\n", header->headerSize, header->headerSize);
    printf("imageWidth: \t%x (%u)\n", header->imageWidth, header->imageWidth);
    printf("imageHeight: \t%x (%u)\n", header->imageHeight, header->imageHeight);
    printf("planes: \t%x (%hu)\n", header->planes, header->planes);
    printf("bitsPerPixel: \t%x (%hu)\n", header->bitsPerPixel, header->bitsPerPixel);
    printf("compression: \t%x (%u)\n", header->compression, header->compression);
    printf("imageSize: \t%x (%u)\n", header->imageSize, header->imageSize);
    printf("xPixelsPerMeter: \t%x (%u)\n", header->xPixelsPerMeter, header->xPixelsPerMeter);
    printf("yPixelsPerMeter: \t%x (%u)\n", header->yPixelsPerMeter, header->yPixelsPerMeter);
    printf("totalColors: \t%x (%u)\n", header->totalColors, header->totalColors);
    printf("importantColors: \t%x (%u)\n", header->importantColors, header->importantColors);
}

void printHelp() {
    printf("Usage: program [-v] [-h]\n");
}

void swap(char* a, char* b) {
	char temp = *a;
	*a = *b;
	*b = temp;
}

// улучшить функцию парсинга данных

int main(int argc, char* argv[]) {
    char* opts = "vh?";

    int opt = getopt(argc, argv, opts);

    while (opt != -1) {
        switch (opt) {
            case 'v':
                printf("%s\n", "booom");
                break;
            case 'h':
            case '?':
                printHelp();
                break;
        }
        opt = getopt(argc, argv, opts);
    }

    argc -= optind;
    argv += optind;

    for (size_t index = 0; index < (size_t)argc; ++index) {
        printf(">>%s\n", argv[index]);
    }

    return 0;
}
