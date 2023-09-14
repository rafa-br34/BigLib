#pragma once
#include "../../../Includes.h"
#include "../../Data/DataIntegrity/SimpleChecksums.h"
#include "../../Data/BaseMemory.h"

namespace BigLib::Protocols::Network {
	// Code used both for ICMPv4 and ICMPv6
	namespace ICMP {
		template<typename TargetClass = uint8*, typename TypeClass = uint8, typename CodeClass = uint8, typename ChecksumClass = uint16>
		FORCE_INLINE void WriteHeader(TargetClass Target, TypeClass MessageType, CodeClass MessageCode = 0, ChecksumClass Checksum = 0) {
			if (Target == nullptr) return;

			((uint8*)Target)[0] = (uint8)MessageType; // TYPE(8)
			((uint8*)Target)[1] = (uint8)MessageCode; // CODE(8)
			((uint16*)Target)[1] = (uint16)Checksum; // CHECKSUM(16)
		}
	}

	// Implemented accordingly to RFC 792
	// Extensions: RFC 8335
	namespace ICMPv4 {
		enum class ICMPv4Error : uint8 {
			NONE = 0,
			INVALID_CHECKSUM,
			INVALID_PARAMETER,
			INVALID_TYPE_PARAMETER,
		};

		enum class ICMPv4MessageTypes : uint8 {
			/* Codes:
			* 0	Network unreachable
			* 1	Host unreachable
			* 2	Protocol unreachable
			* 3	Port unreachable
			* 4	Fragmentation required, and DF flag set
			* 5	Source route failed
			* 
			* 6	Destination network unknown
			* 7	Destination host unknown
			* 8	Source host isolated
			* 9	Network administratively prohibited
			* 10	Host administratively prohibited
			* 11	Network unreachable for ToS
			* 12	Host unreachable for ToS
			* 13	Communication administratively prohibited
			* 14	Host Precedence Violation
			* 15	Precedence cutoff in effect
			*/
			DESTINATION_UNREACHABLE = 3,

			/* Codes:
			* 0	TTL expired
			* 1	Fragment reassembly time exceeded
			*/
			TIME_EXCEEDED = 11, // TODO

			/* Codes:
			* 0	Pointer indicates the error
			* 1	Missing a required option
			* 2	Bad length
			*/
			PARAMETER_PROBLEM = 12, // TODO

			// DEPRECATED
			SOURCE_QUENCH = 4,

			/* DEPRECATED
			Codes:
			* 0	Redirect for the Network
			* 1	Redirect for the Host
			* 2	Redirect for the ToS & network
			* 3	Redirect for the ToS & host
			*/
			REDIRECT = 5,

			ECHO_REQUEST = 8,
			ECHO_REPLY = 0,

			TIMESTAMP_REQUEST = 13,
			TIMESTAMP_REPLY = 14,

			INFORMATION_REQUEST = 15, // DEPRECATED
			INFORMATION_REPLY = 16, // DEPRECATED

			// EXTENDED MESSAGES

			EXTENDED_ECHO_REQUEST = 42,
			/* Codes:
			* 0	No error
			* 1	Malformed query
			* 2	No such interface
			* 3	No such table entry
			* 4	Multiple interfaces satisfy query
			*/
			EXTENDED_ECHO_REPLY = 43,

		};

		FORCE_INLINE void UpdateChecksum(uint8* Packet, uint16 PacketSize) {
			((uint16*)Packet)[1] = Bitwise::InterpretAsBigEndian(
				(uint16)DataIntegrity::SimpleChecksums::InternetChecksum(Packet, PacketSize, true).Checksum
			);
		}

		bool ValidatePacket(CONST uint8* Packet, uint16 PacketSize) {
			auto Checksum = DataIntegrity::SimpleChecksums::InternetChecksum();

			Checksum.Update(Packet, 2); // TYPE & CODE values
			Checksum.Update<uint16>(0x0000); // CHECKSUM value (always 0x0000 since we're recalculating) (do we even need this?)
			Checksum.Update(Packet + 4, PacketSize - 4); // SEQUENCE, IDENTIFIER, and the MESSAGE

			if (Bitwise::InterpretAsBigEndian((uint16)Checksum.Finalize()) != READ_AS(uint16, Packet)[1])
				return false;

			return true;
		}


