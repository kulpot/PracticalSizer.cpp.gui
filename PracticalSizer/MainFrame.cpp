#include "MainFrame.h"
#include <wx/wx.h>
#include <vector>
#include <string>
#include "Task.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
//MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) //<-- None Resizable Layout
{
	//SetClientSize(1000, 600);
	//
	//wxPanel* panel = new wxPanel(this);
	//button = new wxButton(panel, wxID_ANY, "Centered Button", wxPoint(375, 250), wxSize(250, 100));

	//Bind(wxEVT_SIZE, &MainFrame::OnResize, this);

	CreateControls();
	BindEventHandlers();
	AddSavedTasks(); // important: must be after CreateControls();
}

//void MainFrame::OnResize(wxSizeEvent& evt)
//{
//	//wxSize clientSize = GetClientSize();
//	//
//	//wxSize buttonSize(clientSize.GetWidth() * 0.25, 100);
//	//int buttonX = (clientSize.GetWidth() - buttonSize.GetWidth()) / 2;
//	//int buttonY = (clientSize.GetHeight() - buttonSize.GetHeight()) / 2;
//	//
//	//button->SetSize(buttonSize);
//	//button->SetPosition(wxPoint(buttonX, buttonY));
//	//
//	//evt.Skip();
//}

void MainFrame::CreateControls()
{
	wxFont headlineFont(wxFontInfo(wxSize(0, 36)).Bold());
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));

	panel = new wxPanel(this);
	panel->SetFont(mainFont);

	headlineText = new wxStaticText(panel, wxID_ANY, "To-Do List",
		wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	headlineText->SetFont(headlineFont);
	//headlineText->SetBackgroundColour(*wxRED);
	
	inputField = new wxTextCtrl(panel, wxID_ANY, "",
		wxPoint(100, 80), wxSize(495, 35), wxTE_PROCESS_ENTER);
	addButton = new wxButton(panel, wxID_ANY, "Add", wxPoint(600, 80), wxSize(100, 35));
	checkListBox = new wxCheckListBox(panel, wxID_ANY, wxPoint(100, 120), wxSize(600, 400));
	clearButton = new wxButton(panel, wxID_ANY, "Clear", wxPoint(100, 525), wxSize(100, 35));
	
	// (FOR BLURRINESS PROBLEM)->ProjectProperties->Manifest Tool->Input and Output->DPI Awareness->High DPI Aware->Apply

	//headlineText = new wxStaticText(panel, wxID_ANY, "To-Do List");
	//headlineText->SetFont(headlineFont);
	//
	//inputField = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	//addButton = new wxButton(panel, wxID_ANY, "Add", wxDefaultPosition, wxSize(100, -1));
	//checkListBox = new wxCheckListBox(panel, wxID_ANY);
	//clearButton = new wxButton(panel, wxID_ANY, "Clear");
	//
	//wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	//mainSizer->Add(headlineText, wxSizerFlags().CenterHorizontal().Border(wxTOP | wxBOTTOM, 10));

	//wxBoxSizer* inputSizer = new wxBoxSizer

}

void MainFrame::BindEventHandlers()
{
	addButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddButtonClicked, this);
	inputField->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnInputEnter, this);
	checkListBox->Bind(wxEVT_KEY_DOWN, &MainFrame::OnListKeyDown, this);
	clearButton->Bind(wxEVT_BUTTON, &MainFrame::OnClearButtonClicked, this);
	this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnWindowClosed, this);
}

void MainFrame::AddSavedTasks()
{
	std::vector<Task> tasks = LoadTasksFromFile("tasks.txt");

	for (const Task& task : tasks) {
		int index = checkListBox->GetCount();
		checkListBox->Insert(task.description, index);
		checkListBox->Check(index, task.done);
	}
}

void MainFrame::OnAddButtonClicked(wxCommandEvent& evt)
{
	AddTaskFromInput();
}

void MainFrame::OnInputEnter(wxCommandEvent& evt)
{
	AddTaskFromInput();
}

void MainFrame::OnListKeyDown(wxKeyEvent& evt)
{
	switch (evt.GetKeyCode()) {
		case WXK_DELETE:
			DeleteSelectedTask();
			break;
		case WXK_UP:
			MoveSelectedTask(-1);
			break;
		case WXK_DOWN:
			MoveSelectedTask(+1);
			break;
	}
}

void MainFrame::OnClearButtonClicked(wxCommandEvent& evt)
{
	if (checkListBox->IsEmpty()) {
		return;
	}

	wxMessageDialog dialog(this, "Are you sure you want to clear all task?",
		"Clear", wxYES_NO | wxCANCEL);
	int result = dialog.ShowModal();

	if (result == wxID_YES) {
		checkListBox->Clear();
	}
}

void MainFrame::OnWindowClosed(wxCloseEvent& evt)
{
	std::vector<Task> tasks;

	for (int i = 0; i < checkListBox->GetCount(); i++)
	{
		Task task;
		task.description = checkListBox->GetString(i);
		task.done = checkListBox->IsChecked(i);
		tasks.push_back(task);
	}

	SaveTasksToFile(tasks, "tasks.txt");
	evt.Skip();
}

void MainFrame::AddTaskFromInput()
{
	wxString description = inputField->GetValue();

	if (!description.IsEmpty()) {
		checkListBox->Insert(description, checkListBox->GetCount());
		inputField->Clear();
	}

	inputField->SetFocus();
}

void MainFrame::DeleteSelectedTask()
{
	int selectedIndex = checkListBox->GetSelection();

	if (selectedIndex == wxNOT_FOUND) {
		return;
	}

	checkListBox->Delete(selectedIndex);
}

void MainFrame::MoveSelectedTask(int offset)
{
	int selectedIndex = checkListBox->GetSelection();

	if (selectedIndex == wxNOT_FOUND) {
		return;
	}

	int newIndex = selectedIndex + offset;

	if (newIndex >= 0 && newIndex < checkListBox->GetCount()) {
		SwapTasks(selectedIndex, newIndex);
		checkListBox->SetSelection(newIndex, true);
	}
}

void MainFrame::SwapTasks(int i, int j)
{
	Task taskI{ checkListBox->GetString(i).ToStdString(), checkListBox->IsChecked(i) };
	Task taskJ{ checkListBox->GetString(j).ToStdString(), checkListBox->IsChecked(j) };

	checkListBox->SetString(i, taskJ.description);
	checkListBox->Check(i, taskJ.done);

	checkListBox->SetString(j, taskI.description);
	checkListBox->Check(j, taskI.done);

}
