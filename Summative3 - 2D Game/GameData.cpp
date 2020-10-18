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
	std::string Arg1;
	std::string Arg2;
	std::string Arg3;
	std::string Temp;
	std::stack<DataGroup*> Groups;
	std::stack<DataGroup*> Parent;
	DataGroup temp;
	Parent.push(&FileData);
	std::cout << "Started Reading" << std::endl;
	Groups.push(&FileData);
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
			std::cout << "File end reached" << std::endl;
			break;
		}
		
		
		std::getline(tagstream, Arg1, ':');
		
		
		if (Arg1 == "Group")
		{
			
			std::getline(tagstream, Arg2);
			
			std::cout << "Created Group: " << Arg2 << std::endl;
			Parent.push(Groups.top());
			Groups.push(new DataGroup(Arg2));
			
		}
		else if (tag == "/Group")
		{
			
			std::cout << "Closed Group: " << Groups.top()->GroupID << std::endl;
			Parent.top()->m_Groups.push_back(*Groups.top());
			Groups.pop();
			Parent.pop();
			
		}
		else
		{
			Data datamember;
			datamember.DataType = Arg1;
			std::getline(tagstream, datamember.DataID, ':');
			std::getline(tagstream, datamember.DataString);
			std::cout << "Created " << datamember.DataType << " member: " << datamember.DataID << " [" << datamember.DataString << "]" << std::endl;
			
			
			Groups.top()->m_Data.push_back(Data(datamember));
			

		}
	

	}
	std::cout << "Stopped Reading" << std::endl;
	Reader.close();
}

void GameData::AddVariable(Data dat)
{
	FileData.m_Data.push_back(dat);
}

void GameData::AddGroup(DataGroup datg)
{
	FileData.m_Groups.push_back(datg);
}



void GameData::Save(std::string Path, std::string Filename)
{
	std::ofstream Writer;
	Writer.open(Path + Filename + ".dat");
	Writer << "#" + Filename + "\n#DATA  FILE\n";
	Writer << SaveGroup(FileData);
	Writer << "<ENDFILE>";
	Writer.close();
}

std::string GameData::SaveGroup(DataGroup datg, int depth)
{
	std::string result = "";
	std::string indent = "";
	for (int i = 0; i < depth; i++)
	{
		indent += "\t";
	}
	for (Data dat : datg.m_Data)
	{
		result += indent + "<" + dat.DataType + ":" + dat.DataID + ":" + dat.DataString + ">\n";
	}
	for (DataGroup datg : datg.m_Groups)
	{
		result += indent + "<Group:" + datg.GroupID + ">\n";
		result += SaveGroup(datg, depth + 1);
		result += indent + "</Group>\n";
	}
	return result;
}



std::string GameData::GetByID(std::string ID, std::string GroupID)
{
	for (DataGroup dg : FileData.m_Groups)
	{
		if (dg.GroupID == GroupID)
		{
			for (Data d : dg.m_Data)
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

GameData::DataGroup::DataGroup()
{
}

GameData::DataGroup::DataGroup(std::string _name)
{
	GroupID = _name;
}

GameData::Data GameData::DataGroup::GetDataByID(std::string _ID)
{
	for (GameData::Data dat : m_Data)
	{
		if (dat.DataID == _ID)
		{
			return dat;
		}
	}
	return Data();
}

GameData::DataGroup GameData::DataGroup::GetGroupByID(std::string _ID)
{
	for (GameData::DataGroup datg : m_Groups)
	{
		if (datg.GroupID == _ID)
		{
			return datg;
		}
	}
	return DataGroup();
}

GameData::Data::Data()
{
}

GameData::Data::Data(const Data& copy)
{
	DataType = copy.DataType;
	DataID = copy.DataID;
	DataString = copy.DataString;
}
