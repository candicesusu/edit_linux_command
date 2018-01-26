#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/prinfo.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <asm/errno.h>
#include <linux/list.h>
#include <linux/time.h>

asmlinkage int sys_prinfo(struct prinfo *info)
{
	struct task_struct* proc;
	struct task_struct* child;
	struct task_struct* sib;
	struct timespec youngest;

	if (info == NULL)
		printk("%d\n",-EINVAL);

	if (info != NULL)
	{
		for_each_process(proc)
			if (proc->pid == info->pid)
			{
				info->state = proc->state; // state
				int nice_value = task_nice(proc);
				info->nice = nice_value; // nice

				info->pid = proc->pid; // pid
				info->parent_pid = proc->parent->pid; // parent_pid

				int youngest_children_pid = 0;
				list_for_each_entry(child, &(proc->children), sibling)
				{
					if (youngest.tv_sec <= child->start_time.tv_sec)
					{
						youngest = child->start_time;
						youngest_children_pid = child->pid;
					}
				}
				info->youngest_child_pid = youngest_children_pid; // youngest_children_pid

				struct timespec proc_time = proc->start_time;
				int younger_sib_pid = 0;
				struct timespec youngest_sib;
				list_for_each_entry(sib, &(proc->parent->children), sibling)
				{
					if (youngest_sib.tv_sec < sib->start_time.tv_sec)
					{
						youngest_sib = sib->start_time;
					}
				}
				list_for_each_entry(sib, &(proc->parent->children), sibling)
				{
					if (sib->start_time.tv_sec > proc_time.tv_sec && sib->start_time.tv_sec < youngest_sib.tv_sec)
					{
						youngest_sib = sib->start_time;
						younger_sib_pid = sib->pid;
					}
				}
				info->younger_sibling_pid = younger_sib_pid; // younger_sibling_pid

				int older_sib_pid = 0;
				struct timespec oldest_sib;
				list_for_each_entry(sib, &(proc->parent->children), sibling)
				{
					if (oldest_sib.tv_sec > sib->start_time.tv_sec)
						oldest_sib = sib->start_time;
				}
				list_for_each_entry(sib, &(proc->parent->children), sibling)
				{
					if (sib->start_time.tv_sec < proc_time.tv_sec && sib->start_time.tv_sec > oldest_sib.tv_sec)
					{
						oldest_sib = sib->start_time;
						older_sib_pid = sib->pid;
					}
				}
				info->older_sibling_pid = older_sib_pid; // older_sibling_pid

				info->start_time = proc->start_time.tv_sec; // start_time

				info->user_time = proc->utime; // user_time

				info->sys_time = proc->stime; // sys_time

				long children_usertime_total = 0;
				list_for_each_entry(child, &(proc->children), sibling)
				{
					children_usertime_total += child->utime;
				}

				info->cutime = children_usertime_total; // cutime

				long children_systime_total = 0;
				list_for_each_entry(child, &(proc->children), sibling)
				{
					children_systime_total += child->stime;
				}

				info->cstime = children_systime_total; // cstime

				info->uid = proc->uid; // uid

				strcpy(info->comm, proc->comm); //comm

				printk("state is %ld\n", info->state);
				printk("nice value is %ld\n", info->nice);
				printk("pid is %ld\n", info->pid);
				printk("parent pid is %ld\n", info->parent_pid);
				printk("youngest child pid is %ld\n", info->youngest_child_pid);
				printk("younger sibling pid is %ld\n", info->younger_sibling_pid);
				printk("older sibling pid is %ld\n", info->older_sibling_pid);
				printk("start time is %ld\n", info->start_time);
				printk("user time is %ld\n", info->user_time);
				printk("sys time is %ld\n", info->sys_time);
				printk("cutime is %ld\n", info->cutime);
				printk("cstime is %ld\n", info->cstime);
				printk("user id is %ld\n", info->uid);
				printk("name is %s\n", info->comm);

			}
	}

	return 0;
}

