#pragma once
#include "../../../Includes.h"

namespace BigLib {
	namespace MultiThreading {
		// A Rewrite From https://stackoverflow.com/questions/15278343/c11-thread-safe-queue
		template <typename Task>
		class ThreadSafeQueue {
		public:
			ThreadSafeQueue(void)
				: P_ConditionVariable()
				, P_Mutex()
				, P_Queue()
			{}

			void PushTask(const Task& NewTask) {
				std::lock_guard<std::mutex> Lock(this->P_Mutex);
				
				// Push New Task Into The P_Queue
				this->P_Queue.push_back(NewTask);

				// Notify That A New Item Has Been Added
				this->P_ConditionVariable.notify_one();
			}


			Task& PopTask(const bool WaitEmpty = true) {
				std::unique_lock<std::mutex> Lock(this->P_Mutex);

				if (WaitEmpty) {
					// Wait For A Task If The Queue Is Empty
					while (this->P_Queue.empty()) {
						this->P_ConditionVariable.wait(Lock);
					}
				}
				

				// Get And Return Task
				Task& ReturnTask = this->P_Queue.back();
				this->P_Queue.pop_back();
				return ReturnTask;
			}


			size_t TaskCount() {
				std::lock_guard<std::mutex> Lock(this->P_Mutex);

				return this->P_Queue.size();
			}

			template <typename Function>
			void SortTasks(Function SortFunction) {
				std::lock_guard<std::mutex> Lock(this->P_Mutex);
				std::sort(this->P_Queue.begin(), this->P_Queue.end(), SortFunction);
			}

		private:
			std::condition_variable P_ConditionVariable{};
			mutable std::mutex		P_Mutex{};
			std::vector<Task>		P_Queue{};
		};
	}
}


