/*
 * xml_parser.cpp
 *
 *  Created on: Jul 8, 2020
 *      Author: ataylor
 */

#include "../includes/xml_parser.h"
#include <string>
#include <filesystem>
#include <fstream>
#include <iomanip>

namespace AT
{
	XMLParser::XMLParser()
		: m_Location(0),
		  m_MainIndexCount(0),
		  m_SubIndexCount(0)
	{

	}

	XMLParser::~XMLParser()
	{

	}

	int XMLParser::parse_xml_string(char* xmlData, size_t size)
	{
		if(xmlData == nullptr)
			return -1;

		for(m_Location = 0; m_Location < size; ++m_Location)
		{
			std::string value;
			int indentCount = 0;

			if((indentCount = parse_sub_index(xmlData)) > 0)
			{

				m_SubIndexCount += .00001;
				float newIndex = m_MainIndexCount + m_SubIndexCount;
				std::string convert = std::to_string(newIndex);
				std::string newString;
				for(unsigned long i = 0; i < convert.length(); ++i)
				{
					if(convert[i] != '0')
					{
						newString += convert[i];
					}
				}

				if((value = parse_between_brackets(xmlData)) != "NOT_A_VALUE")
					m_SubIndexes.emplace(std::stof(newString), value);

				if((value = parse_data(xmlData)) != "NOT_A_VALUE")
					m_Data.emplace(std::stof(newString), value);

				if((value = parse_to_eol(xmlData)) != "NOT_A_VALUE")
					continue;
			}
			else if(xmlData[m_Location + 1] != '/')
			{
				++m_MainIndexCount;
				m_SubIndexCount = 0;
			}


			if((value = parse_between_brackets(xmlData)) != "NOT_A_VALUE")
				m_MainIndexes.emplace(m_MainIndexCount, value);

			if((value = parse_data(xmlData)) != "NOT_A_VALUE")
				m_Data.emplace(m_MainIndexCount, value);

			if((value = parse_to_eol(xmlData)) != "NOT_A_VALUE")
				continue;
		}

		return 0;
	}

	int XMLParser::parse_xml_file(std::string filePath)
	{
		std::ifstream inFile(filePath);
		if(!inFile)
			return -1;

		long fileSize = std::filesystem::file_size(filePath);
		char* buff = new char[fileSize];
		if(inFile)
			inFile.read(buff, fileSize);

		return parse_xml_string(buff, fileSize);
	}

	std::string XMLParser::parse_between_brackets(char* data)
	{
		char charCheck = data[m_Location];
		std::string wordCheck;

		if(charCheck == '<')
		{
			charCheck = data[++m_Location];
			while(charCheck != '>')
			{
				if(charCheck == '/' && data[m_Location - 1] == '<')
					return "NOT_A_VALUE";
				wordCheck += charCheck;
				charCheck = data[++m_Location];
			}
			return wordCheck;
		}

		return "NOT_A_VALUE";
	}

	std::string XMLParser::parse_data(char* data)
	{
		char charCheck = data[m_Location];
		std::string wordCheck;

		if(charCheck == '>')
		{
			charCheck = data[++m_Location];
			while(charCheck != '<' && data[m_Location + 1] != '/')
			{
				if(charCheck == '\n')
					return "NOT_A_VALUE";
				wordCheck += charCheck;
				charCheck = data[++m_Location];
			}
			return wordCheck;
		}
		return "NOT_A_VALUE";
	}

	std::string XMLParser::parse_to_eol(char* data)
	{
		char charCheck = data[m_Location];
		if((charCheck == '<' && data[m_Location + 1] == '/') || (charCheck == '/' && data[m_Location - 1] == '<'))
		{
			while(charCheck != '\n')
			{
				++m_Location;
				charCheck = data[m_Location];
			}
			return "NOT_A_VALUE";
		}

		return "NOT_A_VALUE";
	}

	int XMLParser::parse_sub_index(char* data)
	{
		char charCheck = data[m_Location];
		int indentCount = 0;

		if(charCheck == '\t' || charCheck == ' ')
		{
			while(charCheck == '\t' || charCheck == ' ')
			{
				++m_Location;
				++indentCount;
				charCheck = data[m_Location];
			}
			return indentCount;
		}
		return 0;
	}
}


