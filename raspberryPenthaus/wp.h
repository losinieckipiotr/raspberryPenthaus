#ifndef IMPLEMENTATION
#define IMPLEMENTATION

//#define WP			//<= ODKOMECTOWAC PRZY KOMPILACJI NA RASPBERRY!
//#define STATS

#ifdef WP
#include <wiringPi.h>
static const unsigned int BASE_INTERVAL = 50U;
static const unsigned int MOTION_INTERVAL = 50U;
static const unsigned int LIGHT_INTERVAL = 1000U;
static const char* CLEAR_SCREEN __attribute__((unused)) = "clear";

//#define PRINTING_INTERVAL 500

#else
static const unsigned int BASE_INTERVAL = 1000U;
static const unsigned int MOTION_INTERVAL = 1000U;
static const unsigned int LIGHT_INTERVAL = 5000U;
//static const unsigned int PRINTING_INTERVAL = 500U;
static const char* CLEAR_SCREEN = "cls";
#endif

#endif // !IMPLEMENTATION
