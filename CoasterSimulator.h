#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>

#include "StringPlus.h"  // for reasons having to do with marshall, this include must go first
#include "Path2D.h"
#include "CubicSpline2D.h"
#include "ColorConverter.h"
#include "Track.h"
#include "Train.h"
#include "MajorEdits.h"
#include "ControlEdits1.h"

#include "DoubleBufferedPanel.h"
// using double buffered panel makes the form design view not work.
// to use double bufferedpanel or allow the design view, use one of these
// in InitComponents() function (place bookmark there)
//    for design view   >>>>    this->mainPanel = (gcnew System::Windows::Forms::Panel());
//    for double buffer >>>>    this->mainPanel = (gcnew DoubleBufferedPanel());


namespace ps7coasterng27sp24 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for CoasterSimulator
	/// </summary>
	public ref class CoasterSimulator : public System::Windows::Forms::Form
	{
	public:
		CoasterSimulator(void)
		{
			initializeHoverHelp();
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			labelHoverBox->Left = -1000;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CoasterSimulator()
		{
			// if simulation is running, stop it to avoid errors on exit
			if (continueSimulation) { 
				continueSimulation = false;  // force thread to quit "naturally"
				simulationThread->Join(); // wait until thread is actually done
			}

			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Button^ buttonLoadPath;

	private: System::Windows::Forms::Button^ buttonSavePath;
	private: System::Windows::Forms::Panel^ mainPanel;
	private: System::Windows::Forms::Label^ labelFeedback;

	protected:  // stuff I coded
		//Path2D* thePath = nullptr; // eliminated this "shortcut"

		// main data variables
		Track* theTrack = nullptr;
		Train* theTrain = nullptr;

		int panX = 300, panY = 300, currX, currY;
		float zoomLevel = 1.f;
		bool mouseInPanel = false;
		Hashtable^ hoverResponses;
		bool inEditMode = false;
		int pointToMove = -1;
		int currCarIndex = -1;

		MajorEdits^ majorEditsForm;
		ControlEdits^ controlEditsForm;

		// simulation management
		bool continueSimulation = false;
		System::Threading::Thread^ simulationThread;


	private: System::Windows::Forms::Button^ buttonResetView;
	private: System::Windows::Forms::Label^ labelLineColor;
	private: System::Windows::Forms::CheckBox^ checkBoxPoints;
	private: System::Windows::Forms::CheckBox^ checkBoxLabels;

	private: System::Windows::Forms::TextBox^ textBoxPointData;
	private: System::Windows::Forms::Button^ buttonMovePoint;

	private: System::Windows::Forms::Button^ buttonAddPoint;
	private: System::Windows::Forms::Button^ buttonRemovePoint;
	private: System::Windows::Forms::Label^ label2;


	private: System::Windows::Forms::CheckBox^ checkBoxEditMode;
	private: System::Windows::Forms::Button^ buttonLoadTrack;
	private: System::Windows::Forms::Button^ buttonSaveTrack;


	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ labelHoverBox;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::HScrollBar^ hScrollBarTrackWidth;

	private: System::Windows::Forms::TextBox^ textBoxTrackWidth;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::Label^ label10;



	private: System::Windows::Forms::Label^ labelMaxDrop;




	private: System::Windows::Forms::CheckBox^ checkBoxGrid;
	private: System::Windows::Forms::CheckBox^ checkBoxScale;
	private: System::Windows::Forms::Label^ labelMaxRise;
	private: System::Windows::Forms::Label^ labelTrackHeight;
	private: System::Windows::Forms::Label^ labelTrackLength;
	private: System::Windows::Forms::TextBox^ textBoxTrackName;
	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::Label^ label12;
	private: System::Windows::Forms::TextBox^ textBoxTrackFriction;
	private: System::Windows::Forms::ComboBox^ comboBoxControls;

	private: System::Windows::Forms::Label^ label13;
private: System::Windows::Forms::TextBox^ textBoxFinalVelocity;


	private: System::Windows::Forms::TextBox^ textBoxStartLoc;



	private: System::Windows::Forms::TextBox^ textBoxEndLoc;

	private: System::Windows::Forms::Label^ label14;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label15;
	private: System::Windows::Forms::Label^ label16;
	private: System::Windows::Forms::Button^ buttonDeleteControl;
	private: System::Windows::Forms::Button^ buttonAddControl;
	private: System::Windows::Forms::Button^ buttonNewTrack;
	private: System::Windows::Forms::Button^ buttonNewPath;

	private: System::Windows::Forms::Button^ buttonInitTrain;






	private: System::Windows::Forms::Label^ label18;




	private: System::Windows::Forms::Button^ buttonSimStartStop;

	private: System::Windows::Forms::Button^ buttonLoadTrain;


	private: System::Windows::Forms::Button^ buttonSaveTrain;

	private: System::Windows::Forms::Label^ label19;
	private: System::Windows::Forms::Button^ buttonMajorEdits;
private: System::Windows::Forms::Label^ label17;
private: System::Windows::Forms::Label^ labelCurrCar;
private: System::Windows::Forms::Button^ buttonSaveCar;
private: System::Windows::Forms::Button^ buttonAddCarFront;
private: System::Windows::Forms::Button^ buttonDeleteCar;
private: System::Windows::Forms::Button^ buttonAddCarRear;
private: System::Windows::Forms::Button^ buttonControlEdit;











	protected:

	protected:


	protected:

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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->buttonLoadPath = (gcnew System::Windows::Forms::Button());
			this->buttonSavePath = (gcnew System::Windows::Forms::Button());
			this->mainPanel = (gcnew System::Windows::Forms::Panel());
			this->labelFeedback = (gcnew System::Windows::Forms::Label());
			this->buttonResetView = (gcnew System::Windows::Forms::Button());
			this->labelLineColor = (gcnew System::Windows::Forms::Label());
			this->checkBoxPoints = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxLabels = (gcnew System::Windows::Forms::CheckBox());
			this->textBoxPointData = (gcnew System::Windows::Forms::TextBox());
			this->buttonMovePoint = (gcnew System::Windows::Forms::Button());
			this->buttonAddPoint = (gcnew System::Windows::Forms::Button());
			this->buttonRemovePoint = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->checkBoxEditMode = (gcnew System::Windows::Forms::CheckBox());
			this->buttonLoadTrack = (gcnew System::Windows::Forms::Button());
			this->buttonSaveTrack = (gcnew System::Windows::Forms::Button());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->labelHoverBox = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->hScrollBarTrackWidth = (gcnew System::Windows::Forms::HScrollBar());
			this->textBoxTrackWidth = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->labelMaxDrop = (gcnew System::Windows::Forms::Label());
			this->checkBoxGrid = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxScale = (gcnew System::Windows::Forms::CheckBox());
			this->labelMaxRise = (gcnew System::Windows::Forms::Label());
			this->labelTrackHeight = (gcnew System::Windows::Forms::Label());
			this->labelTrackLength = (gcnew System::Windows::Forms::Label());
			this->textBoxTrackName = (gcnew System::Windows::Forms::TextBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->textBoxTrackFriction = (gcnew System::Windows::Forms::TextBox());
			this->comboBoxControls = (gcnew System::Windows::Forms::ComboBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->textBoxFinalVelocity = (gcnew System::Windows::Forms::TextBox());
			this->textBoxStartLoc = (gcnew System::Windows::Forms::TextBox());
			this->textBoxEndLoc = (gcnew System::Windows::Forms::TextBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->buttonDeleteControl = (gcnew System::Windows::Forms::Button());
			this->buttonAddControl = (gcnew System::Windows::Forms::Button());
			this->buttonNewTrack = (gcnew System::Windows::Forms::Button());
			this->buttonNewPath = (gcnew System::Windows::Forms::Button());
			this->buttonInitTrain = (gcnew System::Windows::Forms::Button());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->buttonSimStartStop = (gcnew System::Windows::Forms::Button());
			this->buttonLoadTrain = (gcnew System::Windows::Forms::Button());
			this->buttonSaveTrain = (gcnew System::Windows::Forms::Button());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->buttonMajorEdits = (gcnew System::Windows::Forms::Button());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->labelCurrCar = (gcnew System::Windows::Forms::Label());
			this->buttonSaveCar = (gcnew System::Windows::Forms::Button());
			this->buttonAddCarFront = (gcnew System::Windows::Forms::Button());
			this->buttonDeleteCar = (gcnew System::Windows::Forms::Button());
			this->buttonAddCarRear = (gcnew System::Windows::Forms::Button());
			this->buttonControlEdit = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Cooper Black", 30, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::Color::Yellow;
			this->label1->Location = System::Drawing::Point(13, 9);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(454, 69);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Magic Coaster";
			// 
			// buttonLoadPath
			// 
			this->buttonLoadPath->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonLoadPath->Location = System::Drawing::Point(1354, 529);
			this->buttonLoadPath->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->buttonLoadPath->Name = L"buttonLoadPath";
			this->buttonLoadPath->Size = System::Drawing::Size(75, 31);
			this->buttonLoadPath->TabIndex = 1;
			this->buttonLoadPath->Text = L"Load";
			this->buttonLoadPath->UseVisualStyleBackColor = true;
			this->buttonLoadPath->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonLoad_Click);
			this->buttonLoadPath->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->buttonLoadPath->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// buttonSavePath
			// 
			this->buttonSavePath->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonSavePath->Location = System::Drawing::Point(1431, 529);
			this->buttonSavePath->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->buttonSavePath->Name = L"buttonSavePath";
			this->buttonSavePath->Size = System::Drawing::Size(75, 31);
			this->buttonSavePath->TabIndex = 1;
			this->buttonSavePath->Text = L"Save";
			this->buttonSavePath->UseVisualStyleBackColor = true;
			this->buttonSavePath->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonSave_Click);
			this->buttonSavePath->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->buttonSavePath->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// mainPanel
			// 
			this->mainPanel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->mainPanel->BackColor = System::Drawing::SystemColors::Window;
			this->mainPanel->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->mainPanel->Location = System::Drawing::Point(182, 91);
			this->mainPanel->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->mainPanel->Name = L"mainPanel";
			this->mainPanel->Size = System::Drawing::Size(1159, 709);
			this->mainPanel->TabIndex = 2;
			this->mainPanel->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &CoasterSimulator::mainPanel_Paint);
			this->mainPanel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &CoasterSimulator::mainPanel_MouseDown);
			this->mainPanel->MouseEnter += gcnew System::EventHandler(this, &CoasterSimulator::mainPanel_MouseEnter);
			this->mainPanel->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::mainPanel_MouseLeave);
			this->mainPanel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &CoasterSimulator::mainPanel_MouseMove);
			// 
			// labelFeedback
			// 
			this->labelFeedback->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->labelFeedback->AutoSize = true;
			this->labelFeedback->Location = System::Drawing::Point(202, 809);
			this->labelFeedback->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->labelFeedback->Name = L"labelFeedback";
			this->labelFeedback->Size = System::Drawing::Size(24, 20);
			this->labelFeedback->TabIndex = 3;
			this->labelFeedback->Text = L"---";
			// 
			// buttonResetView
			// 
			this->buttonResetView->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonResetView->Location = System::Drawing::Point(1285, 12);
			this->buttonResetView->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->buttonResetView->Name = L"buttonResetView";
			this->buttonResetView->Size = System::Drawing::Size(75, 63);
			this->buttonResetView->TabIndex = 4;
			this->buttonResetView->Text = L"Reset View";
			this->buttonResetView->UseVisualStyleBackColor = true;
			this->buttonResetView->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonResetView_Click);
			this->buttonResetView->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->buttonResetView->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// labelLineColor
			// 
			this->labelLineColor->BackColor = System::Drawing::Color::Red;
			this->labelLineColor->Location = System::Drawing::Point(87, 169);
			this->labelLineColor->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->labelLineColor->MinimumSize = System::Drawing::Size(75, 15);
			this->labelLineColor->Name = L"labelLineColor";
			this->labelLineColor->Size = System::Drawing::Size(84, 23);
			this->labelLineColor->TabIndex = 6;
			this->labelLineColor->Text = L"Color";
			this->labelLineColor->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->labelLineColor->Click += gcnew System::EventHandler(this, &CoasterSimulator::labelLineColor_Click);
			this->labelLineColor->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->labelLineColor->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// checkBoxPoints
			// 
			this->checkBoxPoints->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->checkBoxPoints->AutoSize = true;
			this->checkBoxPoints->Location = System::Drawing::Point(1115, 22);
			this->checkBoxPoints->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->checkBoxPoints->Name = L"checkBoxPoints";
			this->checkBoxPoints->Size = System::Drawing::Size(79, 24);
			this->checkBoxPoints->TabIndex = 7;
			this->checkBoxPoints->Text = L"Points";
			this->checkBoxPoints->UseVisualStyleBackColor = true;
			this->checkBoxPoints->CheckedChanged += gcnew System::EventHandler(this, &CoasterSimulator::checkBoxPoints_CheckedChanged);
			this->checkBoxPoints->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->checkBoxPoints->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// checkBoxLabels
			// 
			this->checkBoxLabels->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->checkBoxLabels->AutoSize = true;
			this->checkBoxLabels->Enabled = false;
			this->checkBoxLabels->Location = System::Drawing::Point(1112, 48);
			this->checkBoxLabels->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->checkBoxLabels->Name = L"checkBoxLabels";
			this->checkBoxLabels->Size = System::Drawing::Size(82, 24);
			this->checkBoxLabels->TabIndex = 8;
			this->checkBoxLabels->Text = L"Labels";
			this->checkBoxLabels->UseVisualStyleBackColor = true;
			this->checkBoxLabels->CheckedChanged += gcnew System::EventHandler(this, &CoasterSimulator::checkBoxLabels_CheckedChanged);
			this->checkBoxLabels->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->checkBoxLabels->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// textBoxPointData
			// 
			this->textBoxPointData->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->textBoxPointData->Location = System::Drawing::Point(1356, 588);
			this->textBoxPointData->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->textBoxPointData->Name = L"textBoxPointData";
			this->textBoxPointData->Size = System::Drawing::Size(148, 26);
			this->textBoxPointData->TabIndex = 0;
			this->textBoxPointData->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->textBoxPointData->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// buttonMovePoint
			// 
			this->buttonMovePoint->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonMovePoint->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonMovePoint->Location = System::Drawing::Point(1353, 620);
			this->buttonMovePoint->Margin = System::Windows::Forms::Padding(0);
			this->buttonMovePoint->Name = L"buttonMovePoint";
			this->buttonMovePoint->Size = System::Drawing::Size(50, 38);
			this->buttonMovePoint->TabIndex = 10;
			this->buttonMovePoint->Text = L"Move";
			this->buttonMovePoint->UseVisualStyleBackColor = true;
			this->buttonMovePoint->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonMovePoint_Click);
			this->buttonMovePoint->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->buttonMovePoint->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// buttonAddPoint
			// 
			this->buttonAddPoint->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonAddPoint->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonAddPoint->Location = System::Drawing::Point(1402, 620);
			this->buttonAddPoint->Margin = System::Windows::Forms::Padding(0);
			this->buttonAddPoint->Name = L"buttonAddPoint";
			this->buttonAddPoint->Size = System::Drawing::Size(51, 38);
			this->buttonAddPoint->TabIndex = 10;
			this->buttonAddPoint->Text = L"Add";
			this->buttonAddPoint->UseVisualStyleBackColor = true;
			this->buttonAddPoint->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonAddPoint_Click);
			this->buttonAddPoint->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->buttonAddPoint->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// buttonRemovePoint
			// 
			this->buttonRemovePoint->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonRemovePoint->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonRemovePoint->Location = System::Drawing::Point(1454, 620);
			this->buttonRemovePoint->Margin = System::Windows::Forms::Padding(0);
			this->buttonRemovePoint->Name = L"buttonRemovePoint";
			this->buttonRemovePoint->Size = System::Drawing::Size(56, 38);
			this->buttonRemovePoint->TabIndex = 10;
			this->buttonRemovePoint->Text = L"Del";
			this->buttonRemovePoint->UseVisualStyleBackColor = true;
			this->buttonRemovePoint->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonRemovePoint_Click);
			this->buttonRemovePoint->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->buttonRemovePoint->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// label2
			// 
			this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(1352, 565);
			this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(84, 20);
			this->label2->TabIndex = 11;
			this->label2->Text = L"Point Data";
			// 
			// checkBoxEditMode
			// 
			this->checkBoxEditMode->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->checkBoxEditMode->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->checkBoxEditMode->Location = System::Drawing::Point(966, 12);
			this->checkBoxEditMode->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->checkBoxEditMode->Name = L"checkBoxEditMode";
			this->checkBoxEditMode->Size = System::Drawing::Size(154, 77);
			this->checkBoxEditMode->TabIndex = 12;
			this->checkBoxEditMode->Text = L"Edit Mode";
			this->checkBoxEditMode->UseVisualStyleBackColor = true;
			this->checkBoxEditMode->CheckedChanged += gcnew System::EventHandler(this, &CoasterSimulator::checkBoxEditMode_CheckedChanged);
			this->checkBoxEditMode->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->checkBoxEditMode->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// buttonLoadTrack
			// 
			this->buttonLoadTrack->Location = System::Drawing::Point(26, 132);
			this->buttonLoadTrack->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->buttonLoadTrack->Name = L"buttonLoadTrack";
			this->buttonLoadTrack->Size = System::Drawing::Size(75, 31);
			this->buttonLoadTrack->TabIndex = 1;
			this->buttonLoadTrack->Text = L"Load";
			this->buttonLoadTrack->UseVisualStyleBackColor = true;
			this->buttonLoadTrack->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonLoad_Click);
			this->buttonLoadTrack->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->buttonLoadTrack->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// buttonSaveTrack
			// 
			this->buttonSaveTrack->Location = System::Drawing::Point(104, 132);
			this->buttonSaveTrack->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->buttonSaveTrack->Name = L"buttonSaveTrack";
			this->buttonSaveTrack->Size = System::Drawing::Size(75, 31);
			this->buttonSaveTrack->TabIndex = 1;
			this->buttonSaveTrack->Text = L"Save";
			this->buttonSaveTrack->UseVisualStyleBackColor = true;
			this->buttonSaveTrack->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonSave_Click);
			this->buttonSaveTrack->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->buttonSaveTrack->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(27, 100);
			this->label5->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(67, 25);
			this->label5->TabIndex = 11;
			this->label5->Text = L"Track";
			// 
			// labelHoverBox
			// 
			this->labelHoverBox->AutoSize = true;
			this->labelHoverBox->BackColor = System::Drawing::Color::Yellow;
			this->labelHoverBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelHoverBox->Location = System::Drawing::Point(861, 22);
			this->labelHoverBox->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->labelHoverBox->Name = L"labelHoverBox";
			this->labelHoverBox->Size = System::Drawing::Size(84, 22);
			this->labelHoverBox->TabIndex = 0;
			this->labelHoverBox->Text = L"Hover Box";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(18, 242);
			this->label6->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(50, 20);
			this->label6->TabIndex = 11;
			this->label6->Text = L"Width";
			// 
			// hScrollBarTrackWidth
			// 
			this->hScrollBarTrackWidth->Location = System::Drawing::Point(18, 269);
			this->hScrollBarTrackWidth->Name = L"hScrollBarTrackWidth";
			this->hScrollBarTrackWidth->Size = System::Drawing::Size(156, 13);
			this->hScrollBarTrackWidth->TabIndex = 13;
			this->hScrollBarTrackWidth->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &CoasterSimulator::hScrollBarTrackWidth_Scroll);
			this->hScrollBarTrackWidth->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->hScrollBarTrackWidth->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// textBoxTrackWidth
			// 
			this->textBoxTrackWidth->Location = System::Drawing::Point(80, 237);
			this->textBoxTrackWidth->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->textBoxTrackWidth->Name = L"textBoxTrackWidth";
			this->textBoxTrackWidth->Size = System::Drawing::Size(91, 26);
			this->textBoxTrackWidth->TabIndex = 14;
			this->textBoxTrackWidth->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &CoasterSimulator::textBoxTrackWidth_KeyDown);
			this->textBoxTrackWidth->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->textBoxTrackWidth->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(44, 365);
			this->label7->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(56, 20);
			this->label7->TabIndex = 15;
			this->label7->Text = L"Height";
			this->label7->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(24, 394);
			this->label8->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(74, 20);
			this->label8->TabIndex = 15;
			this->label8->Text = L"Max Rise";
			this->label8->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(21, 422);
			this->label9->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(77, 20);
			this->label9->TabIndex = 15;
			this->label9->Text = L"Max Drop";
			this->label9->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(40, 332);
			this->label10->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(59, 20);
			this->label10->TabIndex = 15;
			this->label10->Text = L"Length";
			this->label10->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// labelMaxDrop
			// 
			this->labelMaxDrop->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelMaxDrop->Location = System::Drawing::Point(102, 422);
			this->labelMaxDrop->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->labelMaxDrop->Name = L"labelMaxDrop";
			this->labelMaxDrop->Size = System::Drawing::Size(71, 25);
			this->labelMaxDrop->TabIndex = 15;
			this->labelMaxDrop->Text = L"- - -";
			this->labelMaxDrop->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->labelMaxDrop->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->labelMaxDrop->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// checkBoxGrid
			// 
			this->checkBoxGrid->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->checkBoxGrid->AutoSize = true;
			this->checkBoxGrid->Location = System::Drawing::Point(1202, 22);
			this->checkBoxGrid->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->checkBoxGrid->Name = L"checkBoxGrid";
			this->checkBoxGrid->Size = System::Drawing::Size(65, 24);
			this->checkBoxGrid->TabIndex = 7;
			this->checkBoxGrid->Text = L"Grid";
			this->checkBoxGrid->UseVisualStyleBackColor = true;
			this->checkBoxGrid->CheckedChanged += gcnew System::EventHandler(this, &CoasterSimulator::checkBoxGrid_CheckedChanged);
			this->checkBoxGrid->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->checkBoxGrid->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// checkBoxScale
			// 
			this->checkBoxScale->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->checkBoxScale->AutoSize = true;
			this->checkBoxScale->Location = System::Drawing::Point(1202, 51);
			this->checkBoxScale->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->checkBoxScale->Name = L"checkBoxScale";
			this->checkBoxScale->Size = System::Drawing::Size(75, 24);
			this->checkBoxScale->TabIndex = 7;
			this->checkBoxScale->Text = L"Scale";
			this->checkBoxScale->UseVisualStyleBackColor = true;
			this->checkBoxScale->CheckedChanged += gcnew System::EventHandler(this, &CoasterSimulator::checkBoxPoints_CheckedChanged);
			this->checkBoxScale->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->checkBoxScale->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// labelMaxRise
			// 
			this->labelMaxRise->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelMaxRise->Location = System::Drawing::Point(102, 391);
			this->labelMaxRise->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->labelMaxRise->Name = L"labelMaxRise";
			this->labelMaxRise->Size = System::Drawing::Size(71, 25);
			this->labelMaxRise->TabIndex = 15;
			this->labelMaxRise->Text = L"- - -";
			this->labelMaxRise->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->labelMaxRise->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->labelMaxRise->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// labelTrackHeight
			// 
			this->labelTrackHeight->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelTrackHeight->Location = System::Drawing::Point(102, 362);
			this->labelTrackHeight->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->labelTrackHeight->Name = L"labelTrackHeight";
			this->labelTrackHeight->Size = System::Drawing::Size(71, 25);
			this->labelTrackHeight->TabIndex = 15;
			this->labelTrackHeight->Text = L"- - -";
			this->labelTrackHeight->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->labelTrackHeight->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->labelTrackHeight->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// labelTrackLength
			// 
			this->labelTrackLength->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelTrackLength->Location = System::Drawing::Point(102, 332);
			this->labelTrackLength->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->labelTrackLength->Name = L"labelTrackLength";
			this->labelTrackLength->Size = System::Drawing::Size(71, 25);
			this->labelTrackLength->TabIndex = 15;
			this->labelTrackLength->Text = L"- - -";
			this->labelTrackLength->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->labelTrackLength->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->labelTrackLength->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// textBoxTrackName
			// 
			this->textBoxTrackName->Location = System::Drawing::Point(16, 197);
			this->textBoxTrackName->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->textBoxTrackName->Name = L"textBoxTrackName";
			this->textBoxTrackName->Size = System::Drawing::Size(154, 26);
			this->textBoxTrackName->TabIndex = 14;
			this->textBoxTrackName->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &CoasterSimulator::textBoxTrackName_KeyDown);
			this->textBoxTrackName->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->textBoxTrackName->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(18, 172);
			this->label11->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(51, 20);
			this->label11->TabIndex = 11;
			this->label11->Text = L"Name";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(36, 302);
			this->label12->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(61, 20);
			this->label12->TabIndex = 16;
			this->label12->Text = L"Friction";
			// 
			// textBoxTrackFriction
			// 
			this->textBoxTrackFriction->Location = System::Drawing::Point(102, 297);
			this->textBoxTrackFriction->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->textBoxTrackFriction->Name = L"textBoxTrackFriction";
			this->textBoxTrackFriction->Size = System::Drawing::Size(70, 26);
			this->textBoxTrackFriction->TabIndex = 17;
			this->textBoxTrackFriction->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &CoasterSimulator::textBoxTrackFriction_KeyDown);
			this->textBoxTrackFriction->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->textBoxTrackFriction->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// comboBoxControls
			// 
			this->comboBoxControls->FormattingEnabled = true;
			this->comboBoxControls->Location = System::Drawing::Point(18, 546);
			this->comboBoxControls->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->comboBoxControls->Name = L"comboBoxControls";
			this->comboBoxControls->Size = System::Drawing::Size(154, 28);
			this->comboBoxControls->TabIndex = 18;
			this->comboBoxControls->SelectedIndexChanged += gcnew System::EventHandler(this, &CoasterSimulator::comboBoxControls_SelectedIndexChanged);
			this->comboBoxControls->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &CoasterSimulator::textBoxControlSectioins_KeyDown);
			this->comboBoxControls->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->comboBoxControls->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label13->Location = System::Drawing::Point(12, 494);
			this->label13->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(149, 20);
			this->label13->TabIndex = 11;
			this->label13->Text = L"Control Sections";
			// 
			// textBoxFinalVelocity
			// 
			this->textBoxFinalVelocity->Location = System::Drawing::Point(89, 678);
			this->textBoxFinalVelocity->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->textBoxFinalVelocity->Name = L"textBoxFinalVelocity";
			this->textBoxFinalVelocity->Size = System::Drawing::Size(52, 26);
			this->textBoxFinalVelocity->TabIndex = 19;
			this->textBoxFinalVelocity->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &CoasterSimulator::textBoxControlSectioins_KeyDown);
			this->textBoxFinalVelocity->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->textBoxFinalVelocity->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// textBoxStartLoc
			// 
			this->textBoxStartLoc->Location = System::Drawing::Point(89, 605);
			this->textBoxStartLoc->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->textBoxStartLoc->Name = L"textBoxStartLoc";
			this->textBoxStartLoc->Size = System::Drawing::Size(52, 26);
			this->textBoxStartLoc->TabIndex = 19;
			this->textBoxStartLoc->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &CoasterSimulator::textBoxControlSectioins_KeyDown);
			this->textBoxStartLoc->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->textBoxStartLoc->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// textBoxEndLoc
			// 
			this->textBoxEndLoc->Location = System::Drawing::Point(89, 642);
			this->textBoxEndLoc->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->textBoxEndLoc->Name = L"textBoxEndLoc";
			this->textBoxEndLoc->Size = System::Drawing::Size(52, 26);
			this->textBoxEndLoc->TabIndex = 19;
			this->textBoxEndLoc->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &CoasterSimulator::textBoxControlSectioins_KeyDown);
			this->textBoxEndLoc->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->textBoxEndLoc->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// label14
			// 
			this->label14->Location = System::Drawing::Point(10, 678);
			this->label14->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(71, 51);
			this->label14->TabIndex = 11;
			this->label14->Text = L"Exit Velocity";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(71, 580);
			this->label4->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(70, 20);
			this->label4->TabIndex = 11;
			this->label4->Text = L"Location";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(14, 611);
			this->label15->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(44, 20);
			this->label15->TabIndex = 11;
			this->label15->Text = L"Start";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(14, 648);
			this->label16->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(38, 20);
			this->label16->TabIndex = 11;
			this->label16->Text = L"End";
			// 
			// buttonDeleteControl
			// 
			this->buttonDeleteControl->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonDeleteControl->Location = System::Drawing::Point(99, 514);
			this->buttonDeleteControl->Margin = System::Windows::Forms::Padding(0);
			this->buttonDeleteControl->Name = L"buttonDeleteControl";
			this->buttonDeleteControl->Size = System::Drawing::Size(78, 28);
			this->buttonDeleteControl->TabIndex = 10;
			this->buttonDeleteControl->Text = L"Del";
			this->buttonDeleteControl->UseVisualStyleBackColor = true;
			this->buttonDeleteControl->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonDeleteControl_Click);
			this->buttonDeleteControl->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->buttonDeleteControl->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// buttonAddControl
			// 
			this->buttonAddControl->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonAddControl->Location = System::Drawing::Point(20, 514);
			this->buttonAddControl->Margin = System::Windows::Forms::Padding(0);
			this->buttonAddControl->Name = L"buttonAddControl";
			this->buttonAddControl->Size = System::Drawing::Size(78, 28);
			this->buttonAddControl->TabIndex = 10;
			this->buttonAddControl->Text = L"Add";
			this->buttonAddControl->UseVisualStyleBackColor = true;
			this->buttonAddControl->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonAddControl_Click);
			this->buttonAddControl->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->buttonAddControl->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// buttonNewTrack
			// 
			this->buttonNewTrack->Location = System::Drawing::Point(104, 97);
			this->buttonNewTrack->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->buttonNewTrack->Name = L"buttonNewTrack";
			this->buttonNewTrack->Size = System::Drawing::Size(75, 31);
			this->buttonNewTrack->TabIndex = 1;
			this->buttonNewTrack->Text = L"New";
			this->buttonNewTrack->UseVisualStyleBackColor = true;
			this->buttonNewTrack->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonNewTrack_Click);
			this->buttonNewTrack->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->buttonNewTrack->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// buttonNewPath
			// 
			this->buttonNewPath->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonNewPath->Location = System::Drawing::Point(1432, 498);
			this->buttonNewPath->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->buttonNewPath->Name = L"buttonNewPath";
			this->buttonNewPath->Size = System::Drawing::Size(75, 31);
			this->buttonNewPath->TabIndex = 1;
			this->buttonNewPath->Text = L"New";
			this->buttonNewPath->UseVisualStyleBackColor = true;
			this->buttonNewPath->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonNewPath_Click);
			this->buttonNewPath->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->buttonNewPath->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// buttonInitTrain
			// 
			this->buttonInitTrain->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonInitTrain->Location = System::Drawing::Point(1353, 182);
			this->buttonInitTrain->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->buttonInitTrain->Name = L"buttonInitTrain";
			this->buttonInitTrain->Size = System::Drawing::Size(153, 31);
			this->buttonInitTrain->TabIndex = 1;
			this->buttonInitTrain->Text = L"Initialize";
			this->buttonInitTrain->UseVisualStyleBackColor = true;
			this->buttonInitTrain->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonInitTrain_Click);
			this->buttonInitTrain->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->buttonInitTrain->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// label18
			// 
			this->label18->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label18->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label18->Location = System::Drawing::Point(1358, 100);
			this->label18->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(147, 31);
			this->label18->TabIndex = 11;
			this->label18->Text = L"Train";
			// 
			// buttonSimStartStop
			// 
			this->buttonSimStartStop->BackColor = System::Drawing::Color::Lime;
			this->buttonSimStartStop->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonSimStartStop->Location = System::Drawing::Point(14, 749);
			this->buttonSimStartStop->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->buttonSimStartStop->Name = L"buttonSimStartStop";
			this->buttonSimStartStop->Size = System::Drawing::Size(156, 80);
			this->buttonSimStartStop->TabIndex = 20;
			this->buttonSimStartStop->Text = L"Run Sim";
			this->buttonSimStartStop->UseVisualStyleBackColor = false;
			this->buttonSimStartStop->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonSimStartStop_Click);
			// 
			// buttonLoadTrain
			// 
			this->buttonLoadTrain->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonLoadTrain->Location = System::Drawing::Point(501, 38);
			this->buttonLoadTrain->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->buttonLoadTrain->Name = L"buttonLoadTrain";
			this->buttonLoadTrain->Size = System::Drawing::Size(66, 37);
			this->buttonLoadTrain->TabIndex = 21;
			this->buttonLoadTrain->Text = L"Load";
			this->buttonLoadTrain->UseVisualStyleBackColor = true;
			this->buttonLoadTrain->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonLoad_Click);
			// 
			// buttonSaveTrain
			// 
			this->buttonSaveTrain->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonSaveTrain->Location = System::Drawing::Point(1422, 135);
			this->buttonSaveTrain->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->buttonSaveTrain->Name = L"buttonSaveTrain";
			this->buttonSaveTrain->Size = System::Drawing::Size(86, 37);
			this->buttonSaveTrain->TabIndex = 21;
			this->buttonSaveTrain->Text = L"Save";
			this->buttonSaveTrain->UseVisualStyleBackColor = true;
			this->buttonSaveTrain->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonSave_Click);
			// 
			// label19
			// 
			this->label19->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label19->AutoSize = true;
			this->label19->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label19->Location = System::Drawing::Point(1356, 498);
			this->label19->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(56, 25);
			this->label19->TabIndex = 11;
			this->label19->Text = L"Path";
			// 
			// buttonMajorEdits
			// 
			this->buttonMajorEdits->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonMajorEdits->Location = System::Drawing::Point(806, 29);
			this->buttonMajorEdits->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->buttonMajorEdits->Name = L"buttonMajorEdits";
			this->buttonMajorEdits->Size = System::Drawing::Size(152, 52);
			this->buttonMajorEdits->TabIndex = 1;
			this->buttonMajorEdits->Text = L"Major Edits";
			this->buttonMajorEdits->UseVisualStyleBackColor = true;
			this->buttonMajorEdits->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonMajorEdits_Click);
			this->buttonMajorEdits->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->buttonMajorEdits->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// label17
			// 
			this->label17->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label17->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label17->Location = System::Drawing::Point(1356, 255);
			this->label17->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(68, 49);
			this->label17->TabIndex = 11;
			this->label17->Text = L"Curr Car";
			// 
			// labelCurrCar
			// 
			this->labelCurrCar->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->labelCurrCar->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelCurrCar->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->labelCurrCar->Location = System::Drawing::Point(1422, 255);
			this->labelCurrCar->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->labelCurrCar->Name = L"labelCurrCar";
			this->labelCurrCar->Size = System::Drawing::Size(74, 48);
			this->labelCurrCar->TabIndex = 11;
			this->labelCurrCar->Text = L"-";
			this->labelCurrCar->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// buttonSaveCar
			// 
			this->buttonSaveCar->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonSaveCar->Enabled = false;
			this->buttonSaveCar->Location = System::Drawing::Point(1383, 422);
			this->buttonSaveCar->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->buttonSaveCar->Name = L"buttonSaveCar";
			this->buttonSaveCar->Size = System::Drawing::Size(111, 37);
			this->buttonSaveCar->TabIndex = 21;
			this->buttonSaveCar->Text = L"Save";
			this->buttonSaveCar->UseVisualStyleBackColor = true;
			this->buttonSaveCar->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonSave_Click);
			// 
			// buttonAddCarFront
			// 
			this->buttonAddCarFront->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonAddCarFront->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonAddCarFront->Location = System::Drawing::Point(1384, 352);
			this->buttonAddCarFront->Margin = System::Windows::Forms::Padding(0);
			this->buttonAddCarFront->Name = L"buttonAddCarFront";
			this->buttonAddCarFront->Size = System::Drawing::Size(120, 31);
			this->buttonAddCarFront->TabIndex = 21;
			this->buttonAddCarFront->Text = L"Add Car Ahead";
			this->buttonAddCarFront->UseVisualStyleBackColor = true;
			this->buttonAddCarFront->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonLoad_Click);
			this->buttonAddCarFront->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->buttonAddCarFront->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// buttonDeleteCar
			// 
			this->buttonDeleteCar->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonDeleteCar->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonDeleteCar->Location = System::Drawing::Point(1354, 383);
			this->buttonDeleteCar->Margin = System::Windows::Forms::Padding(0);
			this->buttonDeleteCar->Name = L"buttonDeleteCar";
			this->buttonDeleteCar->Size = System::Drawing::Size(141, 31);
			this->buttonDeleteCar->TabIndex = 21;
			this->buttonDeleteCar->Text = L"Delete Selected";
			this->buttonDeleteCar->UseVisualStyleBackColor = true;
			this->buttonDeleteCar->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonDeleteCar_Click);
			this->buttonDeleteCar->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->buttonDeleteCar->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// buttonAddCarRear
			// 
			this->buttonAddCarRear->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->buttonAddCarRear->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonAddCarRear->Location = System::Drawing::Point(1347, 322);
			this->buttonAddCarRear->Margin = System::Windows::Forms::Padding(0);
			this->buttonAddCarRear->Name = L"buttonAddCarRear";
			this->buttonAddCarRear->Size = System::Drawing::Size(123, 31);
			this->buttonAddCarRear->TabIndex = 21;
			this->buttonAddCarRear->Text = L"Add Car Behind";
			this->buttonAddCarRear->UseVisualStyleBackColor = true;
			this->buttonAddCarRear->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonLoad_Click);
			this->buttonAddCarRear->MouseLeave += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedBack_MouseLeave);
			this->buttonAddCarRear->MouseHover += gcnew System::EventHandler(this, &CoasterSimulator::helpFeedback_MouseHover);
			// 
			// buttonControlEdit
			// 
			this->buttonControlEdit->Location = System::Drawing::Point(8, 460);
			this->buttonControlEdit->Name = L"buttonControlEdit";
			this->buttonControlEdit->Size = System::Drawing::Size(171, 31);
			this->buttonControlEdit->TabIndex = 22;
			this->buttonControlEdit->Text = L"Edit Control Sections";
			this->buttonControlEdit->UseVisualStyleBackColor = true;
			this->buttonControlEdit->Click += gcnew System::EventHandler(this, &CoasterSimulator::buttonControlEdit_Click);
			// 
			// CoasterSimulator
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Tan;
			this->ClientSize = System::Drawing::Size(1512, 837);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->buttonControlEdit);
			this->Controls->Add(this->buttonSimStartStop);
			this->Controls->Add(this->labelHoverBox);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->buttonAddControl);
			this->Controls->Add(this->buttonDeleteControl);
			this->Controls->Add(this->checkBoxLabels);
			this->Controls->Add(this->checkBoxScale);
			this->Controls->Add(this->checkBoxGrid);
			this->Controls->Add(this->checkBoxPoints);
			this->Controls->Add(this->buttonResetView);
			this->Controls->Add(this->labelFeedback);
			this->Controls->Add(this->buttonSaveTrack);
			this->Controls->Add(this->mainPanel);
			this->Controls->Add(this->buttonNewTrack);
			this->Controls->Add(this->buttonLoadTrack);
			this->Controls->Add(this->checkBoxEditMode);
			this->Controls->Add(this->textBoxTrackWidth);
			this->Controls->Add(this->textBoxTrackName);
			this->Controls->Add(this->labelLineColor);
			this->Controls->Add(this->hScrollBarTrackWidth);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->labelTrackLength);
			this->Controls->Add(this->labelTrackHeight);
			this->Controls->Add(this->labelMaxRise);
			this->Controls->Add(this->labelMaxDrop);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->textBoxTrackFriction);
			this->Controls->Add(this->label16);
			this->Controls->Add(this->label15);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->textBoxEndLoc);
			this->Controls->Add(this->textBoxStartLoc);
			this->Controls->Add(this->textBoxFinalVelocity);
			this->Controls->Add(this->comboBoxControls);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->labelCurrCar);
			this->Controls->Add(this->buttonSaveCar);
			this->Controls->Add(this->buttonDeleteCar);
			this->Controls->Add(this->buttonAddCarRear);
			this->Controls->Add(this->buttonAddCarFront);
			this->Controls->Add(this->buttonSaveTrain);
			this->Controls->Add(this->buttonLoadTrain);
			this->Controls->Add(this->label17);
			this->Controls->Add(this->label18);
			this->Controls->Add(this->label19);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->buttonRemovePoint);
			this->Controls->Add(this->buttonAddPoint);
			this->Controls->Add(this->buttonMovePoint);
			this->Controls->Add(this->textBoxPointData);
			this->Controls->Add(this->buttonInitTrain);
			this->Controls->Add(this->buttonMajorEdits);
			this->Controls->Add(this->buttonNewPath);
			this->Controls->Add(this->buttonSavePath);
			this->Controls->Add(this->buttonLoadPath);
			this->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->Name = L"CoasterSimulator";
			this->Text = L"Coaster Simulator (NG24)";
			this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &CoasterSimulator::CoasterSimulator_MouseWheel);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void mainPanel_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
		Graphics^ g = e->Graphics;

		g->TranslateTransform(panX, panY);
		g->ScaleTransform(zoomLevel, -zoomLevel);

		if (theTrack != nullptr) {
			updatePropertiesDisplay();  // not very efficient to do this here, but at least I can be sure!

			if (checkBoxGrid->Checked)
				drawOrthoGrid(g);
			if (checkBoxScale->Checked)
				drawScale(g);

			theTrack->paint(g, checkBoxPoints->Checked || inEditMode, checkBoxLabels->Checked || inEditMode);

		}
		if (theTrain != nullptr)
			if (inEditMode) {
				int carToHighlight = currCarIndex;
				if (labelCurrCar->Text != "-")
					carToHighlight = Convert::ToInt32(labelCurrCar->Text);
				theTrain->paint(g, carToHighlight);
			}
			else
				theTrain->paint(g);
	}

	private: System::Void buttonLoad_Click(System::Object^ sender, System::EventArgs^ e) {
		auto fileSelector = gcnew OpenFileDialog;

		// set up some properties for the dialog
		fileSelector->InitialDirectory = ".";  // current folder

		// set up file filter based on object (button) that called this function
		// will show only filtered files, but user can choose to show all files
		if (sender->Equals(buttonLoadPath))
			fileSelector->Filter = "Path files (*.path)|*.path|All files (*.*)|*.*";
		else if (sender->Equals(buttonLoadTrack))
			fileSelector->Filter = "Track files (*.track)|*.track|All files (*.*)|*.*";
		else if (sender->Equals(buttonLoadTrain))
			fileSelector->Filter = "Train files (*.train)|*.train|All files (*.*)|*.*";
		else if (sender->Equals(buttonAddCarFront) || sender->Equals(buttonAddCarRear))
			fileSelector->Filter = "Car files (*.car)|*.car|All files (*.*)|*.*";

		fileSelector->FilterIndex = 1;
		fileSelector->Multiselect = false;  // this is the default, but nice to know
		fileSelector->RestoreDirectory = true; // will remember last folder used

		// now actually open the file dialog and get the name of the file user wants
		if (fileSelector->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			// user did not press cancel button nor closed the dialog
			// create filestream for the file
			if (fileSelector->OpenFile() != nullptr)
			{
				fileSelector->OpenFile()->Close();  // close it since we are using it in another way

				labelFeedback->Text = fileSelector->FileName + " is opening";

				std::ifstream inFile;  // need to #include <fstream>
				inFile.open(StringPlus::convertString(fileSelector->FileName)); // needs StringPlus.h

				if (inFile.is_open()) {
					// do whatever you need with inFile (like use it to create a path, maybe)
					// based on the button that called this function

					if (sender->Equals(buttonLoadPath)) {
						if (theTrack == nullptr) {
							theTrack = new Track();
							labelFeedback->Text = "Created a new track since no tracks were active";
						}
						// note: I no longer have a variable called "thePath" since it is part of theTrack
						theTrack->setPath(new CubicSpline2D(inFile));
						resetView();  // this also does a refresh
					}
					else if (sender->Equals(buttonLoadTrack)) {
						if (theTrack != nullptr)
							delete theTrack;

						theTrack = new Track(inFile);
						if (theTrain != nullptr) {
							theTrain->setTrack(theTrack);
							theTrain->initialize();
							theTrack->setTrain(theTrain);
						}
						updateControlsList();

						resetView();
					}

					else if (sender->Equals(buttonLoadTrain)) {
						if (theTrain != nullptr)
							delete theTrain;

						theTrain = new Train(inFile);
						if (theTrack != nullptr) {
							theTrain->setTrack(theTrack);
							theTrain->initialize();
							theTrack->setTrain(theTrain);
						}
						resetView();
					}

					else if (sender->Equals(buttonAddCarFront) || sender->Equals(buttonAddCarRear)) {
						auto aCar = new Car(inFile);
						if (theTrain == nullptr) {
							theTrain = new Train();
							labelFeedback->Text = "Created a new train since no trains were active";
							if (theTrack != nullptr) {
								theTrain->setTrack(theTrack);
								theTrack->setTrain(theTrain);
							}
							//currCarIndex = 1;
							labelCurrCar->Text = "1";
							theTrain->addCar(aCar, 0, sender->Equals(buttonAddCarFront));
						}
						else if (labelCurrCar->Text != "-"){

							theTrain->addCar(aCar, Convert::ToInt32(labelCurrCar->Text),
								sender->Equals(buttonAddCarFront));
						}
						else {
							delete aCar;
							labelFeedback->Text = "Must select a car position to add a car.";
						}
						mainPanel->Refresh();  // note: NOT resetting zoom and pan

					}

					inFile.close();  // be polite and close the input file
				}
				else {
					// provide feedback that you were unable to open file ???
					labelFeedback->Text = " ERROR: Could not open " + fileSelector->FileName;
				}
			}
		}
	}

	private: System::Void buttonSave_Click(System::Object^ sender, System::EventArgs^ e) {
		if ((theTrack != nullptr && theTrack->getPath() != nullptr) || theTrain != nullptr) {
			auto fileSelector = gcnew SaveFileDialog;

			fileSelector->InitialDirectory = ".";  // current folder
			// set up file filter based on object (button) that called this function
			// will show only filtered files, but user can choose to show all files
			if (sender->Equals(buttonSavePath))
				fileSelector->Filter = "Path files (*.path)|*.path|All files (*.*)|*.*";
			else if (sender->Equals(buttonSaveTrack))
				fileSelector->Filter = "Track files (*.track)|*.track|All files (*.*)|*.*";
			else if (sender->Equals(buttonSaveTrain))
				fileSelector->Filter = "Train files (*.train)|*.train|All files (*.*)|*.*";
			else if (sender->Equals(buttonSaveCar))
				fileSelector->Filter = "Car files (*.car)|*.car|All files (*.*)|*.*";


			fileSelector->FilterIndex = 1;  // will show only linkage files, but user can choose to show all files
			fileSelector->RestoreDirectory = true; // will remember last directory/folder used

			// actually open the file dialog and get the name of the file user wants
			if (fileSelector->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				// user did not press cancel button nor closed the dialog
				// create filestream for the file
				auto myStream = fileSelector->OpenFile();  // needed to be able to close it later
				if (myStream != nullptr) {
					myStream->Close(); // close it since we are using it in another way
					//(fileSelector->OpenFile())->Close(); // for some reason, this DOES NOT WORK

					labelFeedback->Text = fileSelector->FileName + " is opening";

					std::ofstream outFile;
					outFile.open(StringPlus::convertString(fileSelector->FileName)); // needs StringPlus.h

					if (outFile.is_open()) {

						if (sender->Equals(buttonSavePath) && theTrack != nullptr && theTrack->getPath() != nullptr) {
							outFile << *theTrack->getPath(); // follow the pointer then output
						}
						else if (sender->Equals(buttonSaveTrack) && theTrack != nullptr) {
							outFile << *theTrack;
						}
						else if (sender->Equals(buttonLoadTrain) && theTrain != nullptr) {
							outFile << *theTrain;
						}
						else if (sender->Equals(buttonSaveCar) && theTrain != nullptr) {
							// ask which car to save maybe ???
						}

						outFile.close();

					}
					else {
						// provide feedback that you were unable to open file ???
						labelFeedback->Text = " ERROR: Could not save to " + fileSelector->FileName;
					}
				}
			}
		}
		else {
			labelFeedback->Text = " ERROR: There is nothing to save.";
		}

	}

	private: void resetView() {
		Point2D upper, lower;

		// sets panX, panY and zoomLevel so path just fits in panel
		if (theTrack != nullptr && theTrack->getPath() != nullptr) {
			upper = theTrack->getPath()->upperBoundingBox();
			lower = theTrack->getPath()->lowerBoundingBox();

			float width = upper.x - lower.x;
			float height = upper.y - lower.y;

			float widthScale = mainPanel->Width / max(0.001, width);  // just in case width is zero
			float heightScale = mainPanel->Height / max(0.001, height); // just in case height is zero

			float borderSpaceRatio = 0.85;  // value of 1.0 will leave no white space around gear
			zoomLevel = min(widthScale, heightScale) * borderSpaceRatio;  // may need to #include <algorithm> to use min and max

			// now for panX and panY >>> tough!!!
			panY = mainPanel->Height / 2;
			panX = mainPanel->Width / 2;
			panX += -(upper.x + lower.x) / 2. * zoomLevel;
			panY += (upper.y + lower.y) / 2. * zoomLevel;
			// note that panY adjustment is not negative because panY is applied before scaling

		}

		mainPanel->Refresh();
	}

	private: System::Void buttonResetView_Click(System::Object^ sender, System::EventArgs^ e) {
		resetView();
	}

	private: System::Void mainPanel_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {

		if (e->Button == System::Windows::Forms::MouseButtons::Middle
			|| e->Button == System::Windows::Forms::MouseButtons::Left) {
			currX = e->X;		// need to set the initial click position to avoid "jumping"
			currY = e->Y;

			// determine in the click was near a path point
			Point2D clickedPnt = getWorldCoords(e->X, e->Y); // convert the click location to "world" coordinates
			int pixelProximity = 5;  // increase this numner to make it "easier" to click on a point
			float modelSelectDistance = pixelProximity / zoomLevel;
			if (theTrack != nullptr && theTrack->getPath() != nullptr) {
				int clickedIndex = theTrack->getPath()->getIndex(clickedPnt, modelSelectDistance); // this is why you wrote this function
				if (clickedIndex != -1) {// if the click is near a point
					auto clickedPoint = theTrack->getPath()->getPoint(clickedIndex);
					labelFeedback->Text = "Just clicked on point " + clickedIndex.ToString()
						+ " (" + clickedPoint.x + ", " + clickedPoint.y
						+ ")";

					textBoxPointData->Text = clickedIndex.ToString()
						+ " " + StringPlus::sigFigs(clickedPoint.x, 4)
						+ " " + StringPlus::sigFigs(clickedPoint.y, 4);
				}
				else {
					labelFeedback->Text = " - - - ";
				}
			}
			if (inEditMode && currCarIndex != -1) {
				labelCurrCar->Text = currCarIndex.ToString();
			}
			else
				labelCurrCar->Text = "-";
		}
		if (e->Button == System::Windows::Forms::MouseButtons::Right) {
			// show coords on labelFeedback
			Point2D clickedPnt;
			if (theTrack != nullptr && theTrack->getPath() != nullptr)
				clickedPnt = getWorldCoords(e->X, e->Y);
			else
				clickedPnt = { e->X * 1.f, e->Y * 1.f }; // need to convert to float

			labelFeedback->Text = StringPlus::sigFigs(clickedPnt.x, 4) + ", " + StringPlus::sigFigs(clickedPnt.y, 4);
		}
	}

	private: Point2D getWorldCoords(float screenX, float screenY) {
		return { (screenX - panX) / zoomLevel, (screenY - panY) / -zoomLevel };
	}
	private: Point2D getScreenCoords(float worldX, float worldY) {
		return { roundf(worldX * zoomLevel) + panX , roundf(worldY * -zoomLevel) + panY };
	}

	private: System::Void mainPanel_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		// this function allows panning by pressing down on mousewheel (middle button) and dragging
		// essentially, we adjust panning by however much the mouse is moved since the last time we called
		// this function

		if (e->Button == System::Windows::Forms::MouseButtons::Middle // if the middle button was down
			|| (e->Button == System::Windows::Forms::MouseButtons::Left && Control::ModifierKeys == Keys::Shift)) {  // or shift and left
			// pan the screen
			panX += (e->X - currX);
			panY += (e->Y - currY);
			mainPanel->Refresh();  // this will "repaint" the main panel as we drag
			currX = e->X; currY = e->Y; // need to constantly update to allow drag
		}
		else if (e->Button == System::Windows::Forms::MouseButtons::Left && Control::ModifierKeys == Keys::Control) {
			float oldZoom = zoomLevel; // will need to remember this when resetting panX and panY

			float zoomStep = 1.1f;  // change as needed for more effect per wheel roll
			if (e->Y > currY) // down move so zoom out (remember that y is less when moving up)
				zoomLevel = zoomLevel / zoomStep;
			else if (e->Y < currY)  // up move so zoom in
				zoomLevel = zoomLevel * zoomStep;

			// No need to adjust since we are "inside" mainPanel
			int adjustedX = e->X/* - mainPanel->Location.X - mainPanel->Margin.Left*/;
			int adjustedY = e->Y /*- mainPanel->Location.Y - mainPanel->Margin.Top*/;

			// reset panX and panY such that the coords under the mouse pointer are unchanged
			// i.e., we can zoom in/out on a specific point
			panX = (int)round((adjustedX * (oldZoom - zoomLevel)
				+ panX * zoomLevel) / oldZoom);
			panY = (int)round((adjustedY * (oldZoom - zoomLevel)
				+ panY * zoomLevel) / oldZoom);

			mainPanel->Refresh();  // this will "repaint" the main panel as we drag
			currX = e->X; currY = e->Y; // need to constantly update to allow drag
		}
		else if (inEditMode && theTrack != nullptr && theTrack->getPath() != nullptr) {
			Point2D currLoc = getWorldCoords(e->X, e->Y);
			if (e->Button == System::Windows::Forms::MouseButtons::Left && pointToMove != -1) {
				theTrack->getPath()->movePoint(currLoc, pointToMove);
				mainPanel->Refresh();
			}
			else {
				float dist = 5. / zoomLevel;  // this implies a 5 pixel "radius"
				pointToMove = theTrack->getPath()->getIndex(currLoc, dist);
				if (pointToMove != -1)
					this->Cursor = Cursors::Hand;   // just something cute
				else if (theTrain != nullptr) {
					currCarIndex = theTrain->getCar(currLoc);
					if (currCarIndex != -1) {
						this->Cursor = Cursors::NoMove2D;   // Four little arrows
						labelFeedback->Text = "Click to select Car #" + currCarIndex.ToString();
						mainPanel->Refresh(); // to show highlighting
					}
					else {
						this->Cursor = Cursors::Default;
						labelFeedback->Text = "- - -";

					}
				}
				else
					this->Cursor = Cursors::Default;
			}

		}

	}
	private: System::Void mainPanel_MouseEnter(System::Object^ sender, System::EventArgs^ e) {
		mouseInPanel = true;
	}
	private: System::Void mainPanel_MouseLeave(System::Object^ sender, System::EventArgs^ e) {
		mouseInPanel = false;
	}
	private: void CoasterSimulator_MouseWheel(Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		// Need to add this event towards end of InitializeComponent() function above, 
		// just below the line:  this->Name = L"CoasterSimulator";
		// 	 this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &CoasterSimulator::CoasterSimulator_MouseWheel);

		// panel does not take a mousewheel event, so we must handle it at the the level of the form
		// but only if the mouse pointer is inside the panel
		if (mouseInPanel) {

			float oldZoom = zoomLevel; // will need to remember this when resetting panX and panY

			float zoomStep = 1.2f;  // change as needed for more effect per wheel roll
			if (e->Delta < 0) // down roll so zoom out
				zoomLevel = zoomLevel / zoomStep;
			else   // up roll so zoom in
				zoomLevel = zoomLevel * zoomStep;

			// get actual location of pointer inside the panel (as opposed to the form itself)
			int adjustedX = e->X - mainPanel->Location.X - mainPanel->Margin.Left;
			int adjustedY = e->Y - mainPanel->Location.Y - mainPanel->Margin.Top;

			// reset panX and panY such that the coords under the mouse pointer are unchanged
			// i.e., we can zoom in/out on a specific point
			panX = (int)round((adjustedX * (oldZoom - zoomLevel)
				+ panX * zoomLevel) / oldZoom);
			panY = (int)round((adjustedY * (oldZoom - zoomLevel)
				+ panY * zoomLevel) / oldZoom);

			mainPanel->Refresh();

		}
	}

	private: System::Void labelLineColor_Click(System::Object^ sender, System::EventArgs^ e) {
		auto colorChooser = gcnew ColorDialog();

		if (colorChooser->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			labelLineColor->BackColor = colorChooser->Color;
			if (theTrack != nullptr) {
				theTrack->setColor(colorChooser->Color);
				mainPanel->Refresh();
			}
		}
	}
	private: void initializeHoverHelp() {
		// note: the "L" in front of strings is to indicate the use of "wide" text (larger character set)

		hoverResponses = gcnew Hashtable;
		hoverResponses->Add(L"checkBoxEditMode", L"Edit: Will enter into edit mode, allowing graphical editing of points");
		hoverResponses->Add(L"checkBoxGrid", L"Grid: Displays a grid behind track");
		hoverResponses->Add(L"checkBoxScale", L"Scale: Displays a scale on main window");
		hoverResponses->Add(L"checkBoxPoints", L"Points: Show definining points of track");
		hoverResponses->Add(L"checkBoxLabels", L"Labels: Show labels of definining points (if showing)");

		hoverResponses->Add(L"buttonNewTrack", L"Track: Create a new track, with some default parameters.");
		hoverResponses->Add(L"buttonLoadTrack", L"Track: Load a track file from permanent storage (will over-ride currently loaded path)");
		hoverResponses->Add(L"buttonSaveTrack", L"Track: Save the whole track (including path) to a file");
		hoverResponses->Add(L"buttonNewPath", L"Path: Create a new path with 3 points (will create a new track if needed)");
		hoverResponses->Add(L"buttonLoadPath", L"Path: Load a path file from permanent storage and assign it to the track (will create a new track if needed)");
		hoverResponses->Add(L"buttonSavePath", L"Path: Save the current path (not the whole track) to a file");
		hoverResponses->Add(L"vScrollBarLineWidth", L"Track: Change the display width of track");

		hoverResponses->Add(L"buttonResetView", L"Reset View: Changes pan and zoom so that path is centered and scaled to fit on display");
		hoverResponses->Add(L"buttonRemovePoint", L"Remove Point: Removes the given point ");
		hoverResponses->Add(L"buttonAddPoint", L"Add Point: Adds the given point to the path [ index xCoord yCoord]");
		hoverResponses->Add(L"buttonMovePoint", L"Move Point: Moves the given point in the path [ index xCoord yCoord]");
		hoverResponses->Add(L"textBoxPointData", L"Data Entry: Enter data for a point using format [ index xCoord yCoord]. Select a point to autoload data");

		hoverResponses->Add(L"textBoxTrackName", L"Track Data: Name of track. Press ENTER when making changes");
		hoverResponses->Add(L"textBoxTrackWidth", L"Track Data: Width of track. Press ENTER when making changes");
		hoverResponses->Add(L"hScrollBarTrackWidth", L"Track Data: Width of track. Adjust using scroll bar");
		hoverResponses->Add(L"textBoxTrackFriction", L"Track Data: Friction coefficient applied to whole track (includes rolling resistance). Press ENTER when making changes");
		hoverResponses->Add(L"textBoxLocation", L"Location: Coordinates of last clicked point (x, y). Edit to move point");

		hoverResponses->Add(L"labelLineColor", L"Track: Opens a color selector to set the color of the track");
		hoverResponses->Add(L"labelTrackLength", L"Track Data: Overall length of track");
		hoverResponses->Add(L"labelTrackHeight", L"Track Data: Overall height of track (from lowest point to highest point)");
		hoverResponses->Add(L"labelMaxRise", L"Track Data: Elevation change of largest continuous rise in the track");
		hoverResponses->Add(L"labelMaxDrop", L"Track Data: Elevation change of largest continuous drop in the track");

		hoverResponses->Add(L"buttonAddControl", L"Control Data: Adds a new control section, initialized to all zeros");
		hoverResponses->Add(L"buttonDeleteControl", L"Control Data: Deletes the currently selected control section");
		hoverResponses->Add(L"comboBoxControls", L"Control Data: Select a control to edit. Press Enter to edit name");
		hoverResponses->Add(L"textBoxStartLoc", L"Control Data: Starting location of control (measured from start of track)");
		hoverResponses->Add(L"textBoxEndLoc", L"Control Data: Ending location of control (measured from start of track)");
		hoverResponses->Add(L"textBoxStartForce", L"Control Data: Value of force at start of control");
		hoverResponses->Add(L"textBoxEndForce", L"Control Data: Value of force at end of control");

		hoverResponses->Add(L"buttonAddCarFront", L"Train: Adds a car to front of current car from selected data file");
		hoverResponses->Add(L"buttonAddCarRear", L"Train: Adds a car to rear of current car from selected data file");
		hoverResponses->Add(L"buttonDeleteCar", L"Train: Deletes current car from train");
		hoverResponses->Add(L"buttonSaveCar", L"Train: Not functional");

	}
	private: System::Void helpFeedback_MouseHover(System::Object^ sender, System::EventArgs^ e) {
		// need to assign this function to all the GUI things I want to have descriptions

		auto theControl = ((Control^)sender);
		auto controlName = theControl->Name;
		// add in robustness
		auto response = hoverResponses[controlName];

		if (response != nullptr) {
			std::string formattedResponse = StringPlus::wrapLongLine(StringPlus::convertString(response->ToString()), 30);

			//labelFeedback->Text = "Help: " + response + ".";
			labelHoverBox->Text = gcnew String(formattedResponse.c_str());

			labelHoverBox->Left = theControl->Left + theControl->Width / 2;
			if (labelHoverBox->Left + labelHoverBox->Width + 20 > this->Width)
				labelHoverBox->Left = this->Width - labelHoverBox->Width - 20;

			labelHoverBox->Top = theControl->Top + theControl->Height + 3;
			if ((labelHoverBox->Top + labelHoverBox->Height + 40) > this->Height)
				labelHoverBox->Top = this->Height - labelHoverBox->Height - 40;

			int i = 1;
		}

	}
	private: System::Void helpFeedBack_MouseLeave(System::Object^ sender, System::EventArgs^ e) {
		labelHoverBox->Left = -1000;
	}

	private: System::Void checkBoxPoints_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		checkBoxLabels->Enabled = checkBoxPoints->Checked;
		mainPanel->Refresh();
	}
	private: System::Void checkBoxLabels_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		mainPanel->Refresh();
	}
	private: System::Void vScrollBarLineWidth_Scroll(System::Object^ sender, System::Windows::Forms::ScrollEventArgs^ e) {
		mainPanel->Refresh();
	}

	private: System::Void buttonMovePoint_Click(System::Object^ sender, System::EventArgs^ e) {
		applyPointBox(sender);
	}
	private: System::Void buttonAddPoint_Click(System::Object^ sender, System::EventArgs^ e) {
		applyPointBox(sender);
	}
	private: System::Void buttonRemovePoint_Click(System::Object^ sender, System::EventArgs^ e) {
		applyPointBox(sender);
	}
	private: System::Void applyPointBox(System::Object^ sender) {
		if (theTrack != nullptr) {
			if (textBoxPointData->Text != "") {  // something in box
				auto coordStrings = StringPlus::split(StringPlus::convertString(textBoxPointData->Text), " ,");
				int index = -1;
				float xCoord = 0, yCoord = 0;

				// will add error-checking later
				if (coordStrings.size() > 0)
					index = stoi(coordStrings[0]);
				if (coordStrings.size() > 2) {
					xCoord = stof(coordStrings[1]);
					yCoord = stof(coordStrings[2]);
				}

				if (theTrack->getPath() == nullptr && sender->Equals(buttonAddPoint)) {  // create a new path and add the point
					theTrack->setPath(new Path2D());
				}

				if (theTrack->getPath() != nullptr) {  // act depending on who called this function
					bool allOK = true;
					if (sender->Equals(buttonMovePoint) && coordStrings.size() > 2)
						allOK = theTrack->getPath()->movePoint({ xCoord, yCoord }, index);
					if (sender->Equals(buttonAddPoint) && coordStrings.size() > 2)
						allOK = theTrack->getPath()->addPoint({ xCoord, yCoord }, index);
					if (sender->Equals(buttonRemovePoint))
						allOK = theTrack->getPath()->removePoint(index);

					if (!allOK)
						labelFeedback->Text = "Was not able to make changes to path";
					else {
						mainPanel->Refresh();

						// clean up the input
						textBoxPointData->Text = index.ToString();
						if (coordStrings.size() > 2)
							textBoxPointData->Text += " " + StringPlus::sigFigs(xCoord, 4) + " " + StringPlus::sigFigs(yCoord, 4);
					}
				}
			}
		}
		else {
			labelFeedback->Text = "Must load/create a track to use feature.";
		}
	}

	private: System::Void checkBoxEditMode_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		inEditMode = !inEditMode;
		mainPanel->Refresh();
	}
	private: System::Void checkBoxGrid_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		mainPanel->Refresh();
	}

	private: float getScaleSizeX() {
		Point2D topCorner = getWorldCoords(10, 10);
		Point2D botCorner = getWorldCoords(90, 20);
		int decimalPlaces = -5;
		float sizeX = roundf(botCorner.x - topCorner.x) * pow(10., decimalPlaces);
		while (sizeX <= 0.) {
			decimalPlaces++;
			sizeX = roundf((botCorner.x - topCorner.x) * pow(10., decimalPlaces));
		}
		sizeX /= pow(10., decimalPlaces);
		return sizeX;

	}
	private: void drawScale(Graphics^ g) {
		if (theTrack != nullptr && theTrack->getPath() != nullptr) {
			// convert from screen size and location I want it to appear to
			// corresponding world coords
			Point2D topCorner = getWorldCoords(10, 10);
			Point2D botCorner = getWorldCoords(90, 20);
			float sizeY = (topCorner.y - botCorner.y);
			float sizeX = getScaleSizeX();

			// draw the scale box
			Brush^ fillBrush = gcnew SolidBrush(Color::Cyan);
			g->FillRectangle(fillBrush, topCorner.x, botCorner.y, sizeX, sizeY);

			// draw the number
			float locX = topCorner.x + sizeX, locY = botCorner.y;
			g->TranslateTransform(locX, locY);
			g->ScaleTransform(1, -1);
			sizeY = max(1e-8, sizeY);  // sizeY cannot be zero

			System::Drawing::Font^ scaleFont = gcnew System::Drawing::Font(L"Microsoft Sans Serif",
				sizeY, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0));
			g->DrawString(sizeX.ToString(), scaleFont, fillBrush, 0., 0.);
			g->ScaleTransform(1, -1);
			g->TranslateTransform(-locX, -locY);
		}
	}

	private: void drawOrthoGrid(Graphics^ g) {
		Pen^ gridPen = gcnew Pen(ColorNG::colorFromHSV(240, 0.2, 1.0), 0.);
		float spacing = getScaleSizeX() / 2.;
		Point2D topLeftCorner = getWorldCoords(0, 0);
		Point2D botRightCorner = getWorldCoords(mainPanel->Width, mainPanel->Height);
		float minX = topLeftCorner.x, maxX = botRightCorner.x;
		float maxY = topLeftCorner.y, minY = botRightCorner.y;
		float xLimit = max(fabs(minX), fabs(maxX));
		float yLimit = max(fabs(minY), fabs(maxY));
		float currGridX = -spacing;
		float currGridY = -spacing;
		while (currGridX <= xLimit || currGridY <= yLimit) {
			currGridX += spacing;
			currGridY += spacing;
			g->DrawLine(gridPen, currGridX, minY, currGridX, maxY);
			g->DrawLine(gridPen, minX, currGridY, maxX, currGridY);
			g->DrawLine(gridPen, -currGridX, minY, -currGridX, maxY);
			g->DrawLine(gridPen, minX, -currGridY, maxX, -currGridY);
		}
	}

	private: void updatePropertiesDisplay() {
		if (theTrack != nullptr) {
			textBoxTrackName->Text = gcnew String(theTrack->getName().c_str());
			textBoxTrackWidth->Text = StringPlus::sigFigs(theTrack->getWidth(), 3);
			textBoxTrackFriction->Text = StringPlus::sigFigs(theTrack->getFriction(), 3);
			if (theTrack->getPath() != nullptr)
				labelTrackLength->Text = StringPlus::sigFigs(theTrack->getPath()->length(), 3);
			labelTrackHeight->Text = StringPlus::sigFigs(theTrack->getTrackHeight(), 3);
			labelMaxRise->Text = StringPlus::sigFigs(theTrack->getLargestRise(), 3);
			labelMaxDrop->Text = StringPlus::sigFigs(theTrack->getLargestDrop(), 3);

			labelLineColor->BackColor = *theTrack->getColor();

			if (theTrack->getWidth() > hScrollBarTrackWidth->Maximum / 10.f)
				hScrollBarTrackWidth->Maximum = ((int)theTrack->getWidth() + 5) * 10;
			hScrollBarTrackWidth->Value = theTrack->getWidth() * 10.f;

		}
	}
	private: void updateControlsList() {
		if (theTrack != nullptr) {
			ControlSection* currControl;
			comboBoxControls->Items->Clear();
			int i = 1;
			while ((currControl = theTrack->getControlSection(i)) != nullptr) {
				String^ currLabel = i.ToString() + ": " + gcnew String(currControl->getName().c_str());
				comboBoxControls->Items->Add(currLabel);
				i++;
			}
			if (i > 1) { // at least one control
				comboBoxControls->SelectedIndex = 0;
				updateControlTextBoxes();
			}

		}
	}
	private: System::Void textBoxTrackName_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		if (theTrack != nullptr && e->KeyCode == Keys::Enter) {
			e->SuppressKeyPress = true;

			theTrack->setName(StringPlus::convertString(textBoxTrackName->Text));
			mainPanel->Refresh();
		}
	}

	private: System::Void textBoxTrackWidth_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		if (theTrack != nullptr && e->KeyCode == Keys::Enter) {
			e->SuppressKeyPress = true;
			auto newWidth = getFloatFromBox(textBoxTrackWidth);
			if (newWidth != -INFINITY) {
				theTrack->setWidth(newWidth);
				mainPanel->Refresh();
			}
		}
	}
	private: System::Void textBoxTrackFriction_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		if (theTrack != nullptr && e->KeyCode == Keys::Enter) {
			e->SuppressKeyPress = true;
			auto newFriction = getFloatFromBox(textBoxTrackFriction);
			if (newFriction != -INFINITY) {
				theTrack->setFriction(newFriction);
				mainPanel->Refresh();
			}
		}
	}
	private: System::Void textBoxControlSectioins_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		if (theTrack != nullptr && e->KeyCode == Keys::Enter) {
			e->SuppressKeyPress = true;
			readControlTextBoxes();
		}
	}

	private: int getIntegerFromBox(TextBox^ currBox) {
		String^ boxName = currBox->Name;
		try {
			return Convert::ToInt32(currBox->Text); // could generate exception
		}
		catch (Exception^ e) {
			MessageBox::Show(e->Message + "\n\nUnable to get valid integer from " + boxName);
			currBox->Focus();
			return -MININT;
		}
	}

	private: float getFloatFromBox(TextBox^ currBox) {
		String^ boxName = currBox->Name;
		try {
			return Convert::ToSingle(currBox->Text); // could generate exception
		}
		catch (Exception^ e) {
			MessageBox::Show(e->Message + "\n\nUnable to get valid number from " + boxName);
			currBox->Focus();
			return -INFINITY;
		}
	}

	private: System::Void hScrollBarTrackWidth_Scroll(System::Object^ sender, System::Windows::Forms::ScrollEventArgs^ e) {
		if (theTrack != nullptr) {
			float newWidth = hScrollBarTrackWidth->Value / 10.f;
			theTrack->setWidth(newWidth);
			mainPanel->Refresh();
		}
		else
			labelFeedback->Text = "Must load/create a track to use feature.";
	}

	private: System::Void comboBoxControls_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		updateControlTextBoxes();
	}

	private: void updateControlTextBoxes() {
		if (theTrack != nullptr) {
			auto currSelection = comboBoxControls->SelectedIndex;
			ControlSection* currSection = theTrack->getControlSection(currSelection + 1);
			if (currSection != nullptr) {
				textBoxStartLoc->Text = StringPlus::sigFigs(currSection->get("startLoc"), 3);
				textBoxEndLoc->Text = StringPlus::sigFigs(currSection->get("endLoc"), 3);
				textBoxFinalVelocity->Text = StringPlus::sigFigs(currSection->get("finalVelocity"), 3);
			}
		}
	}

	private: void readControlTextBoxes() {
		if (theTrack != nullptr) {
			auto currSelection = comboBoxControls->SelectedIndex;
			auto textInComboBox = StringPlus::convertString(comboBoxControls->Text);
			int colonLocation = textInComboBox.find(":");
			if (currSelection == -1) { // typing in combo box
				currSelection = Convert::ToSingle(comboBoxControls->Text->Substring(0, colonLocation)) - 1;
			}
			ControlSection* currSection = theTrack->getControlSection(currSelection + 1);
			if (currSection != nullptr) {

				currSection->setName(textInComboBox.substr(colonLocation + 1));

				float temp = getFloatFromBox(textBoxStartLoc);
				if (temp != -INFINITY) currSection->set("startLoc", temp);

				temp = getFloatFromBox(textBoxEndLoc);
				if (temp != -INFINITY) currSection->set("endLoc", temp);

				temp = getFloatFromBox(textBoxFinalVelocity);
				if (temp != -INFINITY) currSection->set("finalVelocity", temp);

			}
			updateControlsList();
			comboBoxControls->SelectedIndex = currSelection;
			mainPanel->Refresh();
		}
	}
	private: System::Void buttonAddControl_Click(System::Object^ sender, System::EventArgs^ e) {
		if (theTrack != nullptr) {
			theTrack->addControlSection();
			updateControlsList();
			comboBoxControls->SelectedIndex = comboBoxControls->Items->Count - 1;
		}
		else
			labelFeedback->Text = "Must load/create a track to use feature.";
	}
	private: System::Void buttonDeleteControl_Click(System::Object^ sender, System::EventArgs^ e) {
		if (theTrack != nullptr) {
			theTrack->deleteControlSection(comboBoxControls->SelectedIndex + 1);
			updateControlsList();
		}
		else
			labelFeedback->Text = "Must load/create a track to use feature.";
	}
	private: System::Void buttonNewTrack_Click(System::Object^ sender, System::EventArgs^ e) {
		if (theTrack != nullptr)
			delete theTrack;
		theTrack = new Track();
		theTrack->setName("Unnamed Track");
		theTrack->setColor(Color::IndianRed);
		theTrack->setWidth(0.1);
		updatePropertiesDisplay();
		mainPanel->Refresh();
	}
	private: System::Void buttonNewPath_Click(System::Object^ sender, System::EventArgs^ e) {
		if (theTrack == nullptr) {
			buttonNewTrack_Click(sender, e);
			labelFeedback->Text = "Created a new track since no tracks were active";
		}

		theTrack->setPath(new CubicSpline2D);
		theTrack->getPath()->addPoint({ 0.f, 0.f }, 100);
		theTrack->getPath()->addPoint({ 5.f, 0.f }, 100);
		theTrack->getPath()->addPoint({ 10.f, 5.f }, 100);
		updatePropertiesDisplay();
		//mainPanel->Refresh();
		resetView();
	}
	private: System::Void buttonInitTrain_Click(System::Object^ sender, System::EventArgs^ e) {
		if (theTrack != nullptr && theTrain != nullptr) {
			theTrain->initialize();
			mainPanel->Refresh();
		}
		else
			labelFeedback->Text = "ERROR: There are no trains in the model.";
	}

	private: void moveCarFraction(float fraction) {
		//if (theTrack != nullptr && theTrack->getPath() != nullptr) {
		//	if (theTrack->getCar() != nullptr) {
		//		theTrack->getCar()->setCurrLoc(theTrack->getCar()->getCurrLoc()
		//			+ theTrack->getPath()->length() * fraction);
		//		if (theTrack->getCar()->getCurrLoc() > theTrack->getPath()->length())
		//			theTrack->getCar()->setCurrLoc(0);
		//		mainPanel->Refresh();
		//	}
		//	else
		//		labelFeedback->Text = "ERROR: There are no cars in the model.";
		//}
		//else
		//	labelFeedback->Text = "Must load/create a track with a path to use feature.";
	}
	private: System::Void buttonMoveCarSmall_Click(System::Object^ sender, System::EventArgs^ e) {
		moveCarFraction(0.01);
	}
	private: System::Void buttonMoveCarLarge_Click(System::Object^ sender, System::EventArgs^ e) {
		moveCarFraction(0.05);
	}
	private: System::Void buttonMoveCarBack_Click(System::Object^ sender, System::EventArgs^ e) {
		moveCarFraction(-0.01);
	}
	private: System::Void buttonMoveCarTiny_Click(System::Object^ sender, System::EventArgs^ e) {
		moveCarFraction(0.001);
	}

	private: void simulate() {
		if (theTrain != nullptr && theTrack != nullptr && theTrack->getPath() != nullptr) {
			float timeStep = 0.05f;  // seconds

			float timeElapsedStep = 0, timeElapsedSim = 0; // seconds
			std::chrono::steady_clock::time_point simBegin, stepBegin; // #include <chrono>
			simBegin = std::chrono::high_resolution_clock::now();

			while (continueSimulation) {
				stepBegin = std::chrono::high_resolution_clock::now();
				theTrain->advanceTrain(timeStep);

				if (theTrain->getCurrLoc() > theTrack->getPath()->length())
					theTrain->initialize();
				else if (theTrain->getCurrLoc() < 0)
					//continueSimulation = false;  // stops simulation if reverses before start
					stopSimulation();

				timeElapsedSim = (std::chrono::duration_cast<std::chrono::milliseconds>
					(std::chrono::high_resolution_clock::now() - simBegin).count() / 1e3f);

				labelFeedback->Text = "Simulation running. Elapsed Time = " + timeElapsedSim.ToString("##0.000")
					+ ", Location = " + theTrain->getCurrLoc().ToString("0.000")
					+ ", Velocity = " + (theTrain->getCurrVel() * 3.6f).ToString("0.00") + " kph";
				labelFeedback->Refresh();

				mainPanel->Refresh();

				// wait for next step
				timeElapsedStep = std::chrono::duration_cast<std::chrono::microseconds>
					(std::chrono::high_resolution_clock::now() - stepBegin).count() / 1e6f;

				int sleepTime = (timeStep - timeElapsedStep) * 1e6f; // converting to microseconds
				std::this_thread::sleep_for(std::chrono::microseconds(sleepTime)); // include <thread>
			}

		}
		else {
			labelFeedback->Text = "Must load/create a track with a path AND a train to use feature.";
			stopSimulation();
		}
	}

	private: System::Void buttonSimStartStop_Click(System::Object^ sender, System::EventArgs^ e) {
		if (!continueSimulation) {
			continueSimulation = true;
			buttonSimStartStop->Text = "Pause Sim";
			buttonSimStartStop->BackColor = Color::Red;
			buttonSimStartStop->ForeColor = Color::White;

			// start a new thread
			// setting the following to false risks cross-thread issues, but these
			// risks are not very applicable here since it is what I want.
			System::Windows::Forms::Form::CheckForIllegalCrossThreadCalls = false;

			// define the thread that will run the simulation
			/*System::Threading::Thread^ */
			simulationThread = gcnew System::Threading::Thread(
				gcnew System::Threading::ThreadStart(this, &CoasterSimulator::simulate));

			// actually start the thread
			simulationThread->Start();
		}
		else
			stopSimulation();

	}
	private: void stopSimulation() {
		continueSimulation = false;
		buttonSimStartStop->Text = "Run Sim";
		buttonSimStartStop->BackColor = Color::Lime;
		buttonSimStartStop->ForeColor = Color::Black;
	}
	private: System::Void buttonMajorEdits_Click(System::Object^ sender, System::EventArgs^ e) {
		if (theTrack != nullptr && theTrack->getPath() != nullptr) {
			if (majorEditsForm == nullptr || !majorEditsForm->Visible) {
				if (majorEditsForm != nullptr)
					delete majorEditsForm;
				majorEditsForm = gcnew MajorEdits(theTrack->getPath(), mainPanel);
			}
			majorEditsForm->Show();
		}
		else
			labelFeedback->Text = "Must load/create a track to use feature.";

	}

	private: System::Void buttonDeleteCar_Click(System::Object^ sender, System::EventArgs^ e) {
		if (inEditMode && theTrain != nullptr && labelCurrCar->Text != "-") {
			auto confirm = MessageBox::Show("Car #" + labelCurrCar->Text+
				" will be permanently deleted.\n\nAre you sure ? ",
				"Are you sure?", MessageBoxButtons::YesNo, MessageBoxIcon::Warning);
			if (confirm == System::Windows::Forms::DialogResult::Yes) {
				theTrain->deleteCar(Convert::ToInt32(labelCurrCar->Text));
				mainPanel->Refresh();
			}

		}
	}
private: System::Void buttonControlEdit_Click(System::Object^ sender, System::EventArgs^ e) {
	if (theTrack != nullptr && theTrack->getPath() != nullptr) {
		if (controlEditsForm == nullptr || !controlEditsForm->Visible) {
			if (controlEditsForm != nullptr)
				delete controlEditsForm;
			controlEditsForm = gcnew ControlEdits(theTrack, mainPanel);
		}
		controlEditsForm->Show();
	}
	else
		labelFeedback->Text = "Must load/create a track to use feature.";
}


};
}

