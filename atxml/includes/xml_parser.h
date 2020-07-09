/*
 * xml_parser.h
 *
 *  Created on: Jul 8, 2020
 *      Author: ataylor
 */

#ifndef INCLUDES_XML_PARSER_H_
#define INCLUDES_XML_PARSER_H_

#include <vector>
#include <map>

typedef std::map<long /*Main #*/, std::string> MainIndexMap;
typedef std::map<float /*Sub #*/, std::string> SubIndexMap;
typedef std::map<float /*Index to data */, std::string> DataMap;

namespace AT
{
	class XMLParser
	{
	public:
		XMLParser();
		~XMLParser();

		int parse_xml_string(char* xmlData, size_t size);
		int parse_xml_file(std::string filePath);

		MainIndexMap return_main_indexes() { return m_MainIndexes; }
		SubIndexMap return_sub_indexes() { return m_SubIndexes; }
		DataMap return_data() { return m_Data; }
	private:
		MainIndexMap m_MainIndexes;
		SubIndexMap m_SubIndexes;
		DataMap m_Data;

		unsigned long m_Location;
		unsigned int m_MainIndexCount;
		float m_SubIndexCount;
	private:
		std::string parse_between_brackets(char* data);
		std::string parse_data(char* data);
		std::string parse_to_eol(char* data);
		int parse_sub_index(char* data);
	};
}



#endif /* INCLUDES_XML_PARSER_H_ */
