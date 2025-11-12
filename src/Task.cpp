// src/Task.cpp
#include "Task.h"
#include <sstream>

// Constructor: Initialize a new task
Task::Task(int taskId, const std::string& desc, bool isCompleted)
    : id(taskId), description(desc), completed(isCompleted) {
    // This is called when you create a Task object
    // Uses initialization list (more efficient than assignment)
}

// Getters
int Task::getId() const {
    return id;
}

std::string Task::getDescription() const {
    return description;
}

bool Task::isCompleted() const {
    return completed;
}

// Setters
void Task::setCompleted(bool status) {
    completed = status;
}

void Task::setDescription(const std::string& desc) {
    description = desc;
}

// Convert to CSV format: "id,description,completed"
std::string Task::toCSV() const {
    std::ostringstream oss;
    oss << id << "," << description << "," << (completed ? "1" : "0");
    return oss.str();
}