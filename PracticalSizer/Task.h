#pragma once
#include <string>
#include <vector>

//Requirements: Open Folder->C:\Users\sunny\wxWidgets-\build\msw ->click:wx_vc16.sln
//build: Debug->32->Release->32/Debug->x64->Release->x64
//File->New->Project		//ref link:Ep1 Installing wxWidgets -->https://www.youtube.com/watch?v=ONYW3hBbk-8&list=PLFk1_lkqT8MbVOcwEppCPfjGOGhLvcf9G&index=3 
//View->Other Windows->Property Manager->right click:ToDoList->Add Existing Property Sheet..->C:\Users\wxWidgets-\wxwidgets.props


struct Task
{
	std::string description;
	bool done;
};

void SaveTasksToFile(const std::vector<Task>& tasks, const std::string& fileName);
std::vector<Task> LoadTasksFromFile(const std::string& fileName);

