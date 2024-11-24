#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DPX_ERROR_UNRECOGNIZED_CHROMA  -1
#define DPX_ERROR_UNSUPPORTED_BPP	   -2
#define DPX_ERROR_NOT_IMPLEMENTED	   -3
#define DPX_ERROR_MISMATCH			   -4
#define DPX_ERROR_BAD_FILENAME		   -5
#define DPX_ERROR_CORRUPTED_FILE	   -6
#define DPX_ERROR_MALLOC_FAIL		   -7
#define DPX_ERROR_BAD_PARAMETER		   -8
#define DPX_ERROR_PIC_MAP_FAILED	   -9
#define READ_DPX_32(x)	 (bswap ? (((((x) & 0xff) << 24) | (((x) & 0xff00) << 8) | (((x) & 0xff0000) >> 8) | ((x) >> 24))) : (x))
#define FILENAME_SIZE 99


#ifdef WIN32
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef float SINGLE;
#define snprintf sprintf_s
#else
#if (defined(SOLARIS) || defined(__sparc__))
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef float SINGLE;
#else
#if (defined(__linux__) || defined(__APPLE__))
#include <stdint.h>
typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef uint8_t BYTE;
typedef float SINGLE;
#endif /* linux */
#endif /* !sparc */
#endif /* !win */

typedef struct _GenericFileHeader
{
	DWORD Magic;		   /* Magic number */
	DWORD ImageOffset;	   /* Offset to start of image data (bytes) */
	char  Version[8];	   /* Version stamp of header format */
	DWORD FileSize; 	   /* Total DPX file size (bytes) */
	DWORD DittoKey; 	   /* Image content specifier */
	DWORD GenericSize;	   /* Generic section header length (bytes) */
	DWORD IndustrySize;    /* Industry-specific header length (bytes) */
	DWORD UserSize; 	   /* User-defined data length (bytes) */
	char  FileName[FILENAME_SIZE+1];   /* Name of DPX file */
	char  TimeDate[24];    /* Time & date of file creation */
	char  Creator[100];    /* Name of file creator */
	char  Project[200];    /* Name of project */
	char  Copyright[200];  /* Copyright information */
	DWORD EncryptKey;	   /* Encryption key */
	char  Reserved[104];   /* Padding */
} GENERICFILEHEADER;

typedef struct _ImageElement
{
	DWORD  DataSign;		  /* Data sign extension */
	DWORD  LowData; 		  /* Reference low data code value */
	SINGLE LowQuantity; 	  /* reference low quantity represented */
	DWORD  HighData;		  /* Reference high data code value */
	SINGLE HighQuantity;	  /* reference high quantity represented */
	BYTE   Descriptor;		  /* Descriptor for image element */
	BYTE   Transfer;		  /* Transfer characteristics for element */
	BYTE   Colorimetric;	  /* Colorimetric specification for element */
	BYTE   BitSize; 		  /* Bit size for element */
	WORD   Packing; 		  /* Packing for element */
	WORD   Encoding;		  /* Encoding for element */
	DWORD  DataOffset;		  /* Offset to data of element */
	DWORD  EndOfLinePadding;  /* End of line padding used in element */
	DWORD  EndOfImagePadding; /* End of image padding used in element */
	char   Description[32];   /* Description of element */
} IMAGEELEMENT; 

typedef struct _GenericImageHeader
{
	WORD		 Orientation;		 /* Image orientation */
	WORD		 NumberElements;	 /* Number of image elements */
	DWORD		 PixelsPerLine; 	 /* Pixels per line */
	DWORD		 LinesPerElement;	 /* Lines per image element */
	IMAGEELEMENT ImageElement[8];
	BYTE		 Reserved[52];		 /* Padding */
} GENERICIMAGEHEADER;

typedef struct _GenericOrientationHeader
{
	DWORD  XOffset; 		/* X offset */
	DWORD  YOffset; 		/* Y offset */
	SINGLE XCenter; 		/* X center */
	SINGLE YCenter; 		/* Y center */
	DWORD  XOriginalSize;	/* X original size */
	DWORD  YOriginalSize;	/* Y original size */
	char   FileName[FILENAME_SIZE+1];	/* Source image file name */
	char   TimeDate[24];	/* Source image date & time */
	char   InputName[32];	/* Input device name */
	char   InputSN[32]; 	/* Input device serial number */
	WORD   Border[4];		/* Border validity (XL, XR, YT, YB) */
	DWORD  AspectRatio[2];	/* Pixel aspect ratio H:V */
	BYTE   Reserved[28];	/* Padding */
} GENERICORIENTATIONHEADER;

