#include "XMLParser.h"
#include <tinyxml2.h>
#include <iostream>
using namespace tinyxml2;
using namespace std;

XMLParser::XMLParser()
{
	/*XMLDocument xml_doc;

	XMLError eResult = xml_doc.LoadFile("./CONFIG.xml");
	if (eResult != XML_SUCCESS) {
		cout << "Error1";
		//return (1);
	}

	XMLNode* root = xml_doc.FirstChildElement("opendrone");
	if (root == nullptr) {
		cout << "Error2";
		//return (1);
	}

	XMLElement* element = root->FirstChildElement("droneName");
	if (element == nullptr) {
		cout << "Error3";
		//return (1);
	}
	else {
		cout << element->GetText();
		//XMLText *text = element->ToText();
		//cout << text->Value();
	}*/
}

XMLParser::~XMLParser()
{
}
