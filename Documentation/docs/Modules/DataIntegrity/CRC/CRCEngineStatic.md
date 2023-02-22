
## CRCEngineStatic
CRCEngineStatic is a class that's able to define any CRC in compile time as long as the parameters are defined.


### Template Parameters
`typename Type, Type Polynomial, bool ReflectIn=false, bool ReflectOut=false, Type XORIn=0, Type XOROut=0, size_t Width=SIZEOF_BITS(Type), size_t TableLen=256`

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
`Width` defines the size of the CRC.