		// Assembles a ECHO_REQUEST/ECHO_REPLY packet
		// Requires a buffer with at least 8 bytes
		// MessageSize can be 0 up to 0xFFFF - 28(65507) bytes
		void AssembleEchoPacket(uint16 Identifier, uint16 Sequence, CONST uint8* Message, uint16 MessageSize, uint8* Buffer, ICMPv4MessageTypes Type = ICMPv4MessageTypes::ECHO_REQUEST) {
			if (Buffer == nullptr) return;

			ICMP::WriteHeader(Buffer, Type); // HEADER(32)
			
			((uint16*)Buffer)[2] = Identifier; // IDENTIFIER(16)
			((uint16*)Buffer)[3] = Sequence; // SEQUENCE(16)
			Memory::MemoryCopy(Buffer + 8, Message, MessageSize); // MESSAGE(Up to 0xFFFF - 28(65507) bytes)
			
			UpdateChecksum(Buffer, MessageSize + 8);
		}

		// Assembles a TIMESTAMP_REQUEST/TIMESTAMP_REPLY packet
		// Requires a buffer with at least 20 bytes
		void AssembleTimestampPacket(uint16 Identifier, uint16 Sequence, uint32 OriginateTimestamp, uint32 ReceiveTimestamp, uint32 TransmitTimestamp, uint8* Buffer, ICMPv4MessageTypes Type = ICMPv4MessageTypes::TIMESTAMP_REQUEST) {
			if (Buffer == nullptr) return;

			ICMP::WriteHeader(Buffer, Type); // HEADER(32)
			
			((uint16*)Buffer)[2] = Identifier; // IDENTIFIER(16)
			((uint16*)Buffer)[3] = Sequence; // SEQUENCE(16)
			((uint32*)Buffer)[2] = Bitwise::InterpretAsBigEndian(OriginateTimestamp); // ORIGINATE(32)
			((uint32*)Buffer)[3] = Bitwise::InterpretAsBigEndian(ReceiveTimestamp); // RECEIVE(32)
			((uint32*)Buffer)[4] = Bitwise::InterpretAsBigEndian(TransmitTimestamp); // TRANSMIT(32)

			UpdateChecksum(Buffer, 20);
		}

		// Assembles a INFORMATION_REQUEST/INFORMATION_REPLY packet
		// Requires a buffer with at least 8 bytes
		void AssembleInformationPacket(uint16 Identifier, uint16 Sequence, uint8* Buffer, ICMPv4MessageTypes Type = ICMPv4MessageTypes::INFORMATION_REQUEST) {
			if (Buffer == nullptr) return;

			ICMP::WriteHeader(Buffer, Type); // HEADER(32)

			((uint16*)Buffer)[2] = Identifier; // IDENTIFIER(16)
			((uint16*)Buffer)[3] = Sequence; // SEQUENCE(16)

			UpdateChecksum(Buffer, 8);
		}

		// Assembles a DESTINATION_UNREACHABLE packet
		// Requires a buffer with at least 12 bytes
		void AssembleDestinationUnreachablePacket(uint8 Code, CONST uint8* Message, uint16 MessageSize, uint8* Buffer) {
			if (Buffer == nullptr) return;

			ICMP::WriteHeader(Buffer, ICMPv4MessageTypes::DESTINATION_UNREACHABLE, Code); // HEADER(32)
			
			((uint32*)Buffer)[1] = 0x00000000; // UNUSED
			Memory::MemoryCopy(Buffer + 8, Message, MessageSize);

			UpdateChecksum(Buffer, MessageSize + 8);
		}



		class EchoPacket {
		private:
			Memory::MemoryBuffer p_Buffer;
		public:
			uint16 Identifier = 0; // The identifier value. Usually used to detect packet order.
			uint16 Sequence = 0; // The sequence value. Usually used to mark the sequence.

			CONST uint8* Message = nullptr;
			uint16 MessageSize = 0;

