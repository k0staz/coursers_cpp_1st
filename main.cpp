#include <iostream>
#include <string>
#include <map>
#include <set>
#include <exception>
#include <iomanip>

using namespace std;

// Реализуйте функции и методы классов и при необходимости добавьте свои



class Date {
public:
    Date(){
        year = 0;
        month = 0;
        day = 0;
    }

    Date(int new_year, int new_month, int new_day){
        if(new_month > 12 || new_month < 1){
            throw invalid_argument("Month value is invalid: " + to_string(new_month));
        }
        if(new_day > 31 || new_day < 1){
            throw invalid_argument("Day value is invalid: " + to_string(new_day));
        }
        year = new_year;
        month = new_month;
        day = new_day;
    }

    int GetYear() const{
        return year;
    };
    int GetMonth() const{
        return month;
    };
    int GetDay() const{
        return day;
    };

private:
    int year, month, day;
};

bool operator<(const Date& lhs, const Date& rhs){
    if(lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() == rhs.GetMonth()){
        return lhs.GetDay() < rhs.GetDay();
    }
    else if(lhs.GetYear() == rhs.GetYear()){
        return lhs.GetMonth() < rhs.GetMonth();
    }
    else{
        return lhs.GetYear() < rhs.GetYear();
    }
}

ostream& operator<<(ostream& os, const Date& date){
    return os << setw(4) << setfill('0') << date.GetYear() << '-'
    << setw(2) << setfill('0') << date.GetMonth() << '-'
    << setw(2) << setfill('0') << date.GetDay();
}



class Database {
public:
    void AddEvent(const Date& date, const string& event){
        events[date].insert(event);
    };
    bool DeleteEvent(const Date& date, const string& event){
        if(events[date].count(event) == 1){
            events[date].erase(event);
            return true;
        }
        return false;
    };
    int  DeleteDate(const Date& date){
        int n = events[date].size();
        events.erase(date);
        return n;
    };

    void Find(const Date& date) const{
        for(const string& e : events.at(date)){
            cout << e << endl;
        }
    };

    void Print() const{
        for(const auto& d : events){
            for(const string& e : d.second){
                cout << d.first << ' ' << e << endl;
            }
        }
    };

private:
    map<Date, set<string>> events;
};

Date ParseDate(const string& s){
    stringstream stream(s);
    int y = 0, m = 0, d = 0;
    stream >> y;
    if (stream.peek() != '-') {
        throw runtime_error("Wrong date format: " + s);
    }
    stream.ignore(1);
    if ((stream.peek() < '0' || stream.peek() > '9') && stream.peek()!= '+' && stream.peek() != '-') {
        throw runtime_error("Wrong date format: " + s);
    }
    stream >> m;
    if (stream.peek() != '-') {
        throw runtime_error("Wrong date format: " + s);
    }
    stream.ignore(1);
    if ((stream.peek() < '0' || stream.peek() > '9') && stream.peek()!= '+' && stream.peek() != '-') {
        throw runtime_error("Wrong date format: " + s);
    }
    stream >> d;
    if (stream.peek() != EOF) {
        throw runtime_error("Wrong date format: " + s);
    }
    return {y,m,d};
}

int main() {
    Database db;

    string command;
    while (getline(cin, command)) {
        // Считайте команды с потока ввода и обработайте каждую
        stringstream stream(command);
        string oper;
        stream >> oper;
        if(oper == "Add"){
            string string_date;
            stream >> string_date;
            Date date;
            string event;
            try{
                date = ParseDate(string_date);
            } catch (invalid_argument& ex){
                cout << ex.what();
                return 0;
            } catch (runtime_error& ex){
                cout << ex.what();
                return 0;
            }
            stream >> event;
            db.AddEvent(date, event);
        }
        else if(oper == "Del"){
            Date date;
            string event, string_date;
            stream >> string_date;
            try{
                date = ParseDate(string_date);
            } catch (invalid_argument& ex){
                cout << ex.what();
                return 0;
            } catch (runtime_error& ex){
                cout << ex.what();
                return 0;
            }
            stream >> event;
            if(event != ""){
                if(db.DeleteEvent(date, event)){
                    cout << "Deleted successfully" << endl;
                } else{
                    cout << "Event not found" << endl;
                }
            } else{
                cout << "Deleted " + to_string(db.DeleteDate(date)) + " events" << endl;
            }
        }
        else if(oper == "Find"){
            Date date;
            string string_date;
            stream >> string_date;
            try{
                date = ParseDate(string_date);
            } catch (invalid_argument& ex){
                cout << ex.what();
                return 0;
            } catch (runtime_error& ex){
                cout << ex.what();
                return 0;
            }
            try{
                db.Find(date);
            } catch (exception& ex){

            }

        }
        else if(oper == "Print"){
            db.Print();
        }
        else if(oper != ""){
            cout << "Unknown command: " + oper << endl;
        }

    }

    return 0;
}