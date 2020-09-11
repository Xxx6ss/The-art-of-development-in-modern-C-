//
//  main.cpp
//  TeamTasks
//
//  Created by Andrew Kireev on 10.09.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include <iostream>
#include <map>
#include <vector>
#include <tuple>


// Перечислимый тип для статуса задачи
enum class TaskStatus {
  NEW,          // новая
  IN_PROGRESS,  // в разработке
  TESTING,      // на тестировании
  DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = std::map<TaskStatus, int>;



class TeamTasks {
public:
  // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const std::string& person) const {
        return deleopers_tasks_.at(person);
    }
  
  // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const std::string& person) {
        ++deleopers_tasks_[person][TaskStatus::NEW];
    }
  
  // Обновить статусы по данному количеству задач конкретного разработчика,
  // подробности см. ниже
  std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(
                        const std::string& person, int task_count) {
      TasksInfo updates;
      TasksInfo not_updated;
      
      if (deleopers_tasks_[person][TaskStatus::NEW] >= task_count) {
          updates[TaskStatus::IN_PROGRESS] = task_count;
          not_updated[TaskStatus::IN_PROGRESS] = deleopers_tasks_[person][TaskStatus::IN_PROGRESS];
          not_updated[TaskStatus::TESTING] = deleopers_tasks_[person][TaskStatus::TESTING];
          not_updated[TaskStatus::DONE] = deleopers_tasks_[person][TaskStatus::DONE];
          
          deleopers_tasks_[person][TaskStatus::NEW] -= task_count;
          deleopers_tasks_[person][TaskStatus::IN_PROGRESS] += updates[TaskStatus::IN_PROGRESS];
          return std::make_pair(updates, not_updated);
      } else if (deleopers_tasks_[person][TaskStatus::NEW] < task_count){
          
          task_count -= deleopers_tasks_[person][TaskStatus::NEW];
          updates[TaskStatus::IN_PROGRESS] += deleopers_tasks_[person][TaskStatus::NEW];
          deleopers_tasks_[person][TaskStatus::NEW] = 0;
      }
      
      if (deleopers_tasks_[person][TaskStatus::IN_PROGRESS] >= task_count) {
          updates[TaskStatus::TESTING] = task_count;
          not_updated[TaskStatus::TESTING] = deleopers_tasks_[person][TaskStatus::TESTING];
          not_updated[TaskStatus::DONE] = deleopers_tasks_[person][TaskStatus::DONE];
          
          deleopers_tasks_[person][TaskStatus::IN_PROGRESS] -= task_count;
          deleopers_tasks_[person][TaskStatus::TESTING] += updates[TaskStatus::TESTING];
          return std::make_pair(updates, not_updated);
      } else if (deleopers_tasks_[person][TaskStatus::IN_PROGRESS] < task_count){
          
          task_count -= deleopers_tasks_[person][TaskStatus::IN_PROGRESS];
          updates[TaskStatus::TESTING] += deleopers_tasks_[person][TaskStatus::IN_PROGRESS];
          deleopers_tasks_[person][TaskStatus::IN_PROGRESS] = 0;
      }
      if (deleopers_tasks_[person][TaskStatus::TESTING] >= task_count) {
          updates[TaskStatus::DONE] = task_count;
          not_updated[TaskStatus::DONE] = deleopers_tasks_[person][TaskStatus::DONE];
          
          deleopers_tasks_[person][TaskStatus::TESTING] -= task_count;
          deleopers_tasks_[person][TaskStatus::DONE] += updates[TaskStatus::DONE];
          return std::make_pair(updates, not_updated);
      } else if (deleopers_tasks_[person][TaskStatus::TESTING] < task_count){
          
          task_count -= deleopers_tasks_[person][TaskStatus::TESTING];
          updates[TaskStatus::DONE] += deleopers_tasks_[person][TaskStatus::TESTING];
          deleopers_tasks_[person][TaskStatus::TESTING] = 0;
      }
      if (deleopers_tasks_[person][TaskStatus::DONE] >= task_count) {
          not_updated[TaskStatus::DONE] = deleopers_tasks_[person][TaskStatus::DONE];
          
          deleopers_tasks_[person][TaskStatus::DONE] -= task_count;
          return std::make_pair(updates, not_updated);
      } else if (deleopers_tasks_[person][TaskStatus::TESTING] < task_count){
          
          task_count -= deleopers_tasks_[person][TaskStatus::TESTING];
          updates[TaskStatus::DONE] += deleopers_tasks_[person][TaskStatus::TESTING];
          deleopers_tasks_[person][TaskStatus::TESTING] = 0;
      }
      
      deleopers_tasks_[person][TaskStatus::IN_PROGRESS] += updates[TaskStatus::IN_PROGRESS];
      deleopers_tasks_[person][TaskStatus::TESTING] += updates[TaskStatus::TESTING];
      deleopers_tasks_[person][TaskStatus::DONE] += updates[TaskStatus::DONE];
      
      
      return std::make_pair(updates, not_updated);
  }
    
private:
    std::map<std::string, TasksInfo> deleopers_tasks_;
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
  std::cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
      ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
      ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
      ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << std::endl;
}



int main() {
  TeamTasks tasks;
  tasks.AddNewTask("Ilia");
  for (int i = 0; i < 3; ++i) {
    tasks.AddNewTask("Ivan");
  }
  std::cout << "Ilia's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
  std::cout << "Ivan's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));
  
  TasksInfo updated_tasks, untouched_tasks;
  
  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
  std::cout << "Updated Ivan's tasks: ";
  PrintTasksInfo(updated_tasks);
  std::cout << "Untouched Ivan's tasks: ";
  PrintTasksInfo(untouched_tasks);
  
  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
  std::cout << "Updated Ivan's tasks: ";
  PrintTasksInfo(updated_tasks);
  std::cout << "Untouched Ivan's tasks: ";
  PrintTasksInfo(untouched_tasks);

  return 0;
}