			EchoPacket(uint16 Identifier, uint16 Sequence, CONST uint8* Message = nullptr, uint16 MessageSize = 0)
				: Identifier(Identifier), Sequence(Sequence), Message(Message), MessageSize(MessageSize) {}
			EchoPacket() = default;

			CONST uint8* AssemblePacket(ICMPv4MessageTypes Type = ICMPv4MessageTypes::ECHO_REQUEST) {
				this->p_Buffer.Allocate(8 + this->MessageSize);
				AssembleEchoPacket(this->Identifier, this->Sequence, this->Message, this->MessageSize, this->p_Buffer.Buffer, Type);
				return this->p_Buffer.Buffer;
			}

			ICMPv4Error DisassemblePacket(CONST uint8* Packet, uint16 PacketSize, bool ValidateChecksum = true) {
				if (Packet[0] != (uint8)ICMPv4MessageTypes::ECHO_REPLY && Packet[0] != (uint8)ICMPv4MessageTypes::ECHO_REQUEST)
					return ICMPv4Error::INVALID_TYPE_PARAMETER;

				if (ValidateChecksum && !ValidatePacket(Packet, PacketSize))
					return ICMPv4Error::INVALID_CHECKSUM;

				this->Identifier = READ_AS(uint16, Packet)[2];
				this->Sequence = READ_AS(uint16, Packet)[3];

				if ((this->MessageSize = PacketSize - 8) > 0)
					this->Message = Packet + 8;
				else
					this->Message = nullptr;

				return ICMPv4Error::NONE;
			}

			uint16 Size() CONST { return 8 + this->MessageSize; }

			~EchoPacket() { p_Buffer.Deallocate(); }
		};

		class TimestampPacket {
		private:
			uint8 p_Buffer[20];
		public:
			uint16 Identifier = 0; // The identifier value. Usually used to detect packet order.
			uint16 Sequence = 0; // The sequence value. Usually used to mark the sequence.
			uint32 OriginateTimestamp = 0;
			uint32 ReceiveTimestamp = 0;
			uint32 TransmitTimestamp = 0;

			TimestampPacket(uint16 Identifier, uint16 Sequence, uint32 OriginateTimestamp, uint32 ReceiveTimestamp, uint32 TransmitTimestamp)
				: Identifier(Identifier), Sequence(Sequence), OriginateTimestamp(OriginateTimestamp), ReceiveTimestamp(ReceiveTimestamp), TransmitTimestamp(TransmitTimestamp) {}
			TimestampPacket() = default;

			CONST uint8* AssemblePacket(ICMPv4MessageTypes Type = ICMPv4MessageTypes::TIMESTAMP_REQUEST) {
				AssembleTimestampPacket(this->Identifier, this->Sequence, this->OriginateTimestamp, this->ReceiveTimestamp, this->TransmitTimestamp, this->p_Buffer, Type);
				return this->p_Buffer;
			}

			ICMPv4Error DisassemblePacket(CONST uint8* Packet, bool ValidateChecksum = true) {
				if (Packet[0] != (uint8)ICMPv4MessageTypes::TIMESTAMP_REPLY && Packet[0] != (uint8)ICMPv4MessageTypes::TIMESTAMP_REQUEST)
					return ICMPv4Error::INVALID_TYPE_PARAMETER;

				if (ValidateChecksum && !ValidatePacket(Packet, 20))
					return ICMPv4Error::INVALID_CHECKSUM;

				this->Identifier = READ_AS(uint16, Packet)[2];
				this->Sequence = READ_AS(uint16, Packet)[3];

				this->OriginateTimestamp = Bitwise::InterpretAsBigEndian(READ_AS(uint32, Packet)[2]);
				this->ReceiveTimestamp = Bitwise::InterpretAsBigEndian(READ_AS(uint32, Packet)[3]);
				this->TransmitTimestamp = Bitwise::InterpretAsBigEndian(READ_AS(uint32, Packet)[4]);

				return ICMPv4Error::NONE;
			}

			uint16 Size() CONST { return 20; }
		};

		class InformationPacket {
		private:
			uint8 p_Buffer[8];
		public:
			uint16 Identifier = 0;
			uint16 Sequence = 0;

