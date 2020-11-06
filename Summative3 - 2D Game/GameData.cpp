#include "GameData.h"

Loadable::Loadable(std::string Filepath, std::string _name)
{
	FilePath = "Data/" + Filepath;;
	
	Data = new GameData(FilePath, _name + ".dat");
}

/// <summary>
/// Read from file and create Game Data
/// </summary>
/// <param name="Path"></param>
/// <param name="Filename"></param>
GameData::GameData(std::string Path, std::string Filename)
{
	
	std::ifstream Reader; //File reader
	Reader.open(Path+Filename); //Open file
	if (Reader.fail())//Create file if it doesnt exist
	{
		fs::create_directories(Path);
		std::ofstream Writer(Path+Filename);
		Writer << "#DATA FILE" << std::endl << "<ENDFILE>";
		Writer.close();
		Reader.open(Path+Filename);
	}
	bool bReading = true;

	//Args for Tags
	std::string Arg1;
	std::string Arg2;
	std::string Arg3;
	std::string Temp;

	//Data structure control groups
	std::stack<DataGroup*> Groups;
	std::stack<DataGroup*> Parent;
	DataGroup temp;

	Parent.push(&FileData); //Push main parent group
	std::cout << "Started Reading" << std::endl;
	Groups.push(&FileData);
	while (bReading)//While reading
	{
		std::string line;
		std::string tag;
		std::getline(Reader, line); //Get a line from the file
		if (line == ""||line[0] == '#') //Sip the line if its empty of starts with the comment character #
		{
			continue;
		}

		std::istringstream stream(line); //Make a stringstream from the line
		std::getline(stream, Temp, '<'); //Dump all characters before the first argument
		std::getline(stream, tag, '>'); //Get all text within the angle brackets to parse
		std::istringstream tagstream(tag); //create stringstream from tag

		if (tag == "ENDFILE")//If the argument is ENDFILE
		{
			bReading = false; //Stop reading
			std::cout << "File end reached" << std::endl;
			break;
		}
		
		
		std::getline(tagstream, Arg1, ':'); //Get the 1st Arg
		
		
		if (Arg1 == "Group") //Check if the first arg is a group declaration
		{
			
			std::getline(tagstream, Arg2);
			
			std::cout << "Created Group: " << Arg2 << std::endl;
			Parent.push(Groups.top());
			Groups.push(new DataGroup(Arg2));
			
		}
		else if (tag == "/Group") //Check if the first arg is a group termination
		{
			
			std::cout << "Closed Group: " << Groups.top()->GroupID << std::endl;
			Parent.top()->m_Groups.push_back(*Groups.top());
			Groups.pop();
			Parent.pop();
			
		}
		else //Treat the args as Member args
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
	Reader.close(); //Close file
}

/// <summary>
/// Serialises a member
/// </summary>
/// <param name="dat"></param>
void GameData::AddVariable(Data dat)
{
	FileData.m_Data.push_back(dat); //Add member
}

/// <summary>
/// Serialises a group
/// </summary>
/// <param name="datg"></param>
void GameData::AddGroup(DataGroup datg)
{
	FileData.m_Groups.push_back(datg); //Add group
}



void GameData::Save(std::string Path, std::string Filename) //Saves serialised data to a file
{
	std::ofstream Writer;
	Writer.open(Path + Filename + ".dat");
	Writer << "#" + Filename + "\n#DATA  FILE\n";
	Writer << SaveGroup(FileData);
	Writer << "<ENDFILE>";
	Writer.close();
}

std::string GameData::SaveGroup(DataGroup datg, int depth) //Generates string from group
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



/// <summary>
/// Constructor
/// </summary>
GameData::DataGroup::DataGroup()
{
}

/// <summary>
/// Constructor
/// </summary>
/// <param name="_name"></param>
GameData::DataGroup::DataGroup(std::string _name)
{
	GroupID = _name;
}

/// <summary>
/// Gets data by its ID
/// </summary>
/// <param name="_ID"></param>
/// <returns></returns>
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

/// <summary>
/// Gets group by its ID
/// </summary>
/// <param name="_ID"></param>
/// <returns></returns>
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

/// <summary>
/// Constructor
/// </summary>
GameData::Data::Data()
{
}

/// <summary>
/// Copy constructor
/// </summary>
/// <param name="copy"></param>
GameData::Data::Data(const Data& copy)
{
	DataType = copy.DataType;
	DataID = copy.DataID;
	DataString = copy.DataString;
}
