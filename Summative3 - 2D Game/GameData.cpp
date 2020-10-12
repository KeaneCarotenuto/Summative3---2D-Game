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
	std::string Arg3;
	std::string Temp;
	std::stack<DataGroup> Groups;
	Groups.push(Misc);
	while (bReading)
	{
		std::string line;
		std::string tag;
		std::getline(Reader, line);
		if (line == ""||line[0] == '#')
		{
			continue;
		}

		std::istringstream stream(line);
		std::getline(stream, Temp, '<');
		std::getline(stream, tag, '>');
		std::istringstream tagstream(tag);

		if (tag == "ENDFILE")
		{
			bReading = false;
			break;
		}
		
		
		std::getline(tagstream, Arg1, ':');
		
		
		if (Arg1 == "Group")
		{
			DataGroup temp;
			std::getline(tagstream, temp.GroupID);
			Groups.push(temp);
		}
		else if (tag == "/Group")
		{
			FileData.push_back(Groups.top());
			Groups.pop();
		}
		else
		{
			Data datamember;
			datamember.DataType = Arg1;
			std::getline(tagstream, datamember.DataID, ':');
			std::getline(tagstream, datamember.DataString);
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
