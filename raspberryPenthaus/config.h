#ifndef IMPLEMENTATION
#define IMPLEMENTATION

//#define SYMULATOR		//<= ODKOMECTOWAC ABY KOMPILOWAC SYMULATOR
#define LOG				//<= ODKOMECTOWAC ABY UZYSKAC LOGI NA KONSOLI

#ifndef SYMULATOR
static const unsigned int BASE_INTERVAL = 50U;
static const unsigned int MOTION_INTERVAL = 50U;
static const unsigned int LIGHT_INTERVAL = 1000U;
static const char* CLEAR_SCREEN = "clear";

//#define PRINTING_INTERVAL 500

#else
static const unsigned int BASE_INTERVAL = 50U;
static const unsigned int MOTION_INTERVAL = 1000U;
static const unsigned int LIGHT_INTERVAL = 1000U;
//static const unsigned int PRINTING_INTERVAL = 500U;
static const char* CLEAR_SCREEN = "cls";
#endif

#endif // !IMPLEMENTATION
