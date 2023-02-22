
## About
[CRC(Cyclic Redundancy Check)](https://en.wikipedia.org/wiki/Cyclic_redundancy_check) is an algorithm primarily made for checking data integrity. However, the algorithm is not appropriate for cryptographic uses.

The CRC module in BigLib currently contains 111 predefined CRCs implemented using the [CRCEngineStatic](CRCEngineStatic.md) class.
A list of all predefined CRCs can be found [here](Predefined_CRCs.md).

Here's an example of how to use the CRC module for checking data integrity:
```c++
auto CRC = BigLib::DataIntegrity::CRC::CRC_32_ISO_HDLC(); // Also Known As CRC-32
uint32_t CRCResult = CRC.UpdateCRC("123456789", 9).GetCRC();
if (CRCResult == 0xCBF43926) {
	std::cout << "Data Is Correct\n";
}
else {
	std::cout << "Data Is Not Correct\n";
}
```

