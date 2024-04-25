#define MAIN_FORM CoasterSimulator  // name of the form that will open when program runs
#define PROJECT_NAME ps7coasterng27sp24 // name of the project (with underscores removed) (see line 3 of form's .h file)
#include "CoasterSimulator.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(cli::array <String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	PROJECT_NAME::MAIN_FORM form;
	Application::Run(% form);
	return 0;
}

#undef MAIN_FORM
#undef PROJECT_NAME