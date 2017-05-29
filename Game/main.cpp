
#include "ffpython.h"
#include "direct.h"
#include "vm_fake.h"


#define  TestGuard(X, Y) printf("-------%s begin-----------\n", X);\
						 try {Y;}\
						 catch(exception& e_)\
						 {printf("exception<%s>\n", e_.what());}\
						 printf("-------%s end-----------\n", X);
typedef string ret;

ret TEST(ffpython_t& ffpython)
{
	ret chr=ffpython.call<ret>("game", "change");
	return chr;
}

int main()
{
	
	Py_Initialize();
	ffpython_t::add_path("./");
	ffpython_t ffpython;
	ret input;
	if (_mkdir("C:/Python27/Lib") != 0)
		printf("Go On\n");
	TestGuard("game",input=TEST(ffpython));
	if (input == "")
		return 0;
	run(input.c_str());

	return 0;
}