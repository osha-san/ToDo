// src/TaskManager.cpp
#include "TaskManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

// Constructor: Load existing tasks from file
TaskManager::TaskManager(const std::string& file) 
    : filename(file), nextId(1) {
    loadFromFile();
}

// Destructor: Save tasks before program ends
TaskManager::~TaskManager() {
    saveToFile();
}

// Generate unique ID for new tasks
int TaskManager::generateId() {
    return nextId++;
}

// Load tasks from CSV file
void TaskManager::loadFromFile() {
    std::ifstream file(filename);
    
    // If file doesn't exist, that's okay (first run)
    if (!file.is_open()) {
        std::cout << "No existing tasks file. Starting fresh.\n";
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Parse CSV: id,description,completed
        std::stringstream ss(line);
        std::string idStr, description, completedStr;
        
        std::getline(ss, idStr, ',');
        std::getline(ss, description, ',');
        std::getline(ss, completedStr, ',');
        
        // Convert strings to proper types
        int id = std::stoi(idStr);
        bool completed = (completedStr == "1");
        
        // Add task to vector
        tasks.push_back(Task(id, description, completed));
        
        // Update nextId to avoid duplicates
        if (id >= nextId) {
            nextId = id + 1;
        }
    }
    
    file.close();
    std::cout << "Loaded " << tasks.size() << " tasks.\n";
}

// Save all tasks to CSV file
void TaskManager::saveToFile() {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not save tasks!\n";
        return;
    }
    
    // Write each task as CSV line
    for (const auto& task : tasks) {
        file << task.toCSV() << "\n";
    }
    
    file.close();
}

// Add a new task
void TaskManager::addTask(const std::string& description) {
    int id = generateId();
    tasks.push_back(Task(id, description, false));
    saveToFile();  // Save immediately
    std::cout << "[/] Task added successfully! (ID: " << id << ")\n";
}

// Delete a task by ID
void TaskManager::deleteTask(int id) {
    // Use algorithm library for efficient removal
    auto it = std::remove_if(tasks.begin(), tasks.end(),
        [id](const Task& task) { return task.getId() == id; });
    
    if (it != tasks.end()) {
        tasks.erase(it, tasks.end());
        saveToFile();
        std::cout << "[/] Task deleted successfully!\n";
    } else {
        std::cout << "[X] Task not found!\n";
    }
}

// Toggle task completion status
void TaskManager::toggleComplete(int id) {
    for (auto& task : tasks) {
        if (task.getId() == id) {
            task.setCompleted(!task.isCompleted());
            saveToFile();
            std::cout << "[/] Task status updated!\n";
            return;
        }
    }
    std::cout << "[X] Task not found!\n";
}

// Edit task description
void TaskManager::editTask(int id, const std::string& newDescription) {
    for (auto& task : tasks) {
        if (task.getId() == id) {
            task.setDescription(newDescription);
            saveToFile();
            std::cout << "[/] Task updated successfully!\n";
            return;
        }
    }
    std::cout << "[X] Task not found!\n";
}

// Display all tasks
void TaskManager::displayTasks() const {
    if (tasks.empty()) {
        std::cout << "\n No tasks yet! Add one to get started.\n\n";
        return;
    }
    
    std::cout << "\n YOUR TASKS:\n";
    std::cout << "================================\n";
    
    for (const auto& task : tasks) {
        std::cout << "[" << task.getId() << "] ";
        std::cout << (task.isCompleted() ? "✓" : "○") << " ";
        std::cout << task.getDescription() << "\n";
    }
    
    std::cout << "================================\n\n";
}

// Get all tasks (for GUI versions)
std::vector<Task> TaskManager::getAllTasks() const {
    return tasks;
}