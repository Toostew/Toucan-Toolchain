//this is the preprocessor

#include "preprocess.h"



//MacroTable
MacroTable::MacroTable(){
	
}

std::string MacroTable::search(std::string key){
	auto iterator = hashmap.find(key);

	if(iterator != hashmap.end()){
		return iterator->second; //returns the value associated with iterator->first
	} else {
		return ""; //there's probably a more reliable return than just straight blank
	}

}

void MacroTable::define(std::string key, std::string value){
	hashmap[key] = value; //define the value to macro table
}

void MacroTable::undefine(std::string key){
	hashmap.erase(key); //remove the std::pair of this key, which removes both key and value associated
}

std::vector<std::string> MacroTable::getMacros() {
	std::vector<std::string> macros;

	for (std::pair<std::string,std::string> entry : hashmap) {
		macros.push_back(entry.first);
	}

	return macros;
}




//preprocessor
PreProcessor::PreProcessor(){

}

//scans the line for a directive.
//anything that doesn't immediately start with # is branded as NON_DIRECTIVE
directiveType PreProcessor::getLineDirective(std::string line) {
	std::string bufferFirst;
	std::string bufferRemainder;

	std::istringstream lineStream(line);

	lineStream >> bufferFirst; //load the first non-whitespaced full word

	if (bufferFirst == "#include") {
		return directiveType::INCLUDE;
	}
	else if (bufferFirst == "#define") {
		return directiveType::DEFINE;
	}
	else if (bufferFirst == "#undefine") {
		return directiveType::UNDEFINE;
	} else {
		return directiveType::NON_DIRECTIVE;
	}

}

//search the stack for the file. If it exists, 1. Otherwise 0
//there should never be multiple of the same file to prevent cycles
bool PreProcessor::searchFileEntryStack(std::string fileName) {

	//const is like a declaration to say we will not modify anything related to fileEntry, and Auto& is reference to whatever variable this is (automatically detect)
	for (const auto& fileEntry : fileEntryStack) {
		if (fileEntry.fileName == fileName) {
			return true;
		}
	}
	return false;
}


void PreProcessor::includeHandler(std::string line) {
	std::string buffer;
	std::istringstream lineStream(line);

	lineStream >> buffer >> buffer; //skip the #include, we should get a file name.
	//at this point buffer should be populated with the name of the file,
	//RULE: no quotations allowed for now (check readme)

	//check for filename in stack
	if (!PreProcessor::searchFileEntryStack(buffer)) {
		//perfectly fine, continue on

		std::unique_ptr<std::ifstream> openFileStream = std::make_unique<std::ifstream>(buffer);

		//check to see if the file opens, hence actually exists/readable
		if (!openFileStream->is_open()) {
			//error, exit out
			std::cerr << "Could not open file " << buffer << std::endl;
			exit(20); //code 20 is special code for unable to open file
		}

		//file is actually openable, so we can add it to the stack
		fileEntry wrapperEntry = {buffer, std::move(openFileStream)};

		fileEntryStack.push_back(std::move(wrapperEntry));




	} else {
		//cycle detected, should error out
		std::cerr << "Cycle detected for: " << buffer << ", quitting"<< std::endl;
		exit(10); //code 10 is special code for cycles
	}
};

void PreProcessor::defineHandler(std::string line) {
	std::string key;
	std::string value;
	std::istringstream lineStream(line);

	lineStream >> key >> key; //skip the #define, we should get a file name.
	//read pointer now lies on the first whitespace beyond the key.
	//everything past this point is considered as the value, except trailing whitespace
	std::getline(lineStream, value);
	size_t start = value.find_first_not_of(" \t"); //find the first non-whitespace character


	if (start != std::string::npos) {
		value = value.substr(start);
	} else {
		value.clear(); // The value was just whitespace
	}

	PreProcessor::macroTable.define(key, value);


};

//remove the value associated with the stated key
void PreProcessor::undefineHandler(std::string line) {
	std::string key;
	std::istringstream lineStream(line);

	lineStream >> key >> key;

	PreProcessor::macroTable.undefine(key);
};

