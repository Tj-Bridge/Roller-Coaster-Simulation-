#pragma once
#include "Track.h"

namespace ps7coasterng27sp24 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for ControlEdits
	/// </summary>
	public ref class ControlEdits : public System::Windows::Forms::Form
	{
	public:
		ControlEdits(Track* aTrack, System::Windows::Forms::Panel^ aPanel) : ControlEdits() {
			theTrack = aTrack;
			thePanel = aPanel;
		}
		ControlEdits(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ControlEdits()
		{
			if (components)
			{
				delete components;
			}
		}

		Track* theTrack = nullptr;
		System::Windows::Forms::Panel^ thePanel;
	private: System::Windows::Forms::Button^ buttonConfirm;
	private: System::Windows::Forms::ComboBox^ comboBox1;
	private: System::Windows::Forms::Label^ label1;
	protected:

	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->buttonConfirm = (gcnew System::Windows::Forms::Button());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// buttonConfirm
			// 
			this->buttonConfirm->BackColor = System::Drawing::Color::LightCyan;
			this->buttonConfirm->Location = System::Drawing::Point(242, 488);
			this->buttonConfirm->Name = L"buttonConfirm";
			this->buttonConfirm->Size = System::Drawing::Size(225, 73);
			this->buttonConfirm->TabIndex = 0;
			this->buttonConfirm->Text = L"Confirm";
			this->buttonConfirm->UseVisualStyleBackColor = false;
			this->buttonConfirm->Click += gcnew System::EventHandler(this, &ControlEdits::buttonConfirm_Click);
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(242, 231);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 28);
			this->comboBox1->TabIndex = 1;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->label1->Location = System::Drawing::Point(200, 62);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(265, 20);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Use this window to edit the Controls!";
			// 
			// ControlEdits
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::SeaGreen;
			this->ClientSize = System::Drawing::Size(840, 676);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->buttonConfirm);
			this->Name = L"ControlEdits";
			this->Text = L"ControlEdits";
			this->Load += gcnew System::EventHandler(this, &ControlEdits::ControlEdits_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void ControlEdits_Load(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void buttonConfirm_Click(System::Object^ sender, System::EventArgs^ e) {

	}

	};
}