			InformationPacket(uint16 Identifier, uint16 Sequence)
				: Identifier(Identifier), Sequence(Sequence) {}
			InformationPacket() = default;

			CONST uint8* AssemblePacket(ICMPv4MessageTypes Type = ICMPv4MessageTypes::INFORMATION_REQUEST) {
				AssembleInformationPacket(this->Identifier, this->Sequence, this->p_Buffer, Type);
				return this->p_Buffer;
			}

			ICMPv4Error DisassemblePacket(CONST uint8* Packet, bool ValidateChecksum = true) {
				if (Packet[0] != (uint8)ICMPv4MessageTypes::INFORMATION_REPLY && Packet[0] != (uint8)ICMPv4MessageTypes::INFORMATION_REQUEST)
					return ICMPv4Error::INVALID_TYPE_PARAMETER;

				if (ValidateChecksum && !ValidatePacket(Packet, 8))
					return ICMPv4Error::INVALID_CHECKSUM;

				this->Identifier = READ_AS(uint16, Packet)[2];
				this->Sequence = READ_AS(uint16, Packet)[3];

				return ICMPv4Error::NONE;
			}

			uint16 Size() CONST { return 8; }
		};

		// TODO: Properly test
		class DestinationUnreachablePacket {
		private:
			Memory::MemoryBuffer p_Buffer;
		public:
			uint8 Code = 0;

			CONST uint8* Message = nullptr;
			uint16 MessageSize = 0;

			DestinationUnreachablePacket(uint8 Code, CONST uint8* Message = nullptr, uint16 MessageSize = 0)
				: Code(Code), Message(Message), MessageSize(MessageSize) {}
			DestinationUnreachablePacket() = default;

			CONST uint8* AssemblePacket() {
				this->p_Buffer.Allocate(8 + this->MessageSize);
				AssembleDestinationUnreachablePacket(this->Code, this->Message, this->MessageSize, this->p_Buffer.Buffer);
				return this->p_Buffer.Buffer;
			}

			ICMPv4Error DisassemblePacket(CONST uint8* Packet, uint16 PacketSize, bool ValidateChecksum = true) {
				if (Packet[0] != (uint8)ICMPv4MessageTypes::DESTINATION_UNREACHABLE)
					return ICMPv4Error::INVALID_TYPE_PARAMETER;

				if (ValidateChecksum && !ValidatePacket(Packet, PacketSize))
					return ICMPv4Error::INVALID_CHECKSUM;

				this->Code = READ_AS(uint8, Packet)[1];

				if ((this->MessageSize = PacketSize - 8) > 0)
					this->Message = Packet + 8;
				else
					this->Message = nullptr;

				return ICMPv4Error::NONE;
			}

			uint16 Size() CONST { return 8 + this->MessageSize; }

			~DestinationUnreachablePacket() { p_Buffer.Deallocate(); }
		};
	}

	// Implemented accordingly to RFC 4443
	namespace ICMPv6 {
		enum class ICMPv4Error : uint8 {
			NONE = 0,
			INVALID_CHECKSUM,
			INVALID_PARAMETER,
			INVALID_TYPE_PARAMETER,
		};

		enum class ICMPv6MessageTypes : uint8 {
			/* Codes:
			* 0	No route to destination
			* 1	Communication with destination administratively prohibited
			* 2	Beyond scope of source address
			* 3	Address unreachable
			* 4	Port unreachable
			* 5	Source address failed ingress/egress policy
			* 6	Reject route to destination
			*/
			DESTINATION_UNREACHABLE = 1,

			PACKET_TOO_BIG = 2, // TODO

			/* Codes:
			* 0	TTL expired
			* 1	Fragment reassembly time exceeded
			*/
			TIME_EXCEEDED = 3, // TODO

			/* Codes:
			* 0 Erroneous header field encountered
			* 1 Unrecognized next header type encountered
			* 2 Unrecognized IPv6 option encountered
			*/
			PARAMETER_PROBLEM = 4, // TODO


			ECHO_REQUEST = 128,
			ECHO_REPLY = 129,
		};

