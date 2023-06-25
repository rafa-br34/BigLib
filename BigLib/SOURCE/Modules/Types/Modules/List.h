#pragma once
#include "../../../Includes.h"
#include "../../Data/BaseMemory.h"

namespace BigLib {
	namespace Types {
		template<typename Class, CONST umax t_ItemCountSnap=32>
		class List {
		private:
			template<CONST bool WriteItemsCount = true, CONST bool IgnoreLowerSize = false>
			INLINE void p_Reallocate(umax NewSize) {
				if (NewSize == 0 && this->p_Allocation) {
					FREE(this->p_Allocation);
					this->p_AllocatedItems = NewSize;
					this->p_Allocation = nullptr;
				}
				else if (this->p_ItemsCount != NewSize) {
					// Snap To Size Difference
					umax New = this->ItemCountSnap * ((NewSize / this->ItemCountSnap) + 1);

					if (New != this->p_AllocatedItems) {
						Class* OldAllocation = this->p_Allocation;
						this->p_Allocation = ALLOCATE(Class, this->p_AllocatedItems);

						// Copy Old To New If Needed
						if (!OldAllocation) {
							Memory::MemoryCopy(
								this->p_Allocation,
								OldAllocation,
								this->p_ItemsCount * sizeof(Class)
							);
							FREE(OldAllocation);
						}
					}

					this->p_AllocatedItems = New;
				}

				if CONST_EXPRESSION(WriteItemsCount) this->p_ItemsCount = NewSize;
			}

			INLINE void p_Set(CONST Class* Items, umax Size, const bool Copy = true) {
				// Will Delete If It's 0
				this->p_Reallocate(Size);
				for (umax i = 0; i < Size; i++) {
					this->p_Allocation[i] = Items[i];
				}
			}
			umax p_ItemsCount = 0;
			umax p_AllocatedItems = 0;
			Class* p_Allocation = nullptr;

		public:
			umax ItemCountSnap = t_ItemCountSnap;

			List(CONST Class* Items, umax Size) { this->p_Set(Items, Size, false); }
			List(CONST Class Item, umax Repeat) {
				this->p_Reallocate(Repeat);

				for (umax i = 0; i < Repeat; i++) {
					this->p_Allocation[i] = Item;
				}
			}
			List(umax InitialSize) { this->p_Reallocate(InitialSize); }
			List() = default;

			~List() { this->p_Reallocate(0); }


			void AllocateTotal(umax Size) { this->p_Reallocate(Size); }
			void AllocateMore(umax Size) { this->p_Reallocate(this->p_ItemsCount + Size); }

			Class* AllocationPointer() CONST { return this->p_Allocation; }
			umax ItemCount() CONST { return this->p_ItemsCount; }
			umax AllocatedCount() CONST { return this->p_AllocatedItems; }
			umax MemoryCount() CONST { return this->p_AllocatedItems * sizeof(Class); }
			Class At(umax Index) CONST { return this->p_Allocation[Index]; }
			
			void ResizeToSize() { this->p_Reallocate(this->p_ItemsCount); }

			// Copies 'Start' To 'NewLocation' Using 'Size'
			// When 'NullifyOld' Is Set 'Start' To 'Start' + 'Size' Gets Nullified
			void Move(umax Start, umax Size, umax NewLocation, const bool NullifyOld = false) {
				if (Start == NewLocation)
					return;
				
				this->p_Reallocate(NewLocation + Size);

				if (Start < NewLocation) {
					// Iterate Backwards
					for (umax i = Size; i > 0; i--) {
						this->p_Allocation[NewLocation + (i - 1)] = this->p_Allocation[Start + (i - 1)];

						if (NullifyOld == true) this->p_Allocation[Start + (i - 1)] = 0;
					}
				}
				else if (Start > NewLocation) {
					// Iterate Forward
					for (umax i = 0; i < Size; i++) {
						this->p_Allocation[NewLocation + i] = this->p_Allocation[Start + i];

						if (NullifyOld == true) this->p_Allocation[Start + i] = 0;
					}
				}
				
			}

			void PushFirst(Class Item) {
				this->p_Reallocate(++this->p_ItemsCount);
				this->p_Allocation[this->p_ItemsCount - 1] = Item;
			}
			Class PopFirst() {
				Class Item = this->p_Allocation[this->p_ItemsCount];
				this->p_Reallocate(this->p_ItemsCount--);
				return Item;
			}
			
			void PushLast(Class Item) {
				this->Move(0, this->p_ItemsCount, 1);
				this->p_Allocation[0] = Item;
			}
			Class PopLast() {
				Class Item = this->p_Allocation[0];
				this->p_Reallocate(this->p_ItemsCount--);
				return Item;
			}


			void Insert(Class Item, umax At) {
				this->Move(At, this->p_ItemsCount - At, At + 1);
				this->p_Allocation[At] = Item;
			}
			void Insert(Class Item, umax Repeat, umax At) {
				this->Move(At, this->p_ItemsCount - At, At + Repeat + 1);
				for (umax i = 0; i < Repeat; i++) {
					this->p_Allocation[At + i] = Item;
				}
			}
			void Insert(Class* Items, umax ItemsSize, umax At) {
				this->Move(At, this->p_ItemsCount - At, At + ItemsSize + 1);
				for (umax i = 0; i < ItemsSize; i++) {
					this->p_Allocation[At + i] = Items[i];
				}
			}
			
			// Removes Items At 'At' To 'At + Amount'
			void RemoveForward(umax At, umax Amount = 1) {
				this->Move(At + Amount, this->p_ItemsCount - (At + Amount), At);
			}

			// Removes Items At 'At' To 'At - Amount'
			void RemoveBackward(umax At, umax Amount = 1) {
				this->Move(At, this->p_ItemsCount - (At - Amount), At - Amount);
			}

			Class& operator[](umax Index) { return this->p_Allocation[Index]; };

			const List<Class>& operator+(Class B) {
				this->PushFirst(B);
				return *this;
			}
			const List<Class>& operator+(List<Class> B) {
				this->Insert(B.p_Allocation, B.p_ItemsCount, this->p_ItemsCount);
			}

			const List<Class>& operator*(umax B) {
				umax Start = this->p_ItemsCount;
				for (umax i = 0; i < B; i++) {
					this->Move(0, Start, this->p_ItemsCount);
				}
			}
		};

	}
}
