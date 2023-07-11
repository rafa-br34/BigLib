
## CRCEngineStatic
CRCEngineStatic is a class that's able to define any CRC in compile time as long as the parameters are defined.


## Template Parameters

`typename Type, Type Polynomial, bool ReflectIn=false, bool ReflectOut=false, Type XORIn=0, Type XOROut=0, umax Width=SIZEOF_BITS(Type), umax TableLen=256`

####  Type

`Type` defines what type the CRCEngineStatic will use for the CRC, lookup table, mask, and others. This can be any type as long as it supports all math operators(including bitwise operators) and fits `Width` amount of bits.

#### Polynomial
`Polynomial` defines the used polynomial. The polynomial will be converted into a lookup table.

#### ReflectIn, ReflectOut
`ReflectIn`/`ReflectOut` defines whether to cause a binary reflection in the input or the output.

####  XORIn
`XORIn` defines the value that will be XOR-ed with the initial value. This argument also has the role of the "Initial" value when provided initial value is zero(by default).
Please note that the Initial CRC value is also affected by the `ReflectIn`.

#### XOROut
`XOROut` defines the value that will be XOR-ed with the resulting value.

#### Width
`Width` defines the size of the CRC (in bits). This argument is set as `SIZEOF_BITS(Type)` by default.

#### TableLen
`TableLen` defines the size of the table, this parameter should always be 256.

## Variables

### CRC
```c++
Type CRC
```
Current CRC value.


## Methods

### ResetCRC
```c++
CRCEngineStatic& ResetCRC(Type Initial=Type(0))
```
Resets the CRC value to the default values.<br>
 - `Initial` Initial value to XOR with `XORIn`<br>
#### Returns:
A reference to its own class.

---

### GetCRC
```c++
Type GetCRC()
```
Finalizes and returns the CRC value.
#### Returns
The CRC value.

---

### GetLookupTable
```c++
const Type* GetLookupTable()
```
Gets the lookup table pointer.
#### Returns
The lookup table pointer.

---

### UpdateCRC
```c++
CRCEngineStatic& UpdateCRC(Type Data)
CRCEngineStatic& UpdateCRC(const BufferType* Buffer, umax Size)
```
Updates the CRC checksum with `Data` or `Buffer`.<br>
 - `Data` Integer value that will be added to the CRC.<br>
 ---
 - `Buffer` Array in memory with `BufferType` objects.<br>
 - `Size` Amount of `BufferType` objects to read.<br>
#### Returns
A reference to its own class.

