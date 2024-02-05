#include <iostream>
#include <fstream>

class ConfigAttributes
{
public:
	int influenceRadiusConfig;

	ConfigAttributes()
	{
		readConfiguration();
	}

	void readConfiguration()
	{
		std::ifstream configFile;
		configFile.open("config.txt");
		std::string line;

		if (configFile.is_open())
		{
			while (configFile)
			{
				std::getline(configFile, line);
				
				if (line != "" && line.substr(0, 1) != "$")
				{
					std::string name = line.substr(0, line.find("=") - 1);
					
					if (name == "influenceRadius")
					{
						influenceRadiusConfig = std::stoi(line.substr(line.find("=") + 1, line.length()));
					}
				}
			}
		}
		else
		{
			std::cout << "Couldn't open file\n";
		}
	}
};