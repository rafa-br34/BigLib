#pragma once
#include "../../../Includes.h"
#include "../../Data/BaseMemory.h"

namespace BigLib {
	namespace Types {
		template<typename Class, const umax t_MaxAllocationDifference = 1048576>
		class List {
		public:
			List(Class* Items, umax Size, bool Copy = true);
			List(Class Item, umax Repeat);
			List(umax InitialSize);
			List() {};

			~List() {
				this->Reallocate(0);
			}

			void PreAllocate(umax Size);
			umax Size();
			void Resize();
			void Move(umax Start, umax Size, umax NewLocation, const bool OverwriteToZero = false);

			void PushFront(Class Item);
			Class PopFront();

			void PushBack(Class Item);
			Class PopBack();

			void Insert(Class Item, umax At);
			void Insert(Class Item, umax Repeat, umax At);
			void Insert(Class* Items, umax ItemsSize, umax At);
			void Remove(umax At, umax Amount = 1);

			Class At(umax Index);


			Class& operator[](umax Index);

			const List<Class>& operator+(Class B);
			const List<Class>& operator+(List<Class> B);

			const List<Class>& operator*(umax B);

		private:
			template<const bool WriteItemsCount=true, const bool IgnoreLowerSize=false>
			INLINE void Reallocate(umax NewSize) {

				if (NewSize == 0) {
					FREE(this->Allocation);
					this->Allocation = nullptr;
					this->AllocationSize = NewSize;

					if CONST_EXPRESSION (WriteItemsCount)
						this->ItemsCount = NewSize;
				}
				else if (this->ItemsCount != NewSize) {

					if CONST_EXPRESSION(WriteItemsCount)
						this->ItemsCount = NewSize;

					CONST_EXPRESSION umax MaxItemsDifference = (t_MaxAllocationDifference / sizeof(Class));

					// Snap To Size Difference
					if (NewSize > this->AllocationSize) {
						this->AllocationSize += MaxItemsDifference;
					}
					else if (NewSize < this->AllocationSize) {

						if (this->AllocationSize - NewSize > MaxItemsDifference) {
							this->AllocationSize -= MaxItemsDifference;
						}
						else if CONST_EXPRESSION (IgnoreLowerSize) {
							this->AllocationSize = NewSize;
						}
						else
							return;
					}
					else
						return;


					Class* OldAllocation = this->Allocation;
					this->Allocation = ALLOCATE(Class, this->AllocationSize);
					Memory::MemoryCopy(this->Allocation, OldAllocation, this->ItemsCount * sizeof(Class));
					FREE(OldAllocation);
				}
			}

			INLINE void Set(Class* Items, umax Size, const bool Copy = true) {
				// Will Delete If It's 0
				this->Reallocate(Size);

				if (!Copy) {
					FREE(this->Allocation);
					this->Allocation = Items;
				}
				else {
					for (umax i = 0; i < Size; i++) {
						this->Allocation[i] = Items[i];
					}
				}
			}
			umax ItemsCount = 0;
			umax AllocationSize = 0;
			Class* Allocation = nullptr;
		};



		template<typename Class, const umax MaxAllocationDifference>
		List<Class, MaxAllocationDifference>::List(Class* Items, umax Size, bool Copy) {
			this->Set(Items, Size, Copy);
		}

		template<typename Class, const umax MaxAllocationDifference>
		List<Class, MaxAllocationDifference>::List(Class Item, umax Repeat) {
			this->Reallocate(Repeat);

			for (umax i = 0; i < Repeat; i++) {
				this->Allocation[i] = Item;
			}
		}

		template<typename Class, const umax MaxAllocationDifference>
		List<Class, MaxAllocationDifference>::List(umax InitialSize) {
			this->Reallocate(InitialSize);
		}


		template<typename Class, const umax MaxAllocationDifference>
		void List<Class, MaxAllocationDifference>::Resize() {
			this->Reallocate(this->ItemsCount);
		}

		template<typename Class, const umax MaxAllocationDifference>
		void List<Class, MaxAllocationDifference>::PreAllocate(umax Size) {
			this->Reallocate<false>(this->ItemsCount + Size);
		}

		template<typename Class, const umax MaxAllocationDifference>
		umax List<Class, MaxAllocationDifference>::Size() {
			return this->ItemsCount;
		}


