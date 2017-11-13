#include <stdlib.h>
#include "Manage.h"

using Sdalin::manage;
int main()
{
	manage.insertStudent("123456", "aassdd");
	manage.insertStudent("123457", "aassdd");
	manage.deleteStudentFromID("123456");
	system("pause");
	return 0;
}