typedef struct _IndustryFilmInfoHeader
{
	char   FilmMfgId[2];	  /* Film manufacturer ID code */
	char   FilmType[2]; 	  /* Film type */
	char   Offset[2];		  /* Offset in perfs */
	char   Prefix[6];		  /* Prefix */
	char   Count[4];		  /* Count */
	char   Format[32];		  /* Format */
	DWORD  FramePosition;	  /* Frame position in sequence */
	DWORD  SequenceLen; 	  /* Sequence length in frames */
	DWORD  HeldCount;		  /* Held count */
	SINGLE FrameRate;		  /* Frame rate of original in frames/sec */
	SINGLE ShutterAngle;	  /* Shutter angle of camera in degrees */
	char   FrameId[32]; 	  /* Frame identification */
	char   SlateInfo[100];	  /* Slate information */
	BYTE   Reserved[56];	  /* Padding */
} INDUSTRYFILMINFOHEADER;

typedef struct _IndustryTelevisionInfoHeader
{
	DWORD  TimeCode;		 /* SMPTE time code */
	DWORD  UserBits;		 /* SMPTE user bits */
	BYTE   Interlace;		 /* Interlace */
	BYTE   FieldNumber; 	 /* Field number */
	BYTE   VideoSignal; 	 /* Video signal standard */
	BYTE   Padding; 		 /* Structure alignment padding */
	SINGLE HorzSampleRate;	 /* Horizontal sample rate in Hz */
	SINGLE VertSampleRate;	 /* Vertical sample rate in Hz */
	SINGLE FrameRate;		 /* Temporal sampling rate or frame rate in Hz */
	SINGLE TimeOffset;		 /* time offset from sync to first pixel */
	SINGLE Gamma;			 /* gamma value */
	SINGLE BlackLevel;		 /* Black level code value */
	SINGLE BlackGain;		 /* Black gain */
	SINGLE Breakpoint;		 /* Breakpoint */
	SINGLE WhiteLevel;		 /* Reference white level code value */
	SINGLE IntegrationTimes; /* Integration time(s) */
	BYTE   Reserved[76];	 /* Padding */
} INDUSTRYTELEVISIONINFOHEADER;

typedef struct _DpxFileFormat
{
	GENERICFILEHEADER			 FileHeader;
	GENERICIMAGEHEADER			 ImageHeader;
	GENERICORIENTATIONHEADER	 OrientHeader;
	INDUSTRYFILMINFOHEADER		 FilmHeader;
	INDUSTRYTELEVISIONINFOHEADER TvHeader;
} DPXFILEFORMAT;

// BMP 文件头
#pragma pack(push, 1)
typedef struct {
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
} BMPFileHeader;
 
// BMP 信息头
typedef struct {
	uint32_t biSize;
	int32_t biWidth;
	int32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t biXPelsPerMeter;
	int32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} BMPInfoHeader;
#pragma pack(pop)

