#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
 * Include file for users of JPEG library.
 * You will need to have included system headers that define at least
 * the typedefs FILE and size_t before you can include jpeglib.h.
 * (stdio.h is sufficient on ANSI-conforming systems.)
 * You may also wish to include "jerror.h".
 */
 
#include "jpeglib.h"

/*
 * <setjmp.h> is used for the optional error recovery mechanism shown in
 * the second part of the example.
 */
#include <setjmp.h>

struct my_error_mgr
{
  struct jpeg_error_mgr pub;  /* "public" fields */

  jmp_buf setjmp_buffer;  /* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

void my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}
int decode_jpeg_file (const char *srcfile, const char *optfile)
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  FILE *infile, *outfile;
  JSAMPARRAY buffer;      /* Output row buffer */
  int row_stride;     /* physical row width in output buffer */

  int finished = 1;
  int chromaWidth, chromaHeight;
  int yMask,xMask;

  int x,y;
  int width,height;

  unsigned char *pixels,*rgbPixels, *src;
  unsigned char *yPixels, *uPixels, *vPixels;
  unsigned char *yPtr, *uPtr, *vPtr;

  if ((infile = fopen(srcfile, "rb")) == NULL)
  {
    fprintf(stderr, "Can't open %s\n", srcfile);
    return -1;
  }
  if ((outfile = fopen(optfile, "wb")) == NULL)
  {
    fprintf(stderr, "Can't open %s\n", optfile);
    return -1;
  }

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer))
  {
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return -1;
  }
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  (void) jpeg_read_header(&cinfo, TRUE);

  /*set parameters for decompression */
  cinfo.out_color_space = JCS_YCbCr;

  (void) jpeg_start_decompress(&cinfo);

  width  = cinfo.output_width;
  height = cinfo.output_height;

  pixels = (unsigned char *)malloc(width*height*3);
  memset(pixels, 0, width * height * 3);

  src = rgbPixels = pixels;

  /* JSAMPLEs per row in output buffer */
  row_stride = cinfo.output_width * cinfo.output_components;
  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (*cinfo.mem->alloc_sarray)
           ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
  while (cinfo.output_scanline < cinfo.output_height)
  {
    int num_rows = jpeg_read_scanlines(&cinfo, buffer, 1);
    if (num_rows == 0)
    {
      finished = 0;
      break;
    }
    if (cinfo.output_components == 1)          // greyscale
    {
      unsigned int i;
      unsigned char *in = (unsigned char *)(*buffer);
      for (i = 0; i < width * num_rows; ++i)
      {
        *pixels++ = *in; // red
        *pixels++ = *in; // green
        *pixels++ = *in; // blue
        ++in;
      }
    }
    else if (cinfo.output_components == 3)     // RGB
    {
      memcpy(pixels, (*buffer), num_rows * width * 3);
      pixels += num_rows * width * 3;
    }
  }

  chromaWidth = width / 2;
  chromaHeight = height / 2;
  yMask = xMask = 1;

  yPixels = (unsigned char*)malloc(height * width);
  uPixels = (unsigned char*)malloc(chromaWidth * chromaHeight);
  vPixels = (unsigned char*)malloc(chromaWidth * chromaHeight);

  yPtr = yPixels;
  uPtr = uPixels;
  vPtr = vPixels;

  for(y = 0; y < height; y++)
  {
    for(x = 0; x < width; x++)
    {
      *yPtr++ = *rgbPixels++;
      if((y & yMask) == 0 && (x & xMask) == 0)
      {
        *uPtr++ = *rgbPixels++;
        *vPtr++ = *rgbPixels++;
      }
      else
        rgbPixels += 2;
    }
  }
  if(finished)
  {
    (void) jpeg_finish_decompress(&cinfo);
  }
  jpeg_destroy_decompress(&cinfo);
  fwrite(yPixels, 1, width * height, outfile);
  fwrite(uPixels, 1, chromaWidth * chromaHeight, outfile);
  fwrite(vPixels, 1, chromaWidth * chromaHeight, outfile);

  fclose(infile);
  fclose(outfile);

  free(src);
  //free(yPixels);
  //free(uPixels);
  //free(vPixels);

  return 0;
}

int main(int argc, char *argv[])
{
  if(argc < 3)
  {
    decode_jpeg_file("test.jpg","yuv.bin");
  }
  else
  {
    decode_jpeg_file(argv[1],argv[2]);
  }

  return 0;
}
