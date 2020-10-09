#include "GameData.h"

Loadable::Loadable(std::string Filepath, std::string _name)
{
	FilePath = "Data/" + Filepath;;
	
	Data = new GameData(FilePath, _name + ".dat");
}

GameData::GameData(std::string Path, std::string Filename)
{
	
	std::ifstream Reader;
	Reader.open(Path+Filename);
	if (Reader.fail())
	{
		fs::create_directories(Path);
		std::ofstream Writer(Path+Filename);
		Writer << "#DATA FILE" << std::endl << "<ENDFILE>";
		Writer.close();
		Reader.open(Path+Filename);
	}
	bool bReading = true;
	DataGroup Misc;
	std::string Arg1;
	std::string Arg2;
	std::string Temp;
	std::stack<DataGroup> Groups;
	Groups.push(Misc);
	while (bReading)
	{
		std::string line;
		std::getline(Reader, line);
		if (line == ""||line[0] == '#')
		{
			continue;
		}
		if (line == "<ENDFILE>")
		{
			bReading = false;
			break;
		}
		std::istringstream stream(line);
		std::getline(stream, Temp, '<');
		std::getline(stream, Arg1, ':');
		std::getline(stream, Arg2, '>');
		if (Arg1 == "Group")
		{
			DataGroup temp;
			temp.GroupID = Arg2;
			Groups.push(temp);
		}
		else if (Arg1 == "/Group")
		{
			FileData.push_back(Groups.top());
			Groups.pop();
		}
		else
		{
			Data datamember;
			datamember.DataID = Arg1;
			datamember.DataString = Arg2;
			Groups.top().Data.push_back(datamember);
		}
	

	}
	FileData.push_back(Groups.top());
	Reader.close();
}

std::string GameData::GetByID(std::string ID, std::string GroupID)
{
	for (DataGroup dg : FileData)
	{
		if (dg.GroupID == GroupID)
		{
			for (Data d : dg.Data)
			{
				if (d.DataID == ID)
				{
					return d.DataString;
				}
			}
		}
	}
	return std::string();
}