int my_dpx_read(char *fname, char *outputBmpFile)
{
	FILE *fp;
	DWORD magic;
	int bswap = 0;
	DWORD d;
	int i;
	int ecode;
	int w, h;
	DPXFILEFORMAT f;
	int image_offset;
	static int show_range_warn = 1; // Allow range warning for only the first file.
	int bpp;
	int system_is_le;
	int data_bswap;

	if ((fp = fopen(fname, "rb")) == NULL)
	{
		fprintf(stderr, "Error: Cannot read DPX file %s\n", fname); 
		perror("Cannot read DPX file");
		return(DPX_ERROR_BAD_FILENAME);
	}
	if (fread(&magic, 1, 4, fp) != 4)
	{
		return(DPX_ERROR_CORRUPTED_FILE);
	}
	fseek(fp, 0, SEEK_SET);
	system_is_le = (fgetc(fp) == 'S') ^ (magic == 0x53445058);

	if (magic == 0x53445058)
		bswap = 0;
	else if (magic == 0x58504453)
		bswap = 1;
	else
		return(DPX_ERROR_CORRUPTED_FILE);

	data_bswap = bswap;

	fseek(fp, 0, SEEK_SET);
	memset(&f, 0, sizeof(DPXFILEFORMAT));
	if (fread(&f, sizeof(DPXFILEFORMAT), 1, fp) == 0)
	{
		fprintf(stderr, "Error: Tried to read corrupted DPX file %s\n", fname);
		return(DPX_ERROR_CORRUPTED_FILE);
	}
	printf("f.ImageHeader.ImageElement.Descriptor = %d\n", f.ImageHeader.ImageElement[0].Descriptor);
	image_offset = READ_DPX_32(f.FileHeader.ImageOffset);
	w = READ_DPX_32(f.ImageHeader.PixelsPerLine);
	h = READ_DPX_32(f.ImageHeader.LinesPerElement);
	printf("w = %d\n", w);
	printf("h = %d\n", h);
	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	long bytesReadSize = fileSize - image_offset;
	printf("bytesReadSize = %d\n", bytesReadSize);
	unsigned char *buffer = (unsigned char *)malloc(bytesReadSize);
	fseek(fp, image_offset, SEEK_SET);
	fread(buffer, 1, bytesReadSize, fp);
	uint8_t *Data = (uint8_t *)malloc(w * h * 4);
	switch (f.ImageHeader.ImageElement[0].Descriptor)
	{
	case 51:				  /* BGRA */
		for (int y = h - 1; y >= 0; --y) {
			for (int x = 0; x < w; ++x) {
			// 注意：BMP是BGR顺序，所以我们需要交换R和B的位置
				Data[(y * w + x) * 3 + 0] = buffer[((h - 1 - y) * w + x) * 4 + 2]; // B
				Data[(y * w + x) * 3 + 1] = buffer[((h - 1 - y) * w + x) * 4 + 1]; // G
				Data[(y * w + x) * 3 + 2] = buffer[((h - 1 - y) * w + x) * 4 + 0]; // R
			}
		}
 
		// 设置BMP文件头和信息头（注意：这里假设不使用压缩，并且图像大小是w*h*3字节）
		BMPFileHeader fileHeader;
		BMPInfoHeader infoHeader;
		fileHeader.bfType = 0x4D42; // 'BM'
		fileHeader.bfReserved1 = 0;
		fileHeader.bfReserved2 = 0;
		fileHeader.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
	 
		infoHeader.biSize = sizeof(BMPInfoHeader);
		infoHeader.biWidth = w;
		infoHeader.biHeight = h; // 注意：对于24位BMP，biHeight应该是正数，表示图像高度
		infoHeader.biPlanes = 1;
		infoHeader.biBitCount = 24; // 24位BGR
		infoHeader.biCompression = 0; // 不压缩
		infoHeader.biSizeImage = w * h * 3; // 图像数据大小（字节）
		infoHeader.biXPelsPerMeter = 0; // 水平分辨率（像素/米），通常设置为0
		infoHeader.biYPelsPerMeter = 0; // 垂直分辨率（像素/米），通常设置为0
		infoHeader.biClrUsed = 0; // 实际使用的颜色表中的颜色数，对于24位图像通常为0
		infoHeader.biClrImportant = 0; // 重要颜色数，对于24位图像通常为0
	 
		fileHeader.bfSize = fileHeader.bfOffBits + infoHeader.biSizeImage; // 文件总大小（字节）
	 
		// 写入BMP文件
		FILE *bmpFile = fopen(outputBmpFile, "wb");
		if (!bmpFile) {
			fprintf(stderr, "Error: The file '%s' could not be created.\n", outputBmpFile);
			free(Data);
			return 1;
		}
	 
		fwrite(&fileHeader, sizeof(BMPFileHeader), 1, bmpFile);
		fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, bmpFile);
		fwrite(Data, 1, w * h * 3, bmpFile); // 写入BGR数据（没有alpha通道）
	 
		fclose(bmpFile);
		free(Data);
		break;
		default:
			return(DPX_ERROR_NOT_IMPLEMENTED);
	}
	free(buffer);
	fclose(fp);
}

int main()
{
	char *inputFile = "input.dpx";
	char *outputBmpFile = "output.bmp";
	my_dpx_read(inputFile, outputBmpFile);
	return 0;
}