		FORCE_INLINE void UpdateChecksum(uint8* Packet, uint16 PacketSize) {
			((uint16*)Packet)[1] = Bitwise::InterpretAsBigEndian(
				(uint16)DataIntegrity::SimpleChecksums::InternetChecksum(Packet, PacketSize, true).Checksum
			);
		}

		bool ValidatePacket(CONST uint8* Packet, uint16 PacketSize) {
			auto Checksum = DataIntegrity::SimpleChecksums::InternetChecksum();

			Checksum.Update(Packet, 2); // TYPE & CODE values
			Checksum.Update<uint16>(0x0000); // CHECKSUM value (always 0x0000 since we're recalculating) (do we even need this?)
			Checksum.Update(Packet + 4, PacketSize - 4); // SEQUENCE, IDENTIFIER, and the MESSAGE

			if (Bitwise::InterpretAsBigEndian((uint16)Checksum.Finalize()) != READ_AS(uint16, Packet)[1])
				return false;

			return true;
		}

		// Assembles a ECHO_REQUEST/ECHO_REPLY packet
		// Requires a buffer with at least 8 bytes
		// MessageSize can be 0 up to 0xFFFF - 28(65507) bytes
		void AssembleEchoPacket(uint16 Identifier, uint16 Sequence, CONST uint8* Message, uint16 MessageSize, uint8* Buffer, bool SetChecksum = false, ICMPv6MessageTypes Type = ICMPv6MessageTypes::ECHO_REQUEST) {
			if (Buffer == nullptr) return;

			ICMP::WriteHeader(Buffer, Type); // HEADER(32)

			((uint16*)Buffer)[2] = Identifier; // IDENTIFIER(16)
			((uint16*)Buffer)[3] = Sequence; // SEQUENCE(16)
			Memory::MemoryCopy(Buffer + 8, Message, MessageSize); // MESSAGE(Up to 0xFFFF - 28(65507) bytes)

			if (SetChecksum)
				UpdateChecksum(Buffer, MessageSize + 8);
		}



		class EchoPacket {
		private:
			Memory::MemoryBuffer p_Buffer;
		public:
			uint16 Identifier = 0; // The identifier value. Usually used to detect packet order.
			uint16 Sequence = 0; // The sequence value. Usually used to mark the sequence.

			CONST uint8* Message = nullptr;
			uint16 MessageSize = 0;

			EchoPacket(uint16 Identifier, uint16 Sequence, CONST uint8* Message = nullptr, uint16 MessageSize = 0)
				: Identifier(Identifier), Sequence(Sequence), Message(Message), MessageSize(MessageSize) {}
			EchoPacket() = default;

			CONST uint8* AssemblePacket(bool SetChecksum = false, ICMPv6MessageTypes Type = ICMPv6MessageTypes::ECHO_REQUEST) {
				this->p_Buffer.Allocate(8 + this->MessageSize);
				AssembleEchoPacket(this->Identifier, this->Sequence, this->Message, this->MessageSize, this->p_Buffer.Buffer, SetChecksum, Type);
				return this->p_Buffer.Buffer;
			}

			ICMPv4Error DisassemblePacket(CONST uint8* Packet, uint16 PacketSize, bool ValidateChecksum = true) {
				if (Packet[0] != (uint8)ICMPv6MessageTypes::ECHO_REPLY && Packet[0] != (uint8)ICMPv6MessageTypes::ECHO_REQUEST)
					return ICMPv4Error::INVALID_TYPE_PARAMETER;

				if (ValidateChecksum && !ValidatePacket(Packet, PacketSize))
					return ICMPv4Error::INVALID_CHECKSUM;

				this->Identifier = READ_AS(uint16, Packet)[2];
				this->Sequence = READ_AS(uint16, Packet)[3];

				if ((this->MessageSize = PacketSize - 8) > 0)
					this->Message = Packet + 8;
				else
					this->Message = nullptr;

				return ICMPv4Error::NONE;
			}

			uint16 Size() CONST { return 8 + this->MessageSize; }

			~EchoPacket() { p_Buffer.Deallocate(); }
		};
	}
}
