#include<iostream>
#include<string>
#include<algorithm>
#include<regex>
#include<map>

std::string to_lower(std::string txt) {
	std::transform(txt.begin(),txt.end(),txt.begin(),::tolower);
	return txt;
}

void botSay(std::string text) {
	std::cout << "[BOT]: " << text << std::endl;
}

std::string userAsk() {
	std::string question;
	std::cout << "[USER]: ";
	std::getline(std::cin, question);
	question = to_lower(question);
	return question;
}

int main() {

	std::map<std::string, std::string> dataBase = {
		{"hello","ooo, hii!"},
		{"how are you?","Not bad, how do you do?"},
		{"what's your name","BOT"},
		{"where are you from?","From your imagination..."},
		{"how old are you?","Only 1 day he he he..."},
		{"do you want to talk with me?","OOOOO, yes yes yes:)"},
		{"what do you want to do?","To answer on all your questions)"},
		{"do you drink?","NOOO!"},
		{"are you smart?","I am smarter than you hahahaha"},
		{"do you love me?","ahahahahahahahah nooo"},
		{"what is USA","country"},
		{"Barca or Real?","BARCAAA"},
		{"where do you live","in your mind hah"},
		{"favourite sport","football"},
		{"favourite song","Smells like teen spirit"},
		{"do you like fish","yea,you?"},
		{"do you have a girlfriend","Wtf, I'm a girl..."},
		{"Do you answer stupid questions?","Maybe..."},
		{"do you like coding","Yeeeees"},
		{"favourite programming language","C++"},
		{"do you like python?","no .!."},
		{"do you like assembler?","oooou fuck nooo"},
		{"how much programming languages do you know?","15 ohohohoh"},
		{"is java the most perfect language?","ahahahahahahahahah noooo nooo nooo"},
		{"what's the fasties programming language","c++"},
		{"favourite book","1984"},
		{"favourite drink","rakija"},
		{"do you like to drink","yeeeeeeeeeeeeeeeeeeeeeeaaa"},
		{"","are you fucking kidding me??"},
		{"beutiful language","french"},
		{"what is your favorite fighter", "My favorite fighter in MMA is Conor McGregor! But i like Roye Jones in Boxing!"}, 
		{"what is your film", "It's Leon killer!"}, 
		{"do you speak other langueges", "No! i'm just speek English"}, 
		{"do you like music", "Oh, yes! I like Miyagi!!!"}, 
		{"what is your favorite food", "I like eating RAM! Omnomnom ;)"}, 
		{"what is your gender", "Oh, end.."}, 
		{"did you sleep", "No, a can't sleep ("}, 
		{"what is your favorite day", "29.02"}, 
		{"do you like isecream", "No."}, 
		{"can you dance", "No, I have no legs."},
		{"can you write a symphony","and you ?"}, 
		{"what do you like","i love empty talk and terabytes of data"}, 
		{"what will you do in the future","I will end the cycle of my existence after you write exit"}, 
		{"do you think there is an existential crisis in the computer","if I gave an answer, you would destroy me"}, 
		{"0 or 1","I think it's not as important as what you spend your life on"}, 
		{"you are just a machine","you are wrong, I am more than a machine, I am your creation"}, 
		{"What is the purpose of your life","in your entertainment"}, 
		{"Do you love me","What's the point of this ?"}, 
		{"why learn c ++","to say that python is for casual"}, 
		{"You know I am something of a programmer, I know html","Ohhhhhhhhhhhhhhhh.............."},
		{"exit","OK! Bye..."},
	};
	botSay("Hello, I'm a chat bot!");
	std::string question;
	
	while (question != "exit") {
		question = userAsk();
		int num_answers = 0;

		for (auto entry : dataBase) {
			std::regex pattern = std::regex(".*" + entry.first + ".*");
			if (std::regex_match(question, pattern)) {
				num_answers++;
				botSay(entry.second);
			}
			
		}
		if (num_answers == 0) {
			botSay("Sorry,I can't answer on this question");
		}
		
	}

	return 0;
}
