// src/TaskManager.h
#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Task.h"
#include <vector>
#include <string>

class TaskManager {
private:
    std::vector<Task> tasks;   // Dynamic array of tasks
    std::string filename;      // CSV file path
    int nextId;                // Auto-increment for task IDs

    // Helper functions (private = only TaskManager uses them)
    void loadFromFile();
    void saveToFile();
    int generateId();

public:
    // Constructor
    TaskManager(const std::string& file);
    
    // Core operations
    void addTask(const std::string& description);
    void deleteTask(int id);
    void toggleComplete(int id);
    void editTask(int id, const std::string& newDescription);
    
    // Display
    void displayTasks() const;
    std::vector<Task> getAllTasks() const;
    
    // Destructor: Called when TaskManager is destroyed
    ~TaskManager();
};

#endif