/**
 * Copyright (c) 2012-2016 Moodstocks SAS
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "jpec.h"

/* Global variables */
const char *pname;

/* Function prototypes */
int main(int argc, char **argv);
uint8_t *load_image(const char *path, int *width, int *height);

/* implementation */
int main(int argc, char** argv) {
  pname = argv[0];
  if (argc == 2) {
    int w, h;
    uint8_t *img = load_image(argv[1], &w, &h);
    jpec_enc_t *e = jpec_enc_new(img, w, h);
    int len;
    const uint8_t *jpeg = jpec_enc_run(e, &len);
    FILE *file = fopen("result.jpg", "wb");
    fwrite(jpeg, sizeof(uint8_t), len, file);
    fclose(file);
    printf("Done: result.jpg (%d bytes)\n", len);
    jpec_enc_del(e);
    free(img);
  }
  else {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s /path/to/img\n", pname);
    exit(1);
  }
  return 0;
}

uint8_t *load_image(const char *path, int *width, int *height) {
  assert(path);
  IplImage *img = cvLoadImage(path, 1);
  if (!img) return NULL;
  if (img->nChannels != 1) {
    IplImage *tmp = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
    cvCvtColor(img, tmp, CV_BGR2GRAY);
    cvReleaseImage(&img);
    img = tmp;
  }
  int w = img->width;
  int h = img->height;
  int bpr = img->widthStep;
  uint8_t *data = (uint8_t*)malloc(w*h*sizeof(uint8_t));
  if (w == bpr) memcpy(data, img->imageData, w*h*sizeof(uint8_t));
  else {
    for (int i = 0; i < h; ++i) {
      memcpy(data + i*w*sizeof(uint8_t), img->imageData + i*bpr*sizeof(uint8_t),
             w*sizeof(uint8_t));
    }
  }
  cvReleaseImage(&img);
  *width = w;
  *height = h;
  return data;
}
