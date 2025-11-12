// API base URL
const API_URL = 'http://localhost:8081/api';

// DOM elements
const taskInput = document.getElementById('taskInput');
const addBtn = document.getElementById('addBtn');
const taskList = document.getElementById('taskList');
const taskCount = document.getElementById('taskCount');
const filterBtns = document.querySelectorAll('.filter-btn');
const clearCompletedBtn = document.getElementById('clearCompleted');

// State
let tasks = [];
let currentFilter = 'all';

// Initialize app
document.addEventListener('DOMContentLoaded', () => {
    loadTasks();
    setupEventListeners();
});

// Setup event listeners
function setupEventListeners() {
    addBtn.addEventListener('click', addTask);
    taskInput.addEventListener('keypress', (e) => {
        if (e.key === 'Enter') addTask();
    });
    
    filterBtns.forEach(btn => {
        btn.addEventListener('click', () => {
            filterBtns.forEach(b => b.classList.remove('active'));
            btn.classList.add('active');
            currentFilter = btn.dataset.filter;
            renderTasks();
        });
    });
    
    clearCompletedBtn.addEventListener('click', clearCompleted);
}

// Load tasks from backend
async function loadTasks() {
    try {
        const response = await fetch(`${API_URL}/tasks`);
        if (!response.ok) throw new Error('Failed to fetch');
        tasks = await response.json();
        renderTasks();
    } catch (error) {
        console.error('Error loading tasks:', error);
        taskList.innerHTML = '<li style="color: red; text-align: center;">❌ Cannot connect to server. Make sure web_main.exe is running!</li>';
    }
}

// Add new task
async function addTask() {
    const description = taskInput.value.trim();
    
    if (!description) {
        alert('Please enter a task description!');
        return;
    }
    
    try {
        const response = await fetch(`${API_URL}/tasks`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json'},
            body: JSON.stringify({description})
        });

        if(!response.ok) throw new Error('Failed to add');
        
        taskInput.value = '';
        loadTasks(); // Refresh task list
    } catch (error) {
        console.error('Error adding task:', error);
        alert('Failed to add task!');
    }
}

// Toggle task completion
async function toggleTask(id){
    try {
        const response = await fetch(`${API_URL}/tasks/${id}/toggle`, {
            method: 'PUT'
        });

        if (!response.ok) throw new Error('Failed to toggle');
        loadTasks();
    }
    catch(error){
        console.error('Error toggling task:', error);
    }
}

// Delete Task
async function deleteTask(id){
    if(!confirm('Delete this task?')) return;

    try {
        const response = await fetch(`${API_URL}/tasks/${id}`, {
            method: 'DELETE'
        });

        if (!response.ok) throw new Error('Failed to delete');
        loadTasks();
    }
    catch (error){
        console.error('Error deleting task:', error);
    }
}

// Clear all completed tasks
async function clearCompleted(){
    const completedTasks = tasks.filter(t => t.completed);

    if(completedTasks.length === 0){
        alert('No completed tasks to clear!');
        return;
    }
    
    if (!confirm(`Delete ${completedTasks.length} completed task(s)?`)) return;

    try{
        // Delete each completed task
        for(const task of completedTasks){
            await fetch(`${API_URL}/tasks/${task.id}`, {
                method: 'DELETE'
            });
        }
        loadTasks();
    }
    catch (error){
        console.error('Error clearing completed:', error);
    }
}

// Render tasks based on current filter
function renderTasks(){
    // Filter tasks
    let filteredTasks = tasks;
    if(currentFilter === 'active'){
        filteredTasks = tasks.filter(t => !t.completed);
    }
    else if (currentFilter === 'completed') {
        filteredTasks = tasks.filter(t => t.completed);
    }

    // Update task count
    const activeCount = tasks.filter(t => !t.completed).length;
    taskCount.textContent = `${activeCount} task${activeCount !== 1 ? 's' : ''} left`;

    // Render task list
    if(filteredTasks.length === 0){
        taskList.innerHTML = '<li style="text-align: center; color:#999; padding: 40px;">No tasks to show! 🎉</li>';
        return;
    }
    
    taskList.innerHTML = filteredTasks.map(task => `
        <li class="task-item ${task.completed ? 'completed' : ''}">
            <input 
                type="checkbox" 
                class="task-checkbox" 
                ${task.completed ? 'checked' : ''}
                onchange="toggleTask(${task.id})"
            >
            <span class="task-text">${escapeHtml(task.description)}</span>
            <button class="delete-btn" onclick="deleteTask(${task.id})">🗑️</button>
        </li>
    `).join('');
}

// Helper: Escape HTML to prevent XSS attacks
function escapeHtml(text){
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}