void PreProcessor::processFile(std::string inputFile, std::string outputFile){
	std::string buffer;

	//open inputStream and outputStream
	std::ofstream writeFile(outputFile); //this creates a new file of name outputFile

	fileEntry wrappedEntry = {inputFile, std::make_unique<std::ifstream>(inputFile)};

	fileEntryStack.push_back(std::move(wrappedEntry)); //add entry to end of stack

	while(!fileEntryStack.empty()){
		fileEntry* currentEntry = &fileEntryStack.back();

		if(!std::getline(*currentEntry->fileStream, buffer)){
			//if getline returns nothing, we can move on
			fileEntryStack.pop_back();
			continue;
		}

		directiveType type = getLineDirective(buffer);
		std::string strippedBuffer = stripComment(buffer); //this is the line without comments
		switch(type) {
			case directiveType::INCLUDE:
				includeHandler(strippedBuffer);
				break;
			case directiveType::DEFINE:
				defineHandler(strippedBuffer);
				break;
			case directiveType::UNDEFINE:
				undefineHandler(strippedBuffer);
				break;
			case directiveType::NON_DIRECTIVE:
				//here we do macro expansion.
				std::string processedLine = expandMacro(strippedBuffer);
				writeFile << processedLine << "\n";
				break;

		}

	}

}

//checks if the char is a valid boundary char (to distinguish unique matches with substring matches)
bool PreProcessor::isValidBoundary(char c) {
	return c == ' '  ||
		   c == '('  ||
		   c == ')'  ||
		   c == '\t' ||
		   c == '+' ||
		   c == '-' ||
		   c == ';' ||
		   c == '=' ||
		   c == ',' ||
		   c == '{' ||
		   c == '}' ||
		   c == '<' ||
		   c == '>';

}


std::string PreProcessor::expandMacro(std::string line) {
	//this function expands the macro from its compact to full form
	//we have to check the ENTIRE macrotable and search the line for each entry
	//so for each ENTRY, we need to search EVERY OCCURENCE of the word in the line

	std::vector<std::string> macros = macroTable.getMacros();


	//const means we aren't going to change the value (good for readability)
	//string& means we use references rather than copy the values
	for (const std::string& entry : macros) {

		size_t matchPos = line.find(entry, 0); //search from the very first position
		std::string value = macroTable.search(entry); //this feels  redundant if we just returned both key and value as std::pair...

		//std::string::find returns the index of the first char of a match, or npos if no matches
		while (matchPos != std::string::npos) {

			//check char before match
			bool beforeMatchValid = false;
			if (matchPos == 0) {
				beforeMatchValid = true;
			} else {
				char charBeforeMatch = line[matchPos - 1]; //check the character 1 index BEFORE the start pos
				beforeMatchValid = isValidBoundary(charBeforeMatch);
			}

			//check char after match
			bool afterMatchValid = false;
			size_t indexAfterMatch = matchPos + entry.length(); //this puts the position to the char 1 index after the match
			if (indexAfterMatch >= line.length()) {
				afterMatchValid = true; //the next index after the match is beyond the boundary of the string
			} else {
				char charAfterMatch = line[indexAfterMatch];
				afterMatchValid = isValidBoundary(charAfterMatch);
			}

			if (beforeMatchValid && afterMatchValid) {
				//this is a valid occurrence of the macro, we can now expand it.
				line.replace(matchPos, entry.length(), value);


				matchPos = line.find(entry, matchPos + value.length()); //check the rest of the line AFTER the occurance, to avoid macro expansion
			} else {


				matchPos = line.find(entry, matchPos + 1); //check starting from the next position
			}
		}
	}

	return line;
}

std::string PreProcessor::stripCommentSimple(std::string line) {
	//if it starts with a "//", like this comment, remove the remainder of the line
	//ideally, the preprocessor can detect comments anywhere.
	//this current simple version only strips comments that appear standalone
	//this function cannot detect comments that are at the end of of valid non-comment lines (which is expected in every other language)

	return ""; //literally nothing
}

//this function is run before handing off lines for processing, to strip the line of any comments
std::string PreProcessor::stripComment(std::string line) {
	//actually dynamically strip comments

	size_t pos = line.find("//");
	if (pos != std::string::npos) {
		return line.erase(pos); //wipe everything to the right of the //
	}

	//if npos, then there is no comment
	return line;
}







