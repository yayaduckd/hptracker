
#pragma once

static const uint8_t SPRITE_HP_COUNTOUR_WIDTH = 70;
static const uint8_t SPRITE_HP_COUNTOUR_HEIGHT = 10;
static const uint8_t SPRITE_HP_COUNTOUR_DATA[] = {
  0x77, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 
  0xdd, 0xef, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 
  0x77, 0xbf, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9d, 
  0xde, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x77, 
  0x77, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1c, 
  0x3f, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0x77, 
  0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9d, 0xdf, 
  0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x77, 0x7f, 
  0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xdd, 0xff, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x77, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff
  };

#define PXART_HEIGHT 20
#define PXART_WIDTH 20

// array size is 50
static const uint8_t DNDLOGO_WIDTH = 20;
static const uint8_t DNDLOGO_HEIGHT = 20;
static const uint8_t DNDLOGO_DATA[] = {
  0xff, 0xff, 0xff, 
  0xff, 0xff, 0xf8, 
  0x7f, 0xff, 0x03, 
  0xff, 0xf3, 0x1f, 
  0xff, 0xa1, 0xff, 
  0xf5, 0xbf, 0xff, 
  0x03, 0xff, 0xf0, 
  0x38, 0x3f, 0x43, 
  0x5f, 0xd8, 0x93, 
  0xfd, 0x28, 0x7f, 
  0xc7, 0x8f, 0xfc, 
  0x78, 0xff, 0xc7, 
  0xa1, 0xbc, 0x31, 
  0x13, 0xc0, 0x30, 
  0x3f, 0x07, 0x07, 
  0xfd, 0xfc, 0xff, 
  0xff, 0xff
};


  const uint8_t mlemarray [] = {
// 'Sprite-0001', 84x48px
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xeb, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xfe, 0x9f, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xed, 0xf9, 0x7f, 0xbf, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xfe, 0xcf, 0x37, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xee, 0xf7, 0x7f, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xfc, 0xe6, 0x73, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xdf, 0x6f, 0xbf, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xfd, 0xf0, 0xfb, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xdf, 0x9f, 0xbf, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 
  0xf9, 0xfb, 0xfd, 0xff, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 
  0xff, 0x9f, 0xdf, 0xe2, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xff, 
  0xfd, 0xfd, 0xfe, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 
  0xdf, 0xdf, 0xcf, 0x7f, 0xfb, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xfd, 
  0xfd, 0xfd, 0xc7, 0xff, 0xbe, 0x0f, 0xc0, 0xff, 0xdf, 0xff, 0xdf, 
  0xde, 0x91, 0xff, 0xf9, 0xce, 0xe1, 0xef, 0xfd, 0xff, 0xfd, 0xfd, 
  0xe0, 0x7f, 0xff, 0xd3, 0xe8, 0xff, 0xff, 0xdf, 0xff, 0xdf, 0xdf, 
  0xbf, 0xff, 0xfd, 0x7e, 0x3f, 0xff, 0xfd, 0xff, 0xfc, 0xfd, 0xf9, 
  0xff, 0xef, 0xcf, 0xef, 0xff, 0xff, 0xdf, 0xff, 0xef, 0xdf, 0xdf, 
  0xfc, 0xfc, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xfe, 0xfd, 0xfc, 0x7e, 
  0x1f, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xef, 0xff, 0xf0, 0x0f, 
  0xff, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
