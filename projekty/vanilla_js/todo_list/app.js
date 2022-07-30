class Model {
    constructor() {
        this.tasks = []
    }

    getTasks() {
        return this.tasks
    }

    addTask(task) {
        this.tasks.push(task)
    }

    removeTask(id) {
        this.tasks = this.tasks.filter(task => task.id !== id)
    }

    toggleTask(id) {
        const task = this.tasks.find(task => task.id === id)
        task.completed = !task.completed
    }

    getTask(id) {
        return this.tasks.find(task => task.id === id)
    }

    nextAvailableId() {
        /* some tasks from the middle might be removed
        so we need to find the highest id */
        if (this.tasks.length === 0) {
            return 1
        }

        const ids = this.tasks.map(task => task.id)
        return Math.max(...ids) + 1
    }
  }
  
  class View {
    constructor() {}

    render(model) {
        const tasks = model.getTasks()
        const tasksList = document.querySelector('.todo-list')

        tasksList.innerHTML = ''
        tasks.forEach(task => {
            const taskElement = document.createElement('li')
            taskElement.innerHTML = `
                <input type="checkbox" ${task.completed ? 'checked' : ''}>
                <span>${task.title}</span>
                <button id="${task.id}" class="remove-task">X</button>
            `
            tasksList.appendChild(taskElement)
        })
        
    }        

  }
  
  class Controller {
    constructor(model, view) {
      this.model = model
      this.view = view
    }

    bindRemoveTaskButtons() {
        const removeTaskButtons = document.querySelectorAll('.remove-task')
        removeTaskButtons.forEach(button => {
            button.addEventListener('click', () => { 
                   const id = parseInt(button.id)                
                   this. removeTask(id)
               }
           )
        }
        )
    }

    renderView() {
        this.view.render(this.model)
        this.bindRemoveTaskButtons()
    }

    addTask(title) {
        const task = {
            id: this.model.nextAvailableId(),
            title: title,
            completed: false
        }

        this.model.addTask(task)
        this.renderView()
    }

    removeTask(id) {
        this.model.removeTask(id)
        this.renderView()
    }

    toggleTask(id) {
        this.model.toggleTask(id)
        this.renderView()
    }

  }

  const app = new Controller(new Model(), new View())
    const todoForm = document.querySelector('.todo-form')
    todoForm.addEventListener('submit', event => {
        event.preventDefault();
        const taskTitle = event.target.querySelector('input[type="text"]').value
        app.addTask(taskTitle)
    }
    );
