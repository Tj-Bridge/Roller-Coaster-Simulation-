#pragma once
ref class DoubleBufferedPanel : public System::Windows::Forms::Panel
{

public: DoubleBufferedPanel()
{
	this->DoubleBuffered = true;
}
};
