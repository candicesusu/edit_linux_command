# edit_linux_command

Introduction:
- Download, build, and run a custom Linux kernel.
- Implement a sys-call which returns information about the current process, plus its ancestors (its parent process, it's grandparent process, and so on).

Basic algorithm sketch:
- Start from current process and fill in fields for info_array[0].
- Move to parent of this process (current->parent), and copy its info into info_array[1].
- Repeat until the parent of the process you are working on is itself (cur_task->parent == cur_task).