		template<typename Class, const umax MaxAllocationDifference>
		void List<Class, MaxAllocationDifference>::Move(umax Start, umax Size, umax NewLocation, const bool ZeroGap) {
			if (Start == NewLocation)
				return;
			else {
				this->Reallocate(NewLocation + Size);

				if (Start < NewLocation) {
					// Iterate Backwards
					for (umax i = Size; i > 0; i--) {
						this->Allocation[NewLocation + (i - 1)] = this->Allocation[Start + (i - 1)];

						if (ZeroGap == true)
							this->Allocation[Start + (i - 1)] = 0;
					}
				}
				else if (Start > NewLocation) {
					// Iterate Forward
					for (umax i = 0; i < Size; i++) {
						this->Allocation[NewLocation + i] = this->Allocation[Start + i];

						if (ZeroGap == true)
							this->Allocation[Start + i] = 0;
					}
				}				
			}
		}


		template<typename Class, const umax MaxAllocationDifference>
		void List<Class, MaxAllocationDifference>::PushFront(Class Item) {
			this->Reallocate(++this->ItemsCount);
			this->Allocation[this->ItemsCount - 1] = Item;
		}

		template<typename Class, const umax MaxAllocationDifference>
		Class List<Class, MaxAllocationDifference>::PopFront() {
			Class Item = this->Allocation[this->ItemsCount];
			this->Reallocate(this->ItemsCount--);
			return Item;
		}


		template<typename Class, const umax MaxAllocationDifference>
		void List<Class, MaxAllocationDifference>::PushBack(Class Item) {
			this->Move(0, this->ItemsCount, 1);
			this->Allocation[0] = Item;
		}

		template<typename Class, const umax MaxAllocationDifference>
		Class List<Class, MaxAllocationDifference>::PopBack() {
			Class Item = this->Allocation[0];
			this->Reallocate(this->ItemsCount--);
			return Item;
		}


		template<typename Class, const umax MaxAllocationDifference>
		void List<Class, MaxAllocationDifference>::Insert(Class Item, umax At) {
			this->Move(At, this->ItemsCount - At, At + 1);
			this->Allocation[At] = Item;
		}

		template<typename Class, const umax MaxAllocationDifference>
		void List<Class, MaxAllocationDifference>::Insert(Class Item, umax Repeat, umax At) {
			this->Move(At, this->ItemsCount - At, At + Repeat + 1);
			for (umax i = 0; i < Repeat; i++) {
				this->Allocation[At + i] = Item;
			}
		}

		template<typename Class, const umax MaxAllocationDifference>
		void List<Class, MaxAllocationDifference>::Insert(Class* Items, umax ItemsSize, umax At) {
			this->Move(At, this->ItemsCount - At, At + ItemsSize + 1);
			for (umax i = 0; i < ItemsSize; i++) {
				this->Allocation[At + i] = Items[i];
			}
		}


		template<typename Class, const umax MaxAllocationDifference>
		void List<Class, MaxAllocationDifference>::Remove(umax At, umax Amount) {
			this->Move(At, Amount, At - Amount);
		}


		template<typename Class, const umax MaxAllocationDifference>
		Class List<Class, MaxAllocationDifference>::At(umax Index) {
			if (Index <= this->ItemsCount)
				return this->Allocation[Index];

			throw "Buffer Out Of Bounds";
		}


		template<typename Class, const umax MaxAllocationDifference>
		Class& List<Class, MaxAllocationDifference>::operator[](umax Index) {
			return this->Allocation[Index];
		}


		template<typename Class, const umax MaxAllocationDifference>
		const List<Class>& List<Class, MaxAllocationDifference>::operator+(Class B) {
			this->PushFront(B);
			return *this;
		}

		template<typename Class, const umax MaxAllocationDifference>
		const List<Class>& List<Class, MaxAllocationDifference>::operator+(List<Class> B) {
			this->Insert(B.Allocation, B.ItemsCount, this->ItemsCount);
		}

		template<typename Class, const umax MaxAllocationDifference>
		const List<Class>& List<Class, MaxAllocationDifference>::operator*(umax B) {
			umax Start = this->ItemsCount;
			for (umax i = 0; i < B; i++) {
				this->Move(0, Start, this->ItemsCount);
			}
		}
	}
}
