#include <iostream>
#include <uwebsockets/App.h>
#include <regex>
#include <map>

struct UserConnection {
    std::string name;
    unsigned long user_id;
    uWS::WebSocket<false, true>* ws;
    void virtual sendMessage(std::string message, UserConnection& sender) {
        ws->send("[" + sender.name + "]: " + message, uWS::OpCode::TEXT);
    }
};

struct BotConnection : UserConnection {
    std::map<std::string, std::string> database = {
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
    void sendMessage(std::string message, UserConnection& sender) {
        bool isAnswerFound = false;
        for (auto entry : database) {
            auto pattern = std::regex(".*" + entry.first + ".*");
            if (std::regex_match(message, pattern)) {
                sender.sendMessage(entry.second, *this);
                isAnswerFound = true;
            }
        }
        if (!isAnswerFound) {
            sender.sendMessage("Sorry I do not understand", *this);
        }
    }
};

int main()
{
    int port = 8888;
    unsigned long latest_user_id = 10;
    std::regex SET_NAME("SET_NAME=.*");
    std::regex MESSAGE_TO("MESSAGE_TO=\d+,.*");
    std::map<unsigned long, UserConnection&> online_users;

    BotConnection bot;
    bot.user_id = 1;
    bot.name = "BOT";

    online_users.emplace(1, bot);

    uWS::App()
        .get("/", [](auto* response, auto* request) {
        response->writeHeader("Content-Security-Policy", "default-src 'self'")->end("asdf");
            })
        .ws<UserConnection>("/*", {
            .open = [&latest_user_id, &online_users](auto* ws) {
                auto data = static_cast<UserConnection*>(ws->getUserData());
                data->user_id = ++latest_user_id;
                data->name = "anonymous";
                data->ws = ws;
                std::cout << "New user connected, id: " << data->user_id << std::endl;
                ws->subscribe("broadcast");
                ws->subscribe("user#" + std::to_string(data->user_id));


                online_users.emplace(data->user_id, *data);
                std::cout << "Total users connected: " << online_users.size() << std::endl;
            },
            .message = [&online_users](auto* ws, std::string_view message, uWS::OpCode opCode) {
                std::cout << "Messsage received (opCode " << std::to_string(opCode) << "): " << message << std::endl;
                auto data = static_cast<UserConnection*>(ws->getUserData());
                //std::smatch matchResult;
                //std::regex_match(std::string(message), matchResult, SET_NAME);
                std::string SET_NAME("SET_NAME=");
                std::string MESSAGE_TO("MESSAGE_TO=");
                if (0 == message.find(SET_NAME)) {
                    //data->name = message.remove_prefix(9);
                    auto name = message.substr(SET_NAME.length());
                    auto comma_position = message.find(",");
                    if (std::string::npos == comma_position && name.length() <= 255){
                        data->name = name;
                        std::cout << "User set name: " << data->name << std::endl;
                        ws->publish("broadcast", "NEW_USER=" + std::to_string(data->user_id) + "," + data->name);
                    }else{
                        ws->send("Invalid name", opCode);
                    }
                } else if (0 == message.find(MESSAGE_TO)) {
                        auto str = std::string(message.substr(MESSAGE_TO.length()));
                        auto comma_position = str.find(",");
                        auto id_str = str.substr(0, comma_position);
                        unsigned long id;
                        try
                        {
                            id = std::stoul(id_str);
                        }
                        catch (const std::invalid_argument&)
                        {
                            ws->send("Invalid user id", opCode);
                            return;
                        }
                        auto i = online_users.find(id);
                        if (online_users.end() == i) {
                            ws->send("Error, there is no user with ID = " + std::to_string(id), opCode);
                            return;
                        }
                        auto text = str.substr(comma_position + 1);
                        //ws->publish("user#" + std::to_string(id), text);
                        i->second.sendMessage(text, *data);
                }
            },
                .close = [&online_users](auto* ws, int code, std::string_view message) {
                //std::cout << "close " << code << ", " << message << std::endl;
                auto data = static_cast<UserConnection*>(ws->getUserData());
                std::cout << "Disconnect user id: " << data->user_id << ", name: " << data->name << std::endl;
                online_users.erase(data->user_id);
                std::cout << "Total users connected: " << online_users.size() << std::endl;
                }
        })
                .listen(port, [port](auto* token) {
                    if (token)
                    {
                        std::cout << "Server started on port " << port << std::endl;
                    } else
                    {
                        std::cout << "Server failed to start" << std::endl;
                    }
                })
                    .run();

}
