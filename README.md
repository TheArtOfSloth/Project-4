# Reminder
Project 3 of CS 4310. This project encompasses creation of a reminder application.

PROJECT 3 : REMINDER APPLICATION
--------------------------------

//Implement clock with std::chrono::system_clock or possibly with 
// http://en.cppreference.com/w/cpp/chrono/system_clock
//Send the group some videos on chrono.
CLASSES : 

//Link Events together with a queue or perhaps an unordered_map implementation.

class Event { //Event to schedule itself.
    
    public:
        
        Event(clock alarm, string name);
        
        ~Event();
        
        clock getAlarm() const;
        
        string getName() const;
        
        void toString() const; //toString to just output in a pretty format.
    
    private:
        
        clock alarm;
        
        string name; //Name of event.

};

class Schedule {
    
    public:
        
        Schedule(string filename);
        
        void pollingLoop(); //Handle interrupts and exit when specified.
        
        //Polling loop also needs to use std::chrono::system_clock::now. If an event is less than now, delete those events.

    private:
        
        void createEvent(clock, string);
        
        void removeEvent(string);
        
        void notify(); //Set off alarm to user that event is triggered.
        
        //Make sure user can dismiss notification after they are notified.

};

//Also build a structured text file for storing the output.


/* WHO’S DOING WHAT? */

Event: Evan

Schedule: John

GUI / Output to console: Tarin


/* OTHER TODOS */

User stories: Someone?
