
#include "graphics\Context.h"

int main()
{
	Context context;
	context.init();

	while (context.step())
	{
		
	}
	context.term();
	return 0;
}