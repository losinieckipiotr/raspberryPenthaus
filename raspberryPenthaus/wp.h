#ifndef IMPLEMENTATION
#define IMPLEMENTATION

//#define WP			//<= ODKOMECTOWAC PRZY KOMPILACJI NA RASPBERRY!
//#define STATS

#ifdef WP
#include <wiringPi.h>
static const unsigned int CHECKING_INTERVAL = 1000U;
static const unsigned int LIGHT_INTERVAL = 1U;
static const char* CLEAR_SCREEN __attribute__((unused)) = "clear";

//#define PRINTING_INTERVAL 500

#else
static const unsigned int CHECKING_INTERVAL = 1000U;
static const unsigned int LIGHT_INTERVAL = 1U;
//static const unsigned int PRINTING_INTERVAL = 500U;
static const char* CLEAR_SCREEN = "cls";
#endif

#endif // !IMPLEMENTATION
