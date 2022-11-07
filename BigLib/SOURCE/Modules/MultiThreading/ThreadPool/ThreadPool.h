#pragma once
#include "../../../Includes.h"
#include "../ThreadSafeQueue/ThreadSafeQueue.h"


namespace BigLib {
	namespace MultiThreading {

		template <typename Task>
		class ThreadPool {
		public:
			ThreadSafeQueue<Task> TaskList = {};

			ThreadPool(void)
				: P_ConditionVariable()
				, P_Mutex()
				, P_RunningThreads()
				, P_EndAll()
			{}

			~ThreadPool() {
				std::unique_lock<std::mutex> Lock(this->P_Mutex);
				this->P_EndAll = true;


				// Wait For Other Threads To Notify
				while (this->P_RunningThreads > 0)
					P_ConditionVariable.wait(Lock);
			}

			inline const size_t RunningThreads() const {
				return this->P_RunningThreads;
			}

			typedef void(TaskRunner)(Task& TargetTask);
			std::thread InitializeThread(TaskRunner RunnerFunction) {
				std::thread Thread([&, RunnerFunction]() -> void {
					{
						std::unique_lock<std::mutex> Lock(this->P_Mutex);
						this->P_RunningThreads++;
					}
					
					while (!this->P_EndAll) {
						if (this->TaskList.TaskCount() > 0)
							RunnerFunction(this->TaskList.PopTask());
						std::this_thread::sleep_for(std::chrono::microseconds(1));
					}

					{
						std::unique_lock<std::mutex> Lock(this->P_Mutex);
						this->P_RunningThreads--;
						this->P_ConditionVariable.notify_one();
					}
				});

				Thread.detach();
				return Thread;
			}

		private:
			std::condition_variable P_ConditionVariable{};
			mutable std::mutex		P_Mutex{};
			size_t					P_RunningThreads = 0;
			bool					P_EndAll = false;
		};
	}
}
