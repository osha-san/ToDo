// src/Task.h
#ifndef TASK_H
#define TASK_H

#include <string>

class Task {
private:
    int id;                    // Unique identifier for each task
    std::string description;   // What the task is
    bool completed;            // Is it done?

public:
    // Constructor: Creates a new task
    Task(int taskId, const std::string& desc, bool isCompleted = false);
    
    // Getters: Read task properties
    int getId() const;
    std::string getDescription() const;
    bool isCompleted() const;
    
    // Setters: Modify task properties
    void setCompleted(bool status);
    void setDescription(const std::string& desc);
    
    // Utility: Convert task to CSV format
    std::string toCSV() const;
};

#endif