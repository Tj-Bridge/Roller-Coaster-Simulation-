#pragma once
#include "Path2D.h"

namespace ps7coasterng27sp24 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MajorEdits
	/// </summary>
	public ref class MajorEdits : public System::Windows::Forms::Form
	{
	public:
		MajorEdits(Path2D* aPath, System::Windows::Forms::Panel^ aPanel) : MajorEdits() {
			thePath = aPath;
			thePanel = aPanel;
		}
		MajorEdits(void)
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
		~MajorEdits()
		{
			if (components)
			{
				delete components;
			}
		}

		Path2D* thePath = nullptr;
		System::Windows::Forms::Panel^ thePanel;

	private: System::Windows::Forms::TextBox^ textBoxMoveTrack;
	protected:


	private: System::Windows::Forms::Label^ label2;


	private: System::Windows::Forms::TextBox^ textBoxRotateTrack;
	private: System::Windows::Forms::Label^ label1;


	private: System::Windows::Forms::TextBox^ textBoxScaleTrack;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Button^ buttonMoveTrack;
	private: System::Windows::Forms::Button^ buttonScaleTrack;

	private: System::Windows::Forms::Button^ buttonRotateTrack;
	private: System::Windows::Forms::Button^ buttonMirrorTrackUpDown;
	private: System::Windows::Forms::Button^ buttonMirrorTrackLeftRight;



	private: System::Windows::Forms::Label^ label4;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBoxMoveTrack = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->buttonMoveTrack = (gcnew System::Windows::Forms::Button());
			this->textBoxScaleTrack = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->buttonScaleTrack = (gcnew System::Windows::Forms::Button());
			this->textBoxRotateTrack = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->buttonRotateTrack = (gcnew System::Windows::Forms::Button());
			this->buttonMirrorTrackUpDown = (gcnew System::Windows::Forms::Button());
			this->buttonMirrorTrackLeftRight = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// textBoxMoveTrack
			// 
			this->textBoxMoveTrack->Location = System::Drawing::Point(87, 38);
			this->textBoxMoveTrack->Name = L"textBoxMoveTrack";
			this->textBoxMoveTrack->Size = System::Drawing::Size(60, 20);
			this->textBoxMoveTrack->TabIndex = 1;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(23, 42);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(58, 13);
			this->label2->TabIndex = 12;
			this->label2->Text = L"Delta: X, Y";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// buttonMoveTrack
			// 
			this->buttonMoveTrack->Location = System::Drawing::Point(153, 38);
			this->buttonMoveTrack->Name = L"buttonMoveTrack";
			this->buttonMoveTrack->Size = System::Drawing::Size(50, 20);
			this->buttonMoveTrack->TabIndex = 13;
			this->buttonMoveTrack->Text = L"Move";
			this->buttonMoveTrack->UseVisualStyleBackColor = true;
			this->buttonMoveTrack->Click += gcnew System::EventHandler(this, &MajorEdits::buttonMoveTrack_Click);
			// 
			// textBoxScaleTrack
			// 
			this->textBoxScaleTrack->Location = System::Drawing::Point(87, 73);
			this->textBoxScaleTrack->Name = L"textBoxScaleTrack";
			this->textBoxScaleTrack->Size = System::Drawing::Size(60, 20);
			this->textBoxScaleTrack->TabIndex = 1;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(21, 77);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(60, 13);
			this->label1->TabIndex = 12;
			this->label1->Text = L"Scale: X, Y";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// buttonScaleTrack
			// 
			this->buttonScaleTrack->Location = System::Drawing::Point(153, 73);
			this->buttonScaleTrack->Name = L"buttonScaleTrack";
			this->buttonScaleTrack->Size = System::Drawing::Size(50, 20);
			this->buttonScaleTrack->TabIndex = 13;
			this->buttonScaleTrack->Text = L"Scale";
			this->buttonScaleTrack->UseVisualStyleBackColor = true;
			this->buttonScaleTrack->Click += gcnew System::EventHandler(this, &MajorEdits::buttonScaleTrack_Click);
			// 
			// textBoxRotateTrack
			// 
			this->textBoxRotateTrack->Location = System::Drawing::Point(87, 108);
			this->textBoxRotateTrack->Name = L"textBoxRotateTrack";
			this->textBoxRotateTrack->Size = System::Drawing::Size(60, 20);
			this->textBoxRotateTrack->TabIndex = 1;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(13, 111);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(68, 13);
			this->label3->TabIndex = 12;
			this->label3->Text = L"Angle (CCW)";
			this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// buttonRotateTrack
			// 
			this->buttonRotateTrack->Location = System::Drawing::Point(153, 108);
			this->buttonRotateTrack->Name = L"buttonRotateTrack";
			this->buttonRotateTrack->Size = System::Drawing::Size(50, 20);
			this->buttonRotateTrack->TabIndex = 13;
			this->buttonRotateTrack->Text = L"Rotate";
			this->buttonRotateTrack->UseVisualStyleBackColor = true;
			this->buttonRotateTrack->Click += gcnew System::EventHandler(this, &MajorEdits::buttonRotateTrack_Click);
			// 
			// buttonMirrorTrackUpDown
			// 
			this->buttonMirrorTrackUpDown->Location = System::Drawing::Point(236, 38);
			this->buttonMirrorTrackUpDown->Name = L"buttonMirrorTrackUpDown";
			this->buttonMirrorTrackUpDown->Size = System::Drawing::Size(69, 42);
			this->buttonMirrorTrackUpDown->TabIndex = 13;
			this->buttonMirrorTrackUpDown->Text = L"Mirror Up-Down";
			this->buttonMirrorTrackUpDown->UseVisualStyleBackColor = true;
			this->buttonMirrorTrackUpDown->Click += gcnew System::EventHandler(this, &MajorEdits::buttonMirrorTrackUpDown_Click);
			// 
			// buttonMirrorTrackLeftRight
			// 
			this->buttonMirrorTrackLeftRight->Location = System::Drawing::Point(236, 90);
			this->buttonMirrorTrackLeftRight->Name = L"buttonMirrorTrackLeftRight";
			this->buttonMirrorTrackLeftRight->Size = System::Drawing::Size(69, 38);
			this->buttonMirrorTrackLeftRight->TabIndex = 13;
			this->buttonMirrorTrackLeftRight->Text = L"Mirror Left-Right";
			this->buttonMirrorTrackLeftRight->UseVisualStyleBackColor = true;
			this->buttonMirrorTrackLeftRight->Click += gcnew System::EventHandler(this, &MajorEdits::buttonMirrorTrackLeftRight_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(7, 9);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(317, 16);
			this->label4->TabIndex = 12;
			this->label4->Text = L"Use this dialog to make major changes to track path.";
			// 
			// MajorEdits
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)),
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->ClientSize = System::Drawing::Size(326, 143);
			this->Controls->Add(this->buttonMirrorTrackLeftRight);
			this->Controls->Add(this->buttonMirrorTrackUpDown);
			this->Controls->Add(this->buttonRotateTrack);
			this->Controls->Add(this->buttonScaleTrack);
			this->Controls->Add(this->buttonMoveTrack);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBoxRotateTrack);
			this->Controls->Add(this->textBoxScaleTrack);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->textBoxMoveTrack);
			this->Name = L"MajorEdits";
			this->Text = L"MajorEdits";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MajorEdits::MajorEdits_FormClosed);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: float getFloatFromBox(TextBox^ currBox) {
		String^ boxName = currBox->Name;
		try {
			return Convert::ToSingle(currBox->Text); // could generate exception
		}
		catch (Exception^ e) {
			MessageBox::Show(e->Message + "\n\nUnable to get valid value from " + boxName);
			currBox->Focus();
			return -INFINITY;
		}
	}
	private: Point2D getCoordsFromBox(TextBox^ currBox) {
		String^ boxName = currBox->Name;
		try {
			int commaLoc = currBox->Text->IndexOf(",");
			float newX = Convert::ToSingle(currBox->Text->Substring(0, commaLoc));
			float newY = Convert::ToSingle(currBox->Text->Substring(commaLoc + 1));
			return { newX, newY };
		}
		catch (Exception^ e) {
			MessageBox::Show(e->Message + "\n\nUnable to get coordinates from " + boxName);
			currBox->Focus();
			return { -INFINITY, -INFINITY };
		}
	}

	private: System::Void buttonMirrorTrackUpDown_Click(System::Object^ sender, System::EventArgs^ e) {
		auto centerPnt = thePath->center();
		thePath->mirror(centerPnt, { centerPnt.x + 100, centerPnt.y });
		thePanel->Refresh();
	}
	private: System::Void buttonMirrorTrackLeftRight_Click(System::Object^ sender, System::EventArgs^ e) {
		auto centerPnt = thePath->center();
		thePath->mirror(centerPnt, { centerPnt.x, centerPnt.y + 100 });
		thePanel->Refresh();
	}
	private: System::Void buttonMoveTrack_Click(System::Object^ sender, System::EventArgs^ e) {
		Point2D inputPnt = getCoordsFromBox(textBoxMoveTrack);
		if (inputPnt.x != -INFINITY) {
			thePath->moveAllPoints(inputPnt);
			thePanel->Refresh();
		}
	}
	private: System::Void buttonScaleTrack_Click(System::Object^ sender, System::EventArgs^ e) {
		Point2D inputPnt = getCoordsFromBox(textBoxScaleTrack);
		if (inputPnt.x != -INFINITY) {
			thePath->scale(thePath->center(), inputPnt.x, inputPnt.y);
			thePanel->Refresh();
		}
	}
	private: System::Void buttonRotateTrack_Click(System::Object^ sender, System::EventArgs^ e) {
		float inputAngle = getFloatFromBox(textBoxRotateTrack);
		if (inputAngle != -INFINITY) {
			thePath->rotate(thePath->center(), inputAngle);
			thePanel->Refresh();
		}
	}
	private: System::Void MajorEdits_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e) {

	}
};
}
