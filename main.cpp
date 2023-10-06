#include "logger.h"

int main()
{
	using enum Verbosity;
	log("this logger implementation has possibility to create more than one logger.", Info);
	log("file and console logger are created as default.", Info);
	log("that's why this message will be written to both console and file.", Warning);
	log("If you set Logger class, you can log the message wherever you want.", Error);
}