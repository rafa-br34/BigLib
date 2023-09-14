#pragma once
#include "../../../Includes.h"

namespace BigLib {
	namespace DataIntegrity {
		namespace SimpleChecksums {
			template<typename ChecksumType=uint32, typename SizeType=umax>
			class InternetChecksum {
			private:
				SizeType p_RemainingLength = 0;
				uint8 p_Cache;
			public:
				ChecksumType ClampSize = 0xFFFF;
				ChecksumType Checksum = 0;

				InternetChecksum() = default;

				InternetChecksum(CONST uint8* Data, SizeType DataLength, bool Finalize = true) {
					this->Update(Data, DataLength);
					if (Finalize) this->Finalize();
				}

				InternetChecksum Update(CONST uint8* Data, SizeType DataSize) {
					if (DataSize == 0) { return *this; }
					
					SizeType Size = DataSize;
					SizeType i = 0;

					if (this->p_RemainingLength > 0) {
						this->Checksum += (Data[0] + (this->p_Cache << 8));
						this->p_RemainingLength--; Size--;
						i++;
					}

					while (Size > 1) {
						this->Checksum += (Data[i + 1] + (Data[i + 0] << 8));
						Size -= 2; i += 2;
					}

					if (i < DataSize) {
						this->p_Cache = Data[DataSize - 1];
						this->p_RemainingLength++;
					}

					return *this;
				}

				template<typename Type>
				InternetChecksum Update(CONST Type Value) {
					return this->Update((CONST uint8*)&Value, sizeof(Type));
				}

				InternetChecksum Reset() {
					this->Checksum = 0;
					this->p_RemainingLength = 0;
					return *this;
				}

				ChecksumType Finalize() {
					if (this->p_RemainingLength)
						this->Checksum += this->p_Cache;

					this->Checksum = (this->Checksum >> 16) + (this->Checksum & this->ClampSize);
					this->Checksum += (this->Checksum >> 16);

					this->p_RemainingLength = 0;

					return this->Checksum = (~this->Checksum & this->ClampSize);
				}
			};
		}
	}
}